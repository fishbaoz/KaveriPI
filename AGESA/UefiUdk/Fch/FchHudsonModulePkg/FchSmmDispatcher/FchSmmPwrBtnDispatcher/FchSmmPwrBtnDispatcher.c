/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 POWER BUTTON Dispatcher Driver
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

#include "FchSmmPwrBtnDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMPWRBTNDISPATCHER_FCHSMMPWRBTNDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2Register (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PWRBTN_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       FCH_SMM_PWRBTN_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2UnRegister (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                               DispatchHandle
  );

FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL gFchSmmPwrBtnDispatch2Protocol = {
  FchSmmPwrBtnDispatch2Register,
  FchSmmPwrBtnDispatch2UnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM POWER BUTTON dispatcher handler
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
FchSmmPwrBtnDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  Status = EFI_NOT_FOUND;

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) = PwrBtn;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr!= NULL) {
      if (CurrentFchSmmPwrBtnNodePtr->Context.Phase == EfiPowerButtonEntry) {
        if (CurrentFchSmmPwrBtnNodePtr->CallBack2Function != NULL) {
          Status = CurrentFchSmmPwrBtnNodePtr->CallBack2Function (
                                                 CurrentFchSmmPwrBtnNodePtr->DispatchHandle,
                                                 &CurrentFchSmmPwrBtnNodePtr->Context,
                                                 NULL,
                                                 NULL
                                                 );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] PwrBtn SMM handler dispatched (Phase = Entry) Order = 0x%x, \
                  return - %r\n", CurrentFchSmmPwrBtnNodePtr->Context.Order, Status));
        }
        Status = EFI_SUCCESS;
      }
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }

    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr!= NULL) {
      if (CurrentFchSmmPwrBtnNodePtr->Context.Phase == EfiPowerButtonExit) {
        if (CurrentFchSmmPwrBtnNodePtr->CallBack2Function != NULL) {
          Status = CurrentFchSmmPwrBtnNodePtr->CallBack2Function (
                                                 CurrentFchSmmPwrBtnNodePtr->DispatchHandle,
                                                 &CurrentFchSmmPwrBtnNodePtr->Context,
                                                 NULL,
                                                 NULL
                                                 );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] PwrBtn SMM handler dispatched (Phase = Exit) Order = 0x%x, \
                  return - %r\n", CurrentFchSmmPwrBtnNodePtr->Context.Order, Status));
        }
        Status = EFI_SUCCESS;
      }
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register POWER BOTTON child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   RegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPwrBtnDispatch2Register (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PWRBTN_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       FCH_SMM_PWRBTN_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                               *DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *NewFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *PreviousFchSmmPwrBtnNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC) |= BIT6;
  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_PWRBTN_NODE),
                       &NewFchSmmPwrBtnNodePtr
                       );
  ASSERT_EFI_ERROR (Status);
  NewFchSmmPwrBtnNodePtr->CallBack2Function = CallBackFunction;
  *DispatchHandle = &NewFchSmmPwrBtnNodePtr->DispatchHandle;
  NewFchSmmPwrBtnNodePtr->DispatchHandle = *DispatchHandle;
  NewFchSmmPwrBtnNodePtr->Context = *RegisterContext;

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering PwrBtn SMM handler: Phase = 0x%x, Order = 0x%x\n", \
         RegisterContext->Phase, RegisterContext->Order));
  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    HeadFchSmmPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
  } else {
    PreviousFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    while (CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr != NULL) {
      if (NewFchSmmPwrBtnNodePtr->Context.Order <= CurrentFchSmmPwrBtnNodePtr->Context.Order) {
        if (PreviousFchSmmPwrBtnNodePtr == CurrentFchSmmPwrBtnNodePtr) {
          NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
          HeadFchSmmPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
        PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;

        Status = EFI_SUCCESS;
        return Status;
      }
      PreviousFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
      CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }
    PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = NewFchSmmPwrBtnNodePtr;
    NewFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
  }
  Status = EFI_SUCCESS;



  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister POWER BUTTON child handler
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
FchSmmPwrBtnDispatch2UnRegister (
  IN       CONST FCH_SMM_PWRBTN_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PWRBTN_NODE *CurrentFchSmmPwrBtnNodePtr;
  FCH_SMM_PWRBTN_NODE *PreviousFchSmmPwrBtnNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    CurrentFchSmmPwrBtnNodePtr = HeadFchSmmPwrBtnNodePtr;
    if (CurrentFchSmmPwrBtnNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    } else {
      while (CurrentFchSmmPwrBtnNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr;
        CurrentFchSmmPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
        if (CurrentFchSmmPwrBtnNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr = CurrentFchSmmPwrBtnNodePtr->FchPwrBtnNodePtr;
    }

    Status = gSmst->SmmFreePool (
                         CurrentFchSmmPwrBtnNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


EFI_STATUS
EFIAPI
EfiSmmPwrBtnDispatch2Register (
  IN       CONST EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2                  DispatchFunction,
  IN       EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT         *RegisterContext,
     OUT   EFI_HANDLE                                    *DispatchHandle
  )
{
  FCH_SMM_PWRBTN_REGISTER_CONTEXT  FchRegisterContext;

  FchRegisterContext.Phase = RegisterContext->Phase;
  FchRegisterContext.Order = 0x80;      // SMM_DEFAULT_ORDER
  return FchSmmPwrBtnDispatch2Register (
           &gFchSmmPwrBtnDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );
}

EFI_STATUS
EFIAPI
EfiSmmPwrBtnDispatch2UnRegister (
  IN       CONST EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                                    DispatchHandle
  )
{
  return FchSmmPwrBtnDispatch2UnRegister (
           &gFchSmmPwrBtnDispatch2Protocol,
           DispatchHandle
           );
}

EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL gEfiSmmPwrBtnDispatch2Protocol = {
  EfiSmmPwrBtnDispatch2Register,
  EfiSmmPwrBtnDispatch2UnRegister
};

