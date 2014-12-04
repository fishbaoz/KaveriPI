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
#include <BootTimeRecordHob.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmTable.h>

EFI_PEI_PPI_DESCRIPTOR mPpiLastS3RecordTime = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiLastS3RestorePointPpiGuid,
  NULL
};

EFI_STATUS
StoreS3ResumeBootTime (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
EFIAPI
PeiBootTimeRecordHobInit (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                          Status;
  AMD_BOOT_TIME_RECORD_HOB            BootTimeRecordHob;
  AMD_BOOT_TIME_RECORD_HOB            *BootTimeRecordHobPtr;
  UINT16                              Index;
  UINTN                               SizeOfBootTimeHob;
  AMD_BOOT_TIME_RECORD_PPI            *BootTimeRecordPpi;
  EFI_PEI_NOTIFY_DESCRIPTOR           *mNotifyS3ResumeHobList;
  EFI_HOB_GUID_TYPE                   *Buffer;

  BootTimeRecordPpi = NULL;

  Status = (**PeiServices).LocatePpi (PeiServices, &gBootTimeRecordPpiGuid, 0, NULL, &BootTimeRecordPpi);

  if (!EFI_ERROR (Status)) {
    Index = 0;
    SizeOfBootTimeHob = 0;

    for (Index = 0; Index < MAXIMUM_RECORDS; Index++) {
#ifdef TSC_TIMER_TICK_ENABLE
      BootTimeRecordHob.Record.data[Index].TscTicks = BootTimeRecordPpi->Record->data[Index].TscTicks;
#endif
#ifdef ACPI_TIMER_TICK_ENABLE
      BootTimeRecordHob.Record.data[Index].AcpiTimerTicks = BootTimeRecordPpi->Record->data[Index].AcpiTimerTicks;
#endif
      BootTimeRecordHob.Record.data[Index].Version = BOOT_TIMER_RECORD_RELEASE_VERSION;
      BootTimeRecordHob.Record.data[Index].Description = BootTimeRecordPpi->Record->data[Index].Description;
    }

    BootTimeRecordHob.Record.IndexToWrite = BootTimeRecordPpi->Record->IndexToWrite;
    BootTimeRecordHobPtr = &BootTimeRecordHob;
    SizeOfBootTimeHob = sizeof (AMD_BOOT_TIME_RECORD_HOB);
    If_CpmBuildGuidHob (gAmdBootTimeRecordHobGuid, Buffer, BootTimeRecordHobPtr, SizeOfBootTimeHob, Status) {
      EFI_BOOT_MODE     BootMode;
      EFI_STATUS        Status;

      Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
      if (BootMode == BOOT_ON_S3_RESUME) {
        // Allocate memory for the descriptor
        Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_NOTIFY_DESCRIPTOR), &mNotifyS3ResumeHobList);
        if (!EFI_ERROR (Status)) {
          mNotifyS3ResumeHobList->Flags = (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
          mNotifyS3ResumeHobList->Guid = &gCpmPeiMemoryDiscoveredPpiGuid;
          mNotifyS3ResumeHobList->Notify = StoreS3ResumeBootTime;
          Status = (**PeiServices).NotifyPpi (PeiServices, mNotifyS3ResumeHobList);
          if (EFI_ERROR (Status)) {
            return EFI_SUCCESS;
          }
        }
      }
    }
  }
  return Status;
}

EFI_STATUS
StoreS3ResumeBootTime (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  AMD_BOOT_TIME_RECORD_HOB              *mBootTimeRecord;
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_STATUS                            Status;
  UINT64                                StoreLocation;
  UINT64                                TempData;
  UINT16                                Index;
  AMD_BOOT_TIME_RECORD_PPI              *BootTimeRecordPpi;

  mBootTimeRecord = NULL;
  TempData = 0;
  Index = 0;
  Status = (**PeiServices).InstallPpi (PeiServices, &mPpiLastS3RecordTime);
  Status  = (*PeiServices)->GetHobList (PeiServices, &Hob.Raw);

  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION && CompareGuid (&Hob.Guid->Name, &gAmdBootTimeRecordHobGuid)) {
      mBootTimeRecord = (AMD_BOOT_TIME_RECORD_HOB *) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      break;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (mBootTimeRecord != NULL) {
    Status = (**PeiServices).LocatePpi (PeiServices, &gBootTimeRecordPpiGuid, 0, NULL, &BootTimeRecordPpi);
    if (!EFI_ERROR (Status)) {
      StoreLocation = 0;
      //store location in cmos
      for (Index = 0; Index < 8; Index++) {
        CpmIoWrite8 (0x72, (UINT8) (Index + CMOS_BOOT_TIME_S3RESUME_STORAGE));
        TempData = (UINT8)CpmIoRead8 (0x73);
        StoreLocation |= LShiftU64 (TempData, Index*8);
      }
      BootTimeRecordPpi->S3Record = (AMD_BOOT_TIME_RECORD *) (UINT32)StoreLocation;
      CpmCopyMem (BootTimeRecordPpi->S3Record, &(mBootTimeRecord->Record), sizeof (AMD_BOOT_TIME_RECORD));
    }
  }
  return Status;
}
