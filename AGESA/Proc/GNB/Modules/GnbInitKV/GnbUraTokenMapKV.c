/* $NoKeywords:$ */
/**
 * @file
 *
 * AGESA gnb file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  GNB
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
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
#include  "AGESA.h"
#include  "amdlib.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "GnbUra.h"
#include  "GnbUraToken.h"
#include  "GnbRegistersKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GNBURATOKENMAPKV_FILECODE

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

REG_FIELD_TABLE_STRUCT  UraTableKV = {
  {D0F0xBC_xC2100000_ADDRESS, D0F0xBC_xC2100000_TYPE, 0}, ///< TRxSmuIntReq
  {D0F0xBC_xC2100000_IntToggle_OFFSET, D0F0xBC_xC2100000_IntToggle_WIDTH, FIELD_OFFSET(RxSmuIntReq ,BfxSmuIntToggle)}, ///< TBfxSmuIntToggle
  {D0F0xBC_xC2100000_ServiceIndex_OFFSET, D0F0xBC_xC2100000_ServiceIndex_WIDTH, FIELD_OFFSET(RxSmuIntReq ,BfxSmuServiceIndex)}, ///< TBfxSmuServiceIndex
  {D0F0xBC_xC2100004_ADDRESS, D0F0xBC_xC2100004_TYPE, 0}, ///< TRxSmuIntSts
  {D0F0xBC_xC2100004_IntAck_OFFSET, D0F0xBC_xC2100004_IntAck_WIDTH, FIELD_OFFSET(RxSmuIntSts ,BfxSmuIntAck)}, ///< TBfxSmuIntAck
  {D0F0xBC_xC2100004_IntDone_OFFSET, D0F0xBC_xC2100004_IntDone_WIDTH, FIELD_OFFSET(RxSmuIntSts ,BfxSmuIntDone)}, ///< TBfxSmuIntDone
  {D0F0xBC_xE0003088_ADDRESS, D0F0xBC_xE0003088_TYPE, 0}, ///< TRxSmuAuthSts
  {D0F0xBC_xE0003088_SMU_AUTH_DONE_OFFSET, D0F0xBC_xE0003088_SMU_AUTH_DONE_WIDTH, FIELD_OFFSET(RxSmuAuthSts ,BfxSmuAuthDone)}, ///< TBfxSmuAuthDone
  {D0F0xBC_xE0003088_SMU_AUTH_PASS_OFFSET, D0F0xBC_xE0003088_SMU_AUTH_PASS_WIDTH, FIELD_OFFSET(RxSmuAuthSts ,BfxSmuAuthPass)}, ///< TBfxSmuAuthPass
  {D0F0xBC_xE00030A4_ADDRESS, D0F0xBC_xE00030A4_TYPE, 0}, ///< TRxSmuFwAuth
  {D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_OFFSET, D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_WIDTH, FIELD_OFFSET(RxSmuFwAuth ,BfxSmuProtectedMode)}, ///< TBfxSmuProtectedMode
  {D0F0xBC_xC0000004_ADDRESS, D0F0xBC_xC0000004_TYPE, 0}, ///< TRxSmuRcuUcEvents
  {D0F0xBC_xC0000004_boot_seq_done_OFFSET, D0F0xBC_xC0000004_boot_seq_done_WIDTH, FIELD_OFFSET(RxSmuRcuUcEvents ,BfxSmuBootSeqDone)}, ///< TBfxSmuBootSeqDone
  {D0F0xBC_x3F800_ADDRESS, D0F0xBC_x3F800_TYPE, 0}, ///< TRxSmuFwFlags
  {D0F0xBC_x3F800_InterruptsEnabled_OFFSET, D0F0xBC_x3F800_InterruptsEnabled_WIDTH, FIELD_OFFSET(RxSmuFwFlags ,BfxSmuInterruptsEnabled)}, ///< TBfxSmuInterruptsEnabled
  {D0F0xBC_x80000000_ADDRESS, D0F0xBC_x80000000_TYPE, 0}, ///< TRxSmuResetCntl
  {D0F0xBC_x80000000_smu_rst_reg_OFFSET, D0F0xBC_x80000000_smu_rst_reg_WIDTH, FIELD_OFFSET(RxSmuResetCntl ,BfxSmuRstReg)}, ///< TBfxSmuRstReg
  {D0F0xBC_x80000004_ADDRESS, D0F0xBC_x80000004_TYPE, 0}, ///< TRxSmuClkCntl
  {D0F0xBC_x80000004_smu_ck_disable_OFFSET, D0F0xBC_x80000004_smu_ck_disable_WIDTH, FIELD_OFFSET(RxSmuClkCntl ,BfxSmuCkDisable)}, ///< TBfxSmuCkDisable
  {D0F0xBC_x80008000_ADDRESS, D0F0xBC_x80008000_TYPE, 0}, ///< TRxSmuAuthVector
  {D0F0xBC_x20000_ADDRESS, D0F0xBC_x20000_TYPE, 0}, ///< TRxSmuRamStartAddr
  {D0F0xBC_x0_ADDRESS, D0F0xBC_x0_TYPE, 0}, ///< TRxSmuRomStartAddr
  {D0F0xBC_xC210003C_ADDRESS, D0F0xBC_xC210003C_TYPE, 0}, ///< TRxSmuIntArgument

};

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method to locate register table.
 *
 *
 * @param[in]        Device             Standard configuration header
 * @param[in, out]   UraTableAddress    Ura register table address
 */
VOID
GnbUraLocateRegTblKV (
  IN       DEV_OBJECT    *Device,
  IN OUT   UINT32        *UraTableAddress
  )
{
  *UraTableAddress = (UINT32)((UINTN)(&UraTableKV));
  return;
}


