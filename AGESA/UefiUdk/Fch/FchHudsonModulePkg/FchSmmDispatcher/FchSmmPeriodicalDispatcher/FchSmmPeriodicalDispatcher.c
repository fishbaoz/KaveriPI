/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 PERIODICAL Dispatcher Driver
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

#include "FchSmmPeriodicalDispatcher.h"
#include "Filecode.h"
#define FILECODE UEFI_SMM_FCHSMMDISPATCHER_FCHSMMPERIODICALDISPATCHER_FCHSMMPERIODICALDISPATCHER_FILECODE


UINT64  SmiIntervalPeriod[2] = {LONG_TIMER_SMI_INTERVAL, SHORT_TIMER_SMI_INTERVAL};

EFI_STATUS
EFIAPI
FchSmmPeriodicalDispatch2Register (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PERIODICAL_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_PERIODICAL_REGISTER_CONTEXT    *PeriodicalRegisterContext,
     OUT   EFI_HANDLE                                   *DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmPeriodicalDispatch2UnRegister (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                                   DispatchHandle
  );

EFI_STATUS
EFIAPI
FchSmmGetNextShorterInterval (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL  *This,
  IN OUT   UINT64                                       **SmiTickInterval
  );

FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL gFchSmmPeriodicalDispatch2Protocol = {
  FchSmmPeriodicalDispatch2Register,
  FchSmmPeriodicalDispatch2UnRegister,
  FchSmmGetNextShorterInterval
};

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM PERIODICAL dispatcher handler
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
FchSmmPeriodicalDispatchHandler (
  IN       EFI_HANDLE   SmmImageHandle,
  IN OUT   VOID         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN        *SourceSize OPTIONAL
  )
{
  EFI_STATUS                Status;
  FCH_SMM_PERIODICAL_NODE   *CurrentFchSmmPeriodicalNodePtr;
  UINT16                    PeriodicalSmiDispatched;
  UINTN                     SizeOfContext;

  PeriodicalSmiDispatched = 0;
  Status = EFI_NOT_FOUND;

  SizeOfContext = sizeof (EFI_SMM_PERIODIC_TIMER_CONTEXT);
  EfiSmmPeriodicTimerContext.ElapsedTime = ELAPSED_TIME_UNKNOWN;
  if (HeadFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
  } else {
    CurrentFchSmmPeriodicalNodePtr = HeadFchSmmPeriodicalNodePtr;
    while (CurrentFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr!= NULL) {
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90) = ( ShortTimer + LongTimer);
      Status = CurrentFchSmmPeriodicalNodePtr->CallBack2Function (
                                                 CurrentFchSmmPeriodicalNodePtr->DispatchHandle,
                                                 &CurrentFchSmmPeriodicalNodePtr->Context,
                                                 &EfiSmmPeriodicTimerContext,
                                                 &SizeOfContext
                                                 );
      PeriodicalSmiDispatched++;
      CurrentFchSmmPeriodicalNodePtr = CurrentFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr;
    }
    if (PeriodicalSmiDispatched <= 0) {
      Status = EFI_NOT_FOUND;
    } else {
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG90) = ( ShortTimer + LongTimer);
      Status = EFI_SUCCESS;
    }
  }
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Register PERIODICAL child handler
 *
 *
 * @param[in]       This                  Pointer to protocol
 * @param[in]       CallBackFunction
 * @param[in, out]   PeriodicalRegisterContext
 * @param[out]      DispatchHandle        Handle (see PI 1.1 for more details)
 *
 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmPeriodicalDispatch2Register (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL  *This,
  IN       FCH_SMM_PERIODICAL_HANDLER_ENTRY_POINT2      CallBackFunction,
  IN       CONST FCH_SMM_PERIODICAL_REGISTER_CONTEXT    *PeriodicalRegisterContext,
     OUT   EFI_HANDLE                                   *DispatchHandle
  )
{
  EFI_STATUS               Status;
  FCH_SMM_PERIODICAL_NODE  *NewFchSmmPeriodicalNodePtr;
  UINT64                   SmiTicks;

  if (CallBackFunction == NULL || PeriodicalRegisterContext == NULL || DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPeriodicalNodePtr->CallBack2Function != NULL) {
    return EFI_OUT_OF_RESOURCES;
  } else {
    Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      sizeof (FCH_SMM_PERIODICAL_NODE),
                      &NewFchSmmPeriodicalNodePtr
                      );
    ASSERT_EFI_ERROR (Status);

    DEBUG ((DEBUG_INFO, "[FchSmmDispatcher] Registering PeriodicTimer SMM handler: Period = 0x%d, TickInterval = 0x%d, StartNow = %x\n", \
           PeriodicalRegisterContext->Period, PeriodicalRegisterContext->SmiTickInterval, PeriodicalRegisterContext->StartNow));
    NewFchSmmPeriodicalNodePtr->CallBack2Function = CallBackFunction;
    *DispatchHandle = &NewFchSmmPeriodicalNodePtr->DispatchHandle;
    NewFchSmmPeriodicalNodePtr->DispatchHandle = *DispatchHandle;
    NewFchSmmPeriodicalNodePtr->Context = *PeriodicalRegisterContext;
    NewFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr = HeadFchSmmPeriodicalNodePtr;
    HeadFchSmmPeriodicalNodePtr = NewFchSmmPeriodicalNodePtr;

    SmiTicks = NewFchSmmPeriodicalNodePtr->Context.SmiTickInterval;
    if (SmiTicks == LONG_TIMER_SMI_INTERVAL) {
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) |= BIT29;
    } else if (SmiTicks == SHORT_TIMER_SMI_INTERVAL) {
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~BIT29;
    } else {
      return EFI_INVALID_PARAMETER;
    }
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC3) |= BIT4;
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC3) |= BIT6;
    ACPIMMIO16 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG96) = (UINT16) (NewFchSmmPeriodicalNodePtr->Context.Period / SmiTicks) & 0x7FFF;
    if (NewFchSmmPeriodicalNodePtr->Context.StartNow) {
      ACPIMMIO16 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG96) |= SMI_TIMER_ENABLE;
    }
    return  Status;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Unregister PERIODICAL child handler
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
FchSmmPeriodicalDispatch2UnRegister (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                                   DispatchHandle
  )
{
  EFI_STATUS  Status;
  FCH_SMM_PERIODICAL_NODE *CurrentFchSmmPeriodicalNodePtr;
  FCH_SMM_PERIODICAL_NODE *PreviousFchSmmPeriodicalNodePtr;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (HeadFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr == NULL) {
    Status = EFI_NOT_FOUND;
    return  Status;
  } else {
    PreviousFchSmmPeriodicalNodePtr = HeadFchSmmPeriodicalNodePtr;
    CurrentFchSmmPeriodicalNodePtr = HeadFchSmmPeriodicalNodePtr;
    if (CurrentFchSmmPeriodicalNodePtr->DispatchHandle == DispatchHandle) {
      HeadFchSmmPeriodicalNodePtr = CurrentFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr;
    } else {
      while (CurrentFchSmmPeriodicalNodePtr->DispatchHandle != DispatchHandle) {
        PreviousFchSmmPeriodicalNodePtr = CurrentFchSmmPeriodicalNodePtr;
        CurrentFchSmmPeriodicalNodePtr = CurrentFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr;
        if (CurrentFchSmmPeriodicalNodePtr->DispatchHandle == NULL) {
          Status = EFI_NOT_FOUND;
          return  Status;
        }
      }
      PreviousFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr = CurrentFchSmmPeriodicalNodePtr->FchPeriodicalNodePtr;
    }
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC3) &= ~BIT4;
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGC3) &= ~BIT6;
    Status = gSmst->SmmFreePool (
                      CurrentFchSmmPeriodicalNodePtr
                      );
    ASSERT_EFI_ERROR (Status);
  }
  Status = EFI_SUCCESS;
  return  Status;
}


EFI_STATUS
EFIAPI
FchSmmGetNextShorterInterval (
  IN       CONST FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL *This,
  IN OUT   UINT64                                      **SmiTickInterval
  )
{
  //
  // This function returns the next SMI tick period that is supported by the chipset.
  //
  ASSERT (SmiTickInterval != NULL);
  *SmiTickInterval = (*SmiTickInterval == NULL) ? &SmiIntervalPeriod[0] : &SmiIntervalPeriod[1];
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
EfiSmmPeriodicalDispatch2Register (
  IN       CONST EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2                    DispatchFunction,
  IN       CONST EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                                      *DispatchHandle
  )
{
  FCH_SMM_PERIODICAL_REGISTER_CONTEXT    FchRegisterContext;

  FchRegisterContext.Period = RegisterContext->Period;
  FchRegisterContext.SmiTickInterval = RegisterContext->SmiTickInterval;
  FchRegisterContext.StartNow = TRUE;
  return FchSmmPeriodicalDispatch2Register (
           &gFchSmmPeriodicalDispatch2Protocol,
           DispatchFunction,
           &FchRegisterContext,
           DispatchHandle
           );
}


EFI_STATUS
EFIAPI
EfiSmmPeriodicalDispatch2UnRegister (
  IN       CONST EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL *This,
  IN       EFI_HANDLE                                      DispatchHandle
  )
{
  return FchSmmPeriodicalDispatch2UnRegister (
           &gFchSmmPeriodicalDispatch2Protocol,
           DispatchHandle
           );
}


EFI_STATUS
EFIAPI
EfiSmmGetNextShorterInterval (
  IN       CONST EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL *This,
  IN OUT   UINT64                                          **SmiTickInterval
  )
{
  return FchSmmGetNextShorterInterval (
           &gFchSmmPeriodicalDispatch2Protocol,
           SmiTickInterval
           );
}


EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL gEfiSmmPeriodicalDispatch2Protocol = {
  EfiSmmPeriodicalDispatch2Register,
  EfiSmmPeriodicalDispatch2UnRegister,
  EfiSmmGetNextShorterInterval
};

