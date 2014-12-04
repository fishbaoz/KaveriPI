/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 GPE Dispatcher Driver
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

#include "FchSmmGpeDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMGPEDISPATCHER_FCHSMMGPEDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmGpeDispatchRegister (
  IN       CONST FCH_SMM_GPE_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_GPE_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_GPE_REGISTER_CONTEXT          *GpeRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmGpeDispatchUnRegister (
  IN       CONST FCH_SMM_GPE_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                            DispatchHandle
  );

FCH_SMM_GPE_DISPATCH_PROTOCOL gFchSmmGpeDispatchProtocol = {
  FchSmmGpeDispatchRegister,
  FchSmmGpeDispatchUnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM GPE dispatcher handler
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
FchSmmGpeDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS  Status;
  FCH_SMM_GPE_NODE *CurrentFchSmmGpeNodePtr;
  UINT16 GpeSmiDispatched;
  UINT32 GpeStatus;
  UINT32 GpeEnable;
  UINT32 GpeStatusNum;
  GpeSmiDispatched = 0;
  Status = EFI_NOT_FOUND;

  if (HeadFchSmmGpeNodePtr->FchGpeNodePtr == NULL) {
    GpeStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = GpeStatus;
    Status = EFI_NOT_FOUND;
  } else {
    GpeStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    GpeEnable = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14);
    GpeStatus &= GpeEnable;
    if (GpeStatus == 0) {
      Status = EFI_NOT_FOUND;
    } else {
      GpeStatusNum = 1;
      while ((GpeStatus & BIT0) != 1) {
        GpeStatus >>= 1;
        GpeStatusNum <<= 1;
      }
      CurrentFchSmmGpeNodePtr = HeadFchSmmGpeNodePtr;
      while (CurrentFchSmmGpeNodePtr->FchGpeNodePtr!= NULL) {
        if (CurrentFchSmmGpeNodePtr->Context.AmdGpeNum == GpeStatusNum) {
          Status = CurrentFchSmmGpeNodePtr->CallBackFunction (
                                             CurrentFchSmmGpeNodePtr->DispatchHandle,
                                             &CurrentFchSmmGpeNodePtr->Context
                                             );
          GpeSmiDispatched++;
          break;
        }
        CurrentFchSmmGpeNodePtr = CurrentFchSmmGpeNodePtr->FchGpeNodePtr;
      }
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = GpeStatusNum;

      if (GpeSmiDispatched <= 0) {
        Status = EFI_NOT_FOUND;
      } else {
        Status = EFI_SUCCESS;
      }
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register GPE child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   GpeRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmGpeDispatchRegister (
  IN       CONST FCH_SMM_GPE_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_GPE_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_GPE_REGISTER_CONTEXT          *GpeRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_GPE_NODE *NewFchSmmGpeNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || GpeRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = mSmstPtr->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_GPE_NODE),
                       &NewFchSmmGpeNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  NewFchSmmGpeNodePtr->CallBackFunction = CallBackFunction;
  *DispatchHandle = &NewFchSmmGpeNodePtr->DispatchHandle;
  NewFchSmmGpeNodePtr->DispatchHandle = *DispatchHandle;
  NewFchSmmGpeNodePtr->Context = *GpeRegisterContext;
  NewFchSmmGpeNodePtr->FchGpeNodePtr = HeadFchSmmGpeNodePtr;
  HeadFchSmmGpeNodePtr = NewFchSmmGpeNodePtr;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) |= NewFchSmmGpeNodePtr->Context.AmdGpeNum;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister GPE child handler
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
FchSmmGpeDispatchUnRegister (
  IN       CONST FCH_SMM_GPE_DISPATCH_PROTOCOL      *This,
  IN       EFI_HANDLE                               DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_GPE_NODE *CurrentFchSmmGpeNodePtr;
  FCH_SMM_GPE_NODE *PreviousFchSmmGpeNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmGpeNodePtr->FchGpeNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmGpeNodePtr = HeadFchSmmGpeNodePtr;
    CurrentFchSmmGpeNodePtr = HeadFchSmmGpeNodePtr;
    if (CurrentFchSmmGpeNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmGpeNodePtr = CurrentFchSmmGpeNodePtr->FchGpeNodePtr;
    } else {
      while (CurrentFchSmmGpeNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmGpeNodePtr = CurrentFchSmmGpeNodePtr;
        CurrentFchSmmGpeNodePtr = CurrentFchSmmGpeNodePtr->FchGpeNodePtr;
        if (CurrentFchSmmGpeNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmGpeNodePtr->FchGpeNodePtr = CurrentFchSmmGpeNodePtr->FchGpeNodePtr;
    }
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) &= ~(CurrentFchSmmGpeNodePtr->Context.AmdGpeNum);
    Status = mSmstPtr->SmmFreePool (
                         CurrentFchSmmGpeNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

