/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 USB Dispatcher Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/******************************************************************************
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
****************************************************************************/

#include "FchSmmUsbDispatcher.h"
#include "Filecode.h"
#include <Library\DevicePathLib.h>
#include <Library\BaseMemoryLib.h>
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMUSBDISPATCHER_FCHSMMUSBDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmUsbDispatch2Register (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_USB_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmUsbDispatch2UnRegister (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  );

FCH_SMM_USB_DISPATCH2_PROTOCOL gFchSmmUsbDispatch2Protocol = {
  FchSmmUsbDispatch2Register,
  FchSmmUsbDispatch2UnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM USB dispatcher handler
 *
 *
 * @param[in]       SmmImageHandle        Image Handle
 * @param[in, out]   OPTIONAL CommunicationBuffer   Communication Buffer (see PI 1.1 for more details)
 * @param[in, out]   OPTIONAL SourceSize            Buffer size (see PI 1.1 for more details)

 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmUsbDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;

  Status = EFI_NOT_FOUND;

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) |= UsbSmi;
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (CurrentFchSmmUsbNodePtr->FchUsbNodePtr!= NULL) {
      if (CurrentFchSmmUsbNodePtr->CallBack2Function != NULL) {
        Status = CurrentFchSmmUsbNodePtr->CallBack2Function (
                                            CurrentFchSmmUsbNodePtr->DispatchHandle,
                                            &CurrentFchSmmUsbNodePtr->Context,
                                            NULL,
                                            NULL
                                            );
        DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] USB SMM handler dispatched: Order = 0x%x, \
               return - %r\n", CurrentFchSmmUsbNodePtr->Context.Order, Status));
      }
      CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }
  }
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = UsbSmi;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register USB child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmUsbDispatch2Register (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_USB_REGISTER_CONTEXT    *RegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *NewFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *PreviousFchSmmUsbNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT24;
  ACPIMMIO32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED) |= (BIT4 + BIT0);
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_USB_NODE),
                    &NewFchSmmUsbNodePtr
                    );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmUsbNodePtr->CallBack2Function = CallBackFunction;
  NewFchSmmUsbNodePtr->Context = *RegisterContext;
  *DispatchHandle = &NewFchSmmUsbNodePtr->DispatchHandle;
  NewFchSmmUsbNodePtr->DispatchHandle = *DispatchHandle;

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering USB SMM handler: Type = 0x%x, Order = 0x%x\n", \
         RegisterContext->Type, RegisterContext->Order));
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    NewFchSmmUsbNodePtr->FchUsbNodePtr = HeadFchSmmUsbNodePtr;
    HeadFchSmmUsbNodePtr = NewFchSmmUsbNodePtr;
  } else {
    PreviousFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    while (CurrentFchSmmUsbNodePtr->FchUsbNodePtr != NULL) {
      if (NewFchSmmUsbNodePtr->Context.Order <= CurrentFchSmmUsbNodePtr->Context.Order &&
          CurrentFchSmmUsbNodePtr->Context.Type == NewFchSmmUsbNodePtr->Context.Type &&
          CompareMem (CurrentFchSmmUsbNodePtr->Context.Device, NewFchSmmUsbNodePtr->Context.Device, GetDevicePathSize (NewFchSmmUsbNodePtr->Context.Device)) == 0) {

        if (PreviousFchSmmUsbNodePtr == CurrentFchSmmUsbNodePtr) {
          NewFchSmmUsbNodePtr->FchUsbNodePtr = HeadFchSmmUsbNodePtr;
          HeadFchSmmUsbNodePtr = NewFchSmmUsbNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmUsbNodePtr->FchUsbNodePtr = PreviousFchSmmUsbNodePtr->FchUsbNodePtr;
        PreviousFchSmmUsbNodePtr->FchUsbNodePtr = NewFchSmmUsbNodePtr;

        Status = EFI_SUCCESS;
        return  Status;
      }
      PreviousFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr;
      CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }
    PreviousFchSmmUsbNodePtr->FchUsbNodePtr = NewFchSmmUsbNodePtr;
    NewFchSmmUsbNodePtr->FchUsbNodePtr = CurrentFchSmmUsbNodePtr;
  }
  Status = EFI_SUCCESS;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister USB child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       DispatchHandle
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmUsbDispatch2UnRegister (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS       Status;
  FCH_SMM_USB_NODE *CurrentFchSmmUsbNodePtr;
  FCH_SMM_USB_NODE *PreviousFchSmmUsbNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (HeadFchSmmUsbNodePtr->FchUsbNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    CurrentFchSmmUsbNodePtr = HeadFchSmmUsbNodePtr;
    if (CurrentFchSmmUsbNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    } else {
      while (CurrentFchSmmUsbNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr;
        CurrentFchSmmUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
        if (CurrentFchSmmUsbNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmUsbNodePtr->FchUsbNodePtr = CurrentFchSmmUsbNodePtr->FchUsbNodePtr;
    }

    Status = gSmst->SmmFreePool (
                      CurrentFchSmmUsbNodePtr
                      );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


EFI_STATUS
EFIAPI
EfiSmmUsbDispatch2Register (
  IN       CONST EFI_SMM_USB_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2         DispatchFunction,
  IN       CONST EFI_SMM_USB_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  FCH_SMM_USB_REGISTER_CONTEXT     FchRegisterContext;

  FchRegisterContext.Type   = RegisterContext->Type;
  FchRegisterContext.Device = RegisterContext->Device;
  FchRegisterContext.Order  = 0x80;      // SMM_DEFAULT_ORDER
  return FchSmmUsbDispatch2Register (
           &gFchSmmUsbDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );
}

EFI_STATUS
EFIAPI
EfiSmmUsbDispatch2UnRegister (
  IN       CONST EFI_SMM_USB_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                           DispatchHandle
  )
{
  return FchSmmUsbDispatch2UnRegister (
           &gFchSmmUsbDispatch2Protocol,
           DispatchHandle
           );
}

EFI_SMM_USB_DISPATCH2_PROTOCOL gEfiSmmUsbDispatch2Protocol = {
  EfiSmmUsbDispatch2Register,
  EfiSmmUsbDispatch2UnRegister
};

