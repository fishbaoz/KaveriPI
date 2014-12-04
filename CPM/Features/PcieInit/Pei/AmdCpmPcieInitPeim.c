/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM PCIE Initialization
 *
 * Contains code that initialized PCIE before memory init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  PcieInit
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

EFI_STATUS
EFIAPI
AmdCpmPcieInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmPcieInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmGpioInitFinishedPpiGuid,
  AmdCpmPcieInitPeim
};

/*----------------------------------------------------------------------------------------*/
/**
 * The Function to reset PCIe device.
 *
 * @param[in]     This          Pointer to AMD CPM Table Ppi
 * @param[in]     ResetId       Pcie Device Id which is defined in Pcie Complex Descriptor table
 * @param[in]     ResetControl  Reset Control Flag. 0: Reset assert. 1: Reset deassert
 *
 * @retval        EFI_SUCCESS   Function initialized successfully
 * @retval        EFI_ERROR     Initialization failed (see error for more details)
 */

VOID
EFIAPI
CpmPcieReset (
  IN      VOID                        *This,
  IN      UINT8                       ResetId,
  IN      UINT8                       ResetControl
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr;
  CpmTablePpiPtr = This;
  CpmTablePpiPtr->CommonFunction.ResetDevice (CpmTablePpiPtr, ResetId, ResetControl);

  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Generate Pcie Complex Descriptor table for AGESA and set the trigger level of
 * GEVENT pin for Express Card.
 *
 * @param[in]     PeiServices       Pointer to Pei Services
 * @param[in]     NotifyDescriptor  The descriptor for the notification event
 * @param[in]     Ppi               Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS       Module initialized successfully
 * @retval        EFI_ERROR         Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmPcieInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  AMD_CPM_TABLE_PPI                   *CpmTablePpiPtr;
  AMD_CPM_MAIN_TABLE                  *MainTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE         *PcieTopologyTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE    *PcieTopologyOverrideTablePtr;
  AMD_CPM_EXPRESS_CARD_TABLE          *ExpressCardTablePtr;
  AMD_CPM_WIRELESS_BUTTON_TABLE       *WirelessButtonTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE          *SaveContextTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE            *PcieClockTablePtr;
  PCIe_COMPLEX_DESCRIPTOR             *PcieComplexDescriptorPtr;
  EFI_STATUS                          Status;
  UINT8                               Index;
  UINT8                               Offset;
  UINT32                              PCIeStatus;
  UINT8                               i;
  BOOLEAN                             ClockType;

  RECORD_TIME_PEI (BeginAmdCpmPcieInitPeimDriver);

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

  CpmTablePpiPtr->CommonFunction.PostCode (0xC006);

  MainTablePtr                    = CpmTablePpiPtr->MainTablePtr;
  PcieTopologyTablePtr            = CpmTablePpiPtr->CommonFunction.GetTablePtr2 (CpmTablePpiPtr, CPM_SIGNATURE_PCIE_TOPOLOGY);
  PcieTopologyOverrideTablePtr    = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_PCIE_TOPOLOGY_OVERRIDE);
  ExpressCardTablePtr             = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
  WirelessButtonTablePtr          = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_WIRELESS_BUTTON);
  SaveContextTablePtr             = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  PcieClockTablePtr               = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_PCIE_CLOCK);

  if (PcieTopologyTablePtr && PcieTopologyOverrideTablePtr) {
    for (Index = 0; Index < AMD_PCIE_TOPOLOGY_OVERRIDE_SIZE; Index ++) {
      if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Raw == 0xFF) {
        break;
      }
      if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.Valid) {
        Offset = PcieTopologyOverrideTablePtr->Item[Index].Offset;
        if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.IsDdi) {
          if (Offset < AMD_PCIE_DDI_DESCRIPTOR_SIZE) {
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.EnableOverride) {
              if (PcieTopologyOverrideTablePtr->Item[Index].Enable) {
                PcieTopologyTablePtr->Ddi[Offset].EngineData.EngineType = PcieDdiEngine;
              } else {
                PcieTopologyTablePtr->Ddi[Offset].EngineData.EngineType = 0;
              }
            }
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.DdiTypeOverride) {
              PcieTopologyTablePtr->Ddi[PcieTopologyOverrideTablePtr->Item[Index].Offset].Ddi.ConnectorType = PcieTopologyOverrideTablePtr->Item[Index].DdiType;
            }
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.LaneOverride) {
              PcieTopologyTablePtr->Ddi[Offset].EngineData.StartLane = PcieTopologyOverrideTablePtr->Item[Index].StartLane;
              PcieTopologyTablePtr->Ddi[Offset].EngineData.EndLane = PcieTopologyOverrideTablePtr->Item[Index].EndLane;
            }
          }
        } else {
          if (Offset < AMD_PCIE_PORT_DESCRIPTOR_SIZE) {
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.EnableOverride) {
              if (PcieTopologyOverrideTablePtr->Item[Index].Enable) {
                PcieTopologyTablePtr->Port[Offset].EngineData.EngineType = PciePortEngine;
              } else {
                PcieTopologyTablePtr->Port[Offset].EngineData.EngineType = 0;
              }
            }
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.PortPresentOverride) {
              PcieTopologyTablePtr->Port[Offset].Port.PortPresent = PcieTopologyOverrideTablePtr->Item[Index].PortPresent;
            }
            if (PcieTopologyOverrideTablePtr->Item[Index].Flag.Config.LaneOverride) {
              PcieTopologyTablePtr->Port[Offset].EngineData.StartLane = PcieTopologyOverrideTablePtr->Item[Index].StartLane;
              PcieTopologyTablePtr->Port[Offset].EngineData.EndLane = PcieTopologyOverrideTablePtr->Item[Index].EndLane;
            }
          }
        }
      }
    }
  }

  if (SaveContextTablePtr && PcieTopologyTablePtr && CpmTablePpiPtr->CommonFunction.GetBootMode () == CPM_BOOT_MODE_S3) {
    PCIeStatus = 0;
    PCIeStatus |= CpmTablePpiPtr->CommonFunction.GetSaveContext (CpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x00);
    PCIeStatus |= CpmTablePpiPtr->CommonFunction.GetSaveContext (CpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x01) << 8;
    PCIeStatus |= CpmTablePpiPtr->CommonFunction.GetSaveContext (CpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x02) << 16;
    PCIeStatus |= CpmTablePpiPtr->CommonFunction.GetSaveContext (CpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x03) << 24;
    for ( Index = 0; Index < AMD_PCIE_PORT_DESCRIPTOR_SIZE; Index ++ ) {
      if ((PcieTopologyTablePtr->Port[Index].EngineData.EngineType == PciePortEngine) && (PcieTopologyTablePtr->Port[Index].Port.PortPresent)) {
        if (!(PCIeStatus & (1 << Index))) {
          PcieTopologyTablePtr->Port[Index].Port.PortPresent = PortDisabled;
        }
      }
      if (PcieTopologyTablePtr->Port[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
        break;
      }
    }
  }

  if (ExpressCardTablePtr && PcieTopologyTablePtr) {
    for ( Index = 0; Index < AMD_PCIE_PORT_DESCRIPTOR_SIZE; Index ++ ) {
      if ((PcieTopologyTablePtr->Port[Index].EngineData.EngineType == PciePortEngine) &&
        (PcieTopologyTablePtr->Port[Index].Port.PortPresent) &&
        (PcieTopologyTablePtr->Port[Index].Port.DeviceNumber == ExpressCardTablePtr->Device)  &&
        (PcieTopologyTablePtr->Port[Index].Port.FunctionNumber == ExpressCardTablePtr->Function)) {

        PcieTopologyTablePtr->Port[Index].Port.LinkHotplug = HotplugBasic;
        if (CpmTablePpiPtr->CommonFunction.GetGevent (CpmTablePpiPtr, ExpressCardTablePtr->EventPin)) {
          PcieTopologyTablePtr->Port[Index].Port.EndpointStatus = EndpointNotPresent;
        }
      }
      if (PcieTopologyTablePtr->Port[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
        break;
      }
    }
  }

  if (WirelessButtonTablePtr && PcieTopologyTablePtr) {
    if (CpmTablePpiPtr->MainTablePtr->WirelessButtonEn == 2) {
      for ( Index = 0; Index < AMD_PCIE_PORT_DESCRIPTOR_SIZE; Index ++ ) {
        if ((PcieTopologyTablePtr->Port[Index].EngineData.EngineType == PciePortEngine) && (PcieTopologyTablePtr->Port[Index].Port.PortPresent)) {
          for (i = 0; i < 4; i++) {
            if (WirelessButtonTablePtr->Bridge[i].Device || WirelessButtonTablePtr->Bridge[i].Function) {
              if ((PcieTopologyTablePtr->Port[Index].Port.DeviceNumber == WirelessButtonTablePtr->Bridge[i].Device)  &&
                  (PcieTopologyTablePtr->Port[Index].Port.FunctionNumber == WirelessButtonTablePtr->Bridge[i].Function)) {

                PcieTopologyTablePtr->Port[Index].Port.LinkHotplug = HotplugBasic;
              }
            }
          }
        }
        if (PcieTopologyTablePtr->Port[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
          break;
        }
      }
    }
  }

  if (PcieClockTablePtr) {
    ClockType = (BOOLEAN) CpmTablePpiPtr->CommonFunction.GetStrap (CpmTablePpiPtr) & BIT1;
    for ( Index = 0; Index < AMD_PCIE_PORT_DESCRIPTOR_SIZE; Index ++ ) {
      if ((PcieTopologyTablePtr->Port[Index].EngineData.EngineType == PciePortEngine) && (PcieTopologyTablePtr->Port[Index].Port.PortPresent)) {
        for (i = 0; i < AMD_PCIE_CLOCK_SIZE; i++) {
          if ((PcieTopologyTablePtr->Port[Index].Port.DeviceNumber == PcieClockTablePtr->Item[i].Device)  &&
              (PcieTopologyTablePtr->Port[Index].Port.FunctionNumber == PcieClockTablePtr->Item[i].Function)) {
            if (ClockType) {
              if (PcieClockTablePtr->Item[i].ClkReq != 0x00 && PcieClockTablePtr->Item[i].ClkReq != 0xFF) {
                PcieTopologyTablePtr->Port[Index].Port.MiscControls.ClkPmSupport = 1;
              }
            } else {
              if (PcieClockTablePtr->Item[i].ClkReqExt != 0x00 && PcieClockTablePtr->Item[i].ClkReqExt != 0xFF) {
                PcieTopologyTablePtr->Port[Index].Port.MiscControls.ClkPmSupport = 1;
              }
            }
          }
        }
      }
      if (PcieTopologyTablePtr->Port[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
        break;
      }
    }
  }

  Status = (**PeiServices).AllocatePool (
                                          PeiServices,
                                          sizeof (PCIe_COMPLEX_DESCRIPTOR),
                                          &PcieComplexDescriptorPtr
                                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  (**PeiServices).SetMem (
                          PcieComplexDescriptorPtr,
                          sizeof (PCIe_COMPLEX_DESCRIPTOR),
                          0
                         );
  PcieComplexDescriptorPtr->Flags         = DESCRIPTOR_TERMINATE_LIST;
  PcieComplexDescriptorPtr->SocketId      = 0;
  PcieComplexDescriptorPtr->PciePortList  = &PcieTopologyTablePtr->Port[0];
  PcieComplexDescriptorPtr->DdiLinkList   = &PcieTopologyTablePtr->Ddi[0];


  CpmTablePpiPtr->PeimPublicFunction.PcieReset                = CpmPcieReset;

  CpmTablePpiPtr->PeimPublicFunction.PcieComplexDescriporPtr  = PcieComplexDescriptorPtr;
  CpmTablePpiPtr->CommonFunction.PostCode (0xC106);

  RECORD_TIME_PEI (EndAmdCpmPcieInitPeimDriver);

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM PCIE Init PEIM driver
 *
 * This function registers the function to update PCIe topology table according
 * to AMD CPM PCIE TOPOLOGY OVERRIDE table and set GEVENT trigger level for Express
 * Card if AMD CPM Express Card Table exists.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmPcieInitPeimEntryPoint (
  IN        CPM_PEI_FILE_HANDLE         FileHandle,
  IN        CPM_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS                      Status;

  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmPcieInitPeimNotify);

  return EFI_SUCCESS;
}
