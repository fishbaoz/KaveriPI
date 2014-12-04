/* $NoKeywords:$ */
/**
 * @file
 *
 * Family 15h 'early sample' support
 *
 * This file defines the required structures for family 15h pre-production processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
/*****************************************************************************
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
 *
 ***************************************************************************/

#ifndef _OPTION_FAMILY_15H_EARLY_SAMPLE_H_
#define _OPTION_FAMILY_15H_EARLY_SAMPLE_H_


/*---------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *---------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */

/*
 * Install family 15h model 0x10 - 0x1F Early Sample support
 */

/**
 *  Update the CPU microcode.
 *
 *  @param[in] StdHeader   - Config handle for library and services.
 *
 *  @retval    TRUE   - Patch Loaded Successfully.
 *  @retval    FALSE  - Patch Did Not Get Loaded.
 *
 */

typedef BOOLEAN F_F15_TN_ES_LOAD_MCU_PATCH (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/// Reference to a Method.
typedef F_F15_TN_ES_LOAD_MCU_PATCH *PF_F15_TN_ES_LOAD_MCU_PATCH;

/// Hook points in the Microcode Update feature necessary for
/// providing support for pre-production CPUs.
typedef struct {
  PF_F15_TN_ES_LOAD_MCU_PATCH    F15TnUpdateMcuPatchHook;   ///< Processor MCU Update override
} F15_TN_ES_MCU_PATCH;


/**
 *  P-state Power workaround
 *
 *  @param[in] StdHeader   - Config handle for library and services.
 *
 *  @retval    TRUE   - P-state power has been workarounded.
 *  @retval    FALSE  - P-state remain untouched.
 *
 */

typedef BOOLEAN F_F15_TN_ES_PSTATE_POWER_WORKAROUND (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/// Reference to a Method.
typedef F_F15_TN_ES_PSTATE_POWER_WORKAROUND *PF_F15_TN_ES_PSTATE_POWER_WORKAROUND;

/// Hook points in the core functionality necessary for
/// providing support for pre-production CPUs.
typedef struct {
  PF_F15_TN_ES_PSTATE_POWER_WORKAROUND    F15TnEsPstatePowerWorkaround;   ///< P-state power workaround
} F15_TN_ES_CORE_SUPPORT;


/**
 *  Is NbPstate enabled workaround
 *
 *  @param[in] StdHeader   - Config handle for library and services.
 *
 *  @retval    TRUE   - continue to check is NbPstate enabled
 *  @retval    FALSE  - return FALSE, no need to check other condition.
 *
 */

typedef BOOLEAN F_F15_TN_ES_IS_NB_PSTATE_EN (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/// Reference to a Method.
typedef F_F15_TN_ES_IS_NB_PSTATE_EN *PF_F15_TN_ES_IS_NB_PSTATE_EN;

/**
 *  SkipNbPsLoPart workaround
 *
 *  @param[in] NbPstateStatus  - NbPstate status register value.
 *  @param[in] NbPstateControl - NbPstate control register value.
 *  @param[in] StdHeader       - Config handle for library and services.
 *
 *  @retval    TRUE   - Skip NbPstateLo program
 *  @retval    FALSE  - Do NbPstateLo program
 *
 */

typedef BOOLEAN F_F15_TN_ES_SKIP_NB_PS_LO (
  IN       UINT32            *NbPstateStatus,
  IN       UINT32            *NbPstateControl,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/// Reference to a Method.
typedef F_F15_TN_ES_SKIP_NB_PS_LO *PF_F15_TN_ES_SKIP_NB_PS_LO;


/// Hook points in the NbPstate necessary for
/// providing support for pre-production CPUs.
typedef struct {
  PF_F15_TN_ES_IS_NB_PSTATE_EN    F15TnEsIsNbPsateEn;   ///< Is NbPstate enabled workaround
  PF_F15_TN_ES_SKIP_NB_PS_LO      F15TnEsSkipNbPsLo;    ///< Skip NbPstate Lo program workaround
} F15_TN_ES_NB_PSTATE;

/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */


#endif  // _OPTION_FAMILY_15H_EARLY_SAMPLE_H_