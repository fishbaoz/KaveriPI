/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Display Feature Initialization
 *
 * Contains CPM code to perform GPIO Init under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  DisplayFeature
 * @e \$Revision: 285127 $   @e \$Date: 2014-02-18 10:33:07 -0600 (Tue, 18 Feb 2014) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#include <AmdCpmDxe.h>

VOID
EFIAPI
AmdCpmDisplayFeatureInitMid (
  IN        EFI_EVENT       Event,
  IN        VOID            *Context
  );

VOID
EFIAPI
AmdCpmDisplayFeatureInitLate (
  IN        EFI_EVENT       Event,
  IN        VOID            *Context
  );

VOID
EFIAPI
AmdCpmDisplayFeatureVbiosShadow (
  IN        EFI_EVENT       Event,
  IN        VOID            *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Display Feature DXE driver
 *
 * This function installs AMD CPM DISPLAY FEATURE PROTOCOL and initialize display features.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmDisplayFeatureDxeEntryPoint (
  IN        EFI_HANDLE                      ImageHandle,
  IN        EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                                Status;
  EFI_EVENT                                 ReadyToBootEvent;
  EFI_EVENT                                 PciIoEvent;
  EFI_HANDLE                                Handle;
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL          *CpmDisplayFeatureProtocolPtr;
  AMD_CPM_TABLE_PROTOCOL                    *CpmTableProtocolPtr;
  EFI_PCI_IO_PROTOCOL                       *PciIo;
  AMD_CPM_ALL_PCI_IO_PROTOCOLS_INSTALLED_PROTOCOL *Tmp;
  VOID                                      *RegistrationForPciIoPtr;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  RECORD_TIME_DXE (BeginAmdCpmDisplayFeatureDxeDriver);

  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  &CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD003);

  Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    sizeof (AMD_CPM_DISPLAY_FEATURE_PROTOCOL),
                    &CpmDisplayFeatureProtocolPtr
                    );

  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdCpmDisplayFeatureProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    CpmDisplayFeatureProtocolPtr
                    );
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  CpmDisplayFeatureProtocolPtr->Revision                                      = AMD_CPM_DISPLAY_FEATURE_PROTOCOL_REV;
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr                              = CpmTableProtocolPtr;
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.DisplayFeatureTablePtr     = (VOID *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DISPLAY_FEATURE);
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.DevicePathTablePtr         = (VOID *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DEVICE_PATH);
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.SpecificSsidTablePtr       = (VOID *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SPECIFIC_SSID);
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.RebrandDualGraphicsSsidTablePtr  = (VOID *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_REBRAND_DUAL_GRAPHICS_SSID);
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.VBiosImage                 = 0;
  CpmDisplayFeatureProtocolPtr->DisplayFeatureData.VBiosImageSize             = 0;

  Status = gBS->LocateProtocol (
                    &gEfiPciIoProtocolGuid,
                    NULL,
                    &PciIo
                    );

  if (EFI_ERROR (Status)) {
    Status = gBS->LocateProtocol (
                    &gAmdCpmAllPciIoProtocolsInstalledProtocolGuid,
                    NULL,
                    &Tmp
                    );
    if (EFI_ERROR (Status)) {
      AmdCpmDisplayFeatureInitMid (NULL, CpmDisplayFeatureProtocolPtr);
    } else {
      AmdCpmDisplayFeatureVbiosShadow (NULL, CpmDisplayFeatureProtocolPtr);
    }
  } else {
    Status = gBS->CreateEvent (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmDisplayFeatureInitMid,
                    CpmDisplayFeatureProtocolPtr,
                    &PciIoEvent
                    );

    Status = gBS->RegisterProtocolNotify (
                    &gEfiPciIoProtocolGuid,
                    PciIoEvent,
                    &RegistrationForPciIoPtr
                    );
  }
  Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmDisplayFeatureInitLate,
                    CpmDisplayFeatureProtocolPtr,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD103);

  RECORD_TIME_DXE (EndAmdCpmDisplayFeatureDxeDriver);

  return EFI_SUCCESS;
}
