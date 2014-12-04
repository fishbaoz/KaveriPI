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



GNB_TABLE ROMDATA GnbIommuInitTableKV [] = {
//---------------------------------------------------------------------------
//IOMMU SmifSupW init
  GNB_ENTRY_RMW (
    D0F2x70_TYPE,
    D0F2x70_ADDRESS,
    D0F2x70_SmifSupW_MASK | D0F2x70_PrefSupW_MASK,
    (0x1 << D0F2x70_SmifSupW_OFFSET) | (0 << D0F2x70_PrefSupW_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2x70_TYPE,
    D0F2x70_ADDRESS,
    D0F2x70_PrefSupW_MASK,
    0
    ),
//---------------------------------------------------------------------------
//IOMMU L2 Initialization
  GNB_ENTRY_RMW (
    D0F2xF4_x10_TYPE,
    D0F2xF4_x10_ADDRESS,
    D0F2xF4_x10_DTCInvalidationSel_MASK,
    0x2 << D0F2xF4_x10_DTCInvalidationSel_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x14_TYPE,
    D0F2xF4_x14_ADDRESS,
    D0F2xF4_x14_ITCInvalidationSel_MASK,
    0x2 << D0F2xF4_x14_ITCInvalidationSel_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x18_TYPE,
    D0F2xF4_x18_ADDRESS,
    D0F2xF4_x18_PTCAInvalidationSel_MASK,
    0x2 << D0F2xF4_x18_PTCAInvalidationSel_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x30_TYPE,
    D0F2xF4_x30_ADDRESS,
    D0F2xF4_x30_ERRRuleLock1_MASK,
    0x1 << D0F2xF4_x30_ERRRuleLock1_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x34_TYPE,
    D0F2xF4_x34_ADDRESS,
    D0F2xF4_x34_L2aregHostPgsize_MASK | D0F2xF4_x34_L2aregGstPgsize_MASK,
    (0x2 << D0F2xF4_x34_L2aregHostPgsize_OFFSET) | (0x2 <<  D0F2xF4_x34_L2aregGstPgsize_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x57_TYPE,
    D0F2xF4_x57_ADDRESS,
    D0F2xF4_x47_TwAtomicFilterEn_MASK | D0F2xF4_x47_TwNwEn_MASK,
    (0x1 << D0F2xF4_x47_TwAtomicFilterEn_OFFSET) | (1 << D0F2xF4_x47_TwNwEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x47_TYPE,
    D0F2xF4_x47_ADDRESS,
    D0F2xF4_x47_TwAtomicFilterEn_MASK | D0F2xF4_x47_TwNwEn_MASK,
    (0x1 << D0F2xF4_x47_TwAtomicFilterEn_OFFSET) | (1 << D0F2xF4_x47_TwNwEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x4C_TYPE,
    D0F2xF4_x4C_ADDRESS,
    D0F2xF4_x4C_GstPartialPtcCntrl_MASK,
    0x3 << D0F2xF4_x4C_GstPartialPtcCntrl_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x50_TYPE,
    D0F2xF4_x50_ADDRESS,
    D0F2xF4_x50_PDCInvalidationSel_MASK,
    0x2 << D0F2xF4_x50_PDCInvalidationSel_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x56_TYPE,
    D0F2xF4_x56_ADDRESS,
    D0F2xF4_x56_CPFlushOnInv_MASK | D0F2xF4_x56_CPFlushOnWait_MASK,
    (0x0 << D0F2xF4_x56_CPFlushOnInv_OFFSET) | (1 << D0F2xF4_x56_CPFlushOnWait_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x53_TYPE,
    D0F2xF4_x53_ADDRESS,
    D0F2xF4_x53_L2bUpdateFilterBypass_MASK,
    (0x0 << D0F2xF4_x53_L2bUpdateFilterBypass_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x22_TYPE,
    D0F2xF4_x22_ADDRESS,
    D0F2xF4_x22_L2aUpdateFilterBypass_MASK,
    (0x0 << D0F2xF4_x22_L2aUpdateFilterBypass_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x80_TYPE,
    D0F2xF4_x80_ADDRESS,
    D0F2xF4_x80_ERRRuleLock0_MASK,
    0x1 << D0F2xF4_x80_ERRRuleLock0_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xF4_x94_TYPE,
    D0F2xF4_x94_ADDRESS,
    D0F2xF4_x94_L2bregHostPgsize_MASK | D0F2xF4_x94_L2bregGstPgsize_MASK,
    (0x2 << D0F2xF4_x94_L2bregHostPgsize_OFFSET) | (0x2ull << D0F2xF4_x94_L2bregGstPgsize_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IGFX_DISABLED,
    D0F2xF4_x57_TYPE,
    D0F2xF4_x57_ADDRESS,
    D0F2xF4_x57_L1ImuIntGfxDis_MASK,
    (0x1 << D0F2xF4_x57_L1ImuIntGfxDis_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_DGFX_DISABLED,
    D0F2xF4_x57_TYPE,
    D0F2xF4_x57_ADDRESS,
    D0F2xF4_x57_L1ImuPcieGfxDis_MASK,
    (0x1 << D0F2xF4_x57_L1ImuPcieGfxDis_OFFSET)
    ),
//IOMMU L1 Initialization
  GNB_ENTRY_RMW (
    D0F2xFC_x0C_L1_TYPE,
    D0F2xFC_x0C_L1_ADDRESS (L1_SEL_GFX),
    D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK,
    0x4 << D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x32_L1_TYPE,
    D0F2xFC_x32_L1_ADDRESS (L1_SEL_GFX),
    D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK |
    D0F2xFC_x32_L1_AtsMultipleRespEn_MASK |
    D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK |
    D0F2xFC_x32_L1_DmaBufCredits_MASK |
    D0F2xFC_x32_L1_DmaBufMaxNpCred_MASK,
    (0x1 << D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET) |
    (0x20 << D0F2xFC_x32_L1_DmaBufCredits_OFFSET) |
    (0x1F << D0F2xFC_x32_L1_DmaBufMaxNpCred_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1_SEL_GFX),
    D0F2xFC_x07_L1_L1NwEn_MASK | D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK |
    D0F2xFC_x07_L1_AtsSeqNumEn_MASK | D0F2xFC_x07_L1_SpecReqFilterEn_MASK | BIT6,
    (0x1 << D0F2xFC_x07_L1_L1NwEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET) |
    (0x1 << D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET) | BIT6
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x0C_L1_TYPE,
    D0F2xFC_x0C_L1_ADDRESS (L1_SEL_GPPSB),
    D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK,
    0x4 << D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x32_L1_TYPE,
    D0F2xFC_x32_L1_ADDRESS (L1_SEL_GPPSB),
    D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK |
    D0F2xFC_x32_L1_AtsMultipleRespEn_MASK |
    D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK,
    (0x1 << D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1_SEL_GPPSB),
    D0F2xFC_x07_L1_L1NwEn_MASK | D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK |
    D0F2xFC_x07_L1_AtsSeqNumEn_MASK | D0F2xFC_x07_L1_SpecReqFilterEn_MASK | BIT6,
    (0x1 << D0F2xFC_x07_L1_L1NwEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET) |
    (0x1 << D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET) | BIT6
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x0C_L1_TYPE,
    D0F2xFC_x0C_L1_ADDRESS (L1_SEL_GPP),
    D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK,
    0x4 << D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x32_L1_TYPE,
    D0F2xFC_x32_L1_ADDRESS (L1_SEL_GPP),
    D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK |
    D0F2xFC_x32_L1_AtsMultipleRespEn_MASK |
    D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK,
    (0x1 << D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET)
    ),
 GNB_ENTRY_RMW (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1_SEL_GPP),
    D0F2xFC_x07_L1_L1NwEn_MASK | D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK |
    D0F2xFC_x07_L1_AtsSeqNumEn_MASK | D0F2xFC_x07_L1_SpecReqFilterEn_MASK | BIT6,
    (0x1 << D0F2xFC_x07_L1_L1NwEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET) |
    (0x1 << D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET) | BIT6
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x0C_L1_TYPE,
    D0F2xFC_x0C_L1_ADDRESS (L1_SEL_GBIF),
    D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK,
    0x4 << D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x32_L1_TYPE,
    D0F2xFC_x32_L1_ADDRESS (L1_SEL_GBIF),
    D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK |
    D0F2xFC_x32_L1_AtsMultipleRespEn_MASK |
    D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK |
    D0F2xFC_x32_L1_DmaNpHaltDis_MASK |
    D0F2xFC_x32_L1_DmaBufCredits_MASK |
    D0F2xFC_x32_L1_DmaBufMaxNpCred_MASK,
    (0x1 << D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_DmaNpHaltDis_OFFSET) |
    (0x20 << D0F2xFC_x32_L1_DmaBufCredits_OFFSET) |
    (0x20 << D0F2xFC_x32_L1_DmaBufMaxNpCred_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1_SEL_GBIF),
    D0F2xFC_x07_L1_L1NwEn_MASK | D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK |
    D0F2xFC_x07_L1_AtsSeqNumEn_MASK | D0F2xFC_x07_L1_SpecReqFilterEn_MASK | BIT6,
    (0x1 << D0F2xFC_x07_L1_L1NwEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET) |
    (0x1 << D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET) | BIT6
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x0C_L1_TYPE,
    D0F2xFC_x0C_L1_ADDRESS (L1_SEL_INTGEN),
    D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK,
    0x4 << D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x32_L1_TYPE,
    D0F2xFC_x32_L1_ADDRESS (L1_SEL_INTGEN),
    D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK |
    D0F2xFC_x32_L1_AtsMultipleRespEn_MASK |
    D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK |
    D0F2xFC_x32_L1_DmaNpHaltDis_MASK |
    D0F2xFC_x32_L1_DmaBufCredits_MASK |
    D0F2xFC_x32_L1_DmaBufMaxNpCred_MASK,
    (0x1 << D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET) |
    (0x1 << D0F2xFC_x32_L1_DmaNpHaltDis_OFFSET) |
    (0x4 << D0F2xFC_x32_L1_DmaBufCredits_OFFSET) |
    (0x4 << D0F2xFC_x32_L1_DmaBufMaxNpCred_OFFSET)
    ),
  GNB_ENTRY_RMW (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1_SEL_INTGEN),
    D0F2xFC_x07_L1_L1NwEn_MASK | D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK |
    D0F2xFC_x07_L1_AtsSeqNumEn_MASK | D0F2xFC_x07_L1_SpecReqFilterEn_MASK | BIT6,
    (0x1 << D0F2xFC_x07_L1_L1NwEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET) |
    (0x1 << D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET) | (0x1 << D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET) | BIT6
    ),
//---------------------------------------------------------------------------
// IOMMU Initialization
  GNB_ENTRY_RMW (
    D0F2xF4_x90_TYPE,
    D0F2xF4_x90_ADDRESS,
    D0F2xF4_x90_CKGateL2BMiscDisable_MASK |
    D0F2xF4_x90_CKGateL2BDynamicDisable_MASK |
    D0F2xF4_x90_CKGateL2BRegsDisable_MASK |
    D0F2xF4_x90_CKGateL2BCacheDisable_MASK,
    (0x1 << D0F2xF4_x90_CKGateL2BMiscDisable_OFFSET) |
    (0x1 << D0F2xF4_x90_CKGateL2BDynamicDisable_OFFSET) |
    (0x1 << D0F2xF4_x90_CKGateL2BRegsDisable_OFFSET) |
    (0x1 << D0F2xF4_x90_CKGateL2BCacheDisable_OFFSET)
    ),
// IOMMU L2 clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING,
    D0F2xF4_x90_TYPE,
    D0F2xF4_x90_ADDRESS,
    D0F2xF4_x90_CKGateL2BDynamicDisable_MASK |
    D0F2xF4_x90_CKGateL2BRegsDisable_MASK |
    D0F2xF4_x90_CKGateL2BCacheDisable_MASK,
    (0x0 << D0F2xF4_x90_CKGateL2BDynamicDisable_OFFSET) |
    (0x0 << D0F2xF4_x90_CKGateL2BRegsDisable_OFFSET) |
    (0x0 << D0F2xF4_x90_CKGateL2BCacheDisable_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING,
    D0F2xF4_x33_TYPE,
    D0F2xF4_x33_ADDRESS,
    D0F2xF4_x33_CKGateL2ARegsDisable_MASK | D0F2xF4_x33_CKGateL2ADynamicDisable_MASK | D0F2xF4_x33_CKGateL2ACacheDisable_MASK,
    0x0
    ),
// IOMMU L1 clock gating
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING,
    D0F2xFC_x33_L1_TYPE,
    D0F2xFC_x33_L1_ADDRESS (L1_SEL_GFX),
    D0F2xFC_x33_L1_L1DmaClkgateEn_MASK | D0F2xFC_x33_L1_L1CacheClkgateEn_MASK |
    D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK | D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK |
    D0F2xFC_x33_L1_L1PerfClkgateEn_MASK | D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK |
    D0F2xFC_x33_L1_L1RegClkgateEn_MASK | D0F2xFC_x33_L1_L1L2ClkgateEn_MASK |
    D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK,
    (0x1 << D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING,
    D0F2xFC_x33_L1_TYPE,
    D0F2xFC_x33_L1_ADDRESS (L1_SEL_GPPSB),
    D0F2xFC_x33_L1_L1DmaClkgateEn_MASK | D0F2xFC_x33_L1_L1CacheClkgateEn_MASK |
    D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK | D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK |
    D0F2xFC_x33_L1_L1PerfClkgateEn_MASK | D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK |
    D0F2xFC_x33_L1_L1RegClkgateEn_MASK | D0F2xFC_x33_L1_L1L2ClkgateEn_MASK |
    D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK,
    (0x1 << D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET)
    ),
 GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING,
    D0F2xFC_x33_L1_TYPE,
    D0F2xFC_x33_L1_ADDRESS (L1_SEL_GPP),
    D0F2xFC_x33_L1_L1DmaClkgateEn_MASK | D0F2xFC_x33_L1_L1CacheClkgateEn_MASK |
    D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK | D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK |
    D0F2xFC_x33_L1_L1PerfClkgateEn_MASK | D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK |
    D0F2xFC_x33_L1_L1RegClkgateEn_MASK | D0F2xFC_x33_L1_L1L2ClkgateEn_MASK |
    D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK,
    (0x1 << D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING,
    D0F2xFC_x33_L1_TYPE,
    D0F2xFC_x33_L1_ADDRESS (L1_SEL_GBIF),
    D0F2xFC_x33_L1_L1DmaClkgateEn_MASK | D0F2xFC_x33_L1_L1CacheClkgateEn_MASK |
    D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK | D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK |
    D0F2xFC_x33_L1_L1PerfClkgateEn_MASK | D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK |
    D0F2xFC_x33_L1_L1RegClkgateEn_MASK | D0F2xFC_x33_L1_L1L2ClkgateEn_MASK |
    D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK,
    (0x1 << D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING,
    D0F2xFC_x33_L1_TYPE,
    D0F2xFC_x33_L1_ADDRESS (L1_SEL_INTGEN),
    D0F2xFC_x33_L1_L1DmaClkgateEn_MASK | D0F2xFC_x33_L1_L1CacheClkgateEn_MASK |
    D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK | D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK |
    D0F2xFC_x33_L1_L1PerfClkgateEn_MASK | D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK |
    D0F2xFC_x33_L1_L1RegClkgateEn_MASK | D0F2xFC_x33_L1_L1L2ClkgateEn_MASK |
    D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK,
    (0x1 << D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET) | (0x1 << D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET) |
    (0x1 << D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET)
    ),
//---------------------------------------------------------------------------
// Configure IOMMU Power Island
// Hide IOMMU function if disabled
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_DISABLED,
    D0F2xF4_x3B_TYPE,
    D0F2xF4_x3B_ADDRESS,
    D0F2xF4_x3B_FSM_ADDR_MASK,
    (0xFF << D0F2xF4_x3B_FSM_ADDR_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_DISABLED,
    D0F2xF4_x3B_TYPE,
    D0F2xF4_x3B_ADDRESS,
    D0F2xF4_x3B_Power_Down_MASK,
    (0x1 << D0F2xF4_x3B_Power_Down_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_DISABLED,
    D0F2xF4_x3B_TYPE,
    D0F2xF4_x3B_ADDRESS,
    D0F2xF4_x3B_Power_Down_MASK,
    (0x0 << D0F2xF4_x3B_Power_Down_OFFSET)
    ),
  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_DISABLED,
    D0F0x64_x0D_TYPE,
    D0F0x64_x0D_ADDRESS,
    D0F0x64_x0D_PciDev0Fn2RegEn_MASK,
    0x0
    ),

  GNB_ENTRY_PROPERTY_RMW (
    TABLE_PROPERTY_IOMMU_L2_DEBUG_3_31,
    D0F2xF4_x47_TYPE,
    D0F2xF4_x47_ADDRESS,
    0x80000000,
    (0x1 << 31)
    ),
  GNB_ENTRY_TERMINATE
};
