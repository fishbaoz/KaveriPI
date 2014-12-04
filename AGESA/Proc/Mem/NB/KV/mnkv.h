/* $NoKeywords:$ */
/**
 * @file
 *
 * mnkv.h
 *
 * Northbridge KV
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

#ifndef _MNKV_H_
#define _MNKV_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */
#define DRAM_TYPE_DDR3_KV             0
#define DRAM_TYPE_GDDR5_KV            1

#define MAX_DCTS_PER_NODE_KV          4
#define MAX_CHANNELS_PER_DCT_KV       1
#define MAX_NODES_SUPPORTED_KV        1
#define MAX_CS_PER_CHANNEL_KV         4

#define MAX_D3_DCTS_PER_NODE_KV       2
#define MAX_MEMORY_PSTATE_KV          2

#define DEFAULT_WR_ODT_KV 6
#define DEFAULT_RD_ODT_KV 6
#define DEFAULT_RD_ODT_TRNONDLY_KV 0

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */
#define IS_DCT_MASTER(Dct)  ((Dct) < 2)

#define FIX_DCT(Dct)  ((UINT8) (((NBPtr->IsSupported[MasterDct1Relocate]) && ((Dct) == 1)) ? 3 : (Dct)))

#define MEMCLK_FREQ_TO_ID(Freq)  (((Freq) == DDR2400_FREQUENCY) ? 0x1F : (((Freq) / 33) - 6))

#define MEMCLK_ID_TO_FREQ(Id)  ((((Id) + 6) * 33) + (((Id) + 6) / 3))


/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/// Kaveri package type
typedef enum {
  KV_FP3,           ///< FP3 (BGA)
  KV_FM2R2,         ///< FM2R2 (uPGA)
  UNKNOWN           ///< Unknown package type
} KV_PACKAGE_TYPE;

/// DRAM CAD Bus Configuration for all Memory Pstate.
typedef struct _DRAM_CAD_BUS_CONFIG_KV {
  BOOLEAN SlowMode [MAX_MEMORY_PSTATE_KV];      ///< Slow Access Mode
  UINT32  DctAddrTmg [MAX_MEMORY_PSTATE_KV];    ///< AddrCmdSetup, CsOdtSetup, and CkeSetup
} DRAM_CAD_BUS_CONFIG_KV;

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
MemConstructNBBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  );

VOID
MemNInitNBDataKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitNBRegTableD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitDefaultsKV (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

UINT16
MemNGetMemClkFreqInCurrentContextKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramNbPstateDependentRegistersKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNReleaseNbPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNSetDdrModeD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNSetPhyDdrModeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

VOID
MemNConfigureDctForTrainingD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDctNormalD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNSwitchMemPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );

VOID
MemNSyncChannelInitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramMemPstateRegD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );

BOOLEAN
MemNHtMemMapKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPlatformSpecKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDisableDctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNInitNBRegTableKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   TSEFO NBRegTable[]
  );

BOOLEAN
MemNIsIdSupportedKV (
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  );

BOOLEAN
MemNDctCfgSelectKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  );

UINT32
MemNCmnGetSetFieldKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       BIT_FIELD_NAME FieldName,
  IN       UINT32 InputValue
  );

UINT32
MemNGetUmaSizeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNAllocateC6AndAcpEngineStorageKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramCycTimingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNAutoConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramTurnaroundTimingsD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyGenCfgKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyVoltageLevelKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramChannelFreqKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  );

BOOLEAN
MemNCSIntLvLowAddrAdjKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *LowBit
  );

VOID
MemNPredriverInitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNProgramCadDataBusD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPhyFifoConfigD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNGetMaxLatParamsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly,
  IN OUT   UINT16 *MinDlyPtr,
  IN OUT   UINT16 *MaxDlyPtr,
  IN OUT   UINT16 *DlyBiasPtr
  );

VOID
MemNResetRcvFifoKV (
  IN OUT   struct _MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Dummy
  );

AGESA_STATUS
MemMD3FlowKV (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  );

VOID
MemNSetPmuSequenceControlKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramConfigD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPopulatePmuSramTimingsD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNFixupUmaInfoKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNFinalizeMctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNCapSpeedBatteryLifeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

MEM_PSC_TABLE_BLOCK *
MemNGetPsTablesKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNForceEccSymbolSizeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Size
  );

VOID
MemNInitChannelIntlvAddressBitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDramPhyPowerSavingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNTriStateUnusedPadKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNAddlDramPhyPowerSavingsKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT8
MemNNumberOfPmuFirmwareImageKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNModeRegisterInitializationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

KV_PACKAGE_TYPE
MemNGetPackageTypeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNConfigureDisDllShutdownSrKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPrePhyReceiverLowPowerKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPostPhyReceiverLowPowerKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNInitDramCadBusConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPostDramCadBusConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNDisableMemHoleMappingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

BOOLEAN
MemNRestoreMemHoleMappingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  );

VOID
MemNEnablePhyCalibrationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNPendOnPhyCalibrateCompletionKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNRateOfPhyCalibrateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDisablePmuKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNClockAndPowerGateUnusedDctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNDimmExcludesKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNDramScrubErratum792KV (
  IN OUT  MEM_NB_BLOCK  *NBPtr,
  IN OUT  VOID          *OptParam
  );
#endif  /* _MNKV_H_ */
