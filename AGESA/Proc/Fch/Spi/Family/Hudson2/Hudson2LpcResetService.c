/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch LPC controller
 *
 * Init LPC Controller features.
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

#define FILECODE PROC_FCH_SPI_FAMILY_HUDSON2_HUDSON2LPCRESETSERVICE_FILECODE
#define SPI_BASE UserOptions.FchBldCfg->CfgSpiRomBaseAddress

SPI_CONTROLLER_PROFILE SpiControllerProfile[3] = {
  {64, 66, 66, 66, 66},
  {64, 33, 33, 33, 33},
  {64, 16, 16, 16, 16},
  };
SPI_DEVICE_PROFILE DefaultSpiDeviceTable[] = {
  //JEDEC_ID,RomSize,SecSize;MaxNormal;MaxFast;MaxDual;MaxQuad;QeReadReg;QeWriteReg;QeRegSize;QeLocation;
  {0x001524C2, 2 << 20, 4096,  33, 108, 150, 300, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L1635D
  {0x001525C2, 2 << 20, 4096,  33, 108, 160, 432, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L1635E
  {0x00165EC2, 4 << 20, 4096,  33, 104, 208, 400, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L3235D
  {0x001720C2, 8 << 20, 4096,  33, 104, 208, 400, 0x05, 0x01, 0x1, 0x0040}, //Macronix_MX25L6436E

  {0x0046159D, 4 << 20, 4096,  33, 104, 208, 400, 0x05, 0x01, 0x1, 0x0040}, //PFLASH Pm25LQ032C

  {0x001540EF, 2 << 20, 4096,  33, 104, 208, 416, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q16CV
  {0x001640EF, 4 << 20, 4096,  33, 104, 208, 320, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q32BV
  {0x001740EF, 8 << 20, 4096, 104, 104, 208, 416, 0x35, 0x01, 0x2, 0x0200}, //Wnbond_W25Q64

  {0x001640C8, 4 << 20, 4096,  33, 100, 160, 320, 0x35, 0x01, 0x2, 0x0200}, //GigaDecice GD25Q32BSIGR

  {0x00164037, 4 << 20, 4096,  33, 100, 200, 400, 0x35, 0x01, 0x2, 0x0200}, //AMIC A25LQ32B

  {0x00000000, 4 << 20, 4096,  33,  33,  33,  33, 0x05, 0x01, 0x1, 0x0040}
};


/**
 * FchInitHudson2ResetLpcPciTable - Lpc (Spi) device registers
 * initial during the power on stage.
 *
 *
 *
 *
 */
REG8_MASK FchInitHudson2ResetLpcPciTable[] =
{
  //
  // LPC Device (Bus 0, Dev 20, Func 3)
  //
  {0x00, LPC_BUS_DEV_FUN, 0},

  {FCH_LPC_REG48, 0x00, BIT0 + BIT1 + BIT2},
  {FCH_LPC_REG7C, 0x00, BIT0 + BIT2},
  {FCH_LPC_REG78, 0xF0, BIT2 + BIT3},                       /// Enable LDRQ pin
  //
  // Set 0xBA [6:5] = 11 improve SPI timing margin. (SPI Prefetch enhancement)
  //
  {FCH_LPC_REGBA, 0x9F, BIT5 + BIT6},
  // Force EC_PortActive to 1 to fix possible IR non function issue when NO_EC_SUPPORT is defined
  {FCH_LPC_REGA4, 0xFE, BIT0},
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitResetLpcProgram - Config Lpc controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetLpcProgram (
  IN       VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  //
  // enable prefetch on Host, set LPC cfg 0xBB bit 0 to 1
  //
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBA, AccessWidth16, 0xFFFF, BIT8, StdHeader);

  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG6C, AccessWidth32, 0xFFFFFF00, 0, StdHeader);

  ProgramPciByteTable ( (REG8_MASK*) (&FchInitHudson2ResetLpcPciTable[0]), sizeof (FchInitHudson2ResetLpcPciTable) / sizeof (REG8_MASK), StdHeader);

  //
  // Enabling ClkRun Function
  //
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBB, AccessWidth8, 0xC3, BIT2, StdHeader);
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0, AccessWidth8, 0xFB, 0, StdHeader);
  //
  // LPC CLK0 Power-Down Function
  //
  if (!IsImcEnabled (StdHeader)) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD2, AccessWidth8, 0xFF, BIT3);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD2, AccessWidth8, ~(UINT32) (BIT3), 0);
  }
  if ( LocalCfgPtr->LegacyFree ) {
    RwPci (((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG44), AccessWidth32, 00, 0x0003C000, StdHeader);
  } else {
    RwPci (((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG44), AccessWidth32, 00, 0xFF03FFD5, StdHeader);
  }
}

/**
 * FchSpiControllerNotBusy - SPI Conroller Not Busy
 *
 *
 *
 *
 */
VOID
FchSpiControllerNotBusy (
  )
{
  UINT32 SpiReg00;
  SpiReg00 = FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE;
  do {
    SpiReg00 = ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00);
    ReadIo8 ((UINT16) (0x80));
  } while ((SpiReg00 & (FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE)));
}
/**
 * FchSpiExecute - SPI Execute
 *
 *
 *
 *
 */
VOID
FchSpiExecute (
  )
{
  UINT32 SpiReg00;
  SpiReg00 = FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE;
  ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
  ReadIo8 ((UINT16) (0x80));
  do {
    SpiReg00 = ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00);
    ReadIo8 ((UINT16) (0x80));
  } while ((SpiReg00 & (FCH_SPI_BUSY + FCH_SPI_EXEC_OPCODE)));
}
/**
 * FchResetFifo - Reset SPI FIFO
 *
 *
 *
 *
 */
VOID
FchResetFifo (
  )
{
  ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= BIT20;
  ReadIo8 ((UINT16) (0x80));
}
/**
 * WaitForSpiDeviceWriteEnabled -
 *
 *
 *
 *
 */
BOOLEAN
WaitForSpiDeviceWriteEnabled (
  )
{
  UINT8 bStatus;
  bStatus = 0;
  do
  {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      &bStatus,//IN       OUT UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } while ((bStatus & 2) == 0);
  return TRUE;
}
/**
 * WaitForSpiDeviceComplete -
 *
 *
 *
 *
 */
BOOLEAN
WaitForSpiDeviceComplete (
  )
{
  UINT8 bStatus;
  bStatus = 1;
  do
  {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      &bStatus,//IN       OUT UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } while (bStatus & 1);
  return TRUE;
}
/**
 * FchSpiTransfer - FCH Spi Transfer
 *
 *
 *
 * @param[in] PrefixCode   - Prefix code.
 * @param[in] Opcode       - Opcode.
 * @param[in] DataPtr      - Data Pointer.
 * @param[in] AddressPtr   - Address Pointer.
 * @param[in] Length       - Read/Write Length.
 * @param[in] WriteFlag    - Write Flag.
 * @param[in] AddressFlag  - Address Flag.
 * @param[in] DataFlag     - Data Flag.
 * @param[in] FinishedFlag - Finished Flag.
 *
 */
//static
AGESA_STATUS
FchSpiTransfer (
  IN       UINT8    PrefixCode,
  IN       UINT8    Opcode,
  IN OUT   UINT8    *DataPtr,
  IN       UINT8    *AddressPtr,
  IN       UINT8    Length,
  IN       BOOLEAN  WriteFlag,
  IN       BOOLEAN  AddressFlag,
  IN       BOOLEAN  DataFlag,
  IN       BOOLEAN  FinishedFlag
  )
{
  UINTN  Addr;
  UINTN  Retry;
  UINTN  i;
  UINT8  WriteCount;
  UINT8  ReadCount;
  UINT8  Dummy;
  UINT8  CurrFifoIndex;

  if (!((Opcode == 0x9f) && (!DataFlag))) {
    if (PrefixCode) {
      Retry = 0;
      do {
        ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 0) = PrefixCode;
        ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 1) = 0;
        ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
        FchSpiControllerNotBusy ();

        if (FinishedFlag) {
          if (WaitForSpiDeviceWriteEnabled ()) {
            Retry = 0;
          } else {
            Retry ++;
            if (Retry >= FCH_SPI_RETRY_TIMES) {
              return AGESA_ERROR;
            }
          }
        }
      } while (Retry);
    }
    Retry = 0;
    do {
      WriteCount = 0;
      ReadCount = 0;
      //
      // Reset Fifo Ptr
      //
      FchResetFifo ();
      //
      // Check Address Mode
      //
      Addr = (UINTN) AddressPtr;
      if (AddressFlag) {
        for (i = 0; i < 3; i ++) {
          ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0) = (UINT8) ((Addr >> (16 - i * 8)) & 0xff);
        }
        WriteCount += 3;
      }
      if (DataFlag) {
        //
        // Check Read/Write Mode
        //
        if (WriteFlag) {
          WriteCount += Length + 1;
          for (i = 0; i < (UINTN) (Length + 1); i ++) {
            ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0) = DataPtr[i];
            ReadIo8 ((UINT16) (0x80));
          }
        } else {
          //
          // Read operation must plus extra 1 byte
          //
          ReadCount += Length + 2;
        }
      }
      ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 0) = Opcode;
      ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG00 + 1) = (ReadCount << 4) + WriteCount;
      ACPIMMIO32 (SPI_BASE + FCH_SPI_MMIO_REG00) |= FCH_SPI_EXEC_OPCODE;
      FchSpiControllerNotBusy ();

      if (FinishedFlag) {
        if (WaitForSpiDeviceComplete ()) {
          Retry = 0;
        } else {
          Retry ++;
          if (Retry >= FCH_SPI_RETRY_TIMES) {
            return AGESA_ERROR;
          }
        }
      }
    } while (Retry);
    if (DataFlag && ReadCount) {
      //
      // Reset Fifo Ptr
      //
      FchResetFifo ();
      while (DataFlag && ReadCount) {
        CurrFifoIndex = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 1) & 0x07;
        if (CurrFifoIndex != WriteCount) {
          Dummy = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0);
          ReadIo8 ((UINT16) (0x80));
        } else break;
      }
      for (i = 0; i < (UINTN) (Length + 1); i ++) {
        DataPtr[i] = ACPIMMIO8 (SPI_BASE + FCH_SPI_MMIO_REG0C + 0);
        ReadIo8 ((UINT16) (0x80));
      }
    }
  }
  return AGESA_SUCCESS;
}

/**
 * FchSetQualMode - Set SPI Qual Mode
 *
 *
 *
 * @param[in] SpiQualMode- Spi Qual Mode.
 * @param[in] StdHeader  - Standard Header.
 *
 */
VOID
FchSetQualMode (
  IN       UINT32      SpiQualMode,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{

  RwMem (ACPI_MMIO_BASE + GPIO_BASE + FCH_GEVENT_REG09, AccessWidth8, ~(UINT32) BIT3, BIT3);
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBB, AccessWidth8, ~(UINT32) BIT0, BIT0, StdHeader);
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32) ( BIT18 + BIT29 + BIT30), ((SpiQualMode & 1) << 18) + ((SpiQualMode & 6) << 28));

}

/**
 * FchInitResetSpi - Config Spi controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetSpi (
  IN       VOID     *FchDataPtr
  )
{
  UINT32                    SpiModeByte;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Set Spi ROM Base Address
  //
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGA0, AccessWidth32, 0x001F, SPI_BASE, StdHeader);

  RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, 0xFFFFFFFF, (BIT19 + BIT24 + BIT25 + BIT26));
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, 0xFFC0FFFF, 0 );



  //
  //  Spi Mode Initial
  //
  if (LocalCfgPtr->SpiSpeed) {
    RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, ~(UINT32) (BIT13 + BIT12), ((LocalCfgPtr->SpiSpeed - 1 ) << 12));
  }

  if (LocalCfgPtr->FastSpeed) {
    RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, ~(UINT32) (BIT15 + BIT14), ((LocalCfgPtr->FastSpeed - 1 ) << 14));
  }

  RwMem (SPI_BASE + FCH_SPI_MMIO_REG1C, AccessWidth32, ~(UINT32) (BIT10), ((LocalCfgPtr->BurstWrite) << 10));

  SpiModeByte = LocalCfgPtr->Mode;
  if (LocalCfgPtr->Mode) {
    if ((SpiModeByte == FCH_SPI_MODE_QUAL_114) || (SpiModeByte == FCH_SPI_MODE_QUAL_144) || (SpiModeByte == FCH_SPI_MODE_QUAL_112) || (SpiModeByte == FCH_SPI_MODE_QUAL_122)) {
      if (FchPlatformSpiQe (FchDataPtr)) {
        FchSetQualMode (SpiModeByte, StdHeader);
      }
    } else {
      RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32) ( BIT18 + BIT29 + BIT30), ((LocalCfgPtr->Mode & 1) << 18) + ((LocalCfgPtr->Mode & 6) << 28));
    }
  } else {
    if (FchPlatformSpiQe (FchDataPtr)) {
      //RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, ~(UINT32) (BIT13 + BIT12), ((FCH_SPI_SPEED_33M - 1 ) << 12));
      //RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, ~(UINT32) (BIT15 + BIT14), ((FCH_SPI_SPEED_66M - 1 ) << 14));
      SpiModeByte = FCH_SPI_MODE_QUAL_144;
      //FchSetQualMode (SpiModeByte, StdHeader);
    }
  }
  // Enabling SPI ROM Prefetch
  // Set LPC cfg 0xBA bit 8
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBA, AccessWidth16, 0xFFFF, BIT8, StdHeader);

  // Enable SPI Prefetch for USB, set LPC cfg 0xBA bit 7 to 1.
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBA, AccessWidth16, 0xFFFF, BIT7, StdHeader);
}



UINT32
FchReadSpiId (
  IN       BOOLEAN    Flag
  )
{
  UINT32 DeviceID;
  UINT8 *DeviceIdPtr;
  DeviceID = 0;
  DeviceIdPtr = (UINT8 *) (((UINTN) (&DeviceID)));
  if (Flag) {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x9F,//IN       UINT8    Opcode,
      DeviceIdPtr,//IN  OUT   UINT8    *DataPtr,
      (UINT8 *) (NULL),//IN       UINT8    *AddressPtr,
      2,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  } else {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x9F,//IN       UINT8    Opcode,
      DeviceIdPtr,//IN  OUT   UINT8    *DataPtr,
      (UINT8 *) (NULL),//IN       UINT8    *AddressPtr,
      2,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      FALSE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
  }
  return DeviceID;
}
/**
 * FchReadSpiQe - Read SPI Qual Enable
 *
 *
 *
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 * @param[in] SpiQeRegValue   - Spi QuadEnable Register Value
 *
 */
BOOLEAN
FchReadSpiQe (
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr,
  IN       UINT16     SpiQeRegValue
  )
{
  UINT16     Value16;
  SpiQeRegValue = 0;
  Value16 = 0;

  FchSpiTransfer (
    0, //IN       UINT8    PrefixCode,
    SpiDeviceProfilePtr->QeReadRegister,//IN       UINT8    Opcode,
    (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
    NULL,//IN       UINT8    *AddressPtr,
    0,//IN       UINT8    Length,
    FALSE,//IN       BOOLEAN  WriteFlag,
    FALSE,//IN       BOOLEAN  AddressFlag,
    TRUE,//IN       BOOLEAN  DataFlag,
    FALSE //IN       BOOLEAN  FinishedFlag
  );
  if (SpiDeviceProfilePtr->QeOperateSize == 2) {
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0x05,//IN       UINT8    Opcode,
      (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      SpiDeviceProfilePtr->QeReadRegister,//IN       UINT8    Opcode,
      (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      0,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    SpiQeRegValue |= (Value16 << 8);
  }

  if (SpiDeviceProfilePtr->QeLocation & SpiQeRegValue) {
    return TRUE;
  }
  SpiQeRegValue |= SpiDeviceProfilePtr->QeLocation;
  return FALSE;
}
/**
 * FchWriteSpiQe - Write SPI Qual Enable
 *
 *
 *
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 * @param[in] SpiQeRegValue   - Spi QuadEnable Register Value
 *
 */
VOID
FchWriteSpiQe (
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr,
  IN       UINT16     SpiQeRegValue
  )
{

  SpiQeRegValue |= SpiDeviceProfilePtr->QeLocation;
  FchSpiTransfer (
    0x06, //IN       UINT8    PrefixCode,
    SpiDeviceProfilePtr->QeWriteRegister,//IN       UINT8    Opcode,
    (UINT8 *)(&SpiQeRegValue),//IN  OUT   UINT8    *DataPtr,
    NULL,//IN       UINT8    *AddressPtr,
    SpiDeviceProfilePtr->QeOperateSize - 1,//IN       UINT8    Length,
    TRUE,//IN       BOOLEAN  WriteFlag,
    FALSE,//IN       BOOLEAN  AddressFlag,
    TRUE,//IN       BOOLEAN  DataFlag,
    TRUE //IN       BOOLEAN  FinishedFlag
  );
}

/**
 * FchFindSpiDeviceProfile - Find SPI Device Profile
 *
 *
 *
 * @param[in] DeviceID   - Device ID
 * @param[in] SpiDeviceProfilePtr - Spi Device Profile Pointer
 *
 */
BOOLEAN
FchFindSpiDeviceProfile (
  IN       UINT32     DeviceID,
  IN OUT   SPI_DEVICE_PROFILE    *SpiDeviceProfilePtr
  )
{
  SPI_DEVICE_PROFILE   *CurrentSpiDeviceProfilePtr;
  UINT16     SpiQeRegValue;
  SpiQeRegValue = 0;
  CurrentSpiDeviceProfilePtr = SpiDeviceProfilePtr;
  do {
    if (CurrentSpiDeviceProfilePtr->JEDEC_ID == DeviceID) {
      SpiDeviceProfilePtr = CurrentSpiDeviceProfilePtr;
      if (!(FchReadSpiQe (SpiDeviceProfilePtr, SpiQeRegValue))) {
        FchWriteSpiQe (SpiDeviceProfilePtr, SpiQeRegValue);
        if (!(FchReadSpiQe (SpiDeviceProfilePtr, SpiQeRegValue))) {
          return FALSE;
        }
      }
      return TRUE;
    }
    CurrentSpiDeviceProfilePtr++;
  } while (CurrentSpiDeviceProfilePtr->JEDEC_ID != NULL);
  return FALSE;
}

/**
 * FchConfigureSpiDeviceDummyCycle - Configure Spi Device Dummy
 * Cycle
 *
 *
 *
 * @param[in] DeviceID   - Device ID
 * @param[in] FchDataPtr  - FchData Pointer.
 *
 */
BOOLEAN
FchConfigureSpiDeviceDummyCycle (
  IN       UINT32     DeviceID,
  IN OUT   FCH_RESET_DATA_BLOCK    *FchDataPtr
  )
{
  UINT16     Mode16;
  UINT16     Value16;
  UINT16     DummyValue16;
  UINT16     CurrentDummyValue16;
  UINT16     CurrentMode16;
  AMD_CONFIG_PARAMS         *StdHeader;

  StdHeader = FchDataPtr->StdHeader;
  Value16 = 0;
  DummyValue16 = 8;

  switch (DeviceID) {
  case 0x17BA20:

    FchSpiTransfer (
      0, //IN       UINT8    PrefixCode,
      0xB5,//IN       UINT8    Opcode,
      (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
      NULL,//IN       UINT8    *AddressPtr,
      1,//IN       UINT8    Length,
      FALSE,//IN       BOOLEAN  WriteFlag,
      FALSE,//IN       BOOLEAN  AddressFlag,
      TRUE,//IN       BOOLEAN  DataFlag,
      FALSE //IN       BOOLEAN  FinishedFlag
    );
    CurrentDummyValue16 = Value16 >> 12;
    CurrentMode16 = (Value16 >> 9) & 7;

    switch (FchDataPtr->Mode) {
    case FCH_SPI_MODE_QUAL_144:
      DummyValue16 = 6;
      Mode16 = FCH_SPI_DEVICE_MODE_144;
      break;
    case FCH_SPI_MODE_QUAL_114:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_114;
      Mode16 = 7;
      break;
    case FCH_SPI_MODE_QUAL_122:
      DummyValue16 = 4;
      Mode16 = FCH_SPI_DEVICE_MODE_122;
      break;
    case FCH_SPI_MODE_QUAL_112:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_112;
      break;
    case FCH_SPI_MODE_FAST:
      DummyValue16 = 8;
      Mode16 = FCH_SPI_DEVICE_MODE_FAST;
      break;
    default:
      DummyValue16 = 15;
      Mode16 = 7;
      break;
    }
    if ((CurrentDummyValue16 != DummyValue16) || (CurrentMode16 != Mode16)) {
      //FCH_DEADLOOP();
      Value16 &= ~ (0x7f << 9);
      Value16 |= (DummyValue16 << 12);
      Value16 |= (Mode16 << 9);
      FchSpiTransfer (
        0x06, //IN       UINT8    PrefixCode,
        0xB1,//IN       UINT8    Opcode,
        (UINT8 *)(&Value16),//IN  OUT   UINT8    *DataPtr,
        NULL,//IN       UINT8    *AddressPtr,
        1,//IN       UINT8    Length,
        TRUE,//IN       BOOLEAN  WriteFlag,
        FALSE,//IN       BOOLEAN  AddressFlag,
        TRUE,//IN       BOOLEAN  DataFlag,
        TRUE //IN       BOOLEAN  FinishedFlag
      );
      FchStall (1000, StdHeader);
      WriteIo8 ((UINT16) (0xCF9), 0x0E);
    }
    return TRUE;
  default:
    return FALSE;
  }
}
/**
 * FchPlatformSpiQe - Platform SPI Qual Enable
 *
 *
 *
 * @param[in] FchDataPtr  - FchData Pointer.
 *
 */
BOOLEAN
FchPlatformSpiQe (
  IN       VOID     *FchDataPtr
  )
{
  UINT32 DeviceID;
  SPI_DEVICE_PROFILE        *LocalSpiDeviceProfilePtr;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  if (LocalCfgPtr->QeEnabled) {
    return TRUE;
  }
  StdHeader = LocalCfgPtr->StdHeader;
  FchReadSpiId (FALSE);
  DeviceID = FchReadSpiId (TRUE);
//  if (LocalCfgPtr->OemSpiDeviceTable != NULL) {
//    LocalSpiDeviceProfilePtr = LocalCfgPtr->OemSpiDeviceTable;
//    if (FchFindSpiDeviceProfile (DeviceID, LocalSpiDeviceProfilePtr)) {
//      return TRUE;
//    }
//  }
  LocalSpiDeviceProfilePtr = (SPI_DEVICE_PROFILE *) (&DefaultSpiDeviceTable);
  if (FchConfigureSpiDeviceDummyCycle (DeviceID, LocalCfgPtr)) {
    return TRUE;
  }
  if (FchFindSpiDeviceProfile (DeviceID, LocalSpiDeviceProfilePtr)) {
    return TRUE;
  }
  return FALSE;
}

