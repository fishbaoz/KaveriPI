/* $NoKeywords:$ */
/**
 * @file
 *
 * mrtthrc.c
 *
 * Phy assisted DQS receiver enable training
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem)
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
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mru.h"
#include "mt.h"
#include "PlatformMemoryConfiguration.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_TECH_MRTTHRC_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MAX_BYTELANES 8             /* Max Bytelanes per channel */

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

VOID
STATIC
MemRecTPrepareRcvrEnDlySeed (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 ChipSel
  );

UINT16
STATIC
MemRecTProgramRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 ChipSel
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function executes Phy assisted receiver enable training for current node.
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 *     @pre   Auto refresh and ZQCL must be disabled
 *
 */
VOID
MemRecTTrainRcvrEnHw (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT8  TempBuffer[64];
  UINT8  Count;
  UINT32 TestAddr;
  UINT8  ChipSel;
  UINT16 MaxRcvrDly;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  AGESA_TESTPOINT (TpProcMemReceiverEnableTraining , &(NBPtr->MemPtr->StdHeader));
  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart HW RxEn training\n");

  // Set environment settings before training
  MemRecTBeginTraining (TechPtr);

  ChipSel = NBPtr->DimmToBeUsed << 1;
  TestAddr = 1 << 21;

  IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", TechPtr->NBPtr->Dct);
  IDS_HDT_CONSOLE (MEM_STATUS, "\t\tCS %d\n", ChipSel);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tTestAddr %x\n", TestAddr);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t     Byte:  00  01  02  03  04  05  06  07  ECC\n");

  // 1.Prepare the DIMMs for training
  NBPtr->SetBitField (NBPtr, BFTrDimmSel, ChipSel >> 1);

  // 2.Prepare the phy for DQS receiver enable training.
  MemRecTPrepareRcvrEnDlySeed (TechPtr, ChipSel);

  // 3.BIOS initiates the phy assisted receiver enable training
  NBPtr->SetBitField (NBPtr, BFDqsRcvTrEn, 1);

  // 4.BIOS begins sending out of back-to-back reads to create
  //   a continuous stream of DQS edges on the DDR interface.
  for (Count = 0; Count < 3; Count++) {
    NBPtr->ReadPattern (NBPtr, TempBuffer, TestAddr, 64);
  }

  // 6.Wait 200 MEMCLKs.
  MemRecUWait10ns (200, NBPtr->MemPtr);

  // 7.Program [DqsRcvTrEn]=0 to stop the DQS receive enable training.
  NBPtr->SetBitField (NBPtr, BFDqsRcvTrEn, 0);

  // 8.Get the gross and fine delay values.
  // 9.Calculate the corresponding final delay values
  MaxRcvrDly = MemRecTProgramRcvrEnDly (TechPtr, ChipSel);

  // Set Max Latency for both channels
  NBPtr->SetMaxLatency (NBPtr, MaxRcvrDly);

  // Restore environment settings after training
  MemRecTEndTraining (TechPtr);
  IDS_HDT_CONSOLE (MEM_FLOW, "End HW RxEn training\n\n");
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function calculates RcvEn seed value for each rank
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]  ChipSel - rank to be trained
 *
 */
VOID
STATIC
MemRecTPrepareRcvrEnDlySeed (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 ChipSel
  )
{
  MEM_NB_BLOCK  *NBPtr;
  CH_DEF_STRUCT *ChannelPtr;
  UINT16 SeedTotal;
  UINT16 SeedFine;
  UINT16 SeedGross;
  UINT16 SeedPreGross;
  UINT16 DiffSeedGrossSeedPreGross;
  UINT8  ByteLane;
  UINT16 PlatEst;
  UINT16 *PlatEstSeed;
  UINT16 SeedValue[8];
  UINT16 SeedTtl[8];
  UINT16 SeedPre[8];

  NBPtr = TechPtr->NBPtr;
  ChannelPtr = TechPtr->NBPtr->ChannelPtr;

  // Get platform override seed
  PlatEstSeed = (UINT16 *) MemRecFindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration, PSO_RXEN_SEED, NBPtr->MCTPtr->SocketId, ChannelPtr->ChannelID, ChipSel >> 1);

  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    // For Pass1, BIOS starts with the delay value obtained from the first pass of write
    // levelization training that was done in DDR3 Training and add a delay value of 3Bh.
    PlatEst = 0x3B;
    NBPtr->FamilySpecificHook[OverrideRcvEnSeed] (NBPtr, &PlatEst);
    PlatEst = ((PlatEstSeed != NULL) ? PlatEstSeed[ByteLane] : PlatEst);
    SeedTotal = ChannelPtr->WrDqsDlys[((ChipSel >> 1) * MAX_BYTELANES) + ByteLane] + PlatEst;
    SeedValue[ByteLane] = PlatEst;
    SeedTtl[ByteLane] = SeedTotal;
    // SeedGross = SeedTotal DIV 32.
    SeedGross = SeedTotal >> 5;
    // SeedFine = SeedTotal MOD 32.
    SeedFine = SeedTotal & 0x1F;

    // Next, determine the gross component of SeedTotal. SeedGrossPass1=SeedTotal DIV 32.
    // Then, determine the fine delay component of SeedTotal. SeedFinePass1=SeedTotal MOD 32.
    // Use SeedGrossPass1 to determine SeedPreGrossPass1:

    if ((SeedGross & 0x1) != 0) {
      //if SeedGross is odd
      SeedPreGross = 1;
    } else {
      //if SeedGross is even
      SeedPreGross = 2;
    }

    // (SeedGross - SeedPreGross)
    DiffSeedGrossSeedPreGross = SeedGross - SeedPreGross;

    ChannelPtr->RcvEnDlys[(ChipSel * MAX_BYTELANES) + ByteLane] =  DiffSeedGrossSeedPreGross << 5;

    //BIOS programs registers F2x[1, 0]9C_x[51:50] and F2x[1, 0]9C_x52) with SeedPreGrossPass1
    //and SeedFinePass1 from the preceding steps.
    NBPtr->SetTrainDly (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (ChipSel >> 1, ByteLane), (SeedPreGross << 5) | SeedFine);
    SeedPre[ByteLane] = (SeedPreGross << 5) | SeedFine;

    // 202688: Program seed value to RcvEnDly also.
    NBPtr->SetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS (ChipSel >> 1, ByteLane), SeedGross << 5);
  }
  IDS_HDT_CONSOLE_DEBUG_CODE (
   IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tSeedValue: ");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    IDS_HDT_CONSOLE (MEM_FLOW, "%03x ", SeedValue[ByteLane]);
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\n");

  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tSeedTotal: ");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    IDS_HDT_CONSOLE (MEM_FLOW, "%03x ", SeedTtl[ByteLane]);
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t  SeedPRE: ");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    IDS_HDT_CONSOLE (MEM_FLOW, "%03x ", SeedPre[ByteLane]);
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\n");
  );
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function calculates final RcvrEnDly for each rank
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]  ChipSel - Rank to be trained
 *
 *     @return   MaxDly - The largest delay value
 *
 */
UINT16
STATIC
MemRecTProgramRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 ChipSel
  )
{
  MEM_NB_BLOCK  *NBPtr;
  CH_DEF_STRUCT *ChannelPtr;
  UINT16 DiffSeedGrossSeedPreGross;
  UINT8  ByteLane;
  UINT16 RcvEnDly;
  UINT16 MaxDly;
  UINT16 RankRcvEnDly[8];
  NBPtr = TechPtr->NBPtr;
  ChannelPtr = TechPtr->NBPtr->ChannelPtr;
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t      PRE: ");
  MaxDly = 0;
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    DiffSeedGrossSeedPreGross = (ChannelPtr->RcvEnDlys[(ChipSel * MAX_BYTELANES) + ByteLane]) & 0x1E0;
    RcvEnDly = (UINT8) NBPtr->GetTrainDly (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (ChipSel >> 1, ByteLane));
    IDS_HDT_CONSOLE (MEM_FLOW, "%03x ", RcvEnDly);

    RcvEnDly = RcvEnDly + DiffSeedGrossSeedPreGross;

    // Add 1 UI to get to the midpoint of preamble
    RcvEnDly += 0x20;
    RankRcvEnDly[ByteLane] = RcvEnDly;

    if (RcvEnDly > MaxDly) {
      MaxDly = RcvEnDly;
    }

    NBPtr->SetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((ChipSel >> 1), ByteLane), RcvEnDly);
  }
  IDS_HDT_CONSOLE_DEBUG_CODE (
   IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t     RxEn: ");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    IDS_HDT_CONSOLE (MEM_FLOW, "%03x ", RankRcvEnDly[ByteLane]);
  }
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\n");
  );
  return MaxDly;
}

