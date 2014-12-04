/* $NoKeywords:$ */
/**
 * @file
 *
 * mmAggressor.c
 *
 * Main Memory Feature implementation file for aggressor initialization.
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


#include "AGESA.h"
#include "OptionMemory.h"
#include "amdlib.h"
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "Ids.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_MAIN_MMAGGRESSOR_FILECODE

extern MEM_FEAT_BLOCK_MAIN MemFeatMain;

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
VOID
STATIC
MemMInitializeAggressorData (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  );

/*-----------------------------------------------------------------------------
*                                EXPORTED FUNCTIONS
*
*-----------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *  Find Aggressor Chipselects on all DCTs/Nodes relative to the
 *   current node.
 *
 *     @param[in,out]   *mmPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */
BOOLEAN
MemMAggressor (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  MemMInitializeAggressorData (mmPtr);

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
 *  Determine Aggressor Chipselects for all DCTs on all nodes relative to the
 *   current node.
 *
 *     @param[in,out]   *mmPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 */
VOID
STATIC
MemMInitializeAggressorData (
  IN OUT   MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  UINT8 Die;
  UINT8 AdjacentDie;
  UINT32 AdjacentSocketNum;
  UINT32 TargetSocketNum;
  UINT32 ModuleNum;
  UINT8 Dct;
  UINT8 ChipSel;
  BOOLEAN FirstCsFound;
  UINT8 Max2Aggressors;
  UINT8 MaxAggressors;
  UINT8 MaxAggressorDie;
  UINT8 MaxAggressorDct;
  UINT8 AggressorCount;

  MaxAggressors = 0;
  Max2Aggressors = 0;
  MaxAggressorDie = 0;
  MaxAggressorDct = 0;
  //
  //  Scan all Dies for Aggressors
  //
  for (Die = 0 ; Die < mmPtr->DieCount ; Die ++ ) {
    //
    // For each Die
    //
    GetSocketModuleOfNode (Die, &TargetSocketNum, &ModuleNum, &(mmPtr->MemPtr->StdHeader));
    //
    //  Find the Adjacent Die and cound the aggressors each
    //
    MaxAggressors = 0;
    Max2Aggressors = 0;
    for (AdjacentDie = 0; AdjacentDie < mmPtr->DieCount; AdjacentDie++) {
      mmPtr->NBPtr[Die].DieEnabled[AdjacentDie] = FALSE;
      GetSocketModuleOfNode (AdjacentDie, &AdjacentSocketNum, &ModuleNum, &(mmPtr->MemPtr->StdHeader));
      //
      // If Same Socket, then its the adjacent Die
      //
      if (TargetSocketNum == AdjacentSocketNum) {
        if (AdjacentDie != Die) {
          //
          // Is same socket, different die
          //
          if (mmPtr->NBPtr[AdjacentDie].MCTPtr->NodeMemSize != 0) {
            mmPtr->NBPtr[Die].AdjacentDieNBPtr = &mmPtr->NBPtr[AdjacentDie];
            mmPtr->NBPtr[Die].DieEnabled[AdjacentDie] = TRUE;
          }
        } else {
          if (mmPtr->NBPtr[Die].MCTPtr->NodeMemSize != 0) {
            mmPtr->NBPtr[Die].DieEnabled[Die] = TRUE;
          }
        }
        //
        // Determine the initial target CS,
        // Max Dimms and max CS number for all DCTs (potential aggressors)
        //
        if (mmPtr->NBPtr[AdjacentDie].MCTPtr->NodeMemSize != 0) {
          for (Dct = 0; Dct < mmPtr->NBPtr[AdjacentDie].DctCount; Dct++) {
            FirstCsFound = FALSE;
            mmPtr->NBPtr[AdjacentDie].SwitchDCT (&mmPtr->NBPtr[AdjacentDie], Dct);
            AggressorCount = 0;
            for (ChipSel = 0; ChipSel < mmPtr->NBPtr[AdjacentDie].CsPerChannel; ChipSel = ChipSel + mmPtr->NBPtr[AdjacentDie].CsPerDelay) {
              if ((mmPtr->NBPtr[AdjacentDie].DCTPtr->Timings.CsEnabled & ((UINT16) 1 << ChipSel)) != 0) {
                if (FirstCsFound == FALSE) {
                  // Set Initial CS value for Current Aggressor CS
                  mmPtr->NBPtr[AdjacentDie].InitialAggressorCSTarget[Dct] = ChipSel;
                  mmPtr->NBPtr[AdjacentDie].CurrentAggressorCSTarget[Dct] = mmPtr->NBPtr[AdjacentDie].InitialAggressorCSTarget[Dct];
                  FirstCsFound = TRUE;
                }
                mmPtr->NBPtr[AdjacentDie].MaxAggressorCSEnabled[Dct] = ChipSel;
                AggressorCount++;
              }
            } // ChipSel
            //
            // Track Maximum Aggressors for current channel
            //
            if (AggressorCount >= MaxAggressors) {
              Max2Aggressors = MaxAggressors;
              MaxAggressors = AggressorCount;
              MaxAggressorDie = AdjacentDie;
              MaxAggressorDct = Dct;
            } else {
              //
              // Also count second maximum
              //
              if (AggressorCount > Max2Aggressors) {
                Max2Aggressors = AggressorCount;
              }
            }
          } // Dct
        } // NodeMemSize != 0
      } // Same Socket
    } // AdjacentDie
    //
    // Assign Max Aggressor Values.  For each channel in the same package, the max aggressor will
    // be the highest number out of the other three channels.  That means that the channel with the
    // most chipselects has a max aggressor count equal to the second highest CS count. For the other
    // three, it will be the highest CS count.
    //
    for (AdjacentDie = 0 ; AdjacentDie < mmPtr->DieCount ; AdjacentDie ++ ) {
      if ( mmPtr->NBPtr[Die].DieEnabled[AdjacentDie] == TRUE) {
        for (Dct = 0; Dct < mmPtr->NBPtr[AdjacentDie].DctCount; Dct++) {
          if ((AdjacentDie == MaxAggressorDie) && (Dct == MaxAggressorDct)) {
            mmPtr->NBPtr[AdjacentDie].MaxAggressorDimms[Dct] = Max2Aggressors;
          } else {
            mmPtr->NBPtr[AdjacentDie].MaxAggressorDimms[Dct] = MaxAggressors;
          }
        } // Dct
      } // Enabled
    } // AdjacentDie
  } // Die
}
