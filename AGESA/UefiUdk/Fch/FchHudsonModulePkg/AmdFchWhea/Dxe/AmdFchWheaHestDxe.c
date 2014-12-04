/**
 * @file
 *
 * SB WHEA DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Hudson-2 UEFI Driver
 * @e sub-project   SB WHEA DXE Driver
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

VOID AmdWheaUpdateErrDataEntry (IN GENERIC_ERROR_DATA_ENTRY  *ErrDataEntry);
VOID AmdWheaUpdatePcieErrSection (IN PCIE_ERROR_SECTION  *PcieErrSection, IN UINT8 PortId);
VOID AmdWheaTriggerNMI (IN OUT   VOID);

EFI_GUID     mPcieErrSectionType = PCIE_ERROR_GUID;

STATIC
ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR mAmdErrSource = {
  0x0009,                                                       // Type
  0x0000,                                                       // SourceId
  0xffff,                                                       // Reserved
  0x00,                                                         // Flags
  0x01,                                                         // Enabled
  0x00000001,                                                   // NumberofRecordsToPreAllocate
  0x00000001,                                                   // MaxSectionsPerRecord
  MAX_ERROR_BLOCK_SIZE,                                         // MaxRawDataLength
  {0x00, 0x40, 0x00, 0x04, 0x00},                               // ErrorStatusAddress. Address will be filled in runtime
  {0x04, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       // NotifyMethod (NMI)
  MAX_ERROR_BLOCK_SIZE                                          // ErrorStatusSize
};

EFI_STATUS
AmdFchWheaInitEntryHest ( VOID )
{
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL                     *AcpiSdtProtocol;
  EFI_STATUS                                Status;
  INTN                                      Index;
  EFI_ACPI_TABLE_VERSION                    Version;
  UINTN                                     TableKey;
  EFI_ACPI_SDT_HEADER                       *Table;
  EFI_ACPI_SDT_HEADER                       *NewTable;

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
  // Search HEST table
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
    // Check Signture and update HEST table
    //
    if (Table->Signature == 0x54534548) {
      //
      // allocate memory for new Table
      //
      Status = gBS->AllocatePool (
                      EfiReservedMemoryType,
                      MAX_HEST_SIZE,
                      &NewTable
                      );
      if (Status != EFI_SUCCESS) {
        return Status;
      }
      ZeroMem (NewTable, MAX_HEST_SIZE);
      //
      // Copy Table
      //
      CopyMem (NewTable, Table, Table->Length);
      //
      // Update new table
      //
      AmdFchUpdateHest (NewTable);

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
    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

  return Status;
}


EFI_STATUS
AmdFchUpdateHest (
  IN OUT   EFI_ACPI_SDT_HEADER              *TableHeader
  )
{
  EFI_STATUS                                Status;
  HEST_HEADER                               *HestHeader;
  ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR   *ErrSource;

  Status     = EFI_SUCCESS;
  HestHeader = (HEST_HEADER *)TableHeader;
  ErrSource  = (ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR *) ((UINTN)HestHeader + HestHeader->Length);

  HestHeader->Length += sizeof (ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR);
  HestHeader->ErrorSouceCount++;
  mAmdErrSource.SourceId = (UINT16) (HestHeader->ErrorSouceCount);
  CopyMem (ErrSource, &mAmdErrSource, sizeof (ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR));

  return Status;
}

EFI_STATUS
EFIAPI
AmdErrBlkAddressUpdate (
  IN OUT   VOID
  )
{
  EFI_STATUS       Status;
  UINT64           *ErrBlkAddress;

  Status = EFI_SUCCESS;

  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  sizeof (UINT64),
                  &ErrBlkAddress
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *ErrBlkAddress = (UINT64) (mEinjData->AmdHwErrBlk);
  mAmdErrSource.ErrorStatusAddress.Address = (UINT64 )ErrBlkAddress;

  return Status;
}

VOID
AmdWheaUpdateErrDataEntry (
  IN GENERIC_ERROR_DATA_ENTRY               *ErrDataEntry
  )
{
  ErrDataEntry->SectionType  = mPcieErrSectionType;
  ErrDataEntry->Severity     = ERROR_SEV_FATAL;
  ErrDataEntry->Revision     = 0x0201;
  ErrDataEntry->SectionFlags = 0x1;
  ErrDataEntry->DataLength   = sizeof (PCIE_ERROR_SECTION);
}

VOID
AmdWheaUpdatePcieErrSection (
  IN PCIE_ERROR_SECTION  *PcieErrSection,
  IN UINT8               PortId
  )
{
  UINT8     Index;

  PcieErrSection->Validation            = 0xEF;
  PcieErrSection->PortType              = 0x04;
  PcieErrSection->Revision              = 0x02;
  PcieErrSection->CommandStatus         = ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x04));

  PcieErrSection->DeviceId.VendorId     = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x00));
  PcieErrSection->DeviceId.DeviceId     = ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x02));
  PcieErrSection->DeviceId.ClassCode[0] = ACPIMMIO8 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x09));
  PcieErrSection->DeviceId.ClassCode[1] = ACPIMMIO8 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x0A));
  PcieErrSection->DeviceId.ClassCode[2] = ACPIMMIO8 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x0B));
  PcieErrSection->DeviceId.Function     = PortId;
  PcieErrSection->DeviceId.Device       = GPP_DEV_NUM;
  PcieErrSection->DeviceId.Segment      = 0x00;
  PcieErrSection->DeviceId.PrimaryBus   = ACPIMMIO8 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x18));
  PcieErrSection->DeviceId.SecondaryBus = ACPIMMIO8 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x19));
  PcieErrSection->DeviceId.Slot         = PortId;

  PcieErrSection->BridgeCtrlStatus      = (UINT32) (ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x3E)) << 16) | \
                                            (UINT32)(ACPIMMIO16 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, 0x1E)));

  for (Index = 0; Index < 15; Index++) {
    PcieErrSection->CapabilityStructure.CapabilityData[Index] = \
                                          ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, (0x58 + 4 * Index)));
  }

  for (Index = 0; Index < 24; Index++) {
    PcieErrSection->AerInfo.AerInfoData[Index] = \
                                          ACPIMMIO32 (PCIE_ADDRESS (mEinjData->PcieBaseAddress, 0, GPP_DEV_NUM, PortId, (0x150 + 4 * Index)));
  }
}

VOID
AmdWheaTriggerNMI (
  IN OUT   VOID
  )
{
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + 0xC0) &=  ~(UINT32)BIT20;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + 0xC0) |= BIT21;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + 0x98) &= ~(UINT32)BIT25;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + 0x98) |= BIT25;
  ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + 0xC0) &=  ~(UINT32) (BIT20 + BIT21);
}
