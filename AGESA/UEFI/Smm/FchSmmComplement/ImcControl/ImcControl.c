/* $NoKeywords:$ */
/**
 * @file
 *
 * Imc Control Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Hudson-2 UEFI Driver
 * @e sub-project   Imc Control Driver
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
#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_IMCCONTROL_IMCCONTROL_FILECODE
#include "FchSmm.h"
#include "FchSmmLib.h"
#include "ImcControl.h"

EFI_SMM_SYSTEM_TABLE                 *mSmst;
EFI_SMM_CPU_IO_INTERFACE             *mSmmIo;
UINT32                               mImcThermalEnable;   ///< Bit0=81FunZone0 support(1=On;0=Off); bit1-3=81FunZone1-Zone3;

EFI_STATUS
EFIAPI
ImcControlS3SleepEntryCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT       *DispatchContext
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlS4SleepEntryCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT       *DispatchContext
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlS5SleepEntryCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT       *DispatchContext
  )
{
  FchImcWakeup ();
  FchImcIdle ();
  FchImcSleep ();
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImcControlThermalZoneEnableCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  )
{
  FchImcWakeup ();
  FchImcThermalZoneEnable ();
  return EFI_SUCCESS;
}

EFI_STATUS
ImcControlSmmRegister (
  VOID
  )
{
  EFI_STATUS                               Status;
  FCH_SMM_SX_DISPATCH_PROTOCOL             *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT              SxRegisterContext;
  EFI_HANDLE                               SxHandle;
  FCH_SMM_SW_DISPATCH_PROTOCOL             *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT              SwRegisterContext;
  EFI_HANDLE                               SwHandle;

  //
  // Register Imc Control SX SMM
  //
  Status = gBS->LocateProtocol (
                  &gFchSmmSxDispatchProtocolGuid,
                  NULL,
                  &AmdSxDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  SxRegisterContext.Type  = AmdSxS3;
  SxRegisterContext.Phase = AmdSxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS3SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = AmdSxS4;
  SxRegisterContext.Phase = AmdSxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS4SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = AmdSxS5;
  SxRegisterContext.Phase = AmdSxEntry;
  SxRegisterContext.Order = 0x80;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             ImcControlS5SleepEntryCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

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
  SwRegisterContext.Order       = 0x90;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            ImcControlThermalZoneEnableCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );

  SwRegisterContext.AmdSwValue  = FCHOEM_ENABLE_ACPI_SWSMI;
  SwRegisterContext.Order       = 0x90;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            ImcControlThermalZoneEnableCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );

  return Status;
}

EFI_DRIVER_ENTRY_POINT (ImcControlEntry);

EFI_STATUS
ImcControlEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *CompleteFilePath;
  FCH_INIT_PROTOCOL               *AmdFchInit;
  EFI_HANDLE                      SmmImageHandle;

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
  mSmmIo = &mSmst->SmmIo;
  SmmBase->InSmm (
             SmmBase,
             &InSmm
             );


  if (!InSmm) {
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
    Status = gBS->LocateProtocol (
                    &gFchInitProtocolGuid,
                    NULL,
                    &AmdFchInit
                    );
    if (!EFI_ERROR (Status)) {
      mImcThermalEnable = AmdFchInit->FchPolicy.Imc.EcStruct.IMCFUNSupportBitMap;
    } else {
      return EFI_ABORTED;
    }

    Status = ImcControlSmmRegister ();
    return Status;

  }
  return Status;
}


