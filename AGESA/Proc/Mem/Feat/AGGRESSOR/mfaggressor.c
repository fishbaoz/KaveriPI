/* $NoKeywords:$ */
/**
 * @file
 *
 * mfaggressor.c
 *
 * Aggressor pattern generation code.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/Aggressor)
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
#include "mm.h"
#include "mn.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_AGGRESSOR_MFAGGRESSOR_FILECODE

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

BOOLEAN
STATIC
MemFTurnOnAggressorChannels (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *TurnOn
  );

BOOLEAN
STATIC
MemFStartAggressor (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  );

BOOLEAN
STATIC
MemFStopAggressor (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*-----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes the Aggressor feature hooks.
 *
 *     @param[in,out] *NBPtr - Pointer to the MEM_NB_BLOCK
 *
 *     @return       BOOLEAN
 *                             TRUE - Function was implemented
 */

BOOLEAN
MemFAggressorInit (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  ASSERT (NBPtr != NULL);
  NBPtr->FamilySpecificHook[TurnOnAggressorChannels] = MemFTurnOnAggressorChannels;
  NBPtr->FamilySpecificHook[StartAggressor] = MemFStartAggressor;
  NBPtr->FamilySpecificHook[StopAggressor] = MemFStopAggressor;

  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *       This function enables/disables continuous writes on unused agressor channels
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *TurnOn - Pointer to BOOLEAN
 *                       TRUE - Turn On, False = Turn Off.

 *     @return           BOOLEAN
 *                       TRUE - Function was implemented
 *
 */
BOOLEAN
STATIC
MemFTurnOnAggressorChannels (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *TurnOn
  )
{
  UINT8  CurrChipSel;
  UINT8  CurrDct;
  UINT8  Dct;
  UINT32 Address;
  UINT8  Die;
  BOOLEAN InfiniteModeFlag;
  MEM_NB_BLOCK *TargetNBPtr;
  BOOLEAN SkipContinuous;
  CurrDct = NBPtr->Dct;
  CurrChipSel = NBPtr->TechPtr->ChipSel;
  SkipContinuous = FALSE;
  InfiniteModeFlag = *(BOOLEAN *) TurnOn;

  for (Die = 0; Die < NBPtr->NodeCount; Die++) {
    IDS_HDT_CONSOLE ( MEM_FLOW, "\n\tDie %d : ", Die);
    if (NBPtr->DieEnabled[Die]) {
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        IDS_HDT_CONSOLE ( MEM_FLOW, "\n\t\tDct %d : ", Dct);
        // Make sure that we are not targeting current DCT, current Node
        if (Die == NBPtr->Node) {
          if (CurrDct == Dct) {
            SkipContinuous = TRUE;
          } else {
            SkipContinuous = FALSE;
          }
        } else {
          SkipContinuous = FALSE;
        }
        // Enable/Disable continuous on aggressors
        if (SkipContinuous == FALSE) {
          if (Die == NBPtr->Node) {
            TargetNBPtr = NBPtr;
          } else {
            ASSERT (NBPtr->AdjacentDieNBPtr != NULL);
            TargetNBPtr = NBPtr->AdjacentDieNBPtr;
          }
          // All context switched at this point
          TargetNBPtr->SwitchDCT (TargetNBPtr, Dct);
          if (TargetNBPtr->DCTPtr->Timings.DctMemSize != 0) {
            // Set Targets for agressors
            TargetNBPtr->TechPtr->ChipSel = TargetNBPtr->CurrentAggressorCSTarget[Dct];
            IDS_HDT_CONSOLE ( MEM_FLOW, "\n\t\t\tChipSel %d : ", TargetNBPtr->TechPtr->ChipSel);
            TargetNBPtr->GetSysAddr (TargetNBPtr, TargetNBPtr->TechPtr->ChipSel, &Address);
            TargetNBPtr->RrwSettings.DataPrbsSeed = TargetNBPtr->GetPrbs2dRdDqsSeed (TargetNBPtr, 0);
            if (InfiniteModeFlag) {
              // Enable continuous writes on aggressor channels
              //TargetNBPtr->EnableInfiniteWritePattern (TargetNBPtr);
              TargetNBPtr->FamilySpecificHook[StartAggressor] (NBPtr, NULL);
            } else {
              // Disable continous writes on aggressor channels
              //TargetNBPtr->DisableInfiniteWritePattern (TargetNBPtr);
              TargetNBPtr->FamilySpecificHook[StopAggressor] (NBPtr, NULL);
              // Set the next target CS for aggressor channel
              if (TargetNBPtr->CurrentAggressorCSTarget[Dct] == TargetNBPtr->MaxAggressorCSEnabled[Dct]) {
                TargetNBPtr->CurrentAggressorCSTarget[Dct] = TargetNBPtr->InitialAggressorCSTarget[Dct];
              } else {
                TargetNBPtr->CurrentAggressorCSTarget[Dct] = TargetNBPtr->CurrentAggressorCSTarget[Dct] + TargetNBPtr->CsPerDelay;
              }
            }
          }
        }
      }
    }
  }
  // Restore Node, DCT and ChipSel
  NBPtr->TechPtr->ChipSel = CurrChipSel;
  NBPtr->SwitchDCT (NBPtr, CurrDct);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function generates a continuous stream of writes infinite writes to DRAM using the
 *       Unified Northbridge Reliable Read/Write Engine.
 *
 *     @param[in,out] NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 */

BOOLEAN
STATIC
MemFStartAggressor (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function disables the infinite stream of writes to DRAM using the
 *       Unified Northbridge Reliable Read/Write Engine.
 *
 *     @param[in,out] NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 */

BOOLEAN
STATIC
MemFStopAggressor (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  return TRUE;
}
