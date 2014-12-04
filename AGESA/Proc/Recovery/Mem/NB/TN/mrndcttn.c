/* $NoKeywords:$ */
/**
 * @file
 *
 * mrndcttn.c
 *
 * Northbridge DCT support for TN Recovery
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem/NB/TN)
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
#include "PlatformMemoryConfiguration.h"
#include "Ids.h"
#include "mrport.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mp.h"
#include "mrt3.h"
#include "mrntn.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_RECOVERY_MEM_NB_TN_MRNDCTTN_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MAX_RD_DQS_DLY            0x1F

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
 *   This function gets platform specific config/timing values from the interface layer and
 *   programs them into DCT.
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - An Error value lower than AGESA_ERROR may have occurred
 *     @return          FALSE - An Error value greater than or equal to AGESA_ERROR may have occurred
 */

BOOLEAN
MemRecNPlatformSpecTN (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  if (MemPRecPSCFlow (NBPtr)) {
    // AddrTmg and OdcCtl
    MemRecNSetBitFieldNb (NBPtr, BFODCControl, NBPtr->ChannelPtr->DctOdcCtl);
    MemRecNSetBitFieldNb (NBPtr, BFAddrTmgControl, NBPtr->ChannelPtr->DctAddrTmg);
    // ODT pattern
    MemRecNSetBitFieldNb (NBPtr, BFPhyWODTCSLow, NBPtr->ChannelPtr->PhyWODTCSLow);
    MemRecNSetBitFieldNb (NBPtr, BFPhyWODTCSHigh, NBPtr->ChannelPtr->PhyWODTCSHigh);
    MemRecNSetBitFieldNb (NBPtr, BFPhyRODTCSLow, NBPtr->ChannelPtr->PhyRODTCSLow);
    MemRecNSetBitFieldNb (NBPtr, BFPhyRODTCSHigh, NBPtr->ChannelPtr->PhyRODTCSHigh);
    return TRUE;
  } else {
    return FALSE;
  }
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *   Set Dram ODT for mission mode and write leveling mode.
 *
 *     @param[in,out]   *NBPtr     - Pointer to the MEM_NB_BLOCK
 *     @param[in]       OdtMode    - Mission mode or write leveling mode
 *     @param[in]       ChipSelect - Chip select number
 *     @param[in]       TargetCS   - Chip select number that is being trained
 *
 */

VOID
MemRecNSetDramOdtTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       ODT_MODE OdtMode,
  IN       UINT8 ChipSelect,
  IN       UINT8 TargetCS
  )
{
  UINT8 DramTerm;
  UINT8 DramTermDyn;

  // Dram nominal termination & Dram dynamic termination
  DramTerm = NBPtr->PsPtr->RttNom[ChipSelect];
  DramTermDyn = NBPtr->PsPtr->RttWr[ChipSelect];

  if (OdtMode == WRITE_LEVELING_MODE) {
    if (ChipSelect == TargetCS) {
      DramTerm = DramTermDyn;
      MemRecNSetBitFieldNb (NBPtr, BFWrLvOdt, NBPtr->ChannelPtr->PhyWLODT[TargetCS >> 1]);
    }
  }
  NBPtr->PsPtr->DramTerm = DramTerm;
  NBPtr->PsPtr->DynamicDramTerm = DramTermDyn;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function calculates and programs NB P-state dependent registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemRecNProgramNbPstateDependentRegistersTN (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{

  MemRecNSetBitFieldNb (NBPtr, BFRdPtrInit, 6);

  MemRecNSetBitFieldNb (NBPtr, BFProcOdtAdv, 0);

  MemRecNSetBitFieldNb (NBPtr, BFDataTxFifoWrDly, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *  This function initializes the DRAM devices for TN
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemRecNStartupDCTTN (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 CurDct;
  UINT8 FinalPllLockTime;

  // 1. Program D18F2x9C_x0D0F_E013_dct[1:0] = 0118h.
  MemRecNSetBitFieldNb (NBPtr, BFPllRegWaitTime, 0x118);
  // 2. Force the phy to M0 with the following sequence:
  // A. Program D18F2x9C_x0D0F_E006_dct[1:0][PllLockTime] = 190h. Restore the default PLL lock time.
  MemRecNSetBitFieldNb (NBPtr, BFPllLockTime, 0x190);
  // B. For each DCT: Program D18F2x9C_x0000_000B_dct[1:0] = 80800000h.
  MemRecNSetBitFieldNb (NBPtr, BFDramPhyStatusReg, 0x80800000);
  CurDct = NBPtr->Dct;
  NBPtr->MemRecNSwitchDctNb (NBPtr, 0);
  // C. Program D18F2x9C_x0D0F_E018_dct[0][PhyPSMasterChannel] = 0.
  MemRecNSetBitFieldNb (NBPtr, BFPhyPSMasterChannel, 0);
  // D. Program D18F2x9C_x0000_000B_dct[0] = 40000000h. CH0 only
  MemRecNSetBitFieldNb (NBPtr, BFDramPhyStatusReg, 0x40000000);
  NBPtr->MemRecNSwitchDctNb (NBPtr, CurDct);
  // E. For each DCT: Program D18F2x9C_x0000_000B_dct[1:0] = 80000000h.
  MemRecNSetBitFieldNb (NBPtr, BFDramPhyStatusReg, 0x80000000);

  // Phy Voltage Level Programming
  MemRecNPhyVoltageLevelNb (NBPtr);

  // Run frequency change sequence
  MemRecNSetBitFieldNb (NBPtr, BFPllLockTime, 0x190);
  MemRecNSetBitFieldNb (NBPtr, BFMemClkFreq, 4);
  MemRecNProgramNbPstateDependentRegistersTN (NBPtr);
  MemRecNSetBitFieldNb (NBPtr, BFMemClkFreqVal, 1);
  while (MemRecNGetBitFieldNb (NBPtr, BFFreqChgInProg) != 0) {}
  FinalPllLockTime = 0xF;
  if (MemRecNGetBitFieldNb (NBPtr, BFMemPhyPllPdMode) == 2) {
    FinalPllLockTime = 0x190;
  }
  MemRecNSetBitFieldNb (NBPtr, BFPllLockTime, FinalPllLockTime);

  NBPtr->FeatPtr->DramInit (NBPtr->TechPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function gets the total of sync components for Max Read Latency calculation
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return      Total in PCLKs
 */
UINT32
STATIC
MemRecNTotalSyncComponentsTN (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 P;

  P = 0;

  // 3. If (D18F2x9C_x0000_0004_dct[1:0][AddrCmdSetup] = 0 & D18F2x9C_x0000_0004_dct[1:0][CsOdt-
  // Setup] = 0 & D18F2x9C_x0000_0004_dct[1:0][CkeSetup] = 0)
  // then P = P + 1
  // else P = P + 2
  if ((MemRecNGetBitFieldNb (NBPtr, BFAddrTmgControl) & 0x0202020) == 0) {
    P += 1;
  } else {
    P += 2;
  }

  // 4. P = P + (8 - D18F2x210_dct[1:0]_nbp[3:0][RdPtrInit])
  P = P + (8 - (UINT16) MemRecNGetBitFieldNb (NBPtr, BFRdPtrInit));

  // 6. P = P + (2 * (D18F2x200_dct[1:0][Tcl] - 1 clocks))
  P = P + (2 * (MemRecNGetBitFieldNb (NBPtr, BFTcl) - 1));

  return P;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function  sets the maximum round-trip latency in the system from the processor to the DRAM
 *   devices and back.

 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]     MaxRcvEnDly - Maximum receiver enable delay value
 *
 */

VOID
MemRecNSetMaxLatencyTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly
  )
{
  UINT32 N;
  UINT32 T;
  UINT32 P;
  UINT32 Px2;
  UINT32 NBClkFreq;
  UINT32 MemClkPeriod;

  // Find NB frequency
  NBClkFreq = (200 * (MemRecNGetBitFieldNb (NBPtr, BFNbFid) + 4)) >> MemRecNGetBitFieldNb (NBPtr, BFNbDid);

  // 1. P = N = T = 0.
  P = N = T = 0;

  // Get all sync components BKDG steps 3,4,6
  P = MemRecNTotalSyncComponentsTN (NBPtr);

  // 5. P = P + 5
  P += 5;

  // 7. P = P + CEIL(MAX(D18F2x9C_x0000_00[2A:10]_dct[1:0][DqsRcvEnGrossDelay, DqsRcvEnFineDelay] +
  //    D18F2x9C_x0000_0[3:0]0[6:5]_dct[1:0][RdDqsTime] PCLKs)) + 1
  P = P + ((MaxRcvEnDly + MAX_RD_DQS_DLY) + 31) / 32 + 1;

  // 8. If (NclkFreq/MemClkFreq < 2) then P = P + 4.5 Else P = P + 2.5
  if ((NBClkFreq / DDR667_FREQUENCY) < 2) {
    Px2 = P * 2 + 9;
  } else {
    Px2 = P * 2 + 5;
  }

  // 9. T = T + 1050 ps
  T += 1050;

  // 10. N = (P/(MemClkFreq * 2) + T) * NclkFreq; Convert from PCLKs plus time to NCLKs.
  MemClkPeriod = 1000000 / DDR667_FREQUENCY;
  N = ((((Px2 * MemClkPeriod + 3) / 4) + T) * NBClkFreq + 999999) / 1000000;

  // 11. D18F2x210_dct[1:0]_nbp[3:0][MaxRdLatency] = CEIL(N) - 1
  N = N - 1;

  IDS_HDT_CONSOLE (MEM_FLOW, "NB Freq: %d MHz\n", NBClkFreq);
  IDS_HDT_CONSOLE (MEM_FLOW, "MaxRdLat: %03x\n", N);
  MemRecNSetBitFieldNb (NBPtr, BFMaxLatency, N);
}
