/* $NoKeywords:$ */
/**
 * @file
 *
 * mmStandardTraining.c
 *
 * Main Memory Feature implementation file for Standard Training
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main)
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


#include "Porting.h"
#include "AGESA.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuServices.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "ma.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_MAIN_MMSTANDARDTRAINING_FILECODE
/* features */
#include "mftds.h"

extern MEM_FEAT_BLOCK_MAIN MemFeatMain;
/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

BOOLEAN
MemMStandardTraining (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  );

/*-----------------------------------------------------------------------------
*                                EXPORTED FUNCTIONS
*
*-----------------------------------------------------------------------------
*/
extern BUILD_OPT_CFG UserOptions;
extern MEM_FEAT_TRAIN_SEQ memTrainSequenceDDR3[];
/* -----------------------------------------------------------------------------*/
/**
 *
 * MemMStandardTraining
 *
 * This function implements standard memory training whereby training functions
 * for all nodes are run by the BSP.
 *
 *
 *     @param[in,out]   *mmPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */
BOOLEAN
MemMStandardTraining (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  UINT8 Die;
  //
  // If training is disabled, return success.
  //
  if (!UserOptions.CfgDqsTrainingControl) {
    return TRUE;
  }
  //
  // Run Northbridge-specific Standard Training feature for each die.
  //
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart serial training\n");
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", Die);
    AGESA_TESTPOINT (TpProcMemBeforeAnyTraining, &(mmPtr->MemPtr->StdHeader));
    mmPtr->NBPtr[Die].BeforeDqsTraining (&mmPtr->NBPtr[Die]);
    mmPtr->NBPtr[Die].Execute1dMaxRdLatTraining = TRUE;
    mmPtr->NBPtr[Die].FeatPtr->Training (&mmPtr->NBPtr[Die]);
    mmPtr->NBPtr[Die].TechPtr->TechnologySpecificHook[LrdimmSyncTrainedDlys] (mmPtr->NBPtr[Die].TechPtr, NULL);
    mmPtr->NBPtr[Die].AfterDqsTraining (&mmPtr->NBPtr[Die]);
    if (mmPtr->NBPtr[Die].MCTPtr->ErrCode == AGESA_FATAL) {
      break;
    }
  }
  return (BOOLEAN) (Die == mmPtr->DieCount);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 * MemMStandardTrainingUsingAdjacentDies
 *
 * This function implements standard memory training whereby training functions
 * for all nodes are run by the BSP while enabling other dies to eable argressor channel
 *
 *
 *     @param[in,out]   *mmPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */
BOOLEAN
MemMStandardTrainingUsingAdjacentDies (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  UINT8 Die;
  UINT8 AdjacentDie;
  UINT32 AdjacentSocketNum;
  UINT32 TargetSocketNum;
  UINT32 ModuleNum;
  UINT8 i;
  UINT8 Dct;
  UINT8 ChipSel;
  BOOLEAN FirstCsFound;
  //
  // If training is disabled, return success.
  //
  if (!UserOptions.CfgDqsTrainingControl) {
    return TRUE;
  }
  mmPtr->mmSharedPtr->CommonSmallestMaxNegVref = 0x7F;
  mmPtr->mmSharedPtr->CommonSmallestMaxPosVref = 0x7F;
  //
  // Run Northbridge-specific Standard Training feature for each die.
  //
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart standard serial training\n");
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", Die);
    AGESA_TESTPOINT (TpProcMemBeforeAnyTraining, &(mmPtr->MemPtr->StdHeader));
    mmPtr->NBPtr[Die].BeforeDqsTraining (&mmPtr->NBPtr[Die]);
    mmPtr->NBPtr[Die].Execute1dMaxRdLatTraining = FALSE;
    mmPtr->NBPtr[Die].FeatPtr->Training (&mmPtr->NBPtr[Die]);
    if (mmPtr->NBPtr[Die].MCTPtr->ErrCode == AGESA_FATAL) {
      break;
    }
  }
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart 2D training with agressors\n");
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", Die);
    AGESA_TESTPOINT (TpProcMemBeforeAnyTraining, &(mmPtr->MemPtr->StdHeader));
    GetSocketModuleOfNode (Die, &TargetSocketNum, &ModuleNum, &(mmPtr->MemPtr->StdHeader));
    for (AdjacentDie = 0; AdjacentDie < mmPtr->DieCount; AdjacentDie++) {
      mmPtr->NBPtr[Die].DieEnabled[AdjacentDie] = FALSE;
      GetSocketModuleOfNode (AdjacentDie, &AdjacentSocketNum, &ModuleNum, &(mmPtr->MemPtr->StdHeader));
      if (TargetSocketNum == AdjacentSocketNum) {
        if (AdjacentDie != Die) {
          if (mmPtr->NBPtr[AdjacentDie].MCTPtr->NodeMemSize != 0) {
            mmPtr->NBPtr[Die].AdjacentDieNBPtr = &mmPtr->NBPtr[AdjacentDie];
            mmPtr->NBPtr[Die].DieEnabled[AdjacentDie] = TRUE;
          }
        } else {
          if (mmPtr->NBPtr[Die].MCTPtr->NodeMemSize != 0) {
            mmPtr->NBPtr[Die].DieEnabled[Die] = TRUE;
          }
        }
        // Determine the initial target CS, Max Dimms and max CS number for all DCTs (potential aggressors)
        if (mmPtr->NBPtr[AdjacentDie].MCTPtr->NodeMemSize != 0) {
          for (Dct = 0; Dct < mmPtr->NBPtr[AdjacentDie].DctCount; Dct++) {
            FirstCsFound = FALSE;
            mmPtr->NBPtr[AdjacentDie].SwitchDCT (&mmPtr->NBPtr[AdjacentDie], Dct);
            for (ChipSel = 0; ChipSel < mmPtr->NBPtr[AdjacentDie].CsPerChannel; ChipSel = ChipSel + (mmPtr->NBPtr[Die].IsSupported[PerDimmAggressors2D] ? 2 : mmPtr->NBPtr[AdjacentDie].CsPerDelay) ) {
              if ((mmPtr->NBPtr[AdjacentDie].DCTPtr->Timings.CsEnabled & ((UINT16) 1 << ChipSel)) != 0) {
                if (FirstCsFound == FALSE) {
                  // Set Initial CS value for Current Aggressor CS
                  mmPtr->NBPtr[AdjacentDie].InitialAggressorCSTarget[Dct] = ChipSel;
                  mmPtr->NBPtr[AdjacentDie].CurrentAggressorCSTarget[Dct] = mmPtr->NBPtr[AdjacentDie].InitialAggressorCSTarget[Dct];
                  FirstCsFound = TRUE;
                }
                mmPtr->NBPtr[AdjacentDie].MaxAggressorCSEnabled[Dct] = ChipSel;
                mmPtr->NBPtr[AdjacentDie].MaxAggressorDimms[Dct]++;
              }
            }
          }
        }
      }
    }
    if (mmPtr->NBPtr[Die].MCTPtr->NodeMemSize != 0) {
      //Execute Technology specific 2D training features
      i = 0;
      while (memTrainSequenceDDR3[i].TrainingSequenceEnabled != 0) {
        if (memTrainSequenceDDR3[i].TrainingSequenceEnabled (&mmPtr->NBPtr[Die])) {
          mmPtr->NBPtr[Die].TrainingSequenceIndex = i;
          // Execute 2D RdDqs Training
          memTrainSequenceDDR3[i].MemTechFeatBlock->RdDqs2DTraining (mmPtr->NBPtr[Die].TechPtr);
          // Execute MaxRdLat Training After 2D training
          do {
            if (memTrainSequenceDDR3[i].MemTechFeatBlock->MaxRdLatencyTraining (mmPtr->NBPtr[Die].TechPtr)) {
              MemFInitTableDrive (&mmPtr->NBPtr[Die], MTAfterMaxRdLatTrn);
            }
          } while (mmPtr->NBPtr->ChangeNbFrequency (&mmPtr->NBPtr[Die]));
          break;
        }
        i++;
      }
    }
    mmPtr->NBPtr[Die].TechPtr->TechnologySpecificHook[LrdimmSyncTrainedDlys] (mmPtr->NBPtr[Die].TechPtr, NULL);
    mmPtr->NBPtr[Die].AfterDqsTraining (&mmPtr->NBPtr[Die]);
    if (mmPtr->NBPtr[Die].MCTPtr->ErrCode == AGESA_FATAL) {
      break;
    }
  }
  return (BOOLEAN) (Die == mmPtr->DieCount);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 * MemM2DTrainingWithAggressor
 *
 * This function implements standard memory training whereby training functions
 * for all nodes are run by the BSP while enabling other dies to eable argressor channel
 *
 *
 *     @param[in,out]   *mmPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */
BOOLEAN
MemM2DTrainingWithAggressor (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  UINT8 Die;
  UINT8 Index;
  //
  // If training is disabled, return success.
  //
  if (!UserOptions.CfgDqsTrainingControl) {
    return TRUE;
  }
  mmPtr->mmSharedPtr->CommonSmallestMaxNegVref = 0x7F;
  mmPtr->mmSharedPtr->CommonSmallestMaxPosVref = 0x7F;
  //
  // Run Northbridge-specific Standard Training feature for each die.
  //
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart standard serial training\n");
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", Die);
    AGESA_TESTPOINT (TpProcMemBeforeAnyTraining, &(mmPtr->MemPtr->StdHeader));
    mmPtr->NBPtr[Die].BeforeDqsTraining (&mmPtr->NBPtr[Die]);
    mmPtr->NBPtr[Die].Execute1dMaxRdLatTraining = FALSE;
    mmPtr->NBPtr[Die].FeatPtr->Training (&mmPtr->NBPtr[Die]);
    if (mmPtr->NBPtr[Die].MCTPtr->ErrCode == AGESA_FATAL) {
      break;
    }
  }

  //----------------------------------------------------------------
  // Determine Aggressor Chipselects for all DCTs on all nodes.
  //----------------------------------------------------------------
  MemFeatMain.AggressorDetermination (mmPtr);

  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart 2D training with agressors run independently\n");
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    IDS_HDT_CONSOLE (MEM_STATUS, "Node %d\n", Die);
    AGESA_TESTPOINT (TpProcMemBeforeAnyTraining, &(mmPtr->MemPtr->StdHeader));

    if (mmPtr->NBPtr[Die].MCTPtr->NodeMemSize != 0) {
      //Execute Technology specific 2D training features
      Index = 0;
      while (memTrainSequenceDDR3[Index].TrainingSequenceEnabled != 0) {
        if (memTrainSequenceDDR3[Index].TrainingSequenceEnabled (&mmPtr->NBPtr[Die])) {
          mmPtr->NBPtr[Die].TrainingSequenceIndex = Index;
          // Execute 2D RdDqs Training
          memTrainSequenceDDR3[Index].MemTechFeatBlock->RdDqs2DTraining (mmPtr->NBPtr[Die].TechPtr);
          // Execute MaxRdLat Training After 2D training
          do {
            if (memTrainSequenceDDR3[Index].MemTechFeatBlock->MaxRdLatencyTraining (mmPtr->NBPtr[Die].TechPtr)) {
              MemFInitTableDrive (&mmPtr->NBPtr[Die], MTAfterMaxRdLatTrn);
            }
          } while (mmPtr->NBPtr->ChangeNbFrequency (&mmPtr->NBPtr[Die]));
          break;
        }
        Index++;
      }
    }
    mmPtr->NBPtr[Die].TechPtr->TechnologySpecificHook[LrdimmSyncTrainedDlys] (mmPtr->NBPtr[Die].TechPtr, NULL);
    mmPtr->NBPtr[Die].AfterDqsTraining (&mmPtr->NBPtr[Die]);
    if (mmPtr->NBPtr[Die].MCTPtr->ErrCode == AGESA_FATAL) {
      break;
    }
  } // End Die For Loop

  return (BOOLEAN) (Die == mmPtr->DieCount);
}

