/* $NoKeywords:$ */
/**
 * @file
 *
 * mnphykv.c
 *
 * Northbridge Phy support for KV
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
 * @e \$Revision: 282148 $ @e \$Date: 2014-01-07 10:57:44 -0600 (Tue, 07 Jan 2014) $
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
#include "mnreg.h"
#include "OptionMemory.h"       // need def for MEM_FEAT_BLOCK_NB
#include "mnkv.h"
#include "PlatformMemoryConfiguration.h"
#include "merrhdl.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_KV_MNPHYKV_FILECODE
/* features */
#include "mftds.h"
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

/// The structure of one entry of a TxPrePN table
typedef struct {
  UINT16 MaxSpeed;      ///< Highest memory speed for this setting
  UINT8  DrvStrenP;     ///< Driver strength
  UINT8  TxPrePNVal[3]; ///< Tx predriver value per voltage
} TXPREPN_ENTRY;

/// The structure describes how to extract data and program TxPrePN
typedef struct {
  BIT_FIELD_NAME BfName;   ///< Bit field to set
  TXPREPN_ENTRY  *TabPtr;  ///< Pointer to TxPrePN table
  UINT16         PsOffset; ///< Offset to find DrvStrenP from PS struct
} PROG_TXPREPN_STRUCT;

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

BOOLEAN
STATIC
MemNSlowOpnModeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;
/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function programs DDR mode for each Phy
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Mode     - DDR3/GDDR5
 *
 */

VOID
MemNSetPhyDdrModeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  )
{
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tSet MajorMode=%d\n", Mode);
  // 3. According to the type of DRAM attached, program D18F2x9C_x00FFF04A_dct[3:0][MajorMode],
  //    D18F2x9C_x0002_000E_dct[3:0][G5_Mode], and D18F2x9C_x0002_0098_dct[3:0][CalG5D3].
  MemNSetBitFieldNb (NBPtr, RegRxCtl1, Mode << 4);
  // Mem P-state 0, normal D3 data/dqs receiver
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, RegDctPhy0x01F1F043), 0x0);
  // Mem P-state 1, low-power D3 data/dqs receiver
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDctPhy0x01F1F043), 0x4);
  MemNSetBitFieldNb (NBPtr, BFG5Mode, Mode);
  MemNSetBitFieldNb (NBPtr, BFG5ModeAbyte, Mode);
  MemNSetBitFieldNb (NBPtr, BFG5ModeDbyte, Mode);
  MemNSetBitFieldNb (NBPtr, BFG5ModeAbit, Mode);
  MemNSetBitFieldNb (NBPtr, BFCalG5D3, Mode);
  if (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY) {
    MemNSetBitFieldNb (NBPtr, RegDqDqsRcvCntrl1, 9);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs general Phy configuration registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPhyGenCfgKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT64 RODT;
  UINT64 WODT;
  UINT8  Cs;
  INT8   MemPstate;
  INT8   LowestMemPstate;

  LowestMemPstate = 1;

  // Program D18F2x9C_x01F1_F045_dct[3:0]=0040h.
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDctPhy0x01F1F045), 0x40);

  // Program D18F2x9C_x00F4_00E[7:0]_dct[3:0].
  RODT = ((UINT64) NBPtr->ChannelPtr->PhyRODTCSHigh << 32) | NBPtr->ChannelPtr->PhyRODTCSLow;
  WODT = ((UINT64) NBPtr->ChannelPtr->PhyWODTCSHigh << 32) | NBPtr->ChannelPtr->PhyWODTCSLow;
  for (Cs = 0; Cs < MAX_CS_PER_CHANNEL; Cs++) {
    MemNSetBitFieldNb (NBPtr, RegAcsmOdtCtrl0 + Cs, (UINT32) ((((RODT >> (Cs * 8)) & 0xF) << 4) | ((WODT >> (Cs * 8)) & 0xF)));
  }

  // Program D18F2x9C_x00F0_0015_dct[3:0][VrefFilt], D18F2x9C_x00F1_0015_dct[3:0][VrefFilt], D18F2x9C_x00F2_0015_dct[3:0][VrefFilt].
  MemNSetBitFieldNb (NBPtr, RegVrefByteMaster, 0);
  MemNSetBitFieldNb (NBPtr, RegVrefByteAbyte, 0);
  MemNSetBitFieldNb (NBPtr, RegVrefByteDbyte, 0);

  // Program D18F2x9C_x00[F,8:0]1_0016_dct[3:0].
  MemNSetBitFieldNb (NBPtr, RegProcOdtTmg, 0x1244);

  // Program TxControlDq[EQEnable] = 0 and  TxControlDq[DrvPwrGateEn] = 1
  MemNSetBitFieldNb (NBPtr, RegDctPhy0x00F0F04E, 0x0800);

  // Programs the following for maximum power savings prior to training:
  MemNSetBitFieldNb (NBPtr, RegTxControlDq, 0x0B00);
  MemNSetBitFieldNb (NBPtr, RegCadByteDbgCtrl, 0x0022);
  MemNSetBitFieldNb (NBPtr, RegDataByteDbgCtrl, 0x0033);
  MemNSetBitFieldNb (NBPtr, RegMasterByteDbgCtrl, 0x0001);
  MemNSetBitFieldNb (NBPtr, RegMasterD3EvntMerr, 0x0001);
  MemNSetBitFieldNb (NBPtr, RegDataDqDqsRcvCntrl3, 0x0152);

  for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
    // When memory pstate is enabled, this loop will goes through M1 first then M0
    // Otherwise, this loop only goes through M0.
    MemNSwitchMemPstateKV (NBPtr, MemPstate);
    // Program static value for Abyte DllPumpPeriod
    MemNSetBitFieldNb (NBPtr, BFCadDllPumpPeriod, 3);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the phy registers according to the desired phy VDDIO voltage level
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPhyVoltageLevelKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Voltage;

  Voltage = CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDR3Voltage);

  MemNSetBitFieldNb (NBPtr, BFCalCmptrResTrim, Voltage);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs DRAM channel frequency
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Mode     - DDR3/GDDR5
 *
 */

VOID
MemNProgramChannelFreqKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  )
{
  UINT16 Freq;
  UINT16 PllMultDiv;
  UINT16 PMUClkDiv;

  // 1. Program D18F2x9C_x0002_0093_dct[3:0][PllRegWaitTime] = 4Bh.
  MemNSetBitFieldNb (NBPtr, RegPllRegWaitTime, 0x4B);

  // 2. Program D18F2x9C_x0002_0089_dct[3:0][PllLockTime] = 190h. Restore the default PLL lock time.
  MemNSetBitFieldNb (NBPtr, RegPllLockTime, 0x190);

  // 3. Program D18F2x9C_x0002_0000_dct[3:0][PllMultDiv]. PMU Reset must be high.
  if (Mode == DRAM_TYPE_GDDR5_KV) {
    Freq = NBPtr->DCTPtr->Timings.TargetSpeed * 4;
  } else {
    Freq = NBPtr->DCTPtr->Timings.TargetSpeed * 2;
  }
  PllMultDiv = ((Freq / 100) * 0x100) + ((Freq % 100) / 20);

  // 4. Program D18F2x9C_x0002_0080_dct[3:0][PMUClkDiv].
  // PMU must operate no faster than 533MHz
  PMUClkDiv = CEIL_DIV (((CEIL_DIV (Freq, 533) < 1) ? 0 : CEIL_DIV (Freq, 533)), 2);

  MemNSetBitFieldNb (NBPtr, RegPllMemPs0, PllMultDiv);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv),  PMUClkDiv);

  // 5. IF (Ddr3Mode) program D18F2x9C_x0002_0001_dct[3:0][PllMultDiv] = '667 MT/s'.
  // 6. IF (Ddr3Mode) program D18F2x9C_x0102_0080_dct[3:0][PMUClkDiv].
  if (Mode == DRAM_TYPE_DDR3_KV) {
    MemNSetBitFieldNb (NBPtr, RegPllMemPs1, 0x0603);
    MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFPMUClkDiv), 1);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function does Phy Predriver initialization
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPredriverInitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST STATIC TXPREPN_ENTRY TxPrePNDqDqs[] = {
    {DDR1066_FREQUENCY, 0x31, {0x2F, 0x2F, 0x2F}},
    {DDR1066_FREQUENCY, 0x70, {0x12, 0x2F, 0x2F}},
    {DDR1066_FREQUENCY, 0x75, {0x09, 0x12, 0x12}},
    {DDR1066_FREQUENCY, 0x7F, {0x09, 0x09, 0x09}},
    {DDR1600_FREQUENCY, 0x31, {0x2F, 0x2F, 0x2F}},
    {DDR1600_FREQUENCY, 0x70, {0x12, 0x2F, 0x2F}},
    {DDR1600_FREQUENCY, 0x75, {0x12, 0x12, 0x1B}},
    {DDR1600_FREQUENCY, 0x7F, {0x12, 0x12, 0x1B}},
    {DDR2400_FREQUENCY, 0x31, {0x2F, 0x2F, 0x2F}},
    {DDR2400_FREQUENCY, 0x70, {0x1B, 0x2F, 0x2F}},
    {DDR2400_FREQUENCY, 0x75, {0x1B, 0x24, 0x24}},
    {DDR2400_FREQUENCY, 0x7F, {0x1B, 0x24, 0x25}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNCmdAddr[] = {
    {DDR1066_FREQUENCY, 0x07, {0x00, 0x09, 0x09}},
    {DDR1066_FREQUENCY, 0x0F, {0x00, 0x09, 0x09}},
    {DDR1066_FREQUENCY, 0x1F, {0x00, 0x09, 0x09}},
    {DDR1066_FREQUENCY, 0x3F, {0x00, 0x09, 0x09}},
    {DDR1600_FREQUENCY, 0x07, {0x09, 0x12, 0x1B}},
    {DDR1600_FREQUENCY, 0x0F, {0x09, 0x12, 0x1B}},
    {DDR1600_FREQUENCY, 0x1F, {0x09, 0x12, 0x1B}},
    {DDR1600_FREQUENCY, 0x3F, {0x09, 0x12, 0x1B}},
    {DDR2400_FREQUENCY, 0x07, {0x1B, 0x1B, 0x24}},
    {DDR2400_FREQUENCY, 0x0F, {0x1B, 0x1B, 0x24}},
    {DDR2400_FREQUENCY, 0x1F, {0x1B, 0x1B, 0x24}},
    {DDR2400_FREQUENCY, 0x3F, {0x1B, 0x1B, 0x24}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNClk[] = {
    {DDR1066_FREQUENCY, 0x07, {0x12, 0x1B, 0x24}},
    {DDR1066_FREQUENCY, 0x0F, {0x12, 0x1B, 0x24}},
    {DDR1066_FREQUENCY, 0x1F, {0x12, 0x1B, 0x24}},
    {DDR1066_FREQUENCY, 0x3F, {0x12, 0x1B, 0x24}},
    {DDR1600_FREQUENCY, 0x07, {0x24, 0x2D, 0x36}},
    {DDR1600_FREQUENCY, 0x0F, {0x24, 0x2D, 0x36}},
    {DDR1600_FREQUENCY, 0x1F, {0x24, 0x2D, 0x36}},
    {DDR1600_FREQUENCY, 0x3F, {0x24, 0x2D, 0x36}},
    {DDR2400_FREQUENCY, 0x07, {0x3F, 0x3F, 0x3F}},
    {DDR2400_FREQUENCY, 0x0F, {0x3F, 0x3F, 0x3F}},
    {DDR2400_FREQUENCY, 0x1F, {0x3F, 0x3F, 0x3F}},
    {DDR2400_FREQUENCY, 0x3F, {0x3F, 0x3F, 0x3F}},
  };

  CONST STATIC PROG_TXPREPN_STRUCT ProgTxPrePN[] = {
    {PER_CAD_PAD (PAD_CKE, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CkeStrength)},
    {PER_CAD_PAD (PAD_CS, ALL_PADS, RegCadTxSlewRate),       (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CsOdtStrength)},
    {PER_CAD_PAD (PAD_ODT, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CsOdtStrength)},
    {PER_CAD_PAD (PAD_ADDR_CMD, ALL_PADS, RegCadTxSlewRate), (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, AddrCmdStrength)},
    {PER_CAD_PAD (PAD_CLK, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNClk,     OFFSET_OF (MEM_PS_BLOCK, ClkStrength)},
    {PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataTxSlewRate),   (TXPREPN_ENTRY *)&TxPrePNDqDqs,   OFFSET_OF (MEM_PS_BLOCK, DqStrength)},
    {PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataTxSlewRate),  (TXPREPN_ENTRY *)&TxPrePNDqDqs,   OFFSET_OF (MEM_PS_BLOCK, DqsStrength)},
  };

  TXPREPN_ENTRY *TabPtr;
  UINT8 TxSlewRate;
  UINT8 *DrvStrenPtr;
  UINT8 i;
  UINT8 j;

  // Expect all TxPre tables have the same number of entries
  ASSERT ((GET_SIZE_OF (TxPrePNDqDqs) == GET_SIZE_OF (TxPrePNCmdAddr)) && (GET_SIZE_OF (TxPrePNCmdAddr) == GET_SIZE_OF (TxPrePNClk)));

  if (NBPtr->MemPstate == 0) {
    // Program D18F2x9C_x0002_0087_dct[3:0][DisAutoComp, DisPredriverCal] = {1,1}.
    MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
    MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);

    TxSlewRate = 0;
    for (i = 0; i < GET_SIZE_OF (ProgTxPrePN); i++) {
      DrvStrenPtr = ((UINT8 *) NBPtr->PsPtr) + ProgTxPrePN[i].PsOffset;
      TabPtr = ProgTxPrePN[i].TabPtr;
      for (j = 0; j < GET_SIZE_OF (TxPrePNDqDqs); j++) {
        if (NBPtr->DCTPtr->Timings.Speed <= TabPtr[j].MaxSpeed) {
          if (*DrvStrenPtr <= TabPtr[j].DrvStrenP) {
            TxSlewRate = TabPtr[j].TxPrePNVal[CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDR3Voltage)];
            break;
          }
        }
      }
      ASSERT (j < GET_SIZE_OF (TxPrePNDqDqs));

      MemNSetBitFieldNb (NBPtr, ProgTxPrePN[i].BfName, TxSlewRate);
    }

    // Program D18F2x9C_x0002_0087_dct[3:0][DisAutoComp] = 0.
    // DisAutoComp will be cleared after setting up FIFO pointers
  }
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the phy registers to configure CAD bus and data bus
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramCadDataBusD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 RxRdPtrOffset;
  UINT16 TxRdPtrOffset;

  //------------------------------------------------
  // CAD bus configuration
  //------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCKE\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CKE, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CkeStrength << 6) | NBPtr->PsPtr->CkeStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCS\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CS, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tODT\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ODT, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tAddrCmd\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->AddrCmdStrength << 6) | NBPtr->PsPtr->AddrCmdStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCLK\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CLK, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->ClkStrength << 6) | NBPtr->PsPtr->ClkStrength);

  //------------------------------------------------
  // Data bus configuration
  //------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQ\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqStrength << 7) | NBPtr->PsPtr->DqStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQS\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqsStrength << 7) | NBPtr->PsPtr->DqsStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tProc ODT\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), (NBPtr->PsPtr->OdtStrength << 4) | NBPtr->PsPtr->OdtStrength);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), (NBPtr->PsPtr->OdtStrength << 4) | NBPtr->PsPtr->OdtStrength);

  //------------------------------------------------
  // FIFO pointer init
  //------------------------------------------------
  // 3. D18F2x9C_x0[3,1:0][F,7:0]1_0028_dct[3:0][RxRdPtrOffset] as follows:
  //    Broadcast the value to all chips.
  //    RxRdPtrOffset = MIN(18, CASL).
  RxRdPtrOffset = MIN (18, NBPtr->DCTPtr->Timings.CasL);
  // 4. D18F2x9C_x0[3,1:0][F,7:0]1_0028_dct[3:0][TxRdPtrOffset] as follows:
  //    Broadcast the value to all chips.
  //    TxRdPtrOffset = MAX(5, CWL).
  TxRdPtrOffset = MAX (5, (NBPtr->DCTPtr->Timings.Speed / 133) + 2);
  MemNSetBitFieldNb (NBPtr, RegDataRdPtrOffset, (TxRdPtrOffset << 6) | RxRdPtrOffset);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function initializes FIFO configurations
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNPhyFifoConfigD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 Speed;
  UINT8  RdPtrInit;
  UINT8  NbPs;
  UINT32 NbClkFreqMhz;
  UINT8  MinRdPtrInit;
  UINT8  CpuRevisionKv;
  CONST UINT8 CPU_REVISION_KV_A0 = 0x30;

  RdPtrInit = 0;
  MinRdPtrInit = 0;
  CpuRevisionKv = (UINT8) MemNGetBitFieldNb (NBPtr, BFBaseModel);

  // 1. For NbP0 broadcast the value to all timing groups and chips as follows:
  //    D18F2x9C_x00[F,3:0]0_[F,3:0][8,3:0]2E_dct[3:0][RdPtrInitVal]= Table 42.
  //    D18F2x9C_x00[F,8:0]1_0[8,3:0]2E_dct[3:0][RdPtrInitVal] = Table 42.
  IDS_HDT_CONSOLE (MEM_FLOW, "NBP0 - M0\n");
  switch (NBPtr->DCTPtr->Timings.TargetSpeed) {
  case DDR667_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 800) ? 0x10 : 0x14;
    break;
  case DDR800_FREQUENCY:
  case DDR1066_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 800) ? 0xC : 0x10;
    break;
  case DDR1333_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 800) ? 0x8 : (NBPtr->NBClkFreq <= 1400) ? 0x0C: 0x10;
    break;
  case DDR1600_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 1200) ? 0x8 : 0xC;
    break;
  case DDR1866_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 1000) ? 0x4 : (NBPtr->NBClkFreq <= 1600) ? 0x8: 0xC;
    break;
  case DDR2133_FREQUENCY:
    RdPtrInit = (NBPtr->NBClkFreq <= 1400) ? 0x4 : 0x8;
    break;
  case DDR2400_FREQUENCY:
    break;
  default:
    ASSERT (FALSE);  // MemClk not supported.
  }

  MemNSetBitFieldNb (NBPtr, PER_NB_PS (0, RegRdPtrInitVal), RdPtrInit);
  MemNSetBitFieldNb (NBPtr, PER_NB_PS (0, RegDataRdPtrInitVal), RdPtrInit);

  // 2. For each NbPstate, including the "NbPstate PMU" instance, excluding NbP0, program
  //    Broadcast the value to all timing groups and chips.
  //    D18F2x9C_x00[F,3:0]0_[F,3:0][8,3:0]2E_dct[3:0][RdPtrInitVal]= Table 43.
  //    D18F2x9C_x00[F,8:0]1_0[8,3:0]2E_dct[3:0][RdPtrInitVal] = Table 43.
  for (NbPs = 1; NbPs < 4; NbPs++) {
    NbClkFreqMhz = MemNGetNbClkFreqByPstateUnb (NBPtr, NbPs);
    if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) == 0) {
      Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      IDS_HDT_CONSOLE (MEM_FLOW, "NBP%d - M0\n", NbPs);
    } else {
      Speed = DDR667_FREQUENCY;
      IDS_HDT_CONSOLE (MEM_FLOW, "NBP%d - M1\n", NbPs);
    }

    switch (Speed) {
    case DDR667_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 1200) ? 0x10 : 0x14;
      break;
    case DDR800_FREQUENCY:
    case DDR1066_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 1400) ? 0xC : 0x10;
      break;
    case DDR1333_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 1200) ? 0x8 : 0xC;
      break;
    case DDR1600_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 800) ? 0x8 : (NbClkFreqMhz <= 1000) ? 0x4: 0x8;
      break;
    case DDR1866_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 1000) ? 0x0 : (NbClkFreqMhz <= 1600) ? 0x4: 0x8;
      break;
    case DDR2133_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz <= 1400) ? 0x0 : 0x4;
      break;
    case DDR2400_FREQUENCY:
      break;
    default:
      ASSERT (FALSE);  // MemClk not supported.
    }

    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegRdPtrInitVal), RdPtrInit);
    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegDataRdPtrInitVal), RdPtrInit);
  }

  //Silicon Work Around for processor revision A0/A1: DCTs bug.
  if (CpuRevisionKv == CPU_REVISION_KV_A0) {
    for (NbPs = 0; NbPs < 4; NbPs++) {
      NbClkFreqMhz = MemNGetNbClkFreqByPstateUnb (NBPtr, NbPs);
      if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) == 0) {
        Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
        IDS_HDT_CONSOLE (MEM_FLOW, "NBP%d - M0\n", NbPs);
      } else {
        Speed = DDR667_FREQUENCY;
        IDS_HDT_CONSOLE (MEM_FLOW, "NBP%d - M1\n", NbPs);
      }

      switch (Speed) {
      case DDR667_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 1200) ? 0x10 : 0x14;
        break;
      case DDR800_FREQUENCY:
      case DDR1066_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 1400) ? 0xC : 0x10;
        break;
      case DDR1333_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 1200) ? 0x8 : 0xC;
        break;
      case DDR1600_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 800) ? 0x8 : (NbClkFreqMhz <= 1000) ? 0x4: 0x8;
        break;
      case DDR1866_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 1000) ? 0x0 : (NbClkFreqMhz <= 1600) ? 0x4: 0x8;
        break;
      case DDR2133_FREQUENCY:
        RdPtrInit = (NbClkFreqMhz <= 1400) ? 0x0 : 0x4;
        break;
      case DDR2400_FREQUENCY:
        break;
      default:
        ASSERT (FALSE);  // MemClk not supported.
      }

      if (RdPtrInit < MinRdPtrInit) {
        MinRdPtrInit = RdPtrInit;
      }
    }

    // Optimize MaxRdLatency timing
    if (MemNSlowOpnModeKV (NBPtr)) {
      MinRdPtrInit = 16;
    } else {
      MinRdPtrInit = 20;
    }

    if (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY) {
      MinRdPtrInit = 16;
    }

    IDS_HDT_CONSOLE (MEM_FLOW, "RdPtrInit = %x\n", MinRdPtrInit);
    for (NbPs = 0; NbPs < 4; NbPs++) {
      MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegRdPtrInitVal), MinRdPtrInit);
      MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegDataRdPtrInitVal), MinRdPtrInit);
    }
  }

  MemNSetBitFieldNb (NBPtr, PER_NB_PS (NB_PS_PMU, RegRdPtrInitVal), 2);
  MemNSetBitFieldNb (NBPtr, PER_NB_PS (NB_PS_PMU, RegDataRdPtrInitVal), 2);

  // Clear DisAutoComp to complete predriver programming
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    DRAM Phy Power Savings, tri-state unused pads, and DLLs.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDramPhyPowerSavingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT8 DLL_RESET_RELOCK_SHIFT = 7;
  UINT16 Speed;

  if (NBPtr->MemPstate == 0) {
    Speed = NBPtr->DCTPtr->Timings.Speed;
  } else {
    Speed = DDR667_FREQUENCY;
  }

  // 1. Program DctPhyA41 as follows to disable unused pads.
  // DrvStrenP = DrvStrenP = 0.
  // See 2.10.4. Software does this for each unconnected pad in the package or each pad
  // connected to unused pins
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tTri-state unused pads\n");
  MemNTriStateUnusedPadKV (NBPtr);

  // 2. Program D18F2x9C_x0[3,1:0][F,3:0]0_0014_dct[3:0][MaxDurDllNoLock] = 0.
  // DctPhyA78[DllResetRelock] must be set to 1 prior to writing this register
  // and then DctPhyA78[DllResetRelock] must be cleared after the register
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCAD DLL standby\n");
  MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
  MemNSetBitFieldNb (NBPtr, BFCadMaxDurDllNoLock, 0);
  MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, 0);

  // 3. Program D18F2x9C_x0[3,1:0][F,3:0]1_0014_dct[3:0][MaxDurDllNoLock] as follows:
  // If (DDR rate <= 1066) MaxDurDllNoLock = 9h
  // else MaxDurDllNoLock = 7h
  // DctPhyD78[DllResetRelock] must be set to 1 prior to writing this register
  // and then DctPhyD78[DllResetRelock] must be cleared after the register
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tData DLL standby\n");
  MemNSetBitFieldNb (NBPtr, RegDctPhyD78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
  if (Speed <= DDR1066_FREQUENCY) {
    MemNSetBitFieldNb (NBPtr, BFDataMaxDurDllNoLock, 9);
  } else {
    MemNSetBitFieldNb (NBPtr, BFDataMaxDurDllNoLock, 7);
  }
  MemNSetBitFieldNb (NBPtr, RegDctPhyD78DllControl, 0);

  // Program static value for Dbyte DllPumpPeriod
  MemNSetBitFieldNb (NBPtr, BFDataDllPumpPeriod, 3);

  if (NBPtr->MemPstate == 1) {
    // 4. Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[3:0] as follows:
    // For M1 context program ODTStrenN = ODTStrenP = 0
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), 0);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), 0);

    // 5. Program D18F2x9C_x01F1_F043_dct[3:0] = 0004h.
    // bc to all ccccmmmm in M1. Rcvrs in low power mode
    MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDataRcvMajorMode), 0x0004);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Additional DRAM Phy Power Savings
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNAddlDramPhyPowerSavingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // 6. Program D18F2x9C_x00FA_F04A_dct[3:0] = 0080h.
  // Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
  MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

  // 7. Power down unused DBYTE 9 (undocumented spare) as follows:
  // Program D18F2x9C_x0091_F04A_dct[] = 0280h
  // Program D18F2x9C_x0091_0F77_dct[] = 07CFh. Turn off all DLLs
  // Program D18F2x9C_x0091_0000_dct[] = 00004. Turn Dbyte off
  // Program D18F2x9C_x0091_0F77_dct[] = 07EFh. Turn off XCLK
  // Program D18F2x9C_x0391_F04D_dct[] = 0000h. Disable Odt
  // Program D18F2x9C_x0391_F041_dct[] = 0000h. Disable output driver impedance
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0091F04A), 0x280);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910F77), 0x07CF);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910000), 0x0004);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910F77), 0x07EF);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0391F04D), 0x0000);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0391F041), 0x0000);

  // 8. If ECC memory is not connected or is disabled, or the package does not support ECC,
  // then power down data chiplet 8 with the following:
  // Write to all memory P-state instances if available.
  // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04A_dct[3:0] = 0280h.
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[3:0] = 07CFh.  Turn off all DLLs
  // Program D18F2x9C_x00[F,8:0]1_0000_dct[3:0] = 00004.  Turn Dbyte off
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[3:0] = 07EFh.  Turn off XCLK
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[3:0] = 0000h.  Disable Odt
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]041_dct[3:0] = 0000h.  Disable output driver impedance
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0091F04A), 0x280);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07CF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910000), 0x0004);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07EF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F04D), 0x0000);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F041), 0x0000);
  }

  // 9. Power down unused receivers in data chips as follows:
  // Program D18F2x9C_x00F1_804A_dct[] = 280h.  EDC pad.
  // Program D18F2x9C_x00F1_904A_dct[] = 280h.  DBI pad.
  // If x4 DIMMs are not present then program D18F2x9C_x00F1_B04A_dct[] = 280h.  MEMDQSU_H and MEMDQSU_L pads
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1804A), 0x0280);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1904A), 0x0280);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1B04A), 0x0280);

  // 10. Power down the PMU
  MemNDisablePmuKV (NBPtr);
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *     Pre set PHY recevier power state. This function sets the Max
 *      Limit for DIMM Frequency
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPrePhyReceiverLowPowerKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MEM_DATA_STRUCT *MemPtr;
  MemPtr = NBPtr->MemPtr;

  // Work around for DDR PHY bug.  ENH391725
  if (MemPtr->PhyReceiverLowPower) {
    if (NBPtr->DCTPtr->Timings.TargetSpeed > DDR800_FREQUENCY) {
      NBPtr->DCTPtr->Timings.TargetSpeed = DDR800_FREQUENCY;
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Post set PHY recevier power state. This function gets called after PMU
 *     based Memory Training and sets up the Major Mode in DqDqsRecvCntrl1
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPostPhyReceiverLowPowerKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Data;
  MEM_DATA_STRUCT *MemPtr;
  MemPtr = NBPtr->MemPtr;

  // Work around for DDR PHY bug.  ENH391725
  if (MemPtr->PhyReceiverLowPower) {
    for (Data = 0; Data < 9; Data++) {
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ,  Data, BFMajorMode), 4);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, Data, BFMajorMode), 4);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Enable DDR3 PHY calibrating
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNEnablePhyCalibrationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xA3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xB3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 0);
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x83);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Waits for PHY calibrating to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPendOnPhyCalibrateCompletionKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // read CalBusy and discard result - this is needed to latch the
  // initial 'busy' status
  MemNGetBitFieldNb (NBPtr, RegCalBusy);

  // Wait until D18F2x9C_x20097 == 0.
  MemNPollBitFieldNb (NBPtr, RegCalBusy, 0, PCI_ACCESS_TIMEOUT, FALSE);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Set the rate for PHY calibrate
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNRateOfPhyCalibrateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalInitMode, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x03);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x13);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function tri-state unused pads.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNTriStateUnusedPadKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT8           PinType[4]  = {PSO_MEMCLK_DIS, PSO_CKE_TRI, PSO_ODT_TRI, PSO_CS_TRI};
  CONST UINT8           TabSize[4]  = {             5,           4,           4,          8};
  CONST UINT8           PadName[4]  = {       PAD_CLK,     PAD_CKE,     PAD_ODT,     PAD_CS};
  UINT8  *TabPtr;
  UINT8  i;
  UINT8  k;
  UINT8  Value;
  UINT8  PadNumber;
  BOOLEAN MemoryAllClocks;

  TabPtr = NULL;

  //===================================================================
  // Tristate DATA[8] if ECC is not supported
  //===================================================================
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state ECC Pad\n");
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDataTxImpedance), 0);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, RegDataTxImpedance), 0);
  }

  MemoryAllClocks = UserOptions.CfgMemoryAllClocksOn;
  IDS_OPTION_HOOK (IDS_ALL_MEMORY_CLOCK, &MemoryAllClocks, &(NBPtr->MemPtr->StdHeader));

  //===================================================================
  // Tristate unused MEMCLK, CKE, ODT and CS to save power
  //===================================================================
  //
  for (k = 0; k < GET_SIZE_OF (PinType); k++) {
    if (NBPtr->IsSupported[CheckFindPSOverideWithSocket]) {
      TabPtr = FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration, PinType[k], NBPtr->MCTPtr->SocketId, MemNGetSocketRelativeChannelNb (NBPtr, NBPtr->Dct, 0), 0,
                                    &(NBPtr->MCTPtr->LogicalCpuid), &(NBPtr->MemPtr->StdHeader));
    }

    if (TabPtr != NULL) {
      Value = 0;
      for (i = 0; i < TabSize[k]; i++) {
        if ((NBPtr->DCTPtr->Timings.CsPresent & TabPtr[i]) == 0) {
          Value |= (UINT8) (1 << i);
        }
      }

      // CAD/Pad numbers need tri-state
      while (Value != 0) {
        PadNumber = LibAmdBitScanForward (Value);
        Value &= ~ (1 << PadNumber);

        if (PadName[k] == PAD_CLK) {
          if (!MemoryAllClocks) {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state MEMCLK %d\n", PadNumber);
            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
          }
        } else {
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state %s %d\n", (PadName[k] == PAD_CKE) ? "MEMCKE" :
                                                                (PadName[k] == PAD_ODT) ? "MEMODT" :
                                                                ((PadName[k] == PAD_CS) ? "MEMCS"  : "UnKnown"), PadNumber);

          MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
        }
      }
    }
  }

  MemFInitTableDrive (NBPtr, MTAfterPlatformSpec);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Determine slow OPN mode is true if any valid NB-Pstates not tied
 *     to M1 has NCLK=800MHZ/M0=667MHZ or NCLK=800MHZ/M0=1333MHZ
 *
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *
 *     @return         TRUE  - slow_OPN mode, otherwise False
 *
 * ----------------------------------------------------------------------------
 */
BOOLEAN
STATIC
MemNSlowOpnModeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8  NbPs;
  UINT32 NbClkFreqMhz;
  UINT16 Speed;

  for (NbPs = 0; NbPs < 4; NbPs++) {
    if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) != 1) {
      NbClkFreqMhz = MemNGetNbClkFreqByPstateUnb (NBPtr, NbPs);
      Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      if ((NbClkFreqMhz == 800) && ((Speed == DDR667_FREQUENCY) || (Speed == DDR1333_FREQUENCY))) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

