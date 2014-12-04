/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD FCH SMM Sleep Type Dispatcher Driver
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

#include "FchSmmSxDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMSXDISPATCHER_FCHSMMSXDISPATCHER_FILECODE

EFI_STATUS
EFIAPI
FchSmmSxDispatchRegister (
  IN       CONST FCH_SMM_SX_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_SX_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_SX_REGISTER_CONTEXT          *RegisterContext,
  OUT      EFI_HANDLE                           *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmSxDispatchUnRegister (
  IN       CONST FCH_SMM_SX_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                           DispatchHandle
  );

FCH_SMM_SX_DISPATCH_PROTOCOL gFchSmmSxDispatchProtocol = {
  FchSmmSxDispatchRegister,
  FchSmmSxDispatchUnRegister
};

EFI_STATUS
EFIAPI
FchSmmSxDispatch2Register (
  IN       CONST FCH_SMM_SX_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_SX_HANDLER2_ENTRY_POINT      CallBack2Function,
  IN OUT   FCH_SMM_SX_REGISTER_CONTEXT          *RegisterContext,
  OUT      EFI_HANDLE                           *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmSxDispatch2UnRegister (
  IN       CONST FCH_SMM_SX_DISPATCH2_PROTOCOL   *This,
  IN       EFI_HANDLE                           DispatchHandle
  );

FCH_SMM_SX_DISPATCH2_PROTOCOL gFchSmmSxDispatch2Protocol = {
  FchSmmSxDispatch2Register,
  FchSmmSxDispatch2UnRegister
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM Sleep Type dispatcher handler (Stack)
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
FchSmmSxDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS      Status;
  UINT16          AcpiPmCntBase;
  UINT16          SleepTypeValue;
  UINT16          SetSleepTypeValue;
  UINT16          SleepTypeDispatched;
  FCH_SMM_SX_NODE *CurrentFchSmmSxNodePtr;
  Status = EFI_UNSUPPORTED;
  SleepTypeDispatched = 0;


  AcpiPmCntBase = ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG62);
  LibAmdIoRead (AccessWidth16, AcpiPmCntBase, &SleepTypeValue, NULL);

  SetSleepTypeValue = SleepTypeValue;
  SleepTypeValue = (SleepTypeValue >> 10) & 0x7;

  if (HeadFchSmmSxNodePtr->FchSxNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    while (CurrentFchSmmSxNodePtr->FchSxNodePtr != NULL) {
      if (CurrentFchSmmSxNodePtr->Context.Type == SleepTypeValue &&
          CurrentFchSmmSxNodePtr->Context.Phase == AmdSxEntry) {
        if (CurrentFchSmmSxNodePtr->CallBackFunction != NULL) {
          Status = CurrentFchSmmSxNodePtr->CallBackFunction (
                                          CurrentFchSmmSxNodePtr->DispatchHandle,
                                          &CurrentFchSmmSxNodePtr->Context
                                          );
        }
        if (CurrentFchSmmSxNodePtr->CallBack2Function != NULL) {
          Status = CurrentFchSmmSxNodePtr->CallBack2Function (
                                          CurrentFchSmmSxNodePtr->DispatchHandle,
                                          &CurrentFchSmmSxNodePtr->Context,
                                          NULL,
                                          NULL
                                          );
          ASSERT_EFI_ERROR (Status);
        }
        ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) = Slp_Type;
        SleepTypeDispatched++;
      }
      CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    }
    if (SleepTypeDispatched == 0) {
      Status = EFI_NOT_FOUND;
    } else {
      Status = EFI_SUCCESS;
      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) |= BIT5;
      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT0;

      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);

      SetSleepTypeValue &= (BIT10 + BIT11 + BIT12);
      LibAmdIoWrite (AccessWidth16, AcpiPmCntBase, &SetSleepTypeValue, NULL);

      SetSleepTypeValue |= BIT13;
      LibAmdIoWrite (AccessWidth16, AcpiPmCntBase, &SetSleepTypeValue, NULL);

      CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
      while (CurrentFchSmmSxNodePtr->FchSxNodePtr != NULL) {
        if (CurrentFchSmmSxNodePtr->Context.Phase == AmdSxExit) {
          if (CurrentFchSmmSxNodePtr->CallBackFunction != NULL) {
            Status = CurrentFchSmmSxNodePtr->CallBackFunction (
                                          CurrentFchSmmSxNodePtr->DispatchHandle,
                                          &CurrentFchSmmSxNodePtr->Context
                                          );
          }
          if (CurrentFchSmmSxNodePtr->CallBack2Function != NULL) {
            Status = CurrentFchSmmSxNodePtr->CallBack2Function (
                                          CurrentFchSmmSxNodePtr->DispatchHandle,
                                          &CurrentFchSmmSxNodePtr->Context,
                                          NULL,
                                          NULL
                                          );
          }
        }
        CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
      }

      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT2;

      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) &= ~ (BIT5);
      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
      ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT1;

      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) |= Eos;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register Sleep Type child handler
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
FchSmmSxDispatchRegister (
  IN       CONST FCH_SMM_SX_DISPATCH_PROTOCOL   *This,
  IN       FCH_SMM_SX_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_SX_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SX_NODE *NewFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *CurrentFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *PreviousFchSmmSxNodePtr;

  Status = EFI_OUT_OF_RESOURCES;

  if (CallBackFunction == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = mSmstPtr->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_SX_NODE),
                       &NewFchSmmSxNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3); //Enable Sleep type Smi
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT2;

  NewFchSmmSxNodePtr->CallBackFunction = CallBackFunction;
  NewFchSmmSxNodePtr->CallBack2Function = NULL;
  NewFchSmmSxNodePtr->Context = *RegisterContext;
  *DispatchHandle = &NewFchSmmSxNodePtr->DispatchHandle;
  NewFchSmmSxNodePtr->DispatchHandle = *DispatchHandle;

  if (HeadFchSmmSxNodePtr->FchSxNodePtr == NULL) {
    NewFchSmmSxNodePtr->FchSxNodePtr = HeadFchSmmSxNodePtr;
    HeadFchSmmSxNodePtr = NewFchSmmSxNodePtr;
  } else {
    PreviousFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    while (CurrentFchSmmSxNodePtr->FchSxNodePtr != NULL) {
      if (NewFchSmmSxNodePtr->Context.Order <= CurrentFchSmmSxNodePtr->Context.Order &&
          CurrentFchSmmSxNodePtr->Context.Type == NewFchSmmSxNodePtr->Context.Type &&
          CurrentFchSmmSxNodePtr->Context.Phase == NewFchSmmSxNodePtr->Context.Phase ) {

        if (PreviousFchSmmSxNodePtr == CurrentFchSmmSxNodePtr) {
          NewFchSmmSxNodePtr->FchSxNodePtr = HeadFchSmmSxNodePtr;
          HeadFchSmmSxNodePtr = NewFchSmmSxNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmSxNodePtr->FchSxNodePtr = PreviousFchSmmSxNodePtr->FchSxNodePtr;
        PreviousFchSmmSxNodePtr->FchSxNodePtr = NewFchSmmSxNodePtr;

        Status = EFI_SUCCESS;
        return Status;
      }
      PreviousFchSmmSxNodePtr = CurrentFchSmmSxNodePtr;
      CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    }
    PreviousFchSmmSxNodePtr->FchSxNodePtr = NewFchSmmSxNodePtr;
    NewFchSmmSxNodePtr->FchSxNodePtr = CurrentFchSmmSxNodePtr;
  }
  Status = EFI_SUCCESS;
  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister Sleep Type child handler
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
FchSmmSxDispatchUnRegister (
  IN       CONST FCH_SMM_SX_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                           DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SX_NODE *CurrentFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *PreviousFchSmmSxNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (HeadFchSmmSxNodePtr->FchSxNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    if (CurrentFchSmmSxNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    } else {
      while (CurrentFchSmmSxNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmSxNodePtr = CurrentFchSmmSxNodePtr;
        CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
        if (CurrentFchSmmSxNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmSxNodePtr->FchSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    }

    Status = mSmstPtr->SmmFreePool (
                         CurrentFchSmmSxNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register Sleep Type child handler2
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBack2Function     Callback function
 * @param[in, out]   RegisterContext            Register contecxt (see PI 1.1 for more details)
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmSxDispatch2Register (
  IN       CONST FCH_SMM_SX_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_SX_HANDLER2_ENTRY_POINT      CallBack2Function,
  IN OUT   FCH_SMM_SX_REGISTER_CONTEXT          *RegisterContext,
     OUT   EFI_HANDLE                           *DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SX_NODE *NewFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *CurrentFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *PreviousFchSmmSxNodePtr;

  Status = EFI_OUT_OF_RESOURCES;

  if (CallBack2Function == NULL || RegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = mSmstPtr->SmmAllocatePool (
                       EfiRuntimeServicesData,
                       sizeof (FCH_SMM_SX_NODE),
                       &NewFchSmmSxNodePtr
                       );
  ASSERT_EFI_ERROR (Status);

  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3); //Enable Sleep type Smi
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT2;

  NewFchSmmSxNodePtr->CallBackFunction = NULL;
  NewFchSmmSxNodePtr->CallBack2Function = CallBack2Function;
  NewFchSmmSxNodePtr->Context = *RegisterContext;
  *DispatchHandle = &NewFchSmmSxNodePtr->DispatchHandle;
  NewFchSmmSxNodePtr->DispatchHandle = *DispatchHandle;

  if (HeadFchSmmSxNodePtr->FchSxNodePtr == NULL) {
    NewFchSmmSxNodePtr->FchSxNodePtr = HeadFchSmmSxNodePtr;
    HeadFchSmmSxNodePtr = NewFchSmmSxNodePtr;
  } else {
    PreviousFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    while (CurrentFchSmmSxNodePtr->FchSxNodePtr != NULL) {
      if (NewFchSmmSxNodePtr->Context.Order <= CurrentFchSmmSxNodePtr->Context.Order &&
          CurrentFchSmmSxNodePtr->Context.Type == NewFchSmmSxNodePtr->Context.Type &&
          CurrentFchSmmSxNodePtr->Context.Phase == NewFchSmmSxNodePtr->Context.Phase ) {

        if (PreviousFchSmmSxNodePtr == CurrentFchSmmSxNodePtr) {
          NewFchSmmSxNodePtr->FchSxNodePtr = HeadFchSmmSxNodePtr;
          HeadFchSmmSxNodePtr = NewFchSmmSxNodePtr;
          Status = EFI_SUCCESS;
          return Status;
        }
        NewFchSmmSxNodePtr->FchSxNodePtr = PreviousFchSmmSxNodePtr->FchSxNodePtr;
        PreviousFchSmmSxNodePtr->FchSxNodePtr = NewFchSmmSxNodePtr;

        Status = EFI_SUCCESS;
        return Status;
      }
      PreviousFchSmmSxNodePtr = CurrentFchSmmSxNodePtr;
      CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    }
    PreviousFchSmmSxNodePtr->FchSxNodePtr = NewFchSmmSxNodePtr;
    NewFchSmmSxNodePtr->FchSxNodePtr = CurrentFchSmmSxNodePtr;
  }
  Status = EFI_SUCCESS;
  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister Sleep Type child handler2
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
FchSmmSxDispatch2UnRegister (
  IN       CONST FCH_SMM_SX_DISPATCH2_PROTOCOL   *This,
  IN       EFI_HANDLE                            DispatchHandle
  )
{
  EFI_STATUS      Status;
  FCH_SMM_SX_NODE *CurrentFchSmmSxNodePtr;
  FCH_SMM_SX_NODE *PreviousFchSmmSxNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (HeadFchSmmSxNodePtr->FchSxNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    CurrentFchSmmSxNodePtr = HeadFchSmmSxNodePtr;
    if (CurrentFchSmmSxNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    } else {
      while (CurrentFchSmmSxNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmSxNodePtr = CurrentFchSmmSxNodePtr;
        CurrentFchSmmSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
        if (CurrentFchSmmSxNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmSxNodePtr->FchSxNodePtr = CurrentFchSmmSxNodePtr->FchSxNodePtr;
    }

    Status = mSmstPtr->SmmFreePool (
                         CurrentFchSmmSxNodePtr
                         );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


