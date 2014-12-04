/**
 * @file
 *
 * SB SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Kabini UEFI Driver
 * @e sub-project   SB SMM Driver
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
#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_FCHXHCD3WA_FCHXHCD3WA_FILECODE
#include "FchSmm.h"
#include "FchSmmLib.h"
#include "FchXhcD3WA.h"

EFI_SMM_SYSTEM_TABLE                *mSmst;
FCH_INIT_PROTOCOL                    gAmdFchInitInSmm;



EFI_STATUS
FchSmmXhcD3WASmi ( VOID )
{
  EFI_STATUS                             Status;
  FCH_SMM_SX_DISPATCH_PROTOCOL           *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT            SxRegisterContext;
  EFI_HANDLE                             SxHandle;
  AMD_CONFIG_PARAMS                      *SmmIo;
  UINT8                                  FchRevision;
  UINT32                                 Address32;

  SmmIo = (AMD_CONFIG_PARAMS*) &mSmst->SmmIo;

  //
  // Register AMD SX SMM
  //
  Status = gBS->LocateProtocol (
                  &gFchSmmSxDispatchProtocolGuid,
                  NULL,
                  &AmdSxDispatch
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Initialize Fch turn Lpcclk0 off Call back in ACPI S3 (entry)
  //

  SxRegisterContext.Type  = AmdSxS3;
  SxRegisterContext.Phase = AmdSxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             AmdFchXhcD3WACallback,
                             &SxRegisterContext,
                             &SxHandle
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Initiial Fch Kabini workaround for PS/2 KBD malfunction after ACPI S4 resume.
  //
  FchRevision = 0;

  Address32 = BIT31 + (UINT32) ((SMBUS_BUS_DEV_FUN << 8) + FCH_LPC_REG08);
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (&mSmst->SmmIo, SMM_IO_UINT8, CFG_DATA_PORT, &FchRevision);
  if ( FchRevision >= FCH_YANGTZE ) {
    SxRegisterContext.Type  = AmdSxS4;
    SxRegisterContext.Phase = AmdSxEntry;
    SxRegisterContext.Order = 1;

    Status = AmdSxDispatch->Register (
                               AmdSxDispatch,
                               AmdFchXhcPS2WACallback,
                               &SxRegisterContext,
                               &SxHandle
                               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    SxRegisterContext.Type  = AmdSxS5;
    SxRegisterContext.Phase = AmdSxEntry;
    SxRegisterContext.Order = 1;

    Status = AmdSxDispatch->Register (
                               AmdSxDispatch,
                               AmdFchXhcPS2WACallback,
                               &SxRegisterContext,
                               &SxHandle
                               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

  }
  return Status;
}

EFI_STATUS
EFIAPI
AmdFchXhcPS2WACallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT       *DispatchContext
  )
{
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF) |= 0x3F;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdFchXhcD3WACallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT       *DispatchContext
  )
{
  UINT32     Address32;
  UINT32     Data32;

  Address32 = BIT31 + (UINT32) ((USB_XHCI_BUS_DEV_FUN << 8) + FCH_XHCI_REG04);
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);
  Data32 |= 0x7;
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);

  Address32 = BIT31 + (UINT32) ((USB_XHCI_BUS_DEV_FUN << 8) + FCH_XHCI_REG54);
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);
  Data32 |= 0x3;
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);

  Address32 = BIT31 + (UINT32) ((USB_XHCI_BUS_DEV_FUN << 8) + FCH_XHCI_REG04);
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);
  Data32 &= 0xFFFFFFF0;
  LibFchSmmIoWrite (&mSmst->SmmIo, SMM_IO_UINT32, CFG_DATA_PORT, &Data32);
  return EFI_SUCCESS;
}

EFI_DRIVER_ENTRY_POINT (FchSmmXhcD3WASmiEntry);

EFI_STATUS
FchSmmXhcD3WASmiEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  FCH_INIT_PROTOCOL               *pAmdSbCimxInitProtocol;
  EFI_STATUS                      Status;
  BOOLEAN                         InSmm;
  EFI_SMM_BASE_PROTOCOL           *SmmBase;
  EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *CompleteFilePath;
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
                    &pAmdSbCimxInitProtocol
                    );
    if (!EFI_ERROR (Status)) {
      EfiCopyMem (
        &gAmdFchInitInSmm,
        pAmdSbCimxInitProtocol,
        sizeof (FCH_INIT_PROTOCOL)
        );
    } else {
      return EFI_ABORTED;
    }
    if ( pAmdSbCimxInitProtocol->FchPolicy.Usb.Xhci0Enable ) {
      Status = FchSmmXhcD3WASmi ();
    }
    return Status;

  }
  return Status;
}


