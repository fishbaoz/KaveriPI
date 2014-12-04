/**
 * @file
 *
 * GNB init tables
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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
* ***************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbRegistersKV.h"
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

GNB_TABLE ROMDATA GnbEarlierInitTableBeforeSmuKV [] = {
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_SMUCsrIsocEn_MASK,
    (1 << D0F0x98_x07_SMUCsrIsocEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x1E_TYPE,
    D0F0x98_x1E_ADDRESS,
    D0F0x98_x1E_HiPriEn_MASK,
    (1 << D0F0x98_x1E_HiPriEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0xBC_xC0000000_TYPE,
    D0F0xBC_xC0000000_ADDRESS,
    D0F0xBC_xC0000000_IOMMU_hard_resetb_MASK,
    1 << D0F0xBC_xC0000000_IOMMU_hard_resetb_OFFSET
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEarlyInitTableKV [] = {
  GNB_ENTRY_WR (
    D0F0x04_TYPE,
    D0F0x04_ADDRESS,
    (0x1 << D0F0x04_MemAccessEn_WIDTH)
    ),
  GNB_ENTRY_RMW (
    D0F0x64_x16_TYPE,
    D0F0x64_x16_ADDRESS,
    D0F0x64_x16_AerUrMsgEn_MASK,
    0x0 << D0F0x64_x16_AerUrMsgEn_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IocBwOptEn_MASK | D0F0x98_x07_DropZeroMaskWrEn_MASK | D0F0x98_x07_UnadjustThrottlingStpclk_MASK,
    (1 << D0F0x98_x07_IocBwOptEn_OFFSET) | (1 << D0F0x98_x07_DropZeroMaskWrEn_OFFSET) | (1 << D0F0x98_x07_UnadjustThrottlingStpclk_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x0C_TYPE,
    D0F0x98_x0C_ADDRESS,
    D0F0x98_x0C_StrictSelWinnerEn_MASK,
    1 << D0F0x98_x0C_StrictSelWinnerEn_OFFSET
    ),

  // Enable voltage controller
  GNB_ENTRY_WR (
    SMU_MSG_TYPE,
    SMC_MSG_ENABLE_ALL_SMU_FEATURES,
    VOLTAGE_CONTROLLER_MASK
    ),

//---------------------------------------------------------------------------
// Set SB location for IOMMU
  GNB_ENTRY_COPY (
    D0F2xFC_x09_L1_TYPE,
    D0F2xFC_x09_L1_ADDRESS (L1_SEL_GPPSB),
    0,
    32,
    D0F0x64_x1F_TYPE,
    D0F0x64_x1F_ADDRESS,
    0,
    32
    ),
  GNB_ENTRY_COPY (
    D0F2xF4_x49_TYPE,
    D0F2xF4_x49_ADDRESS,
    0,
    32,
    D0F0x64_x1F_TYPE,
    D0F0x64_x1F_ADDRESS,
    0,
    32
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbEnvInitTableKV [] = {
//---------------------------------------------------------------------------
// ORB Init
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IocBwOptEn_MASK | D0F0x98_x07_DropZeroMaskWrEn_MASK,
    (0x1 << D0F0x98_x07_IocBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_DropZeroMaskWrEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x07_TYPE,
    D0F0x98_x07_ADDRESS,
    D0F0x98_x07_IommuBwOptEn_MASK | D0F0x98_x07_IommuIsocPassPWMode_MASK,
    (0x1 << D0F0x98_x07_IommuBwOptEn_OFFSET) | (0x1 << D0F0x98_x07_IommuIsocPassPWMode_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x08_TYPE,
    D0F0x98_x08_ADDRESS,
    D0F0x98_x08_NpWrrLenC_MASK,
    0x1 << D0F0x98_x08_NpWrrLenC_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x28_TYPE,
    D0F0x98_x28_ADDRESS,
    D0F0x98_x28_ForceCoherentIntr_MASK,
    0x1 << D0F0x98_x28_ForceCoherentIntr_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F0x98_x2C_TYPE,
    D0F0x98_x2C_ADDRESS,
    D0F0x98_x2C_NBOutbWakeMask_MASK | D0F0x98_x2C_OrbRxIdlesMask_MASK,
    (0x1 << D0F0x98_x2C_NBOutbWakeMask_OFFSET) | (0x1 << D0F0x98_x2C_OrbRxIdlesMask_OFFSET)
    ),
//---------------------------------------------------------------------------
// Clump enable Onion+
  GNB_ENTRY_WR (
    D18F0x114_TYPE,
    D18F0x114_ADDRESS,
    0xFFFFFFF2
    ),
  // Programming a conservative watermark for NBP states
  // [INST0]
  GNB_ENTRY_RMW (
    GMMx6CD8_TYPE,
    GMMx6CD8_ADDRESS,
    GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx6CC8_TYPE,
    GMMx6CC8_ADDRESS,
    GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx6CD4_TYPE,
    GMMx6CD4_ADDRESS,
    GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx6CD4_TYPE,
    GMMx6CD4_ADDRESS,
    GMMx6CD4_STUTTER_ENABLE_MASK,
    (1 << GMMx6CD4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx6CD8_TYPE,
    GMMx6CD8_ADDRESS,
    GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),
  // [INST1]
  GNB_ENTRY_RMW (
    GMMx78D8_TYPE,
    GMMx78D8_ADDRESS,
    GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx78C8_TYPE,
    GMMx78C8_ADDRESS,
    GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx78D4_TYPE,
    GMMx78D4_ADDRESS,
    GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx78D4_TYPE,
    GMMx78D4_ADDRESS,
    GMMx78D4_STUTTER_ENABLE_MASK,
    (1 << GMMx78D4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx78D8_TYPE,
    GMMx78D8_ADDRESS,
    GMMx78D8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx78D8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),
  // [INST2]
  GNB_ENTRY_RMW (
    GMMx104D8_TYPE,
    GMMx104D8_ADDRESS,
    GMMx104D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx104D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx104D8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx104D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx104D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx104D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx104C8_TYPE,
    GMMx104C8_ADDRESS,
    GMMx104C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx104C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx104C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx104C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx104D4_TYPE,
    GMMx104D4_ADDRESS,
    GMMx104D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx104D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx104D4_TYPE,
    GMMx104D4_ADDRESS,
    GMMx104D4_STUTTER_ENABLE_MASK,
    (1 << GMMx104D4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx104D8_TYPE,
    GMMx104D8_ADDRESS,
    GMMx104D8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx104D8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),
  // [INST3]
  GNB_ENTRY_RMW (
    GMMx110D8_TYPE,
    GMMx110D8_ADDRESS,
    GMMx110D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK |
    GMMx110D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK |
    GMMx110D8_NB_PSTATE_CHANGE_WATERMARK_MASK,
    (1 << GMMx110D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET) |
    (1 << GMMx110D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET) |
    (0x7FFF << GMMx110D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx110C8_TYPE,
    GMMx110C8_ADDRESS,
    GMMx110C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK |
    GMMx110C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK,
    (0x3 << GMMx110C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET) |
    (0x3 << GMMx110C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx110D4_TYPE,
    GMMx110D4_ADDRESS,
    GMMx110D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK,
    (0x7FFF << GMMx110D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx110D4_TYPE,
    GMMx110D4_ADDRESS,
    GMMx110D4_STUTTER_ENABLE_MASK,
    (1 << GMMx110D4_STUTTER_ENABLE_OFFSET)
    ),
  GNB_ENTRY_RMW (
    GMMx110D8_TYPE,
    GMMx110D8_ADDRESS,
    GMMx110D8_NB_PSTATE_CHANGE_ENABLE_MASK,
    (1 << GMMx110D8_NB_PSTATE_CHANGE_ENABLE_OFFSET)
    ),
  GNB_ENTRY_TERMINATE
};

GNB_TABLE ROMDATA GnbMidInitTableKV [] = {
//---------------------------------------------------------------------------
// ORB clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x49_TYPE,
    D0F0x98_x49_ADDRESS,
    D0F0x98_x49_SoftOverrideClk6_MASK | D0F0x98_x49_SoftOverrideClk5_MASK |
    D0F0x98_x49_SoftOverrideClk4_MASK | D0F0x98_x49_SoftOverrideClk3_MASK |
    D0F0x98_x49_SoftOverrideClk2_MASK | D0F0x98_x49_SoftOverrideClk1_MASK |
    D0F0x98_x49_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_ORB_CLK_GATING,
    D0F0x98_x4A_TYPE,
    D0F0x98_x4A_ADDRESS,
    D0F0x98_x4A_SoftOverrideClk6_MASK | D0F0x98_x4A_SoftOverrideClk5_MASK |
    D0F0x98_x4A_SoftOverrideClk4_MASK | D0F0x98_x4A_SoftOverrideClk3_MASK |
    D0F0x98_x4A_SoftOverrideClk2_MASK | D0F0x98_x4A_SoftOverrideClk1_MASK |
    D0F0x98_x4A_SoftOverrideClk0_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
// IOC clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x22_TYPE,
    D0F0x64_x22_ADDRESS,
    D0F0x64_x22_SoftOverrideClk4_MASK | D0F0x64_x22_SoftOverrideClk3_MASK |
    D0F0x64_x22_SoftOverrideClk2_MASK | D0F0x64_x22_SoftOverrideClk1_MASK |
    D0F0x64_x22_SoftOverrideClk0_MASK,
    0x0
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING,
    D0F0x64_x23_TYPE,
    D0F0x64_x23_ADDRESS,
    D0F0x64_x23_SoftOverrideClk4_MASK | D0F0x64_x23_SoftOverrideClk3_MASK |
    D0F0x64_x23_SoftOverrideClk2_MASK | D0F0x64_x23_SoftOverrideClk1_MASK |
    D0F0x64_x23_SoftOverrideClk0_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IGFX_DISABLED,
    D0F0xBC_xC0200310_TYPE,
    D0F0xBC_xC0200310_ADDRESS,
    D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_MASK,
    0x0
    ),
//---------------------------------------------------------------------------
  GNB_ENTRY_TERMINATE
};
