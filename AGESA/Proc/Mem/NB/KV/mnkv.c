/* $NoKeywords:$ */
/**
 * @file
 *
 * mnkv.c
 *
 * Common Northbridge  functions for KV
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mp.h"
#include "mnkv.h"
#include "mu.h"
#include "S3.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "Filecode.h"
#include "merrhdl.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_KV_MNKV_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MEM_PSC_FLOW_DEFTRUE (BOOLEAN (*) (MEM_NB_BLOCK*, MEM_PSC_TABLE_BLOCK *)) memDefTrue

VOID
STATIC
MemNGetPORFreqLimitTblDrvKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;
extern PSO_ENTRY DefaultPlatformMemoryConfiguration[];
extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];
extern MEM_PSC_FLOW MemPGetMaxFreqSupported;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in]        *FeatPtr  - Pointer to the MEM_FEAT_BLOCK_NB
 *     @param[in]        *SharedPtr - Pointer to the MEM_SHARED_DATA
 *     @param[in]        NodeID  - UINT8 indicating node ID of the NB object.
 *
 *     @return     Boolean indicating that this is the correct memory
 *                 controller type for the node number that was passed in.
 */

BOOLEAN
MemConstructNBBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       MEM_FEAT_BLOCK_NB *FeatPtr,
  IN       MEM_SHARED_DATA *SharedPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 Dct;
  UINT8 Channel;
  UINT8 SpdSocketIndex;
  UINT8 SpdChannelIndex;
  DIE_STRUCT *MCTPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;

  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem->SocketId, &(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedKV (&(MemPtr->DiesPerSystem->LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->MemPtr = MemPtr;
  NBPtr->RefPtr = MemPtr->ParameterListPtr;
  NBPtr->SharedPtr = SharedPtr;

  MCTPtr = MemPtr->DiesPerSystem;
  NBPtr->MCTPtr = MCTPtr;
  NBPtr->MCTPtr->NodeId = NodeID;
  NBPtr->PciAddr.AddressValue = MCTPtr->PciAddr.AddressValue;
  NBPtr->VarMtrrHiMsk = GetVarMtrrHiMsk (&(MemPtr->DiesPerSystem->LogicalCpuid), &(MemPtr->StdHeader));

  //
  // Allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
  //
  AllocHeapParams.RequestedBufferSize = MAX_DCTS_PER_NODE_KV * (
                                          sizeof (DCT_STRUCT) + (
                                            MAX_CHANNELS_PER_DCT_KV * (sizeof (CH_DEF_STRUCT) + sizeof (MEM_PS_BLOCK) + sizeof (CH_TIMING_STRUCT))
                                          )
                                        );
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DCT_STRUCT_HANDLE, NodeID, 0, 0);
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) != AGESA_SUCCESS) {
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_DCT_STRUCT_AND_CH_DEF_STRUCTs, NBPtr->Node, 0, 0, 0, &MemPtr->StdHeader);
    SetMemError (AGESA_FATAL, MCTPtr);
    ASSERT(FALSE); // Could not allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
    return FALSE;
  }

  MCTPtr->DctCount = MAX_DCTS_PER_NODE_KV;
  MCTPtr->DctData = (DCT_STRUCT *) AllocHeapParams.BufferPtr;
  AllocHeapParams.BufferPtr += MAX_DCTS_PER_NODE_KV * sizeof (DCT_STRUCT);
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    MCTPtr->DctData[Dct].Dct = Dct;
    MCTPtr->DctData[Dct].ChannelCount = MAX_CHANNELS_PER_DCT_KV;
    MCTPtr->DctData[Dct].ChData = (CH_DEF_STRUCT *) AllocHeapParams.BufferPtr;
    MCTPtr->DctData[Dct].ChData[0].Dct = Dct;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_KV * sizeof (CH_DEF_STRUCT);
    MCTPtr->DctData[Dct].TimingsMemPs1 = (CH_TIMING_STRUCT *) AllocHeapParams.BufferPtr;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_KV * sizeof (CH_TIMING_STRUCT);
  }
  NBPtr->PSBlock = (MEM_PS_BLOCK *) AllocHeapParams.BufferPtr;

  //
  // Initialize Socket List
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    MemPtr->SocketList[MCTPtr->SocketId].ChannelPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].ChData[0]);
    MemPtr->SocketList[MCTPtr->SocketId].TimingsPtr[(MCTPtr->DieId * 2) + Dct] = &(MCTPtr->DctData[Dct].Timings);
    MCTPtr->DctData[Dct].ChData[0].ChannelID = (MCTPtr->DieId * 2) + Dct;
  }

  MemNInitNBDataKV (NBPtr);

  FeatPtr->InitEarlySampleSupport (NBPtr);

  NBPtr->FeatPtr = FeatPtr;

  //
  // Calculate SPD Offsets per channel and assign pointers to the data.  At this point, we calculate the Node-Dct-Channel
  // centric offsets and store the pointers to the first DIMM of each channel in the Channel Definition struct for that
  // channel.  This pointer is then used later to calculate the offsets to be used for each logical dimm once the
  // dimm types(QR or not) are known. This is done in the Technology block constructor.
  //
  // Calculate the SpdSocketIndex separately from the SpdChannelIndex.
  // This will facilitate modifications due to some processors that might
  // map the DCT-CHANNEL differently.
  //
  SpdSocketIndex = GetSpdSocketIndex (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr->MCTPtr->SocketId, &MemPtr->StdHeader);
  //
  // Traverse the Dct/Channel structures
  //
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    for (Channel = 0; Channel < MAX_CHANNELS_PER_DCT_KV; Channel++) {

      NBPtr->MCTPtr->DctData[Dct].ChData[Channel].ColumnCount = MAX_DELAYS;

      //
      // Calculate the number of Dimms on this channel using the
      //   die/dct/channel to Socket/channel conversion.
      //
      SpdChannelIndex = GetSpdChannelIndex (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                            NBPtr->MCTPtr->SocketId,
                                            MemNGetSocketRelativeChannelNb (NBPtr, Dct & 1, Channel),
                                            &MemPtr->StdHeader);
      NBPtr->MCTPtr->DctData[Dct].ChData[Channel].SpdPtr = &(MemPtr->SpdDataStructure[SpdSocketIndex + SpdChannelIndex]);
    }
  }

  //
  // Initialize Dct and DctCfgSel bit
  //
  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, 0);
  MemNSwitchDCTNb (NBPtr, 0);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes member functions and variables of NB block.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitNBDataKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  NBPtr->DctCachePtr = NBPtr->DctCache;
  NBPtr->PsPtr = NBPtr->PSBlock;

  MemNInitNBRegTableKV (NBPtr, NBPtr->NBRegTable);
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_KV;
  NBPtr->ChannelCount = MAX_CHANNELS_PER_DCT_KV;
  NBPtr->NodeCount = MAX_NODES_SUPPORTED_KV;

  NBPtr->MemCleared = FALSE;
  NBPtr->StartupSpeed = DDR667_FREQUENCY;
  NBPtr->DefDctSelIntLvAddr = 4;
  NBPtr->NbFreqChgState = 0;
  NBPtr->NbPsCtlReg = 0;
  NBPtr->CsRegMsk = 0x7FF8FFE0;
  NBPtr->MemPstate = MEMORY_PSTATE0;
  NBPtr->MemPstateStage = MEMORY_PSTATE_1ST_STAGE;
  NBPtr->CsPerChannel = MAX_CS_PER_CHANNEL_KV;

  //PerRankTimingEn is always set to 0 which force CsPerDelay = 2.
  NBPtr->CsPerDelay = 2;

  // Legacy settings, can probably be removed
  NBPtr->Ganged = FALSE;
  NBPtr->PosTrnPattern = POS_PATTERN_256B;
  NBPtr->RdDqsDlyRetrnStat = RDDQSDLY_RTN_NEEDED;
  NBPtr->RcvrEnDlyLimit = 0x1FF;
  NBPtr->FreqChangeParam = NULL;
  NBPtr->MaxRxEnSeedTotal = 0x1FF;
  NBPtr->MinRxEnSeedGross = 0;

  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &NBPtr->MemPtr->StdHeader);
  MemNInitNBDataNb (NBPtr);

  // ---------------------------------------------------------------------------
  // Common functions
  // ---------------------------------------------------------------------------
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldKV;
  NBPtr->SetEccSymbolSize = MemNSetEccSymbolSizeNb;
  NBPtr->PollBitField = MemNPollBitFieldNb;
  NBPtr->BrdcstCheck = MemNBrdcstCheckNb;
  NBPtr->BrdcstSet = MemNBrdcstSetNb;
  NBPtr->GetTrainDly = MemNGetTrainDlyNb;
  NBPtr->SetTrainDly = MemNSetTrainDlyNb;
  NBPtr->StitchMemory = MemNStitchMemoryNb;
  NBPtr->DisableDCT = MemNDisableDctKV;
  NBPtr->HtMemMapInit = MemNHtMemMapKV;
  NBPtr->SyncAddrMapToAllNodes = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefTrue;
  NBPtr->CpuMemTyping = MemNCPUMemTypingNb;

  // ---------------------------------------------------------------------------
  // Misc functions
  // ---------------------------------------------------------------------------
  NBPtr->GetSocketRelativeChannel = MemNGetSocketRelativeChannelNb;
  NBPtr->MemNPlatformSpecificFormFactorInitNb = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;

  NBPtr->SyncTargetSpeed = MemNSyncTargetSpeedNb;
  NBPtr->MemNCapSpeedBatteryLife = MemNCapSpeedBatteryLifeKV;
  NBPtr->ProgramCycTimings = MemNProgramCycTimingsKV;

  NBPtr->PlatformSpec = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;

  NBPtr->GetMemClkFreqInCurrentContext = MemNGetMemClkFreqInCurrentContextKV;
  NBPtr->ProgramNbPsDependentRegs = MemNProgramNbPstateDependentRegistersKV;

  NBPtr->GetUmaSize = MemNGetUmaSizeKV;
  NBPtr->UMAMemTyping = MemNUMAMemTypingNb;

  NBPtr->MemNDisableScrubber = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->MemNRestoreScrubber = (VOID (*) (MEM_NB_BLOCK *)) memDefRet;
  NBPtr->MemNSetEccExclusionRange = MemNSetEccExclusionRangeUnb;
  NBPtr->MemNDisableMemClk = MemNDisableMemClkUnb;
  NBPtr->MemNTristateUnusedChipSelects = MemNTristateUnusedChipSelectsNb;

  // ---------------------------------------------------------------------------
  // Training functions
  // ---------------------------------------------------------------------------
  NBPtr->WritePattern = MemNContWritePatternUnb;
  NBPtr->ReadPattern = MemNContReadPatternUnb;
  NBPtr->FlushPattern = (VOID (*) (MEM_NB_BLOCK *, UINT32, UINT16)) memDefRet;
  NBPtr->TrainingPatternInit = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->TrainingPatternFinalize = (AGESA_STATUS (*) (MEM_NB_BLOCK *)) memDefRetSuccess;
  NBPtr->CompareTestPattern = MemNCompareTestPatternUnb;
  NBPtr->GetSysAddr = (BOOLEAN (*) (MEM_NB_BLOCK *, UINT8, UINT32 *)) memDefTrue;
  NBPtr->getMaxLatParams = MemNGetMaxLatParamsKV;
  NBPtr->WaitXMemClks = MemNWaitXMemClksNb;

  NBPtr->MemNGetMemoryWidth = MemNGetMemoryWidthUnb;

  // ---------------------------------------------------------------------------
  // Family specific flags
  // ---------------------------------------------------------------------------
  NBPtr->IsSupported[G5DimmInD3Socket] = TRUE;
  NBPtr->IsSupported[CheckFindPSOverideWithSocket] = TRUE;

  // ---------------------------------------------------------------------------
  // Family specific hooks
  // ---------------------------------------------------------------------------
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNDctCfgSelectKV;
  NBPtr->FamilySpecificHook[SetupHwTrainingEngine] = MemNSetupHwTrainingEngineUnb;
  NBPtr->FamilySpecificHook[ReleaseNbPstate] = MemNReleaseNbPstateKV;
  NBPtr->FamilySpecificHook[FixupUmaInfo] = MemNFixupUmaInfoKV;
  NBPtr->FamilySpecificHook[AdjustCSIntLvLowAddr] = MemNCSIntLvLowAddrAdjKV;
  NBPtr->FamilySpecificHook[ForceEccSymbolSize] = MemNForceEccSymbolSizeKV;
  NBPtr->FamilySpecificHook[DisableMemHoleMapping] = MemNDisableMemHoleMappingKV;
  NBPtr->FamilySpecificHook[RestoreMemHoleMapping] = MemNRestoreMemHoleMappingKV;
  NBPtr->FamilySpecificHook[DramScrubErratum792] = MemNDramScrubErratum792KV;
  // ---------------------------------------------------------------------------
  // Initialize PS block's functions
  // ---------------------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    NBPtr->PSBlock[Dct].MemPGetPORFreqLimit = MemNGetPORFreqLimitTblDrvKV;
    NBPtr->PSBlock[Dct].MemPDoPs = (BOOLEAN (*) (MEM_NB_BLOCK *)) memDefRet;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the default values in the MEM_DATA_STRUCT
 *
 *   NOTE: This function must first verify the NB Family.
 *
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *
 */
VOID
MemNInitDefaultsKV (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  //
  // DllPDBypassMode
  //
  MemPtr->ParameterListPtr->EnableDllPDBypassMode = FALSE;

  //
  // AMP
  //
  MemPtr->ParameterListPtr->AmpEnable = FALSE;
  MemPtr->ParameterListPtr->AmpWarningMsgEnable = FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This is function gets KV PS tables
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          Pointer to EntryOfTables
 *
 */

MEM_PSC_TABLE_BLOCK *
MemNGetPsTablesKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;

  for (i = 0; (memPlatSpecFlowArray[i] != NULL); i++) {
    if ((memPlatSpecFlowArray[i])->MaxFrequency == MEM_PSC_FLOW_DEFTRUE) {
      break;
    }
  }
  ASSERT (memPlatSpecFlowArray[i] != NULL);
  return memPlatSpecFlowArray[i]->EntryOfTables;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Get Kaveri package type.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          The package type
 *
 */

KV_PACKAGE_TYPE
MemNGetPackageTypeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CPUID_DATA CpuId;
  UINT8 RegValue;
  KV_PACKAGE_TYPE PkgType;

  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, &(NBPtr->MemPtr->StdHeader));
  RegValue = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28

  if (RegValue == 0) {
    PkgType = KV_FP3;
  } else if (RegValue == 1) {
    PkgType = KV_FM2R2;
  } else {
    PkgType = UNKNOWN;
  }

  return PkgType;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Initialize the CAD Bus Configuration buffer.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - initialize the CAD Bus Configuration buffer successfully.
 *     @return  FALSE - Fail to initialize the CAD Bus Configuration buffer.
 */

BOOLEAN
MemNInitDramCadBusConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  LOCATE_HEAP_PTR LocHeap;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  DRAM_CAD_BUS_CONFIG_KV *DramCadBusConfigPtr;

  LocHeap.BufferHandle = AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    DramCadBusConfigPtr = (DRAM_CAD_BUS_CONFIG_KV *) LocHeap.BufferPtr;
  } else {
    // Allocate temporary buffer for DRAM CAD Bus Configuration
    AllocHeapParams.RequestedBufferSize = sizeof (DRAM_CAD_BUS_CONFIG_KV);
    AllocHeapParams.BufferHandle = AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE;
    AllocHeapParams.Persist = HEAP_LOCAL_CACHE;

    if (HeapAllocateBuffer (&AllocHeapParams, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
      ASSERT (FALSE); // Could not allocate heap for DRAM CAD Bus Configuration.
    }

    DramCadBusConfigPtr = (DRAM_CAD_BUS_CONFIG_KV *) AllocHeapParams.BufferPtr;
  }

  LibAmdMemFill ((VOID *)DramCadBusConfigPtr, 0, (UINTN)sizeof (DRAM_CAD_BUS_CONFIG_KV), &(NBPtr->MemPtr->StdHeader));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Free the DRAM CAD Bus Configuration buffer.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - Free DRAM CAD Bus Configuration buffer successfully.
 *     @return  FALSE - Fail to free DRAM CAD Bus Configuration buffer.
 */

BOOLEAN
MemNPostDramCadBusConfigKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  return (BOOLEAN) (HeapDeallocateBuffer (AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Clock and power gate unused DCTs
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNClockAndPowerGateUnusedDctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFDctClkGateEn, 6);
  MemNSetBitFieldNb (NBPtr, BFDctPwrGateEn, 6);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Exclude DIMM failed PMU training.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDimmExcludesKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  BOOLEAN ErrorRecovery;

  ErrorRecovery = TRUE;

  IDS_OPTION_HOOK (IDS_MEM_ERROR_RECOVERY, &ErrorRecovery, &NBPtr->MemPtr->StdHeader);

  if (ErrorRecovery && (NBPtr->MCTPtr->ChannelTrainFail != 0)) {
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb (NBPtr, Dct);

      // Disable the channel when CS fails PMU training.
      if (NBPtr->DCTPtr->Timings.CsTrainFail != 0) {
        NBPtr->DCTPtr->Timings.CsTestFail = NBPtr->DCTPtr->Timings.CsPresent;
        MemNDisableDctKV (NBPtr);
      }

      MemNStitchMemoryNb (NBPtr);
    }
  }
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This is function gets the POR speed limit for KV
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNGetPORFreqLimitTblDrvKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemPGetMaxFreqSupported (NBPtr, MemNGetPsTablesKV (NBPtr));
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This is function gets the erratum 792 for KV  to disable ScrubRate and ScrubRedir
 *
 *     @param[in,out]   *NBPtr      - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam   - Pointer to Optional Parameter
 *
 */
BOOLEAN
MemNDramScrubErratum792KV (
  IN OUT   MEM_NB_BLOCK  *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  MemNSetBitFieldNb (NBPtr, BFDramScrub, 0);
  MemNSetBitFieldNb (NBPtr, BFScrubReDirEn, 0);
  return TRUE;
}