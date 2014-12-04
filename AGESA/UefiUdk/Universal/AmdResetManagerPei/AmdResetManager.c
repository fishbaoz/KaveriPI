/* $NoKeywords:$ */
  /**
 * @file
 *
 * AMD CPU POST API, and related functions.
 *
 * Contains code that initialized the CPU after memory init.
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
 * ***************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library\DebugLib.h>

#include "AmdResetManager.h"
#include "Filecode.h"
#define FILECODE UEFI_PEI_AMDRESETMANAGER_AMDRESETMANAGER_FILECODE

// Consumed PPIs
// Produced PPIs

#include <Ppi\CpuIo.h>
#include <Ppi\Reset.h>
#include <Ppi\AmdResetRequestPpi.h>
#include <Ppi\AmdResetManagerPpi.h>

/*----------------------------------------------------------------------------------------
 *                     L O C A L   D A T A
 *----------------------------------------------------------------------------------------
 */
STATIC EFI_PEI_PPI_DESCRIPTOR  mPpiListResetManagerPpiGuid = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPeiResetManagerPpiGuid,
  NULL
};


/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdResetManagerEntryPoint
 *
 *  Description:
 *     Entry point for the reset manager
 *     The reset manager currently supports up to 5 callbacks functions
 *
 *  Parameters:
 *    @param[in] *FileHandle
 *    @param[in] **PeiServices
 *
 *    @retval    EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AmdResetManagerEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  AMD_PEI_RESET_REQUEST_PPI  *ResetReqPpi[MAX_RESET_REQUEST];
  AMD_PEI_RESET_REQUEST_PPI  *TempPpiPtr;
  UINTN                      NumPpi;
  UINTN                      NumCallBack;
  UINTN                      i;
  EFI_STATUS                 Status;

  //
  // See if a reset is needed
  //
  Status = (*PeiServices)->LocatePpi (PeiServices,
                                      &gAmdPeiResetRequestPpiGuid,
                                      0,
                                      NULL,
                                      &TempPpiPtr);
  if (EFI_SUCCESS == Status) {
    // at least one reset has been requested, add it to the list
    // and initialize the ppi list variables
    if (NULL != TempPpiPtr->Notify) {
      ResetReqPpi[0] = TempPpiPtr;
      NumCallBack = 1;
    } else {
      NumCallBack = 0;
    }
    NumPpi = 1;
  } else {
    // reset not requested
    // publish the reset manager PPI to let other module know all resets have happened
    Status = (*PeiServices)->InstallPpi (PeiServices, &mPpiListResetManagerPpiGuid);
    return (Status);
  }

  //
  // Find all the reset request PPI and sort callback by priority
  //
  while (EFI_SUCCESS == Status) {
    // locate the reset request ppi
    Status = (*PeiServices)->LocatePpi (PeiServices,
                                        &gAmdPeiResetRequestPpiGuid,
                                        NumPpi,
                                        NULL,
                                        &TempPpiPtr);
    if (EFI_SUCCESS == Status) {
      // increment the PPI counter
      NumPpi++;

      // if a call back is requested put the PPi in the right position in the
      // callback array.
      if (NULL == TempPpiPtr->Notify) {
        // no callback request
        continue;
      }

      // put the call back at the right position using
      ResetReqPpi[NumCallBack] = TempPpiPtr;
      for (i = NumCallBack; i > 0; i--) {
        if (ResetReqPpi[i]->NotifyPriority < ResetReqPpi[i - 1]->NotifyPriority) {
          TempPpiPtr = ResetReqPpi[i];
          ResetReqPpi[i] = ResetReqPpi[i - 1];
          ResetReqPpi[i - 1] = TempPpiPtr;
        }
      }

      // increments number of callbacks
      NumCallBack++;
    }
  }

  //
  // issue the different callbacks according to their priority
  //
  for (i = 0; i < NumCallBack; i++) {
    Status = (ResetReqPpi[i]->Notify) (( EFI_PEI_SERVICES   **)PeiServices);
  }

  //
  // issue the reset
  //
  Status = (*PeiServices)->ResetSystem (PeiServices);
  return (Status);
}

