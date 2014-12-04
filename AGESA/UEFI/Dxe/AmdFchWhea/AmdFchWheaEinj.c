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

#include "AmdFchWhea.h"
#include "Filecode.h"
#define FILECODE UEFI_DXE_AMDFCHWHEA_AMDFCHWHEAEINJ_FILECODE

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
  EFI_ACPI_SUPPORT_PROTOCOL       *AcpiSupport;
  EFI_STATUS                      Status;
  INTN                            Index;
  UINTN                           Handle;
  EFI_ACPI_TABLE_VERSION          Version;
  EFI_ACPI_DESCRIPTION_HEADER     *Table;
  EFI_ACPI_DESCRIPTION_HEADER     *NewTable;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportGuid,
                  NULL,
                  &AcpiSupport
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Search EINJ table
  //
  Index  = 0;
  Handle = 0;
  do {
    Table  = NULL;
    Status = AcpiSupport->GetAcpiTable (
                            AcpiSupport,
                            Index,
                            &Table,
                            &Version,
                            &Handle
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
      EfiZeroMem (NewTable, MAX_EINJ_SIZE);
      //
      // Copy Table
      //
      EfiCopyMem (NewTable, Table, Table->Length);
      //
      // Update new table
      //
      AmdFchUpdateEinj (NewTable);

      if (!EFI_ERROR (Status)) {
      // Patch EINJ table here and save table back.

        //
        // Remove previous table
        //
        gBS->FreePool (Table);
        Table = NULL;
        Status = AcpiSupport->SetAcpiTable (
                                AcpiSupport,
                                Table,
                                TRUE,
                                Version,
                                &Handle
                                );
        //
        // Add new table
        //
        Handle = 0;
        Status = AcpiSupport->SetAcpiTable (
                                AcpiSupport,
                                NewTable,
                                TRUE,
                                Version,
                                &Handle
                                );
        gBS->FreePool (NewTable);
        return Status;
      }
      gBS->FreePool (NewTable);
    }
    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

  return Status;
}

EFI_STATUS
AmdFchUpdateEinj (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER      *TableHeader
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
  EfiCopyMem (EntryPtr, &mAmdFchEinj, sizeof (EINJ_INSTRUCTION_ENTRY));

  return Status;
}

BOOLEAN
AmdSmiEinjValidate (
  IN OUT   VOID
  )
{

  if (mEinjData->PlatformEinjValid) {
    if (*(mEinjData->PlatformEinjBeginOpAddress) == mEinjData->PlatformEinjBeginOpValue) {
      if (*(mEinjData->PlatformEinjSetTypeAddress) == PCI_EXPRESS_UNCORRECTABLE_FATAL) {
        *(mEinjData->PlatformEinjCmdStatus) = (UINT64)WHEA_EINJ_CMD_SUCCESS;
        return TRUE;
      }
    }
  }

  return FALSE;
}

EFI_STATUS
EFIAPI
AmdSmiEinjSystemContextSave (
  IN OUT   VOID
  )
{
  UINT8               PortId;
  UINT32              Value32;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  if (mEinjData != NULL) {
    mEinjData->Valid = TRUE;

    for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
      ReadPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x00), AccessWidth32, &Value32, &StdHeader);
      if (Value32 != 0xffffffff) {
        mEinjData->SystemContext[PortId].CmdReg                   = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x04));
        mEinjData->SystemContext[PortId].BridgeCntlReg            = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x3E));
        mEinjData->SystemContext[PortId].DeviceStatusReg          = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x60));
        mEinjData->SystemContext[PortId].RootCntlReg              = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x74));
        mEinjData->SystemContext[PortId].PcieUncorrErrMaskReg     = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x158));
        mEinjData->SystemContext[PortId].PcieUncorrErrSeverityReg = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x15C));
        mEinjData->SystemContext[PortId].PcieCorrErrMaskReg       = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x164));
        mEinjData->SystemContext[PortId].PcieAdvErrCapCntlReg     = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x168));
        mEinjData->SystemContext[PortId].PcieRootErrCmdReg        = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x174));
      }
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjErrorInjectionSetup (
  IN OUT   VOID
  )
{
  UINT8               PortId;
  UINT32              Value32;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    ReadPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x00), AccessWidth32, &Value32, &StdHeader);
    if (Value32 != 0xffffffff) {
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x60))  |= 0x0F;
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x17C)) |= 0x07;
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x74))  |= 0x07;
      ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x04))  |= 0x0100;
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x3E))  |= 0x02;
      ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x158)) &= 0xffc22fef;
      ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x15C)) &= 0xffc22fef;
      ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x164)) &= 0xce3f;
      ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x168)) |= 0x0140;
    }
  }

  RwAlink (FCH_ABCFG_REGF4 | (UINT32) (ABCFG << 29), ~BIT4, 0x00, &StdHeader);

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjClrStatus (
  IN OUT   VOID
  )
{
  UINT8               PortId;
  UINT32              Value32;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
    ReadPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x00), AccessWidth32, &Value32, &StdHeader);
    if (Value32 != 0xffffffff) {
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x62))  |= 0x0f;
      ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x154)) |= 0x003ff030;
      ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x160)) |= 0x31c0;
      ACPIMMIO8  (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x180)) |= 0x7f;
      ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x06))  |= 0x4000;
    }
  }

  RwAlink (0x104C | (UINT32) (ABCFG << 29), 0xffffff0f, 0xf0, &StdHeader);
  RwMem (ACPI_MMIO_BASE + SMI_BASE + 0x3C, AccessWidth8, 0x00, BIT2);

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjErrorInjection (
  IN OUT   VOID
  )
{
  UINT8               PortId;
  UINT8               ErrId;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  PortId = AMD_FCH_WHEA_INJ_PORT;
  ErrId  = AMD_FCH_WHEA_INJ_ERROR;

  switch (ErrId) {

  case AMD_FCH_WHEA_GEN_SERR:
    RwAlink (0x1050 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT4 << PortId), 0x00, &StdHeader);
    RwAlink (0x1050 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT4 << PortId), (UINT32) (BIT4 << PortId), &StdHeader);

    AmdSmiEinjChkErr ();
    AmdSmiEinjErrInjDis ();
    AmdSmiEinjSystemContextRestore ();

    break;

  case AMD_FCH_WHEA_TR_LCRC:
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT4), 0x00, &StdHeader);
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT4), BIT4, &StdHeader);
    break;

  case AMD_FCH_WHEA_RE_LCRC:
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT5), 0x00, &StdHeader);
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT5), BIT5, &StdHeader);
    break;

  case AMD_FCH_WHEA_TR_ECRC:
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT6), 0x00, &StdHeader);
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT6), BIT6, &StdHeader);
    break;

  case AMD_FCH_WHEA_RE_ECRC:
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT7), 0x00, &StdHeader);
    RwAlink (FCH_RCINDXP_REG6A | PortId << 24, ~(UINT32) (BIT7), BIT7, &StdHeader);
    break;

  case AMD_FCH_WHEA_PTLP_NP:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT0 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT0 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT0), &StdHeader);
    break;

  case AMD_FCH_WHEA_CA_NP:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT1 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT1 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT1), &StdHeader);
    break;

  case AMD_FCH_WHEA_UR_NP:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT2 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT2 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT2), &StdHeader);
    break;

  case AMD_FCH_WHEA_ACS_NP:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT3 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT3 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT3), &StdHeader);
    break;

  case AMD_FCH_WHEA_PTLP:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT16 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT16 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT16), &StdHeader);
    break;

  case AMD_FCH_WHEA_CA:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT17 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT17 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT17), &StdHeader);
    break;

  case AMD_FCH_WHEA_UR:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT18 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT18 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT18), &StdHeader);
    break;

  case AMD_FCH_WHEA_ACS:
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT19 + BIT24 + BIT25 + BIT26), (UINT32) (PortId << 24), &StdHeader);
    RwAlink (0x1010 | (UINT32) (ABCFG << 29), ~(UINT32) (BIT19 + BIT24 + BIT25 + BIT26), (UINT32) ((PortId << 24) + BIT19), &StdHeader);
    break;
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjChkErr (
  IN OUT   VOID
  )
{
  UINT8               Value8;
  UINT8               PortId;
  UINT32              Value32;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;
  PortId = 7;

  ReadMem (ACPI_MMIO_BASE + SMI_BASE + 0x3C, AccessWidth8, &Value8);

  if (Value8 & BIT2) {

    Value32 = ReadAlink (0x104C | (UINT32) (ABCFG << 29), &StdHeader);
    if (Value32 & BIT4) {
      PortId = 0;
      WriteAlink (0x104C | (UINT32) (ABCFG << 29), BIT4, &StdHeader);
    } else if (Value32 & BIT5) {
      PortId = 1;
      WriteAlink (0x104C | (UINT32) (ABCFG << 29), BIT5, &StdHeader);
    } else if (Value32 & BIT6) {
      PortId = 2;
      WriteAlink (0x104C | (UINT32) (ABCFG << 29), BIT6, &StdHeader);
    } else if (Value32 & BIT7) {
      PortId = 3;
      WriteAlink (0x104C | (UINT32) (ABCFG << 29), BIT7, &StdHeader);
    }
    Value8 = BIT2;
    WriteMem (ACPI_MMIO_BASE + SMI_BASE + 0x3C, AccessWidth8, &Value8);

    AmdFchWheaElogGpp (PortId);
  }

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjErrInjDis (
  IN OUT   VOID
  )
{
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  RwAlink (FCH_ABCFG_REGF4 | (UINT32) (ABCFG << 29), ~BIT4, BIT4, &StdHeader);

  return Status;
}

EFI_STATUS
EFIAPI
AmdSmiEinjSystemContextRestore (
  IN OUT   VOID
  )
{
  UINT8               PortId;
  UINT32              Value32;
  EFI_STATUS          Status;
  AMD_CONFIG_PARAMS   StdHeader;

  Status = EFI_SUCCESS;

  if ((mEinjData != NULL) && (mEinjData->Valid)) {
    for ( PortId = 0; PortId < MAX_GPP_PORTS; PortId++ ) {
      ReadPci (PCI_ADDRESS (0, GPP_DEV_NUM, PortId, 0x00), AccessWidth32, &Value32, &StdHeader);
      if (Value32 != 0xffffffff) {
        ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x04))  = mEinjData->SystemContext[PortId].CmdReg;
        ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x3E))  = mEinjData->SystemContext[PortId].BridgeCntlReg;
        ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x60))  = mEinjData->SystemContext[PortId].DeviceStatusReg;
        ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x74))  = mEinjData->SystemContext[PortId].RootCntlReg;
        ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x158)) = mEinjData->SystemContext[PortId].PcieUncorrErrMaskReg;
        ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x15C)) = mEinjData->SystemContext[PortId].PcieUncorrErrSeverityReg;
        ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x164)) = mEinjData->SystemContext[PortId].PcieCorrErrMaskReg;
        ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x168)) = mEinjData->SystemContext[PortId].PcieAdvErrCapCntlReg;
        ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x174)) = mEinjData->SystemContext[PortId].PcieRootErrCmdReg;

      }
    }
    mEinjData->Valid = FALSE;
  }

  return Status;
}
