/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Initialization
 *
 * Contains CPM code to perform CPM initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Kernel
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
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmCpu.h>
#include <AmdCpmFch.h>
#include <AmdCpmTable.h>


AMD_CPM_TABLE_PROTOCOL    *AmdCpmTableProtocolPtr;
AMD_CPM_NV_DATA_PROTOCOL  *AmdCpmNvDataProtocolPtr;
EFI_EVENT                 ReadyToBootEvent;


EFI_GUID  COMMON_ACPI_DEVICE_GUID = {
  0x5b1e5b38, 0x8ebd, 0x4784, 0x9b, 0x52, 0x38, 0xb2, 0xaa, 0x9b, 0xff, 0x2f
};

VOID
EFIAPI
CpmRegisterKernel (
  IN       VOID                                *This
  );

VOID
EFIAPI
CpmInitLate (
  IN       EFI_EVENT                           Event,
  IN       VOID                                *Context
  );

EFI_STATUS
EFIAPI
CpmAddSsdtTable (
  IN       VOID                                *This,
  IN       VOID                                *EfiGuid,
  IN       UINT64                              *OemTableId,
  IN       AMD_CPM_ADDSSDTCALLBACK_FN          Function,
  IN       VOID                                *Context
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Init DXE driver
 *
 * This function installs AmdCpmTableProtocol, AmdCpmNvDataProtocol and CPM Common SSDT Table
 * and registers CPM common functions at AmdCpmInitDxe.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitDxeEntryPoint (
  IN       EFI_HANDLE              ImageHandle,
  IN       EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  VOID                            *HobList;
  VOID                            *Buffer;
  UINTN                           BufferSize;
  UINT8                           *HobBuffer;
  EFI_HANDLE                      Handle;
  AMD_CPM_NV_DATA_STRUCT          *NvDataPtr;
  AMD_CPM_TABLE_LIST              *TableListPtr;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  RECORD_TIME_DXE (BeginAmdCpmInitDxeDriver);

  Status = CpmGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Buffer = NULL;
  BufferSize = 0;
  If_CpmGetHobBuffer (HobList, gAmdCpmTableHobGuid, Buffer, BufferSize, GuidHob) {
    CpmRelocateTableList (Buffer);
    TableListPtr = CpmAdjustTableList (Buffer);

    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    TableListPtr->Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE,
                    &HobBuffer
                    );

    CpmCopyTableListToMemory (TableListPtr, HobBuffer);
    ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision = ((AMD_CPM_HOB_HEADER*)Buffer)->Revision;

    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    sizeof (AMD_CPM_TABLE_PROTOCOL),
                    &AmdCpmTableProtocolPtr
                    );

    MainTablePtr                          = ((AMD_CPM_HOB_HEADER*)HobBuffer)->MainTablePtr.Pointer;
    AmdCpmTableProtocolPtr->Revision      = ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision;
    AmdCpmTableProtocolPtr->MainTablePtr  = MainTablePtr;

    CpmRegisterKernel (AmdCpmTableProtocolPtr);
    CpmRegisterBaseIo (AmdCpmTableProtocolPtr);
    CpmRegisterCpu (AmdCpmTableProtocolPtr);
    CpmRegisterFch (AmdCpmTableProtocolPtr);

    Handle = NULL;
    gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdCpmTableProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    AmdCpmTableProtocolPtr
                    );

    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    sizeof (AMD_CPM_NV_DATA_PROTOCOL),
                    &AmdCpmNvDataProtocolPtr
                    );
    Handle = ImageHandle;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdCpmNvDataProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    AmdCpmNvDataProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      return (Status);
    }

    BufferSize = sizeof (AMD_CPM_NV_DATA_STRUCT);

    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    BufferSize,
                    &NvDataPtr
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    CpmZeroMem (NvDataPtr, BufferSize);
    NvDataPtr->CpmVersion               = (UINT32)AmdCpmTableProtocolPtr->Revision;
    NvDataPtr->CpmPcieMmioBaseAddr      = MainTablePtr->PcieMemIoBaseAddr;
    NvDataPtr->CpmAcpiMmioBaseAddr      = MainTablePtr->AcpiMemIoBaseAddr;
    NvDataPtr->CpmSbChipId              = AmdCpmTableProtocolPtr->ChipId.Sb;
    NvDataPtr->CpmMainTable             = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_MAIN_TABLE);
    NvDataPtr->CpmDeviceDetectionTable  = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_DETECTION);
    NvDataPtr->CpmDeviceResetTable      = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_RESET);
    NvDataPtr->CpmDevicePowerTable      = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_POWER);
    NvDataPtr->CpmWirelessButtonTable   = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_WIRELESS_BUTTON);
    NvDataPtr->CpmSaveContextTable      = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
    NvDataPtr->CpmExpressCardTable      = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
    NvDataPtr->CpmDisplayFeatureTable   = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_DISPLAY_FEATURE);
    NvDataPtr->CpmZeroPowerOddTable     = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    NvDataPtr->CpmAcpiThermalFanTable   = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ACPI_THERMAL_FAN);
    NvDataPtr->CpmAdaptiveS4Table       = (UINT32)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ADAPTIVE_S4);

    AmdCpmNvDataProtocolPtr->NvDataPtr = NvDataPtr;

    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    CpmInitLate,
                    (VOID*)AmdCpmNvDataProtocolPtr,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
  }

  RECORD_TIME_DXE (EndAmdCpmInitDxeDriver);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to read the register of Smbus Device.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     Select         The Smbus number.
 * @param[in]     Address        The address of Smbus device
 * @param[in]     Offset         The Offset of Smbus register
 * @param[in]     Length         The register size in BYTE
 * @param[out]    Value          Data Pointer to save register value
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
CpmSmbusGetByte (
  IN       VOID                    *This,
  IN       UINT8                   Select,
  IN       UINT8                   Address,
  IN       UINT8                   Offset,
  IN       UINTN                   Length,
     OUT   UINT8                   *Value
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  EFI_SMBUS_HC_PROTOCOL           *SmbusController;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  Status = gBS->LocateProtocol (
                                &gCpmSmbusProtocolGuid,
                                NULL,
                                &SmbusController
                                );

  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusController->Execute (
                                SmbusController,
                                SmbusSlaveAddress,
                                SmbusOffset,
                                EfiSmbusReadByte,
                                FALSE,
                                &SmbusLength,
                                Value
                                );
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to set the register of Smbus device.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     Select         The Smbus number.
 * @param[in]     Address        The address of Smbus device
 * @param[in]     Offset         The Offset of Smbus register
 * @param[in]     Length         The register size in BYTE
 * @param[in]     Value          Data Pointer to register value
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
CpmSmbusSetByte (
  IN       VOID                    *This,
  IN       UINT8                   Select,
  IN       UINT8                   Address,
  IN       UINT8                   Offset,
  IN       UINTN                   Length,
  IN       UINT8                   *Value
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  EFI_SMBUS_HC_PROTOCOL           *SmbusController;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  Status = gBS->LocateProtocol (
                                &gCpmSmbusProtocolGuid,
                                NULL,
                                &SmbusController
                                );

  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusController->Execute (
                                SmbusController,
                                SmbusSlaveAddress,
                                SmbusOffset,
                                EfiSmbusWriteByte,
                                FALSE,
                                &SmbusLength,
                                Value
                                );
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to read the register of Smbus Device in block mode.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     Select         The Smbus number.
 * @param[in]     Address        The address of Smbus device
 * @param[in]     Offset         The Offset of Smbus register
 * @param[in]     Length         The register size in BYTE
 * @param[out]    Value          Data Pointer to save register value
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
CpmSmbusGetBlock (
  IN       VOID                    *This,
  IN       UINT8                   Select,
  IN       UINT8                   Address,
  IN       UINT8                   Offset,
  IN       UINTN                   Length,
     OUT   UINT8                   *Value
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  EFI_SMBUS_HC_PROTOCOL           *SmbusController;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  Status = gBS->LocateProtocol (
                                &gCpmSmbusProtocolGuid,
                                NULL,
                                &SmbusController
                                );

  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusController->Execute (
                                SmbusController,
                                SmbusSlaveAddress,
                                SmbusOffset,
                                EfiSmbusReadBlock,
                                FALSE,
                                &SmbusLength,
                                Value
                                );
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to set the register of Smbus device in block mode.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     Select         The Smbus number.
 * @param[in]     Address        The address of Smbus device
 * @param[in]     Offset         The Offset of Smbus register
 * @param[in]     Length         The register size in BYTE
 * @param[in]     Value          Data Pointer to register value
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
CpmSmbusSetBlock (
  IN       VOID                    *This,
  IN       UINT8                   Select,
  IN       UINT8                   Address,
  IN       UINT8                   Offset,
  IN       UINTN                   Length,
  IN       UINT8                   *Value
  )
{
  EFI_STATUS                      Status;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  EFI_SMBUS_HC_PROTOCOL           *SmbusController;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  Status = gBS->LocateProtocol (
                                &gCpmSmbusProtocolGuid,
                                NULL,
                                &SmbusController
                                );

  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusController->Execute (
                                SmbusController,
                                SmbusSlaveAddress,
                                SmbusOffset,
                                EfiSmbusWriteBlock,
                                FALSE,
                                &SmbusLength,
                                Value
                                );
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Common kernel function to check ACPI object
 *
 * This function is used to check whether it is an AML Op Region Object in the table.
 *
 * @param[in]     TablePtr       Pointer to the AML table
 *
 * @retval        TRUE           Is AML Op Region Object
 * @retval        FALSE          Is not AML Op Region Object
 */
BOOLEAN
IsAmlOpRegionObject (
  IN       UINT8                               *TablePtr
  )
{
  UINT16 *Operation;
  Operation = (UINT16*) (TablePtr - 2);
  if (*Operation == AML_OPREGION_OP ) {
    return TRUE;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Common SSDT table
 *
 * This function is used to update the base address and size of the region object: CPNV.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
CpmCommonSsdtCallBack (
  IN       VOID                                *This,
  IN       VOID                                *AmlObjPtr,
  IN       VOID                                *Context
  )
{
  UINT32                  *RegionAddress;
  UINT32                  *RegionSize;
  UINT32                  *Buffer;
  AMD_CPM_TABLE_PROTOCOL  *TableProtocolPtr;

  TableProtocolPtr = This;
  Buffer = (UINT32*) Context;
  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('C', 'P', 'N', 'V')):
    if (TableProtocolPtr->CommonFunction.IsAmlOpRegionObject (AmlObjPtr)) {
      RegionAddress = (UINT32 *) ((UINT8*)AmlObjPtr + 6);
      (* RegionAddress) = *Buffer;
      RegionSize = (UINT32 *) ((UINT8*)AmlObjPtr + 11);
      * RegionSize = *(Buffer + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', 'O', 'E', 'M')):
    if (*(Buffer + 2)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 2);
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load Common SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
CpmInitLate (
  IN       EFI_EVENT               Event,
  IN       VOID                    *Context
  )
{
  AMD_CPM_NV_DATA_PROTOCOL        *NvDataProtocolPtr;
  EFI_STATUS                      Status;
  UINT32                          Buffer[3];
  STATIC BOOLEAN                  InitlateInvoked = FALSE;

  RECORD_TIME_DXE (BeginAmdCpmLoadCommonSsdt);

  if (!InitlateInvoked) {

    NvDataProtocolPtr = (AMD_CPM_NV_DATA_PROTOCOL *)Context;

    Buffer[0] = (UINT32) (NvDataProtocolPtr->NvDataPtr);
    Buffer[1] = (UINT32) (sizeof (AMD_CPM_NV_DATA_STRUCT));
    Buffer[2] = CFG_CPM_ASL_OEM_CALLBACK;

    Status = CpmAddSsdtTable (  AmdCpmTableProtocolPtr,
                                &COMMON_ACPI_DEVICE_GUID,
                                NULL,
                                CpmCommonSsdtCallBack,
                                &Buffer[0]
                                );
  }

  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmLoadCommonSsdt);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to load SSDT table.
 *
 * This function searchs ACPI storage file and finds the matched SSDT table. The table will
 * be updated and registered in ACPI area.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     EfiGuid        The GUID of ACPI storage file of SSDT table
 * @param[in]     OemTableId     OEM Table Id of SSDT table
 * @param[in]     Function       Callback Function
 * @param[in]     Context        The Parameters for callback function
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
EFIAPI
CpmAddSsdtTable (
  IN       VOID                                *This,
  IN       VOID                                *EfiGuid,
  IN       UINT64                              *OemTableId,
  IN       AMD_CPM_ADDSSDTCALLBACK_FN          Function,
  IN       VOID                                *Context
  )
{
  CPM_ACPI_SUPPORT_PROTOCOL           *AcpiSupport;
  UINT8                               *AmlObjPtr;
  UINTN                               TableList;
  EFI_STATUS                          Status;
  EFI_GUID                            *EfiGuidPtr;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               NumberOfHandles;
  UINTN                               Index;
  CPM_FIRMWARE_VOLUME_PROTOCOL        *FwVol;
  UINTN                               Size;
  UINT32                              FvStatus;
  EFI_FV_FILETYPE                     FileType;
  EFI_FV_FILE_ATTRIBUTES              Attributes;
  INTN                                Instance;
  EFI_ACPI_COMMON_HEADER              *CurrentTable;
  UINT64                              TableId;

  CurrentTable  = NULL;
  TableList     = 0;
  EfiGuidPtr    = (EFI_GUID*) EfiGuid;
  FwVol         = NULL;
  Size          = 0;

  //
  // Locate protocol.
  // There is little chance we can't find an FV protocol
  //
  Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gCpmFirmwareVolumeProtocolGuid,
                    NULL,
                    &NumberOfHandles,
                    &HandleBuffer
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gCpmFirmwareVolumeProtocolGuid,
                    &FwVol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // See if it has the ACPI storage file
    //
    Size     = 0;
    FvStatus = 0;
    Status = FwVol->ReadFile (
                    FwVol,
                    EfiGuidPtr,
                    NULL,
                    &Size,
                    &FileType,
                    &Attributes,
                    &FvStatus
                    );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  //
  // Sanity check that we found our data file
  //
  ASSERT (FwVol);

  //
  // Read tables from the storage file.
  //

  Instance     = 0;
  CurrentTable = NULL;

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                    FwVol,
                    EfiGuidPtr,
                    EFI_SECTION_RAW,
                    Instance,
                    &CurrentTable,
                    &Size,
                    &FvStatus
                    );

    if (!EFI_ERROR (Status)) {
      TableId = ((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId;
      if (*((UINT32 *) CurrentTable) == 'TDSS' && (OemTableId ? TableId == *OemTableId: TRUE)) {
        if (Function) {
          for (AmlObjPtr = (UINT8*) CurrentTable + sizeof (EFI_ACPI_COMMON_HEADER); AmlObjPtr < ((UINT8*) CurrentTable) + Size; AmlObjPtr ++) {
            if ((*Function) (This, AmlObjPtr, Context)) {
              break;
            }
          }
        }
        break;
      }

      //
      // Increment the instance
      //
      Instance++;
      CurrentTable = NULL;
    }
  }

  Status = gBS->LocateProtocol (
                    &gCpmAcpiSupportGuid,
                    NULL,
                    &AcpiSupport
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TableList = 0;

  CpmSetAcpiTable (AcpiSupport, CurrentTable, TRUE, AMD_CPM_ACPI_VERSION_SUPPORT, Size, TableList, Status);

  if (CurrentTable) {
    Status = gBS->FreePool (CurrentTable);
  }

  return  Status;
}

AGESA_STATUS
CpmGetPostedVbiosImage (
  IN      GFX_VBIOS_IMAGE_INFO         *VbiosImageInfo
  )
{
  return AGESA_UNSUPPORTED;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Register Common Kenerl functions at the AmdCpmInitDxe entry point.
 *
 * This function registers CPM common kernel functions in AmdCpmTableProtocol
 * at AmdCpmInitDxe.
 *
 *  @param[in]   This                Pointer to Protocol.
 *
 */
VOID
EFIAPI
CpmRegisterKernel (
  IN       VOID                        *This
  )
{
  AMD_CPM_TABLE_PROTOCOL      *TableProtocolPtr = This;

  TableProtocolPtr->CommonFunction.GetTablePtr            = CpmGetTablePtr;
  TableProtocolPtr->CommonFunction.GetTablePtr2           = CpmGetTablePtr2;
  TableProtocolPtr->CommonFunction.AddTable               = CpmAddTable;
  TableProtocolPtr->CommonFunction.RemoveTable            = CpmRemoveTable;
  TableProtocolPtr->CommonFunction.AddSsdtTable           = CpmAddSsdtTable;
  TableProtocolPtr->CommonFunction.IsAmlOpRegionObject    = IsAmlOpRegionObject;
  TableProtocolPtr->CommonFunction.RelocateTable          = CpmRelocateTableList;
  TableProtocolPtr->CommonFunction.CopyMem                = CpmCopyMem;
  TableProtocolPtr->CommonFunction.ReadSmbus              = CpmSmbusGetByte;
  TableProtocolPtr->CommonFunction.WriteSmbus             = CpmSmbusSetByte;
  TableProtocolPtr->CommonFunction.ReadSmbusBlock         = CpmSmbusGetBlock;
  TableProtocolPtr->CommonFunction.WriteSmbusBlock        = CpmSmbusSetBlock;
  TableProtocolPtr->DxePublicFunction.GetPostedVbiosImage = CpmGetPostedVbiosImage;

  return;
}
