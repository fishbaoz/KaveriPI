/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM POST API, and related functions.
 *
 * Contains code that initialized the CPM before memory init.
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

#include <AmdCpmPei.h>
#include <AmdCpmTable.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmCpu.h>
#include <AmdCpmFch.h>

VOID
EFIAPI
CpmLoadPreInitTable (
  IN       VOID                        *This,
  IN       UINT8                       Stage
  );

VOID
EFIAPI
CpmPlatformIdInit (
  IN       AMD_CPM_TABLE_PPI           *CpmTablePpiPtr
  );

VOID
EFIAPI
CpmRegisterKernel (
  IN       VOID                        *This
  );

EFI_STATUS
AmdCpmInitBeforeS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyListS3Restore =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gCpmPeiMemoryDiscoveredPpiGuid,
  AmdCpmInitBeforeS3LateRestore
};

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Init Function Before S3 Late Restore
 *
 * This function updates CPM Main Table Pointer in AMD_CPM_TABLE_PPI Data Structure
 * after PEI MEMORY DISCOVERED PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 * @param[in]     NotifyDesc     The descriptor for the notification event
 * @param[in]     InvokePpi      Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 *
 */
EFI_STATUS
AmdCpmInitBeforeS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                  Status;
  AMD_CPM_TABLE_PPI           *CpmTablePtr;
  AMD_CPM_TABLE_HOB_PPI       *CpmTableHobPtr;

  RECORD_TIME_PEI (BeginAmdCpmInitBeforeS3LateRestore);

  Status = (*PeiServices)->LocatePpi (
                              PeiServices,
                              &gAmdCpmTablePpiGuid,
                              0,
                              NULL,
                              &CpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = (*PeiServices)->LocatePpi (
                              PeiServices,
                              &gAmdCpmTableHobPpiGuid,
                              0,
                              NULL,
                              &CpmTableHobPtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmTablePtr->CommonFunction.RelocateTable (CpmTableHobPtr);

  CpmTablePtr->MainTablePtr = (AMD_CPM_MAIN_TABLE*) ((AMD_CPM_HOB_HEADER*)CpmTableHobPtr)->MainTablePtr.Pointer;

  RECORD_TIME_PEI (EndAmdCpmInitBeforeS3LateRestore);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Init PEIM driver
 *
 * This function gets CPM definition tables from AmdCpmOemTablePpi, reorgnizes the tables
 * and installs AmdCpmTablePpi. It also initializes the registers by load Pre Init Table
 * and update Current Platform Id at AmdCpmInitPeim.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitPeimEntryPoint (
  IN       CPM_PEI_FILE_HANDLE  FileHandle,
  IN       CPM_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  AMD_CPM_OEM_TABLE_PPI       *AmdCpmOemTablePpiPtr;
  AMD_CPM_TABLE_PPI           *AmdCpmTablePpiPtr;
  EFI_PEI_PPI_DESCRIPTOR      *PpiListCpmTablePtr;
  EFI_PEI_PPI_DESCRIPTOR      *PpiListCpmTableHobPtr;
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  EFI_HOB_GUID_TYPE           *Hob;
  EFI_BOOT_MODE               BootMode;
  AMD_CPM_TABLE_LIST          HobList;
  UINT8                       *HobBuffer;

  RECORD_TIME_PEI (BeginAmdCpmInitPeimDriver);

  Status = (*PeiServices)->LocatePpi (
                              PeiServices,
                              &gAmdCpmOemTablePpiGuid,
                              0,
                              NULL,
                              &AmdCpmOemTablePpiPtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmGenerateTableList (AmdCpmOemTablePpiPtr->TableList, AmdCpmOemTablePpiPtr->PlatformId, &HobList);

  Status = (*PeiServices)->GetBootMode (
                             PeiServices,
                             &BootMode
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CpmGetBootMode () != CPM_BOOT_MODE_S3) {
    If_CpmBuildGuidHob (gAmdCpmTableHobGuid, Hob, NULL, HobList.Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE, Status) {

      HobBuffer = (UINT8*) Hob;

    } else {
      return EFI_NOT_FOUND;
    }
  } else {
    Status = (*PeiServices)->AllocatePool (
                                PeiServices,
                                HobList.Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE,
                                &HobBuffer
                                );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = (*PeiServices)->AllocatePool (
                                PeiServices,
                                sizeof (EFI_PEI_PPI_DESCRIPTOR),
                                &PpiListCpmTableHobPtr
                                );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    PpiListCpmTableHobPtr->Flags  = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    PpiListCpmTableHobPtr->Guid   = &gAmdCpmTableHobPpiGuid;
    PpiListCpmTableHobPtr->Ppi    = HobBuffer;

    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PpiListCpmTableHobPtr
                                );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyListS3Restore);
  }

  MainTablePtr = CpmCopyTableListToMemory (&HobList, HobBuffer);

  Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              sizeof (AMD_CPM_TABLE_PPI),
                              &AmdCpmTablePpiPtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MainTablePtr->CurrentPlatformId = AmdCpmOemTablePpiPtr->PlatformId;
  MainTablePtr->Service.Pointer   = (VOID*)PeiServices;
  CommonFunctionPtr               = &(AmdCpmTablePpiPtr->CommonFunction);

  AmdCpmTablePpiPtr->Revision     = (AMD_CPM_KERNEL_VERSION << 16) + AmdCpmOemTablePpiPtr->Revision;
  AmdCpmTablePpiPtr->MainTablePtr = MainTablePtr;

  ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision = (UINT32)AmdCpmTablePpiPtr->Revision;

  CpmRegisterKernel (AmdCpmTablePpiPtr);
  CpmRegisterBaseIo (AmdCpmTablePpiPtr);
  CpmRegisterCpu (AmdCpmTablePpiPtr);
  CpmRegisterFch (AmdCpmTablePpiPtr);

  Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              sizeof (EFI_PEI_PPI_DESCRIPTOR),
                              &PpiListCpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PpiListCpmTablePtr->Flags   = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListCpmTablePtr->Guid    = &gAmdCpmTablePpiGuid;
  PpiListCpmTablePtr->Ppi     = AmdCpmTablePpiPtr;

  CpmLoadPreInitTable (AmdCpmTablePpiPtr, CPM_PRE_INIT_STAGE_0);
  CpmPlatformIdInit (AmdCpmTablePpiPtr);

  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (AmdCpmTablePpiPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    if (SaveContextTablePtr->BufferSize < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT)) {
      CommonFunctionPtr->RemoveTable (AmdCpmTablePpiPtr, SaveContextTablePtr);
    } else {
      CommonFunctionPtr->SetSaveContext (AmdCpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, BootMode), CpmGetBootMode ());
    }
  }

  Status = (*PeiServices)->InstallPpi (
                              PeiServices,
                              PpiListCpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  RECORD_TIME_PEI (EndAmdCpmInitPeimDriver);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load Pre Init Table.
 *
 * @param[in]     This           Pointer to AMD CPM TABLE Ppi
 * @param[in]     Stage          Stage number to initialize the registers
 *
 */
VOID
EFIAPI
CpmLoadPreInitTable (
  IN       VOID                   *This,
  IN       UINT8                  Stage
  )
{
  AMD_CPM_PRE_INIT_TABLE          *PreInitTablePtr;
  AMD_CPM_GPIO_INIT_TABLE         *GpioPreInitTablePtr;
  AMD_CPM_PRE_SETTING_ITEM        *TablePtr;
  AMD_CPM_TABLE_PPI               *CpmTablePpiPtr;
  AMD_CPM_GPIO_ITEM               *GpioTablePtr;

  CpmTablePpiPtr  = (AMD_CPM_TABLE_PPI*) This;
  PreInitTablePtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_PRE_INIT);
  GpioPreInitTablePtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GPIO_PRE_INIT);

  if (PreInitTablePtr) {
    TablePtr = &PreInitTablePtr->Item[0];
    while (TablePtr->Type != 0xFF ) {
      if (TablePtr->Stage == Stage) {
        switch (TablePtr->Type) {
        case 0:
          CpmTablePpiPtr->CommonFunction.MmioAndThenOr8 ( CpmTablePpiPtr->MainTablePtr->AcpiMemIoBaseAddr + (TablePtr->Select << 0x08) + TablePtr->Offset,
                                                          TablePtr->AndMask,
                                                          TablePtr->OrMask
                                                        );
          break;
        case 1:
        case 2:
          CpmTablePpiPtr->CommonFunction.PciAndThenOr8 ( CpmTablePpiPtr,
                                                         0x00,
                                                         (TablePtr->Select & 0xF8) >> 3,
                                                          TablePtr->Select & 7,
                                                          TablePtr->Offset + (TablePtr->Type == 1? 0 : 0x100),
                                                          TablePtr->AndMask,
                                                          TablePtr->OrMask
                                                        );
          break;
        }
      }
      TablePtr ++;
    }
  }
  if (GpioPreInitTablePtr) {
    GpioTablePtr = &GpioPreInitTablePtr->GpioList[0];
    while (GpioTablePtr->Pin != 0xFF ) {
      CpmTablePpiPtr->CommonFunction.SetGpio (CpmTablePpiPtr, GpioTablePtr->Pin, GpioTablePtr->Setting.Raw);
      GpioTablePtr++;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize Current Platform Id.
 *
 * @param[in]     CpmTablePpiPtr Point to Ppi
 *
 */
VOID
EFIAPI
CpmPlatformIdInit (
  IN       AMD_CPM_TABLE_PPI           *CpmTablePpiPtr
  )
{
  AMD_CPM_PLATFORM_ID_TABLE           *PlatformIdTablePtr;
  AMD_CPM_PLATFORM_ID_CONVERT_TABLE   *PlatformIdConvertTablePtr;
  UINT16                              PlatformId;
  UINT8                               Index;
  UINT8                               CpuRevisionId;

  PlatformId                  = CpmTablePpiPtr->MainTablePtr->CurrentPlatformId;
  PlatformIdTablePtr          = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID);
  PlatformIdConvertTablePtr   = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT);
  CpuRevisionId               = CpmTablePpiPtr->CommonFunction.GetCpuRevisionId (CpmTablePpiPtr);

  if (PlatformIdTablePtr) {
    PlatformId = 0;
    for (Index = 0; Index < AMD_PLATFORM_ID_TABLE_SIZE; Index++) {
      if (PlatformIdTablePtr->GpioPin[Index] == 0xFF) {
        break;
      }
      PlatformId = PlatformId + (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, PlatformIdTablePtr->GpioPin[Index]) << Index);
    }
  }
  if (PlatformIdConvertTablePtr) {
    for (Index = 0; Index < AMD_PLATFORM_ID_CONVERT_TABLE_SIZE; Index++) {
      if (PlatformIdConvertTablePtr->Item[Index].CpuRevisionId == 0xFF) {
        PlatformId = 0;
        break;
      }
      if ((PlatformId & PlatformIdConvertTablePtr->Item[Index].OriginalIdMask) == PlatformIdConvertTablePtr->Item[Index].OriginalId &&
          CpuRevisionId == PlatformIdConvertTablePtr->Item[Index].CpuRevisionId) {
        PlatformId = PlatformIdConvertTablePtr->Item[Index].ConvertedId;
        break;
      }
    }
  }
  if (PlatformId >= 32) {
    PlatformId = 0;
  }
  CpmTablePpiPtr->MainTablePtr->CurrentPlatformId = PlatformId;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to read the register of Smbus Device.
 *
 * @param[in]     This           Point to Ppi
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
  EFI_PEI_SERVICES                **PeiServices;
  EFI_STATUS                      Status;
  CPM_SMBUS_PPI                   *SmbusPpi;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  PeiServices = (EFI_PEI_SERVICES**) ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr->Service.Pointer;

  Status = (**PeiServices).LocatePpi (
                                      PeiServices,
                                      &gCpmPeiSmbusPpiGuid,
                                      Select,
                                      NULL,
                                      &SmbusPpi
                                      );
  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusPpi->Execute (    CpmSmbusPpi,
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
 * @param[in]     This           Point to Ppi
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
  EFI_PEI_SERVICES                **PeiServices;
  EFI_STATUS                      Status;
  CPM_SMBUS_PPI                   *SmbusPpi;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  PeiServices = (EFI_PEI_SERVICES**) ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr->Service.Pointer;

  Status = (**PeiServices).LocatePpi (
                                      PeiServices,
                                      &gCpmPeiSmbusPpiGuid,
                                      Select,
                                      NULL,
                                      &SmbusPpi
                                      );
  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusPpi->Execute (    CpmSmbusPpi,
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
 * @param[in]     This           Point to Ppi
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
  EFI_PEI_SERVICES                **PeiServices;
  EFI_STATUS                      Status;
  CPM_SMBUS_PPI                   *SmbusPpi;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  PeiServices = (EFI_PEI_SERVICES**) ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr->Service.Pointer;

  Status = (**PeiServices).LocatePpi (
                                      PeiServices,
                                      &gCpmPeiSmbusPpiGuid,
                                      Select,
                                      NULL,
                                      &SmbusPpi
                                      );
  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusPpi->Execute (    CpmSmbusPpi,
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
 * @param[in]     This           Point to Ppi
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
  EFI_PEI_SERVICES                **PeiServices;
  EFI_STATUS                      Status;
  CPM_SMBUS_PPI                   *SmbusPpi;
  EFI_SMBUS_DEVICE_ADDRESS        SmbusSlaveAddress;
  UINTN                           SmbusLength;
  UINTN                           SmbusOffset;

  PeiServices = (EFI_PEI_SERVICES**) ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr->Service.Pointer;

  Status = (**PeiServices).LocatePpi (
                                      PeiServices,
                                      &gCpmPeiSmbusPpiGuid,
                                      Select,
                                      NULL,
                                      &SmbusPpi
                                      );
  if (!EFI_ERROR (Status)) {
    SmbusLength = Length;
    SmbusOffset = Offset;
    SmbusSlaveAddress.SmbusDeviceAddress = Address;

    Status = SmbusPpi->Execute (    CpmSmbusPpi,
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

/*---------------------------------------------------------------------------------------*/
/**
 * Register Common Kenerl functions at the AmdCpmInitPeim entry point.
 *
 * This function registers CPM common kernel functions in AmdCpmTablePpi
 * at AmdCpmInitPeim.
 *
 *  @param[in]   This                Pointer to Ppi.
 *
 */
VOID
EFIAPI
CpmRegisterKernel (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION             *CommonFunctionPtr;
  CommonFunctionPtr                   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  CommonFunctionPtr->GetTablePtr      = CpmGetTablePtr;
  CommonFunctionPtr->GetTablePtr2     = CpmGetTablePtr2;
  CommonFunctionPtr->AddTable         = CpmAddTable;
  CommonFunctionPtr->RemoveTable      = CpmRemoveTable;
  CommonFunctionPtr->ReadSmbus        = CpmSmbusGetByte;
  CommonFunctionPtr->WriteSmbus       = CpmSmbusSetByte;
  CommonFunctionPtr->ReadSmbusBlock   = CpmSmbusGetBlock;
  CommonFunctionPtr->WriteSmbusBlock  = CpmSmbusSetBlock;
  CommonFunctionPtr->RelocateTable    = CpmRelocateTableList;
  CommonFunctionPtr->CopyMem          = CpmCopyMem;
  CommonFunctionPtr->LoadPreInitTable = CpmLoadPreInitTable;

  return;
}
