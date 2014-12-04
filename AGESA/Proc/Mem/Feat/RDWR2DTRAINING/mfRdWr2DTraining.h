/* $NoKeywords:$ */
/**
 * @file
 *
 * mf2DRdWrTraining.h
 *
 * Common Definitions ot support 2D Read/Write Training
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/2DRdWrTraining)
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

#ifndef _MFRDWR2DTRAINING_H_
#define _MFRDWR2DTRAINING_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */
#define MAX_2D_VREF_ENTRIES         0x20 ///< Maximum number of vref entries
#define MAX_RD_WR_DLY_ENTRIES       0x40 ///< Maximum number of RD/WR Delay Entries
#define VREF_ADDITIONAL_STEP_SIZE   0x0  ///< Vref Additional Step size
#define RDDQS_ADDITIONAL_STEP_SIZE  0x0  ///< RdDqs Additional Step size
#define MAX_2D_RD_SEED_COUNT          4  ///< Max Seed count
#define MAX_2D_RD_WR_CS_PER_CHANNEL   8  ///< Max CS per Rd Wr delay group

#define EYERIM_PARALLEL_SAMPLING    TRUE
#define EYERIM_BROADCAST_DELAYS     TRUE

/// Structure for RD DQS 2D training RDDQS delays.
typedef struct {
  UINT32  PosRdWrDly;      ///< Positive RdDQS Delay
  UINT32  NegRdWrDly;      ///< Negative RdDQS Delay
} RD_WR_2D;

/// Structure for RD DQS 2D training Vref delays
typedef struct {
  RD_WR_2D *Vref;                             ///< Pointer to Vref Entries
  UINT16 Convolution[MAX_RD_WR_DLY_ENTRIES];  ///< Total number of passes in each mask
  UINT8 PosHeight[MAX_RD_WR_DLY_ENTRIES / 2]; ///< Positive Vref height Height per Delay
  UINT8 NegHeight[MAX_RD_WR_DLY_ENTRIES / 2]; ///< Negative Vref height Height per Delay
  UINT8 HalfDiamondHeight;                    ///< Half of the Height per BL (height of pos/neg vref)
  UINT8 MaxRdWrDly;                           ///< Max RdWrDly from convolution function
} VREF_RD_WR_2D;

/// Structure for RD DQS 2D training Nibbles
typedef struct {
  VREF_RD_WR_2D  Lane[MAX_NUMBER_LANES];                      ///< Bytelane or Nibble
  UINT8 Vnom;                                                 ///< Nominal Vref value
  UINT8 MaxRdWrSweep;                                         ///< Maximum RdDqs or WrDat Sweep size
  UINT8 SmallestPosMaxVrefperCS[MAX_2D_RD_WR_CS_PER_CHANNEL]; ///< Smallest Positive Max Vref per CS
  UINT8 SmallestNegMaxVrefperCS[MAX_2D_RD_WR_CS_PER_CHANNEL]; ///< Smallest Negative Max Vref per CS
  UINT8 DiamondLeft[MAX_2D_VREF_ENTRIES];                     ///< Left edge of Diamond for shape display
  UINT8 DiamondRight[MAX_2D_VREF_ENTRIES];                    ///< Left edge of Diamond for shape display
  UINT8 RdWrDly;                                              ///< RdDQS or WrDat Dly setting
  VOID* SavedData;                                            ///< Algorithm-specific saved data
} MEM_RD_WR_2D_ENTRY;

/// Structure used for RD WR 2D Eye Rim Search Algorithm
typedef struct {
  VREF_RD_WR_2D  LaneSaved[MAX_NUMBER_LANES]; ///< Bytelane or Nibble that was saved
  INT8    xMin;                            ///< Minimum value for RdDqs delays
  INT8    xMax;                            ///< Maximum value for RdDqs delays
  INT8    yMin;                            ///< Minimum value for vref delays
  INT8    yMax;                            ///< Maximum value for vref delays
  BOOLEAN ParallelSampling;                ///< Flag to indicate parallel sampling feature
  BOOLEAN BroadcastDelays;                 ///< Flag to indicate if delays will be broadcast
  UINT32  SampleCount;                     ///< Count of samples taken
  UINT32  VrefUpdates;                     ///< Count of updates to Vref
  UINT32  RdWrDlyUpdates;                  ///< Count of updates to RdWrDly
  INT8    Dirs[2];                         ///< List of directions to search for each axis
} MEM_RD_WR_2D_RIM_ENTRY;

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */
BOOLEAN
MemFAmdRdWr2DTraining (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID* OptParam
  );

BOOLEAN
MemFCheckRdWr2DTrainingPerConfig (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemFRdWr2DProgramIntExtVrefSelect (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemFRdWr2DProgramVref (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *VrefPtr
  );

BOOLEAN
MemFRdWr2DProgramDelays (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Delay
  );

VOID
MemFRdWr2DStoreResult (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data,
  IN       UINT32 InPhaseResult[],
  IN       UINT32 PhaseResult180[]
  );

BOOLEAN
MemFRdWr2DHeight (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

UINT8
MemFGetRdWr2DWidth (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

BOOLEAN
MemFCheckRdWr2DDiamondMaskStep (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data,
  IN       UINT8 Vref,
  IN       UINT8 Lane
  );
BOOLEAN
MemFRdWr2DApplyMask (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

BOOLEAN
MemFRdWr2DProcessConvolution (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

BOOLEAN
MemFRdWr2DProgramMaxDelays (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

BOOLEAN
MemFRdWr2DFindCsVrefMargin (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

BOOLEAN
MemFRdWr2DFinalVrefMargin (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

UINT8
MemFRdWr2DGetMaxLanes (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemFRdWr2DTrainingInit (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemFRdWr2DEyeRimSearch (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *OptParam
  );

VOID
MemFRdWr2DDisplaySearch (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       MEM_RD_WR_2D_ENTRY *Data
  );

/*******************************
* Pattern Generation Functions
*******************************/
BOOLEAN
MemFRdWr2DCompareInPhase (
  IN OUT   MEM_NB_BLOCK *NBPtr,
     OUT   VOID *Result
  );

BOOLEAN
MemFRdWr2DCompare180Phase (
  IN OUT   MEM_NB_BLOCK *NBPtr,
     OUT   VOID *Result
  );

BOOLEAN
MemFRdWr2DInitVictim (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemFRdWr2DInitVictimChipSel (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemFRdWr2DStartVictim (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *SeedCountPtr
  );

BOOLEAN
MemFRdWr2DFinalizeVictim  (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemFRdWr2DProgramDataPattern (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID* PatternIndexPtr
  );

#endif  /* _MFRDWR2DTRAINING_H_ */
