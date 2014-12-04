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

  AgesaCallOutPei.c

Abstract:
--*/
#include "AgesaCallOutPei.h"
#include "Filecode.h"
#define FILECODE UEFI_PROC_AGESACALLOUTPEI_FILECODE
#include <Library\PeiServicesTablePointerLib.h>
#include <Ppi\AmdResetRequestPpi.h>
#include <Ppi\AmdPlatformDimmSpdPpi.h>
#include <Ppi\AmdIdsInterfacePpi.h>
#include <Ppi\Smbus2.h>

//
// ResetRequest PPI
//
AMD_PEI_RESET_REQUEST_PPI mResetRequest = {
  AMD_PEI_RESET_REQUEST_REV,
  AMD_WARM_RESET,
  NULL,
  0
};

STATIC  EFI_PEI_PPI_DESCRIPTOR mAmdPeiResetRequestPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiResetRequestPpiGuid,
  &mResetRequest
};

AGESA_STATUS
AgesaReadSpd (
  IN       UINTN                    FcnData,
  IN OUT   AGESA_READ_SPD_PARAMS    *ReadSpd
  )
{
  EFI_STATUS                Status;
  EFI_PEI_SERVICES          **PeiServices;
  EFI_PEI_SMBUS2_PPI        *Smbus2Ppi;

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  Status  = (*PeiServices)->LocatePpi (
            PeiServices,
            &gEfiPeiSmbus2PpiGuid,
            0,
            NULL,
            &Smbus2Ppi
            );

  if (EFI_ERROR (Status)) {
    return AGESA_ERROR;
  }

  // Convert ReadSpd->MemChannelId to correct SPD Device Address.
  // Just return AGESA_FATAL for NULL driver.
  return AGESA_FATAL;

}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaHookBeforeDramInit
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      SocketIdModuleId - (SocketId << 8) | ModuleId
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
 *    @param[in]      SocketIdModuleId - (SocketId << 8) | ModuleId
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
 *  AgesaAllocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  AllocParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaAllocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *AllocParams
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaLocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  LocateParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaLocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *LocateParams
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaDeAllocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  DeallocParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaDeallocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *DeallocParams
  )
{
  return AGESA_SUCCESS;
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
  IN        UINTN               ApicIdOfCore,
  IN        AP_EXE_PARAMS       *LaunchApParams
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Call the host environment interface to provide a user hook opportunity.
 *
 * @param[in]      Data         This parameter indicates the IDS sub-function desired.
 * @param[in,out]  IdsCalloutData  Pointer to a structure containing requirements for the IDS function
 *
 * @return   The AGESA Status returned from the callout.
 *
 */


AGESA_STATUS
AgesaGetIdsData  (
  IN       UINTN              Data,
  IN OUT   IDS_CALLOUT_STRUCT *IdsCalloutData
  )
{
  EFI_STATUS                      Status;
  PEI_AMD_IDS_INTERFACE_PPI       *AmdIdsInterfacePtr;
  EFI_PEI_SERVICES                **PeiServices;

  if (Data == IDS_CALLOUT_INIT) {
    PeiServices = (EFI_PEI_SERVICES **) IdsCalloutData->StdHeader.ImageBasePtr;

    if (PeiServices == NULL) {
      return AGESA_UNSUPPORTED;
    }

    Status = (**PeiServices).LocatePpi (
               PeiServices,
               &gAmdPeiIdsInterfacePpiGuid,
               0,
               NULL,
               &AmdIdsInterfacePtr
               );
    if (EFI_ERROR (Status)) {
      return AGESA_UNSUPPORTED;
    }
    Status = AmdIdsInterfacePtr->Interface (PeiServices, AGESA_GET_IDS_INIT_DATA, Data, IdsCalloutData);
    if (EFI_ERROR (Status)) {
      return AGESA_UNSUPPORTED;
    }
  } else if (Data == IDS_CALLOUT_POWER_PLAN_INIT) {
    //Override Power Plan register here
    //Skip Power Plan init cpu code, with set SkipPwrPlan to true
    //SkipPwrPlan = (BOOLEAN *)IdsCalloutData->Reserved;
    // *SkipPwrPlan = TRUE;
  } else if (IDS_CALLOUT_GET_PERF_BUFFER) {
    return AGESA_UNSUPPORTED;
  }
  IDS_EXTENDED_GET_DATA_EARLY (Data, &IdsCalloutData->StdHeader);
  return AGESA_SUCCESS;
}


/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaDoReset
 *
 * Description
 *    Call the host environment interface to perform reset.
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
  EFI_PEI_SERVICES      **PeiServices;
  UINT8                 Value;
  EFI_STATUS            Status;
  WARM_RESET_REQUEST    Request;

  PeiServices = (EFI_PEI_SERVICES **)StdHeader->ImageBasePtr;

  GetWarmResetFlag (StdHeader, &Request);
  Request.RequestBit = FALSE;
  Request.StateBits = Request.PostStage;
  SetWarmResetFlag (StdHeader, &Request);

  //
  // Perform the RESET based upon the ResetType. In case of
  // WARM_RESET_WHENVER and COLD_RESET_WHENEVER, the request will go to
  // AmdResetManager. During the critical condition, where reset is required
  // immediately, the reset will be invoked directly by writing 0x04 to port
  // 0xCF9 (Reset Port).
  //
  switch (ResetType) {
  case WARM_RESET_WHENEVER:
  case COLD_RESET_WHENEVER:
    (**PeiServices).InstallPpi (PeiServices, &mAmdPeiResetRequestPpi);
    break;
  case WARM_RESET_IMMEDIATELY:
  case COLD_RESET_IMMEDIATELY:
    Status = (**PeiServices).ResetSystem (PeiServices);
    //
    // We should not come here if PeiResetSystem is present.
    // In case it is missing and gets reported through
    // EFI_NOT_AVAILABLE_YET, perform the force reset
    //
    if (EFI_ERROR (Status)) {
      Value = 0x06;
      LibAmdIoWrite (AccessWidth8, 0xCf9, &Value, StdHeader);
    }
    break;
  default:
    (**PeiServices).InstallPpi (PeiServices, &mAmdPeiResetRequestPpi);
    break;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIE slot reset control
 *
 *
 *
 * @param[in]  FcnData              Function data
 * @param[in]  ResetInfo            Reset information
 * @retval     Status               This feature is not supported
 */

AGESA_STATUS
AgesaPcieSlotResetControl (
  IN      UINTN                 FcnData,
  IN      PCIe_SLOT_RESET_INFO  *ResetInfo
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
  AGESA_STATUS Status;
  Status = AGESA_UNSUPPORTED;
  return  Status;
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
 *  AgesaHeapRebase
 *
 * Description
 *    Call the host environment interface to provide a address
 *    which heap content will be temporarily stored in
 *
 *  @param[in]      FcnData
 *  @param[in]      RebaseParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaHeapRebase (
  IN       UINTN                 FcnData,
  IN OUT   AGESA_REBASE_PARAMS   *RebaseParams
  )
{
  return AGESA_SUCCESS;
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
