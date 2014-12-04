/* $NoKeywords:$ */
/**
 * @file
 *
 * mrtthrcSeedTrain.c
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

#define FILECODE PROC_RECOVERY_MEM_TECH_MRTTHRCSEEDTRAIN_FILECODE
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
 *     This function executes Phy assisted receiver enable training without
 *     needing a correct seed.
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 *     @pre   Auto refresh and ZQCL must be disabled
 *
 */
VOID
MemRecTTrainRcvrEnHwSeedless (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT8  TempBuffer[64];
  UINT8  Count;
  UINT32 TestAddrRJ16;
  UINT8  ChipSel;
  UINT16 MaxRcvrDly;
  UINT8  PassMask;
  UINT8  PrevTest;
  UINT8  CurTest;
  UINT8  ByteLane;
  UINT32 OrgReg;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart HW RxEn training\n");
  AGESA_TESTPOINT (TpProcMemReceiverEnableTraining , &(NBPtr->MemPtr->StdHeader));

  //  Disable auto refresh by configuring F2x[1, 0]8C[DisAutoRefresh] = 1.
  NBPtr->SetBitField (NBPtr, BFDisAutoRefresh, 1);
  //  Disable ZQ calibration short command by configuring F2x[1, 0]94[ZqcsInterval] = 00b.
  NBPtr->SetBitField (NBPtr, BFZqcsInterval, 0);

  // Set environment settings before training
  MemRecTBeginTraining (TechPtr);

  ChipSel = NBPtr->DimmToBeUsed << 1;
  TechPtr->ChipSel = ChipSel;
  TestAddrRJ16 = 1 << 21;

  // 1.Prepare the DIMMs for training
  NBPtr->SetBitField (NBPtr, BFTrDimmSel, ChipSel >> 1);

  // 2.Prepare the phy for DQS receiver enable training.
  MemRecTPrepareRcvrEnDlySeed (TechPtr, ChipSel);

  // 3.BIOS initiates the phy assisted receiver enable training
  NBPtr->SetBitField (NBPtr, BFDqsRcvTrEn, 1);

  // 4.BIOS begins sending out of back-to-back reads to create
  //   a continuous stream of DQS edges on the DDR interface.
  for (Count = 0; Count < 3; Count++) {
    NBPtr->ReadPattern (NBPtr, TempBuffer, TestAddrRJ16, 64);
  }

  // 6.Wait 200 MEMCLKs.
  MemRecUWait10ns (200, NBPtr->MemPtr);

  // 7.Program [DqsRcvTrEn]=0 to stop the DQS receive enable training.
  NBPtr->SetBitField (NBPtr, BFDqsRcvTrEn, 0);

  // 8.Get the gross and fine delay values.
  // 9.Calculate the corresponding final delay values
  MaxRcvrDly = MemRecTProgramRcvrEnDly (TechPtr, ChipSel);

  //
  // SEEDLESS TRAINING
  // Sweep RxEn even gross delays to find the correct RxEn delays
  //
  OrgReg = MemRecNGetBitFieldNb (NBPtr, BFBlockRxDqsLock);
  NBPtr->SetBitField (NBPtr, BFBlockRxDqsLock, OrgReg | 0x0100);
  PassMask = 0;
  PrevTest = 0;

  // Use 3 CL pattern since recovery mode uses conservative settings and is not expected to work with long burst
  NBPtr->WritePattern (NBPtr, TestAddrRJ16, TempBuffer, 3);

  while (PassMask != 0xFF) {
    NBPtr->SetMaxLatency (NBPtr, MaxRcvrDly);
    NBPtr->ReadPattern (NBPtr, TempBuffer, TestAddrRJ16, 3);
    CurTest = (UINT8) NBPtr->CompareTestPattern (NBPtr, TempBuffer, TempBuffer, 3);

    // Mark pass when transition from P to F
    PassMask |= (PrevTest & (~CurTest));

    for (ByteLane = 0; ByteLane < 8; ByteLane++) {
      IDS_HDT_CONSOLE (MEM_FLOW, "  %02x", TechPtr->RcvrEnDlyOpt[ByteLane]);
      if ((CurTest & (1 << ByteLane)) == 0) {
        IDS_HDT_CONSOLE (MEM_FLOW, " .\n");
      } else {
        IDS_HDT_CONSOLE (MEM_FLOW, " P\n");
      }
      if ((PassMask & (1 << ByteLane)) == 0) {
        // For byte lanes that has not passed, increase by 2 UIs
        TechPtr->RcvrEnDlyOpt[ByteLane] += 0x40;
        NBPtr->SetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((ChipSel >> 1), ByteLane), TechPtr->RcvrEnDlyOpt[ByteLane]);
        if (TechPtr->RcvrEnDlyOpt[ByteLane] > MaxRcvrDly) {
          MaxRcvrDly = TechPtr->RcvrEnDlyOpt[ByteLane];
        }
      } else {
        // For byte lanes that has passed, decrease 5/2 UI to get back to the middle of preamble
        NBPtr->SetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((ChipSel >> 1), ByteLane), TechPtr->RcvrEnDlyOpt[ByteLane] - 0x50);
      }
    }
    PrevTest = CurTest;
  }
  NBPtr->SetBitField (NBPtr, BFBlockRxDqsLock, OrgReg);

  // Set final MaxRdLat
  NBPtr->SetMaxLatency (NBPtr, MaxRcvrDly - 0x50);

  // Restore environment settings after training
  MemRecTEndTraining (TechPtr);
  //  13.Program F2x[1, 0]8C[DisAutoRefresh] = 0.
  NBPtr->SetBitField (NBPtr, BFDisAutoRefresh, 0);
  //  14.Program F2x[1, 0]94[ZqcsInterval] to the proper interval for the current memory configuration.
  NBPtr->SetBitField (NBPtr, BFZqcsInterval, 2);

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
  UINT8  ByteLane;

  NBPtr = TechPtr->NBPtr;

  // Program a seed of 0x20 to make the result falls in (0,40) range
  IDS_HDT_CONSOLE (MEM_FLOW, "Seeds: 20\n");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    NBPtr->SetTrainDly (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (ChipSel >> 1, ByteLane), 0x20);
  }
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
  UINT8  ByteLane;
  UINT16 RcvEnDly;
  UINT16 MaxDly;

  NBPtr = TechPtr->NBPtr;
  ChannelPtr = TechPtr->NBPtr->ChannelPtr;
  MaxDly = 0;
  IDS_HDT_CONSOLE (MEM_FLOW, " PRE RxEn\n");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    // Read PRE
    RcvEnDly = (UINT8) NBPtr->GetTrainDly (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (ChipSel >> 1, ByteLane));
    IDS_HDT_CONSOLE (MEM_FLOW, "  %02x ", RcvEnDly);

    // Add 1 UI to get to the midpoint of preamble
    RcvEnDly += 0x20;

    // And add 1/2 UI to start seedless training
    RcvEnDly += 0x10;

    // Record Max RxEn for MaxRdLat calculation
    if (RcvEnDly > MaxDly) {
      MaxDly = RcvEnDly;
    }

    // Write RxEn delays
    NBPtr->SetTrainDly (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((ChipSel >> 1), ByteLane), RcvEnDly);
    TechPtr->RcvrEnDlyOpt[ByteLane] = RcvEnDly;
    IDS_HDT_CONSOLE (MEM_FLOW, "  %02x\n", RcvEnDly);
  }

  return MaxDly;
}

