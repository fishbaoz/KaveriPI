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
  PLATFORM_NAME                         = FchHudsonModulePkg
  PLATFORM_GUID                         = D8232F12-4B68-32B2-0626-6D896021891A
  PLATFORM_VERSION                      = 0.1
  DSC_SPECIFICATION                     = 0x00010006
  OUTPUT_DIRECTORY                      = Build/FchHudsonPkg
  SUPPORTED_ARCHITECTURES               = IA32|X64|EBC
  BUILD_TARGETS                         = DEBUG|RELEASE
  SKUID_IDENTIFIER                      = DEFAULT
!endif #!ifndef PLATFORM_NAME

  DEFINE AGESA_PKG                      = AmdAgesaPkg
  DEFINE AMD_LIB_PATH                   = $(AGESA_PKG)/UefiUdk/Library
  DEFINE AMD_DRIVER_PATH                = $(AGESA_PKG)/UefiUdk
  DEFINE AMD_FCH_LIB_PATH               = $(AMD_DRIVER_PATH)/Fch/FchHudsonModulePkg/Library
  DEFINE AMD_FCH_DRIVER_PATH            = $(AMD_DRIVER_PATH)/Fch/FchHudsonModulePkg/

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

#########
[LibraryClasses]
  FchDxeLib                             |$(AMD_FCH_DRIVER_PATH)/Library/FchDxeLib/FchDxeLib.inf

[LibraryClasses.common.PEIM]
  FchPeiLib                             |$(AMD_FCH_DRIVER_PATH)/Library/FchPeiLib/FchPeiLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  FchSmmLib                             |$(AMD_FCH_DRIVER_PATH)/Library/FchSmmLib/FchSmmLib.inf

[Components.IA32]
  $(AMD_FCH_DRIVER_PATH)/FchPei/FchPei.inf
  $(AMD_FCH_DRIVER_PATH)/SmbusPei/Smbus.inf
  $(AMD_FCH_DRIVER_PATH)/FchPeiAux/FchPeiAux.inf

[Components.X64]
  $(AMD_FCH_DRIVER_PATH)/LegacyInterruptDxe/LegacyInterrupt.inf
  $(AMD_FCH_DRIVER_PATH)/SmmControlDxe/SmmControl.inf
  $(AMD_FCH_DRIVER_PATH)/FchDxe/FchDxe.inf
  $(AMD_FCH_DRIVER_PATH)/FchDxeMmio/FchDxeMmio.inf
  $(AMD_FCH_DRIVER_PATH)/FchDxeAux/FchDxeAux.inf
  $(AMD_FCH_DRIVER_PATH)/SmbusDxe/SmbusLight.inf
  $(AMD_FCH_DRIVER_PATH)/Cf9ResetDxe/Cf9Reset.inf

  $(AMD_FCH_DRIVER_PATH)/AmdFchHwm/Dxe/AmdFchHwmDxe.inf
  $(AMD_FCH_DRIVER_PATH)/AmdFchHwm/Smm/AmdFchHwmSmm.inf
  $(AMD_FCH_DRIVER_PATH)/AmdFchWhea/Dxe/AmdFchWheaDxe.inf
  $(AMD_FCH_DRIVER_PATH)/AmdFchWhea/Smm/AmdFchWheaSmm.inf

  $(AMD_FCH_DRIVER_PATH)/FchSmmDispatcher/FchSmmDispatcher.inf
  $(AMD_FCH_DRIVER_PATH)/FchSmm/FchSmm.inf
  $(AMD_FCH_DRIVER_PATH)/FchSmmComplement/ImcControl/ImcControl.inf
  $(AMD_FCH_DRIVER_PATH)/FchSmmComplement/Usb3PortProtectWA/Usb3PortProtectWA.inf
  $(AMD_FCH_DRIVER_PATH)/FchSmmComplement/FchPcieWakeWA/FchPcieWakeWA.inf

