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

#include "AmdFchWheaDxe.h"

//
// WHEA Serialization Table
//
STATIC
EINJ_INSTRUCTION_ENTRY mAmdFchEinj = {
  EXECUTE_OPERATION,                  // BEGIN_INJECTION_OPERATION
  WRITE_REGISTER_VALUE,               // WRITE_REGISTER_VALUE
  0x01,                               // Flags
  0x00,                               // Reserved
  {0x01, 0x10, 0x00, 0x02, 0xB0},     // GAS (WORD IO). Address will be filled in runtime
  EINJ_TRIGGER_ACTION_SWSMI,          // Value for InjectError()
  0xffff                              // Mask is only valid for 16-bits
};

EFI_STATUS
AmdFchWheaInitEntryEinj (
  VOID
  )
{
  EFI_ACPI_TABLE_PROTOCOL         *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL           *AcpiSdtProtocol;
  EFI_STATUS                      Status;
  INTN                            Index;
  UINTN                           TableKey;
  EFI_ACPI_TABLE_VERSION          Version;
  EFI_ACPI_SDT_HEADER             *Table;
  EFI_ACPI_SDT_HEADER             *NewTable;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Search EINJ table
  //
  Index  = 0;
  do {
    Table  = NULL;
    Status = AcpiSdtProtocol->GetAcpiTable (
                            Index,
                            &Table,
                            &Version,
                            &TableKey
                            );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Check Signture and update EINJ table
    //
    if (Table->Signature == 0x4A4E4945) {
      //
      // allocate memory for new Table
      //
      Status = gBS->AllocatePool (
                      EfiReservedMemoryType,
                      MAX_EINJ_SIZE,
                      &NewTable
                      );
      if (Status != EFI_SUCCESS) {
        return Status;
      }
      ZeroMem (NewTable, MAX_EINJ_SIZE);
      //
      // Copy Table
      //
      CopyMem (NewTable, Table, Table->Length);
      //
      // Update new table
      //
      AmdFchUpdateEinj (NewTable);

      if (!EFI_ERROR (Status)) {
      // Patch EINJ table here and save table back.

        //
        // Remove previous table
        //
        Status = AcpiTableProtocol->UninstallAcpiTable (
                                      AcpiTableProtocol,
                                      TableKey
                                      );
        //
        // Add new table
        //
        Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      NewTable,
                                      (UINTN)NewTable->Length,
                                      &TableKey
                                      );
        gBS->FreePool (NewTable);
        return Status;
      }
      gBS->FreePool (NewTable);
    }
    Index++;
  } while (TRUE);

  return Status;
}

EFI_STATUS
AmdFchUpdateEinj (
  IN OUT   EFI_ACPI_SDT_HEADER              *TableHeader
  )
{
  EFI_STATUS                                Status;
  EINJ_HEADER                               *EinjHeader;
  EINJ_INSTRUCTION_ENTRY                    *FirstEntryPtr;
  EINJ_INSTRUCTION_ENTRY                    *CurrentEntryPtr;
  UINT32                                    EntryCount;

  Status = EFI_SUCCESS;

  // Find EINJ_HEADER
  EinjHeader = (EINJ_HEADER *) ((UINTN )TableHeader + sizeof (ACPI_EINJ_STANDARD_HEADER));

  // Get Injection Entry Count
  EntryCount = EinjHeader->InjectionInstructionEntryCount;

  // Get First Injection Instruction Entry;
  FirstEntryPtr = (EINJ_INSTRUCTION_ENTRY *) ((UINTN )TableHeader + sizeof (ACPI_EINJ_STANDARD_HEADER) + sizeof (EINJ_HEADER));
  CurrentEntryPtr = FirstEntryPtr;

  mEinjData->PlatformEinjValid = TRUE;

  // Get BEGIN_INJECTION_OPERATION entry
  FindNextInjectionAction (FirstEntryPtr, EntryCount, BEGIN_INJECTION_OPERATION, &CurrentEntryPtr);
  if ((CurrentEntryPtr->Instruction == WRITE_REGISTER_VALUE) && (CurrentEntryPtr->RegisterRegion.AddressSpaceID == SYSTEM_MEMORY)) {
    mEinjData->PlatformEinjBeginOpAddress = (UINT64 *) (CurrentEntryPtr->RegisterRegion.Address);
    mEinjData->PlatformEinjBeginOpValue = CurrentEntryPtr->Value;
  } else {
    mEinjData->PlatformEinjValid = FALSE;
    return Status;
  }

  // Get SET_ERROR_TYPE entry
  FindNextInjectionAction (FirstEntryPtr, EntryCount, SET_ERROR_TYPE, &CurrentEntryPtr);
  if ((CurrentEntryPtr->Instruction == WRITE_REGISTER) && (CurrentEntryPtr->RegisterRegion.AddressSpaceID == SYSTEM_MEMORY)) {
    mEinjData->PlatformEinjSetTypeAddress = (UINT64 *) (CurrentEntryPtr->RegisterRegion.Address);
  } else {
    mEinjData->PlatformEinjValid = FALSE;
    return Status;
  }

  // Get Command Status entry
  FindNextInjectionAction (FirstEntryPtr, EntryCount, GET_COMMAND_STATUS, &CurrentEntryPtr);
  if ((CurrentEntryPtr->Instruction == READ_REGISTER) && (CurrentEntryPtr->RegisterRegion.AddressSpaceID == SYSTEM_MEMORY)) {
    mEinjData->PlatformEinjCmdStatus = (UINT64 *) (CurrentEntryPtr->RegisterRegion.Address);
  } else {
    mEinjData->PlatformEinjValid = FALSE;
    return Status;
  }

  // Get GET_ERROR_TYPE Entry
  FindNextInjectionAction (FirstEntryPtr, EntryCount, GET_ERROR_TYPE, &CurrentEntryPtr);

  // Update error injection Capabilities
  AmdFchUpdateInjCap (CurrentEntryPtr);

  // Pointer to end of Last Injection Entry
  CurrentEntryPtr = (EINJ_INSTRUCTION_ENTRY *) ((UINTN )TableHeader + TableHeader->Length);
  // Add AMD FCH injection entry - Excution with SW SMI
  AmdAddFchInjEntry (CurrentEntryPtr);

  EinjHeader->InjectionInstructionEntryCount++;
  TableHeader->Length += sizeof (EINJ_INSTRUCTION_ENTRY);

  return Status;
}
EFI_STATUS
EFIAPI
FindNextInjectionAction (
  IN OUT   EINJ_INSTRUCTION_ENTRY           *BeginEntryPtr,
  IN OUT   UINT32                           EntryCount,
  IN OUT   UINT8                            ActionType,
  IN OUT   EINJ_INSTRUCTION_ENTRY           **EntryPtr
  )
{
  EFI_STATUS                                Status;
  UINT32                                    Index;
  EINJ_INSTRUCTION_ENTRY                    *LocalEntryPtr;

  Status = EFI_UNSUPPORTED;
  LocalEntryPtr = BeginEntryPtr;
  for (Index = 0; Index < EntryCount; Index++) {
    if (LocalEntryPtr->InjectionAction == ActionType) {
      *EntryPtr = LocalEntryPtr;
      Status    = EFI_SUCCESS;
      return Status;
    }

    LocalEntryPtr++;
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdFchUpdateInjCap (
  IN OUT   EINJ_INSTRUCTION_ENTRY           *EntryPtr
  )
{
  EFI_STATUS       Status;
  UINT64           *ErrorInjectionCapPtr;

  Status = EFI_SUCCESS;
  if (EntryPtr->InjectionAction == GET_ERROR_TYPE) {
    if ((EntryPtr->Instruction == READ_REGISTER) && (EntryPtr->RegisterRegion.AddressSpaceID == SYSTEM_MEMORY)) {
      // get Capability pointer
      ErrorInjectionCapPtr = (UINT64 *) (EntryPtr->RegisterRegion.Address);
      // add PCIe fatal error in capability
      *ErrorInjectionCapPtr |= PCI_EXPRESS_UNCORRECTABLE_FATAL;
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdAddFchInjEntry (
  IN OUT   EINJ_INSTRUCTION_ENTRY      *EntryPtr
  )
{
  EFI_STATUS          Status;
  UINT16              SwSmiCmdAddress;
  Status = EFI_SUCCESS;

  SwSmiCmdAddress = ACPIMMIO16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A);
  mAmdFchEinj.RegisterRegion.Address = (UINT64)SwSmiCmdAddress;
  CopyMem (EntryPtr, &mAmdFchEinj, sizeof (EINJ_INSTRUCTION_ENTRY));

  return Status;
}
