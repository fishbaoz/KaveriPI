/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU POST API, and related functions.
 *
 * Contains code that initialized the CPU after memory init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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
/*++
Module Name:

  AmdProcessorInitPeim.c

Abstract:
--*/

#include "Tiano.h"
#include "Pei.h"
#include "PeiLib.h"

#include "AGESA.h"
#include "amdlib.h"
#include "OemAgesaCustomize.h"
#include "GeneralServices.h"
#include "OemS3Restore.h"
#include "Filecode.h"
#define FILECODE UEFI_PEI_AMDPROCESSORINITPEIM_AMDPROCESSORINITPEIM_FILECODE

#include EFI_PPI_CONSUMER (Variable)
#include EFI_PPI_DEFINITION (AmdInitResetPpi)
#include EFI_PPI_DEFINITION (AmdInitEarlyPpi)
#include EFI_PPI_DEFINITION (AmdInitPostPpi)
#include EFI_PPI_DEFINITION (AgesaMemPpi)
#include EFI_GUID_DEFINITION (AmdMemoryInfoHob)


#ifndef FOURGB
#define FOURGB      0x100000000ul
#endif

#define MAX_NUMBER_OF_EXTENDED_MEMORY_DESCRIPTOR    2
#define MAX_SIZEOF_AMD_MEMORY_INFO_HOB_BUFFER       (sizeof (AMD_MEMORY_INFO_HOB) +  \
          (MAX_NUMBER_OF_EXTENDED_MEMORY_DESCRIPTOR * sizeof (AMD_MEMORY_RANGE_DESCRIPTOR)))

EFI_STATUS
DiscoverMemory (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
InvokeAmdInitS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
InvokeAmdInitS3FinalRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

STATIC AMD_PEI_INIT_RESET_PPI mAmdInitResetPpi =
{
 AMD_PEI_INIT_RESET_REV
};

STATIC EFI_PEI_PPI_DESCRIPTOR mInitResetPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiInitResetPpiGuid,
  &mAmdInitResetPpi
};

STATIC AMD_PEI_INIT_EARLY_PPI mAmdInitEarlyPpi =
{
 AMD_PEI_INIT_EARLY_REV
};

STATIC EFI_PEI_PPI_DESCRIPTOR mInitEarlyPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiInitEarlyPpiGuid,
  &mAmdInitEarlyPpi
};


STATIC AMD_PEI_INIT_POST_PPI mAmdInitPostPpi =
{
 AMD_PEI_INIT_POST_REV
};

STATIC EFI_PEI_PPI_DESCRIPTOR mInitPostPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiInitPostPpiGuid,
  &mAmdInitPostPpi
};

//
// Global function
//
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyList =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gAgesaMemPpiGuid,
  DiscoverMemory
};

// The following GUID has been initialized with the value which is available from Framework
// as PEI_PERMANENT_MEMORY_INSTALLED_PPI_GUID which is defined in memoryDiscovered.h and
// it is architectural PPI GUID. In case you see any issue with this notification, please
// ensure that GUID has not been modified or vendor/customer is not using any other PPI
// to signal Permanent Memory Available.

EFI_GUID  gEfiPeiPermanentMemoryInstallPpiGuid =   { \
    0xf894643d, 0xc449, 0x42d1, 0x8e, 0xa8, 0x85, 0xbd, 0xd8, 0xc6, 0x5b, 0xde \
  };


//
// Global function
//
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyListS3LateRestore =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiPermanentMemoryInstallPpiGuid,
  InvokeAmdInitS3LateRestore
};

// The following GUID has been initialized with the value which is available from Framework
// as PEI_POST_BOOT_SCRIPT_TABLE_PPI_GUID which is defined in PostBootScriptTable.h and
// it is architectural PPI GUID. In case you see any issue with this notification, please
// ensure that GUID has not been modified or vendor/customer is not using any other PPI
// to signal resume BootScriptExecute has completed.

EFI_GUID  gEfiPeiPostBootScriptTableGuid =   { \
    0x88c9d306, 0x900, 0x4eb5, 0x82, 0x60, 0x3e, 0x2d, 0xbe, 0xda, 0x1f, 0x89\
  };


//
// Global function
//
STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyListS3FinalRestore =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiPostBootScriptTableGuid,
  InvokeAmdInitS3FinalRestore
};

EFI_STATUS
BuildHobInfo (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  AMD_POST_PARAMS                 *PostInterfaceParams
  )
/*++

Routine Description:

  This function build HOB info from post interface parameters

Arguments:

  PeiServices - PerServices
  PostInterfaceParams - Post interface parameters pointer

Returns:
  EFI_STATUS - Status code
--*/
{
  EFI_STATUS            Status;
  UINT64                TOM1;
  UINT64                TOM2;
  UINT64                UmaBase;
  UINT64                UmaSize;

  UINT8                               MemInfoHobBuffer[MAX_SIZEOF_AMD_MEMORY_INFO_HOB_BUFFER];
  AMD_MEMORY_INFO_HOB                 *MemInfoHob;
  AMD_MEMORY_RANGE_DESCRIPTOR         *MemRangDesc;
  UINTN                               SizeOfMemInfoHob;
  UINT32                              MemRangIndex;

  MemInfoHob = (AMD_MEMORY_INFO_HOB *) &MemInfoHobBuffer[0];
  MemRangDesc = &MemInfoHob->Ranges[0];
  MemInfoHob->Version = AMD_MEMORY_INFO_HOB_VERISION;

  UmaSize = (UINT64) (PostInterfaceParams->MemConfig.UmaSize) << 16;
  UmaBase = (UINT64) (PostInterfaceParams->MemConfig.UmaBase) << 16;
  TOM1 = (UINT64) (PostInterfaceParams->MemConfig.Sub4GCacheTop);
  TOM2 = (UINT64) (PostInterfaceParams->MemConfig.SysLimit + 1) << 16;

  MemRangIndex = 0;
  //
  // Memory Rang 0 : 0 - TOM1 (exclude UMA)
  //
  MemRangDesc[MemRangIndex].Attribute = AMD_MEMORY_ATTRIBUTE_AVAILABLE;
  MemRangDesc[MemRangIndex].Base = 0;
  if ((UmaSize != 0) && (UmaBase <= TOM1)) {
    // The case of UMA under 4GB
    MemRangDesc[MemRangIndex].Size = UmaBase;
  } else {
    // The case of UMA is not under 4GB
    MemRangDesc[MemRangIndex].Size = TOM1;
  }
  MemRangIndex++;
  //
  // Memory Rang 1 : 4G - TOM2 (exclude UMA)
  //
  if ((TOM2 > FOURGB) && (UmaSize < (TOM2 - FOURGB))) {
    MemRangDesc[MemRangIndex].Attribute = AMD_MEMORY_ATTRIBUTE_AVAILABLE;
    if ((UmaSize != 0) && (UmaBase > TOM1)) {
      // The case of UMA is above 4GB
      MemRangDesc[MemRangIndex].Size = TOM2 - UmaSize - FOURGB;
      if ((TOM2 - UmaBase) == UmaSize) {
        // The case of UMA base is near TOM2
        MemRangDesc[MemRangIndex].Base = FOURGB;
      } else {
        // The case of UMA base is at beginning of 4GB
        MemRangDesc[MemRangIndex].Base = UmaBase + UmaSize;
      }
    } else {
      // The case of UMA is not above 4GB
      MemRangDesc[MemRangIndex].Size = TOM2 - FOURGB;
      MemRangDesc[MemRangIndex].Base = FOURGB;
    }
    MemRangIndex++;
  }
  //
  // Memory Rang 2 : UMA base - UMA top
  //
  if (UmaSize != 0) {
    // UMA is enabled
    MemRangDesc[MemRangIndex].Attribute = AMD_MEMORY_ATTRIBUTE_RESERVED | AMD_MEMORY_ATTRIBUTE_UMA;
    MemRangDesc[MemRangIndex].Base = UmaBase;
    MemRangDesc[MemRangIndex].Size = UmaSize;
    MemRangIndex++;
  }

  MemInfoHob->NumberOfDescriptor = MemRangIndex - 1;
  SizeOfMemInfoHob = sizeof (AMD_MEMORY_INFO_HOB) + MemInfoHob->NumberOfDescriptor * sizeof (AMD_MEMORY_RANGE_DESCRIPTOR);

  Status = PeiBuildHobGuidData (PeiServices, &gAmdMemoryInfoHobGuid, &MemInfoHobBuffer, SizeOfMemInfoHob);

  return Status;
}

EFI_STATUS
InitializeMemory (
  IN  EFI_PEI_SERVICES                **PeiServices
  )
/*++

Routine Description:

  This function invokes the AmdInitPost entry point to initialize memory.

Arguments:

  PeiServices - PerServices

Returns:
  EFI_STATUS - Status code
--*/
{
  EFI_STATUS            Status;
  AGESA_STATUS          AgStatus;
  AMD_INTERFACE_PARAMS  AmdInterfaceParams;

  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  AmdInterfaceParams.AllocationMethod = PreMemHeap;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_POST;
  AgStatus = AmdCreateStruct (&AmdInterfaceParams);

  Status = EFI_SUCCESS;

  // OEM Should Customize the defaults through this hook
  OemCustomizeInitPost (PeiServices, (AMD_POST_PARAMS *)AmdInterfaceParams.NewStructPtr);

  //
  // Mem Auto Init performs the memory Initialization. There will be 3 call outs
  // during this initialization. Need to update the details here.
  //
  AgStatus = AmdInitPost ((AMD_POST_PARAMS *)AmdInterfaceParams.NewStructPtr);

  OemHookAfterInitPost (PeiServices, (AMD_POST_PARAMS *)AmdInterfaceParams.NewStructPtr);

  //
  // Install PPI
  //
  if ((AgStatus == AGESA_CRITICAL) || (AgStatus == AGESA_FATAL)) {
    return EFI_DEVICE_ERROR;
  }

  Status = BuildHobInfo (PeiServices, (AMD_POST_PARAMS *)AmdInterfaceParams.NewStructPtr);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = (**PeiServices).InstallPpi (PeiServices, &mInitPostPpiList);
  return Status;
}

EFI_STATUS
InvokeAmdInitS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
/*++

Routine Description:

  This function gets invoked as part of PeiPermanentMemoryInstallPpi Notification
  once PEI have permannet memory installed during S3 boot mode  to perfomr S3LateRestore

Arguments:

  PeiServices - PerServices
  NotifyDesc  - NotifyDesc
  InvokePpi   - Ppi to be invoked.

Returns:
  EFI_STATUS - Status code
--*/
{
  AGESA_STATUS            AgStatus;
  AMD_INTERFACE_PARAMS    AmdInterfaceParams;
  EFI_STATUS              Status;
  S3_DATA_TYPE            S3DataType;
  AMD_S3LATE_PARAMS       AmdS3LateParams;
  AMD_S3LATE_PARAMS       *AmdS3LateParamsPtr;

  //
  // Perform AmdS3LateRestore
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  AmdInterfaceParams.AllocationMethod = ByHost;
  AmdInterfaceParams.AgesaFunctionName = AMD_S3LATE_RESTORE;
  AmdInterfaceParams.NewStructPtr = &AmdS3LateParams;
  AmdS3LateParamsPtr = &AmdS3LateParams;

  //
  // Initialize the content of AmdS3LateParams to zero
  //
  (**PeiServices).SetMem (AmdS3LateParamsPtr, sizeof (AMD_S3LATE_PARAMS), 0);

  AmdInterfaceParams.NewStructSize = sizeof (AMD_S3LATE_PARAMS);

  AgStatus = AmdCreateStruct (&AmdInterfaceParams);

  AmdS3LateParamsPtr->S3DataBlock.VolatileStorageSize = 0;

  S3DataType = S3DataTypeVolatile;

  Status = OemAgesaGetS3Info (
              PeiServices,
              S3DataType,
              &AmdS3LateParamsPtr->S3DataBlock.VolatileStorageSize,
              &AmdS3LateParamsPtr->S3DataBlock.VolatileStorage
              );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Customize S3Late Restore parameters
  //
  OemCustomizeS3LateRestore (PeiServices, AmdS3LateParamsPtr);

  AgStatus = AmdS3LateRestore (AmdS3LateParamsPtr);

  OemHookAfterS3LateRestore (PeiServices, AmdS3LateParamsPtr);

  return EFI_SUCCESS;
}

EFI_STATUS
InvokeAmdInitS3FinalRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
/*++

Routine Description:

  This function gets invoked as part of PeiPostBootScriptTableInstallPpi Notification
  once the platform BIOS has run the resume script table

Arguments:

  PeiServices - PerServices
  NotifyDesc  - NotifyDesc
  InvokePpi   - Ppi to be invoked.

Returns:
  EFI_STATUS - Status code
--*/
{
  AGESA_STATUS          AgStatus;
  AMD_INTERFACE_PARAMS  AmdInterfaceParams;
  EFI_STATUS            Status;
  S3_DATA_TYPE          S3DataType;
  AMD_S3FINAL_PARAMS    AmdS3FinalParams;
  AMD_S3FINAL_PARAMS    *AmdS3FinalParamsPtr;

  //
  // Perform AmdS3FinalRestore
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  AmdInterfaceParams.AllocationMethod = ByHost;
  AmdInterfaceParams.AgesaFunctionName = AMD_S3FINAL_RESTORE;
  AmdInterfaceParams.NewStructPtr = &AmdS3FinalParams;
  AmdS3FinalParamsPtr = &AmdS3FinalParams;

  //
  // Initialize the content of AmdS3FinalParams to zero
  //
  (**PeiServices).SetMem (AmdS3FinalParamsPtr, sizeof (AMD_S3FINAL_PARAMS), 0);

  AmdInterfaceParams.NewStructSize = sizeof (AMD_S3FINAL_PARAMS);

  AgStatus = AmdCreateStruct (&AmdInterfaceParams);

  AmdS3FinalParamsPtr->S3DataBlock.VolatileStorageSize = 0;

  S3DataType = S3DataTypeVolatile;

  Status = OemAgesaGetS3Info (
              PeiServices,
              S3DataType,
              &AmdS3FinalParamsPtr->S3DataBlock.VolatileStorageSize,
              &AmdS3FinalParamsPtr->S3DataBlock.VolatileStorage
              );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Customize S3 post script Restore parameters
  //
  OemCustomizeS3FinalRestore (PeiServices, AmdS3FinalParamsPtr);

  AgStatus = AmdS3FinalRestore (AmdS3FinalParamsPtr);

  OemHookAfterS3FinalRestore (PeiServices, AmdS3FinalParamsPtr);

  return EFI_SUCCESS;
}

EFI_STATUS
InvokeAmdInitResume (
  IN EFI_PEI_SERVICES           **PeiServices
  )
{
  AGESA_STATUS            AgStatus;
  AMD_INTERFACE_PARAMS    AmdInterfaceParams;
  AMD_RESUME_PARAMS       *AmdResumeParamsPtr;
  EFI_STATUS              Status;
  S3_DATA_TYPE            S3DataType;

  //
  // Create structure for S3 Resume.
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  AmdInterfaceParams.AllocationMethod = PreMemHeap;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_RESUME;
  AgStatus = AmdCreateStruct (&AmdInterfaceParams);
  AmdResumeParamsPtr = (AMD_RESUME_PARAMS *)AmdInterfaceParams.NewStructPtr;

  //
  // Initialize all the storage pointer to indicate zero size as safe step.
  //
  AmdResumeParamsPtr->S3DataBlock.NvStorageSize = 0;
  AmdResumeParamsPtr->S3DataBlock.VolatileStorageSize = 0;

  //
  // Perform AmdInitResume
  //
  S3DataType = S3DataTypeNonVolatile;

  Status = OemAgesaGetS3Info (
              PeiServices,
              S3DataType,
              &AmdResumeParamsPtr->S3DataBlock.NvStorageSize,
              &AmdResumeParamsPtr->S3DataBlock.NvStorage
              );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  OemCustomizeInitResume (PeiServices, AmdResumeParamsPtr);

  AgStatus = AmdInitResume (AmdResumeParamsPtr);

  OemHookAfterInitResume (PeiServices, AmdResumeParamsPtr);

  if ((AgStatus == AGESA_CRITICAL) || (AgStatus == AGESA_FATAL)) {
    return EFI_DEVICE_ERROR;
  }

  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyListS3LateRestore);
  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyListS3FinalRestore);

  return Status;
}

EFI_STATUS
AmdCpuInitialize (
  IN EFI_PEI_SERVICES           **PeiServices
  )
{
  AGESA_STATUS            AgStatus;
  AMD_INTERFACE_PARAMS    AmdInterfaceParams;
  AMD_EARLY_PARAMS        *AmdEarlyParamsPtr;
  AMD_RESET_PARAMS        AmdResetParams = {0};

  //
  // OEM Should Customize the defaults through this hook.This should
  // be done only for BSP. AP will not need this. Also there will be
  // no PeiServices pointer for AP and they don't require
  // because there is no PEI Core stack for AP.
  //
  if (!IsBsp (&AmdInterfaceParams.StdHeader, &AgStatus)) {
    PeiServices = NULL;
  }

  //
  // Create a structure for Reset Init.
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  //
  // The data structure has been intialized during variable declaration
  // since allocation method which is being used is ByHost.
  //
  AmdInterfaceParams.AllocationMethod = ByHost;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_RESET;
  AmdInterfaceParams.NewStructPtr = &AmdResetParams;
  AmdInterfaceParams.NewStructSize = sizeof (AMD_RESET_PARAMS);
  AgStatus = AmdCreateStruct (&AmdInterfaceParams);

  OemCustomizeInitReset (PeiServices, &AmdResetParams);

  AgStatus = AmdInitReset (&AmdResetParams);

  OemHookAfterInitReset (PeiServices, &AmdResetParams);
  //
  // Install PPI
  //
  if ((AgStatus == AGESA_CRITICAL) || (AgStatus == AGESA_FATAL)) {
    return EFI_DEVICE_ERROR;
  }

  AmdReleaseStruct (&AmdInterfaceParams);

  if (IsBsp (&AmdInterfaceParams.StdHeader, &AgStatus)) {
    (**PeiServices).InstallPpi (PeiServices, &mInitResetPpiList);
  }

  //
  // Perform CPU INIT here
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = (UINT32) (PeiServices);
  AmdInterfaceParams.AllocationMethod = PreMemHeap;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_EARLY;
  AgStatus = AmdCreateStruct (&AmdInterfaceParams);
  AmdEarlyParamsPtr = (AMD_EARLY_PARAMS *)AmdInterfaceParams.NewStructPtr;

  //
  // OEM Should Customize the defaults through this hook.This should
  // be done only for BSP. AP will not need this. Also there will be
  // no PeiServices pointer for AP and they don't require
  // because there is no PEI Core stack for AP.
  //
  if (!IsBsp (&AmdInterfaceParams.StdHeader, &AgStatus)) {
    PeiServices = NULL;
  }

  OemCustomizeInitEarly (PeiServices, AmdEarlyParamsPtr);

  AgStatus = AmdInitEarly (AmdEarlyParamsPtr);

  OemHookAfterInitEarly (PeiServices, AmdEarlyParamsPtr);

  if ((AgStatus == AGESA_CRITICAL) || (AgStatus == AGESA_FATAL)) {
    return EFI_DEVICE_ERROR;
  }

  AmdReleaseStruct (&AmdInterfaceParams);
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
InitializeAmdProcessorInitPeim (
  IN EFI_FFS_FILE_HEADER        *FfsHeader,
  IN EFI_PEI_SERVICES           **PeiServices
  )
/*++

Routine Description:

  Initializes AGESA CPU Component.

Arguments:

Returns:

  EFI_STATUS

--*/
{
  EFI_STATUS             Status;
  UINT32                 BspApInit;
  UINT32                 ContentToRestore;

  //
  // Now Save the pointer to launch BSP and AP for AmdInitReset and AmdInitEarly
  //
  BspApInit = (UINT32)AmdCpuInitialize;

  //
  // Call the platform hook to save the Vector which can be
  // used to perform the ApInit
  //
  ContentToRestore = 0;
  SaveApInitVector (BspApInit, &ContentToRestore);

  Status = AmdCpuInitialize (PeiServices);

  if (EFI_ERROR (Status)) {
    return Status;
  }


  //
  // Call the platform hook to restore the previous Vector which was saved
  // previously
  //
  RestoreContentVector (ContentToRestore);

  //
  // Install PPI to help notify any consumer registered for AmdInitEarly
  //
  Status = (**PeiServices).InstallPpi (PeiServices, &mInitEarlyPpiList);

  Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyList);
  return Status;
}


EFI_STATUS
DiscoverMemory (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
/*++

Routine Description:

  This function gets invoked as part of AgesaMemPpi Notification by AmdInitPost PEIM.

Arguments:

  PeiServices - PerServices
  NotifyDesc  - NotifyDesc
  InvokePpi   - Ppi to be invoked.

Returns:
  EFI_STATUS - Status code
--*/
{
  EFI_BOOT_MODE     BootMode;
  EFI_STATUS        Status;

  Status = (*PeiServices)->GetBootMode (
             PeiServices,
             &BootMode
             );
  //
  // If not in S3, initialize memory otherwise perform S3 Resume.
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = InitializeMemory (PeiServices);
  } else {
    Status = InvokeAmdInitResume (PeiServices);
  }
  return Status;
}

