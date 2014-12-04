/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#include <AmdCpmPei.h>


EFI_STATUS
EFIAPI
AmdCpmEcInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmEcInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmGpioInitFinishedPpiGuid,
  AmdCpmEcInitPeim
};

EFI_STATUS
EFIAPI
AmdCpmEcInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS              Status;
  AMD_CPM_EC_INIT_TABLE   *EcInitTablePtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_TABLE_PPI       *CpmTablePpiPtr;
  UINT8                   Data;

  Status = (*PeiServices)->LocatePpi (
                                      PeiServices,
                                      &gAmdCpmTablePpiGuid,
                                      0,
                                      NULL,
                                      &CpmTablePpiPtr
                                      );
  if (EFI_ERROR (Status)) {
    return  Status;
  }

  CpmTablePpiPtr->CommonFunction.PostCode (0xC004);

  MainTablePtr    = CpmTablePpiPtr->MainTablePtr;
  EcInitTablePtr  = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_EC_INIT);

  if (EcInitTablePtr) {

    Data = (UINT8) MainTablePtr->Ec.Config.AcDcSwitchEn << 7;
    Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xEA, &Data);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (MainTablePtr->Ec.Config.AcDcSwitchEn) {
      Data = EcInitTablePtr->AcTimer;
      Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xEB, &Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      Data = EcInitTablePtr->DcTimer;
      Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xEC, &Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }

    if (MainTablePtr->Ec.Config.ColdBootCyclingEn) {
      Data = EcInitTablePtr->OnTimer;
      Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xEF, &Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
    Data = (UINT8)MainTablePtr->Ec.Config.ColdBootCyclingEn;
    Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xED, &Data);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if (MainTablePtr->Ec.Config.S5PlusEn) {
    if (CpmTablePpiPtr->CommonFunction.GetStrap (CpmTablePpiPtr) & BIT2) {
      Status = CpmTablePpiPtr->CommonFunction.KbcWrite (0x60, 0xA2, NULL);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  CpmTablePpiPtr->CommonFunction.PostCode (0xC104);

  return Status;
}

EFI_STATUS
EFIAPI
AmdCpmEcInitPeimEntryPoint (
  IN        CPM_PEI_FILE_HANDLE         FileHandle,
  IN        CPM_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS                      Status;

  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmEcInitPeimNotify);

  return EFI_SUCCESS;
}
