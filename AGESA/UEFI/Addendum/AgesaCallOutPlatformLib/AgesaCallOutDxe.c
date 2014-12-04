/* $NoKeywords:$ */
/**
 * @file
 *
 * Declaration Related to AGESA Call Out. and related functions.
 *
 * Contains code that declaration for AGESA Call Out..
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
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
 ******************************************************************************
 */
/*++
Module Name:

  AgesaCallOutDxe.c

Abstract:
--*/

#include "AgesaCallOutDxe.h"
#include "Filecode.h"
#define FILECODE UEFI_PROC_AGESACALLOUTDXE_FILECODE

#include EFI_PROTOCOL_CONSUMER (AmdIdsInterfaceProtocol)
#include EFI_PROTOCOL_CONSUMER (MpService)

extern EFI_BOOT_SERVICES                 *gBS;

//
// Global Data
//
STATIC UINT32       FunctionNumber;
VOID                *RelatedDataBlock;
UINT32              RelatedBlockLength;

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaHookBeforeDramInit
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      SocketIdModuleId - (SocketID << 8) | ModuleId
 *    @param[in, out] *MemData
 *
 *    @retval         AGESA_STATUS
 *
 **/
/*---------------------------------------------------------------------------------------*/
AGESA_STATUS
AgesaHookBeforeDramInit (
  IN       UINTN           SocketIdModuleId,
  IN OUT   MEM_DATA_STRUCT *MemData
  )
{
  AGESA_STATUS Status;

  Status = AGESA_SUCCESS;

  return Status;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaHookBeforeDQSTraining
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      SocketIdModuleId - (SocketID << 8) | ModuleId
 *    @param[in, out] *MemData
 *
 *    @retval         AGESA_STATUS
 *
 **/
/*---------------------------------------------------------------------------------------*/
AGESA_STATUS
AgesaHookBeforeDQSTraining (
  IN       UINTN           SocketIdModuleId,
  IN OUT   MEM_DATA_STRUCT *MemData
  )
{
  AGESA_STATUS Status;

  Status = AGESA_SUCCESS;

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaHookBeforeSelfExitRefresh
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      FcnData
 *    @param[in, out] *MemData
 *
 *    @retval         AGESA_STATUS
 *
 **/
/*---------------------------------------------------------------------------------------*/
AGESA_STATUS
AgesaHookBeforeSelfExitRefresh (
  IN       UINTN           FcnData,
  IN OUT   MEM_DATA_STRUCT *MemData
  )
{
  AGESA_STATUS Status;

  Status = AGESA_SUCCESS;

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaHookBeforeExitSelfRefresh
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  MemData
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaHookBeforeExitSelfRefresh (
  IN       UINTN           FcnData,
  IN OUT   MEM_DATA_STRUCT *MemData
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaLaunchApForGivenTask
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaLaunchApForGivenTask (
  VOID
  )
{
  AMD_INTERFACE_PARAMS  AmdInterfaceParams;
  AGESA_STATUS          AgesaStatus;
  AP_EXE_PARAMS         *ApExeParams;
  //
  // Invoke AmdAmdCpuLateInit
  //
  AmdInterfaceParams.AllocationMethod = PostMemDram;
  AmdInterfaceParams.AgesaFunctionName = AMD_LATE_RUN_AP_TASK;
  AgesaStatus = AmdCreateStruct (&AmdInterfaceParams);
  ApExeParams = (AP_EXE_PARAMS *)AmdInterfaceParams.NewStructPtr;

  ApExeParams->RelatedBlockLength = RelatedBlockLength;
  ApExeParams->RelatedDataBlock = RelatedDataBlock;
  ApExeParams->FunctionNumber = FunctionNumber;

  AgesaStatus = AmdLateRunApTask (ApExeParams);

  AmdReleaseStruct (&AmdInterfaceParams);

  return AgesaStatus;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaDoReset
 *
 * Description
 *    Dummy function, only used to fix build error of  common code link to both PEI & DXE use this callout.
 *
 *  @param[in]      ResetType
 *  @param[in,out]  *StdHeader
 *
 * @return   void
 *
 */
VOID
AgesaDoReset (
  IN       UINTN               ResetType,
  IN OUT   AMD_CONFIG_PARAMS   *StdHeader
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaRunFcnOnAp
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      ApicIdOfCore
 *  @param[in,out]  LaunchApParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaRunFcnOnAp (
  IN       UINTN               ApicIdOfCore,
  IN       AP_EXE_PARAMS       *LaunchApParams
  )
{
  AGESA_STATUS              AgesaStatus;
  EFI_STATUS             Status;
  EFI_MP_SERVICES_PROTOCOL *MpServices;
  UINTN                  CpuData_Index;
  UINTN                     NumberOfEnabledCPUs;
  UINTN                     BufferSize;
  EFI_MP_PROC_CONTEXT      ApCoreData;

  AgesaStatus = AGESA_ERROR;
  RelatedBlockLength = LaunchApParams->RelatedBlockLength;
  RelatedDataBlock = LaunchApParams->RelatedDataBlock;
  FunctionNumber = LaunchApParams->FunctionNumber;


  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);

  if (EFI_ERROR (Status)) {
    return AgesaStatus;
  }

  Status = MpServices->GetGeneralMPInfo (
                MpServices,
                0,
                0,
                &NumberOfEnabledCPUs,
                0,
                0
                );

  if (EFI_ERROR (Status)) {
    return AgesaStatus;
  }

  for (CpuData_Index = 0; CpuData_Index < NumberOfEnabledCPUs; CpuData_Index++) {
    Status = MpServices->GetProcessorContext (
                                    MpServices,
                                    CpuData_Index,
                                    &BufferSize,
                                    &ApCoreData);

    if ( ApCoreData.ApicID == (UINT32) ApicIdOfCore ) {
      break;
    }
  }

  Status = MpServices->StartupThisAP (
           MpServices,
           (EFI_AP_PROCEDURE) AgesaLaunchApForGivenTask,
           CpuData_Index,
           NULL,
           0,
           NULL
           );

  if (!EFI_ERROR (Status))  {
    AgesaStatus = AGESA_SUCCESS;
  }

  return AgesaStatus;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  AgesGetIdsData
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      Data         This parameter indicates the IDS sub-function desired.
 *  @param[in,out]  IdsCalloutData  Pointer to a structure containing requirements for the IDS function
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaGetIdsData (
  IN       UINTN                Data,
  IN OUT   IDS_CALLOUT_STRUCT   *IdsCalloutData
  )
{
  EFI_STATUS                  Status;
  AMD_IDS_INTERFACE_PROTOCOL  *AmdIdsInterfacePtr;


  if (Data == IDS_CALLOUT_INIT) {
    Status = gBS->LocateProtocol (
                    &gAmdIdsInterfaceProtocolGuid,
                    NULL,
                    &AmdIdsInterfacePtr
                    );
    if (EFI_ERROR (Status)) {
      return AGESA_UNSUPPORTED;
    }
    Status = AmdIdsInterfacePtr->Interface (AmdIdsInterfacePtr, AGESA_GET_IDS_INIT_DATA, Data, IdsCalloutData);
    if (EFI_ERROR (Status)) {
      return AGESA_UNSUPPORTED;
    }
  } else if (IDS_CALLOUT_GET_PERF_BUFFER) {
    //Platform bios pass the performance buffer address to reserved field
    //IdsCalloutData->Reserved = &PlatformPerfBuf;
    //return AGESA_SUCCESS;
  }

  IDS_EXTENDED_GET_DATA_LATE (Data, &IdsCalloutData->StdHeader);
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get VBIOS image
 *
 *
 *
 * @param[in]  FcnData              Function data
 * @param[in]  VbiosImageInfo       VBIOS image info
 * @retval     Status               Agesa status
 */

AGESA_STATUS
AgesaGetVbiosImage (
  IN       UINTN                 FcnData,
  IN OUT   GFX_VBIOS_IMAGE_INFO *VbiosImageInfo
  )
{
  AGESA_STATUS Status;
  Status = AGESA_UNSUPPORTED;
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * OEM callout function for FCH data override
 *
 *
 * @param[in]  FchData              FCH data pointer
 * @retval     Status               This feature is not supported
 */

AGESA_STATUS
AgesaFchOemCallout (
  IN      VOID              *FchData
  )
{
  AGESA_STATUS                    Status;

  FCH_DATA_BLOCK                  *FchParams;
  EFI_SETUP_UTILITY_PROTOCOL      *SetupUtility;
  SYSTEM_CONFIGURATION            *SetupVariable;

  FchParams = (FCH_DATA_BLOCK *)FchData;

  Status = gBS->LocateProtocol (&gEfiSetupUtilityProtocolGuid, NULL, &SetupUtility);
  if (EFI_ERROR (Status)) {
    return AGESA_UNSUPPORTED;
  }
  SetupVariable = (SYSTEM_CONFIGURATION*) (SetupUtility->SetupNvData);
  Status = AGESA_SUCCESS;
  //
  // SD Controller
  //

  FchParams->Sd.SdHostControllerVersion = SetupVariable->SD2_0;     // This is reference code for IBV's SETUP
                                                                    // SetupVariable come from each IBV or customer their setup variable.
                                                                    //
                                                                    // For SD Host Controller Version.
                                                                    //
                                                                    // AGESA FCH provide a parameter as below,
                                                                    //  SdHostControllerVersion = 1 --> SD 2.0
                                                                    //  SdHostControllerVersion = 2 --> SD 3.0
                                                                    //
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Optional call to the host environment interface to change the external Vref for 2D Training.
 *
 * @param[in]      SocketIdModuleId - (SocketID << 8) | ModuleId
 * @param[in,out]  MemData
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaExternal2dTrainVrefChange (
  IN       UINTN           SocketIdModuleId,
  IN OUT   MEM_DATA_STRUCT *MemData
  )
{
  AGESA_STATUS Status;

  Status = AGESA_SUCCESS;

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Call to the host environment interface to change an external Voltage
 *
 * @param[in]      SocketIdModuleId - (SocketID << 8) | ModuleId
 * @param[in,out]  *AdjustValue - Pointer to VOLTAGE_ADJUST structure
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaExternalVoltageAdjust (
  IN       UINTN           SocketIdModuleId,
  IN OUT   VOLTAGE_ADJUST *AdjustValue
  )
{
  return AGESA_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIE CLKREQ# control
 *
 *
 *
 * @param[in]      StdHeader            Standard header
 * @param[in]      FcnData              Function data
 * @param[in,out]  GnbCalloutData       Gnb callout data pointer
 * @return   The AGESA Status returned from the callout.
 */

AGESA_STATUS
AgesaGnbOemCallout (
  IN       AMD_CONFIG_PARAMS   *StdHeader,
  IN       UINTN               FcnData,
  IN OUT   VOID                *GnbCalloutData
  )
{
  return AGESA_SUCCESS;
}
