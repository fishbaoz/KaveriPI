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
#ifndef _GNBURATOKEN_H_
#define _GNBURATOKEN_H_

/// Template structure for register/field table
typedef struct {
  URA_REGISTER_64B_ENTRY                                        RxSmuIntReq;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuIntToggle;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuServiceIndex;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuIntSts;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuIntAck;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuIntDone;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuAuthSts;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuAuthDone;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuAuthPass;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuFwAuth;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuProtectedMode;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuRcuUcEvents;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuBootSeqDone;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuFwFlags;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuInterruptsEnabled;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuResetCntl;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuRstReg;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuClkCntl;///<
  URA_FIELD_16B_ENTRY                                           BfxSmuCkDisable;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuAuthVector;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuRamStartAddr;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuRomStartAddr;///<
  URA_REGISTER_64B_ENTRY                                        RxSmuIntArgument;///<

} REG_FIELD_TABLE_STRUCT;

#define  IDX_CALC(VarName) (offsetof (REG_FIELD_TABLE_STRUCT, VarName) / 2)  // TODO: Why the ## Microsoft-ism before VarName
#define  SEL_CALC(VarName) VarName
#define  FIELD_OFFSET(RegName, FieldName)  ((((IDX_CALC(FieldName) - IDX_CALC(RegName) - 1) / 2) <= 15) ? ((IDX_CALC(FieldName) - IDX_CALC(RegName) - 1) / 2) : 0)

#define  TRxSmuIntReq                                           TOKEN_DEF (IDX_CALC (RxSmuIntReq), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuIntToggle                                       TOKEN_DEF (IDX_CALC (BfxSmuIntToggle), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuServiceIndex                                    TOKEN_DEF (IDX_CALC (BfxSmuServiceIndex), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuIntSts                                           TOKEN_DEF (IDX_CALC (RxSmuIntSts), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuIntAck                                          TOKEN_DEF (IDX_CALC (BfxSmuIntAck), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuIntDone                                         TOKEN_DEF (IDX_CALC (BfxSmuIntDone), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuAuthSts                                          TOKEN_DEF (IDX_CALC (RxSmuAuthSts), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuAuthDone                                        TOKEN_DEF (IDX_CALC (BfxSmuAuthDone), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuAuthPass                                        TOKEN_DEF (IDX_CALC (BfxSmuAuthPass), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuFwAuth                                           TOKEN_DEF (IDX_CALC (RxSmuFwAuth), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuProtectedMode                                   TOKEN_DEF (IDX_CALC (BfxSmuProtectedMode), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuRcuUcEvents                                      TOKEN_DEF (IDX_CALC (RxSmuRcuUcEvents), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuBootSeqDone                                     TOKEN_DEF (IDX_CALC (BfxSmuBootSeqDone), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuFwFlags                                          TOKEN_DEF (IDX_CALC (RxSmuFwFlags), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_PROTOCOL_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuInterruptsEnabled                               TOKEN_DEF (IDX_CALC (BfxSmuInterruptsEnabled), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_PROTOCOL_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuResetCntl                                        TOKEN_DEF (IDX_CALC (RxSmuResetCntl), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuRstReg                                          TOKEN_DEF (IDX_CALC (BfxSmuRstReg), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuClkCntl                                          TOKEN_DEF (IDX_CALC (RxSmuClkCntl), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TBfxSmuCkDisable                                       TOKEN_DEF (IDX_CALC (BfxSmuCkDisable), URA_TYPE_FIELD_16, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuAuthVector                                       TOKEN_DEF (IDX_CALC (RxSmuAuthVector), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuRamStartAddr                                     TOKEN_DEF (IDX_CALC (RxSmuRamStartAddr), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuRomStartAddr                                     TOKEN_DEF (IDX_CALC (RxSmuRomStartAddr), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)
#define  TRxSmuIntArgument                                      TOKEN_DEF (IDX_CALC (RxSmuIntArgument), URA_TYPE_REGISTER_64, SEL_CALC (TYPE_GNB_INDIRECT_ACCESS), URA_TOKEN_PARENT_TYPE_64)


#endif

