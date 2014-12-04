/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * Contains code that used for Ids Internal PEI phase
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
#include "PiPei.h"
#include <Library\DebugLib.h>
#include <Library/AgesaUefiLib.h>
#include <AGESA.h>
#include <amdlib.h>
#include <Ids.h>
#include <IdsLib.h>
#include <internal\IdsInternalLib.h>
#include <heapManager.h>
#include <cpuFamilyTranslation.h>
#include <cpuFamRegisters.h>
#include <cpuServices.h>

#include <Ppi\AmdResetRequestPpi.h>
#include <Ppi\Reset.h>

#define LOWEST_PRIORITY ((UINTN)(-1))
#define FILECODE 0xE813

EFI_STATUS
EFIAPI
BvmPeiAmdResetCallback (
  IN EFI_PEI_SERVICES     **PeiServices
  );

STATIC AMD_PEI_RESET_REQUEST_PPI mBVMAmdPeiResetRequestPpi =
{
  AMD_PEI_RESET_REQUEST_REV,
  AMD_WARM_RESET,
  &BvmPeiAmdResetCallback,
  LOWEST_PRIORITY
};

STATIC EFI_PEI_PPI_DESCRIPTOR mBVMAmdPeiResetRequestPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiResetRequestPpiGuid,
  &mBVMAmdPeiResetRequestPpi
};

/*---------------------------------------------------------------------------------------*/
/**
 *  BVMPeiRegisterResetNotify
 *
 * Description
 *    PPI for Amd reset manager
 *
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]   PeiServices    PEI service
 *
 */

VOID
BVMPeiRegisterResetNotify (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       EFI_PEI_SERVICES           **PeiServices
  )
{
  AMD_PEI_RESET_REQUEST_PPI  *TempPpiPtr;

  if (!IsWarmReset (StdHeader)) {
    //Check if anyone request Warmreset
    if ((**PeiServices).LocatePpi (PeiServices, &gAmdPeiResetRequestPpiGuid, 0, NULL, &TempPpiPtr) == EFI_SUCCESS) {
      (**PeiServices).InstallPpi (PeiServices, &mBVMAmdPeiResetRequestPpiList);
    }
  }

}

/*---------------------------------------------------------------------------------------*/
/**
 *  BvmPeiAmdResetCallback
 *
 * Description
 *    PPI for Amd reset manager
 *
 *  @param[in]   PeiServices    PEI service
 *
 *  @retval EFI_SUCCESS         Function is called successfully.
 */
EFI_STATUS
EFIAPI
BvmPeiAmdResetCallback (
  IN EFI_PEI_SERVICES     **PeiServices
  )
{
  AMD_CONFIG_PARAMS StdHeader;

  if (NULL != PeiServices) {

    StdHeader.ImageBasePtr = (UINT32) (PeiServices);
    StdHeader.HeapStatus = HEAP_TEMP_MEM;
    StdHeader.HeapBasePtr = HeapGetBaseAddress (&StdHeader);
    if (StdHeader.HeapBasePtr != (UINT64) NULL) {
      IdsBvmOptionHook (BVM_PLATFORM_TP_BEFORE_WARMRESET, NULL, &StdHeader);
    }
  }

  return EFI_SUCCESS;
}
/*---------------------------------------------------------------------------------------*/
/**
 *  UefiIdsIntEarlyAfter
 *
 * Description
 *    call Register PPI routine
 *
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 */
VOID
UefiIdsIntEarlyAfter (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  EFI_PEI_SERVICES                        **PeiServices;

  PeiServices = (EFI_PEI_SERVICES **) StdHeader->ImageBasePtr;
  if (PeiServices != NULL) {
    BVMPeiRegisterResetNotify (StdHeader, PeiServices);
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 *  IdsInternalGetDataPei
 *
 * Description
 *    Internal hook for AmdGetIdsData
 *
 *  @param[in]   Data    Indentify the input function ID.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 */
VOID
IdsInternalGetDataPei (
  IN       UINTN              Data,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  if (Data == IDS_CALLOUT_INT_EARLY_AFTER) {
    UefiIdsIntEarlyAfter (StdHeader);
  }
}
