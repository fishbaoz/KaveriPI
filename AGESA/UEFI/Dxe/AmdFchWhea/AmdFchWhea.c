/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH WHEA DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH WHEA DXE Driver
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

#include "AmdFchWhea.h"
#include "Filecode.h"
#define FILECODE UEFI_DXE_AMDFCHWHEA_AMDFCHWHEA_FILECODE



EFI_DRIVER_ENTRY_POINT (AmdFchWheaInitEntry);

STATIC EFI_GUID            mEfiAmdFchWheaDataGuid = AMD_FCH_WHEA_DATA_GUID;
AMD_FCH_WHEA_EINJ_BUFFER   *mEinjData;

UINTN
DevicePathSize (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *Start;

  if (DevicePath == NULL) {
    return 0;
  }
  //
  // Search for the end of the device path structure
  //
  Start = DevicePath;
  while (!EfiIsDevicePathEnd (DevicePath)) {
    DevicePath = EfiNextDevicePathNode (DevicePath);
  }
  //
  // Compute the size and add back in the size of the end device path structure
  //
  return ((UINTN) DevicePath - (UINTN) Start) + sizeof (EFI_DEVICE_PATH_PROTOCOL);
}

EFI_DEVICE_PATH_PROTOCOL*
AppendDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL  *Src1,
  IN EFI_DEVICE_PATH_PROTOCOL  *Src2
  )
{
  EFI_STATUS                Status;
  UINTN                     Size;
  UINTN                     Size1;
  UINTN                     Size2;
  EFI_DEVICE_PATH_PROTOCOL  *NewDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *SecondDevicePath;

  //
  // Allocate space for the combined device path. It only has one end node of
  // length EFI_DEVICE_PATH_PROTOCOL
  //
  Size1 = DevicePathSize (Src1);
  Size2 = DevicePathSize (Src2);
  Size  = Size1 + Size2;

  if (Size1 != 0 && Size2 != 0) {
    Size -= sizeof (EFI_DEVICE_PATH_PROTOCOL);
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  Size,
                  (VOID **) &NewDevicePath
                  );

  if (EFI_ERROR (Status)) {
    return NULL;
  }

  gBS->CopyMem (
         NewDevicePath,
         Src1,
         Size1
         );
  //
  // Over write Src1 EndNode and do the copy
  //
  if (Size1 != 0) {
    SecondDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) ((CHAR8 *) NewDevicePath + (Size1 - sizeof (EFI_DEVICE_PATH_PROTOCOL)));
  } else {
    SecondDevicePath = NewDevicePath;
  }

  gBS->CopyMem (
         SecondDevicePath,
         Src2,
         Size2
         );

  return NewDevicePath;
}

/********************************************************************************
 * Name: AmdFchWheaInitEntry
 *
 * Description
 *   AmdFchWheaInit Entrypoint
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
AmdFchWheaInitEntry (
  IN EFI_HANDLE                             ImageHandle,
  IN EFI_SYSTEM_TABLE                       *SystemTable
  )
{
  EFI_STATUS                                Status;
  BOOLEAN                                   InSmm;
  FCH_INIT_PROTOCOL                         *AmdFchInit;
  EFI_SMM_BASE_PROTOCOL                     *SmmBase;
  FCH_SMM_SW_DISPATCH_PROTOCOL              *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT               SwRegisterContext;
  EFI_HANDLE                                SwHandle;
  FCH_SMM_MISC_DISPATCH_PROTOCOL            *AmdFchSmmMiscDispatch;
  FCH_SMM_MISC_REGISTER_CONTEXT             MiscRegisterContext;
  EFI_HANDLE                                MiscHandle;
  EFI_SMM_SYSTEM_TABLE                      *mSmst;
  EFI_LOADED_IMAGE_PROTOCOL                 *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL                  *ImageDevicePath;
  EFI_DEVICE_PATH_PROTOCOL                  *CompleteFilePath;
  EFI_HANDLE                                SmmImageHandle;
  EFI_EVENT                                 InstallAmdTableEvent;
  EFI_HANDLE                                Handle;
  UINT8                                     *buffer;

  InSmm    = FALSE;
  DxeInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &AmdFchInit
                  );
  ASSERT_EFI_ERROR (Status);

  if (AmdFchInit->FchPolicy.Gpp.PcieAer == 0) {
    return Status;
  }

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
    Status = EfiCreateEventReadyToBoot (
               EFI_TPL_CALLBACK,
               AmdWheaCheckInstallTables,
               NULL,
               &InstallAmdTableEvent
               );

    //
    //  Allocate memory and Initialize for Data block
    //
    Status = gBS->AllocatePool (
                    EfiReservedMemoryType,
                    sizeof (AMD_FCH_WHEA_EINJ_BUFFER),
                    (VOID **)&buffer
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    EfiZeroMem (buffer, sizeof (AMD_FCH_WHEA_EINJ_BUFFER));

    mEinjData = (AMD_FCH_WHEA_EINJ_BUFFER *)buffer;
    mEinjData->Valid             = FALSE;
    mEinjData->PlatformEinjValid = FALSE;

    //
    //  Allocate memory and Initialize for Error Data block
    //
    Status = gBS->AllocatePool (
                    EfiReservedMemoryType,
                    MAX_ERROR_BLOCK_SIZE,
                    (VOID **)&buffer
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    EfiZeroMem (buffer, MAX_ERROR_BLOCK_SIZE);

    mEinjData->AmdHwErrBlk = (GENERIC_ERROR_STATUS_BLOCK *)buffer;

    AmdErrBlkAddressUpdate ();

    Handle = ImageHandle;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &mEfiAmdFchWheaDataGuid,
                    EFI_NATIVE_INTERFACE,
                    mEinjData
                    );
    if (EFI_ERROR (Status)) {
      return (Status);
    }

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
    Status = gBS->LocateProtocol (
                    &mEfiAmdFchWheaDataGuid,
                    NULL,
                    &mEinjData
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->LocateProtocol (
                    &gFchSmmSwDispatchProtocolGuid,
                    NULL,
                    &AmdSwDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    SwRegisterContext.AmdSwValue  = EINJ_TRIGGER_ACTION_SWSMI;
    Status = AmdSwDispatch->Register (
                              AmdSwDispatch,
                              AmdSmiEinjTriggerActionCallBack,
                              &SwRegisterContext,
                              &SwHandle
                              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->LocateProtocol (
                    &gFchSmmMiscDispatchProtocolGuid,
                    NULL,
                    &AmdFchSmmMiscDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
    MiscRegisterContext.SmiStatusBit = BIT21;
    MiscRegisterContext.Order        = 0x80;
    Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      AmdMiscFchWheaHwSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );

    MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
    MiscRegisterContext.SmiStatusBit = BIT22;
    MiscRegisterContext.Order        = 0x80;
    Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      AmdMiscFchWheaHwSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );

    MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
    MiscRegisterContext.SmiStatusBit = BIT23;
    MiscRegisterContext.Order        = 0x80;
    Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      AmdMiscFchWheaHwSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );

    MiscRegisterContext.SmiStatusReg = FCH_SMI_REG88;
    MiscRegisterContext.SmiStatusBit = BIT24;
    MiscRegisterContext.Order        = 0x80;
    Status = AmdFchSmmMiscDispatch->Register (
                                      AmdFchSmmMiscDispatch,
                                      AmdMiscFchWheaHwSmiCallback,
                                      &MiscRegisterContext,
                                      &MiscHandle
                                      );

    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) &= ~(BIT11 + BIT13 + BIT15 + BIT17);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB4) |= (BIT10 + BIT12 + BIT14 + BIT16);
  }
  return Status;
}

VOID
EFIAPI
AmdWheaCheckInstallTables (
  IN EFI_EVENT                              Event,
  IN VOID                                   *Context
  )
{
  EFI_STATUS                Status;
  UINT64                    MmioMsr;
  AMD_CONFIG_PARAMS         StdHeader;

  LibAmdMsrRead (0xC0010058, &MmioMsr, &StdHeader);

  mEinjData->PcieBaseAddress = (UINT32) (MmioMsr & 0xfff00000);

  Status = AmdFchWheaInitEntryEinj ();
  if (!EFI_ERROR (Status)) {
    Status = AmdFchWheaInitEntryBert ();
  }
  if (!EFI_ERROR (Status)) {
    Status = AmdFchWheaInitEntryErst ();
  }
  if (!EFI_ERROR (Status)) {
    Status = AmdFchWheaInitEntryHest ();
  }
}


EFI_STATUS
AmdFchWheaInitEntryBert ( VOID )
{
  EFI_STATUS                  Status;
  Status = EFI_SUCCESS;
  return Status;
}

EFI_STATUS
AmdFchWheaInitEntryErst ( VOID )
{
  EFI_STATUS                  Status;
  Status = EFI_SUCCESS;
  return Status;
}


EFI_STATUS
EFIAPI
AmdSmiEinjTriggerActionCallBack (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT           *DispatchContext
  )
{
  EFI_STATUS          Status;
  Status = EFI_SUCCESS;

  if (AmdSmiEinjValidate ()) {

    AmdSmiEinjSystemContextSave ();

    AmdSmiEinjErrorInjectionSetup ();

    AmdSmiEinjClrStatus ();

    AmdSmiEinjErrorInjection ();
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdMiscFchWheaHwSmiCallback (
  IN  EFI_HANDLE                            DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT         *DispatchContext
  )
{
  EFI_STATUS          Status;
  Status = EFI_SUCCESS;

  AmdSmiEinjChkErr ();
  AmdSmiEinjClrStatus ();
  AmdSmiEinjErrInjDis ();
  AmdSmiEinjSystemContextRestore ();

  return Status;
}

