/* $NoKeywords:$ */
/**
 * @file
 *
 * mmflowd3kv.c
 *
 * Main Memory initialization sequence for DDR3 only for KV
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main/KV)
 * @e \$Revision: 287810 $ @e \$Date: 2014-03-21 14:33:29 -0500 (Fri, 21 Mar 2014) $
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
#include "Ids.h"
#include "OptionMemory.h"
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mnkv.h"
#include "mt.h"
#include "mt3.h"
#include "mu.h"
#include "mmlvddr3.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mnPmuSramMsgBlockKV.h"
#include "cpuFamilyTranslation.h"
#include "IdsF15KvAllService.h"
#include "Filecode.h"
#include "GeneralServices.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_MAIN_KV_MMFLOWD3KV_FILECODE
/* features */
#include "mftds.h"

extern MEM_FEAT_BLOCK_MAIN MemFeatMain;
extern OPTION_MEM_FEATURE_MAIN MemMS3Save;

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
 *
 *      This function defines the memory initialization flow for
 *      systems that only support DDR3 and KV processor.
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_CRITICAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
MemMFlowD3KV (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  MEM_NB_BLOCK  *NBPtr;
  MEM_TECH_BLOCK *TechPtr;
  MEM_DATA_STRUCT *MemPtr;

  NBPtr = MemMainPtr->NBPtr;
  TechPtr = MemMainPtr->TechPtr;
  MemPtr = MemMainPtr->MemPtr;

  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[BSP_DIE].SocketId, &(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedKV (&(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid))) {
    MemPtr->IsFlowControlSupported = FALSE;
    return AGESA_FATAL;
  } else {
    MemPtr->IsFlowControlSupported = TRUE;
  }

  MemFInitTableDrive (&NBPtr[BSP_DIE], MTBeforeInitializeMCT);

  // Clear DisDllShutdownSR prior any P-State changes.
  MemNConfigureDisDllShutdownSrKV (NBPtr);

  //----------------------------------------------------------------
  // Force NB-Pstate to NBP0
  //----------------------------------------------------------------
  MemNChangeNbFrequencyWrapUnb (NBPtr, 0);

  NBPtr->IsSupported[G5DimmInD3Socket] = FALSE;

  if (MemTDIMMPresence3 (NBPtr[BSP_DIE].TechPtr) && (NBPtr[BSP_DIE].MCTPtr->DimmPresent != 0)) {
    return MemMD3FlowKV (MemMainPtr);
  } else {
    PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL; // No DIMMs found
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function defines the DDR3 initialization flow
 *      when only DDR3 DIMMs are present in the system
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_CRITICAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
MemMD3FlowKV (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  UINT8            Dct;
  MEM_NB_BLOCK     *NBPtr;
  MEM_DATA_STRUCT  *MemPtr;
  ID_INFO          CallOutIdInfo;
  INT8             MemPstate;
  UINT8            LowestMemPstate;
  UINT8            PmuImage;
  BOOLEAN          ErrorRecovery;
  BOOLEAN          IgnoreErr;

  NBPtr = &MemMainPtr->NBPtr[BSP_DIE];
  MemPtr = MemMainPtr->MemPtr;
  ErrorRecovery = TRUE;
  IgnoreErr = FALSE;

  IDS_HDT_CONSOLE (MEM_FLOW, "DDR3 Mode\n");

  //----------------------------------------------------------------
  // Defines DDR3 registers
  //----------------------------------------------------------------
  MemNInitNBRegTableD3KV (NBPtr);

  //----------------------------------------------------------------
  // Clock and power gate unsued channels
  //----------------------------------------------------------------
  MemNClockAndPowerGateUnusedDctKV (NBPtr);

  //----------------------------------------------------------------
  // Set DDR3 mode
  //----------------------------------------------------------------
  MemNSetDdrModeD3KV (NBPtr);

  //----------------------------------------------------------------
  // Enable PHY Calibration
  //----------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctDimmValid != 0) {
      MemNEnablePhyCalibrationKV (NBPtr);
    }
  }

  //----------------------------------------------------------------
  // Low voltage DDR3
  //----------------------------------------------------------------
  // Levelize DDR3 voltage based on socket, as each socket has its own voltage for dimms.
  AGESA_TESTPOINT (TpProcMemLvDdr3, &(MemMainPtr->MemPtr->StdHeader));
  if (!MemFeatMain.LvDDR3 (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Find the maximum speed that all DCTs are capable running at
  //----------------------------------------------------------------
  if (!MemTSPDGetTargetSpeed3 (NBPtr->TechPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Adjust memClkFreq based on MaxDdrRate
  //----------------------------------------------------------------
  MemNAdjustDdrSpeed3Unb (NBPtr);

  //------------------------------------------------
  // Finalize target frequency
  //------------------------------------------------
  if (!MemMLvDdr3PerformanceEnhFinalize (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  //  Program DCT address map
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "DCT addr map\n");
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctDimmValid == 0) {
      MemNDisableDctKV (NBPtr);
    } else {
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCS Addr Map\n");
      if (MemTSPDSetBanks3 (NBPtr->TechPtr)) {
        if (MemNStitchMemoryNb (NBPtr)) {
          if (NBPtr->DCTPtr->Timings.CsEnabled == 0) {
            MemNDisableDctKV (NBPtr);
          } else {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tAuto Cfg\n");
            MemNAutoConfigKV (NBPtr);
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tTraining Cfg\n");
            MemNConfigureDctForTrainingD3KV (NBPtr);
          }
        }
      }
    }
  }

  IDS_OPTION_HOOK (IDS_BEFORE_DRAM_INIT, NBPtr, &(MemMainPtr->MemPtr->StdHeader));
  //----------------------------------------------------------------
  //  Init Phy mode
  //----------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

      // 1. Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
      // 2. Program D18F2x9C_x0002_000E_dct[3:0][PhyDisable]=0. Tester_mode=0.
      MemNPmuResetNb (NBPtr);

      // 3. According to the type of DRAM attached, program D18F2x9C_x00FFF04A_dct[3:0][MajorMode],
      //    D18F2x9C_x0002_000E_dct[3:0][G5_Mode], and D18F2x9C_x0002_0098_dct[3:0][CalG5D3].
      //    D18F2x9C_x0[3,1:0][F,7:0]1_[F,B:0]04A_dct[3:0].
      MemNSetPhyDdrModeKV (NBPtr, DRAM_TYPE_DDR3_KV);

      // Work-around for CPU A0/A1, PhyReceiverPowerMode
      if ((NBPtr->MCTPtr->LogicalCpuid.Revision & AMD_F15_KV_A0) != 0) {
        MemNPrePhyReceiverLowPowerKV (NBPtr);
      }
    }
  }

  //----------------------------------------------------------------
  //  Temporary buffer for DRAM CAD Bus Configuration
  //----------------------------------------------------------------
  if (!MemNInitDramCadBusConfigKV (NBPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  //  Program Mem Pstate dependent registers
  //----------------------------------------------------------------
  IEM_SKIP_CODE (IEM_EARLY_DCT_CONFIG) {
    // PMU required M1 settings regardless Memory Pstate disabled.
    LowestMemPstate = 1;
  }

  for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
    // When memory pstate is enabled, this loop will goes through M1 first then M0
    // Otherwise, this loop only goes through M0.
    MemNSwitchMemPstateKV (NBPtr, MemPstate);

    // By default, start up speed is DDR667 for M1
    // For M0, we need to set speed to highest possible frequency
    if (MemPstate == 0) {
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb (NBPtr, Dct);
        NBPtr->DCTPtr->Timings.Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      }
    }
    IDS_HDT_CONSOLE (MEM_FLOW, "MemClkFreq = %d MHz\n", NBPtr->DCTPtr->Timings.Speed);

    // Program SPD timings and frequency dependent settings
    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
      MemNSwitchDCTNb (NBPtr, Dct);

      if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
        IDS_HDT_CONSOLE (MEM_FLOW, "\t\tSPD timings\n");
        if (MemTAutoCycTiming3 (NBPtr->TechPtr)) {
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMemPs Reg\n");
          MemNProgramMemPstateRegD3KV (NBPtr, MemPstate);
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPlatform Spec\n");
          if (MemNPlatformSpecKV (NBPtr)) {
            MemNSetBitFieldNb (NBPtr, BFMemClkDis, 0);
            // 7. Program default CAD bus values.
            // 8. Program default data bus values.
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCAD Data Bus Cfg\n");
            MemNProgramCadDataBusD3KV (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPredriver\n");
            MemNPredriverInitKV (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMode Register initialization\n");
            MemNModeRegisterInitializationKV (NBPtr);

            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDRAM PHY Power Savings\n");
            MemNDramPhyPowerSavingsKV (NBPtr);
          }
        }
      }
    }

    MemFInitTableDrive (NBPtr, MTBeforeDInit);
  }


  //----------------------------------------------------------------
  //  Program Phy
  //----------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);

      // 4. Program general phy static configuration. See 2.10.7.3.1.
      MemNPhyGenCfgKV (NBPtr);

      // 5. Phy Voltage Level Programming. See 2.10.7.3.2.
      MemNPhyVoltageLevelKV (NBPtr);

      // 6. Program DRAM channel frequency. See 2.10.7.3.3.
      MemNProgramChannelFreqKV (NBPtr, DRAM_TYPE_DDR3_KV);

      // Step 7 and 8 are done in MemPs dependent section

      // 9. Program FIFO pointer init values. See 2.10.7.3.6.
      MemNPhyFifoConfigD3KV (NBPtr);
    }
  }

  IEM_INSERT_CODE (IEM_EARLY_DEVICE_INIT, IemEarlyDeviceInitD3KV, (NBPtr));

  //------------------------------------------------
  // Callout before Dram Init
  //------------------------------------------------
  AGESA_TESTPOINT (TpProcMemBeforeAgesaHookBeforeDramInit, &(MemMainPtr->MemPtr->StdHeader));
  CallOutIdInfo.IdField.SocketId = NBPtr->MCTPtr->SocketId;
  CallOutIdInfo.IdField.ModuleId = NBPtr->MCTPtr->DieId;
  //------------------------------------------------------------------------
  // Callout to Platform BIOS to set the VDDP/VDDR voltage based upon Bit 21
  // ProductIdentification Register (Dev18Fun3x1FC)
  //------------------------------------------------------------------------
  if (MemNGetBitFieldNb (NBPtr, BFVddpVddrLowVoltSupp)) {
    MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.Voltage = VOLT0_95;
    MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.IsValid = TRUE;
    NBPtr->DCTPtr->Timings.TargetSpeed = DDR1600_FREQUENCY;
  } else {
    MemMainPtr->MemPtr->ParameterListPtr->VddpVddrVoltage.IsValid = FALSE;
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\nCalling out to Platform BIOS on Socket %d, Module %d...\n", CallOutIdInfo.IdField.SocketId, CallOutIdInfo.IdField.ModuleId);
  AgesaHookBeforeDramInit ((UINTN) CallOutIdInfo.IdInformation, MemMainPtr->MemPtr);
  NBPtr[BSP_DIE].FamilySpecificHook[AmpVoltageDisp] (&NBPtr[BSP_DIE], NULL);
  IDS_HDT_CONSOLE (MEM_FLOW, "\nVDDIO = 1.%dV\n", (NBPtr->RefPtr->DDR3Voltage == VOLT1_5) ? 5 :
                                        (NBPtr->RefPtr->DDR3Voltage == VOLT1_35) ? 35 :
                                        (NBPtr->RefPtr->DDR3Voltage == VOLT1_25) ? 25 : 999);
  AGESA_TESTPOINT (TpProcMemAfterAgesaHookBeforeDramInit, &(NBPtr->MemPtr->StdHeader));

  //----------------------------------------------------------------------------
  // Deassert MemResetL
  //----------------------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      // Deassert Procedure:
      //   MemResetL = 0
      //   Go to LP2
      //   Go to PS0
      MemNSetBitFieldNb (NBPtr, BFMemResetL, 0);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 0);
    }
  }
  MemUWait10ns (20000, NBPtr->MemPtr);

  //----------------------------------------------------------------------------
  //  Program PMU SRAM Message Block, Initiate PMU based Dram init and training
  //----------------------------------------------------------------------------
  for (PmuImage = 0; PmuImage < MemNNumberOfPmuFirmwareImageKV (NBPtr); ++PmuImage) {
    NBPtr->PmuFirmwareImage = PmuImage;
    NBPtr->FeatPtr->LoadPmuFirmware (NBPtr);

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
        IDS_HDT_CONSOLE (MEM_STATUS, "Dct %d\n", Dct);
        IDS_HDT_CONSOLE (MEM_FLOW, "Initialize the PMU SRAM Message Block buffer\n");
        if (MemNInitPmuSramMsgBlockKV (NBPtr) == FALSE) {
          IDS_HDT_CONSOLE (MEM_FLOW, "\tNot able to initialize the PMU SRAM Message Block buffer\n");
          // Not able to initialize the PMU SRAM Message Block buffer.  Log an event.
          PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
          return AGESA_FATAL;
        }

        for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
          // When memory pstate is enabled, this loop will goes through M1 first then M0
          // Otherwise, this loop only goes through M0.
          MemNSwitchMemPstateKV (NBPtr, MemPstate);

          IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU MemPs Reg\n");
          MemNPopulatePmuSramTimingsD3KV (NBPtr);
        }

        MemNPopulatePmuSramConfigD3KV (NBPtr);
        MemNSetPmuSequenceControlKV (NBPtr);
        if (MemNWritePmuSramMsgBlockKV (NBPtr) == FALSE) {
          IDS_HDT_CONSOLE (MEM_FLOW, "\tNot able to load the PMU SRAM Message Block in to DMEM\n");
          // Not able to load the PMU SRAM Message Block in to DMEM.  Log an event.
          PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_LOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
          return AGESA_FATAL;
        }

        // Query for the calibrate completion.
        MemNPendOnPhyCalibrateCompletionKV (NBPtr);

        // Set calibration rate.
        MemNStartPmuNb (NBPtr);
      }
    }

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
        IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
        if (MemNPendOnPmuCompletionNb (NBPtr) == FALSE) {
          PutEventLog (AGESA_FATAL, MEM_ERROR_PMU_TRAINING, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
          AGESA_TESTPOINT (TpProcMemPmuFailed, &(MemMainPtr->MemPtr->StdHeader));

          IDS_OPTION_HOOK (IDS_MEM_ERROR_RECOVERY, &ErrorRecovery, &(MemMainPtr->MemPtr->StdHeader));
          if (ErrorRecovery) {
            IDS_HDT_CONSOLE (MEM_FLOW, "Chipselects that PMU failed training %x\n",MemNGetBitFieldNb (NBPtr, PmuTestFail));
            NBPtr->DCTPtr->Timings.CsTrainFail = (UINT16) MemNGetBitFieldNb (NBPtr, PmuTestFail);
            NBPtr->MCTPtr->ChannelTrainFail |= (UINT32)1 << Dct;
          } else {
            IDS_OPTION_HOOK (IDS_MEM_IGNORE_ERROR, &IgnoreErr, &(MemMainPtr->MemPtr->StdHeader));
            if (!(IgnoreErr)) {
              return AGESA_FATAL;
            }
          }
        }

        MemNRateOfPhyCalibrateKV (NBPtr);
      }
    }
  }
  //----------------------------------------------------------------
  //  De-allocate the PMU SRAM Message Block buffer.
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "De-allocate PMU SRAM Message Block buffer\n");
  if (MemNPostPmuSramMsgBlockKV (NBPtr) == FALSE) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\tNot able to free the PMU SRAM Message Block buffer\n");
    // Not able to free the PMU SRAM Message Block buffer.  Log an event.
    PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_DEALLOCATE_FOR_PMU_SRAM_MSG_BLOCK, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  //  De-allocate temporary buffer for DRAM CAD Bus Configuration
  //----------------------------------------------------------------
  if (!MemNPostDramCadBusConfigKV (NBPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // Disable chipselects that failed training
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDIMM Excludes\n");
  MemNDimmExcludesKV (NBPtr);

  //----------------------------------------------------------------
  //  Synchronize Channels
  //----------------------------------------------------------------
  MemNSyncChannelInitKV (NBPtr);

  //----------------------------------------------------------------
  //  Train MaxRdLatency
  //----------------------------------------------------------------
  IEM_SKIP_CODE (IEM_LATE_DCT_CONFIG) {
    NBPtr->TechPtr->FindMaxDlyForMaxRdLat = MemTFindMaxRcvrEnDlyTrainedByPmuByte;
    NBPtr->TechPtr->ResetDCTWrPtr = MemNResetRcvFifoKV;
    MemTTrainMaxLatency (NBPtr->TechPtr);
    // The fourth loop will restore the Northbridge P-State control register
    // to the original value.
    for (NBPtr->NbFreqChgState = 1; NBPtr->NbFreqChgState <= 4; NBPtr->NbFreqChgState++) {
      if (!MemNChangeNbFrequencyWrapUnb (NBPtr, NBPtr->NbFreqChgState) || (NBPtr->NbFreqChgState == 4)) {
        break;
      }
      MemTTrainMaxLatency (NBPtr->TechPtr);
    }
  }

  //----------------------------------------------------------------
  // Set MajorMode
  //----------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    // Work-around for CPU A0/A1, PhyReceiverPowerMode
    if ((NBPtr->MCTPtr->LogicalCpuid.Revision & AMD_F15_KV_A0) != 0) {
      MemNPostPhyReceiverLowPowerKV (NBPtr);
    }
  }

  //----------------------------------------------------------------
  //  Configure DCT for normal operation
  //----------------------------------------------------------------
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMission mode cfg\n");
      MemNConfigureDctNormalD3KV (NBPtr);

      //----------------------------------------------------------------
      //  Program turnaround timings
      //----------------------------------------------------------------
      for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
        MemNSwitchMemPstateKV (NBPtr, MemPstate);
        MemNProgramTurnaroundTimingsD3KV (NBPtr);

        //----------------------------------------------------------------
        // After Mem Pstate1 Partial Training Table values
        //----------------------------------------------------------------
        MemFInitTableDrive (NBPtr, MTAfterMemPstate1PartialTrn);
      }
    }
  }

  IEM_INSERT_CODE (IEM_LATE_DCT_CONFIG, IemLateDctConfigD3KV, (NBPtr));

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tAdditional DRAM PHY Power Savings\n");
      MemNAddlDramPhyPowerSavingsKV (NBPtr);
    }
  }

  //----------------------------------------------------------------
  //  Initialize Channels interleave address bit.
  //----------------------------------------------------------------
  MemNInitChannelIntlvAddressBitKV (NBPtr);

  //----------------------------------------------------------------
  // Assign physical address ranges for DCTs and node. Also, enable channel interleaving.
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tHT mem map\n");
  if (!NBPtr->FeatPtr->InterleaveChannels (NBPtr)) {
    MemNHtMemMapKV (NBPtr);
  }

  //----------------------------------------------------
  // If there is no dimm on the system, do fatal exit
  //----------------------------------------------------
  if (NBPtr->RefPtr->SysLimit == 0) {
    PutEventLog (AGESA_FATAL, MEM_ERROR_NO_DIMM_FOUND_ON_SYSTEM, 0, 0, 0, 0, &(MemMainPtr->MemPtr->StdHeader));
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // CpuMemTyping
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMem typing\n");
  MemNCPUMemTypingNb (NBPtr);

  IDS_OPTION_HOOK (IDS_BEFORE_DQS_TRAINING, MemMainPtr, &(MemMainPtr->MemPtr->StdHeader));

  //----------------------------------------------------------------
  // After Training Table values
  //----------------------------------------------------------------
  MemFInitTableDrive (NBPtr, MTAfterTrn);

  //----------------------------------------------------------------
  // Interleave banks
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tBank Intlv\n");
  if (NBPtr->FeatPtr->InterleaveBanks (NBPtr)) {
    if (NBPtr->MCTPtr->ErrCode == AGESA_FATAL) {
      return AGESA_FATAL;
    }
  }

  //----------------------------------------------------------------
  // After Programming Interleave registers
  //----------------------------------------------------------------
  MemFInitTableDrive (NBPtr, MTAfterInterleave);

  //----------------------------------------------------------------
  // Memory Clear
  //----------------------------------------------------------------
  AGESA_TESTPOINT (TpProcMemMemClr, &(MemMainPtr->MemPtr->StdHeader));
  if (!MemFeatMain.MemClr (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // ECC
  //----------------------------------------------------------------
  if (!MemFeatMain.InitEcc (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // C6 and ACP Engine Storage Allocation
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tC6 and ACP Engine Storage\n");
  MemNAllocateC6AndAcpEngineStorageKV (NBPtr);


  //----------------------------------------------------------------
  // UMA Allocation & UMAMemTyping
  //----------------------------------------------------------------
  AGESA_TESTPOINT (TpProcMemUMAMemTyping, &(MemMainPtr->MemPtr->StdHeader));
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tUMA Alloc\n");
  if (!MemFeatMain.UmaAllocation (MemMainPtr)) {
    return AGESA_FATAL;
  }

  //----------------------------------------------------------------
  // OnDimm Thermal
  //----------------------------------------------------------------
  if (NBPtr->FeatPtr->OnDimmThermal (NBPtr)) {
    if (NBPtr->MCTPtr->ErrCode == AGESA_FATAL) {
      return AGESA_FATAL;
    }
  }

  //----------------------------------------------------------------
  // Finalize MCT
  //----------------------------------------------------------------
  MemNFinalizeMctKV (NBPtr);
  MemFInitTableDrive (NBPtr, MTAfterFinalizeMCT);

  //----------------------------------------------------------------
  // Memory Context Save
  //----------------------------------------------------------------
  MemFeatMain.MemSave (MemMainPtr);

  //----------------------------------------------------------------
  // Memory DMI support
  //----------------------------------------------------------------
  if (!MemFeatMain.MemDmi (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  // Memory CRAT support
  //----------------------------------------------------------------
  if (!MemFeatMain.MemCrat (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  // Save memory S3 data
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tS3 Save\n");
  if (!MemMS3Save (MemMainPtr)) {
    return AGESA_CRITICAL;
  }

  //----------------------------------------------------------------
  // Switch back to DCT 0 before sending control back
  //----------------------------------------------------------------
  MemNSwitchDCTNb (NBPtr, 0);

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
