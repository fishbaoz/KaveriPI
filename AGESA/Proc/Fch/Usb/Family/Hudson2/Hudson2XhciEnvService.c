/* $NoKeywords:$ */
/**
 * @file
 *
 * Config FCH USB3 controller
 *
 * Init USB3 features.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*;********************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*********************************************************************************/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_USB_FAMILY_HUDSON2_HUDSON2XHCIENVSERVICE_FILECODE

//
// Declaration of local functions
//

/**
 * FchXhciInitIndirectReg - Config XHCI Indirect Registers
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciInitIndirectReg (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT8                 Index;
  UINT32                DrivingStrength;
  UINT32                Port;
  UINT32                Register;
  UINT32                RegValue;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  DrivingStrength = 0;

  //
  // SuperSpeed PHY Configuration (adaptation mode setting)
  //
  RwXhciIndReg ( FCH_XHCI_IND_REG94, 0xFFFFFC00, 0x00000021, StdHeader);
  RwXhciIndReg ( FCH_XHCI_IND_REGD4, 0xFFFFFC00, 0x00000021, StdHeader);
  //
  // SuperSpeed PHY Configuration (CR phase and frequency filter settings)
  //
  RwXhciIndReg ( FCH_XHCI_IND_REG98, 0xFFFFFFC0, 0x0000000A, StdHeader);
  RwXhciIndReg ( FCH_XHCI_IND_REGD8, 0xFFFFFFC0, 0x0000000A, StdHeader);
  //
  // BLM Meaasge
  //
  RwXhciIndReg ( FCH_XHCI_IND_REG00, 0xF8FFFFFF, 0x07000000, StdHeader);
  //
  // xHCI USB 2.0 PHY Settings
  // Step 1 is done by hardware default
  // Step 2
  for (Port = 0; Port < 4; Port ++) {
    DrivingStrength = (UINT32) (LocalCfgPtr->Usb.Xhci20Phy[Port]);
    if (Port < 2) {
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + DrivingStrength, StdHeader);
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    } else {
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + BIT12 + DrivingStrength, StdHeader);
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + DrivingStrength, StdHeader);
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + BIT12 + DrivingStrength, StdHeader);
    }
  }

  // Step 3
  RwXhciIndReg ( FCH_XHCI_IND60_REG0C, ~ ((UINT32) (0x0f << 8)), ((UINT32) (0x02 << 8)), StdHeader);
  RwXhciIndReg ( FCH_XHCI_IND60_REG08, ~ ((UINT32) (0xff << 8)), ((UINT32) (0x0f << 8)), StdHeader);

  // RPR 8.25 Clock Gating in PHY
  for (Port = 0; Port < 4; Port ++) {
    DrivingStrength = 0x1E4;
    if (Port < 2) {
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        FchStall (10, StdHeader);
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        FchStall (10, StdHeader);
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    } else {
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, ((Port - 2) << 13) + BIT12 + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI1_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI1_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        FchStall (10, StdHeader);
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, ((Port - 2) << 13) + DrivingStrength, StdHeader);
      Register = FCH_XHCI_IND60_REG00;
      Index = 0;
      do {
        WritePci ((USB_XHCI1_BUS_DEV_FUN << 16) + 0x48, AccessWidth32, &Register, StdHeader);
        ReadPci ((USB_XHCI1_BUS_DEV_FUN << 16) + 0x4C, AccessWidth32, &RegValue, StdHeader);
        Index++;
        FchStall (10, StdHeader);
      } while ((RegValue & BIT17) && (Index < 10 ));
      RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFE0000, ((Port - 2) << 13) + BIT12 + DrivingStrength, StdHeader);
    }
  }

  if ( ReadFchChipsetRevision ( StdHeader ) >= FCH_BOLTON ) {
   // RRG 8.36 Enhance XHC LS Rise time
    for (Port = 0; Port < 2; Port ++) {
      DrivingStrength = 0x1C4;
      RwXhciIndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
      RwXhciIndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + DrivingStrength, StdHeader);
      RwXhciIndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + DrivingStrength, StdHeader);
    }
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT23), BIT23);

    // RPR 8.61 XHCI ISO device CRC false error detection
    for (Port = 0; Port < 4; Port ++) {
      if (Port < 2) {
        ReadXhci0Phy (Port, (0x7 << 7), &DrivingStrength, StdHeader);
        DrivingStrength &= 0x0000000F8;
        DrivingStrength |= BIT0 + BIT2;
        RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
        RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + (0x7 << 7) + DrivingStrength, StdHeader);
        RwXhci0IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, (Port << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
      } else {
        ReadXhci1Phy ((Port - 2), (0x7 << 7), &DrivingStrength, StdHeader);
        DrivingStrength &= 0x0000000F8;
        DrivingStrength |= BIT0 + BIT2;
        RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
        RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + (0x7 << 7) + DrivingStrength, StdHeader);
        RwXhci1IndReg ( FCH_XHCI_IND60_REG00, 0xFFFFC000, ((Port - 2) << 13) + BIT12 + (0x7 << 7) + DrivingStrength, StdHeader);
      }
    }
    //8.64 L1 Residency Duration
    RwXhciIndReg ( FCH_XHCI_IND60_REG48, 0xFFFFFFFE0, BIT0, StdHeader);
  }
}

/**
 *  FchCombineSigRomInfo - Combine SIG ROM information
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
UINT32
FchCombineSigRomInfo (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINTN                 RomSigStartingAddr;
  UINT32                RomStore[NUM_OF_ROMSIG_FILED];
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;
  UINT32                SelNum;
  UINT32                RomSignatureTag;
  UINT16                XhciFwTag;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  GetRomSigPtr (&RomSigStartingAddr, StdHeader);
  RomSignatureTag = ACPIMMIO32 (RomSigStartingAddr);
  if ( RomSignatureTag == ROMSIG_SIG ) {
    for ( SelNum = 0; SelNum < NUM_OF_ROMSIG_FILED; SelNum++) {
      RomStore[SelNum] = ACPIMMIO32 (RomSigStartingAddr + (SelNum << 2));
    }
  } else {
    RomStore[XHCI_FILED_NUM] = 0;
  }
  if ( LocalCfgPtr->Usb.UserDefineXhciRomAddr != NULL ) {
    RomStore[XHCI_FILED_NUM] = LocalCfgPtr->Usb.UserDefineXhciRomAddr;
  }
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGC8 + 3, AccessWidth8, 0x7F, BIT7, StdHeader);
  for ( SelNum = 1; SelNum < NUM_OF_ROMSIG_FILED; SelNum++) {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGCC, AccessWidth32, ~(UINT32) (BIT2 + BIT1 + BIT0), (BIT2 + SelNum), StdHeader);
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGCC, AccessWidth32, ROMSIG_CFG_MASK, RomStore[SelNum], StdHeader);
  }
  XhciFwTag = (UINT16) ACPIMMIO32 (RomStore[XHCI_FILED_NUM]);
  if ( XhciFwTag != INSTRUCTION_RAM_SIG ) {
    RomStore[XHCI_FILED_NUM] = 0;
  }
  return RomStore[XHCI_FILED_NUM];
}

/**
 * FchXhciInitBeforePciInit - Config XHCI controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciInitBeforePciInit (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
  UINT16                BcdAddress;
  UINT16                BcdSize;
  UINT16                AcdAddress;
  UINT16                AcdSize;
  UINT16                FwAddress;
  UINT16                FwSize;
  UINT32                XhciFwStarting;
  UINT32                SpiValidBase;
  UINT32                RegData;
  UINT16                Index;
  BOOLEAN               Xhci0Enable;
  BOOLEAN               Xhci1Enable;
  FCH_DATA_BLOCK        *LocalCfgPtr;
  AMD_CONFIG_PARAMS     *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *)FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  Xhci0Enable = LocalCfgPtr->Usb.Xhci0Enable;
  Xhci1Enable = LocalCfgPtr->Usb.Xhci1Enable;

  if (( Xhci0Enable == 0 ) && (Xhci1Enable == 0)) {
    return;
  }
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0x00000000, 0x00400700);
  FchStall (20, StdHeader);

  XhciFwStarting = FchCombineSigRomInfo (FchDataPtr);
  if ( XhciFwStarting == 0 ) {
    return;
  }
  //
  // Enable SuperSpeed receive special error case logic. 0x20 bit8
  // Enable USB2.0 RX_Valid Synchronization. 0x20 bit9
  // Enable USB2.0 DIN/SE0 Synchronization. 0x20 bit10
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, 0xFFFFF8FF, 0x00000700);
  //
  // SuperSpeed PHY Configuration (adaptation timer setting)
  // XHC U1 LFPS Exit time
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG90, AccessWidth32, 0xCFF00000, 0x000AAAAA);
  //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG90 + 0x40, AccessWidth32, 0xFFF00000, 0x000AAAAA);

  //
  // Step 1. to enable Xhci IO and Firmware load mode
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT4 + BIT5), 0);                /// Disable Device 22
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT7), BIT7);                    /// Enable 2.0 devices
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0xF0FFFFFC, (Xhci0Enable + (Xhci1Enable << 1)) & 0x03);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, 0xEFFFFFFF, 0x10000000);

  //
  // Step 2. to read a portion of the USB3_APPLICATION_CODE from BIOS ROM area and program certain registers.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA0, AccessWidth32, 0x00000000, (SPI_HEAD_LENGTH << 16));

  BcdAddress = ACPIMMIO16 (XhciFwStarting + BCD_ADDR_OFFSET);
  BcdSize = ACPIMMIO16 (XhciFwStarting + BCD_SIZE_OFFSET);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA4, AccessWidth16, 0x0000, BcdAddress);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA4 + 2, AccessWidth16, 0x0000, BcdSize);

  AcdAddress = ACPIMMIO16 (XhciFwStarting + ACD_ADDR_OFFSET);
  AcdSize = ACPIMMIO16 (XhciFwStarting + ACD_SIZE_OFFSET);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA8, AccessWidth16, 0x0000, AcdAddress);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGA8 + 2, AccessWidth16, 0x0000, AcdSize);

  SpiValidBase = SPI_BASE2 (XhciFwStarting + 4) | SPI_BAR0_VLD | SPI_BASE0 | SPI_BAR1_VLD | SPI_BASE1 | SPI_BAR2_VLD;
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB0, AccessWidth32, 0x00000000, SpiValidBase);
  //
  //   Copy Type0/1/2 data block from ROM image to MMIO starting from 0xC0
  //
  for (Index = 0; Index < SPI_HEAD_LENGTH; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + Index));
  }

  for (Index = 0; Index < BcdSize; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + SPI_HEAD_LENGTH + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + BcdAddress + Index));
  }

  for (Index = 0; Index < AcdSize; Index++) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGC0 + SPI_HEAD_LENGTH + BcdSize + Index, AccessWidth8, 0, ACPIMMIO8 (XhciFwStarting + AcdAddress + Index));
  }

  //
  // Step 3. to enable the instruction RAM preload functionality.
  //
  FwAddress = ACPIMMIO16 (XhciFwStarting + FW_ADDR_OFFSET);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth16, 0x0000, ACPIMMIO16 (XhciFwStarting + FwAddress));
  FwAddress += 2;
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04, AccessWidth16, 0x0000, FwAddress);

  FwSize = ACPIMMIO16 (XhciFwStarting + FW_SIZE_OFFSET);
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG04 + 2, AccessWidth16, 0x0000, FwSize);
  //
  //   Set the starting address offset for Instruction RAM preload.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG08, AccessWidth16, 0x0000, 0);

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, BIT29);

  for (;;) {
    ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00 , AccessWidth32, &RegData);
    if (RegData & BIT30) {
      break;
    }
  }
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) BIT29, 0);

  //
  // Step 4. to release resets in XHCI_ACPI_MMIO_AMD_REG00. wait for USPLL to lock by polling USPLL lock.
  //

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3PLL_RESET, 0);       ///Release U3PLLreset
  for (;;) {
    ReadMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00 , AccessWidth32, &RegData);
    if (RegData & U3PLL_LOCK) {
      break;
    }
  }

  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3PHY_RESET, 0);       ///Release U3PHY
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) U3CORE_RESET, 0);      ///Release core reset

  //
  // SuperSpeed PHY Configuration
  //
  //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG90, AccessWidth32, 0xFFF00000, 0x000AAAAA);
  //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGD0, AccessWidth32, 0xFFF00000, 0x000AAAAA);

  FchXhciInitIndirectReg (LocalCfgPtr);

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT4 + BIT5), 0);                /// Disable Device 22
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF, AccessWidth8, ~(UINT32) (BIT7), BIT7);                    /// Enable 2.0 devices
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) (BIT21), BIT21);
  //
  // Step 5.
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG00, AccessWidth32, ~(UINT32) (BIT17 + BIT18 + BIT19), BIT17 + BIT18);

  //
  // PLUG/UNPLUG of USB 2.0 devices make the XHCI USB 2.0 ports unfunctional - fix enable
  // ACPI_USB3.0_REG 0x20[12:11] = 2'b11
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x3 << 11)), (UINT32) (0x3 << 11));
  //
  // XHC 2 USB2 ports interactional issue - fix enable
  // ACPI_USB3.0_REG 0x20[16] = 1'b1
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x1 << 16)), (UINT32) (0x1 << 16));
  //
  // XHC USB2.0 Ports suspend Enhancement
  // ACPI_USB3.0_REG 0x20[15] = 1'b1
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x1 << 15)), (UINT32) (0x1 << 15));
  //
  // XHC HS/FS IN Data Buffer Underflow issue - fix enable
  // ACPI_USB3.0_REG 0x20[20:18] = 0x7
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~((UINT32) (0x7 << 18)), (UINT32) (0x7 << 18));
  //
  // XHC stuck in U3 after system resuming from S3 -fix enable
  // ACPI_USB3.0_REG 0x98[19] = 1'b1
  //
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG98, AccessWidth32, ~((UINT32) (0x1 << 19)), (UINT32) (0x1 << 19));
  //
  // Change XHC1 ( Dev 16 function 1) Interrupt Pin register to INTB# - Fix enable
  // ACPI_PMIO_F0[18] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 18)), (UINT32) (0x1 << 18));
  //
  // EHCI3/OHCI3 blocks Blink Global Clock Gating when EHCI/OHCI Dev 22 fn 0/2 are disabled
  // ACPI_PMIO_F0[13] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 13)), (UINT32) (0x1 << 13));
  //
  // Access register through JTAG fail when switch from XHCI to EHCI/OHCI - Fix enable
  // ACPI_PMIO_F0[17] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 17)), (UINT32) (0x1 << 17));
  //
  // USB leakage current on differential lines when ports are switched to XHCI - Fix enable
  // ACPI_PMIO_F0[14] =1
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~((UINT32) (0x1 << 14)), (UINT32) (0x1 << 14));

  // RRG 8.26 XHCI Clear pending PME on Sx entry
  RwXhciIndReg ( FCH_XHCI_IND_REG54, ~(UINT32) (BIT15), BIT15, StdHeader);

  //
  // UMI Lane Configuration Information for XHCI Firmware to Calculate the Bandwidth for USB 3.0 ISOC Devices
  //
  if (!(IsUmiOneLaneGen1Mode (StdHeader))) {
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT25 + BIT24), BIT24);
  }
  // RPR 8.23 FS/LS devices not functional after resume from S4 fix enable (SB02699)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT22), BIT22);
  // RPR 8.24 XHC USB2.0 Hub disable issue fix enable (SB02702)
  RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT20), BIT20);
  if ( ReadFchChipsetRevision ( FchDataPtr->StdHeader ) >= FCH_BOLTON ) {
    // RPR 8.27 Enhance D3Cold ccu sequencing
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG10, AccessWidth32, ~(UINT32) (BIT11), BIT11);
    // RPR 8.28 Set HCIVERSION to 1.0
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT15), BIT15);
    // RPR 8.29 xHCI 1.0 Sub-Features Supported
    // Blobk Event Interrupt Flag (BEI)
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT1), BIT1);
    // Force Stopped Event (FSE)
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT2), BIT2);
    // Software LPM
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT3), BIT3);
    // Hardware LPM
    // Skip TRB IOC Event
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT6), BIT6);
    // Remove Secondary Bandwith Domain Reporting
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT7), BIT7);
    // Cold Attach Status
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT8), BIT8);
    // Endpoint Status Update Ordering
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT9), BIT9);
    // Report Event during SKIP on Missed Service Error
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT10), BIT10);
    // Soft Retry
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT11), BIT11);
    // U3 Exit
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT12), BIT12);
    // USB3.0 Link Command
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT13), BIT13);
    // MSE FrameId invalid
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT14), BIT14);
    // Port Test Mode
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT16), BIT16);
    // Miscellaneous Design Improvements
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG40, AccessWidth32, ~(UINT32) (BIT0), BIT0);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG40, AccessWidth32, ~(UINT32) (BIT25), 0);
    // RRG 8.29 End

    // RRG 8.30 XHC USB2 Loopback RX SE0
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG20, AccessWidth32, ~(UINT32) (BIT13 + BIT14 + BIT21), BIT13 + BIT14 + BIT21);
    // RRG 8.31 XHC U2IF_Enabled_Quiettermination off
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT20), BIT20);
    // RRG 8.32 XHC S0 BLM Reset Mode
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF2, AccessWidth8, ~(UINT32) (BIT3), BIT3);
    // RRG 8.33 Enhance XHC Ent_Flag
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT22), BIT22);
    // RRG 8.34 Enhance TRB Pointer when both MSE and SKIP TRB IOC evt opened
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT17), BIT17);
    // RRG 8.35 LPM Broadcast disable
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT18), BIT18);

    // RRG 8.37 Enhance XHC FS/LS connect
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT24), BIT24);
    // RRG 8.38 Enhance XHC ISOCH td_cmp
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT25), BIT25);
    // RRG 8.39 LPM Clock 5us Select
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT8), BIT8);
    // RRG 8.40 Enhance DPP ERR as XactErr
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT9), BIT9);
    // RRG 8.41 Enhance U2IF PME Enable
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT10), BIT10);
    // RRG 8.42 U2IF S3 Disconnect detection
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT12), BIT12);
    // RRG 8.43 Stream Error Handling
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT20 + BIT21 + BIT22), (BIT20 + BIT21 + BIT22));
    // RRG 8.44 FLA Deassert
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT23), BIT23);
    // RRG 8.45 LPM Ctrl Improve
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT27), BIT27);
    // 393674 Enable fix for control transfer error
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) (BIT24), BIT24);

    //8.60 Enhance LPM Host initial L1 Exit
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG30, AccessWidth32, ~(UINT32) BIT29, BIT29);

    // RRG 8.46 Enhance resume after disconnect
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG98, AccessWidth32, ~(UINT32) (BIT30), BIT30);
    // RRG 8.47 Enhance SS HD Detected on Plug-in during S3
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG98, AccessWidth32, ~(UINT32) (BIT31), BIT31);
    // RRG 8.48 Frame Babble Reporting
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REGB4, AccessWidth32, ~(UINT32) (BIT27), BIT27);
    // xHCI Debug Capability
    // RRG 8.49 DCP Halt RSTSM OFF
    // RRG 8.50 Enable DCP DPH check
    // RRG 8.51 DCP LTSSM Inactive to Rxdetect
    // RRG 8.52 Enhance DCP EP State
    // RRG 8.53 DCP Remote Wakeup Capable
    // RRG 8.58 Enable ERDY send once DBC detectes HIT/HOT
    // RRG 8.59 Block HIT/HOT until service interval done
    //RegData = FCH_XHCI_IND_REG100;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0 + BIT1 + BIT2), BIT0 + BIT1 + BIT2);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0 + BIT1 + BIT2), BIT0 + BIT1 + BIT2);
    //ReadMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, &RegData);

    //RegData = FCH_XHCI_IND_REG120;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG120);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3), BIT3);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3), BIT3);
    //ReadMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, &RegData);

    //RegData = FCH_XHCI_IND_REG100;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT21), BIT21);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT21), BIT21);
    //ReadMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, &RegData);

    //RegData = FCH_XHCI_IND_REG128;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG128);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0), BIT0);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT0), BIT0);
    //ReadMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, &RegData);

    //RegData = FCH_XHCI_IND_REG100;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3 + BIT4), BIT3 + BIT4);
    //RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT3 + BIT4), BIT3 + BIT4);
    //ReadMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, &RegData);

    // RRG 8.54 Enhance SS HS detected during S3
    //RegData = FCH_XHCI_IND_REG48;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT1), BIT1);
    // RRG 8.55 Enhance U1 timer (shorten U1 exit response time)
    //RegData = FCH_XHCI_IND_REG48;
    //WriteMem ( ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, &RegData);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT14), BIT14);
    // RRG 8.56 Enhance HW LPM U2Entry state
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT17), BIT17);
    // RRG 8.57 Enhance SSIF PME
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT14), BIT14);
    // RPR 8.62 Enable FW enhancement on XHC clock control when memory power saving is disabled
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG10, AccessWidth32, ~(UINT32) (BIT13), BIT13);
    // RPR 8.63 U2IF Remote Wake Select
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT11), BIT11);
    // RPR 8.64 HS Data Toggle Error Handling
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT16), BIT16);
    // A1
    // UBTS 444589 PIL failures when reset device command hit link td points to invaild trb
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT3), BIT3);
    // UBTS 437021 PME not generated by U2IF logic during S3 shutdown
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT30 + BIT31), (BIT30 + BIT31));
    // UBTS 432864 When a USB3 device is disconnected, and connected again, xHC cannot detect it.
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT16), BIT16);
    // UBTS 430710 USB3 port enters compliance state when debug port enabled (0: enable)
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT27), BIT27);
    // UBTS 430715 [DbC] Compatibility issue with 3rd party (I) chip as Host and AMD as Target
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG100);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT5), BIT5);
    // UBTS 430708  The second configure endpoint command haven't been executed completely when the port was in L1
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT26), BIT26);
    // UBTS 428048 [LPM] LPM 2ports interrupt in is ERROR after host initiate L1exit respond a nak in HS
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT23), 0);
    // UBTS 428045 LPM packet with incorrect address
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT22 + BIT24 + BIT25), 0);
    // UBTS 443139 Buffer overrun during S4 loop
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG48, AccessWidth32, 0x00000000, FCH_XHCI_IND_REG48);
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG4C, AccessWidth32, ~(UINT32) (BIT17), BIT17);
    // UBTS 419582 LPM arbitration between USB ports
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT21), 0);
    // UBTS 429765 USB3.0 ACPI Indirect registers get changed after reboot
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT0), BIT0);
    // UBTS 439658 Black Magic USB 3.0 device when enabled/disabled under windows causes the driver to register disconnect
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG28, AccessWidth32, ~(UINT32) (BIT1 + BIT2), BIT2);
    // UBTS SS RX terminations blocked unexpectedly
    RwMem (ACPI_MMIO_BASE + XHCI_BASE + XHCI_ACPI_MMIO_AMD_REG24, AccessWidth32, ~(UINT32) (BIT15), BIT15);
  }
}

/**
 * FchXhciPowerSavingProgram - Config XHCI for Power Saving mode
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchXhciPowerSavingProgram (
  IN  FCH_DATA_BLOCK     *FchDataPtr
  )
{
}
