/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Adaptive S4 Initialization
 *
 * Contains code that initialized Adaptive S4 before memory init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  AdaptiveS4
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
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


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Adaptive S4 PEIM driver
 *
 * This function will set the flag if the system wakes up from Adaptive S4 and
 * RTC method is used.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmAdaptiveS4PeimEntryPoint (
  IN      CPM_PEI_FILE_HANDLE         FileHandle,
  IN      CPM_PEI_SERVICES            **PeiServices
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr;
  AMD_CPM_MAIN_TABLE                  *MainTablePtr;
  AMD_CPM_ADAPTIVE_S4_TABLE           *AdaptiveS4TablePtr;
  EFI_STATUS                          Status;
  UINT32                              Offset;
  UINT8                               BufferSignature0;
  UINT8                               BufferSignature1;
  UINT8                               BufferSignature2;
  UINT8                               BufferSignature3;

  RECORD_TIME_PEI (BeginAmdCpmAdaptiveS4PeimDriver);

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

  CpmTablePpiPtr->CommonFunction.PostCode (0xC00B);

  MainTablePtr                  = CpmTablePpiPtr->MainTablePtr;
  AdaptiveS4TablePtr            = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_ADAPTIVE_S4);

  if (AdaptiveS4TablePtr) {
    if (MainTablePtr->AdaptiveS4En == 2) {
      if (CpmTablePpiPtr->CommonFunction.GetBootMode () == CPM_BOOT_MODE_S4) {
        // RTC Method
        Offset = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * AdaptiveS4TablePtr->BufferType + AdaptiveS4TablePtr->BufferOffset;
        BufferSignature0 = CpmTablePpiPtr->CommonFunction.MmioRead8 (Offset + 0x00);
        BufferSignature1 = CpmTablePpiPtr->CommonFunction.MmioRead8 (Offset + 0x01);
        BufferSignature2 = CpmTablePpiPtr->CommonFunction.MmioRead8 (Offset + 0x02);
        BufferSignature3 = CpmTablePpiPtr->CommonFunction.MmioRead8 (Offset + 0x03);
        if (CpmTablePpiPtr->CommonFunction.IsRtcWakeup (CpmTablePpiPtr) && BufferSignature3 == 'A' && BufferSignature2 == 'S' && BufferSignature1 == '3' && BufferSignature0 == 'Y') {
          CpmTablePpiPtr->CommonFunction.MmioOr8 (Offset + 0x08, BIT1);
          CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x00, 'P');
        } else {
          CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x00, 0x00);
          CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x01, 0x00);
          CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x02, 0x00);
          CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x03, 0x00);
          CpmTablePpiPtr->CommonFunction.MmioAnd8 (Offset + 0x08, 0xFD);
        }
      } else {
        // RTC Method
        Offset = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * AdaptiveS4TablePtr->BufferType + AdaptiveS4TablePtr->BufferOffset;
        CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x00, 0x00);
        CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x01, 0x00);
        CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x02, 0x00);
        CpmTablePpiPtr->CommonFunction.MmioWrite8 (Offset + 0x03, 0x00);
        CpmTablePpiPtr->CommonFunction.MmioAnd8 (Offset + 0x08, 0xFD);
      }
    }
  }

  CpmTablePpiPtr->CommonFunction.PostCode (0xC10B);

  RECORD_TIME_PEI (EndAmdCpmAdaptiveS4PeimDriver);

  return Status;
}
