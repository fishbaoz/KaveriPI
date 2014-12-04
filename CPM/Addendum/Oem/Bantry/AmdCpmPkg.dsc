#;*****************************************************************************
#;
#; Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
  PLATFORM_NAME                         = Bantry
  PLATFORM_GUID                         = 81948467-1E5A-471e-A141-AAF78677BBC4
  PLATFORM_VERSION                      = 0.1
  DSC_SPECIFICATION                     = 0x00010006
  OUTPUT_DIRECTORY                      = Build/$(SOLUTION)CpmPkg
  SUPPORTED_ARCHITECTURES               = IA32|X64|EBC
  BUILD_TARGETS                         = DEBUG|RELEASE
  SKUID_IDENTIFIER                      = DEFAULT
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

[LibraryClasses]
  AmdCpmBaseLib | AmdCpmPkg\Library\Proc\Base\AmdCpm2BaseLib.inf
  AmdCpmCpuLib  | AmdCpmPkg\Library\Proc\Cpu\AmdCpm2Cpu.inf
  AmdCpmFchLib  | AmdCpmPkg\Library\Proc\Fch\AmdCpm2Fch.inf

[Components.IA32]
  AmdCpmPkg\Addendum\Oem\Bantry\Pei\AmdCpm2OemInitPeimBantry.inf
  AmdCpmPkg\Kernel\Pei\AmdCpm2InitPeim.inf
  AmdCpmPkg\Features\AcpiThermalFan\Pei\AmdCpm2AcpiThermalFanPeim.inf
  AmdCpmPkg\Features\AdaptiveS4\Pei\AmdCpm2AdaptiveS4Peim.inf
  AmdCpmPkg\Features\GpioInit\Pei\AmdCpm2GpioInitPeim.inf
  AmdCpmPkg\Features\DisplayFeature\Pei\AmdCpm2DisplayFeaturePeim.inf
  AmdCpmPkg\Features\PcieInit\Pei\AmdCpm2PcieInitPeim.inf
  AmdCpmPkg\Features\BootTimeRecord\Pei\AmdCpm2BootTimerRecordPeim.inf

[Components.x64]
  AmdCpmPkg\Kernel\Asl\AmdCpm2InitAsl.inf
  AmdCpmPkg\Kernel\Dxe\AmdCpm2InitDxe.inf
  AmdCpmPkg\Kernel\Smm\AmdCpm2InitSmm.inf
  AmdCpmPkg\Features\GpioInit\Dxe\AmdCpm2GpioInitDxe.inf
  AmdCpmPkg\Features\GpioInit\Smm\AmdCpm2GpioInitSmm.inf
  AmdCpmPkg\Features\DisplayFeature\Asl\DisplayFeatureI\AmdCpm2DisplayFeatureIAsl.inf
  AmdCpmPkg\Features\DisplayFeature\Asl\DisplayFeatureD\AmdCpm2DisplayFeatureDAsl.inf
  AmdCpmPkg\Features\DisplayFeature\Asl\MxmOverT\AmdCpm2DisplayFeatureMxmOverTAsl.inf
  AmdCpmPkg\Features\DisplayFeature\Asl\DisplayConnectEvent\AmdCpm2DisplayFeatureDisplayConnectEventAsl.inf
  AmdCpmPkg\Features\DisplayFeature\Dxe\AmdCpm2DisplayFeatureDxe.inf
  AmdCpmPkg\Features\DisplayFeature\Smm\AmdCpm2DisplayFeatureSmm.inf
  AmdCpmPkg\Features\PcieInit\Asl\AmdCpm2PcieInitAsl.inf
  AmdCpmPkg\Features\PcieInit\Dxe\AmdCpm2PcieInitDxe.inf
  AmdCpmPkg\Features\AdaptiveS4\Asl\AmdCpm2AdaptiveS4Asl.inf
  AmdCpmPkg\Features\AdaptiveS4\Dxe\AmdCpm2AdaptiveS4Dxe.inf
  AmdCpmPkg\Features\AdaptiveS4\Smm\AmdCpm2AdaptiveS4Smm.inf
  AmdCpmPkg\Features\AcpiThermalFan\Asl\AmdCpm2AcpiThermalFanAsl.inf
  AmdCpmPkg\Features\AcpiThermalFan\Dxe\AmdCpm2AcpiThermalFanDxe.inf
  AmdCpmPkg\Features\BootTimeRecord\Dxe\AmdCpm2BootTimerRecordDxe.inf
