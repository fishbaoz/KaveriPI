/* $NoKeywords:$ */
 /**
 * @file
 *
 * AMD Boot Time Record PPI, and related functions.
 *
 * Contains code that initializes the AMD Boot Time Record PPI.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      Common Platform Module
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

#ifndef _AMD_CPM_BOOT_TIME_RECORD_PPI_H_
#define _AMD_CPM_BOOT_TIME_RECORD_PPI_H_
#include <AmdCpmBase.h>

#define AMD_BOOT_TIME_RECORD_PPI_GUID \
{ \
  0x90599780, 0x50b0, 0x43d1, 0x9f, 0x22, 0x43, 0xd1, 0xeb, 0xc, 0x0, 0x3f \
}

#define AMD_S3_RESUME_BEFORE_WAK_PPI_GUID \
{ \
  0x90599782, 0x50c0, 0x63d1, 0x9f, 0x22, 0x48, 0xd6, 0xeb, 0xc, 0x0, 0x3f \
}

#pragma pack (push, 1)
///Boot Time Data
typedef struct _AMD_BOOT_TIME_DATA {
  #ifdef TSC_TIMER_TICK_ENABLE
    UINT64    TscTicks;       ///< Cpu Tsc Ticks
  #endif
  #ifdef ACPI_TIMER_TICK_ENABLE
    UINT32    AcpiTimerTicks; ///< ACPI Timer Ticks
  #endif
  UINT32    Version;          ///< Boot Time Record Version
  UINT64    Description;      ///< Test Points description
}  AMD_BOOT_TIME_DATA;

///Boot Time Record data holder
typedef struct _AMD_BOOT_TIME_RECORD {
  UINT16               IndexToWrite;          ///< Last Index + 1;
  AMD_BOOT_TIME_DATA   data[MAXIMUM_RECORDS]; ///< Boot Time Data Holder
} AMD_BOOT_TIME_RECORD;

#ifdef IDS_PERFORMANCE_ENABLE
  #define MAX_PERFORMANCE_UNIT_NUM MAX_IDS_PERFORMANCE_UNIT_NUM
  /// Data Structure of Parameters for TestPoint_TSC.
  typedef struct {
    UINT32 LineInFile;                    ///< Line of current time counter
    UINT64 Description;                   ///< Description ID
    UINT64 StartTsc;                      ///< The StartTimer of TestPoint_TSC
    #ifdef ACPI_TIMER_TICK_ENABLE
      UINT32 StartAcpiTimer;                ///< Acpi Timer
    #endif
  } TestPoint_TSC;

  /// Data Structure of Parameters for TP_Perf_STRUCT.
  typedef struct {
    UINT32 Signature;                                 ///< "TIME"
    UINT32 Index;                                     ///< The Index of TP_Perf_STRUCT
    UINT32 TscInMhz;                                  ///< Tsc counter in 1 mhz
    UINT32 Version;                                   ///< Version number
    #ifdef ACPI_TIMER_TICK_ENABLE
      UINT32 AcpiTimer;                                 ///< Acpi Timer
    #endif
    TestPoint_TSC TP[MAX_PERFORMANCE_UNIT_NUM];       ///< The TP of TP_Perf_STRUCT
  } TP_Perf_STRUCT;
#endif

#pragma pack (pop)
typedef
EFI_STATUS
(EFIAPI *AMD_BOOT_TIME_RECORD_PPI_ADD) (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN AMD_BOOT_TIME_RECORD      *Record,
  IN UINT64                     desc
  );

#ifdef  IDS_PERFORMANCE_ENABLE
  typedef
  EFI_STATUS
  (EFIAPI *AMD_BOOT_TIME_RECORD_PPI_IDS) (
    IN EFI_PEI_SERVICES          **PeiServices,
    IN TP_Perf_STRUCT            *Record,
    IN  OUT VOID *IdsCalloutData
    );
#endif

///Boot Time Record PPI
typedef struct _AMD_BOOT_TIME_RECORD_PPI {
  AMD_BOOT_TIME_RECORD            *Record;                            ///<Boot time record for bios post
  AMD_BOOT_TIME_RECORD            *S3Record;                          ///<Boot time record for Standby resume
  #ifdef IDS_PERFORMANCE_ENABLE
    TP_Perf_STRUCT                  *IdsS3Record;                       ///<Boot time record for AGESA
    AMD_BOOT_TIME_RECORD_PPI_IDS    CopyIdsPerformanceDataToIdsRecord;  ///<Function for transfer AGESA IDS points to Boot time record
  #endif
  AMD_BOOT_TIME_RECORD_PPI_ADD    AddRecord;                          ///<Function to add record
} AMD_BOOT_TIME_RECORD_PPI;

extern EFI_GUID gBootTimeRecordPpiGuid;
extern EFI_GUID gPeiLastS3RestorePointPpiGuid;

extern EFI_STATUS RecordTimeTicks (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN UINT64                    desc
);

extern EFI_STATUS RecordTimeTicksS3Resume (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN UINT64                     desc
);


#ifdef IDS_PERFORMANCE_ENABLE
  extern EFI_STATUS RecordTimeTicksAGESAS3Resume (
    IN  EFI_PEI_SERVICES          **PeiServices,
    IN  OUT VOID                  *IdsCalloutData
  );
#endif

#ifdef AMD_BIOS_BOOT_TIME_RECORD
  #define RECORD_TIME_PEI(x) RecordTimeTicks((EFI_PEI_SERVICES**)PeiServices, x);
  #define RECORD_TIME_S3RESUME(x) RecordTimeTicksS3Resume((EFI_PEI_SERVICES**)PeiServices, x);
  #ifdef IDS_PERFORMANCE_ENABLE
    #define RECORD_TIME_AGESA_S3RESUME(x) RecordTimeTicksAGESAS3Resume((EFI_PEI_SERVICES**)PeiServices, (VOID *)x);
  #else
    #define RECORD_TIME_AGESA_S3RESUME(x)
  #endif
#else
  #define RECORD_TIME_PEI(x)
  #define RECORD_TIME_S3RESUME(x)
  #define RECORD_TIME_AGESA_S3RESUME(x)
#endif


#endif
