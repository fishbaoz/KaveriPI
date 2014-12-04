/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Adaptive S4 Initialization
 *
 * Contains CPM code to perform Adaptive S4 initialization in SMM
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

#include <AmdCpmSmm.h>

CPM_DEFINE_gBS;
CPM_DEFINE_gSmst;

AMD_CPM_TABLE_PROTOCOL *gCpmTableSmmProtocol;


EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AdaptiveS4Callback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SX_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Adaptive S4 SMM driver
 *
 * This function registers the callback function before go to S4.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
AmdCpmAdaptiveS4SmmEntryPoint (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                          Status;
  CPM_SMM_SX_DISPATCH_PROTOCOL        *SxDispatch;
  CPM_SMM_SX_DISPATCH_CONTEXT         SxContext;
  EFI_HANDLE                          SxHandle;

  If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status) {

    //
    // Initialize global variables
    //
    Status = gBS->LocateProtocol (
                    &gAmdCpmTableSmmProtocolGuid,
                    NULL,
                    &gCpmTableSmmProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    gCpmTableSmmProtocol->CommonFunction.PostCode (0xE00B);

    //
    //  Locate SMM Sx dispatch protocol
    //
    Status = CpmSmmLocateProtocol (
                    &gCpmSmmSxDispatchProtocolGuid,
                    NULL,
                    &SxDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    SxContext.Type  = SxS4;
    SxContext.Phase = SxEntry;
    Status = SxDispatch->Register (
                    SxDispatch,
                    AdaptiveS4Callback,
                    &SxContext,
                    &SxHandle
                    );

    gCpmTableSmmProtocol->CommonFunction.PostCode (0xE10B);

  }

  RECORD_TIME_DXE (EndAmdCpmAdaptiveS4SmmDriver);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * S4 SMI Handler for Adaptive S4
 *
 * This function programs RTC alarm registers if the OS will go to Adaptive S4 RTC mode
 * and RTC alarm register is not set by OS.
 *
 */
EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     AdaptiveS4Callback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SX_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  AMD_CPM_ADAPTIVE_S4_TABLE *CpmAdaptiveS4TablePtr;
  UINT32                    Offset;
  UINT8                     Flag;
  UINT8                     BufferSignature0;
  UINT8                     BufferSignature1;
  UINT8                     BufferSignature2;
  UINT8                     BufferSignature3;
  UINT8                     RtcValue[8];
  UINT8                     TimeOut;

  gCpmTableSmmProtocol->CommonFunction.PostCode (0xE20B);

  CpmAdaptiveS4TablePtr = gCpmTableSmmProtocol->CommonFunction.GetTablePtr (gCpmTableSmmProtocol, CPM_SIGNATURE_ADAPTIVE_S4);

  if (CpmAdaptiveS4TablePtr && gCpmTableSmmProtocol->MainTablePtr->AdaptiveS4En == 2) {
    Offset = gCpmTableSmmProtocol->MainTablePtr->AcpiMemIoBaseAddr + 0x100 * CpmAdaptiveS4TablePtr->BufferType + CpmAdaptiveS4TablePtr->BufferOffset;
    Flag   = gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x08);
    BufferSignature0 = gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x00);
    BufferSignature1 = gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x01);
    BufferSignature2 = gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x02);
    BufferSignature3 = gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x03);

    if (BufferSignature3 == 'A' && BufferSignature2 == 'S' && BufferSignature1 == '3' && BufferSignature0 == '4' && (Flag & BIT0)) {
      RtcValue[0] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x00);
      RtcValue[1] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x01);
      RtcValue[2] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x02);
      RtcValue[3] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x03);
      RtcValue[4] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x04);
      RtcValue[5] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x05);
      RtcValue[6] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x07);
      RtcValue[7] = gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x0D) && 0x3F;
      Flag = ((gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x0B) & 0x20) == 0x00);
      if (((RtcValue[7] != 0) && (RtcValue[7] < RtcValue[6])) || ((RtcValue[5] & 0xC0) == 0x80) || ((RtcValue[3] & 0xC0) == 0x80) || ((RtcValue[1] & 0xC0) == 0x80)) {
        Flag = 1;
      } else if (((RtcValue[5] & 0xC0) != 0xC0) && RtcValue[5] > 23) {
        Flag = 1;
      } else if (((RtcValue[3] & 0xC0) != 0xC0) && RtcValue[3] > 59) {
        Flag = 1;
      } else if (((RtcValue[1] & 0xC0) != 0xC0) && RtcValue[1] > 59) {
        Flag = 1;
      } else {
        TimeOut = 1;
        if (((RtcValue[5] & 0xC0) == 0xC0) || (RtcValue[5] > RtcValue[4])) {
          TimeOut = 0;
        }
        if (TimeOut && (((RtcValue[3] & 0xC0) == 0xC0) || (RtcValue[3] > RtcValue[2]))) {
          TimeOut = 0;
        }
        if (TimeOut && (((RtcValue[1] & 0xC0) == 0xC0) || (RtcValue[1] > RtcValue[0]))) {
          TimeOut = 0;
        }
        Flag = Flag | TimeOut;
      }

      if (Flag) {
        gCpmTableSmmProtocol->CommonFunction.MmioAnd8 (
                                                    Offset + 0x08,
                                                    0xFD);  // Clean Flag
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x0B,
                                                    gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x0B) & 0xDF
                                                    );      // Disable alarm
        gCpmTableSmmProtocol->CommonFunction.SetAcpi (
                                                    gCpmTableSmmProtocol,
                                                    0x03,
                                                    gCpmTableSmmProtocol->CommonFunction.GetAcpi (gCpmTableSmmProtocol, 0x03) & 0xFB
                                                    );     // Disable alarm
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x01,
                                                    gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x00)
                                                    );    // Set second alarm
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x03,
                                                    gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x09)
                                                    );    // Set minute alarm
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x05,
                                                    gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x0A)
                                                    );    // Set hour alarm
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x0D,
                                                    (gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x0D) & 0xC0) | gCpmTableSmmProtocol->CommonFunction.MmioRead8 (Offset + 0x0B)
                                                    );    // Set day alarm

        gCpmTableSmmProtocol->CommonFunction.GetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x0C
                                                    );    // Clear alarm
        gCpmTableSmmProtocol->CommonFunction.SetAcpi (
                                                    gCpmTableSmmProtocol,
                                                    0x01,
                                                    0x04
                                                    );    // Clear alarm
        gCpmTableSmmProtocol->CommonFunction.SetRtc (
                                                    gCpmTableSmmProtocol,
                                                    0x0B,
                                                    gCpmTableSmmProtocol->CommonFunction.GetRtc (gCpmTableSmmProtocol, 0x0B) | 0x20
                                                    );    // Enable alarm
        gCpmTableSmmProtocol->CommonFunction.SetAcpi (
                                                    gCpmTableSmmProtocol,
                                                    0x03,
                                                    gCpmTableSmmProtocol->CommonFunction.GetAcpi (gCpmTableSmmProtocol, 0x03) | 0x04
                                                    );    // Enable alarm

        gCpmTableSmmProtocol->CommonFunction.MmioWrite8 (Offset + 0x00, 'Y');
      }
    } else {
      gCpmTableSmmProtocol->CommonFunction.MmioWrite8 (Offset + 0x00, 0x00);
      gCpmTableSmmProtocol->CommonFunction.MmioWrite8 (Offset + 0x01, 0x00);
      gCpmTableSmmProtocol->CommonFunction.MmioWrite8 (Offset + 0x02, 0x00);
      gCpmTableSmmProtocol->CommonFunction.MmioWrite8 (Offset + 0x03, 0x00);
    }
  }
  gCpmTableSmmProtocol->CommonFunction.PostCode (0xE30B);

  return EFI_SUCCESS;
}
