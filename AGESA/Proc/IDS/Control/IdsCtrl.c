/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug Option Backend Routines
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
/*****************************************************************************
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
 ******************************************************************************
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "Ids.h"
#include "Topology.h"
#include "amdlib.h"
#include "mm.h"
#include "mn.h"
#include "cpuRegisters.h"
#include "heapManager.h"
#include "cpuFamilyTranslation.h"
#include "GeneralServices.h"
#include "IdsLib.h"
#include "IdsNvToCmos.h"
#include "Filecode.h"
#ifdef __IDS_EXTENDED__
  #include IDS_EXT_INCLUDE (IdsInternalLib)
#endif
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_IDS_CONTROL_IDSCTRL_FILECODE

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */


extern CONST IDS_FAMILY_FEAT_STRUCT* ROMDATA IdsControlFeats[];


/**
 * IDS option hooking function dispatcher.
 *
 * This is the top level interface for IDS option Backend code.
 *
 * @param[in]     IdsOption IDS indicator value, see @ref AGESA_IDS_OPTION
 * @param[in,out] DataPtr   Data pointer.
 * @param[in,out] StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 **/
IDS_STATUS
AmdIdsCtrlDispatcher (
  IN       AGESA_IDS_OPTION IdsOption,
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_NV_ITEM *IdsNvPtr;
  IDS_STATUS   ReturnFlag;
  IDS_STATUS ExtendedFlag;

  IdsNvPtr  = NULL;
  ReturnFlag = IDS_SUCCESS;

  ASSERT (StdHeader != NULL);
  if (AmdGetIdsNvTable ((VOID **) &IdsNvPtr, StdHeader) != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (IDS_TRACE , "IDS initialize\n");
    AmdIdsCtrlInitialize (StdHeader);
    AmdGetIdsNvTable ((VOID **) &IdsNvPtr, StdHeader);
  }

  if (IdsNvPtr != NULL) {
    ReturnFlag = IdsParseFeatTbl (IdsOption, IdsControlFeats, DataPtr, IdsNvPtr, StdHeader);
    ExtendedFlag = IDS_EXTENDED_HOOK (IdsOption, DataPtr, IdsNvPtr, StdHeader);
    if (ExtendedFlag != IDS_SUCCESS) {
      ReturnFlag = IDS_UNSUPPORTED;
    }
  }
  return ReturnFlag;
}

/**
 * Ids code for parse IDS feat table.
 *
 * Feat table in IDS is used to decribe the IDS support feat and its according family,handler.
 *
 * @param[in]     PIdsFeatTbl point to Ids Feat table
 * @param[in]     IdsOption IDS indicator value, see @ref AGESA_IDS_OPTION
 * @param[in,out] DataPtr   Data pointer.
 * @param[in]     IdsNvPtr   Ids Nvram pointer.
 * @param[in,out] StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 **/
IDS_STATUS
IdsParseFeatTbl (
  IN       AGESA_IDS_OPTION IdsOption,
  IN       CONST IDS_FAMILY_FEAT_STRUCT * PIdsFeatTbl[],
  IN OUT   VOID *DataPtr,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT16 i;
  AGESA_STATUS Tmpsts;
  CPU_LOGICAL_ID CpuLogicalId;
  BOOLEAN No_Check_Bsp;
  CONST IDS_FAMILY_FEAT_STRUCT *PIdsFeat;
  IDS_STATUS ReturnFlag;

  ReturnFlag = IDS_SUCCESS;

  for (i = 0; PIdsFeatTbl[i] != NULL; i++) {
    PIdsFeat = PIdsFeatTbl[i];
    //Does specified IdsOption reached
    if (PIdsFeat->IdsOption == IdsOption) {
      //check if bsp only
      if (PIdsFeat->IsBsp) {
        No_Check_Bsp = 0;
      } else {
        No_Check_Bsp = 1;
      }
      if (No_Check_Bsp || IsBsp (StdHeader, &Tmpsts)) {
        //Does Family Match required
        GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);
        if ((CpuLogicalId.Family) & (PIdsFeat->CpuFamily)) {
          //Excute the code for specific Ids Feat
          IDS_HDT_CONSOLE (IDS_TRACE, "\tIDS Excute HookPoint [%x] Start\n", IdsOption);
          ReturnFlag = PIdsFeat->pf_idsoption (DataPtr, StdHeader, IdsNvPtr);
          IDS_HDT_CONSOLE (IDS_TRACE, "\tIDS Excute HookPoint [%x] End\n", IdsOption);
        }
      }
    }
  }
  return ReturnFlag;
}

/**
 *
 *  IDS Object Initialization
 *
 *  Initializer routine that will be invoked by the wrapper to initialize
 *  the data buffer in heap for the IDS object. It includes IDS control
 *  structure, IDS mem table and IDS GRA table.
 *
 *  @param[in,out] StdHeader    The Pointer of IDS Initial Parameter
 *
 *  @retval AGESA_SUCCESS       Success to init IDS Object.
 *  @retval AGESA_ERROR         Fail to init IDS Object.
 *
 **/

#ifdef IDS_HEAP_2STAGES
  #define IDS_HEAP_PERSIST_EARLY  HEAP_LOCAL_CACHE
  #define IDS_HEAP_ASSERTION_LATE
#else
  #define IDS_HEAP_PERSIST_EARLY  HEAP_SYSTEM_MEM
  #define IDS_HEAP_ASSERTION_LATE ASSERT(FALSE)
#endif

AGESA_STATUS
AmdIdsCtrlInitialize (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  AGESA_STATUS status;
  AGESA_STATUS IgnoreStatus;
  UINT16 NvTblSize;
  UINT16 i;
  IDS_NV_ITEM *TempNvTable;
  IDS_NV_ITEM *NewNvTable;
  IDS_CONTROL_STRUCT *IdsCtrlPtr;
  IDS_CALLOUT_STRUCT IdsCalloutData;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  UINT16 MemTblSize;
  UINT8 HeapPersist;

  NvTblSize = 0;
  MemTblSize = 0;
  HeapPersist = HEAP_SYSTEM_MEM;
  //Heap status with HEAP_LOCAL_CACHE, will allocate heap with HEAP_LOCAL_CACHE
  //with HEAP_TEMP_MEM  HEAP_SYSTEM_MEM  HEAP_DO_NOT_EXIST_ANYMORE  HEAP_S3_RESUME
  // with allocate with HEAP_SYSTEM_MEM
  if (StdHeader->HeapStatus == HEAP_LOCAL_CACHE) {
    MemTblSize = IDS_MAX_MEM_ITEMS;
    HeapPersist = IDS_HEAP_PERSIST_EARLY;
  } else if ((StdHeader->HeapStatus == HEAP_DO_NOT_EXIST_YET) || (StdHeader->HeapStatus == HEAP_DO_NOT_EXIST_ANYMORE)) {
    return AGESA_ERROR;
  } else {
    IDS_HEAP_ASSERTION_LATE;
  }

  //Create a temp buffer
  AllocHeapParams.RequestedBufferSize = IDS_NUM_NV_ITEM * sizeof (IDS_NV_ITEM);
  AllocHeapParams.BufferHandle = IDS_TEMP_DATA_HANDLE;
  AllocHeapParams.Persist = HeapPersist;
  if (HeapAllocateBuffer (&AllocHeapParams, (AMD_CONFIG_PARAMS *) StdHeader) == AGESA_SUCCESS) {
    TempNvTable = (IDS_NV_ITEM *) AllocHeapParams.BufferPtr;
    IdsCalloutData.IdsNvPtr = TempNvTable;
    IdsCalloutData.StdHeader = *StdHeader;
    IdsCalloutData.Reserved = FALSE;
    //init IDS_CALLOUT_STRUCT before calling out, give NVITEM default value
    for (i = AGESA_IDS_EXT_ID_START; i < IDS_NUM_NV_ITEM; i++) {
      TempNvTable[i].IdsNvId = i;
      TempNvTable[i].IdsNvValue = AGESA_IDS_DFT_VAL;
    }

    AGESA_TESTPOINT (TpIfBeforeGetIdsData, StdHeader);
    if (AgesaGetIdsData (IDS_CALLOUT_INIT, &IdsCalloutData) == AGESA_SUCCESS) {
      IDS_HDT_CONSOLE (IDS_TRACE , "Get IDS options from CBS Done\n");
      //Caculate the size of data passed in
      i = 0;
      while (TempNvTable[i++].IdsNvId != AGESA_IDS_NV_END) {
      }
      NvTblSize = i;

      AllocHeapParams.RequestedBufferSize = sizeof (IDS_CONTROL_STRUCT);
      AllocHeapParams.RequestedBufferSize += NvTblSize  * sizeof (IDS_NV_ITEM);
      AllocHeapParams.RequestedBufferSize += MemTblSize * sizeof (MEM_TABLE_ALIAS);
      AllocHeapParams.RequestedBufferSize += IDS_EXTENDED_HEAP_SIZE;
      AllocHeapParams.BufferHandle = IDS_CONTROL_HANDLE;
      AllocHeapParams.Persist = HeapPersist;

      //
      // Allocate data buffer in heap
      //
      if (HeapAllocateBuffer (&AllocHeapParams, (AMD_CONFIG_PARAMS *) StdHeader) == AGESA_SUCCESS) {
        //
        // Initialize IDS Data Buffer
        //
        IdsCtrlPtr = (IDS_CONTROL_STRUCT *) AllocHeapParams.BufferPtr;
        IdsCtrlPtr->IgnoreIdsDefault = (BOOLEAN) IdsCalloutData.Reserved;
        IdsCtrlPtr->IdsHeapMemSize = AllocHeapParams.RequestedBufferSize;
        IdsCtrlPtr->IdsNvTableOffset = sizeof (IDS_CONTROL_STRUCT);
        IdsCtrlPtr->IdsMemTableOffset = IdsCtrlPtr->IdsNvTableOffset + NvTblSize  * sizeof (IDS_NV_ITEM);
        IdsCtrlPtr->IdsExtendOffset = IdsCtrlPtr->IdsMemTableOffset + MemTblSize * sizeof (MEM_TABLE_ALIAS);
        //Copy the data from temp heap to new heap
        NewNvTable = (IDS_NV_ITEM *) (AllocHeapParams.BufferPtr + IdsCtrlPtr->IdsNvTableOffset);
        for (i = 0; i < NvTblSize ; i++) {
          NewNvTable[i] = TempNvTable[i];
          IDS_HDT_CONSOLE (IDS_TRACE , "\tIDS_ID (%X) = %X\n", NewNvTable[i].IdsNvId, NewNvTable[i].IdsNvValue);
        }
        status = AGESA_SUCCESS;
      } else {
        status = AGESA_ERROR;
      }
    } else {
      //Check if we can get AP data via CMOS interface
      if (!IsBsp (StdHeader, &IgnoreStatus)) {
        status = IDS_AP_GET_NV_FROM_CMOS (StdHeader);
      } else {
        IDS_HDT_CONSOLE (IDS_TRACE , "Get IDS options from CBS Fail\n");
        status = AGESA_ERROR;
      }
    }
    //Deallocate the temp buffer
    HeapDeallocateBuffer (IDS_TEMP_DATA_HANDLE, StdHeader);
  } else {
    IDS_HDT_CONSOLE (IDS_TRACE , "Allocate Temp Heap Data Fail\n");
    status = AGESA_ERROR;
  }
  AGESA_TESTPOINT (TpIfAfterGetIdsData, StdHeader);

  return status;
}
/**
 *  IDS Backend Function for Target Pstate
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_EARLY_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubTargetPstate (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS tarpst;
  CPU_SPECIFIC_SERVICES   *FamilySpecificServices;

  IDS_NV_READ_SKIP (tarpst, AGESA_IDS_NV_TARGET_PSTATE, IdsNvPtr, StdHeader) {
    GetCpuServicesOfCurrentCore (&FamilySpecificServices, StdHeader);
    FamilySpecificServices->TransitionPstate (FamilySpecificServices, (UINT8) tarpst, (BOOLEAN) FALSE, StdHeader);
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for HdtOut
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_EARLY_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubHdtOut (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS idsvalue;

//set HDTOUT En/Dis
  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_HDTOUT, IdsNvPtr, StdHeader) {
//if HDTOUT set to enable, set the corresponding DR2 flag to 0x99cc
    if (idsvalue == 1) {
      LibAmdWriteCpuReg (DR2_REG, 0x99cc);
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Power down mode
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubPowerDownCtrl (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AMD_POST_PARAMS *PostParamsPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  IDS_STATUS idsvalue;
  MEM_DATA_STRUCT * memdataptr;

  PostParamsPtr = (AMD_POST_PARAMS *)DataPtr;
  memdataptr = PostParamsPtr->MemConfig.MemData;
  RefPtr = memdataptr->ParameterListPtr;

  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_MEMORY_POWER_DOWN, IdsNvPtr, StdHeader) {
    //if the idsvalue isn't auto do the override
    if (idsvalue < (IDS_STATUS)0x2) {
      RefPtr->EnablePowerDown = (BOOLEAN) idsvalue;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  Backend Function for IDS Option Hook Point: IDS_UCODE
 *
 *  This function is used to disable UCode Installation if IDS Option disables ucode.
 *  The method is to force the number of total patches to ZERO.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubUCode (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS status;
  IDS_STATUS NvValue;
  UINT8 ** pUcodeptr;

  pUcodeptr = (UINT8 **) DataPtr;
  status = IDS_SUCCESS;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_UCODE, IdsNvPtr, StdHeader) {
    //Disabled
    if (NvValue == 0) {
      status = IDS_UNSUPPORTED;
    }
  }

  IDS_EXTENDED_CODE (
    IdsGetBvmUcodeBase (pUcodeptr, StdHeader);
  )

  return status;
}

/**
 *  IDS Backend Function for Post P-State
 *
 *  This function is used to set Post P-State which are CPU specifically.
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_EARLY_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubPostPState (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AMD_CPU_EARLY_PARAMS *PCpuEarlyParams;
  IDS_STATUS idsvalue;
  UINT8 curpstatesnum;

  PCpuEarlyParams = (AMD_CPU_EARLY_PARAMS *)DataPtr;
  curpstatesnum = IdsGetNumPstatesFamCommon (StdHeader);
  idsvalue = AmdIdsNvReader (AGESA_IDS_NV_POSTPSTATE, IdsNvPtr, StdHeader);

  if (idsvalue < (IDS_STATUS) (curpstatesnum + 3)) {
    switch (idsvalue) {
    case (IDS_STATUS) 0x0:
      // Auto
      break;
    case (IDS_STATUS) 0x1:
      // Maximum Performance
      PCpuEarlyParams->MemInitPState = 0;
      break;
    case (IDS_STATUS) 0x2:
      // Minimum Performance
      PCpuEarlyParams->MemInitPState = curpstatesnum - 1;
      break;
    default:
      PCpuEarlyParams->MemInitPState = (UINT8) (idsvalue - 3);
      break;
    }
  }
  return IDS_SUCCESS;
}



/**
 *  IDS Backend Function for Memory Mode Unganged
 *
 *  This function is used to override Memory Mode Unganged.
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubGangingMode (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS GangingMode;
  IDS_NV_READ_SKIP (GangingMode, AGESA_IDS_NV_DCT_GANGING_MODE, IdsNvPtr, StdHeader) {
    *((UINT8 *)DataPtr) = (UINT8) GangingMode;
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Power Down Mode
 *
 *  This function is used to override Power Down Mode.
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubPowerDownMode (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS PowerDownMode;
  PowerDownMode = AmdIdsNvReader (AGESA_IDS_NV_MEMORY_POWER_DOWN_MODE, IdsNvPtr, StdHeader);
  if (PowerDownMode < (IDS_STATUS)0x2) {
    *((UINT8 *) DataPtr) = (UINT8)PowerDownMode;
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for All Memory Clks Enable
 *
 *  This function is used to override All Memory Clks Enable
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubAllMemClkEn (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS AllMemClkEn;
  IDS_STATUS IDSStatus;

  IDSStatus = AmdIdsNvReader (AGESA_IDS_NV_DRAMCON , IdsNvPtr, StdHeader);
  if (IDSStatus != 1) {
    return IDS_SUCCESS;
  }

  AllMemClkEn = AmdIdsNvReader (AGESA_IDS_NV_ALL_MEMCLKS , IdsNvPtr, StdHeader);
  if (AllMemClkEn < (IDS_STATUS)0x2) {
    *((UINT8 *) DataPtr) = (UINT8)AllMemClkEn;
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Select Platform Power Policy
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 */
IDS_STATUS
IdsSubPowerPolicyOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  PLATFORM_CONFIGURATION *PlatformConfig;
  IDS_STATUS NvValue;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_POWER_POLICY, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IDS_POWER_POLICY_PERFORMANCE:
      PlatformConfig->PlatformProfile.PlatformPowerPolicy = Performance;
      break;
    case IDS_POWER_POLICY_POWER:
      PlatformConfig->PlatformProfile.PlatformPowerPolicy = BatteryLife;
      break;
    case IDS_POWER_POLICY_AUTO:
      PlatformConfig->PlatformProfile.PlatformPowerPolicy = Auto;
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return IDS_SUCCESS;
}
