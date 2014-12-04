/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM GPIO Initialization
 *
 * Contains CPM code to perform GPIO Init in SMM
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  GpioInit
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

AMD_CPM_TABLE_PROTOCOL                  *gCpmTableSmmProtocol;


EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     GpioInitS3RestoreCallback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM GPIO Init SMM driver
 *
 * This function registers the SMI handlers to set internal PCIe clock when resume
 * from S3.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
AmdCpmGpioInitSmmEntryPoint (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                          Status;
  CPM_SMM_SW_DISPATCH_PROTOCOL        *SwDispatch;
  CPM_SMM_SW_DISPATCH_CONTEXT         SwContext;
  EFI_HANDLE                          SwHandle;

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

    gCpmTableSmmProtocol->CommonFunction.PostCode (0xE005);
    //
    //  Locate SMM SW dispatch protocol
    //
    Status = CpmSmmLocateProtocol (
                    &gCpmSmmSwDispatchProtocolGuid,
                    NULL,
                    &SwDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = CFG_CPM_ACPI_RESTORE_SW_SMI;
    Status = SwDispatch->Register (
                    SwDispatch,
                    GpioInitS3RestoreCallback,
                    &SwContext,
                    &SwHandle
                    );

    gCpmTableSmmProtocol->CommonFunction.PostCode (0xE105);
  }
  RECORD_TIME_DXE (EndAmdCpmGpioInitSmmDriver);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIE Clock Init
 *
 * This function sets internal PCIe clock.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */

VOID
CpmPcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr
  )
{
  AMD_CPM_PCIE_CLOCK_TABLE            *CpmPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *TablePtr;
  UINT8                               SlotStatus;
  BOOLEAN                             ClockType;
  AMD_CPM_EXPRESS_CARD_TABLE          *ExpressCardTablePtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE        *ZeroPowerOddTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE          *SaveContextTablePtr;
  UINT32                              PcieSlotStatus;

  CpmPcieClockTablePtr                = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_CLOCK);
  SaveContextTablePtr                 = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  ClockType                           = (BOOLEAN) CpmTableProtocolPtr->CommonFunction.GetStrap (CpmTableProtocolPtr) & BIT1;

  if (ClockType && CpmPcieClockTablePtr && CpmTableProtocolPtr->MainTablePtr->UnusedGppClkOffEn) {
    PcieSlotStatus = (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x03);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x02);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x01);
    PcieSlotStatus = (PcieSlotStatus << 8) + (UINT32) CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x00);
    TablePtr = &CpmPcieClockTablePtr->Item[0];
    while (TablePtr->ClkId != 0xFF) {
      SlotStatus = 1;
      if (TablePtr->SlotCheck & BIT1) {
        SlotStatus = CpmTableProtocolPtr->CommonFunction.DetectDevice (CpmTableProtocolPtr, TablePtr->DeviceId, NULL);
      }
      if (SlotStatus && (TablePtr->SlotCheck & BIT0)) {
        SlotStatus = (UINT8) PcieSlotStatus & (BIT0 | BIT1);
        if (SlotStatus) {
          if ((SlotStatus != 1) && (TablePtr->SlotCheck & BIT2)) {
            TablePtr->ClkReq = 0xFF;
            TablePtr->ClkReqExt = 0xFF;
          } else if (SlotStatus == 0) {
            TablePtr->ClkReq = 0;
            TablePtr->ClkReqExt = 0;
          }
        }
      }
      if (SlotStatus) {
        CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, TablePtr->ClkId, TablePtr->ClkReq);
      } else {
        CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, TablePtr->ClkId, 0);
      }
      TablePtr++;
      PcieSlotStatus = PcieSlotStatus >> 2;
    }
  }
  ExpressCardTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
  if (ExpressCardTablePtr) {
    CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1
                                                );
    CpmTableProtocolPtr->CommonFunction.SetGeventSci (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin
                                                );
  }
  if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & (BIT0 | BIT1)) {
    ZeroPowerOddTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (ZeroPowerOddTablePtr) {
      if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT0) {
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1
                                                );
      }
      if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT1) {
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin2,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1
                                                );
      }
    }
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * SMI Handler to set internal PCIe clock
 *
 * This function restores internal PCIe clock when resume from S3.
 *
 */

EFI_STATUS
EFIAPI
CPM_DEFINE_CALLBACK (
  IN      CPM_CALLBACK_NAME     GpioInitS3RestoreCallback,
  IN      CPM_CALLBACK_VAR_TYPE EFI_HANDLE,
  IN      CPM_CALLBACK_VAR_NAME DispatchHandle,
  IN      CPM_CALLBACK_VAR_TYPE CPM_SMM_SW_DISPATCH_CONTEXT,
  IN      CPM_CALLBACK_VAR_NAME *DispatchContext
  )
{
  CpmPcieClockInit (gCpmTableSmmProtocol);
  return EFI_SUCCESS;
}
