/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 Misc Dispatcher Driver
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

#include "FchSmmMiscDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMMISCDISPATCHER_FCHSMMMISCDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmMiscDispatchRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_MISC_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext,
     OUT   EFI_HANDLE                             *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmMiscDispatchUnRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                             DispatchHandle
  );

FCH_SMM_MISC_DISPATCH_PROTOCOL gFchSmmMiscDispatchProtocol = {
  FchSmmMiscDispatchRegister,
  FchSmmMiscDispatchUnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM Misc dispatcher handler
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
FchSmmMiscDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  FCH_SMM_MISC_NODE                   *CurrentFchSmmMiscNodePtr;
  FCH_SMM_COMMUNICATION_BUFFER        *CommunicationBufferPtr;
  UINT16 MiscSmiDispatched;
  MiscSmiDispatched = 0;
  Status = EFI_NOT_FOUND;

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    CommunicationBufferPtr = (FCH_SMM_COMMUNICATION_BUFFER *)CommunicationBuffer;
    while (CurrentFchSmmMiscNodePtr->FchMiscNodePtr!= NULL) {
      if (CurrentFchSmmMiscNodePtr->Context.SmiStatusReg == CommunicationBufferPtr->SmiStatusReg &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusBit == CommunicationBufferPtr->SmiStatusBit) {
        ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + CommunicationBufferPtr->SmiStatusReg) = CommunicationBufferPtr->SmiStatusBit;
        Status = CurrentFchSmmMiscNodePtr->CallBackFunction (
                                            CurrentFchSmmMiscNodePtr->DispatchHandle,
                                            &CurrentFchSmmMiscNodePtr->Context
                                            );
        MiscSmiDispatched++;
      }
      CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }
    if (MiscSmiDispatched <= 0) {
      Status = EFI_NOT_FOUND;
    } else {
      Status = EFI_SUCCESS;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register MISC child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   MiscRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmMiscDispatchRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_MISC_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_MISC_REGISTER_CONTEXT          *MiscRegisterContext,
     OUT   EFI_HANDLE                             *DispatchHandle
  )
{
  EFI_STATUS            Status;
  FCH_SMM_MISC_NODE     *NewFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE     *CurrentFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE     *PreviousFchSmmMiscNodePtr;

  Status = EFI_OUT_OF_RESOURCES;
  if (CallBackFunction == NULL || MiscRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = mSmstPtr->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_MISC_NODE),
                       &NewFchSmmMiscNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmMiscNodePtr->CallBackFunction = CallBackFunction;
  *DispatchHandle = &NewFchSmmMiscNodePtr->DispatchHandle;
  NewFchSmmMiscNodePtr->DispatchHandle   = *DispatchHandle;
  NewFchSmmMiscNodePtr->Context          = *MiscRegisterContext;
  NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;
    HeadFchSmmMiscNodePtr = NewFchSmmMiscNodePtr;
  } else {
    PreviousFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    while (CurrentFchSmmMiscNodePtr->FchMiscNodePtr != NULL) {
      if (NewFchSmmMiscNodePtr->Context.Order <= CurrentFchSmmMiscNodePtr->Context.Order &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusReg == NewFchSmmMiscNodePtr->Context.SmiStatusReg &&
          CurrentFchSmmMiscNodePtr->Context.SmiStatusBit == NewFchSmmMiscNodePtr->Context.SmiStatusBit) {
        if (PreviousFchSmmMiscNodePtr == CurrentFchSmmMiscNodePtr) {
          NewFchSmmMiscNodePtr->FchMiscNodePtr = HeadFchSmmMiscNodePtr;
          HeadFchSmmMiscNodePtr = NewFchSmmMiscNodePtr;
          Status = EFI_SUCCESS;
          return  Status;
        }
        NewFchSmmMiscNodePtr->FchMiscNodePtr = PreviousFchSmmMiscNodePtr->FchMiscNodePtr;
        PreviousFchSmmMiscNodePtr->FchMiscNodePtr = NewFchSmmMiscNodePtr;

        Status = EFI_SUCCESS;
        return  Status;
      }
      PreviousFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr;
      CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }
    PreviousFchSmmMiscNodePtr->FchMiscNodePtr = NewFchSmmMiscNodePtr;
    NewFchSmmMiscNodePtr->FchMiscNodePtr = CurrentFchSmmMiscNodePtr;
  }
  Status = EFI_SUCCESS;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister MISC child handler
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
FchSmmMiscDispatchUnRegister (
  IN       CONST FCH_SMM_MISC_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                             DispatchHandle
  )
{
  EFI_STATUS        Status;
  FCH_SMM_MISC_NODE *CurrentFchSmmMiscNodePtr;
  FCH_SMM_MISC_NODE *PreviousFchSmmMiscNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmMiscNodePtr->FchMiscNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    CurrentFchSmmMiscNodePtr = HeadFchSmmMiscNodePtr;
    if (CurrentFchSmmMiscNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    } else {
      while (CurrentFchSmmMiscNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr;
        CurrentFchSmmMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
        if (CurrentFchSmmMiscNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmMiscNodePtr->FchMiscNodePtr = CurrentFchSmmMiscNodePtr->FchMiscNodePtr;
    }
    Status = mSmstPtr->SmmFreePool (
                       CurrentFchSmmMiscNodePtr
                       );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

