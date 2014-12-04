/* $NoKeywords:$ */
/**
 * @file
 *
 * mrttsrc.c
 *
 * Technology Software based DQS receiver enable training Recovery
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
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_TECH_MRTTSRC_FILECODE
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
MemRecTSetRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver,
  IN       UINT16 RcvEnDly
  );

VOID
STATIC
MemRecTLoadRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver
  );

BOOLEAN
STATIC
MemRecTSaveRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver,
  IN       UINT16 RcvEnDly,
  IN       UINT8 CmpResult
  );

UINT8
STATIC
MemRecTCompare1ClPattern (
  IN       UINT8 Buffer[],
  IN       UINT8 Pattern[],
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function executes receiver enable training for BSP
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTTrainRcvrEnSw (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  _16BYTE_ALIGN  UINT8  PatternBuffer[3 * 64];
  UINT8  TestBuffer[120];
  UINT8  *PatternBufPtr[2];
  UINT32 TestAddr[4];
  UINT8  TestResult;
  UINT8 Receiver;
  UINT8 i;
  UINT8 j;
  UINT16 RcvrEnDly;

  MEM_DATA_STRUCT *MemPtr;
  DIE_STRUCT *MCTPtr;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;
  MCTPtr = NBPtr->MCTPtr;

  AGESA_TESTPOINT (TpProcMemReceiverEnableTraining, &(MemPtr->StdHeader));

  // Set environment settings before training
  MemRecTBeginTraining (TechPtr);

  PatternBufPtr[0] = PatternBuffer;
  MemRecUFillTrainPattern (TestPattern0, PatternBufPtr[0], 64, &(MemPtr->StdHeader));
  PatternBufPtr[1] = PatternBufPtr[0] + 128;
  MemRecUFillTrainPattern (TestPattern1, PatternBufPtr[1], 64, &(MemPtr->StdHeader));

  // Begin receiver enable training
  MemRecTSetWrDatRdDqs (TechPtr, 0);

  // there are four receiver pairs, loosely associated with chipselects.
  Receiver = NBPtr->DimmToBeUsed << 1;
  TechPtr->DqsRcvEnSaved = 0;

  TestAddr[0] = NBPtr->GetSysAddrRec ();
  TestAddr[1] = TestAddr[0] + BIGPAGE_X8;

  IDS_HDT_CONSOLE (MEM_FLOW, "\tDct %d\n", NBPtr->Dct);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCS %d\n", Receiver);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tWrite to addresses:  %04x0000\n", TestAddr[0]);

  // Sweep receiver enable delays
  AGESA_TESTPOINT (TpProcMemRcvrStartSweep, &(MemPtr->StdHeader));
  for (RcvrEnDly = 0; RcvrEnDly < 0xFF; RcvrEnDly++) {

    TestResult = 0xFF;
    for (i = 0; i < 2; i++) {

      // Set RcvrEn delay for all byte lanes
      AGESA_TESTPOINT (TpProcMemRcvrSetDelay, &(MemPtr->StdHeader));
      MemRecTSetRcvrEnDly (TechPtr, Receiver, RcvrEnDly);
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDly %3x", RcvrEnDly);

      // Swap the test pointers such that even and odd steps alternate.
      j = ((RcvrEnDly & 1) != 0) ? (i ^ 1) : i;

      // Write, read and compare the first beat of data
      AGESA_TESTPOINT (TpProcMemRcvrWritePattern, &(MemPtr->StdHeader));
      MemRecUWrite1CL (TestAddr[j], PatternBufPtr[j]);
      AGESA_TESTPOINT (TpProcMemRcvrReadPattern, &(MemPtr->StdHeader));
      MemRecURead1CL (TestBuffer, TestAddr[j]);
      AGESA_TESTPOINT (TpProcMemRcvrTestPattern, &(MemPtr->StdHeader));
      TestResult &= MemRecTCompare1ClPattern (TestBuffer, PatternBufPtr[j], &(MemPtr->StdHeader));
      MemRecUProcIOClFlush (TestAddr[j], MemPtr);
    }

    if (MemRecTSaveRcvrEnDly (TechPtr, Receiver, RcvrEnDly, TestResult)) {
      // if all bytelanes pass
      break;
    }
  }   // End of delay sweep

  if (RcvrEnDly == 0xFF) {
    // no passing window
    // LibAmdEventLog (AGESA_ERROR, MEM_ERROR_RCVR_EN_NO_PASSING_WINDOW, 0, NBPtr->Dct, NBPtr->Channel, 0);   //@attention commented out since it is not defined in recovery code
    SetMemRecError (AGESA_ERROR, MCTPtr);
  }

  // set final delays
  MemRecTLoadRcvrEnDly (TechPtr, Receiver);

  // Clear training bit when done
  NBPtr->SetBitField (NBPtr, BFDqsRcvEnTrain, 0);

  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMaxRcvrEnDly: %03x\n", RcvrEnDly + 0x20);

  // Set Max Latency for both channels
  NBPtr->SetMaxLatency (NBPtr, RcvrEnDly + 0x20);

  // Restore environment settings after training
  MemRecTEndTraining (TechPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      If WrDatDly is 0, this function sets the DQS Positions in preparation
 *      for Receiver Enable Training. (Write Position is no delay, Read Position is 1.5 Memclock delay).
 *      Otherwise it will set WrDat and RdDqs to center of data eye.
 *
 *     @param[in,out]   *TechPtr - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       WrDatDly - either 0 or 0x0F
 *
 */

VOID
MemRecTSetWrDatRdDqs (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 WrDatDly
  )
{
  UINT8 ByteLane;
  UINT8 Dimm;
  UINT8 WrDqs;
  UINT8 RdDqs;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  Dimm = NBPtr->DimmToBeUsed;
  IDS_HDT_CONSOLE (MEM_FLOW, "\nWrDat RdDqs\n");
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    WrDqs = NBPtr->ChannelPtr->WrDqsDlys[(Dimm * MAX_BYTELANES) + ByteLane];
    NBPtr->SetTrainDly (NBPtr, AccessWrDatDly, DIMM_BYTE_ACCESS (Dimm, ByteLane), WrDqs + WrDatDly);
    RdDqs = (WrDatDly == 0) ? 0x2F : 0x012;
    NBPtr->SetTrainDly (NBPtr, AccessRdDqsDly, DIMM_BYTE_ACCESS (Dimm, ByteLane), RdDqs);
    IDS_HDT_CONSOLE (MEM_FLOW, "  %02x  %02x\n\n", WrDqs + WrDatDly, RdDqs);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function programs DqsRcvEnDly to additional index for DQS receiver enabled training
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       Receiver  - Current Chip select value
 *     @param[in]       RcvEnDly  - receiver enable delay to be saved
 *
 */

VOID
STATIC
MemRecTSetRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver,
  IN       UINT16 RcvEnDly
  )
{
  UINT8 ByteLane;

  ASSERT (Receiver <= MAX_CS_PER_CHANNEL);
  for (ByteLane = 0; ByteLane < 8; ByteLane++) {
    TechPtr->NBPtr->SetTrainDly (TechPtr->NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS (Receiver >> 1, ByteLane), RcvEnDly);
  }
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *       This function compares test pattern with data in buffer and return a pass/fail bitmap
 *       for 8 Bytes
 *
 *     @param[in]       Buffer[]  -  Buffer data from DRAM (Measured data from DRAM) to compare
 *     @param[in]       Pattern[]  - Pattern (Expected data in ROM/CACHE) to compare against
 *     @param[in,out] StdHeader - The Pointer of AGESA Header.
 *
 *     @return  PASS - Bit map of results of comparison
 */

UINT8
STATIC
MemRecTCompare1ClPattern (
  IN       UINT8 Buffer[],
  IN       UINT8 Pattern[],
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 i;
  UINT8 Pass;

  Pass = 0xFF;
  IDS_HDT_CONSOLE (MEM_FLOW, " -");
  for (i = 0; i < 8; i++) {
    if (Buffer[i] != Pattern[i]) {
      // if bytelane n fails
      Pass &= ~((UINT16) 1 << (i % 8));    // clear bit n
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "  %c", (Buffer[i] == Pattern[i]) ? 'P' : '.');
  }

  IDS_HDT_CONSOLE_DEBUG_CODE (
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t        -");
    for (i = 0; i < 8; i++) {
      IDS_HDT_CONSOLE (MEM_FLOW, " %02x", Buffer[i]);
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t        -");
    for (i = 0; i < 8; i++) {
      IDS_HDT_CONSOLE (MEM_FLOW, " %02x", Pattern[i]);
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\n");
  );

  return Pass;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function saves passing DqsRcvEnDly values to the stack
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       Receiver  - Current Chip select value
 *     @param[in]       RcvEnDly  - receiver enable delay to be saved
 *     @param[in]       CmpResult - compare result for Rank 0
 *
 *     @return  TRUE - All bytelanes pass
 *     @return  FALSE - Some bytelanes fail
 */

BOOLEAN
STATIC
MemRecTSaveRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver,
  IN       UINT16 RcvEnDly,
  IN       UINT8 CmpResult
  )
{
  UINT8 i;
  UINT8 Passed;
  UINT8 Saved;
  UINT8 Mask;
  UINT8 Dimm;

  ASSERT (Receiver <= MAX_CS_PER_CHANNEL);

  Passed = CmpResult;
  Saved = (UINT8) (TechPtr->DqsRcvEnSaved & Passed);
  Dimm = Receiver >> 1;
  Mask = 1;
  for (i = 0; i < 8; i++) {
    if ((Passed & Mask) != 0) {
      if ((Saved & Mask) == 0) {
        TechPtr->NBPtr->ChannelPtr->RcvEnDlys[Dimm * MAX_BYTELANES + i] = (UINT8) (RcvEnDly + 0x20);
        IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tBL %d = %02x", i, RcvEnDly + 0x20);
      }
      Saved |= Mask;
    }
    Mask <<= 1;
  }
  TechPtr->DqsRcvEnSaved = Saved;

  if (Saved == 0xFF) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function loads the DqsRcvEnDly from saved data and program to additional index
 *  for DQS receiver enabled training
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       Receiver  - Current Chip select value
 *
 */

VOID
STATIC
MemRecTLoadRcvrEnDly (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Receiver
  )
{
  UINT8 i;
  UINT8 Dimm;
  UINT16 Saved;
  CH_DEF_STRUCT *ChannelPtr;

  ASSERT (Receiver <= MAX_CS_PER_CHANNEL);
  ChannelPtr = TechPtr->NBPtr->ChannelPtr;

  Dimm = Receiver >> 1;
  Saved = TechPtr->DqsRcvEnSaved;
  for (i = 0; i < 8; i++) {
    if ((Saved & 1) != 0) {
      TechPtr->NBPtr->SetTrainDly (TechPtr->NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS (Receiver >> 1, i),
      ChannelPtr->RcvEnDlys[Dimm * MAX_BYTELANES + i]);
    }
    Saved >>= 1;
  }
}
