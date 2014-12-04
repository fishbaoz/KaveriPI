/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH SMM Driver
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
#include "FchSmm.h"
#define FILECODE UEFI_SMM_FCHSMM_SWSMI_FILECODE

extern FCH_INIT_PROTOCOL                gFchInitInSmm;
extern  UINT8                           *mFchPciIrqRoutingTable;


UINT16
FchGetPmBase (
  VOID
  )
{
  UINT8       Lsb;
  UINT8       Msb;

  LibFchSmmPmioRead8 (&gSmst->SmmIo, FCH_PMIOA_REG60, &Lsb);
  LibFchSmmPmioRead8 (&gSmst->SmmIo, FCH_PMIOA_REG60 + 1, &Msb);
  return (UINT16) ((Msb << 8) + Lsb);
}


/*++

Routine Description:
  SMI handler to restore ACPI mode

Arguments:

  DispatchHandle  - The handle of this callback, obtained when registering
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:

  None.

--*/
EFI_STATUS
EFIAPI
AmdSmiBeforePciS3RestoreCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FCH_DATA_BLOCK      *pFchPolicy;
  EFI_STATUS          Status;
  UINT8               Index;
  UINT8               *pData;

  Status = EFI_SUCCESS;

  // Restore entire FCH PCI IRQ routing space (C00/C01)
  pData = mFchPciIrqRoutingTable;
  Index = 0xFF;
  do {
    Index++;
    LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC00, &Index);
    LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC01, pData++);
  } while (Index != 0xFF);

  pFchPolicy = &gFchInitInSmm.FchPolicy;
  FchInitS3EarlyRestore (pFchPolicy);
  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiAfterPciS3RestoreCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FCH_DATA_BLOCK      *pFchPolicy;
  EFI_STATUS          Status;

  Status = EFI_SUCCESS;
  pFchPolicy = &gFchInitInSmm.FchPolicy;
  FchInitS3LateRestore (pFchPolicy);
  return Status;
}


EFI_STATUS
EFIAPI
AmdSmiAcpiOnCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FCH_DATA_BLOCK      *pFchPolicy;
  UINT16              AcpiPmbase;
  UINT8               dbIndex;
  UINT8               dbIrq;
  UINT32              ddValue;

  AcpiPmbase = FchGetPmBase ();

  pFchPolicy = &gFchInitInSmm.FchPolicy;
  FchSmmAcpiOn (pFchPolicy);

  // Disable all GPE events and clear all GPE status
  ddValue = 0;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_EVENT_ENABLE, &ddValue);
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_EVENT_STATUS, &ddValue);

  // Set ACPI IRQ to IRQ9 for non-APIC OSes
  dbIndex = 0x10;                                          // PIC - SCI
  dbIrq = 9;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC00, &dbIndex);
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC01, &dbIrq);

  dbIndex |= BIT7;                                         // IOAPIC - SCI
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC00, &dbIndex);
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGC01, &dbIrq);

  // Finally enable SCI
  LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &ddValue);
  ddValue |= BIT0;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &ddValue);

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
AmdSmiAcpiOffCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINT16      AcpiPmbase;
  UINT32      ddValue;

  AcpiPmbase = FchGetPmBase ();

  // Turn off SCI
  LibFchSmmIoRead (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &ddValue);
  ddValue &= ~BIT0;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT32, AcpiPmbase + R_FCH_ACPI_PM_CONTROL, &ddValue);

  return EFI_SUCCESS;
}

#ifdef FCH_SPI_PROTECT_SMI
EFI_STATUS
EFIAPI
AmdSmiSpiUnlockCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FCH_DATA_BLOCK      *pFchPolicy;
  EFI_STATUS          Status;

  Status = EFI_SUCCESS;
  pFchPolicy = &gFchInitInSmm.FchPolicy;
  FchSpiUnlock (pFchPolicy);
  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiSpiLockCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  FCH_DATA_BLOCK      *pFchPolicy;
  EFI_STATUS          Status;

  Status = EFI_SUCCESS;
  pFchPolicy = &gFchInitInSmm.FchPolicy;
  FchSpiLock (pFchPolicy);
  return Status;
}
#endif

STATIC UINT8          SavePort80 = 0x00;

EFI_STATUS
EFIAPI
AmdStartTimerSmiCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  SavePort80 = ACPIMMIO8 (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG44);

  ACPIMMIO16 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG96) |= SMI_TIMER_ENABLE;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AmdStopTimerSmiCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  ACPIMMIO16 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG96) &= ~SMI_TIMER_ENABLE;
  LibFchSmmIoWrite (&gSmst->SmmIo, SMM_IO_UINT8, 0x80, &SavePort80);
  return EFI_SUCCESS;
}




EFI_STATUS
FchSmmRegisterSwSmi (
  VOID
  )
{
  EFI_STATUS                               Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL            *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT              SwRegisterContext;
  EFI_HANDLE                               SwHandle;
  FCH_MISC                                 *FchMisc;

  FchMisc = &gFchInitInSmm.FchPolicy.Misc;

  //
  //  Locate SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                  &gFchSmmSwDispatch2ProtocolGuid,
                  NULL,
                  &AmdSwDispatch
                  );
  ASSERT_EFI_ERROR (Status);

  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemBeforePciRestoreSwSmi); // use of PCD in place of FCHOEM_BEFORE_PCI_RESTORE_SWSMI    0xD3
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiBeforePciS3RestoreCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemAfterPciRestoreSwSmi); // use of PCD in place of FCHOEM_AFTER_PCI_RESTORE_SWSMI    0xD4
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiAfterPciS3RestoreCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemEnableAcpiSwSmi); // use of PCD in place of FCHOEM_ENABLE_ACPI_SWSMI           0xA0
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiAcpiOnCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemDisableAcpiSwSmi); // use of PCD in place of FCHOEM_DISABLE_ACPI_SWSMI          0xA1
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiAcpiOffCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

#ifdef FCH_SPI_PROTECT_SMI
  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemSpiUnlockSwSmi); // use of PCD in place of FCHOEM_SPI_UNLOCK_SWSMI            0xAA
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiSpiUnlockCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemSpiLockSwSmi); // use of PCD in place of FCHOEM_SPI_LOCK_SWSMI              0xAB
  SwRegisterContext.Order       = 0x80;
  Status = AmdSwDispatch->Register (
                            AmdSwDispatch,
                            AmdSmiSpiLockCallback,
                            &SwRegisterContext,
                            &SwHandle
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }
#endif

  if (FchMisc->LongTimer.Enable || FchMisc->ShortTimer.Enable) {
    SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemStartTimerSmi); // use of PCD in place of FCHOEM_START_TIMER_SMI             0xBC
    SwRegisterContext.Order       = 0x80;
    Status = AmdSwDispatch->Register (
                              AmdSwDispatch,
                              AmdStartTimerSmiCallback,
                              &SwRegisterContext,
                              &SwHandle
                              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    SwRegisterContext.AmdSwValue  = PcdGet8(PcdFchOemStopTimerSmi); // use of PCD in place of FCHOEM_STOP_TIMER_SMI              0xBD
    SwRegisterContext.Order       = 0x80;
    Status = AmdSwDispatch->Register (
                              AmdSwDispatch,
                              AmdStopTimerSmiCallback,
                              &SwRegisterContext,
                              &SwHandle
                              );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

