/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Display Feature Initialization
 *
 * Contains CPM code to perform GPIO Init under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  DisplayFeature
 * @e \$Revision: 287560 $   @e \$Date: 2014-03-18 13:52:46 -0500 (Tue, 18 Mar 2014) $
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

#include <AmdCpmDxe.h>


VOID
SetApcAndiGFX (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  )
{
  AMD_CPM_COMMON_FUNCTION         *CommonFunctionPtr;
  CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr;

  CommonFunctionPtr       = &CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction;
  DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  if (DisplayFeatureDataPtr->GfxBridgePfa[0].Raw) {
    //APC: Set ISA_EN and VGA_EN bit in the APC bridge
    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciOr8 (
                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Bus,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Device,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Function,
                0x3E,
                BIT2 + BIT3
                );

    //APC: Enable Bus Master, memory and IO decoding
    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciOr8 (
                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Bus,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Device,
                DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Function,
                PCI_COMMAND_OFFSET,
                BIT0 + BIT1 + BIT2
                );
  }

  if (DisplayFeatureDataPtr->GfxDevicePfa[0].Raw) {
    //Internal GFX: Enable memory and IO decoding
    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciOr8 (
                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Bus,
                DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Device,
                DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Function,
                PCI_COMMAND_OFFSET,
                BIT0 + BIT1
                );
  }
  return;
}

VOID
DgpuSpecialPostForHcfiGpu (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL  *CpmDisplayFeatureProtocolPtr,
  IN        VOID                              *VBiosImage,
  IN        UINTN                             VBiosImageSize
  )
{
#ifndef AMD_CPM_EDKII
  EFI_STATUS                                  Status;
  EFI_LEGACY_BIOS_PROTOCOL                    *LegacyBios;
  EFI_IA32_REGISTER_SET                       Regs;
  UINT32                                      TempDBuf;
  UINT8                                       *DSegBuffer;
  UINT16                                      Patch;
  CPM_DISPLAY_FEATURE_PRIVATE                 *DisplayFeatureDataPtr;
  UINT8                                       *SaveBufferPtr;
  EFI_LEGACY_REGION_PROTOCOL                  *LegacyRegion;

  DisplayFeatureDataPtr = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  //Step 00. Allocate memory to save original data
  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  0x10000,
                  &SaveBufferPtr
                  );
  ASSERT_EFI_ERROR (Status);

  //Locate legacy region protocol
  Status = gBS->LocateProtocol (&gEfiLegacyRegionProtocolGuid, NULL, &LegacyRegion);
  ASSERT_EFI_ERROR (Status);

  //Step 01. dGPUVBiosSpecialPost
  Status          = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  ASSERT_EFI_ERROR (Status);

  LegacyRegion->UnLock (LegacyRegion, 0xD0000, 0x10000, NULL);

  TempDBuf        = 0x000D0000;
  DSegBuffer      = (UINT8 *) (UINTN)TempDBuf;

  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.CopyMem (SaveBufferPtr, DSegBuffer, 0x10000);
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.CopyMem (DSegBuffer, VBiosImage, VBiosImageSize);

  Patch           = *((UINT16 *) (UINTN) (TempDBuf + 0x40));
  EfiZeroMem (&Regs, sizeof (EFI_IA32_REGISTER_SET));

  Regs.X.AX       = DisplayFeatureDataPtr->GfxDevicePfa[0].Raw;
  Status          = LegacyBios->FarCall86 (
                                  LegacyBios,
                                  0xD000,
                                  Patch,
                                  &Regs,
                                  NULL,
                                  0
                                  );
  ASSERT_EFI_ERROR (Status);

  //Step 02. CopyVBiosFromD000
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.CopyMem (VBiosImage, DSegBuffer, VBiosImageSize);
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.CopyMem (DSegBuffer, SaveBufferPtr, 0x10000);

   // lock the D000:0000 after we restore it
  LegacyRegion->Lock (LegacyRegion, 0xD0000, 0x10000, NULL);

  Status = gBS->FreePool (SaveBufferPtr);

  Status          = EFI_SUCCESS;
#endif
  return;
}

VOID
DisableDgpuAudioInPX (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  )
{
  CPM_DISPLAY_FEATURE_PRIVATE *DisplayFeatureDataPtr;
  UINT32                      Mmiobase;
  UINT32                      RomStrapOffset;
  UINT32                      BridgeCommand;
  UINT32                      DeviceCommand;
  RomStrapOffset = 0x540C;

  DisplayFeatureDataPtr = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw) {

    BridgeCommand = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead32 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                      0x04
                      );

    DeviceCommand = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead32 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                      0x04
                      );

    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciOr8 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                      0x04,
                      0x02
                      );

    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciOr8 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                      0x04,
                      0x02
                      );

    Mmiobase = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead32 (
                    CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                    DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                    DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                    DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                    0x18
                    );
    if (Mmiobase != 0xFFFFFFFF) {
      CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.MmioAnd32 (
                    (Mmiobase & 0xFFFFFFF0) + RomStrapOffset,
                    0xFFFFEFFF
                    );
    }

    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciWrite32 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                      0x04,
                      BridgeCommand
                      );

    CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciWrite32 (
                      CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function,
                      0x04,
                      DeviceCommand
                      );

  }
  return;
}

VOID
DisableIoDecodeOfPossiblePcieGfx (
  IN        EFI_EVENT   Event,
  IN        VOID        *Context
  )
{
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE         *DisplayFeatureDataPtr;
  AMD_CPM_DEVICE_PATH_TABLE           *DevicePathTablePtr;
  UINT8                               Data8;
  UINT8                               i;

  CpmDisplayFeatureProtocolPtr = Context;
  Data8 = 0;
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD403);

  DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  DevicePathTablePtr      = DisplayFeatureDataPtr->DevicePathTablePtr;

  for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
    if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
      if (DevicePathTablePtr->Path[i].IsDgpu && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga) {
        Data8 = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead8 (
                                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                                0,
                                DevicePathTablePtr->Path[i].Bridge.Device,
                                DevicePathTablePtr->Path[i].Bridge.Function,
                                0x3E                                  // R_IRQ_BRIDGE_CNTL
                                );
        if (!(Data8 & BIT3)) {
          // Clear IO decoding if VGA
          CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciAnd8 (
                                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                                (UINT8)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus,
                                DevicePathTablePtr->Path[i].Device.Device,
                                DevicePathTablePtr->Path[i].Device.Function,
                                0x04,                                 // PCI_COMMAND_OFFSET,
                                0xFE
                                );
        }
      }
    } else {
      break;
    }
  }
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD503);
  return;
}

VOID
DisableIoDecodeOfPossibleApcGfx (
  IN        EFI_EVENT   Event,
  IN        VOID        *Context
  )
{
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE         *DisplayFeatureDataPtr;
  AMD_CPM_DEVICE_PATH_TABLE           *DevicePathTablePtr;
  UINT8                               SecondaryBus;
  UINT8                               Data8;
  UINT8                               i;

  CpmDisplayFeatureProtocolPtr = Context;
  SecondaryBus = 0;
  Data8 = 0;

  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD403);

  DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  DevicePathTablePtr      = DisplayFeatureDataPtr->DevicePathTablePtr;

  for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
    if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
      if (!DevicePathTablePtr->Path[i].IsDgpu && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
        if (!DevicePathTablePtr->Path[i].Bridge.Device) {
          SecondaryBus = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead8 (
                                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                                0,
                                DevicePathTablePtr->Path[i].Bridge.Device,
                                DevicePathTablePtr->Path[i].Bridge.Function,
                                0x19                                            // PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                                );
        }
        Data8 = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead8 (
                                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                                SecondaryBus,
                                DevicePathTablePtr->Path[i].Device.Device,
                                DevicePathTablePtr->Path[i].Device.Function,
                                0x0B                                            // R_BASE_CODE
                                );
        if (Data8 == 0x3) {
          // Clear IO decoding if VGA
           CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciAnd8 (
                                CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                                SecondaryBus,
                                DevicePathTablePtr->Path[i].Device.Device,
                                DevicePathTablePtr->Path[i].Device.Function,
                                0x04,                                           // PCI_COMMAND_OFFSET,
                                0xFE
                                );
          break;
        }
      }
    } else {
      break;
    }
  }
  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD503);
  return;
}

VOID
ClearPossiblePcieGFX (
  IN        VOID        *Context
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   LegacyBootEvent;

  LegacyBootEvent = NULL;
  Status = CpmCreateEventReadyToBoot (
              CPM_TPL_CALLBACK,
              DisableIoDecodeOfPossiblePcieGfx,
              Context,
              &LegacyBootEvent
              );
  ASSERT_EFI_ERROR (Status);
  return;
}

VOID
ClearPossibleApcGFX (
  IN        VOID        *Context
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   LegacyBootEvent;

  LegacyBootEvent = NULL;
  Status = CpmCreateEventReadyToBoot (
              CPM_TPL_CALLBACK,
              DisableIoDecodeOfPossibleApcGfx,
              Context,
              &LegacyBootEvent
              );
  ASSERT_EFI_ERROR (Status);
  return;
}

VOID
GetGpuVbiosImage (
  IN       AMD_CPM_DISPLAY_FEATURE_PROTOCOL   *CpmDisplayFeatureProtocolPtr,
  IN       UINT8                              GpuType,
     OUT   VOID                               **VBiosImage,
     OUT   UINTN                              *VBiosImageSize
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       HandleCount;
  EFI_HANDLE                                  *HandleBuffer;
  UINTN                                       Index;
  EFI_PCI_IO_PROTOCOL                         *PciIo;
  BOOLEAN                                     Found;
  BOOLEAN                                     dGpVbiosBarStatus;
  AMD_CPM_ALL_PCI_IO_PROTOCOLS_INSTALLED_PROTOCOL   *Tmp;
  UINT32                                      *VbiosInfo;
  UINTN                                       Seg;
  UINTN                                       Bus;
  UINTN                                       Dev;
  UINTN                                       Fnc;
  UINT8                                       GpuBus;
  EFI_HANDLE                                  VgaHandle;
  UINT16                                      dGpVid;
  UINT32                                      dGpVbiosBar;
  CPM_DISPLAY_FEATURE_PRIVATE                 *DisplayFeatureDataPtr;
  UINT16                                      *TmpPtr;
  PCI_DATA_STRUCTURE                          *TmpPciDataStruct;
  UINT32                                      FrameBufferAddress;
  UINT32                                      RomAddress;
  UINT32                                      Temp;

  dGpVbiosBarStatus = FALSE;
  dGpVid = 0;
  Found = FALSE;
  PciIo = NULL;
  GpuBus = 0x00;
  Seg = 0x00;
  Bus = 0x00;
  Dev = 0x00;
  Fnc = 0x00;
  VgaHandle = NULL;
  DisplayFeatureDataPtr = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

  //
  // A. Search Int or Ext GFX bridge and then get the SubBus information from this bridge
  //

  //  Step 01.
  //  Search handles that support the gEfiPciIoProtocolGuid protocol
  //  gBS->LocateHandleBuffer returns an array of handles.

  Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiPciIoProtocolGuid,
                      NULL,
                      &HandleCount,
                      &HandleBuffer
                      );

  ASSERT_EFI_ERROR (Status);

  //
  //B. Search iGPU / dGPU device under bridge
  //

  *VBiosImage     = NULL;
  *VBiosImageSize = 0;

  for (Index = 0; (Index < HandleCount) && (Found == FALSE); Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    &PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Step 01.
    // See if this is a PCI Graphics Controller by looking at Class Code Register
    //
    PciIo->GetLocation (
             PciIo,
             &Seg,
             &Bus,
             &Dev,
             &Fnc
             );

    if (((UINTN)Bus == DisplayFeatureDataPtr->GfxDevicePfa[GpuType].Pfa.Bus) &&
        ((UINTN)Dev == DisplayFeatureDataPtr->GfxDevicePfa[GpuType].Pfa.Device) &&
        ((UINTN)Fnc == DisplayFeatureDataPtr->GfxDevicePfa[GpuType].Pfa.Function)) {

      Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint16,
                          PCI_VENDOR_ID_OFFSET,       //Start offset: 0x00
                          1,                          //Length of bytes
                          &dGpVid
                          );
      Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0x30,                       //Start offset: 0x30
                          1,                          //Length of bytes
                          &dGpVbiosBar
                          );

      if (dGpVid != 0xFFFF) {
        if (dGpVbiosBar) {
          Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0x10,                       //Start offset: 0x10
                          1,                          //Length of bytes
                          &FrameBufferAddress
                          );
          RomAddress = FrameBufferAddress & 0xFFFFFFF0;
          if (RomAddress) {
            Temp = 0;
            Status = PciIo->Pci.Write (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0x10,                       //Start offset: 0x10
                          1,                          //Length of bytes
                          &Temp
                          );
            Temp = RomAddress | 0x00000001;
            Status = PciIo->Pci.Write (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0x30,                       //Start offset: 0x30
                          1,                          //Length of bytes
                          &Temp
                          );
            if (CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.MmioRead16 (RomAddress) == 0xAA55) {
              dGpVbiosBarStatus = TRUE;
            }
            Status = PciIo->Pci.Write (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0x10,                       //Start offset: 0x10
                            1,                          //Length of bytes
                            &FrameBufferAddress
                            );
            Status = PciIo->Pci.Write (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0x30,                       //Start offset: 0x30
                            1,                          //Length of bytes
                            &dGpVbiosBar
                            );
          }
        }
        VgaHandle       = HandleBuffer[Index];
        *VBiosImage     = PciIo->RomImage;
        *VBiosImageSize = (UINTN) PciIo->RomSize;
      } else {
        *VBiosImage     = NULL;
        *VBiosImageSize = 0;
      }
      Found = TRUE;
    }
  }

  //
  // Check VBIOS ROM SIZE. if it is harger 64K bytes, it must be adjusted.
  //
  if (*VBiosImageSize > 0x10000) {
    //
    // Get a Pointer to PCI Data Structure from offset 0x18,0x19
    //
    TmpPtr = (UINT16 *) ((UINT8 *) (*VBiosImage) + 0x18);

    //
    // Get actual VBios image size
    //
    TmpPciDataStruct = (PCI_DATA_STRUCTURE *) ((UINT8 *) (*VBiosImage) + (*TmpPtr));
    *VBiosImageSize = (UINT16) (TmpPciDataStruct->ImageLength) * 512;
  }

  //
  //Step03. Save OpRom info. to an Array of AmdCpmAllPciIoProtocolsInstalledProtocolGuid
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  12,                      //(UINT32 = 4 bytes, 4 byte * 3 = 12 bytes)
                  (VOID *)&VbiosInfo
                  );

  Status = gBS->LocateProtocol (&gAmdCpmAllPciIoProtocolsInstalledProtocolGuid, NULL, &Tmp);
  VbiosInfo[0] = (UINT32) (UINTN)*VBiosImage;
  VbiosInfo[1] = (UINT32) *VBiosImageSize;
  VbiosInfo[2] = (UINT32) (UINTN)VgaHandle;

  DisplayFeatureDataPtr->VBiosImage       = (UINT32) (UINTN) *VBiosImage;
  DisplayFeatureDataPtr->VBiosImageSize   = (UINT32) *VBiosImageSize;

  if (dGpVbiosBarStatus) {
    DisplayFeatureDataPtr->VBiosFlag      = 1;
  } else {
    DisplayFeatureDataPtr->VBiosFlag      = 0;
  }

  Tmp->Buffer = (VOID *)VbiosInfo;

  return;
}

AGESA_STATUS
AmdCpmGetPostedVbiosImage (
  IN      GFX_VBIOS_IMAGE_INFO         *VbiosImageInfo
  )
{
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE         *DisplayFeatureDataPtr;
  EFI_STATUS                          Status;
  AGESA_STATUS                        CpmStatus;
  VOID                                *VBiosImage;
  UINT32                              VBiosImageSize;
  UINT32                              IsDgpuPrimary;

  CpmStatus = AGESA_UNSUPPORTED;
  Status = gBS->LocateProtocol (
                  &gAmdCpmDisplayFeatureProtocolGuid,
                  NULL,
                  &CpmDisplayFeatureProtocolPtr
                  );
  if (!EFI_ERROR (Status)) {
    DisplayFeatureDataPtr = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;
    IsDgpuPrimary         = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;
    if ((DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress || DisplayFeatureDataPtr->CurrentFeature.Mask.SurroundView ||
      DisplayFeatureDataPtr->CurrentFeature.Mask.HyperCrossFire || CpmDisplayFeatureProtocolPtr->TableProtocolPtr->MainTablePtr->DisplayFeature.Config.SpecialPostIgpu) && IsDgpuPrimary) {
      if (DisplayFeatureDataPtr->GfxDevicePfa[0].Raw == 0) {
        DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Bus = 0;
        DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Device = 1;
        DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Function = 0;
      }
      if ((DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Bus == VbiosImageInfo->GfxPciAddress.Address.Bus) &&
          (DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Device == VbiosImageInfo->GfxPciAddress.Address.Device) &&
          (DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Function == VbiosImageInfo->GfxPciAddress.Address.Function)) {
        if ((VbiosImageInfo->Flags & GFX_VBIOS_IMAGE_FLAG_SPECIAL_POST) != 0) {
          // HCF-dGPU or SurroundView
          GetGpuVbiosImage (CpmDisplayFeatureProtocolPtr, 0, (VOID *)&VBiosImage, (UINTN*)&VBiosImageSize);
          if (VBiosImage && VBiosImageSize) {
            SetApcAndiGFX (CpmDisplayFeatureProtocolPtr);
            DgpuSpecialPostForHcfiGpu (CpmDisplayFeatureProtocolPtr, VBiosImage, VBiosImageSize);
            VbiosImageInfo->ImagePtr = VBiosImage;
            CpmStatus = AGESA_SUCCESS;
          }
        }
      }
    }
  }
  return CpmStatus;
}

VOID
EFIAPI
AmdCpmDisplayFeatureVbiosShadow (
  IN        EFI_EVENT                   Event,
  IN        VOID                        *Context
  )
{
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE         *DisplayFeatureDataPtr;
  AMD_CPM_MAIN_TABLE                  *MainTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE           *DevicePathTablePtr;
  AMD_CPM_DEVICE_PATH_ITEM            *iGpuDevice;
  AMD_CPM_DEVICE_PATH_ITEM            *dGpuDevice;
  AMD_CPM_GPIO_DEVICE_POWER_TABLE     *GpioDevicePowerTablePtr;
  AMD_CPM_TABLE_PROTOCOL              *CpmTableProtocolPtr;

  STATIC BOOLEAN                      IsInvoked = FALSE;
  UINT32                              IsDgpuPrimary;
  UINT8                               i;
  UINT8                               bus;
  UINT8                               DisplayStatus;
  UINT16                              GfxVid;
  UINT8                               ibus;
  UINT8                               dbus;
  UINT8                               GpioDeviceId;
  UINT8                               DeviceId;
  UINT8                               Mode;
  UINT8                               Index;
  VOID                                *VBiosImage;
  UINTN                               VBiosImageSize;

  RECORD_TIME_DXE (BeginAmdCpmDisplayFeatureMidInit);

  CpmDisplayFeatureProtocolPtr = Context;
  DisplayStatus = 0;
  iGpuDevice = NULL;
  dGpuDevice = NULL;

  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD203);

  if (!IsInvoked) {
    MainTablePtr            = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->MainTablePtr;
    DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;
    DevicePathTablePtr      = DisplayFeatureDataPtr->DevicePathTablePtr;
    IsDgpuPrimary           = MainTablePtr->DisplayFeature.Config.IsDgpuPrimary;
    CpmTableProtocolPtr     = CpmDisplayFeatureProtocolPtr->TableProtocolPtr;
    GpioDevicePowerTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_POWER);

#ifndef AMD_CPM_EDKII
    CpmTableProtocolPtr->DxePublicFunction.GetPostedVbiosImage = AmdCpmGetPostedVbiosImage;
#endif

    DisplayFeatureDataPtr->GfxDevicePfa[0].Raw  = 0;
    DisplayFeatureDataPtr->GfxDevicePfa[1].Raw  = 0;
    DisplayFeatureDataPtr->GfxDevicePfa[2].Raw  = 0;
    DisplayFeatureDataPtr->GfxBridgePfa[0].Raw  = 0;
    DisplayFeatureDataPtr->GfxBridgePfa[1].Raw  = 0;
    DisplayFeatureDataPtr->GfxBridgePfa[2].Raw  = 0;
    DisplayFeatureDataPtr->CurrentFeature.Raw   = 0;

    if (MainTablePtr->DisplayFeature.Config.PowerXpressFixedMode || MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode) {
      DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress = 1;
    } else if (MainTablePtr->DisplayFeature.Config.HyperCrossFire) {
      DisplayFeatureDataPtr->CurrentFeature.Mask.HyperCrossFire = 1;
    } else if (MainTablePtr->DisplayFeature.Config.SurroundView) {
      DisplayFeatureDataPtr->CurrentFeature.Mask.SurroundView = 1;
    }

    for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
      DevicePathTablePtr->Path[i].FeatureMask.Raw &= 0x8000FFFF;
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
        bus = 0;
        if (DevicePathTablePtr->Path[i].Bridge.Device == 0) {
          GfxVid = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead16 (
                              CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                              bus,
                              DevicePathTablePtr->Path[i].Device.Device,
                              DevicePathTablePtr->Path[i].Device.Function,
                              PCI_VENDOR_ID_OFFSET
                              );
        } else {
          GfxVid = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead16 (
                              CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                              0,
                              DevicePathTablePtr->Path[i].Bridge.Device,
                              DevicePathTablePtr->Path[i].Bridge.Function,
                              PCI_VENDOR_ID_OFFSET
                              );
          if (GfxVid != 0xFFFF && GfxVid != 0x0000) {
            bus = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead8 (
                              CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                              0,
                              DevicePathTablePtr->Path[i].Bridge.Device,
                              DevicePathTablePtr->Path[i].Bridge.Function,
                              PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                              );
            GfxVid = CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead16 (
                              CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                              bus,
                              DevicePathTablePtr->Path[i].Device.Device,
                              DevicePathTablePtr->Path[i].Device.Function,
                              PCI_VENDOR_ID_OFFSET
                              );
          }
        }
        if (GfxVid != 0xFFFF && GfxVid != 0x0000) {
          if (CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PciRead8 (
                              CpmDisplayFeatureProtocolPtr->TableProtocolPtr,
                              bus,
                              DevicePathTablePtr->Path[i].Device.Device,
                              DevicePathTablePtr->Path[i].Device.Function,
                              0x0B                                            // R_BASE_CODE
                              ) == 3) {
            DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist = 1;
            if (DevicePathTablePtr->Path[i].FeatureMask.Raw & DisplayFeatureDataPtr->CurrentFeature.Raw) {
              if (DevicePathTablePtr->Path[i].IsDgpu) {
                if (!dGpuDevice) {
                  dbus = bus;
                  dGpuDevice = &(DevicePathTablePtr->Path[i]);
                  DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus = 0;
                  DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device = dGpuDevice->Bridge.Device;
                  DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function = dGpuDevice->Bridge.Function;
                  DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Bus = dbus;
                  DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Device = dGpuDevice->Device.Device;
                  DisplayFeatureDataPtr->GfxDevicePfa[1].Pfa.Function = dGpuDevice->Device.Function;
                  if (IsDgpuPrimary) {
                    DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga = 1;
                  }
                }
              } else {
                if (!iGpuDevice) {
                  ibus = bus;
                  iGpuDevice = &(DevicePathTablePtr->Path[i]);
                  DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Bus = 0;
                  DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Device = iGpuDevice->Bridge.Device;
                  DisplayFeatureDataPtr->GfxBridgePfa[0].Pfa.Function = iGpuDevice->Bridge.Function;
                  DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Bus = ibus;
                  DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Device = iGpuDevice->Device.Device;
                  DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Function = iGpuDevice->Device.Function;
                  if (!IsDgpuPrimary) {
                    DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga = 1;
                    if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress) {
                      DevicePathTablePtr->Path[i].FeatureMask.Mask.Removable = 1;
                    }
                  }
                }
              }
            } else {
              DevicePathTablePtr->Path[i].FeatureMask.Raw &= 0xFFFF0000;
            }
            if (DevicePathTablePtr->Path[i].IsDgpu) {
              DisplayStatus |= BIT0;
            } else {
              DisplayStatus |= BIT1;
            }
            DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus = bus;
          } else {
            DevicePathTablePtr->Path[i].FeatureMask.Raw &= 0xFFFF0000;
          }
        } else {
          DevicePathTablePtr->Path[i].FeatureMask.Raw &= 0xFFFF0000;
        }
      } else {
        break;
      }
    }

    if (!(iGpuDevice && dGpuDevice)) {
      if (iGpuDevice) {
        MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 0;
      } else if (dGpuDevice) {
        MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 1;
      } else {
        for (Index = 0; Index < 2; Index ++) {
          for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
            if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
              break;
            }
            if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist == 1) {
              if (DevicePathTablePtr->Path[i].IsDgpu == MainTablePtr->DisplayFeature.Config.IsDgpuPrimary) {
                DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga = 1;
                break;
              }
            }
          }
          if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga == 1) {
            break;
          }
          MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 1;
        }
      }
      DisplayFeatureDataPtr->CurrentFeature.Raw = 0;
      MainTablePtr->DisplayFeature.Raw &= 0xFFFFFF00;
      if (DisplayStatus == (BIT0 | BIT1)) {
        if (!IsDgpuPrimary) {
          ClearPossiblePcieGFX (Context);
        } else {
          ClearPossibleApcGFX (Context);
          ClearPossiblePcieGFX (Context);
        }
      } else if (DisplayStatus == BIT0) {
        ClearPossiblePcieGFX (Context);
      }

      for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
        if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
          break;
        }
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist == 1) {
          if (DevicePathTablePtr->Path[i].IsDgpu == MainTablePtr->DisplayFeature.Config.IsDgpuPrimary) {
            if (GpioDevicePowerTablePtr) {
              DeviceId  = DevicePathTablePtr->Path[i].DeviceId;
              Mode      = DevicePathTablePtr->Path[i].Mode;
              if (DeviceId != 0xFF && DeviceId != 0x00) {
                for (Index = 0; Index < AMD_GPIO_DEVICE_POWER_SIZE; Index++) {
                  GpioDeviceId = GpioDevicePowerTablePtr->DevicePowerList[Index].DeviceId;
                  if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
                    break;
                  }
                  if (GpioDeviceId == DeviceId &&
                      GpioDevicePowerTablePtr->DevicePowerList[Index].Mode == Mode &&
                      GpioDevicePowerTablePtr->DevicePowerList[Index].Type == 0) {
                    CpmTableProtocolPtr->CommonFunction.SetGpio (
                                                                  CpmTableProtocolPtr,
                                                                  GpioDevicePowerTablePtr->DevicePowerList[Index].Config.SetGpio.Pin,
                                                                  GpioDevicePowerTablePtr->DevicePowerList[Index].Config.SetGpio.Value
                                                                  );
                  }
                }
              }
            }
            break;
          }
        }
      }
    } else {
      if ((DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress || DisplayFeatureDataPtr->CurrentFeature.Mask.HyperCrossFire) && !IsDgpuPrimary) {
        // PowerXpress
        GetGpuVbiosImage (CpmDisplayFeatureProtocolPtr, 1, (VOID *)&VBiosImage, &VBiosImageSize);
        if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress && MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX && !MainTablePtr->DisplayFeature.Config.DgpuDisplayOutput) {
          DisableDgpuAudioInPX (CpmDisplayFeatureProtocolPtr);
        }
      }
      if (IsDgpuPrimary) {
        ClearPossibleApcGFX (Context);
        ClearPossiblePcieGFX (Context);
      } else {
        ClearPossiblePcieGFX (Context);
      }
    }
  }

  CpmDisplayFeatureProtocolPtr->TableProtocolPtr->CommonFunction.PostCode (0xD303);

  IsInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmDisplayFeatureMidInit);
}

/*----------------------------------------------------------------------------------------*/
/**
 * AmdCpmDisplayFeatureInitMid
 *
 * This function is called as part of CPM DXE Driver Initialization. It gets called
 * each time the AMD CPM ALL PCI IO PROTOCOLS INSTALLED PROTOCOL is installed.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmDisplayFeatureInitMid (
  IN        EFI_EVENT                       Event,
  IN        VOID                            *Context
  )
{
  EFI_STATUS      Status = 0;
  STATIC BOOLEAN  AmdCpmDisplayFeatureInitMidInvoked = FALSE;
  EFI_EVENT       DisplayFeatureVbiosShadow;
  VOID            *mRegisterationForVbiosShadow;

  if (!AmdCpmDisplayFeatureInitMidInvoked) {

    //
    // Register the event handling function for AmdCpmDisplayFeatureInitMid to be launched after
    // PciIo protocol
    //
    Status = gBS->CreateEvent (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmDisplayFeatureVbiosShadow,
                    Context,
                    &DisplayFeatureVbiosShadow
                    );

    if (EFI_ERROR (Status)) {
      return;
    }

    Status = gBS->RegisterProtocolNotify (
                    &gAmdCpmAllPciIoProtocolsInstalledProtocolGuid,
                    DisplayFeatureVbiosShadow,
                    &mRegisterationForVbiosShadow
                    );

    if (EFI_ERROR (Status)) {
      return;
    }
  }

  AmdCpmDisplayFeatureInitMidInvoked = TRUE;
}
