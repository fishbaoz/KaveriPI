/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of family 15h support
 *
 * This file generates the defaults tables for family 15h processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
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
 *
 ***************************************************************************/

#ifndef _OPTION_FAMILY_15H_INSTALL_H_
#define _OPTION_FAMILY_15H_INSTALL_H_

#include "OptionFamily15hEarlySample.h"
#include "cpuFamilyTranslation.h"

/*
 * Pull in family specific services based on entry point
 */

/*
 * Common Family 15h routines
 */

/*
 * Install family 15h model 10h - 1Fh support
 */
#ifdef OPTION_FAMILY15H_TN
  #if OPTION_FAMILY15H_TN == TRUE
    extern F_CPU_SET_CFOH_REG SetF15TnCacheFlushOnHaltRegister;
    extern CONST REGISTER_TABLE ROMDATA F15TnPciRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnPciWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnMsrRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnSharedMsrRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnSharedMsrCuRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnSharedMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15TnPerNodeMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15PciRegisterTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15MsrRegisterTable;

    /**
     * Compute unit and Compute unit primary determination table.
     *
     * The two fields from the compute unit status hardware register can be used to determine whether
     * even number cores are primary or all cores are primary.  It can be extended if it is
     * decided to have other configs as well.  The other logically possible value sets are BitMapMapping,
     * but they are currently not supported by the processor.
     */
    CONST COMPUTE_UNIT_MAP ROMDATA TopologyFam15TnComputeUnitMapping[] =
    {
      {1, 1,'x', 'x', EvenCoresMapping},                                     ///< 1 Compute Unit with 2 cores
      {3, 3, 'x', 'x', EvenCoresMapping},                                     ///< 2 Compute Units both with 2 Cores
      {1, 0, 'x', 'x', AllCoresMapping},                                      ///< 1 Compute Unit with 1 core
      {3, 0, 'x', 'x', AllCoresMapping},                                      ///< 2 Compute Units both with 1 Core
      {TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, MaxComputeUnitMapping}   ///< End
    };


    #if USES_REGISTER_TABLES == TRUE
      CONST REGISTER_TABLE ROMDATA *F15TnRegisterTablesAfterApLaunch[] =
      {
        #if BASE_FAMILY_MSR == TRUE
          &F15MsrRegisterTable,
        #endif
        // the end.
        NULL
      };

      CONST REGISTER_TABLE ROMDATA *F15TnRegisterTablesBeforeApLaunch[] =
      {
        #if BASE_FAMILY_PCI == TRUE
          &F15PciRegisterTableBeforeApLaunch,
        #endif
        #if MODEL_SPECIFIC_PCI == TRUE
          &F15TnPciRegisterTable,
          &F15TnPciWorkaroundTable,
        #endif
        #if MODEL_SPECIFIC_MSR == TRUE
          &F15TnMsrRegisterTable,
          &F15TnMsrWorkaroundTable,
          &F15TnSharedMsrRegisterTable,
          &F15TnSharedMsrCuRegisterTable,
          &F15TnSharedMsrWorkaroundTable,
          &F15TnPerNodeMsrWorkaroundTable,
        #endif
        // the end.
        NULL
      };
    #endif

    #include "OptionCpuSpecificServicesInstallReset.h"
    #define CpuSrvcTableName                            cpuF15TnServices

    #define CpuSrvcDisablePstate                        F15DisablePstate
    #define CpuSrvcTransitionPstate                     F15TransitionPstate
    #define CpuSrvcGetProcIddMax                        F15TnGetProcIddMax
    #define CpuSrvcGetTscRate                           F15GetTscRate
    #define CpuSrvcGetCurrentNbFrequency                F15TnGetCurrentNbFrequency
    #define CpuSrvcGetMinMaxNbFrequency                 F15TnGetMinMaxNbFrequency
    #define CpuSrvcGetNbPstateInfo                      F15TnGetNbPstateInfo
    #define CpuSrvcIsNbCofInitNeeded                    F15CommonGetNbCofVidUpdate
    #define CpuSrvcGetNbIddMax                          F15TnGetNbIddMax
    #define CpuSrvcLaunchApCore                         F15LaunchApCore
    #define CpuSrvcGetNumberOfPhysicalCores             F15TnGetNumberOfPhysicalCores
    #define CpuSrvcGetApCoreNumber                      F15TnGetApCoreNumber
    #define CpuSrvcCoreIdPositionInInitialApicId        F15CpuAmdCoreIdPositionInInitialApicId
    #define CpuSrvcSetWarmResetFlag                     F15SetAgesaWarmResetFlag
    #define CpuSrvcGetWarmResetFlag                     F15GetAgesaWarmResetFlag
    #define CpuSrvcGetMicroCodePatchesStruct            GetF15TnMicroCodePatchesStruct
    #define CpuSrvcGetMicrocodeEquivalenceTable         GetF15TnMicrocodeEquivalenceTable
    #define CpuSrvcGetCacheInfo                         GetF15CacheInfo
    #define CpuSrvcGetSysPmTableStruct                  GetF15TnSysPmTable
    #define CpuSrvcGetWheaInitData                      GetF15WheaInitData
    #define CpuSrvcIsNbPstateEnabled                    F15TnIsNbPstateEnabled
    #define CpuSrvcDoEntryTypeMsrRegister               SetRegisterForMsrEntry
    #define CpuSrvcDoEntryTypePciRegister               SetRegisterForPciEntry
    #define CpuSrvcDoEntryTypeFamSpecificWorkaround     SetRegisterForFamSpecificWorkaroundEntry
    #define CpuSrvcDoEntryTypeProfileFixup              SetRegisterForPerformanceProfileEntry
    #define CpuSrvcDoEntryTypeCoreCountsPciRegister     SetRegisterForCoreCountsPerformanceEntry
    #define CpuSrvcDoEntryTypeProcCountsPciRegister     SetRegisterForProcessorCountsEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsPciRegister SetRegisterForComputeUnitCountsEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsMsr         SetMsrForComputeUnitCountsEntry
    #define CpuSrvcRegisterTableListAfterApLaunch       (REGISTER_TABLE **) F15TnRegisterTablesAfterApLaunch
    #define CpuSrvcRegisterTableListBeforeApLaunch      (REGISTER_TABLE **) F15TnRegisterTablesBeforeApLaunch
    #define CpuSrvcComputeUnitMap                       (COMPUTE_UNIT_MAP *) &TopologyFam15TnComputeUnitMapping
    #define CpuSrvcInitCacheDisabled                    InitCacheEnabled
    #define CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable GetF15TnEarlyInitBeforeApLaunchOnCoreTable
    #define CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable GetF15TnEarlyInitAfterApLaunchOnCoreTable
    #define CpuSrvcPatchLoaderIsSharedByCU              TRUE

    #include "OptionCpuSpecificServicesInstall.h"
    INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);

    #define TN_SOCKETS 1
    #define TN_MODULES 1
    #define TN_RECOVERY_SOCKETS 1
    #define TN_RECOVERY_MODULES 1
    extern F_CPU_GET_SUBFAMILY_ID_ARRAY GetF15TnLogicalIdAndRev;
    #define OPT_F15_TN_ID (PF_CPU_GET_SUBFAMILY_ID_ARRAY) GetF15TnLogicalIdAndRev,
    #ifndef ADVCFG_PLATFORM_SOCKETS
      #define ADVCFG_PLATFORM_SOCKETS TN_SOCKETS
    #else
      #if ADVCFG_PLATFORM_SOCKETS < TN_SOCKETS
        #undef ADVCFG_PLATFORM_SOCKETS
        #define ADVCFG_PLATFORM_SOCKETS TN_SOCKETS
      #endif
    #endif
    #ifndef ADVCFG_PLATFORM_MODULES
      #define ADVCFG_PLATFORM_MODULES TN_MODULES
    #else
      #if ADVCFG_PLATFORM_MODULES < TN_MODULES
        #undef ADVCFG_PLATFORM_MODULES
        #define ADVCFG_PLATFORM_MODULES TN_MODULES
      #endif
    #endif

    #if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
      #define F15_TN_UCODE_6100
      #define F15_TN_UCODE_6101

      #if AGESA_ENTRY_INIT_EARLY == TRUE
        #if OPTION_EARLY_SAMPLES == TRUE
          extern  CONST UINT8 ROMDATA CpuF15TnId6100MicrocodePatch [];
          #undef F15_TN_UCODE_6100
          #define F15_TN_UCODE_6100 CpuF15TnId6100MicrocodePatch,
        #endif
        extern  CONST UINT8 ROMDATA CpuF15TnId6101MicrocodePatch [];
        #undef F15_TN_UCODE_6101
        #define F15_TN_UCODE_6101 CpuF15TnId6101MicrocodePatch,

      #endif

      CONST UINT8 ROMDATA *CpuF15TnMicroCodePatchArray[] =
      {
        F15_TN_UCODE_6101
        F15_TN_UCODE_6100
        NULL
      };

      CONST UINT8 ROMDATA CpuF15TnNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15TnMicroCodePatchArray) / sizeof (CpuF15TnMicroCodePatchArray[0])) - 1);
    #endif

    #if OPTION_EARLY_SAMPLES == TRUE
      extern F_F15_TN_ES_LOAD_MCU_PATCH F15TnEarlySamplesLoadMicrocodePatch;

      CONST F15_TN_ES_MCU_PATCH ROMDATA F15TnEarlySampleLoadMcuPatch =
      {
        F15TnEarlySamplesLoadMicrocodePatch
      };
    #else
      CONST F15_TN_ES_MCU_PATCH ROMDATA F15TnEarlySampleLoadMcuPatch =
      {
        (PF_F15_TN_ES_LOAD_MCU_PATCH) LoadMicrocodePatch
      };
    #endif

    #if OPTION_EARLY_SAMPLES == TRUE
      extern F_F15_TN_ES_PSTATE_POWER_WORKAROUND F15TnEarlySamplePstatePowerWorkaround;

      CONST F15_TN_ES_CORE_SUPPORT ROMDATA F15TnEarlySampleCoreSupport =
      {
        #if AGESA_ENTRY_INIT_EARLY == TRUE
          F15TnEarlySamplePstatePowerWorkaround,
        #else
          (PF_F15_TN_ES_PSTATE_POWER_WORKAROUND) CommonAssert,
        #endif
      };
    #else
      CONST F15_TN_ES_CORE_SUPPORT ROMDATA F15TnEarlySampleCoreSupport =
      {
        #if AGESA_ENTRY_INIT_EARLY == TRUE
          (PF_F15_TN_ES_PSTATE_POWER_WORKAROUND) CommonVoid,
        #else
          (PF_F15_TN_ES_PSTATE_POWER_WORKAROUND) CommonAssert,
        #endif
      };
    #endif

    #if OPTION_EARLY_SAMPLES == TRUE
      extern F_F15_TN_ES_IS_NB_PSTATE_EN F15TnEarlySampleIsNbPstateEn;
      extern F_F15_TN_ES_SKIP_NB_PS_LO F15TnEarlySampleSkipNbPsLo;

      CONST F15_TN_ES_NB_PSTATE ROMDATA F15TnEarlySampleNbPstateSupport =
      {
        #if AGESA_ENTRY_INIT_EARLY == TRUE
          F15TnEarlySampleIsNbPstateEn,
          F15TnEarlySampleSkipNbPsLo,
        #else
          (PF_F15_TN_ES_IS_NB_PSTATE_EN) CommonAssert,
          (PF_F15_TN_ES_SKIP_NB_PS_LO) CommonAssert,
        #endif
      };
    #else
      CONST F15_TN_ES_NB_PSTATE ROMDATA F15TnEarlySampleNbPstateSupport =
      {
        #if AGESA_ENTRY_INIT_EARLY == TRUE
          (PF_F15_TN_ES_IS_NB_PSTATE_EN) CommonReturnTrue,
          (PF_F15_TN_ES_SKIP_NB_PS_LO) CommonReturnFalse,
        #else
          (PF_F15_TN_ES_IS_NB_PSTATE_EN) CommonAssert,
          (PF_F15_TN_ES_SKIP_NB_PS_LO) CommonAssert,
        #endif
      };
    #endif

    CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F15TnLogicalIdTable[] =
    {
      OPT_F15_TN_ID
    };

    #define OPT_F15_TN_ID_TABLE {0x00610F00ul, {AMD_FAMILY_15_TN, AMD_F15_UNKNOWN}, F15TnLogicalIdTable, (sizeof (F15TnLogicalIdTable) / sizeof (F15TnLogicalIdTable[0]))},
    #define OPT_F15_TN_CPU {AMD_FAMILY_15_TN, &cpuF15TnServices},

  #else  //  OPTION_FAMILY15H_TN == TRUE
    #define OPT_F15_TN_CPU
    #define OPT_F15_TN_ID
    #define OPT_F15_TN_ID_TABLE
    #if defined (BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT) && (BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT)
      #if OPTION_EARLY_SAMPLES == TRUE
        CONST F15_TN_ES_MCU_PATCH ROMDATA F15TnEarlySampleLoadMcuPatch =
        {
          NULL
        };
      #endif
      CONST UINT8 ROMDATA *CpuF15TnMicroCodePatchArray[] =
      {
        NULL
      };
      CONST UINT8 ROMDATA CpuF15TnNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15TnMicroCodePatchArray) / sizeof (CpuF15TnMicroCodePatchArray[0])) - 1);
      #if OPTION_EARLY_SAMPLES == TRUE
        CONST F15_TN_ES_CORE_SUPPORT ROMDATA F15TnEarlySampleCoreSupport =
        {
          NULL
        };
      #endif
      #if OPTION_EARLY_SAMPLES == TRUE
        CONST F15_TN_ES_NB_PSTATE ROMDATA F15TnEarlySampleNbPstateSupport =
        {
          NULL
        };
      #endif
    #endif
  #endif  //  OPTION_FAMILY15H_TN == TRUE
#else  //  defined (OPTION_FAMILY15H_TN)
  #define OPT_F15_TN_CPU
  #define OPT_F15_TN_ID
  #define OPT_F15_TN_ID_TABLE
#endif  //  defined (OPTION_FAMILY15H_TN)

/*
 * Install family 15h model 30h - 3Fh support
 */
#ifdef OPTION_FAMILY15H_KV
  #if OPTION_FAMILY15H_KV == TRUE
    extern F_CPU_SET_CFOH_REG SetF15KvCacheFlushOnHaltRegister;
    extern CONST REGISTER_TABLE ROMDATA F15KvPciRegisterTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15KvPciRegisterTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15KvPciWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15KvMsrRegisterTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15KvMsrRegisterTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15KvMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15KvSharedMsrRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15KvSharedMsrCuRegisterTable;
    extern CONST REGISTER_TABLE ROMDATA F15KvSharedMsrWorkaroundTable;
    extern CONST REGISTER_TABLE ROMDATA F15PciRegisterTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15MsrRegisterTable;

    /**
     * Compute unit and Compute unit primary determination table.
     *
     * The two fields from the compute unit status hardware register can be used to determine whether
     * even number cores are primary or all cores are primary.  It can be extended if it is
     * decided to have other configs as well.  The other logically possible value sets are BitMapMapping,
     * but they are currently not supported by the processor.
     */
    CONST COMPUTE_UNIT_MAP ROMDATA TopologyFam15KvComputeUnitMapping[] =
    {
      {1, 1, 'x', 'x', EvenCoresMapping},                                     ///< 1 Compute Unit with 2 cores
      {3, 3, 'x', 'x', EvenCoresMapping},                                     ///< 2 Compute Units both with 2 Cores
      {1, 0, 'x', 'x', AllCoresMapping},                                      ///< 1 Compute Unit with 1 core
      {3, 0, 'x', 'x', AllCoresMapping},                                      ///< 2 Compute Units both with 1 Core
      {TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, MaxComputeUnitMapping}   ///< End
    };


    #if USES_REGISTER_TABLES == TRUE
      CONST REGISTER_TABLE ROMDATA *F15KvRegisterTablesAfterApLaunch[] =
      {
        #if BASE_FAMILY_PCI == TRUE
          &F15PciRegisterTableAfterApLaunch,
        #endif
        #if MODEL_SPECIFIC_PCI == TRUE
          &F15KvPciRegisterTableAfterApLaunch,
          &F15KvPciWorkaroundTable,
        #endif
        #if BASE_FAMILY_MSR == TRUE
          &F15MsrRegisterTable,
        #endif
        #if MODEL_SPECIFIC_MSR == TRUE
          &F15KvMsrRegisterTableAfterApLaunch,
          &F15KvMsrWorkaroundTable,
          &F15KvSharedMsrRegisterTable,
          &F15KvSharedMsrCuRegisterTable,
          &F15KvSharedMsrWorkaroundTable,
        #endif
        // the end.
        NULL
      };

      CONST REGISTER_TABLE ROMDATA *F15KvRegisterTablesBeforeApLaunch[] =
      {
        #if MODEL_SPECIFIC_PCI == TRUE
          &F15KvPciRegisterTableBeforeApLaunch,
        #endif
        #if MODEL_SPECIFIC_MSR == TRUE
          &F15KvMsrRegisterTableBeforeApLaunch,
        #endif
        // the end.
        NULL
      };
    #endif

    #include "OptionCpuSpecificServicesInstallReset.h"
    #define CpuSrvcTableName                            cpuF15KvServices

    #define CpuSrvcDisablePstate                        F15DisablePstate
    #define CpuSrvcTransitionPstate                     F15TransitionPstate
    #define CpuSrvcGetProcIddMax                        F15KvGetProcIddMax
    #define CpuSrvcGetTscRate                           F15GetTscRate
    #define CpuSrvcGetCurrentNbFrequency                F15KvGetCurrentNbFrequency
    #define CpuSrvcGetMinMaxNbFrequency                 F15KvGetMinMaxNbFrequency
    #define CpuSrvcGetNbPstateInfo                      F15KvGetNbPstateInfo
    #define CpuSrvcIsNbCofInitNeeded                    F15CommonGetNbCofVidUpdate
    #define CpuSrvcGetNbIddMax                          F15KvGetNbIddMax
    #define CpuSrvcLaunchApCore                         F15LaunchApCore
    #define CpuSrvcGetNumberOfPhysicalCores             F15KvGetNumberOfPhysicalCores
    #define CpuSrvcGetApCoreNumber                      F15KvGetApCoreNumber
    #define CpuSrvcCoreIdPositionInInitialApicId        F15CpuAmdCoreIdPositionInInitialApicId
    #define CpuSrvcSetWarmResetFlag                     F15SetAgesaWarmResetFlag
    #define CpuSrvcGetWarmResetFlag                     F15GetAgesaWarmResetFlag
    #define CpuSrvcGetMicroCodePatchesStruct            GetF15KvMicroCodePatchesStruct
    #define CpuSrvcGetMicrocodeEquivalenceTable         GetF15KvMicrocodeEquivalenceTable
    #define CpuSrvcGetCacheInfo                         GetF15CacheInfo
    #define CpuSrvcGetSysPmTableStruct                  GetF15KvSysPmTable
    #define CpuSrvcGetWheaInitData                      GetF15WheaInitData
    #define CpuSrvcIsNbPstateEnabled                    F15KvIsNbPstateEnabled
    #define CpuSrvcDoEntryTypeMsrRegister               SetRegisterForMsrEntry
    #define CpuSrvcDoEntryTypePciRegister               SetRegisterForPciEntry
    #define CpuSrvcDoEntryTypeFamSpecificWorkaround     SetRegisterForFamSpecificWorkaroundEntry
    #define CpuSrvcDoEntryTypeProfileFixup              SetRegisterForPerformanceProfileEntry
    #define CpuSrvcDoEntryTypeCoreCountsPciRegister     SetRegisterForCoreCountsPerformanceEntry
    #define CpuSrvcDoEntryTypeProcCountsPciRegister     SetRegisterForProcessorCountsEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsPciRegister SetRegisterForComputeUnitCountsEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsMsr         SetMsrForComputeUnitCountsEntry
    #define CpuSrvcRegisterTableListAfterApLaunch       (REGISTER_TABLE **) F15KvRegisterTablesAfterApLaunch
    #define CpuSrvcRegisterTableListBeforeApLaunch      (REGISTER_TABLE **) F15KvRegisterTablesBeforeApLaunch
    #define CpuSrvcComputeUnitMap                       (COMPUTE_UNIT_MAP *) &TopologyFam15KvComputeUnitMapping
    #define CpuSrvcInitCacheDisabled                    InitCacheEnabled
    #define CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable GetF15KvEarlyInitBeforeApLaunchOnCoreTable
    #define CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable GetF15KvEarlyInitAfterApLaunchOnCoreTable
    #define CpuSrvcPatchLoaderIsSharedByCU              TRUE

    #include "OptionCpuSpecificServicesInstall.h"
    INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);

    #define KV_SOCKETS 1
    #define KV_MODULES 1
    #define KV_RECOVERY_SOCKETS 1
    #define KV_RECOVERY_MODULES 1
    extern F_CPU_GET_SUBFAMILY_ID_ARRAY GetF15KvLogicalIdAndRev;
    #define OPT_F15_KV_ID (PF_CPU_GET_SUBFAMILY_ID_ARRAY) GetF15KvLogicalIdAndRev,
    #ifndef ADVCFG_PLATFORM_SOCKETS
      #define ADVCFG_PLATFORM_SOCKETS KV_SOCKETS
    #else
      #if ADVCFG_PLATFORM_SOCKETS < KV_SOCKETS
        #undef ADVCFG_PLATFORM_SOCKETS
        #define ADVCFG_PLATFORM_SOCKETS KV_SOCKETS
      #endif
    #endif
    #ifndef ADVCFG_PLATFORM_MODULES
      #define ADVCFG_PLATFORM_MODULES KV_MODULES
    #else
      #if ADVCFG_PLATFORM_MODULES < KV_MODULES
        #undef ADVCFG_PLATFORM_MODULES
        #define ADVCFG_PLATFORM_MODULES KV_MODULES
      #endif
    #endif

    #if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
      #define F15_KV_UCODE_6300
      #define F15_KV_UCODE_6301

      #if AGESA_ENTRY_INIT_EARLY == TRUE
        #if OPTION_EARLY_SAMPLES == TRUE
          extern  CONST UINT8 ROMDATA CpuF15KvId6300MicrocodePatch [];
          #undef F15_KV_UCODE_6300
          #define F15_KV_UCODE_6300 CpuF15KvId6300MicrocodePatch,
        #endif

        extern  CONST UINT8 ROMDATA CpuF15KvId6301MicrocodePatch [];
        #undef F15_KV_UCODE_6301
        #define F15_KV_UCODE_6301 CpuF15KvId6301MicrocodePatch,
      #endif

      CONST UINT8 ROMDATA *CpuF15KvMicroCodePatchArray[] =
      {
        F15_KV_UCODE_6301
        F15_KV_UCODE_6300
        NULL
      };

      CONST UINT8 ROMDATA CpuF15KvNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15KvMicroCodePatchArray) / sizeof (CpuF15KvMicroCodePatchArray[0])) - 1);
    #endif

    #if OPTION_EARLY_SAMPLES == TRUE
    #endif
    CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F15KvLogicalIdTable[] =
    {
      OPT_F15_KV_ID
    };

    #define OPT_F15_KV_ID_TABLE {0x00630F00ul, {AMD_FAMILY_15_KV, AMD_F15_UNKNOWN}, F15KvLogicalIdTable, (sizeof (F15KvLogicalIdTable) / sizeof (F15KvLogicalIdTable[0]))},
    #define OPT_F15_KV_CPU {AMD_FAMILY_15_KV, &cpuF15KvServices},

  #else  //  OPTION_FAMILY15H_KV == TRUE
    #define OPT_F15_KV_CPU
    #define OPT_F15_KV_ID
    #define OPT_F15_KV_ID_TABLE
    #if defined (BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT) && (BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT)
      CONST UINT8 ROMDATA *CpuF15KvMicroCodePatchArray[] =
      {
        NULL
      };
      CONST UINT8 ROMDATA CpuF15KvNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15KvMicroCodePatchArray) / sizeof (CpuF15KvMicroCodePatchArray[0])) - 1);
    #endif
  #endif  //  OPTION_FAMILY15H_KV == TRUE
#else  //  defined (OPTION_FAMILY15H_KV)
  #define OPT_F15_KV_CPU
  #define OPT_F15_KV_ID
  #define OPT_F15_KV_ID_TABLE
#endif  //  defined (OPTION_FAMILY15H_KV)


// Family 15h maximum base address is 48 bits. Limit BLDCFG to 48 bits, if appropriate.
#if (FAMILY_MMIO_BASE_MASK < 0xFFFF000000000000ull)
  #undef  FAMILY_MMIO_BASE_MASK
  #define FAMILY_MMIO_BASE_MASK (0xFFFF000000000000ull)
#endif


#undef OPT_F15_ID_TABLE
#define OPT_F15_ID_TABLE  OPT_F15_TN_ID_TABLE  OPT_F15_KV_ID_TABLE

#undef OPT_F15_TABLE
#define OPT_F15_TABLE   OPT_F15_TN_CPU OPT_F15_KV_CPU


#if OPTION_FS1_SOCKET_SUPPORT == TRUE
  #define F15_FS1_BRANDSTRING1 NULL,
  #define F15_FS1_BRANDSTRING2 NULL,
#else
  #define F15_FS1_BRANDSTRING1
  #define F15_FS1_BRANDSTRING2
#endif
#if OPTION_FP3_SOCKET_SUPPORT == TRUE
  #define F15_FP3_BRANDSTRING1 NULL,
  #define F15_FP3_BRANDSTRING2 NULL,
#else
  #define F15_FP3_BRANDSTRING1
  #define F15_FP3_BRANDSTRING2
#endif
#if OPTION_FM2r2_SOCKET_SUPPORT == TRUE
  #define F15_FM2r2_BRANDSTRING1 NULL,
  #define F15_FM2r2_BRANDSTRING2 NULL,
#else
  #define F15_FM2r2_BRANDSTRING1
  #define F15_FM2r2_BRANDSTRING2
#endif
#if OPTION_FM2_SOCKET_SUPPORT == TRUE
  #define F15_FM2_BRANDSTRING1 NULL,
  #define F15_FM2_BRANDSTRING2 NULL,
#else
  #define F15_FM2_BRANDSTRING1
  #define F15_FM2_BRANDSTRING2
#endif
#if OPTION_FP2_SOCKET_SUPPORT == TRUE
  #define F15_FP2_BRANDSTRING1 NULL,
  #define F15_FP2_BRANDSTRING2 NULL,
#else
  #define F15_FP2_BRANDSTRING1
  #define F15_FP2_BRANDSTRING2
#endif


#if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  CONST CPU_BRAND_TABLE ROMDATA *F15BrandIdString1Tables[] =
  {
    F15_FS1_BRANDSTRING1
    F15_FP3_BRANDSTRING1
    F15_FM2r2_BRANDSTRING1
    F15_FM2_BRANDSTRING1
    F15_FP2_BRANDSTRING1
    NULL
  };

  CONST UINT8 F15BrandIdString1TableCount = (sizeof (F15BrandIdString1Tables) / sizeof (F15BrandIdString1Tables[0]));

  CONST CPU_BRAND_TABLE ROMDATA *F15BrandIdString2Tables[] =
  {
    F15_FS1_BRANDSTRING2
    F15_FP3_BRANDSTRING2
    F15_FM2r2_BRANDSTRING2
    F15_FM2_BRANDSTRING2
    F15_FP2_BRANDSTRING2
    NULL
  };

  CONST UINT8 F15BrandIdString2TableCount = (sizeof (F15BrandIdString2Tables) / sizeof (F15BrandIdString2Tables[0]));
#endif

CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F15LogicalIdTable[] =
{
  OPT_F15_TN_ID
  OPT_F15_KV_ID
};

#endif  // _OPTION_FAMILY_15H_INSTALL_H_
