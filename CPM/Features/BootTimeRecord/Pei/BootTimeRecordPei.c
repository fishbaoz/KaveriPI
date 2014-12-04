/* $NoKeywords:$ */
  /**
 * @file
 *
 * AMD Boot Time Record Hob, and related functions.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      Common Platform Modules
 * @e sub-project:  Boot Time Record
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
 /*****************************************************************************
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
 * ***************************************************************************
 */
#include <BootTimeRecordPei.h>
#include <BootTimeRecordHob.h>
#include <AmdCpmBaseIo.h>

EFI_STATUS
EFIAPI
PeiBootTimeRecordInit (
  IN       CPM_PEI_FILE_HANDLE  FileHandle,
  IN       CPM_PEI_SERVICES     **PeiServices
  )
{
#ifdef AMD_BIOS_BOOT_TIME_RECORD
  EFI_STATUS  Status;
  EFI_PEI_PPI_DESCRIPTOR        *PpiListAmdBootTimeRecordPei;
  EFI_PEI_NOTIFY_DESCRIPTOR     *PpiNotifyAmdBootTimerRecordHob;
  AMD_BOOT_TIME_RECORD_PPI      *mAmdBootTimeRecord;

#ifdef IDS_PERFORMANCE_ENABLE
  UINT64                                StoreLocation;
  UINT64                                TempData;
  UINT16                                Index;

  TempData = 0;
  Index = 0;
#endif

  // Allocate memory for the AMD_BOOT_TIME_RECORD_PPI
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (AMD_BOOT_TIME_RECORD_PPI), &mAmdBootTimeRecord);
  if (!EFI_ERROR (Status)) {
    mAmdBootTimeRecord->Record = NULL;
    mAmdBootTimeRecord->S3Record = NULL;

#ifdef IDS_PERFORMANCE_ENABLE
    mAmdBootTimeRecord->IdsS3Record = NULL;
    StoreLocation = 0;
    //store location in cmos
    for (Index = 0; Index < 8; Index++) {
      CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_IDS_S3_STORAGE));
      TempData = (UINT8)CpmIoRead8 (0x73);
      StoreLocation |= LShiftU64 (TempData, Index*8);
    }

    if ((StoreLocation!= 0) && (StoreLocation!= 0xFFFFFFFF)) {
      mAmdBootTimeRecord->IdsS3Record = (TP_Perf_STRUCT *) (UINT32)StoreLocation;
    }

    mAmdBootTimeRecord->CopyIdsPerformanceDataToIdsRecord = SetIdsPerformanceDataMemoryLocation;
#endif

    mAmdBootTimeRecord->AddRecord = AddRecord;

    Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (AMD_BOOT_TIME_RECORD), &mAmdBootTimeRecord->Record);
    if (!EFI_ERROR (Status)) {
      (*PeiServices)->SetMem (mAmdBootTimeRecord->Record, sizeof(AMD_BOOT_TIME_RECORD), 0x00);
    }
    // Allocate memory for the descriptor
    Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_PPI_DESCRIPTOR), &PpiListAmdBootTimeRecordPei);
    if (!EFI_ERROR (Status)) {
      PpiListAmdBootTimeRecordPei->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
      PpiListAmdBootTimeRecordPei->Guid = &gBootTimeRecordPpiGuid;
      PpiListAmdBootTimeRecordPei->Ppi = mAmdBootTimeRecord;
      Status = (**PeiServices).InstallPpi (PeiServices, PpiListAmdBootTimeRecordPei);
      if (EFI_ERROR (Status)) {
        return EFI_SUCCESS;
      }

      // Allocate memory for the descriptor
      Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_NOTIFY_DESCRIPTOR), &PpiNotifyAmdBootTimerRecordHob);
      if (!EFI_ERROR (Status)) {
        PpiNotifyAmdBootTimerRecordHob->Flags = (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
        PpiNotifyAmdBootTimerRecordHob->Guid = &gCpmPeiMemoryDiscoveredPpiGuid;
        PpiNotifyAmdBootTimerRecordHob->Notify = PeiBootTimeRecordHobInit;
        Status = (**PeiServices).NotifyPpi (PeiServices, PpiNotifyAmdBootTimerRecordHob);
        if (EFI_ERROR (Status)) {
          return EFI_SUCCESS;
        }
      }
    }
    RECORD_TIME_PEI (AtStartRecording);
  }
  return Status;
#else
  return EFI_SUCCESS;
#endif
}

#ifdef IDS_PERFORMANCE_ENABLE
EFI_STATUS
EFIAPI
SetIdsPerformanceDataMemoryLocation (
  IN       EFI_PEI_SERVICES          **PeiServices,
  IN       TP_Perf_STRUCT            *Record,
  IN OUT   VOID *IdsCalloutData
  )
{
  UINT64 MemoryLocation;
  IDS_CALLOUT_STRUCT *IdsCData;

  MemoryLocation = 0;
  IdsCData = NULL;
  IdsCData = (IDS_CALLOUT_STRUCT *)IdsCalloutData;
  MemoryLocation = (UINT64) (TP_Perf_STRUCT*)Record;
  MemoryLocation&= 0x00000000FFFFFFFF;
  IdsCData->Reserved = (UINTN)MemoryLocation;
  return EFI_SUCCESS;
}
#endif

EFI_STATUS
EFIAPI
AddRecord (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN AMD_BOOT_TIME_RECORD      *Record,
  IN UINT64                    desc
  )
{
  AMD_BOOT_TIME_DATA     data;
  UINT64                 msrValue;

#if BTR_HIDDEN_AFTER
  if (desc >= BTR_HIDDEN) {
    return EFI_SUCCESS;
  }
#endif
  msrValue = CpmMsrRead (0x1B); // EFI_MSR_IA32_APIC_BASE
  if (msrValue & 0x0000000000000100) {
    //check freqsel
    msrValue = CpmMsrRead (0xC0010015);
    if ((msrValue&0x0000000001000000) == 0) {
      msrValue |= 0x0000000001000000; //set BIT24 freqsel
      CpmMsrWrite (0xC0010015, msrValue);
    }
#ifdef TSC_TIMER_TICK_ENABLE
    data.TscTicks = CpmReadTsc ();
#endif
#ifdef ACPI_TIMER_TICK_ENABLE
    data.AcpiTimerTicks = (UINT32)CpmIoRead32 (0x408);
#endif
    data.Version = BOOT_TIMER_RECORD_RELEASE_VERSION;
    data.Description = desc;
#ifdef TSC_TIMER_TICK_ENABLE
    Record->data[Record->IndexToWrite].TscTicks = data.TscTicks;
#endif
#ifdef ACPI_TIMER_TICK_ENABLE
    Record->data[Record->IndexToWrite].AcpiTimerTicks = data.AcpiTimerTicks;
#endif
    Record->data[Record->IndexToWrite].Version = BOOT_TIMER_RECORD_RELEASE_VERSION;
    Record->data[Record->IndexToWrite].Description = data.Description;
    Record->IndexToWrite++;
  }

  return EFI_SUCCESS;
}