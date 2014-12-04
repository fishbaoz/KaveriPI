/* $NoKeywords:$ */
/**
 * @file
 *
 * mfRdWr2DPatternGeneration.c
 *
 * Common Northbridge features
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/RdWr2DTraining)
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "mport.h"
#include "PlatformMemoryConfiguration.h"
#include "merrhdl.h"
#include "OptionMemory.h"
#include "mfRdWr2DTraining.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_FEAT_RDWR2DTRAINING_MFRDWR2DPATTERNGENERATION_FILECODE
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
 *     This function Initializes the Victim for 2D RdDqs Training
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return          BOOLEAN
 *                                TRUE
 */
BOOLEAN
MemFRdWr2DInitVictim (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  RRW_SETTINGS *Rrw;
  UINT8 InitialCS;
  UINT8 ChipSel;
  UINT8 SeedCount;
  BOOLEAN OptPatWr2D;

  OptPatWr2D = (NBPtr->IsSupported[OptimizedPatternWrite2D]) && (NBPtr->TechPtr->Direction == DQS_READ_DIR);
  InitialCS = NBPtr->TechPtr->ChipSel;
  Rrw = &NBPtr->RrwSettings;
  // Program the Bubble Count and CmdStreamLen
  NBPtr->SetBitField (NBPtr, BFBubbleCnt, 32);
  NBPtr->SetBitField (NBPtr, BFBubbleCnt2, 0);
  NBPtr->SetBitField (NBPtr, BFCmdStreamLen, 63);
  // Set Comparison Masks
  NBPtr->SetBitField (NBPtr, BFDramDqMaskLow, Rrw->CompareMaskLow);
  NBPtr->SetBitField (NBPtr, BFDramDqMaskHigh, Rrw->CompareMaskHigh);
  // If All Dimms are ECC Capable Test ECC. Otherwise, mask it off
  NBPtr->SetBitField (NBPtr, BFDramEccMask, (NBPtr->MCTPtr->Status[SbEccDimms] == TRUE) ? Rrw->CompareMaskEcc : 0xFF);
  // Program the Starting Address
  NBPtr->SetBitField (NBPtr, BFTgtBankA, Rrw->TgtBankAddressA);
  NBPtr->SetBitField (NBPtr, BFTgtAddressA, Rrw->TgtColAddressA);
  NBPtr->SetBitField (NBPtr, BFTgtBankB, Rrw->TgtBankAddressB);
  NBPtr->SetBitField (NBPtr, BFTgtAddressB, Rrw->TgtColAddressB);
  NBPtr->SetBitField (NBPtr, BFCmdTgt, CMD_TGT_AB);
  // Wait for RRW Engine to be ready and turn it on
  NBPtr->PollBitField (NBPtr, BFCmdSendInProg, 0, PCI_ACCESS_TIMEOUT, FALSE);
  NBPtr->SetBitField (NBPtr, BFCmdTestEnable, 1);
  for (ChipSel = InitialCS; ChipSel < (InitialCS + NBPtr->CsPerDelay); ChipSel++) {
    // Ensure that Odd and Even CS are trained
    if ((NBPtr->DCTPtr->Timings.CsEnabled & ((UINT16) 1 << ChipSel)) == 0) {
      continue;
    }
    NBPtr->TechPtr->ChipSel = ChipSel;
    for (SeedCount = 0; SeedCount < (OptPatWr2D ? NBPtr->MaxSeedCount : 1 ); SeedCount++) {
      if (OptPatWr2D) {
        //
        // Set BankAddress according to seed
        //
        ASSERT (NBPtr->MaxSeedCount <= 4);
        Rrw->TgtBankAddressA = (SeedCount * 2) + CPG_BANK_ADDRESS_A;
        Rrw->TgtBankAddressB = (SeedCount * 2) + CPG_BANK_ADDRESS_B;
      }
      //
      // Send ACTIVATE to all Banks
      //
      // Set Chip select
      MemNSetBitFieldNb (NBPtr, BFCmdChipSelect, (1 << NBPtr->TechPtr->ChipSel));
      // Set Bank Address
      MemNSetBitFieldNb (NBPtr, BFCmdBank, Rrw->TgtBankAddressA);
      // Set Row Address
      MemNSetBitFieldNb (NBPtr, BFCmdAddress, Rrw->TgtRowAddressA);
      // Send the command
      MemNSetBitFieldNb (NBPtr, BFSendActCmd, 1);
      // Wait for command complete
      MemNPollBitFieldNb (NBPtr, BFSendActCmd, 0, PCI_ACCESS_TIMEOUT, FALSE);
      // Set Chip select
      MemNSetBitFieldNb (NBPtr, BFCmdChipSelect, (1 << NBPtr->TechPtr->ChipSel));
      // Set Bank Address
      MemNSetBitFieldNb (NBPtr, BFCmdBank, Rrw->TgtBankAddressB);
      // Set Row Address
      MemNSetBitFieldNb (NBPtr, BFCmdAddress, Rrw->TgtRowAddressB);
      // Send the command
      MemNSetBitFieldNb (NBPtr, BFSendActCmd, 1);
      // Wait for command complete
      MemNPollBitFieldNb (NBPtr, BFSendActCmd, 0, PCI_ACCESS_TIMEOUT, FALSE);

      if (OptPatWr2D) {
        //
        // Write the Pattern to the bank pairs for each seed.
        //
        NBPtr->FamilySpecificHook[RdWr2DInitVictimChipSel] (NBPtr, NULL);
        NBPtr->SetBitField (NBPtr, BFTgtBankA, Rrw->TgtBankAddressA);
        NBPtr->SetBitField (NBPtr, BFTgtBankB, Rrw->TgtBankAddressB);
        // Program the PRBS Seed
        NBPtr->FamilySpecificHook[RdWr2DProgramDataPattern] (NBPtr, &SeedCount);
        //
        // Enable continuous writes on the victim channels
        //
        // Set the Command Count
        NBPtr->SetBitField (NBPtr, BFCmdCount, 256);
        NBPtr->SetBitField (NBPtr, BFCmdType, CMD_TYPE_WRITE);
        NBPtr->SetBitField (NBPtr, BFSendCmd, 1);
        // Wait for TestStatus = 1 and CmdSendInProg = 0.
        NBPtr->PollBitField (NBPtr, BFTestStatus, 1, PCI_ACCESS_TIMEOUT, FALSE);
        NBPtr->SetBitField (NBPtr, BFSendCmd, 0);
      }
    }
  }
  if (NBPtr->TechPtr->Direction == DQS_READ_DIR) {
    //
    // Do not use LFSR Rollover during Wr Training
    //
    NBPtr->SetBitField (NBPtr, BFLfsrRollOver, 1);
  }
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function Initializes the Victim chipSelects for 2D Read or Write
 *     Training Continuous Writes
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return          BOOLEAN
 *                               TRUE
 */
BOOLEAN
MemFRdWr2DInitVictimChipSel (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  NBPtr->SetBitField (NBPtr, BFTgtChipSelectA, NBPtr->TechPtr->ChipSel);
  NBPtr->SetBitField (NBPtr, BFTgtChipSelectB, NBPtr->TechPtr->ChipSel);
  NBPtr->SetBitField (NBPtr, BFResetAllErr, 1);
  return TRUE;
}



/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function checks the In Phase Error status bits for comparison
 *     results for RD/WR 2D training
 *
 *     @param[in,out] *NBPtr  - Pointer to the MEM_NB_BLOCK
 *     @param[out]    *Result - Pointer to UINT32 for storing results
 *
 *     @return         BOOLEAN
 *                               TRUE
 *
 */
BOOLEAN
MemFRdWr2DCompareInPhase (
  IN OUT   MEM_NB_BLOCK *NBPtr,
     OUT   VOID *Result
  )
{
  *(UINT32*)Result = NBPtr->GetBitField (NBPtr, BFNibbleErrSts);
  return TRUE;
}

 /*-----------------------------------------------------------------------------*/
/**
 *
 *       This function checks the 180 Error status bits for RD/WR 2D training
 *
 *     @param[in,out] *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[out]    *Result  - Pointer to UINT32 for storing results
 *
 *     @return         BOOLEAN
 *                               TRUE
 *
 */
BOOLEAN
MemFRdWr2DCompare180Phase (
  IN OUT   MEM_NB_BLOCK *NBPtr,
     OUT   VOID *Result
  )
{
  *(UINT32*)Result = NBPtr->GetBitField (NBPtr, BFNibbleErr180Sts);
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *       This function starts the Victim for 2D RdDqs Training Continuous Writes
 *
 *     @param[in,out] *NBPtr       -  Pointer to the MEM_NB_BLOCK
 *     @param[in]     SeedCountPtr -  UINT8 Pointer to Seed count
 *
 *     @return             BOOLEAN
 *                                    TRUE
 *
 */
BOOLEAN
MemFRdWr2DStartVictim (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *SeedCountPtr
  )
{
  BOOLEAN OptPatWr2D;
  UINT32 CommandCount;
  UINT8 SeedCount;

  ASSERT (NBPtr != NULL);
  ASSERT (SeedCountPtr != NULL);

  SeedCount = *(UINT8*)SeedCountPtr;

  OptPatWr2D = FALSE;
  if (NBPtr->TechPtr->Direction == DQS_READ_DIR) {
    OptPatWr2D = (NBPtr->IsSupported[OptimizedPatternWrite2D]);
    CommandCount = NBPtr->TotalBitTimes2DRdTraining / (8 * NBPtr->MaxSeedCount * NBPtr->MaxAggressorDimms[NBPtr->Dct]);
  } else {
    CommandCount = 256;
  }

  // Program the PRBS Seed
  NBPtr->FamilySpecificHook[RdWr2DProgramDataPattern] (NBPtr, &SeedCount);
  if (OptPatWr2D) {
    //
    // Set BankAddress according to seed
    //
    NBPtr->SetBitField (NBPtr, BFTgtBankA, (SeedCount * 2) + CPG_BANK_ADDRESS_A);
    NBPtr->SetBitField (NBPtr, BFTgtBankB, (SeedCount * 2) + CPG_BANK_ADDRESS_B);
  } else {
    //
    // Enable continuous writes on the victim channels
    //
    // Set the Command Count
    NBPtr->SetBitField (NBPtr, BFCmdCount, 256);
    NBPtr->SetBitField (NBPtr, BFCmdType, CMD_TYPE_WRITE);
    NBPtr->SetBitField (NBPtr, BFSendCmd, 1);
    // Wait for TestStatus = 1 and CmdSendInProg = 0.
    NBPtr->PollBitField (NBPtr, BFTestStatus, 1, PCI_ACCESS_TIMEOUT, FALSE);
    NBPtr->SetBitField (NBPtr, BFSendCmd, 0);
  }
  //
  // Enable continuous reads on the victim channels
  //
  // Set the Command Count
  ASSERT (NBPtr->MaxAggressorDimms[NBPtr->Dct] != 0);
  //
  NBPtr->SetBitField (NBPtr, BFCmdCount, CommandCount );
  // Reset All Errors and Disable StopOnErr
  NBPtr->SetBitField (NBPtr, BFCmdType, CMD_TYPE_READ);
  NBPtr->SetBitField (NBPtr, BFSendCmd, 1);
  // Wait for TestStatus = 1 and CmdSendInProg = 0
  NBPtr->PollBitField (NBPtr, BFTestStatus, 1, PCI_ACCESS_TIMEOUT, FALSE);
  //}
  NBPtr->SetBitField (NBPtr, BFSendCmd, 0);
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function finalizes the Victim for 2D RdDqs Training
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return         BOOLEAN
 *                               TRUE - Success
 */
BOOLEAN
MemFRdWr2DFinalizeVictim  (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 InitialCS;
  UINT8 ChipSel;
  InitialCS = NBPtr->TechPtr->ChipSel;
  NBPtr->SetBitField (NBPtr, BFLfsrRollOver, 0);
  for (ChipSel = InitialCS; ChipSel < (InitialCS + NBPtr->CsPerDelay); ChipSel++) {
    // Ensure that Odd and Even CS are precharged
    if ((NBPtr->DCTPtr->Timings.CsEnabled & ((UINT16) 1 << ChipSel)) == 0) {
      continue;
    }
    NBPtr->TechPtr->ChipSel = ChipSel;
    // Send the Precharge All Command
    MemNRrwPrechargeCmd (NBPtr, ChipSel, PRECHARGE_ALL_BANKS);
  }
  // Turn Off the RRW Engine
  NBPtr->SetBitField (NBPtr, BFCmdTestEnable, 0);
  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the Data Pattern that will be sent and compared
 *     against.
 *
 *     @param[in,out] *NBPtr           - Pointer to the MEM_NB_BLOCK
 *     @param[in]     *PatternIndexPtr - Pointer to a generic index used to
 *                                         determine which pattern to program.
 *
 *     @return                 BOOLEAN
 *                                      TRUE
 *
 */
BOOLEAN
MemFRdWr2DProgramDataPattern (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID* PatternIndexPtr
  )
{
  UINT8 SeedCount;
  UINT32 PrbsSeed;
  CONST STATIC UINT32 PrbsSeedTbl[4] = {0x7ea05, 0x44443, 0x22b97, 0x3f167};
  CONST STATIC UINT32 CmdStreamLenTbl[4] = {13, 61, 127, 251};

  ASSERT (NBPtr != 0);
  ASSERT (PatternIndexPtr != NULL);
  SeedCount = *(UINT8*)PatternIndexPtr;
  ASSERT (SeedCount <= (NBPtr->MaxSeedCount - 1));
  //
  // Program the Command Stream Length
  //
  if (NBPtr->TechPtr->Direction == DQS_READ_DIR) {
    NBPtr->SetBitField (NBPtr, BFCmdStreamLen, CmdStreamLenTbl[SeedCount]);
  } else {
    NBPtr->SetBitField (NBPtr, BFCmdStreamLen, 63);
  }
  PrbsSeed = PrbsSeedTbl[SeedCount % GET_SIZE_OF (PrbsSeedTbl)];
  ASSERT (PrbsSeed != 0);
  //
  // Program the PRBS Seed
  //
  NBPtr->SetBitField (NBPtr, BFDataPrbsSeed, PrbsSeed);
  return TRUE;
}
