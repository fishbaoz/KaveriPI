/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 GPI SMM Dispatcher Driver
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

#include "FchSmmGpiDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMGPEDISPATCHER_FCHSMMGPEDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmGpiDispatch2Register (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_GPI_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT    *GpiRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmGpiDispatch2UnRegister (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  );

FCH_SMM_GPI_DISPATCH2_PROTOCOL gFchSmmGpiDispatch2Protocol = {
  FchSmmGpiDispatch2Register,
  FchSmmGpiDispatch2UnRegister,
  MAX_GPI_VALUE
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM GPI dispatcher handler
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
FchSmmGpiDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS        Status;
  FCH_SMM_GPI_NODE  *CurrentFchSmmGpiNodePtr;
  UINT16            GpiSmiDispatched;
  UINT32            GpiStatus;
  UINT32            GpiEnable;
  UINT32            GpiStatusNum;
  UINTN             SizeOfContext;

  GpiSmiDispatched = 0;
  Status = EFI_NOT_FOUND;
  SizeOfContext = sizeof (EFI_SMM_GPI_REGISTER_CONTEXT);

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    GpiStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = GpiStatus;
    Status = EFI_NOT_FOUND;
  } else {
    GpiStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10);
    GpiEnable = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14);
    GpiStatus &= GpiEnable;
    if (GpiStatus == 0) {
      Status = EFI_NOT_FOUND;
    } else {
      GpiStatusNum = 1;
      while ((GpiStatus & BIT0) != 1) {
        GpiStatus >>= 1;
        GpiStatusNum <<= 1;
      }
      CurrentFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
      while (CurrentFchSmmGpiNodePtr->FchGpiNodePtr!= NULL) {
        if (CurrentFchSmmGpiNodePtr->Context.GpiNum == GpiStatusNum) {
          Status = CurrentFchSmmGpiNodePtr->CallBack2Function (
                                             CurrentFchSmmGpiNodePtr->DispatchHandle,
                                             &CurrentFchSmmGpiNodePtr->Context,
                                             &CurrentFchSmmGpiNodePtr->Context,
                                             &SizeOfContext
                                             );
          DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] GPI SMM handler dispatched GpiNum = 0x%x, \
                  return - %r\n", CurrentFchSmmGpiNodePtr->Context.GpiNum, Status));
          GpiSmiDispatched++;
          break;
        }
        CurrentFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
      }
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG10) = GpiStatusNum;

      if (GpiSmiDispatched <= 0) {
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
 * Register GPI child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   GpiRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmGpiDispatch2Register (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_GPI_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT    *GpiRegisterContext,
     OUT   EFI_HANDLE                            *DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_GPI_NODE *NewFchSmmGpiNodePtr;
  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || GpiRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gSmst->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_GPI_NODE),
                       &NewFchSmmGpiNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering GPI SMM handler: GpiNum = 0x%x\n", GpiRegisterContext->GpiNum));
  NewFchSmmGpiNodePtr->CallBack2Function = CallBackFunction;
  *DispatchHandle = &NewFchSmmGpiNodePtr->DispatchHandle;
  NewFchSmmGpiNodePtr->DispatchHandle = *DispatchHandle;
  NewFchSmmGpiNodePtr->Context = *GpiRegisterContext;
  NewFchSmmGpiNodePtr->FchGpiNodePtr = HeadFchSmmGpiNodePtr;
  HeadFchSmmGpiNodePtr = NewFchSmmGpiNodePtr;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) |= NewFchSmmGpiNodePtr->Context.GpiNum;
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister GPI child handler
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
FchSmmGpiDispatch2UnRegister (
  IN       CONST FCH_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_GPI_NODE *CurrentFchSmmGpiNodePtr;
  FCH_SMM_GPI_NODE *PreviousFchSmmGpiNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmGpiNodePtr->FchGpiNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    CurrentFchSmmGpiNodePtr = HeadFchSmmGpiNodePtr;
    if (CurrentFchSmmGpiNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
    } else {
      while (CurrentFchSmmGpiNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr;
        CurrentFchSmmGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
        if (CurrentFchSmmGpiNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmGpiNodePtr->FchGpiNodePtr = CurrentFchSmmGpiNodePtr->FchGpiNodePtr;
    }
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG14) &= ~(CurrentFchSmmGpiNodePtr->Context.GpiNum);
    Status = gSmst->SmmFreePool (
                         CurrentFchSmmGpiNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


EFI_STATUS
EFIAPI
EfiSmmGpiDispatch2Register (
  IN       CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2         DispatchFunction,
  IN       CONST EFI_SMM_GPI_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  return FchSmmGpiDispatch2Register (
           &gFchSmmGpiDispatch2Protocol,
           DispatchFunction,
           RegisterContext,
           DispatchHandle
           );
}

EFI_STATUS
EFIAPI
EfiSmmGpiDispatch2UnRegister (
  IN       CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                           DispatchHandle
  )
{
  return FchSmmGpiDispatch2UnRegister (
           &gFchSmmGpiDispatch2Protocol,
           DispatchHandle
           );
}

EFI_SMM_GPI_DISPATCH2_PROTOCOL gEfiSmmGpiDispatch2Protocol = {
  EfiSmmGpiDispatch2Register,
  EfiSmmGpiDispatch2UnRegister,
  MAX_GPI_VALUE
};

