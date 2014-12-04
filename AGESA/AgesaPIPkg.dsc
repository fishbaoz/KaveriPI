#;*****************************************************************************
#;
#; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
#;
#; AMD is granting you permission to use this software (the Materials)
#; pursuant to the terms and conditions of your Software License Agreement
#; with AMD.  This header does *NOT* give you permission to use the Materials
#; or any rights under AMD's intellectual property.  Your use of any portion
#; of these Materials shall constitute your acceptance of those terms and
#; conditions.  If you do not agree to the terms and conditions of the Software
#; License Agreement, please do not use any portion of these Materials.
#;
#; CONFIDENTIALITY:  The Materials and all other information, identified as
#; confidential and provided to you by AMD shall be kept confidential in
#; accordance with the terms and conditions of the Software License Agreement.
#;
#; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
#; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
#; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
#; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
#; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
#; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
#; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
#; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
#; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
#; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
#; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
#; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
#; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
#;
#; AMD does not assume any responsibility for any errors which may appear in
#; the Materials or any other related information provided to you by AMD, or
#; result from use of the Materials or any related information.
#;
#; You agree that you will not reverse engineer or decompile the Materials.
#;
#; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
#; further information, software, technical information, know-how, or show-how
#; available to you.  Additionally, AMD retains the right to modify the
#; Materials at any time, without notice, and is not obligated to provide such
#; modified Materials to you.
#;
#; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
#; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
#; subject to the restrictions as set forth in FAR 52.227-14 and
#; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
#; Government constitutes acknowledgement of AMD's proprietary rights in them.
#;
#; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
#; direct product thereof will be exported directly or indirectly, into any
#; country prohibited by the United States Export Administration Act and the
#; regulations thereunder, without the required authorization from the U.S.
#; government nor will be used for any purpose prohibited by the same.
#;*****************************************************************************


[Defines]
!ifndef PLATFORM_NAME
  PLATFORM_NAME                         = Agesa
  PLATFORM_GUID                         = 02485717-94DD-4a96-BF39-3C90453C1213
  PLATFORM_VERSION                      = 0.2
  DSC_SPECIFICATION                     = 0x00010006
  OUTPUT_DIRECTORY                      = Build/$(SOLUTION)AmdAgesaPkg
  SUPPORTED_ARCHITECTURES               = IA32|X64|EBC
  BUILD_TARGETS                         = DEBUG|RELEASE
  SKUID_IDENTIFIER                      = DEFAULT

  ### Copy proper DEC file to AgesaPkg.dec to match SOLUTION. ###
  ### Platform build does this during the dbuild.cmd          ###
  DEFINE SOLUTION                       = RichlandFm2
  DEFINE INTERNAL_IDS                   = 0
  DEFINE FCH_FAMILY                     = Hudson
!endif #!ifndef PLATFORM_NAME

  # SOLUTION list supported at this time
  # RichlandFm2 | RichlandFs1r2
  # KaveriFp3
  # KabiniFt3
  #

  DEFINE AGESA_PKG                      = AmdAgesaPkg
  DEFINE AMD_LIB_PATH                   = $(AGESA_PKG)/UefiUdk/Library
  DEFINE AMD_DRIVER_PATH                = $(AGESA_PKG)/UefiUdk

  #Enable INTERNAL_IDS to 1 to enable Internal IDS stuff. Also make sure
  #to copy <PI>IntPkg.dec (instead of <PI>Pkg.dsc) to AgesaPkg.dec
!if $(INTERNAL_IDS)== 1
  DEFINE INTERNAL_IDS_PREFIX            = IntnlLib
!else
  DEFINE INTERNAL_IDS_PREFIX            = Lib
!endif

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################

[BuildOptions]
#  *_*_IA32_CC_FLAGS   = /FI$(WORKSPACE)\$(PLATFORM_NAME)Pkg\Include\UdkBuildDef.h
#  *_*_X64_CC_FLAGS    = /FI$(WORKSPACE)\$(PLATFORM_NAME)Pkg\Include\UdkBuildDef.h

#########
[LibraryClasses]
  BaseLib                               |MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib                         |MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  CpuLib                                |MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  DebugLib                              |MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DevicePathLib                         |MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  DxeServicesLib                        |MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  IoLib                                 |MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PcdLib                                |MdePkg/Library/DxePcdLib/DxePcdLib.inf
  PrintLib                              |MdePkg/Library/BasePrintLib/BasePrintLib.inf
  UefiRuntimeServicesTableLib           |MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiDriverEntryPoint                  |MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiRuntimeLib                        |MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiBootServicesTableLib              |MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiLib                               |MdePkg/Library/UefiLib/UefiLib.inf
  PeiServicesLib                        |MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesTableLib                   |MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  PciLib                                |MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib                             |MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  HobLib                                |MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib                   |MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DebugPrintErrorLevelLib               |MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  MtrrLib                               |UefiCpuPkg/Library/MtrrLib/MtrrLib.inf

  PeiServicesTablePointerLib            |AmdPlatformCommonPkg/Library/PeiServicesTablePointerLibPciScratch/PeiServicesTablePointerLibPciScratch.inf
  SerialPortLib                         |AmdPlatformCommonPkg/Library/SerialIoLib/SerialIoLib.inf
  #SerialPortLib for HDT out
# SerialPortLib                         |AmdPlatformCommonPkg/Library/SerialIoHdtoutLib/SerialIoHdtoutLib.inf

  ### Agesa specific common libraries
  AgesaDxeLib                           |$(AGESA_PKG)/$(SOLUTION)AgesaDxe$(INTERNAL_IDS_PREFIX).inf
  AgesaGetHeapBase                      |$(AMD_LIB_PATH)/AgesaGetHeapBaseDxe/AgesaGetHeapBaseDxe.inf
  AgesaBufferLib                        |$(AMD_LIB_PATH)/DxeAgesaBufferLib/DxeAgesaBufferLib.inf
  AgesaUefiLib                          |$(AMD_LIB_PATH)/AgesaUefiLib/AgesaUefiLib.inf
!if $(INTERNAL_IDS)== 1
  IdsIntLib                             |$(AMD_LIB_PATH)/IdsIntLib/IdsIntLib.inf
!endif #
  AgesaPlatformLib                      |$(AMD_LIB_PATH)/PlatformLibNull/AgesaPlatformLibDxe.inf

[LibraryClasses.common.PEIM]
  PeimEntryPoint                        |MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  MemoryAllocationLib                   |MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  HobLib                                |MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PcdLib                                |MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  S3BootScriptLib                       |MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf

  ### Agesa specific common.PEIM libraries
  AgesaPeiLib                           |$(AGESA_PKG)/$(SOLUTION)AgesaPei$(INTERNAL_IDS_PREFIX).inf
  AgesaGetHeapBase                      |$(AMD_LIB_PATH)/AgesaGetHeapBasePei/AgesaGetHeapBasePei.inf
  AgesaPlatformLib                      |$(AMD_LIB_PATH)/PlatformLibNull/AgesaPlatformLibPei.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  SmmServicesTableLib                   |MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  MemoryAllocationLib                   |MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf

  ### Agesa specific common.DXE_SMM_DRIVER libraries

[Components.IA32]
  $(AMD_DRIVER_PATH)/Universal/AmdInitPostPeim/AmdInitPostPeim.inf
  $(AMD_DRIVER_PATH)/Universal/AmdResetManagerPei/AmdResetManager.inf
  $(AMD_DRIVER_PATH)/Universal/AmdProcessorInitPeim/AmdProcessorInitPeim.inf

[Components.X64]
  $(AMD_DRIVER_PATH)/Universal/AmdAgesaDxeDriver/AmdAgesaDxeDriver.inf

