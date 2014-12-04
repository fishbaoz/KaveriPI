/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH FchHwm DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH FchHwm DXE Driver
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
#include "FchSmm.h"
#include "FchSmmLib.h"
#include "AmdFchHWM.h"
#include "Filecode.h"
#define FILECODE UEFI_DXE_AMDFCHHWM_AMDFCHHWM_FILECODE


EFI_DRIVER_ENTRY_POINT (AmdFchHWMInitEntry);

/********************************************************************************
 * Name: AmdFchHWMInitEntry
 *
 * Description
 *   AmdFchHWMInit Entrypoint
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
AmdFchHWMInitEntry (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  FCH_INIT_PROTOCOL               *AmdFchInit;
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;
  EFI_SMM_SYSTEM_TABLE            *mSmst;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *CompleteFilePath;
  EFI_HANDLE                      Handle;
  EFI_HANDLE                      SmmImageHandle;
  FCH_HWM_PROTOCOL                *AmdFchHwm;

  //
  // Initialize EFI library
  //
  InSmm    = FALSE;
  gST      = SystemTable;
  gBS      = SystemTable->BootServices;

  Status = gBS->LocateProtocol (
                  &gEfiSmmBaseProtocolGuid,
                  NULL,
                  &SmmBase
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  SmmBase->GetSmstLocation (
             SmmBase,
             &mSmst
             );
  SmmBase->InSmm (
             SmmBase,
             &InSmm
             );


  if (!InSmm) {
    Status = gBS->LocateProtocol (
                    &gFchInitProtocolGuid,
                    NULL,
                    &AmdFchInit
                    );
    if (!EFI_ERROR (Status)) {
      if (AmdFchInit->FchPolicy.Hwm.HwMonitorEnable == 1) {
        //
        // Initialize EFI library
        //
        Status = LibFchDxeInitialization (SystemTable);
        ASSERT_EFI_ERROR (Status);

        Status = gBS->AllocatePool (
                        EfiACPIMemoryNVS,
                        sizeof (FCH_HWM_PROTOCOL),
                        &AmdFchHwm
                        );

        ASSERT_EFI_ERROR (Status);

        gBS->CopyMem (
               &AmdFchHwm->FchHwm,
               &AmdFchInit->FchPolicy.Hwm,
               sizeof (FCH_HWM)
               );
        AmdFchHwm->HwmInit                   = FchHwmInit;
        AmdFchHwm->HwmUpdateData             = FchHwmUpdateData;
        AmdFchHwm->HwmFchtsiAutoPolling      = FchHwmFchtsiAutoPolling;
        AmdFchHwm->HwmFchtsiAutoPollingPause = FchHwmFchtsiAutoPollingPause;

        Handle = ImageHandle;
        Status = gBS->InstallProtocolInterface (
                        &Handle,
                        &gFchHwmProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        AmdFchHwm
                        );

        FchHwmInit (AmdFchHwm);
        FchHwmUpdateData (AmdFchHwm);

        if (EFI_ERROR (Status)) {
          return (Status);
        }
      } else {
        return EFI_ABORTED;
      }
    } else {
      return EFI_ABORTED;
    }

    //
    // Not in SMM. Load this driver's image into SMRAM
    //
    if (ImageHandle != NULL) {
      Status = gBS->HandleProtocol (
                      ImageHandle,
                      &gEfiLoadedImageProtocolGuid,
                      &LoadedImage
                      );
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Status = gBS->HandleProtocol (
                      LoadedImage->DeviceHandle,
                      &gEfiDevicePathProtocolGuid,
                      (VOID*) &ImageDevicePath
                      );

      if (EFI_ERROR (Status)) {
        return Status;
      }

      CompleteFilePath = AppendDevicePath (
                           ImageDevicePath,
                           LoadedImage->FilePath
                           );

      // Load the image in memory to SMRAM, this automatically triggers SMI
      SmmBase->Register (
                 SmmBase,
                 CompleteFilePath,
                 NULL,
                 0,
                 &SmmImageHandle,
                 FALSE
                 );
    }
  } else {
    //
    // We're now in SMM!
    //
    Status = FchHwmSmmRegister ();
    return Status;

  }
  return Status;
}

EFI_STATUS
FchHwmSmmRegister ( VOID )
{
  EFI_STATUS                             Status;
  FCH_SMM_SW_DISPATCH_PROTOCOL           *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT            SwRegisterContext;
  EFI_HANDLE                             SwHandle;

  //
  //  Locate SMM SW dispatch protocol
  //
  Status = gBS->LocateProtocol (
                  &gFchSmmSwDispatchProtocolGuid,
                  NULL,
                  &AmdSwDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  SwRegisterContext.AmdSwValue  = FCHOEM_AFTER_PCI_RESTORE_SWSMI;
  SwRegisterContext.Order       = 0x82;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            FchHwmSmmCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

EFI_STATUS
EFIAPI
FchHwmSmmCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT       *DispatchContext
  )
{
  EFI_STATUS          Status;

  Status = EFI_SUCCESS;
//  FchHwmInit (AmdFchInit->FchPolicy);

  return Status;
}

/**
 * FchHwmInit - Init Hardware Monitor.
 *
 *
 */
VOID
FchHwmInit (
  IN       VOID* AmdFchConfig
  )
{
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  FchHwmInitRegister (FchConfig);
  FchHwmCopyFanControl (FchConfig);
  FchHwmProcessParameter (FchConfig);
  FchHwmSetRegister (FchConfig);
  FchHwmFchtsiAutoPolling (FchConfig);
}

/**
 * FchHwmUpdateData - Hardware Monitor Update Data.
 *
 *
 *
 */
VOID
FchHwmUpdateData (
  IN       VOID* AmdFchConfig
  )
{
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  FchHwmFchtsiAutoPolling (FchConfig);
  FchHwmGetRawData (FchConfig);
  FchHwmCaculate (FchConfig);
}

VOID
HwmMemRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       UINT32                                   AndMask,
  IN       UINT32                                   OrMask
  )
{
  LibFchMemRw ( EfiWidth, Address, &AndMask, &OrMask);
}

FCH_HWM_TEMP_PAR TempParDefault0[] = {
  { FCH_T0AT, FCH_T0CT , 0 },
  { FCH_T1AT, FCH_T1CT , 0 },
  { FCH_T2AT, FCH_T2CT , BIT0 },         ///High Ratio
  { FCH_T3AT, FCH_T3CT , BIT1 },         ///High Current
  { FCH_T4AT, FCH_T4CT , 0 }
};

/**
 * FchHwmInitRegister - Init Hardware Monitor Register.
 *
 *
 */
VOID
FchHwmInitRegister (
  IN       VOID* AmdFchConfig
  )
{
  UINT32            MmioAddr;
  UINT32            LinearRangeOutLimit;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  LibFchGetAcpiMmioBase (&MmioAddr);
  MmioAddr &= 0xFFFFC000;

  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xB2, 0, 0x55);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xB3, 0, 0x55);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x91, 0, 0x55);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x92, 0, 0x55);

  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x00, 0, 0x06);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x10, 0, 0x06);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x20, 0, 0x06);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x30, 0, 0x06);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x40, 0, 0x06);

  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x66, 0, 0x01);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x6B, 0, 0x01);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x70, 0, 0x01);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x75, 0, 0x01);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0x7A, 0, 0x01);

  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xE6, 0xff, 0x02);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xF8, 0, 0x05);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xF9, 0, 0x06);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xFF, 0, 0x42);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xE9, 0, 0xFF);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xEB, 0, 0x1F);
  //RPR 2.12 FchHwm Sensor Clk
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xEF, 0, 0x0A);
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + 0xFB, 0, 0x00);
  //2.9 Enhancement of FanOut0 Control
  HwmMemRw ( EfiPciWidthUint32, MmioAddr + MISC_BASE + FCH_MISC_REG50, ~ (BIT11 + BIT20), (BIT11 + BIT20));
  HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO_BASE + FCH_PMIOA_REGB6, 0x0F, 0x10);

  //check for fanLinearEnhanceEn
  if (FchConfig->FchHwm.FanLinearEnhanceEn == 0) {
    HwmMemRw ( EfiPciWidthUint32, MmioAddr + MISC_BASE + FCH_MISC_REG50, ~ BIT11, BIT11);
    LinearRangeOutLimit = (UINT32) (FchConfig->FchHwm.FanLinearRangeOutLimit);
    LinearRangeOutLimit = LinearRangeOutLimit << 20;
    HwmMemRw ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REGB4, 0xFF0FFFFF, LinearRangeOutLimit);
  } else {
    HwmMemRw ( EfiPciWidthUint32, MmioAddr + MISC_BASE + FCH_MISC_REG50, ~ BIT11, 0);
  }
  //check for fanLinearHoldFix
  if (FchConfig->FchHwm.FanLinearHoldFix == 0) {
    HwmMemRw ( EfiPciWidthUint32, MmioAddr + MISC_BASE + FCH_MISC_REG50, ~ BIT20, BIT20);
  } else {
    HwmMemRw ( EfiPciWidthUint32, MmioAddr + MISC_BASE + FCH_MISC_REG50, ~ BIT20, 0);
  }
}

/**
 * FchHwmCopyFanControl - Hardware Monitor Copy Fan Control Data.
 *
 *
 */
VOID
FchHwmCopyFanControl (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             *FanControl;
  UINT8             *FanControlCooked;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  if ( FchConfig->FchHwm.HwMonitorEnable ) {
    FanControl = & FchConfig->FchHwm.HwmFanControl[0].InputControlReg00;
    FanControlCooked = & FchConfig->FchHwm.HwmFanControlCooked[0].InputControlReg00;
    gBS->CopyMem (
           FanControlCooked,
           FanControl,
           (sizeof (FCH_HWM_FAN_CTR) * 5)
           );
  }
}

/**
 * FchHwmProcessParameter - Hardware Monitor process Parameter
 *
 *
 */
VOID
FchHwmProcessParameter (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             Index;
  UINT8             TempChannel;
  UINT8             ValueByte;
  UINT16            ValueWord;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  FchHwmGetCalibrationFactor (FchConfig);
  //
  //temperatue parameter
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    if ( FchConfig->FchHwm.HwmTempPar[Index].At == 0 ) {
      FchConfig->FchHwm.HwmTempPar[Index] = TempParDefault0[Index];
    }
  }

  for ( Index = 0; Index < 5 ; Index++ ) {
    if ( FchConfig->FchHwm.HwmFanControl[Index].LowDutyReg03 == 100 ) {
      FchConfig->FchHwm.HwmFanControlCooked[Index].LowDutyReg03 = 255;
    } else {
      FchConfig->FchHwm.HwmFanControlCooked[Index].LowDutyReg03 = (FchConfig->FchHwm.HwmFanControl[Index].LowDutyReg03 << 8) / 100;
    }

    if ( FchConfig->FchHwm.HwmFanControl[Index].MedDutyReg04 == 100 ) {
      FchConfig->FchHwm.HwmFanControlCooked[Index].MedDutyReg04 = 255;
    } else {
      FchConfig->FchHwm.HwmFanControlCooked[Index].MedDutyReg04 = (FchConfig->FchHwm.HwmFanControl[Index].MedDutyReg04 << 8) / 100;
    }

    if ( FchConfig->FchHwm.HwmFanControl[Index].HighTempReg0A > FchConfig->FchHwm.HwmFanControl[Index].MedTempReg08 ) {
      ValueByte = (UINT8) ((256 - FchConfig->FchHwm.HwmFanControl[Index].LowDutyReg03) / (FchConfig->FchHwm.HwmFanControl[Index].HighTempReg0A - FchConfig->FchHwm.HwmFanControl[Index].MedTempReg08));
    } else {
      ValueByte = (UINT8) (256 - FchConfig->FchHwm.HwmFanControl[Index].LowDutyReg03);
    }
    ValueWord = FchConfig->FchHwm.HwmFanControl[Index].LowTempReg06;

    if (FchConfig->FchHwm.HwmFanControl[Index].InputControlReg00 > 4) {
      TempChannel = 0;
    } else {
      TempChannel = FchConfig->FchHwm.HwmFanControl[Index].InputControlReg00;
    }

    if ((FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) && (Index == 0)) {
      ValueWord = ValueWord << 8;
    } else {
      ValueByte = (UINT8) (ValueByte * 10000 * FchConfig->FchHwm.HwmCalibrationFactor / FchConfig->FchHwm.HwmTempPar[TempChannel].At / 512);
      ValueWord = ((ValueWord * 100 + FchConfig->FchHwm.HwmTempPar[TempChannel].Ct ) * 100 * FchConfig->FchHwm.HwmCalibrationFactor / FchConfig->FchHwm.HwmTempPar[TempChannel].At) >> 3;
    }
    FchConfig->FchHwm.HwmFanControlCooked[Index].LowTempReg06 = ValueWord;
    FchConfig->FchHwm.HwmFanControlCooked[Index].MultiplierReg05 = ValueByte & 0x3f;

    ValueWord = FchConfig->FchHwm.HwmFanControl[Index].MedTempReg08;
    if ((FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) && (Index == 0)) {
      ValueWord = ValueWord << 8;
    } else {
      ValueWord = ((ValueWord * 100 + FchConfig->FchHwm.HwmTempPar[TempChannel].Ct ) * 100 * FchConfig->FchHwm.HwmCalibrationFactor / FchConfig->FchHwm.HwmTempPar[TempChannel].At) >> 3;
    }
    FchConfig->FchHwm.HwmFanControlCooked[Index].MedTempReg08 = ValueWord;

    ValueWord = FchConfig->FchHwm.HwmFanControl[Index].HighTempReg0A;
    if ((FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) && (Index == 0)) {
      ValueWord = ValueWord << 8;
    } else {
      ValueWord = ((ValueWord * 100 + FchConfig->FchHwm.HwmTempPar[TempChannel].Ct ) * 100 * FchConfig->FchHwm.HwmCalibrationFactor / FchConfig->FchHwm.HwmTempPar[TempChannel].At) >> 3;
    }
    FchConfig->FchHwm.HwmFanControlCooked[Index].HighTempReg0A = ValueWord;
  }
}

/**
 * FchHwmSetRegister - Hardware Monitor Set Parameter
 *
 *
 */
VOID
FchHwmSetRegister (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             *DbValuePtr;
  UINT8             Index;
  UINT8             RegisterN;
  UINT8             RegisterPM2RegF8;
  UINT8             RegisterPM2RegF9;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  //
  //Configure Fans
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    DbValuePtr = &(FchConfig->FchHwm.HwmFanControlCooked[Index].InputControlReg00);
    for ( RegisterN = 0; RegisterN < 0x0E ; RegisterN++ ) {
      LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGCD0, Index * 0x10 + RegisterN, DbValuePtr);
      DbValuePtr ++;
    }
  }

  //
  //Configure Sample Frequency Divider
  //
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGCD0, FCH_PMIO2_REG63, &(FchConfig->FchHwm.FanSampleFreqDiv));

  //
  //Configure Mode
  //
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGCD0, 0xF8, &RegisterPM2RegF8);
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGCD0, 0xF9, &RegisterPM2RegF9);
  for ( Index = 0; Index < 5 ; Index++ ) {
    if (FchConfig->FchHwm.HwmTempPar[Index].Mode == BIT0) {
      RegisterPM2RegF8 |= 1 << (Index + 3);
    } else if (FchConfig->FchHwm.HwmTempPar[Index].Mode == BIT1) {
      RegisterPM2RegF9 |= 1 << (Index + 3);
    }
  }
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGCD0, 0xF8, &RegisterPM2RegF8);
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGCD0, 0xF9, &RegisterPM2RegF9);
}

/**
 * FchHwmGetCalibrationFactor - Hardware Monitor Get Calibration
 * Factor
 *
 *
 */
VOID
FchHwmGetCalibrationFactor (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             ValueByte;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  //
  //temperatue parameter
  //
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGCD0, FCH_PMIO2_REGEA, &ValueByte);
  if ( ValueByte & BIT7 ) {
    if ( ValueByte & BIT6 ) {
      FchConfig->FchHwm.HwmCalibrationFactor = 0x100 + ValueByte;
    } else {
      FchConfig->FchHwm.HwmCalibrationFactor = 0x200 + (ValueByte & 0x3f );
    }
  } else {
    FchConfig->FchHwm.HwmCalibrationFactor = 0x200;
  }
}
/**
 * FchHwmFchtsiAutoPolling - Hardware Monitor Auto Poll SB-TSI.
 *
 *
 */
VOID
FchHwmFchtsiAutoPolling (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             ValueByte;
  UINT16            SmbusBase;
  UINT32            MmioAddr;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  LibFchGetAcpiMmioBase (&MmioAddr);
  MmioAddr &= 0xFFFFC000;

  SmbusBase = (UINT16) ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG2C);
  SmbusBase &= 0xFFE0;

  if (FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) {
    FchHwmFchtsiAutoPollingPause (FchConfig);

    HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO_BASE + FCH_PMIOA_REG2E, ~(BIT1 + BIT2), BIT2);
    ValueByte = FCH_SMB_ALL_HOST_STATUS;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG00, &ValueByte);
    ValueByte = FCH_SMB_CMD_BYTE_DATA;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG02, &ValueByte);
    ValueByte = SBTSI_REG09;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG03, &ValueByte);
    ValueByte = SBTSI_ADDR;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG04, &ValueByte);
    ValueByte = SBTSI_READORDER;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG05, &ValueByte);
    ValueByte = FCH_SMB_CMD_BYTE_DATA_START;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG02, &ValueByte);

    LibFchIoRead (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG00, &ValueByte);
    while ( ValueByte & BIT0 ) {
      LibFchIoRead (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG00, &ValueByte);
    }

    ValueByte = FCH_SMB_CMD_BYTE_DATA;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG02, &ValueByte);
    ValueByte = SBTSI_REG10;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG03, &ValueByte);
    ValueByte = SBTSI_ADDR + FCH_SMB_READ_ENABLE;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG04, &ValueByte);

    ValueByte = FCH_SMB_POLL2BYTE;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG14, &ValueByte);
    ValueByte = SBTSI_REG01;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG17, &ValueByte);
    ValueByte = FCH_SMB_POLL2BYTE + FCH_SMB_AUTO_POLL_EN;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG14, &ValueByte);

    //
    //map SB-TSI to tempin0
    //
    HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + FCH_PMIO2_REG92, ~BIT3, BIT3);
    ValueByte = 0x00;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG16, &ValueByte);
    FchConfig->FchHwm.HwmFchtsiAutoPollStarted = TRUE;
  } else {
    HwmFchtsiAutoPollingOff (FchConfig);
  }
}

/**
 * FchHwmFchtsiAutoPollingOff - Hardware Monitor Auto Poll SB-TSI
 * Off.
 *
 *
 */
VOID
FchHwmFchtsiAutoPollingOff (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             ValueByte;
  UINT16            SmbusBase;
  UINT32            MmioAddr;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  if ( FchConfig->FchHwm.HwMonitorEnable ) {
    SmbusBase = (UINT16) ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG2C);
    SmbusBase &= 0xFFE0;

    LibFchGetAcpiMmioBase (&MmioAddr);
    MmioAddr &= 0xFFFFC000;

    ValueByte = 0x00;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG14, &ValueByte);
    FchHwmFchtsiAutoPollingPause (FchConfig);
    HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO_BASE + FCH_PMIOA_REG2E, ~(BIT1 + BIT2), 0);
    HwmMemRw ( EfiPciWidthUint8, MmioAddr + PMIO2_BASE + FCH_PMIO2_REG92, ~BIT3, 0x00);
    FchConfig->FchHwm.HwmFchtsiAutoPollStarted = FALSE;
  }
}

/**
 * FchHwmFchtsiAutoPollingPause - Pause Hardware Monitor Auto Poll
 * SB-TSI Off.
 *
 *
 */
VOID
FchHwmFchtsiAutoPollingPause (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             ValueByte;
  UINT16            SmbusBase;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  if ( FchConfig->FchHwm.HwMonitorEnable && (FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) ) {
    SmbusBase = (UINT16) ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG2C);
    SmbusBase &= 0xFFE0;
    ValueByte = 0x01;
    LibFchIoWrite (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG16, &ValueByte);
    ValueByte = 0x00;
    while ( ValueByte == 0x00 ) {
      LibFchIoRead (EfiPciWidthUint8, SmbusBase + FCH_SMB_IOREG16, &ValueByte);
    }
  }
}

/**
 * FchHwmGetRawData - Hardware Monitor Get Raw Data.
 *
 *
 */
VOID
FchHwmGetRawData (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             Index;
  UINT16            ValueWord;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  //
  //fan speed
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD0, FCH_PMIO2_REG69 + Index * 5, &ValueWord);
    if ( (ValueWord & 0xFFC0) != 0xFFC0 ) {
      FchConfig->FchHwm.HwmCurrentRaw.FanSpeed[Index] = ValueWord;
    } else {
      FchConfig->FchHwm.HwmCurrentRaw.FanSpeed[Index] = 0xFFFF;
    }
  }
  //
  //temperatue
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD0, FCH_PMIO2_REG95 + Index * 4, &ValueWord);
    if ( ( Index == 1 ) || (ValueWord > 0x4000) ) {
      FchConfig->FchHwm.HwmCurrentRaw.Temperature[Index] = ValueWord;
    }
  }
  //
  //voltage
  //
  for ( Index = 0; Index < 8 ; Index++ ) {
    LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD0, FCH_PMIO2_REGB8 + Index * 4, &ValueWord);
    if ( (ValueWord & 0xFFC0) != 0xFFC0 ) {
      FchConfig->FchHwm.HwmCurrentRaw.Voltage[Index] = ValueWord;
    }
  }
}

/**
 * FchHwmCaculate - Hardware Monitor Caculate Raw Data to Display Data.
 *
 *
 */
VOID
FchHwmCaculate (
  IN       VOID* AmdFchConfig
  )
{
  UINT8             Index;
  UINT16            ValueWord;
  FCH_HWM_PROTOCOL  *FchConfig;
  FchConfig = (FCH_HWM_PROTOCOL*)AmdFchConfig;

  //
  //fan speed
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    ValueWord = FchConfig->FchHwm.HwmCurrentRaw.FanSpeed[Index];
    if ((ValueWord == 0xffff) || (ValueWord == 0x0000)) {
      FchConfig->FchHwm.HwmCurrent.FanSpeed[Index] = 0;
    } else {
      FchConfig->FchHwm.HwmCurrent.FanSpeed[Index] = ( 22720 >> FchConfig->FchHwm.FanSampleFreqDiv ) * 60 / ValueWord / 2;
    }
  }
  //
  //temperatue
  //
  for ( Index = 0; Index < 5 ; Index++ ) {
    ValueWord = FchConfig->FchHwm.HwmCurrentRaw.Temperature[Index];
    if ((FchConfig->FchHwm.HwmFchtsiAutoPoll == 1) && (Index == 1)) {
      ValueWord = ((ValueWord & 0xff00) >> 8) * 10 + (((ValueWord & 0x00ff) * 10 ) >> 8);
    } else {
      ValueWord = ((ValueWord << 3) * FchConfig->FchHwm.HwmTempPar[Index].At / FchConfig->FchHwm.HwmCalibrationFactor / 100 - FchConfig->FchHwm.HwmTempPar[Index].Ct) / 10;
    }
    if ( FchConfig->FchHwm.HwmCurrent.Temperature[Index] == 0 ) {
      ValueWord = 0;
    }
    if ( ValueWord < 10000 ) {
      FchConfig->FchHwm.HwmCurrent.Temperature[Index] = ValueWord;
    } else {
      FchConfig->FchHwm.HwmCurrent.Temperature[Index] = 0;
    }
  }
  //
  //voltage
  //
  for ( Index = 0; Index < 8 ; Index++ ) {
    ValueWord = FchConfig->FchHwm.HwmCurrentRaw.Voltage[Index];
    FchConfig->FchHwm.HwmCurrent.Voltage[Index] = (ValueWord >> 6) * 512 / FchConfig->FchHwm.HwmCalibrationFactor;
  }
}

