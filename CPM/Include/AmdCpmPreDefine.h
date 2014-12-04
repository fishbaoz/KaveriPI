/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM structures and definitions
 *
 * Contains AMD CPM Platform Interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Include
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef _AMD_CPM_PRE_DEFINE_H_
#define _AMD_CPM_PRE_DEFINE_H_

#include <AmdCpmRevision.h>
#include <AmdCpmPlatform.h>
#include <AmdCpmDefine.h>

#if (PI_SPECIFICATION_VERSION < 0x00010000)
  #ifndef AMD_CPM_EDKII
    #define gCpmFirmwareVolumeProtocolGuid  gEfiFirmwareVolumeProtocolGuid
    #define CpmFirmwareVolume               FirmwareVolume
    #define CPM_FIRMWARE_VOLUME_PROTOCOL    EFI_FIRMWARE_VOLUME_PROTOCOL
  #else
    #define gCpmFirmwareVolumeProtocolGuid  gEfiFirmwareVolume2ProtocolGuid
    #define CpmFirmwareVolume               FirmwareVolume2
    #define CPM_FIRMWARE_VOLUME_PROTOCOL    EFI_FIRMWARE_VOLUME2_PROTOCOL
  #endif
#else
  #define gCpmFirmwareVolumeProtocolGuid    gEfiFirmwareVolume2ProtocolGuid
  #define CpmFirmwareVolume                 FirmwareVolume2
  #define CPM_FIRMWARE_VOLUME_PROTOCOL      EFI_FIRMWARE_VOLUME2_PROTOCOL
#endif

#define CPM_STRINGIZE(a)            #a
#define CPM_PROTOCOL_DEFINITION(a)  CPM_STRINGIZE (Protocol/a/a.h)
#define CPM_GUID_DEFINITION(a)      CPM_STRINGIZE (Guid/a/a.h)
#define CPM_PPI_DEFINITION(a)       CPM_STRINGIZE (Ppi/a/a.h)
#define CPM_LIBRARY_DEFINITION(a)   CPM_STRINGIZE (Library/a.h)
#define CPM_INDUSTRYSTANDARD_DEFINITION(a)  CPM_STRINGIZE (IndustryStandard/a.h)
#define CPM_CALLBACK_NAME
#define CPM_CALLBACK_VAR_TYPE
#define CPM_CALLBACK_VAR_NAME


#ifndef AMD_CPM_EDKII

  #define CpmInitializeDriverLib(x, y)    EfiInitializeDriverLib (x, y)
  #define CPM_PEI_FILE_HANDLE             EFI_FFS_FILE_HEADER*
  #define CPM_PEI_SERVICES                EFI_PEI_SERVICES
  #define gCpmPeiMemoryDiscoveredPpiGuid  gPeiMemoryDiscoveredPpiGuid
  #define CPM_SMBUS_PPI                   PEI_SMBUS_PPI
  #define gCpmPeiSmbusPpiGuid             gPeiSmbusPpiGuid
  #define CpmSmbusPpi                     PeiServices, SmbusPpi
  #define CPM_DEADLOOP                    EFI_DEADLOOP
  #define CPM_EVENT_NOTIFY_SIGNAL         EFI_EVENT_NOTIFY_SIGNAL
  #define CPM_EVENT_SIGNAL_EXIT_BOOT_SERVICES EFI_EVENT_SIGNAL_EXIT_BOOT_SERVICES
  #define CPM_TPL_NOTIFY                  EFI_TPL_NOTIFY
  #define CPM_TPL_CALLBACK                EFI_TPL_CALLBACK
  #define CpmZeroMem                      EfiZeroMem
  #define CPM_SIGNATURE_32                EFI_SIGNATURE_32
  #define gCpmSmbusProtocolGuid           gEfiSmbusProtocolGuid
  #define CPM_ACPI_SUPPORT_PROTOCOL       EFI_ACPI_SUPPORT_PROTOCOL
  #define gCpmAcpiSupportGuid             gEfiAcpiSupportGuid
  #define CpmGetSystemConfigurationTable  EfiLibGetSystemConfigurationTable
  #define CPM_SMM_SX_DISPATCH_PROTOCOL    EFI_SMM_SX_DISPATCH_PROTOCOL
  #define CPM_SMM_SX_DISPATCH_CONTEXT     EFI_SMM_SX_DISPATCH_CONTEXT
  #define CPM_SMM_SW_DISPATCH_PROTOCOL    EFI_SMM_SW_DISPATCH_PROTOCOL
  #define CPM_SMM_SW_DISPATCH_CONTEXT     EFI_SMM_SW_DISPATCH_CONTEXT
  #define CpmSmmLocateProtocol            gBS->LocateProtocol
  #define gCpmSmmSxDispatchProtocolGuid   gEfiSmmSxDispatchProtocolGuid
  #define gCpmSmmSwDispatchProtocolGuid   gEfiSmmSwDispatchProtocolGuid
  #define CpmCreateEventReadyToBoot       EfiCreateEventReadyToBoot
  #define CPM_PEI_READ_ONLY_VARIABLE_PPI  PEI_READ_ONLY_VARIABLE_PPI
  #define gCpmPeiReadOnlyVariablePpiGuid  gPeiReadOnlyVariablePpiGuid
  #define CpmGetVariable(Ppi, Name, Guid, Attr, Size, Setup) Ppi->PeiGetVariable (PeiServices, Name, &Guid, NULL, &Size, &Setup)
  #define CpmVariableAttribute            NULL
  #define CpmVariable                     Variable

  #define CPM_DEFINE_gBS                  EFI_BOOT_SERVICES *gBS
  #define CPM_DEFINE_gSmst                EFI_SMM_SYSTEM_TABLE *gSmst
  #define CPM_EXTERN_gBS                  extern EFI_BOOT_SERVICES *gBS

  #define CFG_CPM_ASL_OEM_CALLBACK        AMD_CPM_ASL_OEM_CALLBACK
  #define CFG_AMD_CPM_ASL_CPU0            AMD_CPM_ASL_CPU0
  #define CFG_AMD_CPM_ASL_EC_NAME         AMD_CPM_ASL_EC_NAME
  #define CFG_AMD_CPM_ASL_EC_MUTEX        AMD_CPM_ASL_EC_MUTEX
  #define CFG_CPM_ACPI_RESTORE_SW_SMI     CPM_ACPI_RESTORE_SW_SMI
  #define CFG_CPM_ACPI_ENABLE_SW_SMI      CPM_ACPI_ENABLE_SW_SMI

  #define If_CpmBuildGuidHob(Guid, Hob, Data, Size, Status) Status = PeiBuildHobGuid (PeiServices, &Guid, Size, &Hob); \
    if (!EFI_ERROR (Status)) { \
      ((EFI_HOB_GUID_TYPE *) Hob) ++; \
      if (Data) { \
        CpmCopyMem (Hob, Data, Size); \
      } \
    }  \
    if (!EFI_ERROR (Status)) \

  #define CpmSetAcpiTable(AcpiSupport, CurrentTable, Flag, Mask, Size, TableList, Status) \
    Status = AcpiSupport->SetAcpiTable ( \
            AcpiSupport, \
            CurrentTable, \
            Flag, \
            Mask, \
            &TableList \
            ); \

  #define If_CpmGetHobBuffer(HobList, Guid, Buffer, BufferSize, GuidHob) \
    if (GetNextGuidHob (&HobList, &Guid, &Buffer, &BufferSize) == EFI_SUCCESS ) \


  #define If_CpmInSmm(ImageHandle, SystemTable, gBS, gSmst, Status) \
    BOOLEAN                         InSmm; \
    EFI_HANDLE                      SmmImageHandle; \
    EFI_SMM_BASE_PROTOCOL           *SmmBase; \
    EFI_LOADED_IMAGE_PROTOCOL       *LoadedImage; \
    EFI_DEVICE_PATH_PROTOCOL        *ImageDevicePath; \
    EFI_DEVICE_PATH_PROTOCOL        *CompleteFilePath; \
    InSmm   = FALSE; \
    gBS     = SystemTable->BootServices; \
    RECORD_TIME_DXE (BeginAmdCpmInitSmmDriver); \
    Status = gBS->LocateProtocol (&gEfiSmmBaseProtocolGuid, NULL, &SmmBase); \
    if (EFI_ERROR (Status)) { \
      return Status; \
    } \
    SmmBase->GetSmstLocation (SmmBase, &gSmst); \
    SmmBase->InSmm (SmmBase, &InSmm); \
    if (!InSmm) { \
      if (ImageHandle != NULL) { \
        Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, &LoadedImage); \
        if (EFI_ERROR (Status)) { \
          return Status; \
        } \
        Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID*) &ImageDevicePath); \
        if (EFI_ERROR (Status)) { \
          return Status; \
        } \
        CompleteFilePath = AppendDevicePath (ImageDevicePath, LoadedImage->FilePath); \
        SmmBase->Register (SmmBase, CompleteFilePath, NULL, 0, &SmmImageHandle, FALSE); \
      } \
    } else \

  #define CPM_DEFINE_CALLBACK(function, type1, var1, type2, var2) function (type1 var1, type2 var2)

#else

  #define CpmInitializeDriverLib(x, y)
  #define CPM_PEI_FILE_HANDLE             EFI_PEI_FILE_HANDLE
  #define CPM_PEI_SERVICES                CONST EFI_PEI_SERVICES
  #define gCpmPeiMemoryDiscoveredPpiGuid  gEfiPeiMemoryDiscoveredPpiGuid
  #define CPM_SMBUS_PPI                   EFI_PEI_SMBUS2_PPI
  #define gCpmPeiSmbusPpiGuid             gEfiPeiSmbus2PpiGuid
  #define CpmSmbusPpi                     SmbusPpi
  #define CPM_DEADLOOP                    CpuDeadLoop
  #define CPM_EVENT_NOTIFY_SIGNAL         EVT_NOTIFY_SIGNAL
  #define CPM_EVENT_SIGNAL_EXIT_BOOT_SERVICES EVT_SIGNAL_EXIT_BOOT_SERVICES
  #define CPM_TPL_NOTIFY                  TPL_NOTIFY
  #define CPM_TPL_CALLBACK                TPL_CALLBACK
  #define CpmZeroMem                      ZeroMem
  #define CPM_SIGNATURE_32                SIGNATURE_32
  #define gCpmSmbusProtocolGuid           gEfiSmbusHcProtocolGuid
  #define CPM_ACPI_SUPPORT_PROTOCOL       EFI_ACPI_TABLE_PROTOCOL
  #define gCpmAcpiSupportGuid             gEfiAcpiTableProtocolGuid
  #define CpmGetSystemConfigurationTable  EfiGetSystemConfigurationTable
  #define CPM_PEI_READ_ONLY_VARIABLE_PPI  EFI_PEI_READ_ONLY_VARIABLE2_PPI
  #define gCpmPeiReadOnlyVariablePpiGuid  gEfiPeiReadOnlyVariable2PpiGuid
  #define CpmGetVariable(Ppi, Name, Guid, Attr, Size, Setup) Ppi->GetVariable (Ppi, Name, &Guid, &Attr, &Size, &Setup)
  #define CpmVariable                     ReadOnlyVariable2
  #define CpmVariableAttribute EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS

  #define CPM_SMM_SX_DISPATCH_PROTOCOL    EFI_SMM_SX_DISPATCH2_PROTOCOL
  #define CPM_SMM_SX_DISPATCH_CONTEXT     EFI_SMM_SX_REGISTER_CONTEXT
  #define CPM_SMM_SW_DISPATCH_PROTOCOL    EFI_SMM_SW_DISPATCH2_PROTOCOL
  #define CPM_SMM_SW_DISPATCH_CONTEXT     EFI_SMM_SW_REGISTER_CONTEXT
  #define CpmSmmLocateProtocol            gSmst->SmmLocateProtocol
  #define gCpmSmmSxDispatchProtocolGuid   gEfiSmmSxDispatch2ProtocolGuid
  #define gCpmSmmSwDispatchProtocolGuid   gEfiSmmSwDispatch2ProtocolGuid
  #define CpmCreateEventReadyToBoot       EfiCreateEventReadyToBootEx

  #define CPM_DEFINE_gBS
  #define CPM_DEFINE_gSmst
  #define CPM_EXTERN_gBS

  #define CFG_CPM_ASL_OEM_CALLBACK        PcdGet32 (OemCallBackAslName)
  #define CFG_AMD_CPM_ASL_CPU0            PcdGet32 (Cpu0AslName)
  #define CFG_AMD_CPM_ASL_EC_NAME         PcdGet32 (EcAslName)
  #define CFG_AMD_CPM_ASL_EC_MUTEX        PcdGet32 (EcMutexAslName)
  #define CFG_CPM_ACPI_RESTORE_SW_SMI     PcdGet8 (AcpiRestoreSwSmi)
  #define CFG_CPM_ACPI_ENABLE_SW_SMI      PcdGet8 (AcpiEnableSwSmi)

  #define If_CpmBuildGuidHob(Guid, Hob, Data, Size, Status) Hob = BuildGuidHob (&Guid, Size); \
    if (Hob != NULL) { \
      if (Data) { \
        CpmCopyMem (Hob, Data, Size); \
      } \
    }  \
    if (Hob != NULL) \

  #define CpmSetAcpiTable(AcpiSupport, CurrentTable, Flag, Mask, Size, TableList, Status) \
    Status = AcpiSupport->InstallAcpiTable ( \
            AcpiSupport, \
            CurrentTable, \
            Size, \
            &TableList \
            ); \

  #define If_CpmGetHobBuffer(HobList, Guid, Buffer, BufferSize, GuidHob) \
    { \
      EFI_HOB_GUID_TYPE *GuidHob; \
      GuidHob = GetNextGuidHob (&Guid, HobList); \
      Buffer = GET_GUID_HOB_DATA (GuidHob); \
      BufferSize = GET_GUID_HOB_DATA_SIZE (GuidHob); \
    } \
    if (Buffer != NULL ) \

  #define If_CpmInSmm(ImageHandle, SystemTable, gBS, gSmst, Status) \
    BOOLEAN                         InSmm; \
    EFI_SMM_BASE2_PROTOCOL          *SmmBase; \
    RECORD_TIME_DXE (BeginAmdCpmInitSmmDriver); \
    Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &SmmBase); \
    ASSERT_EFI_ERROR (Status); \
    if (EFI_ERROR (Status)) { \
      return Status; \
    } \
    SmmBase->InSmm (SmmBase, &InSmm); \
    if (InSmm) \

  #define CPM_DEFINE_CALLBACK(function, type1, var1, type2, var2) function (type1 var1, CONST VOID var2, VOID *CommBuffer, UINTN *CommBufferSize)

  #define AMD_CPM_CPU_FAMILY_0x14_ON_SUPPORT
  #define AMD_CPM_CPU_FAMILY_0x15_TN_SUPPORT
  #define AMD_CPM_CPU_FAMILY_0x15_KV_SUPPORT
  #define AMD_CPM_CPU_FAMILY_0x15_CZ_SUPPORT
  #define AMD_CPM_CPU_FAMILY_0x16_KB_SUPPORT
  #define AMD_CPM_CPU_FAMILY_0x16_ML_SUPPORT

  #ifndef EFI_PPI_DEFINITION
    #define EFI_PPI_DEFINITION(a)       CPM_STRINGIZE (Ppi/a.h)
  #endif
  #ifndef EFI_PROTOCOL_DEFINITION
    #define EFI_PROTOCOL_DEFINITION(a)  CPM_STRINGIZE (Protocol/a.h)
  #endif
  #ifndef EFI_GUID_DEFINITION
    #define EFI_GUID_DEFINITION(a)      CPM_STRINGIZE (Guid/a.h)
  #endif

#endif

#endif
