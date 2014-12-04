/* $NoKeywords:$ */
/**
 * @file
 *
 * mndctkv.c
 *
 * Northbridge DCT support for KV
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
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
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mu.h"
#include "mp.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "mnkv.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mftds.h"
#include "merrhdl.h"
#include "cpuFamRegisters.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "Filecode.h"
#include "heapManager.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_KV_MNDCTKV_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define UNUSED_CLK 4
#define MAX_RD_DQS_DLY 0x1F

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
typedef struct {
  UINT32 BitField; ///< Bit field name of the timing
  UINT8 Min;               ///< Minimum value for timing
  UINT8 Max;               ///< Maximum value for timing
} SPD_TMG;

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

extern BUILD_OPT_CFG UserOptions;
extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];
extern MEM_PSC_FLOW MemPGetODTPattern;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function disables the DCT and mem clock for KV
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDisableDctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, NBPtr, &NBPtr->MemPtr->StdHeader) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDisable DCT %d\n", NBPtr->Dct);

    // Clear BFDphyMemPsSelEn prior PHY accesses
    MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);

    //  Tristate Vref on unused channel
    MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 2);
    MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut0Sel, 0);
    MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut1Sel, 0);
    MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut2Sel, 0);
    MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut3Sel, 0);

    // Program D18F2x9C_x03FF_F041_dct[3:0]=0000_0000h. Tri-state drivers
    MemNSetBitFieldNb (NBPtr, RegTxImpedance, 0);

    // Program D18F2x9C_x00FA_F04A_dct[3:03:03:0] = 0080h.  Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
    MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

    // Program D18F2x9C_x0002_000B_dct[3:0] = 0000_0004h. Phy LP2 state
    MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);

    // Ensure that D18F2x90_dct[3:0][DisDllShutdownSR] = 1.
    // Disable powergating the ss channel when the Dcc is powergated
    MemNSetBitFieldNb (NBPtr, BFDisDllShutdownSR, 1);

    // Program D18F2x94_dct[3:0][DisDramInterface] = 1. Disable DCT
    MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);

    // Shut down the power to the SRAM array for maximum power savings.
    MemNSetBitFieldNb (NBPtr, BFPmuSramSd, 1);
  }

  NBPtr->DCTPtr->Timings.DctMemSize = 0;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function uses calculated values from DCT.Timings structure to
 *      program its registers for KV
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramCycTimingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST CTENTRY TmgAdjTab[] = {
    // BitField, Min, Max, Bias, Ratio_x2
    {BFTcl, 5, 19, 0, 2},
    {BFTrcd, 2, 19, 0, 2},
    {BFTrp, 2, 19, 0, 2},
    {BFTrtp, 4, 11, 0, 2},
    {BFTras, 8, 42, 0, 2},
    {BFTrc, 10, 58, 0, 2},
    {BFTwrDDR3, 5, 18, 0, 2},
    {BFTrrd, 4, 9, 0, 2},
    {BFTwtr, 4, 11, 0, 2},
    {BFFourActWindow, 6, 44, 0, 2}
  };

  DCT_STRUCT *DCTPtr;
  UINT8  *MiniMaxTmg;
  UINT8  *MiniMaxTrfc;
  UINT8  Value8;
  UINT8  ValFAW;
  UINT8  ValTrrd;
  UINT8  j;
  UINT8  Tcwl;
  UINT8 RdOdtTrnOnDly;
  BIT_FIELD_NAME BitField;
  MEM_PARAMETER_STRUCT *RefPtr;

  DCTPtr = NBPtr->DCTPtr;
  RefPtr = NBPtr->RefPtr;

  ValFAW = 0;
  ValTrrd = 0;

  //======================================================================
  // Program DRAM Timing values
  //======================================================================
  //
  MiniMaxTmg = &DCTPtr->Timings.CasL;
  for (j = 0; j < GET_SIZE_OF (TmgAdjTab); j++) {
    BitField = TmgAdjTab[j].BitField;

    if (BitField == BFTrp) {
      if (NBPtr->IsSupported[AdjustTrp]) {
        MiniMaxTmg[j] ++;
        if (MiniMaxTmg[j] < 5) {
          MiniMaxTmg[j] = 5;
        }
      }
    }

    if (MiniMaxTmg[j] < TmgAdjTab[j].Min) {
      MiniMaxTmg[j] = TmgAdjTab[j].Min;
    } else if (MiniMaxTmg[j] > TmgAdjTab[j].Max) {
      MiniMaxTmg[j] = TmgAdjTab[j].Max;
    }

    Value8 = (UINT8) MiniMaxTmg[j];

    if (BitField == BFTwrDDR3) {
      //
      // Override Twr on MemPstate 0 and DDR2400
      //
      if ((NBPtr->MemPstate == 0) && (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY)) {
        Value8 = 16;
      }

      if ((Value8 > 8) && ((Value8 & 1) != 0)) {
        ASSERT (FALSE);
      }
    } else if (BitField == BFTrrd) {
      ValTrrd = Value8;
    } else if (BitField == BFFourActWindow) {
      ValFAW = Value8;
    }

    MemNSetBitFieldNb (NBPtr, BitField, Value8);
  }

  MiniMaxTrfc = &DCTPtr->Timings.Trfc0;
  for (j = 0; j < 4; j++) {
    if ((NBPtr->DCTPtr->Timings.DctDimmValid & (1 << j)) != 0) {
      ASSERT (MiniMaxTrfc[j] <= 4);
      MemNSetBitFieldNb (NBPtr, BFTrfc0 + j, MiniMaxTrfc[j]);
    }
  }

  Tcwl = (UINT8) (DCTPtr->Timings.Speed / 133) + 2;
  Tcwl = (Tcwl > 5) ? Tcwl : 5;
  //
  // Override Twr on MemPstate 0 and DDR2400
  //
  if ((NBPtr->MemPstate == 0) && (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY)) {
    Tcwl = 8;
  }
  MemNSetBitFieldNb (NBPtr, BFTcwl, Tcwl);

  if (RefPtr->DramDoubleRefreshRate) {
    MemNSetBitFieldNb (NBPtr, BFTref, 3);      // 3.9 us
  } else {
    MemNSetBitFieldNb (NBPtr, BFTref, 2);      // 7.8 us
  }

  RdOdtTrnOnDly = (DCTPtr->Timings.CasL > Tcwl) ? (DCTPtr->Timings.CasL - Tcwl) : 0;
  MemNSetBitFieldNb (NBPtr, BFRdOdtTrnOnDly, RdOdtTrnOnDly);
  NBPtr->FamilySpecificHook[ProgOdtControl] (NBPtr, NULL);

  //
  // Program Tstag
  //
  // Tstag = BIOS: MAX(D18F2x204_dct[3:0]_mp[1:0][Trrd], CEIL(D18F2x204_dct[3:0]_mp[1:0][FourActWindow]/4))
  if (NBPtr->MemPstate == 0) {
    for (j = 0; j < 4; j++) {
      MemNSetBitFieldNb (NBPtr, BFTstag0 + j, MAX (ValTrrd,  (ValFAW + 3) / 4));
    }
  }

  //
  // Program Tmod
  //
  MemNSetBitFieldNb (NBPtr, BFTmod, (DCTPtr->Timings.Speed < DDR1866_FREQUENCY) ? 0x0C :
                                    (DCTPtr->Timings.Speed > DDR1866_FREQUENCY) ? 0x10 : 0x0E);
  //
  // Program Tzqcs and Tzqoper
  //
  // Tzqcs max(64nCK, 80ns)
  MemNSetBitFieldNb (NBPtr, BFTzqcs, MIN (6, (MAX (64, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 80)) + 15) / 16));
  // Tzqoper max(256nCK, 320ns)
  MemNSetBitFieldNb (NBPtr, BFTzqoper, MIN (0xC, (MAX (256, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 320)) + 31) / 32));

  // Program power management timing
  MemNDramPowerMngTimingNb (NBPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function programs the memory controller with configuration parameters
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - An Error value lower than AGESA_FATAL may have occurred
 *     @return          FALSE - An Error value greater than or equal to AGESA_FATAL may have occurred
 *     @return          NBPtr->MCTPtr->ErrCode - Contains detailed AGESA_STATUS value
 */

BOOLEAN
MemNAutoConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;
  DIE_STRUCT *MCTPtr;
  DCT_STRUCT *DCTPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  UINT32 PowerDownMode;

  RefPtr = NBPtr->RefPtr;
  MCTPtr = NBPtr->MCTPtr;
  DCTPtr = NBPtr->DCTPtr;

  //
  //======================================================================
  // NB Config 1 Lo Register Value
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFDisDramScrub, 1);

  //
  //======================================================================
  // Build Dram Config Lo Register Value
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFUnBuffDimm, 1);
  MemNSetBitFieldNb (NBPtr, BFPendRefPaybackS3En, 1);
  MemNSetBitFieldNb (NBPtr, BFStagRefEn, 1);
  MemNSetBitFieldNb (NBPtr, BFIdleCycLimit, 8);

  // DimmEccEn
  if (NBPtr->RefPtr->EnableEccFeature && MCTPtr->Status[SbEccDimms]) {
    MemNSetBitFieldNb (NBPtr, BFDimmEccEn, 1);
  }

  //
  //======================================================================
  // Build Dram Config Hi Register Value
  //======================================================================
  //
  PowerDownMode = 1;
  IDS_OPTION_HOOK (IDS_POWERDOWN_MODE, &PowerDownMode, &(NBPtr->MemPtr->StdHeader));
  MemNSetBitFieldNb (NBPtr, BFPowerDownMode, PowerDownMode);
  MemNSetBitFieldNb (NBPtr, BFMxMrsEn, 7);

  MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);
  //
  //======================================================================
  // Build Dram MRS Register Value
  //======================================================================
  //
  MemNSetBitFieldNb (NBPtr, BFPchgPDModeSel, 1);
  MemNSetBitFieldNb (NBPtr, BFBurstCtrl, 1);

  //======================================================================
  // DRAM Controller Miscellaneous 2
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFPerRankTimingEn, 0);
  MemNSetBitFieldNb (NBPtr, BFPrtlChPDEnhEn, 0);
  MemNSetBitFieldNb (NBPtr, BFPrtlChPDDynDly, 4);
  MemNSetBitFieldNb (NBPtr, BFPDPhyPSDis, 1);

  //Program D18F2x78_dct[3:0][DccPgSsBusDis]=0
  MemNSetBitFieldNb (NBPtr, BFDccPgSsBusDis, 0);

  //======================================================================
  // GMC to DCT control
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFGmcTokenLimit, 0x4);
  MemNSetBitFieldNb (NBPtr, BFMctTokenLimit, 0x4);
  MemNSetBitFieldNb (NBPtr, BFUrgentTknDis, 0x0);
  MemNSetBitFieldNb (NBPtr, BFUrGmcMinTokens, 0x4);
  MemNSetBitFieldNb (NBPtr, BFUrGmcTokenLimit, 0x4);
  MemNSetBitFieldNb (NBPtr, BFUrMctTokenLimit, 0x4);
  MemNSetBitFieldNb (NBPtr, BFUrMctMinTokens, 0x4);
  MemNSetBitFieldNb (NBPtr, BFCpuElevPrioPeriod, 0xC);

  for (i = 0; i < 4; i++) {
    MemNSetBitFieldNb (NBPtr, BFTstag0 + i, 0x14);
  }

  MemNSetBitFieldNb (NBPtr, BFFlushWrOnS3StpGnt, 1);
  MemNSetBitFieldNb (NBPtr, BFFastSelfRefEntryDis, 0);

  if (MemNGetPackageTypeKV (NBPtr) == KV_FP3) {
    MemNSetBitFieldNb (NBPtr, RegCkeToCsMap, 0x08040201);
  } else if (MemNGetPackageTypeKV (NBPtr) == KV_FM2R2) {
    MemNSetBitFieldNb (NBPtr, RegCkeToCsMap, 0x00000A05);
  } else {
    ASSERT (FALSE); // Package not supported.
  }

  MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 1);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs Mem Pstate registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       MemPstate - Mem Pstate
 *
 */

VOID
MemNProgramMemPstateRegD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  )
{

  if (MemPstate == 0) {
    MemNSetBitFieldNb (NBPtr, BFMemClkFreq, MEMCLK_FREQ_TO_ID (NBPtr->DCTPtr->Timings.Speed));
    MemNSetBitFieldNb (NBPtr, BFMemClkFreqVal, 1);
  } else {
    MemNSetBitFieldNb (NBPtr, BFM1MemClkFreq, MEMCLK_FREQ_TO_ID (DDR667_FREQUENCY));
  }

  //
  // Non-SPD Timings
  //
  MemNSetBitFieldNb (NBPtr, BFTrwtWB, 0x1C);
  MemNSetBitFieldNb (NBPtr, BFTrwtTO, 0x1B);
  MemNSetBitFieldNb (NBPtr, BFTwrrd, 0xB );

  MemNSetBitFieldNb (NBPtr, BFTrdrdBan, 0x2);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdSc, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdDc, 0xB);
  MemNSetBitFieldNb (NBPtr, BFTrdrdDd, 0xB);

  MemNSetBitFieldNb (NBPtr, BFTwrwrSdSc, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdDc, 0xB);
  MemNSetBitFieldNb (NBPtr, BFTwrwrDd, 0xB);

  MemNSetBitFieldNb (NBPtr, BFWrOdtOnDuration, DEFAULT_WR_ODT_KV);
  MemNSetBitFieldNb (NBPtr, BFRdOdtOnDuration, DEFAULT_RD_ODT_KV);
  MemNSetBitFieldNb (NBPtr, BFWrOdtTrnOnDly, DEFAULT_RD_ODT_TRNONDLY_KV);

  MemNSetBitFieldNb (NBPtr, BFTmrd, 4);
  MemNSetBitFieldNb (NBPtr, BFEffArbDis, 0);
  MemNSetBitFieldNb (NBPtr, BFAggrPDDelay, 0x20);

  if (NBPtr->RefPtr->EnablePowerDown) {
    MemNSetTxpNb (NBPtr);
    MemNSetBitFieldNb (NBPtr, BFPchgPDEnDelay, (MAX (MAX ((NBPtr->DCTPtr->Timings.CasL + 5),
                                                          (UINT8) (MemNGetBitFieldNb (NBPtr, BFTcwl) + NBPtr->DCTPtr->Timings.Twr + 5)),
                                                     (UINT8) MemNGetBitFieldNb (NBPtr, BFTmod))));
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the memory controller for training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDctForTrainingD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFGsyncDis, 0x0);
  MemNSetBitFieldNb (NBPtr, BFAddrCmdTriEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDisAutoRefresh, 0x1);
  MemNSetBitFieldNb (NBPtr, BFForceAutoPchg, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDynPageCloseEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBankSwizzleMode, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDcqBypassMax, 0x0);
  MemNSetBitFieldNb (NBPtr, BFPowerDownEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFZqcsInterval, 0x0);
  MemNSetBitFieldNb (NBPtr, BFCmdThrottleMode, 0x0);
  MemNSetBitFieldNb (NBPtr, BFODTSEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBwCapEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBankSwap, 0x0);
  MemNSetBitFieldNb (NBPtr, BFTraceModeEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDramScrub, 0);
  MemNSetBitFieldNb (NBPtr, BFScrubReDirEn, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the memory controller for mission mode
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDctNormalD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFAddrCmdTriEn, 0x1);
  MemNSetBitFieldNb (NBPtr, BFDisAutoRefresh, 0x0);
  MemNSetBitFieldNb (NBPtr, BFForceAutoPchg, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDynPageCloseEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDcqBypassMax, 0x1F);
  MemNSetBitFieldNb (NBPtr, BFZqcsInterval, 0x2);
  MemNSetBitFieldNb (NBPtr, BFTraceModeEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFMemPhyPllPdMode, 2);
  MemNSetBitFieldNb (NBPtr, BFAggrPDEn, 1);

  //
  // If DDR Frequency is 2400 then turn off the PowerDown Enable
  //
  if (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY) {
    NBPtr->RefPtr->EnablePowerDown = FALSE;
  }

  //
  // Power Down Enable
  //
  if (NBPtr->RefPtr->EnablePowerDown) {
    MemNSetBitFieldNb (NBPtr, BFPowerDownEn, 0x1);
  }

  IEM_SKIP_CODE (IEM_LATE_DCT_CONFIG) {
    MemNSetBitFieldNb (NBPtr, BFBankSwap, 0x1);
    MemNSetBitFieldNb (NBPtr, BFBankSwapAddr8En, 0x1);

    if (NBPtr->RefPtr->EnableBankSwizzle) {
      MemNSetBitFieldNb (NBPtr, BFBankSwizzleMode, 1);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs turnaround timings for each DCT
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramTurnaroundTimingsD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdSc, 1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdDc, MemNGetBitFieldNb (NBPtr, PmuCD_R_R_SD));
  MemNSetBitFieldNb (NBPtr, BFTrdrdDd, MemNGetBitFieldNb (NBPtr, PmuCD_R_R));
  MemNSetBitFieldNb (NBPtr, BFTrdrdBan, MemNGetBitFieldNb (NBPtr, PmuTrdrdban_Phy));

  MemNSetBitFieldNb (NBPtr, BFTwrwrSdSc, 1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdDc, MemNGetBitFieldNb (NBPtr, PmuCD_W_W_SD));
  MemNSetBitFieldNb (NBPtr, BFTwrwrDd, MemNGetBitFieldNb (NBPtr, PmuCD_W_W));

  MemNSetBitFieldNb (NBPtr, BFTwrrd, MemNGetBitFieldNb (NBPtr, PmuCD_W_R));
  MemNSetBitFieldNb (NBPtr, BFTrwtTO, MemNGetBitFieldNb (NBPtr, PmuCD_R_W));
  MemNSetBitFieldNb (NBPtr, BFTrwtWB, MemNGetBitFieldNb (NBPtr, BFTrwtTO) + 1);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function looks up platform specific config/timing values from the interface layer and
 *   programs them into DCT.
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE  - No errors
 *     @return          FALSE - Found errors on a channel and disable it
 */

BOOLEAN
MemNPlatformSpecKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  BOOLEAN Result;
  LOCATE_HEAP_PTR LocHeap;
  DRAM_CAD_BUS_CONFIG_KV *DramCadBusConfigPtr;

  LocHeap.BufferHandle = AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    return FALSE;
  }

  DramCadBusConfigPtr = (DRAM_CAD_BUS_CONFIG_KV *) LocHeap.BufferPtr;
  Result = FALSE;

  // Extract recommended settings from tables
  MemPPreparePsTabLookupConditions (NBPtr);
  if (MemPLookupCadBusCfgTabs (NBPtr, (MemNGetPsTablesKV (NBPtr))->TblEntryOfSAO)) {
    if (MemPLookupDataBusCfgTabs (NBPtr, (MemNGetPsTablesKV (NBPtr))->TblEntryOfDramTerm)) {
      if (MemPGetODTPattern (NBPtr, MemNGetPsTablesKV (NBPtr))) {
        Result = TRUE;
      }
    }
  }

  // Disable channel if there are errors
  IDS_SKIP_HOOK (IDS_ENFORCE_PLAT_TABLES, NBPtr, &(NBPtr->MemPtr->StdHeader)) {
    if (!Result) {
      IDS_HDT_CONSOLE (MEM_FLOW, "\tDisable DCT%d due to unsupported DIMM configuration\n", NBPtr->Dct);
      if (!NBPtr->MemPtr->ErrorHandling (NBPtr->MCTPtr, NBPtr->Dct, EXCLUDE_ALL_CHIPSEL, &NBPtr->MemPtr->StdHeader)) {
        ASSERT (FALSE);
      }
      MemNDisableDctKV (NBPtr);
    }
  }

  if (Result) {
    // Program settings
    MemNSetBitFieldNb (NBPtr, BFSlowAccessMode, (NBPtr->ChannelPtr->SlowMode) ? 1 : 0);
    MemNSetBitFieldNb (NBPtr, RegPhyRODTCSLow, NBPtr->ChannelPtr->PhyRODTCSLow);
    MemNSetBitFieldNb (NBPtr, RegPhyRODTCSHigh, NBPtr->ChannelPtr->PhyRODTCSHigh);
    MemNSetBitFieldNb (NBPtr, RegPhyWODTCSLow, NBPtr->ChannelPtr->PhyWODTCSLow);
    MemNSetBitFieldNb (NBPtr, RegPhyWODTCSHigh, NBPtr->ChannelPtr->PhyWODTCSHigh);

    // Save DRAM CAD Bus Configuration info for later use in constructing the
    // PMU SRAM Message Block
    DramCadBusConfigPtr->SlowMode[NBPtr->MemPstate] = NBPtr->ChannelPtr->SlowMode;
    DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] = NBPtr->ChannelPtr->DctAddrTmg;

    // Override settings as requested
    MemProcessConditionalOverrides (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr, PSO_ACTION_ODT, ALL_DIMMS);
  }

  return Result;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function retrieves the Max latency parameters
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @param[in]  *MinDlyPtr - Pointer to variable to store the Minimum Delay value
 *     @param[in]  *MaxDlyPtr - Pointer to variable to store the Maximum Delay value
 *     @param[in]  *DlyBiasPtr - Pointer to variable to store Delay Bias value
 *     @param[in]  MaxRcvEnDly - Maximum receiver enable delay value
 */

VOID
MemNGetMaxLatParamsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly,
  IN OUT   UINT16 *MinDlyPtr,
  IN OUT   UINT16 *MaxDlyPtr,
  IN OUT   UINT16 *DlyBiasPtr
  )
{
  UINT32 N;
  UINT32 T;
  UINT32 P;
  UINT32 MemClkPeriod;

  // 1. P = N = T = 0.
  P = N = T = 0;

  // Calculate a starting MaxRdLatency delay value by converting Tcl to NCLKs
  P = 2 * NBPtr->DCTPtr->Timings.CasL;

  // 10. N = (P/(MemClkFreq * 2) + T) * NclkFreq; Convert from PCLKs plus time to NCLKs.
  MemClkPeriod = 1000000 / ((NBPtr->MemPstate == 0) ? NBPtr->DCTPtr->Timings.Speed : DDR667_FREQUENCY);
  N = ((((P * MemClkPeriod + 1) / 2) + T) * NBPtr->NBClkFreq + 999999) / 1000000;

  // Calculate a starting MaxRdLatency delay value with steps 5, 9, and 12 excluded
  *MinDlyPtr = (UINT16) N;

  *MaxDlyPtr = 0x3FF;

  // Left edge of MaxRdLat will be added as follow
  // Nbp0: 1 NCLK + 3 MEMCLKs
  // Nbp[3:1]: 3 NCLK + 3 MEMCLKs
  if (NBPtr->NbFreqChgState == 0) {
    N = 1;
  } else {
    N = 3;
  }

  P = 6;
  N += (((P * MemClkPeriod + 1) / 2) * NBPtr->NBClkFreq + 999999) / 1000000;
  *DlyBiasPtr = (UINT16) N;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     The function resets Rcv Fifo
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       Dummy - Dummy parameter
 *
 */

VOID
MemNResetRcvFifoKV (
  IN OUT   struct _MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Dummy
  )
{
  // Kaveri does not need to reset Rcv FIFO.
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function modifies CS interleaving low address according to several conditions for KV.
 *
 *     @param[in,out]  *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  *LowBit   - Pointer to low bit
 *
 */

BOOLEAN
MemNCSIntLvLowAddrAdjKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *LowBit
  )
{
  UINT8 DctSelIntLvAddr;
  UINT8 BankSwapAddr8En;

  DctSelIntLvAddr = (UINT8) MemNGetBitFieldNb (NBPtr, BFDctSelIntLvAddr);
  BankSwapAddr8En =   (UINT8) MemNGetBitFieldNb (NBPtr, BFBankSwapAddr8En);

  //
  //D18F2x[5C:40]_dct[1:0][15:5] = BaseAddr[21:11] &&
  //D18F2x[6C:60]_dct[1:0][15:5] = AddrMask[21:11], so *LowBit needs to be added with 2.
  //
  *(UINT8 *) LowBit += 2;

  if (MemNGetBitFieldNb (NBPtr, BFBankSwap) == 1) {
    if ((BankSwapAddr8En == 1) && (DctSelIntLvAddr == 4)) {
      *(UINT8 *) LowBit = 5;
    } else {
      *(UINT8 *) LowBit = 6;
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function caps speed based on battery life check.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 */
VOID
MemNCapSpeedBatteryLifeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT16 SupportedFreq[] = {
    DDR2400_FREQUENCY,
    DDR2133_FREQUENCY,
    DDR1866_FREQUENCY,
    DDR1600_FREQUENCY,
    DDR1333_FREQUENCY,
    DDR1066_FREQUENCY,
    DDR800_FREQUENCY,
    DDR667_FREQUENCY
  };

  UINT32 FreqNumeratorInMHz;
  UINT32 FreqDivisor;
  UINT32 VoltageInuV;
  UINT32 NBFreq;
  UINT16 DdrFreq;
  UINT16 j;
  UINT8 Dct;
  INT8   NbPs;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  FamilySpecificServices = NULL;
  GetCpuServicesOfSocket (NBPtr->MCTPtr->SocketId, &FamilySpecificServices, &(NBPtr->MemPtr->StdHeader));


  // Find the lowest supported NB Pstate
  NBFreq = 0;
  for (NbPs = 3; NbPs >= 0; NbPs--) {
    if (FamilySpecificServices->GetNbPstateInfo (FamilySpecificServices,
                                                  NBPtr->MemPtr->PlatFormConfig,
                                                  &NBPtr->PciAddr,
                                                  (UINT32) NbPs,
                                                  &FreqNumeratorInMHz,
                                                  &FreqDivisor,
                                                  &VoltageInuV,
                                                  &(NBPtr->MemPtr->StdHeader))) {
      if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) == 0) {
        NBFreq = FreqNumeratorInMHz / FreqDivisor;
        break;
      }
    }
  }

  ASSERT (NBFreq > 0);

  // Pick Max MEMCLK that is less than or equal to NCLK
  DdrFreq = DDR800_FREQUENCY;
  for (j = 0; j < GET_SIZE_OF (SupportedFreq); j++) {
    if (NBFreq >= SupportedFreq[j]) {
      DdrFreq = SupportedFreq[j];
      break;
    }
  }

  // Cap MemClk frequency to lowest NCLK frequency
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.TargetSpeed > DdrFreq) {
      NBPtr->DCTPtr->Timings.TargetSpeed = DdrFreq;
    }
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Mode Register initialization.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNModeRegisterInitializationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 Mr0;
  UINT32 Mr1;
  UINT32 Mr2;

  Mr0 = MemNCalcMR0 (NBPtr);
  Mr1 = MemNCalcMR1 (NBPtr, 0);
  Mr2 = MemNCalcMR2 (NBPtr, 0);

  MemNSetBitFieldNb (NBPtr, BFMxMr0, Mr0);
  MemNSetBitFieldNb (NBPtr, BFMxMr1, Mr1);
  MemNSetBitFieldNb (NBPtr, BFMxMr2, Mr2);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Configure disable DLL shutdown in self-refresh mode.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDisDllShutdownSrKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNBrdcstSetUnConditionalNb (NBPtr, BFDisDllShutdownSR, 0);
}
