/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM Function to Get Setup Options.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 284517 $   @e \$Date: 2014-02-11 15:57:17 -0600 (Tue, 11 Feb 2014) $
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
#include <AmdCpmOem.h>
#include "AmdCpmOemInitPeim.h"
#include EFI_PPI_DEFINITION (CpmVariable)

/*----------------------------------------------------------------------------------------*/
/**
 * Update Setup Options
 *
 * This function reads setup options from ReadOnlyVariable and fills in the data
 * structure of CPM OEM Setup Option.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 * @param[in]     SetupOption    Pointer to CPM Setup Option Data
 *
 * @retval        EFI_SUCCESS    Function initialized successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
CpmOemSetupOption (
  IN       EFI_PEI_SERVICES       **PeiServices,
  IN       CPM_OEM_SETUP_OPTION   *SetupOption
  )
{
#ifdef AMD_CPM_SETUP_GUID
  EFI_STATUS                          Status;
  CPM_PEI_READ_ONLY_VARIABLE_PPI      *ReadOnlyVariable;
  UINTN                               VariableSize;
  UINT32                              Attributes;
  AMD_CPM_SETUP_OPTION                CpmSetupOption;
  EFI_GUID                            CpmSetupOptionGuid = AMD_CPM_SETUP_GUID;
#endif

  SetupOption->SpecialVgaFeature        = 0;
  SetupOption->PowerExpressDynamicMode  = 0;
  SetupOption->PrimaryVideoAdaptor      = 1;
  SetupOption->DisplayOutput            = 0;
  SetupOption->BrightnessControlMethod  = 0;
  SetupOption->BlueToothEn              = 0;
  SetupOption->ZeroPowerOddEn           = 0;
  SetupOption->UnusedGppClkOff          = 1;
  SetupOption->ClockRequest             = 0;
  SetupOption->SystemBootWithPS0        = 0;
  SetupOption->AcpiThermalFanEn         = 0;
  SetupOption->AdaptiveS4En             = 0;
  SetupOption->SpreadSpectrumSwitch     = 0;
  SetupOption->WirelessSwitch           = 0;
  SetupOption->PX56Support              = 0;
#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
  SetupOption->SkipSsidOverride         = 0;
#endif

#ifdef AMD_CPM_SETUP_GUID
  Attributes    = CpmVariableAttribute;
  VariableSize  = sizeof (AMD_CPM_SETUP_OPTION);

  Status = (*PeiServices)->LocatePpi (
                              PeiServices,
                              &gCpmPeiReadOnlyVariablePpiGuid,
                              0,
                              NULL,
                              &ReadOnlyVariable
                              );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = CpmGetVariable (
                              ReadOnlyVariable,
                              AMD_CPM_SETUP_VARIABLE_NAME,
                              CpmSetupOptionGuid,
                              Attributes,
                              VariableSize,
                              CpmSetupOption
                              );
  if (!EFI_ERROR (Status)) {
    SetupOption->SpecialVgaFeature        = CpmSetupOption.AMD_CPM_SETUP_OPTION_SPECIAL_VGA_FEATURE;
    SetupOption->PowerExpressDynamicMode  = CpmSetupOption.AMD_CPM_SETUP_OPTION_POWER_XPRESS_DYNAMIC_MODE;
    SetupOption->PrimaryVideoAdaptor      = CpmSetupOption.AMD_CPM_SETUP_OPTION_PRIMARY_VIDEO_ADAPTOR;
    SetupOption->DisplayOutput            = CpmSetupOption.AMD_CPM_SETUP_OPTION_DISPLAY_OUTPUT;
    SetupOption->BrightnessControlMethod  = CpmSetupOption.AMD_CPM_SETUP_OPTION_BRIGHTNESS_CONTROL_METHOD;
    SetupOption->BlueToothEn              = CpmSetupOption.AMD_CPM_SETUP_OPTION_BLUE_TOOTH_EN;
    SetupOption->ZeroPowerOddEn           = CpmSetupOption.AMD_CPM_SETUP_OPTION_ZERO_POWER_ODD_EN;
    SetupOption->UnusedGppClkOff          = CpmSetupOption.AMD_CPM_SETUP_OPTION_UNUSED_GPP_CLOCK_OFF;
    SetupOption->ClockRequest             = CpmSetupOption.AMD_CPM_SETUP_OPTION_CLKREQ;
    SetupOption->SystemBootWithPS0        = CpmSetupOption.AMD_CPM_SETUP_OPTION_SYSTEM_BOOT_WITH_PS0;
    SetupOption->AcpiThermalFanEn         = CpmSetupOption.AMD_CPM_SETUP_OPTION_ACPI_THERMAL_FAN_EN;
    SetupOption->AdaptiveS4En             = CpmSetupOption.AMD_CPM_SETUP_OPTION_ADAPTIVE_S4_EN;
    SetupOption->SpreadSpectrumSwitch     = CpmSetupOption.AMD_CPM_SETUP_OPTION_SPREAD_SPECTRUM_SWITCH;
    SetupOption->WirelessSwitch           = CpmSetupOption.AMD_CPM_SETUP_OPTION_WIRELESS_SWITCH;
    SetupOption->PX56Support              = CpmSetupOption.AMD_CPM_SETUP_OPTION_POWERXPRESS_5_6;
#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
    SetupOption->SkipSsidOverride         = CpmSetupOption.AMD_CPM_SETUP_OPTION_SKIP_SSID_OVERRIDE;
#endif
  }
#endif
  return EFI_SUCCESS;
}
