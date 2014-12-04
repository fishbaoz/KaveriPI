/* $NoKeywords:$ */
/**
 * @file
 *
 * mfDllPDBypass.c
 *
 * DllPDBypass Mode
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/DllPDBypass)
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
#include "mu.h"
#include "mt.h"
#include "mport.h"
#include "mfDllPDBypass.h"
#include "OptionMemory.h"
#include "Filecode.h"

CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_FEAT_DLLPDBYPASS_MFDLLPDBYPASS_FILECODE
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
 *   This function initializes the DllPDBypass Feature Hooks.
 *
 *     @param[in,out] *NBPtr - Pointer to the MEM_NB_BLOCK
 *
 *     @return       BOOLEAN
 *                             TRUE - Function was implemented
 */

BOOLEAN
MemFDllPDBypassInit (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  ASSERT (NBPtr != NULL);
  NBPtr->FamilySpecificHook[DllPDBypassClkAddrCmd] = MemFDllPDBypassClkAddrCmd;
  NBPtr->FamilySpecificHook[DllPDBypassTxRxEn] = MemFDllPDBypassTxRxEn;
  NBPtr->FamilySpecificHook[DllPDBypassWL] = MemFDllPDBypassWL;
  NBPtr->FamilySpecificHook[DllPDBypassAdjustRxEnDly] = MemFDllPDBypassAdjustRxEnDly;

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function bypasses Clk/Addr/Cmd
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return  TRUE - Clk/Addr/Cmd not bypassed
 *     @return  FALSE - Clk/Addr/Cmd bypassed
*/
BOOLEAN
MemFDllPDBypassClkAddrCmd (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  // If (DllPDBypassMode) then For each memory P-state: Program D18F2x9C_x0D0F_[C,8,2][2:0]1E_dct[0]_mp[1:0][DllPDBypass] = 1.
  if (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) {
    MemNBrdcstSetUnConditionalNb (NBPtr, BFClkDllPDBypass, 0x80);
    MemNBrdcstSetUnConditionalNb (NBPtr, BFCmdDllPDBypass, 0x80);
    MemNBrdcstSetUnConditionalNb (NBPtr, BFAddrDllPDBypass, 0x80);
    MemNBrdcstSetUnConditionalNb (NBPtr, BFDllPDBypass, 0x00);
    return FALSE;
  } else {
    return TRUE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function bypasses Clk/Addr/Cmd
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return  TRUE - Tx/Rx bypassed
 *     @return  FALSE - Tx/Rx not bypassed
*/
BOOLEAN
MemFDllPDBypassTxRxEn (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  // If (DllPDBypassMode) D18F2x9C_x0D0F_0[F,8:0]1E_dct[0]_mp[1:0][DllPDBypass] = 0
  if (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) {
    MemNSetBitFieldNb (NBPtr, BFDllPDBypass, 0);
    return FALSE;
  } else {
    return TRUE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function bypasses write leveling by fixing WrDqsDly at 0
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return  TRUE - write leveling not bypassed
 *     @return  FALSE - write leveling bypassed
*/
BOOLEAN
MemFDllPDBypassWL (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  UINT8 ChipSel;
  UINT8 Dimm;
  UINT8 ByteLane;

  MEM_TECH_BLOCK *TechPtr;
  DCT_STRUCT *DCTPtr;

  TechPtr = NBPtr->TechPtr;
  DCTPtr = NBPtr->DCTPtr;

  if (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) {
    for (ChipSel = 0; ChipSel < NBPtr->CsPerChannel; ChipSel = ChipSel + NBPtr->CsPerDelay) {
      if ((DCTPtr->Timings.CsEnabled & ((UINT16) ((NBPtr->CsPerDelay == 2)? 3 : 1) << ChipSel)) != 0) {
        if (!(NBPtr->MCTPtr->Status[SbLrdimms]) || ((NBPtr->ChannelPtr->LrDimmPresent & ((UINT8) 1 << (ChipSel >> 1))) != 0)) {
          IDS_HDT_CONSOLE (MEM_STATUS, "\t\tCS %d\n", ChipSel);
          Dimm = ChipSel / NBPtr->CsPerDelay;
          ASSERT (Dimm < (NBPtr->CsPerChannel / NBPtr->CsPerDelay));

          for (TechPtr->TrnNibble = NIBBLE_0; TechPtr->TrnNibble <= (NBPtr->FamilySpecificHook[TrainWlPerNibble] (NBPtr, &Dimm)? NIBBLE_0 : NIBBLE_1); TechPtr->TrnNibble++) {
            // If (DllPDBypassMode) then
            // For each rank and lane: Program D18F2x9C_x0000_00[4A:30]_dct[0]_mp[1:0][WrDqsGrossDly, WrDqs-FineDly] = {0, 0}.
            // Skip Write Levelization Training
            IDS_HDT_CONSOLE (MEM_STATUS, "\nSkip write leveling in DllPDBypassModes.\n");
            for (ByteLane = 0; ByteLane < (NBPtr->MCTPtr->Status[SbEccDimms] ? 9 : 8); ByteLane++) {
              // This includes ECC as byte 8
              NBPtr->SetTrainDly (NBPtr, AccessWrDqsDly, DIMM_BYTE_ACCESS (Dimm, ByteLane), 0);
            }
          }
        }
      }
    }
    // return FALSE to skip regular WL training
    return FALSE;
  } else {
    return TRUE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function adjusts DQS Receiver enable delay in DllPDBypassMode
 *
 *     @param[in,out] *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out] *OptParam - Unused
 *
 *     @return  TRUE - RxEn Delay adjusted
 *     @return  FALSE - RxEn Delay not adjusted
*/
BOOLEAN
MemFDllPDBypassAdjustRxEnDly (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  )
{
  UINT8  ByteLane;
  UINT8  BypassFlopDelay;
  UINT16  AdjDelay[MAX_BYTELANES_PER_CHANNEL];
  UINT16  AdjDelayGross[MAX_BYTELANES_PER_CHANNEL];
  UINT16  AdjDelayFine;
  UINT16  RcvEnDly;

  if (NBPtr->IsSupported[SelectMotherboardPower] && NBPtr->RefPtr->EnableDllPDBypassMode) {
    // DQS Receiver EnableDllPDBypassMode Adjustment
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\nStart DQS Receiver EnableDllPDBypassMode Adjustments\n\n");
    // 1. Program D18F2x[1,0]9C_x0000_0008[FenceTrSel]=00b.
    MemNSetBitFieldNb (NBPtr, BFFenceTrSel, 0);
    // 2. Program D18F2x9C_x0000_00[52:50]_dct[0]=1010_1010h.

    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t           Byte:  00  01  02  03  04  05  06  07  ECC\n");
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\t          Seeds: ");
    for (ByteLane = 0; ByteLane < MAX_BYTELANES_PER_CHANNEL; ByteLane++) {
      // This includes ECC as byte 8
      MemNSetTrainDlyNb (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (0, ByteLane), 19);
      IDS_HDT_CONSOLE (MEM_FLOW, "%02x ", 19);
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t   PhyFenceTrEn = 1");
    // 3-2. Set F2x[1, 0]9C_x08[PhyFenceTrEn]=1.
    MemNSetBitFieldNb (NBPtr, BFPhyFenceTrEn, 1);

    if (!NBPtr->IsSupported[UnifiedNbFence]) {
      // 3-3. Wait 200 MEMCLKs.
      MemNWaitXMemClksNb (NBPtr, 200);
    } else {
      // 3-3. Wait 2000 MEMCLKs.
      MemNWaitXMemClksNb (NBPtr, 2000);
    }

    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t   PhyFenceTrEn = 0");
    // 3-4. Clear F2x[1, 0]9C_x08[PhyFenceTrEn]=0.
    MemNSetBitFieldNb (NBPtr, BFPhyFenceTrEn, 0);

    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\tBypassFlopDelay: ");
    for (ByteLane = 0; ByteLane < MAX_BYTELANES_PER_CHANNEL; ByteLane++) {
      //
      // This includes ECC as byte 8.   ECC Byte lane (8) is ignored by MemNGetTrainDlyNb function where
      // ECC is not supported.
      //
      BypassFlopDelay = (UINT8) (0x1F & MemNGetTrainDlyNb (NBPtr, AccessPhRecDly, DIMM_BYTE_ACCESS (0, ByteLane)));
      IDS_HDT_CONSOLE (MEM_FLOW, "%02x ", BypassFlopDelay);
      // AdjDelay = D18F2x9C_x0000_00[2A:10]_dct[0]_mp[1:0][DqsRcvEnGrossDelay, DqsRcvEnFineDelay] - BypassFlopDelay + 8.
      AdjDelay[ByteLane] = (UINT16) MemNGetTrainDlyNb (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((NBPtr->TechPtr->ChipSel / NBPtr->CsPerDelay), ByteLane)) - BypassFlopDelay + 8;
    }

    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t  AdjDelayGross: ");
    for (ByteLane = 0; ByteLane < MAX_BYTELANES_PER_CHANNEL; ByteLane++) {
      // AdjDelayGross = AdjDelay DIV 32
      AdjDelayGross[ByteLane] = AdjDelay[ByteLane] >> 5;
      IDS_HDT_CONSOLE (MEM_FLOW, "%02x ", AdjDelayGross[ByteLane]);
    }

    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\t\t   AdjDelayFine: ");
    for (ByteLane = 0; ByteLane < MAX_BYTELANES_PER_CHANNEL; ByteLane++) {
      // AdjDelayFine = AdjDelay & 10h
      AdjDelayFine = AdjDelay[ByteLane] & 0x10;
      IDS_HDT_CONSOLE (MEM_FLOW, "%02x ", AdjDelayFine);
      // Program D18F2x9C_x0000_00[2A:10]_dct[0]_mp[1:0][DqsRcvEnGrossDelay, DqsRcvEnFineDelay] = {AdjDelayGross, AdjDelayFine}.
      RcvEnDly = (AdjDelayGross[ByteLane] << 5) + AdjDelayFine;
      NBPtr->ChannelPtr->RcvEnDlys[(NBPtr->TechPtr->ChipSel / NBPtr->CsPerDelay) * NBPtr->TechPtr->DlyTableWidth () + ByteLane] = RcvEnDly;
      MemNSetTrainDlyNb (NBPtr, AccessRcvEnDly, DIMM_BYTE_ACCESS ((NBPtr->TechPtr->ChipSel / NBPtr->CsPerDelay), ByteLane), RcvEnDly);
    }

    // Program D18F2x9C_x0D0F_0[F,8:0]1E_dct[0]_mp[1:0][DllPDBypass] = 1.
    MemNSetBitFieldNb (NBPtr, BFDllPDBypass, 0x80);
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\nEnd DQS Receiver EnableDllPDBypassMode Adjustments\n\n");
    return TRUE;
  } else {
    return FALSE;
  }
}