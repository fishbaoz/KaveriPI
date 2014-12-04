/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Initialization
 *
 * Contains CPU code to perform CPU initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdAgesaDxeDriver.c - AGESA DXE Driver to perform Mid and Late
 * Init.
 *
 ******************************************************************************
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdAgesaDxeDriver.h"
#include "OemAgesaCustomizeDxe.h"
#include "Filecode.h"
#include <Protocol/AcpiTable.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#define FILECODE UEFI_DXE_AMDAGESADXEDRIVER_AMDAGESADXEDRIVER_FILECODE

// Consumed protocols
#include <Protocol\PciIo.h>

//GUID Definitions
#include <Guid\AmdHeapHob.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_GUID  gAcpiTableProtocolGuid = EFI_ACPI_TABLE_PROTOCOL_GUID;
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
AGESA_DXE_PRIVATE_DATA                *mAgesaDxePrivate;
AGESA_DXE_BUFFER_MANAGER              *AgesaBufferManager;
EFI_EVENT                             PciIoEvent;
EFI_EVENT                             AmdInitMidEvent;
VOID                                  *mRegistrationForPciIo;
VOID                                  *mRegistrationForAmdInitMid;
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
AmdMmioMapManager (
  IN       AMD_AGESA_MMIO_PROTOCOL    *This,
  IN       UINT64                     BaseAddress,
  IN       UINT64                     Length,
  IN       PCI_ADDR                   TargetAddress,
  IN       AMD_MMIO_ATTRIBUTE         Attributes
  );

EFI_STATUS
EFIAPI
AmdCpuCreateProcessorTables (
  IN       AMD_AGESA_DXE_PROTOCOL       *This,
  IN       AMD_PROCESSOR_TABLE_TYPE     TableType,
  IN OUT   UINTN                        *BufferLength,
  IN OUT   VOID                         **BufferPtr
  );

VOID
EFIAPI
InvokeAmdInitMid (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
InvokeAmdInitLate (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
AgesaS3Save (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

EFI_STATUS
RebuildHeap (
  IN OUT   HEAP_MANAGER                 **HeapManagerPtr
  );

EFI_STATUS
AgesaDxeCallout (
  IN       UINT32       Param1,
  IN       UINTN        Param2,
  IN OUT   VOID         *ConfigPtr
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  AgesaDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the cpu driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AgesaDxeDriverEntryPoint (
  IN       EFI_HANDLE                       ImageHandle,
  IN       EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS                  Status;
  AMD_INTERFACE_PARAMS        AmdInterfaceParams;
  AGESA_STATUS                AgesaStatus;
  AMD_ENV_PARAMS              AmdInitEnvParams;
  EFI_EVENT                   ReadyToBootEvent;
  EFI_HANDLE                  Handle;
  AMD_BUFFER_MANAGER_PROTOCOL *BufferMgr;

  //
  // Initialize Global Variable
  //
  //
  // Initialize the configuration structure and private data area
  //
  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AGESA_DXE_PRIVATE_DATA),
                  &mAgesaDxePrivate);

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Install the AMD_AGESA_MMIO_PROTOCOL
  //
  mAgesaDxePrivate->MmioMapManager.AmdMmioMapManager = AmdMmioMapManager;

  Handle = NULL;
  gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdAgesaMmioProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAgesaDxePrivate->MmioMapManager
                  );
  //
  // Initialize the private data structure
  //
  mAgesaDxePrivate->Signature = AGESA_DXE_PRIVATE_DATA_SIGNATURE;

  //
  // Create the first buffer for AGESA_ALLOCATE_BUFFER Manager
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AGESA_DXE_BUFFER_MANAGER),
                  &mAgesaDxePrivate->BufferManagerPtr
                  );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (mAgesaDxePrivate->BufferManagerPtr, sizeof (AGESA_DXE_BUFFER_MANAGER));
  mAgesaDxePrivate->BufferManagerPtr->NextAgesaBufferManagerPtr = NULL;
  AgesaBufferManager = mAgesaDxePrivate->BufferManagerPtr;

  //
  // Rebuild persist heap from HOB.
  //
  Status = RebuildHeap (&mAgesaDxePrivate->HeapManagerPtr);
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Invoke AmdInitEnv
  //
  ZeroMem (&AmdInterfaceParams, sizeof (AMD_INTERFACE_PARAMS));
  ZeroMem (&AmdInitEnvParams, sizeof (AMD_ENV_PARAMS));
  AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
  AmdInterfaceParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
  AmdInterfaceParams.AllocationMethod = ByHost;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_ENV;
  AmdInterfaceParams.NewStructPtr = &AmdInitEnvParams;
  AmdInterfaceParams.NewStructSize = sizeof (AMD_ENV_PARAMS);
  AgesaStatus = AmdCreateStruct (&AmdInterfaceParams);

  AmdInitEnvParams.StdHeader = AmdInterfaceParams.StdHeader;

  OemCustomizeInitEnv (gBS, &AmdInitEnvParams);

  AgesaStatus = AmdInitEnv (&AmdInitEnvParams);

  OemHookAfterInitEnv (gBS, &AmdInitEnvParams);

  if ((AgesaStatus == AGESA_CRITICAL) || (AgesaStatus == AGESA_FATAL)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Install AmdBufferManagerProtocol which notifies other dependent drivers
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AMD_BUFFER_MANAGER_PROTOCOL),
                  &BufferMgr
                  );
  ASSERT_EFI_ERROR (Status);

  BufferMgr->StdHeader = AmdInitEnvParams.StdHeader;
  BufferMgr->DxeBufferManager = mAgesaDxePrivate->BufferManagerPtr;
  BufferMgr->AmdBufferCallout = AgesaDxeCallout;

  Handle = NULL;
  gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdBufferManagerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  BufferMgr
                  );


  //
  // Register the event handling function for AmdInitMid to be launched after
  // PciIo protocol
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             InvokeAmdInitMid,
             NULL,
             &PciIoProtocolInstallEventGuid,
             &PciIoEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciIoProtocolGuid,
              PciIoEvent,
              &mRegistrationForPciIo
              );

  //
  // Set up call back for AmdInitLate after AmdInitMid has been launched.
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             InvokeAmdInitLate,
             NULL,
             &AmdInitMidProtocolInstallEventGuid,
             &AmdInitMidEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gAmdDxeInitMidProtocolGuid,
              AmdInitMidEvent,
              &(mRegistrationForAmdInitMid)
              );


  //
  // Initialize AMD CPU Interface protocol
  //
  mAgesaDxePrivate->CpuInterface.CreateProcessorTables = AmdCpuCreateProcessorTables;

  // S3 Interface
  // Register the event handling function to produce
  // resulting Smbios image.
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             AgesaS3Save,
             NULL,
             &gEfiEventReadyToBootGuid,
             &ReadyToBootEvent
             );

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AgesaDxeCallout
 *
 *  Description:
 *     AGESA DXE Call out dispatcher
 *
 *  Parameters:
 *    @param[in]         Param1
 *    @param[in]         Param2
 *    @param[in, out]    *ConfigPtr
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AgesaDxeCallout (
  IN       UINT32       Param1,
  IN       UINTN        Param2,
  IN OUT   VOID         *ConfigPtr
  )
{
  EFI_STATUS                Status;
  AGESA_STATUS              AgesaStatus;
  AGESA_BUFFER_PARAMS       *BufferConfigPtr;
  AGESA_DXE_BUFFER_MANAGER  *AgesaLocateBufferPtr;
  AGESA_DXE_BUFFER_MANAGER  *PreviousBufferPtr;

  Status = EFI_UNSUPPORTED;
  AgesaStatus = AGESA_UNSUPPORTED;
  BufferConfigPtr = (AGESA_BUFFER_PARAMS *) ConfigPtr;
  AgesaLocateBufferPtr = NULL;
  PreviousBufferPtr = NULL;

  switch (Param1) {
  case AGESA_ALLOCATE_BUFFER:
    if (Param2 == HEAP_CALLOUT_BOOTTIME) {
      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      BufferConfigPtr->BufferLength,
                      &BufferConfigPtr->BufferPointer
                      );
    } else if (Param2 == HEAP_CALLOUT_RUNTIME) {
      Status = gBS->AllocatePool (
                      EfiACPIMemoryNVS,
                      BufferConfigPtr->BufferLength,
                      &BufferConfigPtr->BufferPointer
                      );
    }
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // Initialize the memory
    //
    ZeroMem (BufferConfigPtr->BufferPointer, BufferConfigPtr->BufferLength);

    //
    // Create the first buffer for AGESA_ALLOCATE_BUFFER Manager
    //
    AgesaBufferManager->BufferLength = BufferConfigPtr->BufferLength;
    AgesaBufferManager->BufferHandle = BufferConfigPtr->BufferHandle;
    AgesaBufferManager->BufferPtr = BufferConfigPtr->BufferPointer;

    Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AGESA_DXE_BUFFER_MANAGER),
                  &AgesaBufferManager->NextAgesaBufferManagerPtr
                  );

    ASSERT_EFI_ERROR (Status);

    if (EFI_ERROR (Status)) {
      return Status;
    }
    ZeroMem (AgesaBufferManager->NextAgesaBufferManagerPtr, sizeof (AGESA_DXE_BUFFER_MANAGER));
    AgesaBufferManager = AgesaBufferManager->NextAgesaBufferManagerPtr;
    AgesaBufferManager->NextAgesaBufferManagerPtr = NULL;
    break;

  case AGESA_DEALLOCATE_BUFFER:
    AgesaLocateBufferPtr = mAgesaDxePrivate->BufferManagerPtr;
    while (AgesaLocateBufferPtr->NextAgesaBufferManagerPtr != NULL) {
      if (AgesaLocateBufferPtr->BufferHandle == BufferConfigPtr->BufferHandle) {
        //
        // Before deleting the current buffer from the link, setup the link from
        // previous to next buffer pointer to maintain the continuity of the link
        // list.
        //
        PreviousBufferPtr->NextAgesaBufferManagerPtr = AgesaLocateBufferPtr->NextAgesaBufferManagerPtr;
        Status = gBS->FreePool (AgesaLocateBufferPtr);
        break;
      }
      PreviousBufferPtr = AgesaLocateBufferPtr;
      AgesaLocateBufferPtr = AgesaLocateBufferPtr->NextAgesaBufferManagerPtr;
    }
    break;

  case AGESA_LOCATE_BUFFER:
    AgesaLocateBufferPtr = mAgesaDxePrivate->BufferManagerPtr;
    while (AgesaLocateBufferPtr->NextAgesaBufferManagerPtr != NULL) {
      if (AgesaLocateBufferPtr->BufferHandle == BufferConfigPtr->BufferHandle) {
        BufferConfigPtr->BufferPointer = AgesaLocateBufferPtr->BufferPtr;
        BufferConfigPtr->BufferLength = (UINT32) AgesaLocateBufferPtr->BufferLength;
        Status = EFI_SUCCESS;
        break;
      }
      AgesaLocateBufferPtr = AgesaLocateBufferPtr->NextAgesaBufferManagerPtr;
    }
    break;

  default:
    break;
  }
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdMmioMapManager
 *
 *  Description:
 *     Final CPU initialization before passing control to the OS through BDS
 *
 *  Parameters:
 *    @param[in]         *This
 *    @param[in]         BaseAddress        Starting address of the requested MMIO range.
 *    @param[in]         Length             Length of the requested MMIO range.
 *    @param[in]         TargetAddress      PCI address of the requestor.
 *    @param[in]         Attributes         Attributes of the requested MMIO range indicating whether
 *                                          it is readable/writable/posted/secured/s3saved.
 *
 *    @retval            EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
AmdMmioMapManager (
  IN       AMD_AGESA_MMIO_PROTOCOL    *This,
  IN       UINT64                     BaseAddress,
  IN       UINT64                     Length,
  IN       PCI_ADDR                   TargetAddress,
  IN       AMD_MMIO_ATTRIBUTE         Attributes
  )
{
  EFI_STATUS         Status;
  AMD_ADD_MMIO_PARAMS AmdAddMmioParams;

  Status = EFI_SUCCESS;

  // Initialize StdHeader
  AmdAddMmioParams.StdHeader.ImageBasePtr = 0;
  AmdAddMmioParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
  AmdAddMmioParams.StdHeader.HeapBasePtr = 0;

  AmdAddMmioParams.BaseAddress = BaseAddress;
  AmdAddMmioParams.Length = Length;
  AmdAddMmioParams.TargetAddress = TargetAddress;
  AmdAddMmioParams.Attributes = Attributes;
  if (AmdAddMmioMapping (AmdAddMmioParams) != AGESA_SUCCESS) {
    Status = EFI_OUT_OF_RESOURCES;
  }
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdCpuCreateProcessorTables
 *
 *  Description:
 *     Final CPU initialization before passing control to the OS through BDS
 *
 *  Parameters:
 *    @param[in]         *This
 *    @param[in]         TableType
 *    @param[in, out]    *BufferLength
 *    @param[in, out]    **BufferPtr
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
AmdCpuCreateProcessorTables (
  IN       AMD_AGESA_DXE_PROTOCOL     *This,
  IN       AMD_PROCESSOR_TABLE_TYPE   TableType,
  IN OUT   UINTN                      *BufferLength,
  IN OUT   VOID                       **BufferPtr
  )
{
  EFI_STATUS                          Status;
  AMD_LATE_PARAMS                     *LateParamsPtr;
  AGESA_DXE_PRIVATE_DATA              *AgesaDxePrivate;


  Status = EFI_SUCCESS;

  if (BufferPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  AgesaDxePrivate = AGESA_DXE_PRIVATE_DATA_INSTANCE_FROM_THIS (This);
  LateParamsPtr = mAgesaDxePrivate->LateParamsPtr;

  switch (TableType) {

  case UndefinedProcessorTableType:
    Status = EFI_UNSUPPORTED;
    break;

  case AcpiPstateType:
    if ((LateParamsPtr->PlatformConfig.UserOptionPState) ||
      (IsFeatureEnabled (IoCstate, &LateParamsPtr->PlatformConfig, &LateParamsPtr->StdHeader))) {

      if (*((UINT32 *) LateParamsPtr->AcpiPState) == 'TDSS') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiPState + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiPState + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiPState,
                 *((UINT32 *) LateParamsPtr->AcpiPState + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiSratType:
    if (LateParamsPtr->PlatformConfig.UserOptionSrat) {
      if (*((UINT32 *) LateParamsPtr->AcpiSrat) == 'TARS') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiSrat + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiSrat + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiSrat,
                 *((UINT32 *) LateParamsPtr->AcpiSrat + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiSlitType:
    if (LateParamsPtr->PlatformConfig.UserOptionSlit) {
      if (*((UINT32 *) LateParamsPtr->AcpiSlit) == 'TILS') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiSlit + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiSlit + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiSlit,
                 *((UINT32 *) LateParamsPtr->AcpiSlit + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiWheaHestCmcType:
    if (LateParamsPtr->PlatformConfig.UserOptionWhea) {
      if (*BufferLength < *((UINT16 *) LateParamsPtr->AcpiWheaCmc)) {
        *BufferLength = *((UINT16 *) LateParamsPtr->AcpiWheaCmc);
        Status = EFI_BUFFER_TOO_SMALL;
      } else {
        gBS->CopyMem (
          *BufferPtr,
          LateParamsPtr->AcpiWheaCmc,
          *((UINT16 *) LateParamsPtr->AcpiWheaCmc)
          );
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiWheaHestMceType:
    if (LateParamsPtr->PlatformConfig.UserOptionWhea) {
      if (*BufferLength < *((UINT16 *) LateParamsPtr->AcpiWheaMce)) {
        *BufferLength = *((UINT16 *) LateParamsPtr->AcpiWheaMce);
        Status = EFI_BUFFER_TOO_SMALL;
      } else {
        gBS->CopyMem (
          *BufferPtr,
          LateParamsPtr->AcpiWheaMce,
          *((UINT16 *)LateParamsPtr->AcpiWheaMce)
          );
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case DmiTableType:
    if (LateParamsPtr->PlatformConfig.UserOptionDmi) {
      if (*BufferLength < sizeof (DMI_INFO)) {
        *BufferLength = sizeof (DMI_INFO);
        Status = EFI_BUFFER_TOO_SMALL;
      } else if (*BufferPtr != NULL) {
        gBS->CopyMem (
               *BufferPtr,
               LateParamsPtr->DmiTable,
               sizeof (DMI_INFO)
               );
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiAlibType:
    if (LateParamsPtr->AcpiAlib != NULL) {
      if (*((UINT32 *) LateParamsPtr->AcpiAlib) == 'TDSS') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiAlib + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiAlib + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiAlib,
                 *((UINT32 *) LateParamsPtr->AcpiAlib + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;
  case AcpiIvrsType:
    if (LateParamsPtr->AcpiIvrs != NULL) {
      if (*((UINT32 *) LateParamsPtr->AcpiIvrs) == 'SRVI') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiIvrs + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiIvrs + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiIvrs,
                 *((UINT32 *) LateParamsPtr->AcpiIvrs + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiCratType:
    if (LateParamsPtr->PlatformConfig.UserOptionCrat) {
      if (*((UINT32 *) LateParamsPtr->AcpiCrat) == 'TARC') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiCrat + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiCrat + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiCrat,
                 *((UINT32 *) LateParamsPtr->AcpiCrat + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  case AcpiCditType:
    if (LateParamsPtr->PlatformConfig.UserOptionCdit) {
      if (*((UINT32 *) LateParamsPtr->AcpiCdit) == 'TIDC') {
        if (*BufferLength < *((UINT32 *) LateParamsPtr->AcpiCdit + 1)) {
          *BufferLength = *((UINT32 *) LateParamsPtr->AcpiCdit + 1);
          Status = EFI_BUFFER_TOO_SMALL;
        } else if (*BufferPtr != NULL) {
          gBS->CopyMem (
                 *BufferPtr,
                 LateParamsPtr->AcpiCdit,
                 *((UINT32 *) LateParamsPtr->AcpiCdit + 1)
                 );
        }
      }
    } else {
      Status = EFI_ACCESS_DENIED;
    }
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

AGESA_STATUS
AgesaAllocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *AllocParams
  )
{
  AGESA_STATUS Status;
  EFI_STATUS   EfiStatus;
  Status = AGESA_SUCCESS;

  EfiStatus =  AgesaDxeCallout (AGESA_ALLOCATE_BUFFER, FcnData, (VOID *) AllocParams);

  if (EFI_ERROR (EfiStatus)) {
    return AGESA_ERROR;
  }
  return Status;
}

AGESA_STATUS
AgesaDeallocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *DeallocParams
  )
{
  AGESA_STATUS Status;
  EFI_STATUS   EfiStatus;

  Status = AGESA_SUCCESS;

  EfiStatus =  AgesaDxeCallout (AGESA_DEALLOCATE_BUFFER, FcnData, (VOID *) DeallocParams);

  if (EFI_ERROR (EfiStatus)) {
    return AGESA_ERROR;
  }
  return Status;
}

AGESA_STATUS
AgesaLocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *LocateParams
  )
{
  AGESA_STATUS Status;
  EFI_STATUS   EfiStatus;
  Status = AGESA_SUCCESS;

  EfiStatus =  AgesaDxeCallout (AGESA_LOCATE_BUFFER, FcnData, (VOID *) LocateParams);

  if (EFI_ERROR (EfiStatus)) {
    return AGESA_ERROR;
  }
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InvokeAmdInitMid
 *
 *  Description:
 *     Calls AmdInitMid as part of Agesa DXE Driver Initialization.
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
EFIAPI
InvokeAmdInitMid (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  Installs the AmdInitMid This function gets called
  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

Arguments & Return Values: Standard event handling function prototype

--*/
{
  AMD_INTERFACE_PARAMS        AmdInterfaceParams;
  AGESA_STATUS                AgesaStatus;
  AMD_DXE_INIT_MID_PROTOCOL   AmdInitMidProtocol;
  EFI_HANDLE                  Handle;
  STATIC BOOLEAN              InitMidInvoked = FALSE;

  //
  // Prepare for AmdInitMid
  //
  if (!InitMidInvoked) {
    ZeroMem (&AmdInterfaceParams, sizeof (AMD_INTERFACE_PARAMS));
    AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
    AmdInterfaceParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
    AmdInterfaceParams.AllocationMethod = PostMemDram;
    AmdInterfaceParams.AgesaFunctionName = AMD_INIT_MID;
    AgesaStatus = AmdCreateStruct (&AmdInterfaceParams);

    ((AMD_MID_PARAMS *)AmdInterfaceParams.NewStructPtr)->StdHeader = AmdInterfaceParams.StdHeader;

    OemCustomizeInitMid (gBS, (AMD_MID_PARAMS *)AmdInterfaceParams.NewStructPtr);

    AgesaStatus = AmdInitMid ((AMD_MID_PARAMS *)AmdInterfaceParams.NewStructPtr);

    OemHookAfterInitMid (gBS, (AMD_MID_PARAMS *)AmdInterfaceParams.NewStructPtr);

    if ((AgesaStatus == AGESA_CRITICAL) || (AgesaStatus == AGESA_FATAL)) {
      return;
    }
    AmdInitMidProtocol.Revision = AGESA_DXE_INIT_MID_REV;
    Handle = NULL;
    gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdDxeInitMidProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &AmdInitMidProtocol
                  );
  }
  InitMidInvoked = TRUE;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  InvokeAmdInitLate
 *
 *  Description:
 *     Calls AmdInitLate as part of Agesa DXE Driver Initialization.
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

VOID
EFIAPI
InvokeAmdInitLate (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  Invoke AmdinitLate entry point. This function gets called
  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

Arguments & Return Values: Standard event handling function prototype

--*/
{
  AMD_INTERFACE_PARAMS        AmdInterfaceParams;
  AGESA_STATUS                AgesaStatus;
  AMD_DXE_INIT_LATE_PROTOCOL  AmdInitLateProtocol;
  AMD_AGESA_DXE_PROTOCOL      *AmdAgesaDxe;
  EFI_HANDLE                  Handle;
  UINTN                       TableBufferLength;
  UINT8                       *TableBufferPtr;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  UINTN                       TableKey;
  EFI_STATUS                  Status;

  //
  // Prepare for AmdInitLate
  //
  ZeroMem (&AmdInterfaceParams, sizeof (AMD_INTERFACE_PARAMS));
  AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
  AmdInterfaceParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
  AmdInterfaceParams.AllocationMethod = PostMemDram;
  AmdInterfaceParams.AgesaFunctionName = AMD_INIT_LATE;
  AgesaStatus = AmdCreateStruct (&AmdInterfaceParams);

  ((AMD_LATE_PARAMS *)AmdInterfaceParams.NewStructPtr)->StdHeader = AmdInterfaceParams.StdHeader;

  OemCustomizeInitLate (gBS, (AMD_LATE_PARAMS *)AmdInterfaceParams.NewStructPtr);

  AgesaStatus = AmdInitLate ((AMD_LATE_PARAMS *)AmdInterfaceParams.NewStructPtr);

  OemHookAfterInitLate (gBS, (AMD_LATE_PARAMS *)AmdInterfaceParams.NewStructPtr);

  if ((AgesaStatus == AGESA_CRITICAL) || (AgesaStatus == AGESA_FATAL)) {
    return;
  }

  mAgesaDxePrivate->LateParamsPtr = (AMD_LATE_PARAMS *)AmdInterfaceParams.NewStructPtr;

  //
  // Now Install the AmdDxeInitLateProtocol which helps notify any consumer
  // which is depending upon it.
  //
  AmdInitLateProtocol.Revision = AGESA_DXE_INIT_LATE_REV;
  Handle = NULL;
  gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdDxeInitLateProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &AmdInitLateProtocol
                  );
  //
  // Install the AMD_AGESA_DXE_PROTOCOL to notify OEM to override default value
  //
  Handle = NULL;

  gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdAgesaDxeProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAgesaDxePrivate->CpuInterface
                  );

  //Publish ACPI TABLES.
  (UINT8*)TableBufferPtr = NULL;
  TableKey = NULL;
  AcpiTable = NULL;
  AmdAgesaDxe = NULL;

  //By default PCD is set to FALSE. Before Enabling PCD make sure to disable publishing of ACPI Tables by IBVs to avoid conflict.
  if (PcdGetBool (PcdPublishAgesaAcpiTable)) {
    Status = gBS->LocateProtocol (
                  &gAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTable
                  );
    ASSERT_EFI_ERROR (Status);

    //-PSTATE
    TableBufferLength = 0;
    mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiPstateType, &TableBufferLength, &TableBufferPtr); //First get the buffer size needed for the table.
    if (TableBufferLength != 0 ) {
      TableBufferPtr = AllocateZeroPool (TableBufferLength);
      mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiPstateType, &TableBufferLength, &TableBufferPtr);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, TableBufferPtr, TableBufferLength, &TableKey);
      FreePool (TableBufferPtr);
    }

    //-Srat
    TableBufferLength = 0;
    mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiSratType, &TableBufferLength, &TableBufferPtr); //First get the buffer size needed for the table.
    if (TableBufferLength != 0 ) {
      TableBufferPtr = AllocateZeroPool (TableBufferLength);
      mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiSratType, &TableBufferLength, &TableBufferPtr);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, TableBufferPtr, TableBufferLength, &TableKey);
      FreePool (TableBufferPtr);
    }

    //-SLIT
    TableBufferLength = 0;
    mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiSlitType, &TableBufferLength, &TableBufferPtr); //First get the buffer size needed for the table.
    if (TableBufferLength != 0 ) {
      TableBufferPtr = AllocateZeroPool (TableBufferLength);
      mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiSlitType, &TableBufferLength, &TableBufferPtr);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, TableBufferPtr, TableBufferLength, &TableKey);
      FreePool (TableBufferPtr);
    }

    //-Crat
    TableBufferLength = 0;
    mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiCratType, &TableBufferLength, &TableBufferPtr); //First get the buffer size needed for the table.
    if (TableBufferLength != 0 ) {
      TableBufferPtr = AllocateZeroPool (TableBufferLength);
      mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiCratType, &TableBufferLength, &TableBufferPtr);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, TableBufferPtr, TableBufferLength, &TableKey);
      FreePool (TableBufferPtr);
    }

    //-Cdit
    TableBufferLength = 0;
    mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiCditType, &TableBufferLength, &TableBufferPtr); //First get the buffer size needed for the table.
    if (TableBufferLength != 0 ) {
      TableBufferPtr = AllocateZeroPool (TableBufferLength);
      mAgesaDxePrivate->CpuInterface.CreateProcessorTables (AmdAgesaDxe, AcpiCditType, &TableBufferLength, &TableBufferPtr);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, TableBufferPtr, TableBufferLength, &TableKey);
      FreePool (TableBufferPtr);
    }
  }
  return;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  RebuildHeap
 *
 *  Description:
 *     Rebuild Heap Manager from HOB
 *     This routine retrieve persist heap data from HOB in 1-byte alignment and
 *     recreate them in system memory in 16-byte alignment.
 *     The heap space is allocated by callout to external manager (IBV) with buffer handle
 *     AMD_HEAP_IN_MAIN_MEMORY_HANDLE.
 *     NOTE: No free space buffer is created, thus no HeapAllocatebuffer call will return a buffer
 *     from this recreated heap space.
 *
 *  Parameters:
 *    @param[in, out]   HeapManagerPtr
 *
 *    @retval           EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
RebuildHeap (
  IN OUT   HEAP_MANAGER                 **HeapManagerPtr
  )
{
  EFI_STATUS                  Status;
  AGESA_STATUS                AgesaStatus;
  UINTN                       AlignTo16ByteInDxeMem;
  UINTN                       TotalAlignTo16ByteInHob;
  EFI_PEI_HOB_POINTERS        Hob;
  VOID                        *HobList;
  VOID                        *HeapBufferInHob;
  HEAP_MANAGER                *HeapManagerInHob;
  BUFFER_NODE                 *HeapHeaderNodeInHob;
  BUFFER_NODE                 *HeapCurrentNodeInHob;
  BUFFER_NODE                 *HeapPreNodeInHob;

  HEAP_MANAGER                *HeapManagerInDxeMem;
  BUFFER_NODE                 *HeapHeaderNodeInDxeMem;
  BUFFER_NODE                 *HeapCurrentNodeInDxeMem;
  BUFFER_NODE                 *HeapPreNodeInDxeMem;
  UINT32                      OffsetOfHeapCurrentNodeInDxeMem;
  EFI_HOB_GUID_TYPE           *GuidHob;

  AGESA_BUFFER_PARAMS         AllocParams;

  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  Hob.Raw = HobList;
  HeapBufferInHob = NULL;

  while (!END_OF_HOB_LIST (Hob)) {
    GuidHob = GetNextGuidHob (&gAmdHeapHobGuid, HobList);
    HeapBufferInHob = GET_GUID_HOB_DATA (GuidHob);

    if (HeapBufferInHob != NULL) {
      HeapManagerInHob = (HEAP_MANAGER *) HeapBufferInHob;
      HeapHeaderNodeInHob = (BUFFER_NODE *) ((UINT8 *) HeapManagerInHob + HeapManagerInHob->FirstActiveBufferOffset);
      HeapCurrentNodeInHob = HeapHeaderNodeInHob;
      //
      // 1. Analyse heap buffer from HOB data to calculate the final size for recreating the heap buffers
      //    Reserve maximum pad size for each heap node.
      // 2. Allocate memory for heap buffers
      // 3. Copying heap manager data from HOB include extracing 1-byte alignment to 16-byte alignment
      //

      //
      // 1. Analyse heap buffer from HOB data to calculate the final size for recreating the heap buffers.
      //    Reserve maximum pad size for each heap node.
      //
      TotalAlignTo16ByteInHob = 0;
      do {
        HeapPreNodeInHob = HeapCurrentNodeInHob;
        TotalAlignTo16ByteInHob += 0xF;
        HeapCurrentNodeInHob = (BUFFER_NODE *) ((UINT8 *) HeapBufferInHob + HeapCurrentNodeInHob->OffsetOfNextNode);
      } while (HeapPreNodeInHob->OffsetOfNextNode != AMD_HEAP_INVALID_HEAP_OFFSET);

      //
      // 2. Allocate memory for heap buffers
      //
      AllocParams.BufferLength = (UINT32) (HeapManagerInHob->UsedSize + TotalAlignTo16ByteInHob + 0x0F);
      AllocParams.BufferHandle = AMD_HEAP_IN_MAIN_MEMORY_HANDLE;
      if ((AgesaStatus = AgesaAllocateBuffer (0, &AllocParams)) != AGESA_SUCCESS) {
        if (AGESA_ERROR > AgesaStatus) {
          return EFI_OUT_OF_RESOURCES;
        }
      }
      ZeroMem (AllocParams.BufferPointer, AllocParams.BufferLength);
      *HeapManagerPtr = AllocParams.BufferPointer;

      //
      // 3. Copying heap manager data from HOB include extracing 1-byte alignment to 16-byte alignment
      //
      HeapManagerInDxeMem = *HeapManagerPtr;
      HeapManagerInDxeMem->FirstActiveBufferOffset = sizeof (HEAP_MANAGER);
      HeapManagerInDxeMem->UsedSize = sizeof (HEAP_MANAGER);
      HeapHeaderNodeInDxeMem = (BUFFER_NODE *) ((UINT8 *) HeapManagerInDxeMem + HeapManagerInDxeMem->FirstActiveBufferOffset);
      OffsetOfHeapCurrentNodeInDxeMem = HeapManagerInDxeMem->FirstActiveBufferOffset;
      HeapCurrentNodeInDxeMem = HeapHeaderNodeInDxeMem;
      HeapCurrentNodeInHob = HeapHeaderNodeInHob;

      HeapPreNodeInHob = NULL;
      do {
        // Create BUFFER_NODE with 16-byte alignment padding considered.
        // The beginning of data buffer is on 16-byte boundary address.
        // The structure of a heap buffer would be looked like below.
        //
        // +---------------------------------------------------------------------------------+
        // | BUFFER_NODE | Pad | IDS SENTINEL ("Head") | Data buffer | IDS SENTINEL ("Tail") |
        // +---------------------------------------------------------------------------------+
        //
        AlignTo16ByteInDxeMem = ((0x10 - (((UINTN) (VOID *) HeapCurrentNodeInDxeMem + sizeof (BUFFER_NODE) + SIZE_OF_SENTINEL) & 0xF)) & 0xF);
        HeapCurrentNodeInDxeMem->BufferHandle = HeapCurrentNodeInHob->BufferHandle;
        HeapCurrentNodeInDxeMem->BufferSize = (UINT32) (HeapCurrentNodeInHob->BufferSize + AlignTo16ByteInDxeMem);
        HeapCurrentNodeInDxeMem->Persist = HeapCurrentNodeInHob->Persist;
        HeapCurrentNodeInDxeMem->PadSize = (UINT8) AlignTo16ByteInDxeMem;
        HeapCurrentNodeInDxeMem->OffsetOfNextNode = OffsetOfHeapCurrentNodeInDxeMem + sizeof (BUFFER_NODE) + HeapCurrentNodeInDxeMem->BufferSize;
        // Copy buffer data
        gBS->CopyMem (
          (UINT8 *) ((UINT8 *) HeapCurrentNodeInDxeMem + sizeof (BUFFER_NODE) + AlignTo16ByteInDxeMem),
          (UINT8 *) ((UINT8 *) HeapCurrentNodeInHob + sizeof (BUFFER_NODE)),
          HeapCurrentNodeInHob->BufferSize
          );
        // Point to the next heap node
        HeapPreNodeInHob = HeapCurrentNodeInHob;
        HeapPreNodeInDxeMem = HeapCurrentNodeInDxeMem;
        HeapCurrentNodeInHob = (BUFFER_NODE *) ((UINT8 *) HeapBufferInHob + HeapCurrentNodeInHob->OffsetOfNextNode);
        HeapCurrentNodeInDxeMem = (BUFFER_NODE *) ((UINT8 *) HeapManagerInDxeMem + HeapCurrentNodeInDxeMem->OffsetOfNextNode);
        OffsetOfHeapCurrentNodeInDxeMem = (UINT32) ((UINTN) HeapCurrentNodeInDxeMem - (UINTN) HeapManagerInDxeMem);

      } while (HeapPreNodeInHob->OffsetOfNextNode != AMD_HEAP_INVALID_HEAP_OFFSET);
      //
      // Finalize the last heap node
      //
      HeapManagerInDxeMem->UsedSize = (UINT32) HeapPreNodeInDxeMem->OffsetOfNextNode;
      HeapPreNodeInDxeMem->OffsetOfNextNode = AMD_HEAP_INVALID_HEAP_OFFSET;

      //
      // Finalize Heap Manager pointer
      // No free buffer node is provide after heap recreation.
      //
      *HeapManagerPtr = HeapManagerInDxeMem;
      HeapManagerInDxeMem->FirstActiveBufferOffset = (UINT32) ((UINT8 *) HeapHeaderNodeInDxeMem - (UINT8 *) HeapManagerInDxeMem);
      HeapManagerInDxeMem->FirstFreeSpaceOffset = AMD_HEAP_INVALID_HEAP_OFFSET;
      HeapManagerInDxeMem->Signature = HeapManagerInHob->Signature;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

VOID
EFIAPI
AgesaS3Save (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
/*++

Routine Description:

  Installs the Smbios image to the System Table. This function gets called
  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

Arguments & Return Values: Standard event handling function prototype

--*/
{
  EFI_STATUS            Status;
  AMD_INTERFACE_PARAMS  AmdInterfaceParams;
  AMD_S3SAVE_PARAMS     *AmdS3SaveParamsPtr;
  AGESA_STATUS          AgesaStatus;
  S3_DATA_TYPE          S3DataType;

  //
  // Prepare for S3Save
  //
  AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
  AmdInterfaceParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
  AmdInterfaceParams.AllocationMethod = PostMemDram;
  AmdInterfaceParams.AgesaFunctionName = AMD_S3_SAVE;

  AgesaStatus = AmdCreateStruct (&AmdInterfaceParams);
  AmdS3SaveParamsPtr = (AMD_S3SAVE_PARAMS *)AmdInterfaceParams.NewStructPtr;
  AmdS3SaveParamsPtr->StdHeader = AmdInterfaceParams.StdHeader;
  //
  // OEM hook to customize any S3 Info required
  //
  OemCustomizeS3Save (gBS, AmdS3SaveParamsPtr);

  AgesaStatus = AmdS3Save (AmdS3SaveParamsPtr);

  if (AgesaStatus !=  AGESA_SUCCESS) {
    return;
  }

  if (AmdS3SaveParamsPtr->S3DataBlock.NvStorageSize != 0) {
    S3DataType = S3DataTypeNonVolatile;

    Status = OemAgesaSaveS3Info (
      S3DataType,
      AmdS3SaveParamsPtr->S3DataBlock.NvStorageSize,
      AmdS3SaveParamsPtr->S3DataBlock.NvStorage
      );
  }

  if (AmdS3SaveParamsPtr->S3DataBlock.VolatileStorageSize != 0) {
    S3DataType = S3DataTypeVolatile;

    Status = OemAgesaSaveS3Info (
      S3DataType,
      AmdS3SaveParamsPtr->S3DataBlock.VolatileStorageSize,
      AmdS3SaveParamsPtr->S3DataBlock.VolatileStorage
      );
  }
}

