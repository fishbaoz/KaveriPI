/* $NoKeywords:$ */
/**
 * @file
 *
 * mrntrain3.c
 *
 * Common Recovery Northbridge function for training flow for DDR3
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
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
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mrt3.h"
#include "Filecode.h"
#define FILECODE PROC_RECOVERY_MEM_NB_MRNTRAIN3_FILECODE
/* features */

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function sets the training control flow
 *      The DDR3 mode bit must be set prior to calling this function
 *
 *      @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 */
VOID
MemNRecTrainingFlowNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemRecTTrainDQSWriteHw3 (NBPtr->TechPtr);

  MemRecTTrainRcvrEnSw (NBPtr->TechPtr);

  MemRecTTrainDQSPosSw (NBPtr->TechPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function sets the client training control flow
 *
 *      @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 */
VOID
MemNRecTrainingFlowClientNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart serial training\n");
  IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", NBPtr->MCTPtr->DieId);

  MemRecTTrainDQSWriteHw3 (NBPtr->TechPtr);

  MemRecTTrainRcvrEnHw (NBPtr->TechPtr);

  // Clear DisableCal and set DisablePredriverCal
  NBPtr->FamilySpecificHook[ReEnablePhyComp] (NBPtr, NBPtr);
  NBPtr->SetBitField (NBPtr, BFRxPtrInitReq, 1);
  while (NBPtr->GetBitField (NBPtr, BFRxPtrInitReq) != 0) {}
  NBPtr->SetBitField (NBPtr, BFDisDllShutdownSR, 1);
  NBPtr->SetBitField (NBPtr, BFEnterSelfRef, 1);
  while (NBPtr->GetBitField (NBPtr, BFEnterSelfRef) != 0) {}
  IDS_HDT_CONSOLE (MEM_FLOW, "\tMemClkAlign = 2\n");
  NBPtr->SetBitField (NBPtr, BFDbeGskMemClkAlignMode, 2);
  NBPtr->SetBitField (NBPtr, BFExitSelfRef, 1);
  while (NBPtr->GetBitField (NBPtr, BFExitSelfRef) != 0) {}
  NBPtr->SetBitField (NBPtr, BFDisDllShutdownSR, 0);

  MemRecTTrainDQSPosSw (NBPtr->TechPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function sets the Unb training control flow
 *
 *      @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 */
VOID
MemNRecTrainingFlowUnb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemRecTTrainDQSWriteHw3 (NBPtr->TechPtr);

  MemRecTTrainDQSPosSw (NBPtr->TechPtr);

  MemRecTTrainRcvrEnHwSeedless (NBPtr->TechPtr);
}

