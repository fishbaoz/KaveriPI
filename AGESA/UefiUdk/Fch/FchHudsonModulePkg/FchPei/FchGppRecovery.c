/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEIM
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH PEIM
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */

/*****************************************************************************
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
 ******************************************************************************
 */

#define FILECODE UEFI_PEI_FCHPEI_FCHGPPRECOVERY_FILECODE

#include <Library\DebugLib.h>
#include "FchPei.h"
#include "FchGppRecovery.h"
#include "FchPlatform.h"


/**
 * FchGppForceGen2R - Set GPP to Gen2
 *
 *
 * @param[in] FchGpp      Pointer to Fch GPP configuration structure
 * @param[in] ActivePorts Activate Ports
 * @param[in] StdHeader   Pointer to AMD_CONFIG_PARAMS
 *
 */
VOID
FchGppForceGen2R (
  IN       FCH_GPP_R           *FchGpp,
  IN       CONST UINT8         ActivePorts,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32            PortId;

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    if (ActivePorts & (1 << PortId)) {
      RwAlink (FCH_RCINDXP_REGA4 | PortId << 24, 0xFFFFFFFF, BIT29 + BIT0, StdHeader);
      RwAlink ((FCH_ABCFG_REG340 + PortId * 4) | (UINT32) (ABCFG << 29), 0xFFFFFFFF, BIT21, StdHeader);
      RwAlink (FCH_RCINDXP_REGA2 | PortId << 24, ~BIT13, 0, StdHeader);
      RwAlink (FCH_RCINDXP_REGC0 | PortId << 24, ~BIT15, 0, StdHeader);
      RwPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x88), AccessWidth8, 0xf0, 0x02, StdHeader);

      (&FchGpp->PortCfg[PortId])->PortIsGen2 = TRUE;
    }
  }
}

/**
 * FchGppForceGen1R - Set GPP to Gen1
 *
 *
 * @param[in] FchGpp      Pointer to Fch GPP configuration structure
 * @param[in] ActivePorts Activate Ports
 * @param[in] StdHeader   Pointer to AMD_CONFIG_PARAMS
 *
 */
VOID
FchGppForceGen1R (
  IN       FCH_GPP_R           *FchGpp,
  IN       CONST UINT8         ActivePorts,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32            PortId;

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    if (ActivePorts & (1 << PortId) && FchGpp->GppHardwareDownGrade != PortId + 1) {
      RwAlink ((FCH_ABCFG_REG340 + PortId * 4) | (UINT32) (ABCFG << 29), ~BIT21, 0, StdHeader);
      RwAlink (FCH_RCINDXP_REGA4 | PortId << 24, ~BIT0, BIT29, StdHeader);
      RwAlink (FCH_RCINDXP_REGA2 | PortId << 24, 0xFFFFFFFF, BIT13, StdHeader);
      RwAlink (FCH_RCINDXP_REGC0 | PortId << 24, ~BIT15, 0, StdHeader);
      RwPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x88), AccessWidth8, 0xf0, 0x01, StdHeader);

      (&FchGpp->PortCfg[PortId])->PortIsGen2 = FALSE;
    }
  }
}

/**
 * GppPortPollingLtssmR - Loop polling the LTSSM for each GPP
 * port marked in PortMap
 *
 *
 * @param[in] FchGpp        Pointer to Fch GPP configuration structure
 * @param[in] ActivePorts   A bitmap of ports which should be polled
 * @param[in] IsGen2        TRUE if the polling is in Gen2 mode
 * @param[in] StdHeader     Pointer to AMD_CONFIG_PARAMS
 *
 * @retval       FailedPorts     A bitmap of ports which failed to train
 *
 */
UINT8
GppPortPollingLtssmR (
  IN       FCH_GPP_R           *FchGpp,
  IN       UINT8               ActivePorts,
  IN       BOOLEAN             IsGen2,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32                 RetryCounter;
  UINT8                  PortId;
  UINT8                  FailedPorts;
  UINT8                  HotPlugPorts;
  FCH_GPP_PORT_CONFIG_R  *PortCfg;
  UINT32                 AbIndex;
  UINT32                 GppData32;
  UINT8                  EmptyPorts;
  UINT8                  Index;
  UINT8                  FixedPolling;

  FailedPorts = 0;
  HotPlugPorts = 0;
  RetryCounter = MAX_LT_POLLINGS;
  EmptyPorts = ActivePorts;
  FixedPolling = FchGpp->GppPortMinPollingTime;

  while (RetryCounter-- && ActivePorts) {
    for (PortId = 0; PortId < MAX_GPP_PORTS; PortId++) {
      if (ActivePorts & (1 << PortId)) {
        PortCfg = &FchGpp->PortCfg[PortId];
        if ( PortCfg->PortHotPlug == TRUE ) {
          HotPlugPorts |= ( 1 << PortId);
        }
        AbIndex = FCH_RCINDXP_REGA5 | (UINT32) (PortId << 24);
        GppData32 = ReadAlink (AbIndex, StdHeader) & 0x3F3F3F3F;

        if ((UINT8) (GppData32) > 0x04) {
          EmptyPorts &= ~(1 << PortId);
        }

        if ((UINT8) (GppData32) == 0x10) {
          ActivePorts &= ~(1 << PortId);
          PortCfg->PortDetected = TRUE;
          break;
        }

        if (IsGen2) {
          for (Index = 0; Index < 4; Index++) {
            if ((UINT8) (GppData32) == 0x29 || (UINT8) (GppData32) == 0x2A ) {
              ActivePorts &= ~(1 << PortId);
              FailedPorts |= (1 << PortId);
              break;
            }
            GppData32 >>= 8;
          }
        }
      }
    }
    if (EmptyPorts && RetryCounter < (MAX_LT_POLLINGS - (UINT32) FixedPolling)) {
      ActivePorts &= ~EmptyPorts;
    }
    FchStall (1000, StdHeader);
  }
  FchGpp->HotPlugPortsStatus = HotPlugPorts;

  FailedPorts |= ActivePorts;
  return FailedPorts;
}

/**
 * CheckGppLinkStatusR - loop polling the link status for each
 * GPP port
 *
 *
 * Return:     ToggleStatus[3:0] = Port bitmap for those need to clear De-emphasis
 *
 * @param[in] FchGpp      Pointer to Fch GPP configuration
 *       structure
 * @param[in] StdHeader   Pointer to AMD_CONFIG_PARAMS
 *
 */
UINT8
CheckGppLinkStatusR (
  IN       FCH_GPP_R           *FchGpp,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32                 PortId;
  UINT8                  PortScanMap;
  UINT8                  GppHwDowngrade;
  FCH_GPP_PORT_CONFIG_R  *PortCfg;
  UINT8                  FailedPorts;

  PortScanMap = 0;
  FailedPorts = 0;

  //
  // Obtain a list of ports to be checked
  //
  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    PortCfg = &FchGpp->PortCfg[PortId];
    if ( PortCfg->PortPresent == TRUE && PortCfg->PortDetected == FALSE ) {
      PortScanMap |= 1 << PortId;
    }
  }

  GppHwDowngrade = (UINT8) FchGpp->GppHardwareDownGrade;
  if (GppHwDowngrade != 0) {
    //
    // Skip polling and always assume this port to be present
    //
    PortScanMap &= ~(1 << (GppHwDowngrade - 1));
  }

  FchStall (5000, StdHeader);
  if (FchGpp->GppGen2 && FchGpp->GppGen2Strap) {
    FchGppForceGen2R ( FchGpp, PortScanMap, StdHeader);
    FailedPorts = GppPortPollingLtssmR ( FchGpp, PortScanMap, TRUE, StdHeader);

    if (FailedPorts) {
      FchGppForceGen1R ( FchGpp, FailedPorts, StdHeader);
      FailedPorts = GppPortPollingLtssmR ( FchGpp, FailedPorts, FALSE, StdHeader);
    }
  } else {
    FchGppForceGen1R ( FchGpp, PortScanMap, StdHeader);
    FailedPorts = GppPortPollingLtssmR ( FchGpp, PortScanMap, FALSE, StdHeader);
  }
  return FailedPorts;
}



BOOLEAN
FoundInfiniteCrsR (
  IN     FCH_GPP_R           *FchGpp,
  IN     AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32                PortId;
  UINT32                Value32;
  UINT32                RegBusNo;
  UINT32                FchTempBus;
  FCH_GPP_PORT_CONFIG_R *PortCfg;

  FchTempBus = 0x10;
  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    PortCfg = &FchGpp->PortCfg[PortId];
    if ( PortCfg->PortDetected == TRUE ) {
      RegBusNo = (FchTempBus << 16) + (FchTempBus << 8);
      WritePci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x18), AccessWidth32, &RegBusNo, StdHeader);
      ReadPci (PCI_ADDRESS (FchTempBus, 0, 0, 0x08), AccessWidth32, &Value32, StdHeader);
      RegBusNo = 0;
      WritePci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x18), AccessWidth32, &RegBusNo, StdHeader);

      if ( Value32 == 0xFFFFFFFF ) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * PreInitGppLinkR - Enable GPP link training.
 *
 *
 *
 * @param[in] FchGpp     Pointer to Fch GPP configuration
 *       structure
 * @param[in] StdHeader  Pointer to AMD_CONFIG_PARAMS
 *
 */
VOID
PreInitGppLinkR (
  IN       FCH_GPP_R           *FchGpp,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  GPP_LINKMODE          CfgMode;
  UINT8                 PortId;
  UINT32                GppPortCfg;
  UINT16                Tmp16Value;
  UINT8                 GppS3Data;
  UINT8                 HotPlugPorts;

  UINT8               PortMask[5] = {
    0x01,
    0x00,
    0x03,
    0x07,
    0x0F
  };

  HotPlugPorts = 0;
  CfgMode = FchGpp->GppLinkConfig;
  GppPortCfg = (UINT32) PortMask[CfgMode];

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    FchGpp->PortCfg[PortId].PortPresent &= (UINT8 ) (GppPortCfg >> PortId) & BIT0;
    if ( FchGpp->PortCfg[PortId].PortHotPlug == TRUE ) {
      HotPlugPorts |= ( 1 << PortId);
    }
  }

  Tmp16Value = (UINT16) (~GppPortCfg << 12);
  GppPortCfg = (UINT32) (Tmp16Value + (GppPortCfg << 4) + CfgMode);
  WriteAlink (FCH_ABCFG_REGC0 | (UINT32) (ABCFG << 29), GppPortCfg, StdHeader);

  GppPortCfg = ReadAlink (0xC0 | (UINT32) (RCINDXC << 29), StdHeader);
  WriteAlink (0xC0 | (UINT32) (RCINDXC << 29), GppPortCfg | 0x400, StdHeader);      /// Set STRAP_F0_MSI_EN

  RwAlink (FCH_AX_INDXP_REGA0, 0xFFFFFF0F, 0x30, StdHeader);
  RwAlink (FCH_AX_INDXP_REGB1, 0xFFFFFFFF, BIT19, StdHeader);
  RwAlink (FCH_AX_INDXP_REGB1, 0xFFFFFFFF, BIT28, StdHeader);

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    RwAlink (FCH_RCINDXP_REGA0 | PortId << 24, 0xFFFFFF0F, 0x10, StdHeader);
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(BIT1), 0, StdHeader);
  }

  ReadPmio (FCH_PMIOA_REG62, AccessWidth16, &Tmp16Value, StdHeader);
  LibAmdIoRead (AccessWidth16, Tmp16Value, &Tmp16Value, StdHeader);
  if ( ((Tmp16Value >> 10) & 7) == ACPI_SLPTYP_S3) {

    ReadMem ( ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0D, AccessWidth8, &GppS3Data);
    for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
      if ( GppS3Data & (1 << (PortId + 4))) {
        if ( GppS3Data & (1 << PortId)) {
          FchGppForceGen1R ( FchGpp, (1 << PortId), StdHeader);
        } else {
          FchGppForceGen2R ( FchGpp, (1 << PortId), StdHeader);
        }
      }
    }
  }
  FchGpp->GppGen2Strap = (UINT8) (ReadAlink (FCH_RCINDXP_REGA4 | 0 << 24, StdHeader) & BIT0);
  FchGpp->HotPlugPortsStatus = HotPlugPorts;
}

/**
 * FchRecoveryGppResetPcie - Toggle GEVENT4 to assert/deassert
 * GPP device reset
 *
 *
 * @param[in] ResetBlock - PCIE reset for FCH GPP or NB PCIE
 * @param[in] ResetOp    - Assert or deassert PCIE reset
 * @param[in] StdHeader
 *
 */
VOID
FchRecoveryGppResetPcie (
  IN       RESET_BLOCK         ResetBlock,
  IN       RESET_OP            ResetOp,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Or8;
  UINT8    Mask8;

  if (ResetBlock == NbBlock) {
    if (ResetOp == AssertReset) {
      Or8 = BIT4;
      Mask8 = 0;
    } else if (ResetOp == DeassertReset) {
      Or8 = 0;
      Mask8 = BIT4;
    }
    RwMem ((ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
  } else if (ResetBlock == FchBlock) {
    Or8 = BIT1;
    Mask8 = BIT1 + BIT0;
    RwMem ((ACPI_MMIO_BASE + IOMUX_BASE + FCH_GEVENT_REG04), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
    if (ResetOp == AssertReset) {
      Or8 = 0;
      Mask8 = BIT5;
      RwMem ((ACPI_MMIO_BASE + GPIO_BASE + FCH_GEVENT_REG04), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
      Or8 = BIT4;
      Mask8 = 0;
      RwMem ((ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBF), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
    } else if (ResetOp == DeassertReset) {
      Or8 = 0;
      Mask8 = BIT4;
      RwMem ((ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBF), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
      Or8 = BIT5;
      Mask8 = 0;
      RwMem ((ACPI_MMIO_BASE + GPIO_BASE + FCH_GEVENT_REG04), AccessWidth8, (UINT8) &Or8, (UINT8) &Mask8);
    }
  }
}

/**
 * AfterGppLinkInitR
 *       - Search for display device behind each GPP port
 *       - If the port is empty AND not hotplug-capable:
 *           * Turn off link training
 *           * (optional) Power down the port
 *           * Hide the configuration space (Turn off the port)
 *
 * @param[in] FchGpp      Pointer to Fch GPP configuration structure
 * @param[in] StdHeader   Pointer to AMD_CONFIG_PARAMS
 *
 */
VOID
AfterGppLinkInitR (
  IN       FCH_GPP_R           *FchGpp,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32                  PortId;
  FCH_GPP_PORT_CONFIG_R   *PortCfg;
  UINT32                  RegBusNumber;
  UINT32                  FchTempBus;
  UINT32                  AbValue;
  UINT32                  AbIndex;
  UINT8                   Value;

  FchGpp->GppFoundGfxDev = 0;
  AbValue = ReadAlink (FCH_ABCFG_REGC0 | (UINT32) (ABCFG << 29), StdHeader);

  RwAlink (FCH_RCINDXC_REGC1, 0xFFFFFFFF, BIT0, StdHeader);

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    AbIndex = FCH_RCINDXP_REG21 | (UINT32) (PortId << 24);
    WriteAlink (AbIndex, (FCH_GPP_DEV << 3) + PortId, StdHeader);
    RwPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x68), AccessWidth16, ~(BIT10 + BIT11), 0, StdHeader);

    PortCfg = &FchGpp->PortCfg[PortId];
    FchTempBus = 0x10;
    if ( PortCfg->PortDetected == TRUE ) {
      RegBusNumber = (FchTempBus << 16) + (FchTempBus << 8);
      WritePci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x18), AccessWidth32, &RegBusNumber, StdHeader);
      ReadPci (PCI_ADDRESS (FchTempBus, 0, 0, 0x0B), AccessWidth8, &Value, StdHeader);
      if ( Value == 3 ) {
        FchGpp->GppFoundGfxDev |= (1 << PortId);
      }

      RegBusNumber = 0;
      WritePci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x18), AccessWidth32, &RegBusNumber, StdHeader);
    } else if ( PortCfg->PortPresent == FALSE || PortCfg->PortHotPlug == FALSE ) {
      AbValue &= ~(1 << (PortId + 4));
    }

    if ( PortCfg->PortHotPlug == TRUE ) {
      RwAlink ((FCH_RCINDXP_REG10 | (UINT32) (PortId << 24)), 0xFFFFFFFF, BIT3, StdHeader);
      RwPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x5b), AccessWidth8, 0xff, BIT0, StdHeader);
      RwPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x6c), AccessWidth8, 0xff, BIT6, StdHeader);
      RwAlink ((FCH_RCINDXP_REG20 | (UINT32) (PortId << 24)), ~BIT19, 0, StdHeader);
    }
  }

  if ( FchGpp->GppUnhidePorts == FALSE ) {
    if ((AbValue & 0xF0) == 0) {
    } else if ((AbValue & 0xE0) != 0 && (AbValue & 0x10) == 0) {
      AbValue |= BIT4;               // PortA should always be visible whenever other ports are exist
    }
    WriteAlink (FCH_ABCFG_REGC0 | (UINT32) (ABCFG << 29), AbValue, StdHeader);
  }

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    ReadPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x80), AccessWidth8, &Value, StdHeader);
    if (Value != 0xff) {

      Value &= 0xf0;
      Value |= 0x06;
      WritePci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x80), AccessWidth8, &Value, StdHeader);

      AbIndex = FCH_RCINDXP_REG70 | (UINT32) (PortId << 24);
      AbValue = ReadAlink (AbIndex, StdHeader) | BIT19;
      WriteAlink (AbIndex, AbValue, StdHeader);

      AbIndex = FCH_RCINDXP_REG20 | (UINT32) (PortId << 24);
      AbValue = ReadAlink (AbIndex, StdHeader) & ~BIT19;
      WriteAlink (AbIndex, AbValue, StdHeader);

      AbIndex = FCH_RCINDXP_REGA0 | (UINT32) (PortId << 24);
      AbValue = ReadAlink (AbIndex, StdHeader) & ~BIT23;
      if ( FchGpp->GppL1ImmediateAck == 0) {
        AbValue |= BIT23;
      }
      WriteAlink (AbIndex, AbValue, StdHeader);
    }
  }
}

/*********************************************************************************
 * Name: FchGppOnRecovery
 *
 * Description
 *
 *
 * Input
 *   FchPrivate  : FCH PEI private data structure
 *   FchGpp      : Pointer to Fch GPP configuration structure
 *
 * Output
 *
 *
 *********************************************************************************/
EFI_STATUS
FchGppOnRecovery (
  IN      FCH_PEI_PRIVATE      FchPrivate,
  IN      FCH_GPP_R            *FchGpp
  )
{
  AMD_CONFIG_PARAMS     StdHeader;
  UINT32                AbValue;
  UINT8                 ResetCounter;
  UINT8                 FailPorts;

  StdHeader = FchPrivate.StdHdr;

  if (FchGpp->GppFunctionEnable) {
    if ( FchGpp->UmiPhyPllPowerDown == TRUE ) {
      WriteAlink (FCH_AX_INDXC_REG30 | (UINT32) (AXINDC << 29), 0x40, &StdHeader);
      AbValue = ReadAlink (FCH_AX_DATAC_REG34 | (UINT32) (AXINDC << 29), &StdHeader);
      AbValue |= BIT12 + BIT3 + BIT0;
      AbValue &= ~(BIT9 + BIT4);
      WriteAlink (FCH_AX_DATAC_REG34 | (UINT32) (AXINDC << 29), AbValue, &StdHeader);
      RwAlink (FCH_AX_INDXC_REG02 | (UINT32) (AXINDC << 29), ~(BIT8), (BIT8), &StdHeader);
      RwAlink (FCH_AX_INDXC_REG02 | (UINT32) (AXINDC << 29), ~(BIT3), (BIT3), &StdHeader);
    }
    WriteAlink (FCH_AX_INDXP_REG38 | (UINT32) (AXINDP << 29), 0xA4, &StdHeader);
    AbValue = ReadAlink (FCH_AX_DATAP_REG3C | (UINT32) (AXINDP << 29), &StdHeader);
    AbValue |= BIT18;
    WriteAlink (FCH_AX_DATAP_REG3C | (UINT32) (AXINDP << 29), AbValue, &StdHeader);
    AbValue = ReadAlink (FCH_ABCFG_REG31C | (UINT32) (ABCFG << 29), &StdHeader);

    if ( FchGpp->GppLaneReversal == TRUE ) {
      WriteAlink (FCH_ABCFG_REG31C | (UINT32) (ABCFG << 29), AbValue | BIT0, &StdHeader);
    } else {
      WriteAlink (FCH_ABCFG_REG31C | (UINT32) (ABCFG << 29), AbValue | 0x00, &StdHeader);
    }
    AbValue = ReadAlink (FCH_ABCFG_REG90 | (UINT32) (ABCFG << 29), &StdHeader);
    WriteAlink (FCH_ABCFG_REG90 | (UINT32) (ABCFG << 29), AbValue | BIT20, &StdHeader);

    // Toggle GEVENT4 to reset all GPP devices
    FchRecoveryGppResetPcie (FchBlock, AssertReset, &StdHeader);
    FchStall (500, &StdHeader);
    FchRecoveryGppResetPcie (FchBlock, DeassertReset, &StdHeader);
    FchStall (5000, &StdHeader);

    PreInitGppLinkR (FchGpp, &StdHeader);

    if ( FchGpp->GppMemWrImprove == TRUE ) {
      RwAlink (FCH_ABCFG_REG54 | (UINT32) (ABCFG << 29), ~BIT26, (BIT26), &StdHeader);
      RwAlink (FCH_RCINDXC_REG10, ~(BIT12 + BIT11 + BIT10), (BIT12), &StdHeader);
    }

    ResetCounter = 3;
    while (ResetCounter--) {
      FailPorts = CheckGppLinkStatusR (FchGpp, &StdHeader);
      if (FoundInfiniteCrsR (FchGpp, &StdHeader)) {
        FchRecoveryGppResetPcie (FchBlock, AssertReset, &StdHeader);
        FchStall (500, &StdHeader);
        FchRecoveryGppResetPcie (FchBlock, DeassertReset, &StdHeader);
      } else {
        break;
      }
    }
    AfterGppLinkInitR (FchGpp, &StdHeader);
    RwAlink (FCH_ABCFG_REGBC | (UINT32) (ABCFG << 29), ~BIT31, (BIT31), &StdHeader);
  }
  return EFI_SUCCESS;
}
