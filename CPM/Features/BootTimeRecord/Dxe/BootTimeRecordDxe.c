/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Boot Time Record DXE Iniatial
 *
 * Contains CPM code to perform Boot Time Record initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      Common Platform Modules
 * @e sub-project:  Boot Time Record
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#include "BootTimeRecordDxe.h"

EFI_STATUS
InsertRecordData (
  IN OUT   AMD_BOOT_TIME_RECORD *RecordData,
  IN OUT   AMD_BOOT_TIME_DATA *data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  AMD_BOOT_TIME_RECORD_ITEM *Item;

  RecordData->Index++;

  Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (AMD_BOOT_TIME_RECORD_ITEM), &Item);

  if (!EFI_ERROR (Status)) {
#ifdef TSC_TIMER_TICK_ENABLE
    Item->data.TscTicks = data->TscTicks;
#endif
#ifdef ACPI_TIMER_TICK_ENABLE
    Item->data.AcpiTimerTicks = data->AcpiTimerTicks;
#endif
    Item->data.Version = BOOT_TIMER_RECORD_RELEASE_VERSION;
    Item->data.Description = data->Description;
    Item->NextRecord = RecordData->ItemHead;
    RecordData->ItemHead = Item;
  } else {
    return Status;
  }

  return Status;
}

#ifdef IDS_PERFORMANCE_ENABLE
EFI_STATUS
EFIAPI
SetIdsPerformanceDataMemoryLocation (
  IN OUT   VOID *IdsCalloutData
  )
{
  UINT64 MemoryLocation;
  UINT8 Index;
  IDS_CALLOUT_STRUCT *IdsCData;

  MemoryLocation = 0;
  Index = 0;
  IdsCData = (IDS_CALLOUT_STRUCT *)IdsCalloutData;

  for (Index = 0; Index < 8; Index++) {
    CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_STORAGE));
    MemoryLocation |= (UINT64) ((CpmIoRead8 (0x73)&0xFF) << (Index*8));
  }

  MemoryLocation &= 0x00000000FFFFFFFF;
  IdsCData->Reserved = (UINTN)MemoryLocation;
  return EFI_SUCCESS;
}
#endif

EFI_STATUS
CopyPeiRecordToDxeRecord (
  IN OUT   AMD_BOOT_TIME_PEI_RECORD *PeiRecordData
  )
{
  EFI_STATUS                            Status;
  AMD_BOOT_TIME_RECORD_PROTOCOL         *mBootTimeRecordProtocol = NULL;
  AMD_BOOT_TIME_RECORD                  *mBootTimeRecord = NULL;
  AMD_BOOT_TIME_PEI_RECORD              *mBootTimeS3Record = NULL;
#ifdef IDS_PERFORMANCE_ENABLE
  TP_Perf_STRUCT                        *mBootTimeIdsRecord = NULL;
  TP_Perf_STRUCT                        *mBootTimeIdsS3Record = NULL;
#endif
  UINT16                                Index;
  UINT64                                StoreLocation;

  Index = 0;
  StoreLocation = 0;
  Status = gBS->LocateProtocol (&gBootTimeRecordProtocolGuid, NULL, &mBootTimeRecordProtocol);
  if (!EFI_ERROR (Status)) {
    if (mBootTimeRecordProtocol->Record == NULL) {
      Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (AMD_BOOT_TIME_RECORD), &mBootTimeRecordProtocol->Record);
      if (!EFI_ERROR (Status)) {
        gBS->SetMem (mBootTimeRecordProtocol->Record, sizeof (AMD_BOOT_TIME_RECORD), 0);
      } else {
        return Status;
      }
      mBootTimeRecord = (AMD_BOOT_TIME_RECORD *)mBootTimeRecordProtocol->Record;
      StoreLocation = (UINT64) (AMD_BOOT_TIME_RECORD*)mBootTimeRecord;
      for (Index = 0; Index < 8; Index++) {
        CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_STORAGE));
        CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
      }
    }

    if (mBootTimeRecordProtocol->S3Record == NULL) {
      Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (AMD_BOOT_TIME_PEI_RECORD), &mBootTimeRecordProtocol->S3Record);
      if (!EFI_ERROR (Status)) {
        gBS->SetMem (mBootTimeRecordProtocol->S3Record, sizeof (AMD_BOOT_TIME_PEI_RECORD), 0);
      } else {
        return Status;
      }

      mBootTimeS3Record = (AMD_BOOT_TIME_PEI_RECORD *)mBootTimeRecordProtocol->S3Record;
      StoreLocation = (UINT64) (AMD_BOOT_TIME_PEI_RECORD*)mBootTimeS3Record;
      for (Index = 0; Index < 8; Index++) {
        CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_S3RESUME_STORAGE));
        CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
      }
    }

#ifdef IDS_PERFORMANCE_ENABLE
    if (mBootTimeRecordProtocol->IdsRecord == NULL) {
      Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (TP_Perf_STRUCT), &mBootTimeRecordProtocol->IdsRecord);
      if (!EFI_ERROR (Status)) {
        gBS->SetMem (mBootTimeRecordProtocol->IdsRecord, sizeof (TP_Perf_STRUCT), 0);
      } else {
        return Status;
      }

      mBootTimeIdsRecord = (TP_Perf_STRUCT *)mBootTimeRecordProtocol->IdsRecord;
      StoreLocation = (UINT64) (TP_Perf_STRUCT*)mBootTimeIdsRecord;

      for (Index = 0; Index < 8; Index++) {
        CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_STORAGE));
        CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
      }
    }

    if (mBootTimeRecordProtocol->IdsS3Record == NULL) {
      Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (TP_Perf_STRUCT), &mBootTimeRecordProtocol->IdsS3Record);
      if (!EFI_ERROR (Status)) {
        gBS->SetMem (mBootTimeRecordProtocol->IdsS3Record, sizeof (TP_Perf_STRUCT), 0);
      } else {
        return Status;
      }

      mBootTimeIdsS3Record = (TP_Perf_STRUCT *)mBootTimeRecordProtocol->IdsS3Record;
      StoreLocation = (UINT64) (TP_Perf_STRUCT*)mBootTimeIdsS3Record;

      for (Index = 0; Index < 8; Index++) {
        CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_S3_STORAGE));
        CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
      }
    }
#else
    StoreLocation = 0;
    for (Index = 0; Index < 8; Index++) {
      CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_STORAGE));
      CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
    }

    for (Index = 0; Index < 8; Index++) {
      CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_S3_STORAGE));
      CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
    }
#endif

    for (Index = 0; Index < PeiRecordData->IndexToWrite; Index ++) {
      InsertRecordData (mBootTimeRecord, (AMD_BOOT_TIME_DATA *)&PeiRecordData->data[Index]);
    }
  }

  return EFI_SUCCESS;
}

VOID
ExitBootServiceTimingPoints (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  RECORD_TIME_DXE (AtExitBootServices);
}

EFI_STATUS
EFIAPI
DxeBootTimeRecordInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
#ifdef AMD_BIOS_BOOT_TIME_RECORD
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  VOID                                  *HobList;
  AMD_BOOT_TIME_PEI_RECORD              *BootTimeRecordData = NULL;
  AMD_BOOT_TIME_RECORD_PROTOCOL         *mBootTimeRecordProtocol = NULL;
  EFI_HANDLE                            mBootTimeRecordProtocolHandle = NULL;
  EFI_EVENT                             ExitBootServicesEvent;

  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = 0;
  BufferSize = 0;
  HobList = NULL;

  //get data from PEI record.
  Status = CpmGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (!EFI_ERROR (Status)) {
    If_CpmGetHobBuffer (HobList, gAmdBootTimeRecordHobGuid, BootTimeRecordData, BufferSize, GuidHob) {
    } else {
      CPM_DEADLOOP ();
    }
  }

  Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (AMD_BOOT_TIME_RECORD_PROTOCOL), &mBootTimeRecordProtocol);

  if (!EFI_ERROR (Status)) {
    mBootTimeRecordProtocol->Record = NULL;
    mBootTimeRecordProtocol->S3Record = NULL;
    mBootTimeRecordProtocol->AddRecord = AddRecord;
#ifdef IDS_PERFORMANCE_ENABLE
    mBootTimeRecordProtocol->IdsRecord = NULL;
    mBootTimeRecordProtocol->IdsS3Record = NULL;
    mBootTimeRecordProtocol->CopyIdsPerformanceDataToIdsRecord = SetIdsPerformanceDataMemoryLocation;
#endif
    //
    // Install on a new handle
    //
    Status = gBS->InstallProtocolInterface (
              &mBootTimeRecordProtocolHandle,
              &gBootTimeRecordProtocolGuid,
              EFI_NATIVE_INTERFACE,
              mBootTimeRecordProtocol
             );

    if (!EFI_ERROR (Status)) {
      //store data from PEI record
      Status = CopyPeiRecordToDxeRecord (BootTimeRecordData);
    }

    ExitBootServicesEvent = NULL;
    Status = gBS->CreateEvent (
              CPM_EVENT_SIGNAL_EXIT_BOOT_SERVICES,
              CPM_TPL_NOTIFY,
              ExitBootServiceTimingPoints,
              NULL,
              &ExitBootServicesEvent
             );
  }
#else
  UINT16                                Index;
  UINT64                                StoreLocation;

  Index = 0;
  StoreLocation = 0;
  for (Index = 0; Index < 8; Index++) {
    CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_STORAGE));
    CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
  }

  for (Index = 0; Index < 8; Index++) {
    CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_S3RESUME_STORAGE));
    CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
  }

  for (Index = 0; Index < 8; Index++) {
    CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_STORAGE));
    CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
  }

  for (Index = 0; Index < 8; Index++) {
    CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_S3_STORAGE));
    CpmIoWrite8 (0x73, (UINT8) ((StoreLocation >> (Index*8))&(0xFF)));
  }
#endif
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
AddRecord (
  IN UINT64                    desc
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  AMD_BOOT_TIME_DATA                    data;
  AMD_BOOT_TIME_RECORD                  *mBootTimeRecord = NULL;
  AMD_BOOT_TIME_RECORD_PROTOCOL         *mBootTimeRecordProtocol = NULL;

#if BTR_HIDDEN_AFTER
  if (desc >= BTR_HIDDEN) {
    return EFI_SUCCESS;
  }
#endif

  Status = gBS->LocateProtocol (&gBootTimeRecordProtocolGuid, NULL, &mBootTimeRecordProtocol);
  if (!EFI_ERROR (Status)) {
    if (mBootTimeRecordProtocol->Record == NULL) {
      Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (AMD_BOOT_TIME_RECORD), &mBootTimeRecordProtocol->Record);
      if (!EFI_ERROR (Status)) {
        gBS->SetMem (mBootTimeRecordProtocol->Record, sizeof (AMD_BOOT_TIME_RECORD), 0);
      } else {
        return Status;
      }
    }

    mBootTimeRecord = (AMD_BOOT_TIME_RECORD *)mBootTimeRecordProtocol->Record;
#ifdef TSC_TIMER_TICK_ENABLE
    data.TscTicks = CpmReadTsc ();
#endif
#ifdef ACPI_TIMER_TICK_ENABLE
    data.AcpiTimerTicks = (UINT32)CpmIoRead32 (0x408);
#endif
    data.Version = BOOT_TIMER_RECORD_RELEASE_VERSION;
    data.Description = desc;
    InsertRecordData (mBootTimeRecord, &data);
  }
  return EFI_SUCCESS;
}
