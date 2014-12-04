/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmu.c
 *
 * Common Northbridge function for PMU initialization
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
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "merrhdl.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_NB_MNPMU_FILECODE

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

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function resets the PMU
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPmuResetNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // 1. Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
  MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);

  // 2. Program D18F2x9C_x0002_000E_dct[3:0][PhyDisable]=0.
  MemNSetBitFieldNb (NBPtr, BFPhyDisable, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function commands PMU to start DRAM initialization and/or DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNStartPmuNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_HDT_CONSOLE (MEM_FLOW, "Start PMU on image%d...\n", NBPtr->PmuFirmwareImage);
  // Initialize the protocol bit prior use.
  MemNSetBitFieldNb (NBPtr, BFRdy, 1);
  MemNSetBitFieldNb (NBPtr, BFRdy2, 1);

  // Clear PmuStall to start PMU
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function waits for PMU to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - PMU completes
 *     @return          FALSE - PMU fails
 */

BOOLEAN
MemNPendOnPmuCompletionNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 UsMsg;
  UINT16 Us2Msg;
  INT16 Us2MsgIndex;
  INT16 Count;

  UsMsg = 0;
  Us2Msg = 0;
  Count = 0;

  while (UsMsg != USMSG_PMUQEMPTY) {
    // 1. Wait until D18F2x9C_x0002_0004_dct[3:0][UsRdy]==0.
    MemNPollBitFieldNb (NBPtr, BFUsRdy, 0, PCI_ACCESS_TIMEOUT, FALSE);

    // 2. Read D18F2x9C_x0002_0032_dct[3:0][Message]
    UsMsg = (UINT16) MemNGetBitFieldNb (NBPtr, BFUsMessage);
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tUsMsg: %x\n", UsMsg);

    Us2MsgIndex = 0;

    // Print out debug data if available
    if (UsMsg == USMSG_US2MSGRDY) {
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tStream message through US mailbox 2\n");
      do {
        // 1. Wait until D18F2x9C_x0002_0004_dct[3:0][Us2Rdy]==0.
        MemNPollBitFieldNb (NBPtr, BFUs2Rdy, 0, PCI_ACCESS_TIMEOUT, FALSE);

        // 2. Read D18F2x9C_x0002_0034_dct[3:0][Message]
        Us2Msg = (UINT16) MemNGetBitFieldNb (NBPtr, BFUs2Message);
        IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tUs2Msg : %x\n", Us2Msg);

        if (Us2MsgIndex == 0) {
          // The first item received is the COUNT.
          Count = (INT16)Us2Msg;
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tUs2Msg COUNT: %x\n", Count);
        }

        Us2MsgIndex++;

        // 3. Program D18F2x9C_x0002_0035_dct[3:0][Rdy] = 1.
        MemNSetBitFieldNb (NBPtr, BFRdy2, 1);

        // 4. Decrement COUNT and loop to step 1 until COUNT == -1.
      } while (--Count >= 0);
    }

    // 3. Program D18F2x9C_x0002_0033_dct[3:0][Rdy] = 1.
    MemNSetBitFieldNb (NBPtr, BFRdy, 1);

    if (UsMsg == USMSG_FAIL) {
      return FALSE;
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR0
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          MR0 value
 *
 */

UINT32
MemNCalcMR0 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 MrsAddress;
  UINT32 TclEnc;
  UINT32 TwrEnc;
  UINT32 Ppd;

  // program MrsAddress[1:0]=burst length and control method
  // (BL):based on F2x[1,0]84[BurstCtrl]
  MrsAddress = MemNGetBitFieldNb (NBPtr, BFBurstCtrl);

  // program MrsAddress[3]=1 (BT):interleaved
  MrsAddress |= (UINT16) 1 << 3;

  // program MrsAddress[6:4,2]=read CAS latency
  TclEnc = MemNGetBitFieldNb (NBPtr, BFTcl) - 4;
  MrsAddress |= (TclEnc >> 3) << 2;
  MrsAddress |= (TclEnc & 7) << 4;

  // program MrsAddress[11:9]=write recovery for auto-precharge
  TwrEnc = MemNGetBitFieldNb (NBPtr, BFTwrDDR3);
  TwrEnc = (TwrEnc >= 10) ? ((TwrEnc + 1) / 2) : (TwrEnc - 4);
  MrsAddress |= TwrEnc << 9;

  // program MrsAddress[12] (PPD):based on F2x[1,0]84[PChgPDModeSel]
  Ppd = MemNGetBitFieldNb (NBPtr, BFPchgPDModeSel);
  MrsAddress |= Ppd << 12;

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR1
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       ChipSel - Rank to be trained
 *
 *     @return          MR1 value
 *
 */

UINT32
MemNCalcMR1 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  )
{
  UINT32 MrsAddress;
  UINT32 RttNom;

  MrsAddress = 0;

  // program MrsAddress[5,1]=output driver impedance control (DIC):
  MrsAddress |= ((UINT16) 1 << 1);

  // program MrsAddress[9,6,2]=nominal termination resistance of ODT (RTT):
  // Different CS may have different RTT.
  RttNom = NBPtr->PsPtr->RttNom[ChipSel];
  IDS_OPTION_HOOK (IDS_MEM_DRAM_TERM, &RttNom, &NBPtr->MemPtr->StdHeader);
  MrsAddress |= (((RttNom >> 2) & 1) << 9) | (((RttNom >> 1) & 1) << 6) | ((RttNom & 1) << 2);

  // program MrsAddress[12]=output disable (QOFF): 0

  // program MrsAddress[11]=TDQS:
  if ((NBPtr->DCTPtr->Timings.Dimmx4Present != 0) && (NBPtr->DCTPtr->Timings.Dimmx8Present != 0)) {
    if ((NBPtr->DCTPtr->Timings.Dimmx8Present & ((UINT8) 1 << (ChipSel >> 1))) != 0) {
      MrsAddress |= ((UINT16) 1 << 11);
    }
  }

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR2
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       ChipSel - Rank to be trained
 *
 *     @return          MR2 value
 *
 */

UINT32
MemNCalcMR2 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  )
{
  UINT32 MrsAddress;
  UINT32 RttWr;

  // program MrsAddress[5:3]=CAS write latency (CWL):
  MrsAddress = (MemNGetBitFieldNb (NBPtr, BFTcwl) - 5) << 3;

  // program MrsAddress[6]=auto self refresh method (ASR):
  // program MrsAddress[7]=self refresh temperature range (SRT):
  MrsAddress |= 1 << 6;
  MrsAddress &= ( ~ (1 << 7));

  // program MrsAddress[10:9]=dynamic termination during writes (RTT_WR):
  RttWr = NBPtr->PsPtr->RttWr[ChipSel];
  IDS_OPTION_HOOK (IDS_MEM_DYN_DRAM_TERM, &RttWr, &NBPtr->MemPtr->StdHeader);
  MrsAddress |= RttWr << 9;

  return MrsAddress;
}
