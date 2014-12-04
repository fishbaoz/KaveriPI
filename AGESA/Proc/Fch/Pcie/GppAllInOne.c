/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch Gpp controller
 *
 * Init Gpp features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
*
* Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
* AMD is granting you permission to use this software (the Materials)
* pursuant to the terms and conditions of your Software License Agreement
* with AMD.  This header does *NOT* give you permission to use the Materials
* or any rights under AMD's intellectual property.  Your use of any portion
* of these Materials shall constitute your acceptance of those terms and
* conditions.  If you do not agree to the terms and conditions of the Software
* License Agreement, please do not use any portion of these Materials.
*
* CONFIDENTIALITY:  The Materials and all other information, identified as
* confidential and provided to you by AMD shall be kept confidential in
* accordance with the terms and conditions of the Software License Agreement.
*
* LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
* PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
* OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
* IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
* (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
* INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
* GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
* RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
* EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
* THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
*
* AMD does not assume any responsibility for any errors which may appear in
* the Materials or any other related information provided to you by AMD, or
* result from use of the Materials or any related information.
*
* You agree that you will not reverse engineer or decompile the Materials.
*
* NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
* further information, software, technical information, know-how, or show-how
* available to you.  Additionally, AMD retains the right to modify the
* Materials at any time, without notice, and is not obligated to provide such
* modified Materials to you.
*
* U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
* "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
* subject to the restrictions as set forth in FAR 52.227-14 and
* DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
* Government constitutes acknowledgement of AMD's proprietary rights in them.
*
* EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
* direct product thereof will be exported directly or indirectly, into any
* country prohibited by the United States Export Administration Act and the
* regulations thereunder, without the required authorization from the U.S.
* government nor will be used for any purpose prohibited by the same.
****************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_PCIE_GPPALLINONE_FILECODE

//
// Declaration of local functions
//


/**
 * FchGppSetAspmA - Set GPP ASPM
 *
 *
 * @param[in] PciAddress PCI Address.
 * @param[in] LxState    Lane State.
 * @param[in] StdHeader
 *
 */
VOID
FchGppSetAspmA (
  IN  UINT32                    PciAddress,
  IN  UINT8                     LxState,
  IN  AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8      PcieCapOffset;
  UINT8      DeviceType;

  PcieCapOffset = FchFindPciCap (PciAddress, PCIE_CAP_ID, StdHeader);

  if (PcieCapOffset) {
    //
    // Read link capabilities register (0x0C[11:10] - ASPM support)
    //
    ReadPci (PciAddress + PcieCapOffset + 0x0D, AccessWidth8, &DeviceType, StdHeader);
    if (DeviceType & BIT2) {
      DeviceType = (DeviceType >> 2) & (BIT1 + BIT0);
      //
      // Set ASPM state in link control register
      //
      RwPci (PciAddress + PcieCapOffset + 0x10, AccessWidth8, 0xffffffff, LxState & DeviceType, StdHeader);
    }
  }
}

/**
 * FchGppSetEpAspmA - Set EP ASPM
 *
 *
 * @param[in] PciAddress PCI Address.
 * @param[in] LxState    Lane State.
 * @param[in] StdHeader
 *
 */
VOID
FchGppSetEpAspmA (
  IN  UINT32                    PciAddress,
  IN  UINT8                     LxState,
  IN  AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8      DeviceType;
  UINT8      MaxFuncs;
  UINT32     DevBDF;

  MaxFuncs = 1;
  ReadPci (PciAddress + 0x0E, AccessWidth8, &DeviceType, StdHeader);

  if (DeviceType & BIT7) {
    MaxFuncs = 8;                                                                   /// multi-function device
  }

  while (MaxFuncs != 0) {
    DevBDF = PciAddress + (UINT32) ((MaxFuncs - 1) << 16);
    FchGppSetAspmA (DevBDF, LxState, StdHeader);
    MaxFuncs--;
  }
}

/**
 * FchGppValidateAspmA - Validate EndPoint support for GPP ASPM
 *
 *
 * @param[in] PciAddress PCI Address.
 * @param[in] LxState    Lane State.
 * @param[in] StdHeader
 *
 */
VOID
FchGppValidateAspmA (
  IN  UINT32                    PciAddress,
  IN  UINT8                     *LxState,
  IN  AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8      PcieCapOffset;
  UINT8      DeviceType;
  UINT8      MaxFuncs;
  UINT32     DevBDF;

  MaxFuncs = 1;
  ReadPci (PciAddress + 0x0E, AccessWidth8, &DeviceType, StdHeader);

  if (DeviceType & BIT7) {
    MaxFuncs = 8;                                                                   /// multi-function device
  }

  while (MaxFuncs != 0) {
    DevBDF = PciAddress + (UINT32) ((MaxFuncs - 1) << 16);
    PcieCapOffset = FchFindPciCap (DevBDF, PCIE_CAP_ID, StdHeader);

    if (PcieCapOffset) {
      //
      // Read link capabilities register (0x0C[11:10] - ASPM support)
      //
      ReadPci (DevBDF + PcieCapOffset + 0x0D, AccessWidth8, &DeviceType, StdHeader);
      if (DeviceType & BIT2) {
        DeviceType = (DeviceType >> 2) & (BIT1 + BIT0);
        //
        // Update ASPM state as what endpoint support
        //
        *LxState &= DeviceType;
      }
    }
    MaxFuncs--;
  }
}

/**
 * FchInitAllinoneGpp - Config Gpp controller
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitAllinoneGpp (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                     PortId;
  UINT8                     BusNum;
  UINT8                     PortAspmValue;
  UINT8                     AllowStrapControlByAB;
  UINT8                     GppS3Data;
  FCH_GPP_PORT_CONFIG       *PortCfg;
  UINT32                    PciAspmValue;
  UINT32                    AbValue;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if ( LocalCfgPtr->Gpp.NewGppAlgorithm == TRUE ) {
    if ((ReadFchSleepType (StdHeader) == ACPI_SLPTYP_S3) && (LocalCfgPtr->Gpp.IsCapsuleMode == FALSE)) {
      FchGppPortInitS3Phase (&LocalCfgPtr->Gpp, StdHeader);
    }
  }
  if ( !LocalCfgPtr->Gpp.NewGppAlgorithm) {
    ProgramFchGppInitReset (&LocalCfgPtr->Gpp, StdHeader);
    FchStall (5000, StdHeader);
  }
  FchGppPortInit (&LocalCfgPtr->Gpp, StdHeader);

  if ( LocalCfgPtr->Gpp.NewGppAlgorithm == TRUE ) {
    FchGppPortInitPhaseII (&LocalCfgPtr->Gpp, StdHeader);
  }

  AbValue = ReadAlink (FCH_ABCFG_REG330 | (UINT32) (ABCFG << 29), StdHeader);
  WriteAlink (FCH_ABCFG_REG330 | (UINT32) (ABCFG << 29), AbValue & ~(BIT26 + BIT10), StdHeader);
  AllowStrapControlByAB = 0x01;
  GppS3Data = 0x00;
  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    //
    // write pci_reg3d with 0x01 to fix yellow mark for GPP bridge under some OS
    // when native PCIE is enabled but MSI is not available
    // BIF/GPP allowing strap STRAP_BIF_INTERRUPT_PIN_SB controlled by AB reg
    //
    PortCfg = &LocalCfgPtr->Gpp.PortCfg[PortId];
    if (PortCfg->PortDetected) {
      GppS3Data |= 1 << (PortId + 4);
      if (PortCfg->PortIsGen2 == FALSE) {
        GppS3Data |= 1 << (PortId);
      }
    }
    if (PortCfg->PortHotPlug) {
      RwPci (PCI_ADDRESS (0, 21, PortId, 0x04), AccessWidth8, 0xFE, 0x00, StdHeader);         ///clear IO enable to fix possible hotplug hang
    }

    WritePci (PCI_ADDRESS (0, 21, PortId, 0x3d), AccessWidth8, &AllowStrapControlByAB, StdHeader);
    ReadPci (PCI_ADDRESS (0, 21, PortId, 0x19), AccessWidth8, &BusNum, StdHeader);

    if (BusNum != 0xFF) {
      ReadPci (PCI_ADDRESS (BusNum, 0, 0, 0x00), AccessWidth32, &PciAspmValue, StdHeader);
      if (PciAspmValue != 0xffffffff) {
        PortAspmValue = LocalCfgPtr->Gpp.GppPortAspm;
        //
        // Validate ASPM support on EP side
        //
        FchGppValidateAspmA (PCI_ADDRESS (BusNum, 0, 0, 0), &PortAspmValue, StdHeader);
        //
        // Set ASPM on EP side
        //
        FchGppSetEpAspmA (PCI_ADDRESS (BusNum, 0, 0, 0), PortAspmValue, StdHeader);
        //
        // Set ASPM on port side
        //
        FchGppSetAspmA (PCI_ADDRESS (0, 21, PortId, 0), PortAspmValue, StdHeader);
      }
    }
    RwAlink ((FCH_RCINDXP_REG02 | (UINT32) (PortId << 24)), ~(UINT32) BIT15, BIT15, StdHeader);
  }
  RwAlink (FCH_RCINDXC_REG02, ~(UINT32) BIT0, BIT0, StdHeader);

  if ( LocalCfgPtr->Gpp.GppPhyPllPowerDown == TRUE ) {
    //
    // Power Saving Feature for GPP Lanes
    //
    // Set PCIE_P_CNTL in Alink PCIEIND space
    //
    AbValue = ReadAlink (FCH_RCINDXC_REG40, StdHeader);
    AbValue |= BIT12 + BIT0;
    AbValue &= ~(BIT9 + BIT4);
    WriteAlink (FCH_RCINDXC_REG40, AbValue, StdHeader);
    RwAlink (FCH_RCINDXC_REG02, ~(UINT32) (BIT8 + BIT3), BIT8 + BIT3, StdHeader);
    GppGen2Workaround (&LocalCfgPtr->Gpp, StdHeader);
  }

  //
  // Configure Lock HWInit registers
  //
  AbValue = ReadAlink (FCH_ABCFG_REGC0 | (UINT32) (ABCFG << 29), StdHeader);
  if (AbValue & 0xF0) {
    AbValue = ReadAlink (FCH_RCINDXC_REG10, StdHeader);
    WriteAlink (FCH_RCINDXC_REG10, AbValue | BIT0, StdHeader);    /// Set HWINIT_WR_LOCK
  }

  //
  // Restore strap0 via override
  //
  if (LocalCfgPtr->Gpp.PcieAer) {
    RwAlink (FCH_ABCFG_REG310 | (UINT32) (ABCFG << 29), 0xFFFFFFFF, BIT7, StdHeader);
    RwAlink (FCH_RCINDXC_REGC0, 0xFFFFFFFF, BIT9, StdHeader);
  }
  RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0D, AccessWidth8, 0, GppS3Data);
}


