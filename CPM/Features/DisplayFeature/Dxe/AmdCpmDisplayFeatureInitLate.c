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
 * @e \$Revision: 285127 $   @e \$Date: 2014-02-18 10:33:07 -0600 (Tue, 18 Feb 2014) $
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

EFI_GUID DISPLAY_FEATURE_I_ACPI_DEVICE_GUID = {
  0x9d695853, 0xa840, 0x4670, 0x9d, 0xae, 0x95, 0x11, 0x16, 0xe8, 0x11, 0x51
};

EFI_GUID DISPLAY_FEATURE_D_ACPI_DEVICE_GUID = {
  0x78db63b7, 0xe9f4, 0x4244, 0x90, 0xc6, 0xf5, 0x84, 0x59, 0x12, 0xeb, 0x3b
};

EFI_GUID DISPLAY_FEATURE_MXM_OVERT_ACPI_DEVICE_GUID = {
  0xe0ec8f82, 0xfd17, 0x4f8d, 0x8d, 0xff, 0xa4, 0xcf, 0xfb, 0xe7, 0x4c, 0x4d
};

EFI_GUID DISPLAY_FEATURE_DISPLAY_CONNECT_EVENT_ACPI_DEVICE_GUID = {
  0x6476864d, 0xf8a0, 0x460e, 0x94, 0x7b, 0x5f, 0x7b, 0x88, 0xa5, 0x21, 0x16
};

VOID
CpmDisplayFeatureInitLate (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  );

BOOLEAN
EFIAPI
CpmIGpuSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
CpmDGpuSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
CpmMxmOverTSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

BOOLEAN
EFIAPI
CpmDisplayConnectEventSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * AmdCpmDisplayFeatureInitLate
 *
 * This function is called as part of CPM DXE Driver Initialization. It gets called
 * each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmDisplayFeatureInitLate (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  STATIC BOOLEAN  AmdCpmDisplayFeatureInitLateInvoked = FALSE;

  RECORD_TIME_DXE (BeginAmdCpmDisplayFeatureLateInit);

  if (!AmdCpmDisplayFeatureInitLateInvoked) {
    CpmDisplayFeatureInitLate (Context);
  }

  AmdCpmDisplayFeatureInitLateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmDisplayFeatureLateInit);
}


/*----------------------------------------------------------------------------------------*/
/**
 *
 * The function to set Sub-System Id on iGPU and dGPU, update NV Data for
 * Display Feature and install Display Feature SSDT tables for iGPU and dGPU.
 *
 * @param[in]     CpmDisplayFeatureProtocolPtr  Pointer to AMD CPM Display Feature Protocol
 *
 */

VOID
CpmDisplayFeatureInitLate (
  IN        AMD_CPM_DISPLAY_FEATURE_PROTOCOL    *CpmDisplayFeatureProtocolPtr
  )
{
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_NV_DATA_PROTOCOL        *pAmdCpmNvDataProtocol;
  AMD_CPM_DISPLAY_FEATURE_TABLE   *DisplayFeatureTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE       *DevicePathTablePtr;
  AMD_CPM_SPECIFIC_SSID_TABLE     *SpecificSsidTablePtr;
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE  *RebrandDualGraphicsSsidTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE     *PcieTopologyTablePtr;
  PCIe_PORT_DESCRIPTOR            *PciePortDescriptorPtr;
  AMD_CPM_GFX_DETECT              GfxDetect;
  EFI_STATUS                      Status;
  UINT8                           Bus;
  UINT8                           Dev;
  UINT8                           Func;
  UINT16                          VendorId;
  UINT16                          VendorId2;
  UINT16                          DeviceId;
  UINT16                          DeviceId2;
  UINT32                          Index;
  AMD_CPM_NV_DATA_STRUCT          *eNvStore;
#ifndef AMD_CPM_EDKII
  UINT32                          VbiosAddr;
  UINT32                          VbiosSize;
#endif
  UINT32                          Ssid;
  UINT32                          NewSsid;
  UINT8                           i;
  UINT8                           iGpuIndex;
  UINT8                           dGpuIndex;
  UINT32                          Buffer[2];
  CPM_DISPLAY_FEATURE_PRIVATE     *DisplayFeatureDataPtr;
  UINT8                           SciMap;

  DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;
  CpmTableProtocolPtr     = CpmDisplayFeatureProtocolPtr->TableProtocolPtr;
  PcieTopologyTablePtr    = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_TOPOLOGY);

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD603);

  Status      = EFI_SUCCESS;
  Bus         = 0;
  Dev         = 0;
  Func        = 0;
  VendorId    = 0;
  DeviceId    = 0;
  eNvStore    = 0;

  MainTablePtr            = CpmTableProtocolPtr->MainTablePtr;
  DisplayFeatureTablePtr  = DisplayFeatureDataPtr->DisplayFeatureTablePtr;
  DevicePathTablePtr      = DisplayFeatureDataPtr->DevicePathTablePtr;
  SpecificSsidTablePtr    = DisplayFeatureDataPtr->SpecificSsidTablePtr;
  RebrandDualGraphicsSsidTablePtr = DisplayFeatureDataPtr->RebrandDualGraphicsSsidTablePtr;

  if (DisplayFeatureTablePtr) {

    if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT0)) {
      if (MainTablePtr->DisplayFeature.Config.PowerXpressFixedMode || MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode) {
        Ssid = 0x01240000;
      } else if (MainTablePtr->DisplayFeature.Config.HyperCrossFire) {
        if (MainTablePtr->DisplayFeature.Config.IsDgpuPrimary) {
          Ssid = 0x01250000;
        } else {
          Ssid = 0x01260000;
        }
      } else {
        Ssid = 0x01230000;
      }

      DisplayFeatureDataPtr->Ssid = Ssid;

      for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
        if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist) {
          Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
          Dev       = DevicePathTablePtr->Path[Index].Device.Device;
          Func      = DevicePathTablePtr->Path[Index].Device.Function;
          VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x00);
          DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x02);
          DeviceId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func + 1, 0x02);
          NewSsid   = Ssid | (UINT32)VendorId;
          if (SpecificSsidTablePtr) {
            for (i = 0; i < AMD_SPECIFIC_SSID_DEVICE_SIZE; i++) {
              if (SpecificSsidTablePtr->Item[i].VendorId == 0xFFFF) {
                break;
              } else {
                if ((SpecificSsidTablePtr->Item[i].VendorId == VendorId) && (SpecificSsidTablePtr->Item[i].DeviceId == DeviceId)) {
                  CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x4C, NewSsid);
                }
                if ((DeviceId2 != 0xFFFF) && (SpecificSsidTablePtr->Item[i].VendorId == VendorId) && (SpecificSsidTablePtr->Item[i].DeviceId == DeviceId2)) {
                  CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func + 1, 0x4C, NewSsid);
                }
              }
            }
          } else {
            CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x4C, NewSsid);
          }
        }
      }
    }

    if (MainTablePtr->DisplayFeature.Config.RebrandDualGraphics && RebrandDualGraphicsSsidTablePtr) {
      Ssid = 0xFFFF0000;
      for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
        if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[Index].IsDgpu) {
          Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
          Dev       = DevicePathTablePtr->Path[Index].Device.Device;
          Func      = DevicePathTablePtr->Path[Index].Device.Function;
          VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x00);
          DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x02);

          for (i = 0; i < AMD_SPECIFIC_SSID_DEVICE_SIZE; i++) {
            if (RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == 0xFFFF) {
              break;
            } else {
              if ((RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == VendorId) &&
                  (RebrandDualGraphicsSsidTablePtr->Item[i].DeviceId == DeviceId) &&
                  (RebrandDualGraphicsSsidTablePtr->Item[i].IsDgpu == 0)) {
                  Ssid = ((UINT32) DeviceId) << 16;
                break;
              }
            }
          }
        }
      }
      DisplayFeatureDataPtr->Ssid2 = Ssid;
      if (Ssid != 0xFFFF0000) {
        for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
          if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
            break;
          }
          if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[Index].IsDgpu) {
            Bus       = (UINT8) DevicePathTablePtr->Path[Index].FeatureMask.Mask.Bus;
            Dev       = DevicePathTablePtr->Path[Index].Device.Device;
            Func      = DevicePathTablePtr->Path[Index].Device.Function;
            VendorId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x00);
            VendorId2 = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x2C);
            DeviceId  = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, Bus, Dev, Func, 0x02);
            NewSsid   = Ssid | (UINT32)VendorId2;

            for (i = 0; i < AMD_SPECIFIC_SSID_DEVICE_SIZE; i++) {
              if (RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == 0xFFFF) {
                break;
              } else {
                if ((RebrandDualGraphicsSsidTablePtr->Item[i].VendorId == VendorId) &&
                    (RebrandDualGraphicsSsidTablePtr->Item[i].DeviceId == DeviceId) &&
                    (RebrandDualGraphicsSsidTablePtr->Item[i].IsDgpu == 1)) {
                  CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x4C, NewSsid);
                  break;
                }
              }
            }
          }
        }
      }
    }

    Status = gBS->LocateProtocol (
                      &gAmdCpmNvDataProtocolGuid,
                      NULL,
                      &pAmdCpmNvDataProtocol
                      );

    if (!EFI_ERROR (Status)) {

      eNvStore = pAmdCpmNvDataProtocol->NvDataPtr;

#ifndef AMD_CPM_EDKII
      VbiosAddr = DisplayFeatureDataPtr->VBiosImage;
      if (VbiosAddr != 0xFFFFFFFF && VbiosAddr != 0) {
        VbiosSize = (*((UINT8*) VbiosAddr + 2)) << 9;
        eNvStore->CpmAtrmRomSize = (UINT32)VbiosSize;
        if (VbiosSize != 0 && VbiosSize <= 0x10000) {
          CpmTableProtocolPtr->CommonFunction.CopyMem (&eNvStore->CpmAtrmRomImage[0], (VOID *) (UINTN)VbiosAddr, (UINTN)VbiosSize);
        }
      }
#endif

//    if (DisplayFeatureDataPtr->GfxDevicePfa[1].Raw){
//      CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x04, 3);
//      VbiosAddr = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, Bus, Dev, Func, 0x10);
//      if (VbiosAddr != 0xFFFFFFFF )
//      {
//        VbiosAddr &= 0xFFFFFFF0;
//        VbiosSize = *(UINT16 *)((UINT8 *)(UINTN)(VbiosAddr + 0x44));
//        if (VbiosSize == 0x0000)
//        {
//          VbiosSize = *(UINT16 *)((UINT8 *)(UINTN)(VbiosAddr + 0x02));
//          VbiosSize <<= 9;
//        }
//        eNvStore->CpmAtrmRomSize = (UINT32)VbiosSize;
//        CpmTableProtocolPtr->CommonFunction.CopyMem (&eNvStore->CpmAtrmRomImage[0], (VOID *)(UINTN)VbiosAddr, (UINTN)VbiosSize);
//      }
//    }


      eNvStore->CpmDisplayFeatureConfig = MainTablePtr->DisplayFeature.Raw;

      // ATCS
      eNvStore->CpmAtcsSupportedFunctionMask = 0x0F;                                                    // AtcsSupportedFunctionMask;

      // AtpxSupportedFunctionMask
      if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress) {
        if (DisplayFeatureDataPtr->GfxDevicePfa[0].Raw && DisplayFeatureDataPtr->GfxDevicePfa[1].Raw) {
          if (DisplayFeatureTablePtr->MuxFlag) {
            eNvStore->CpmAtpxSupportedFunctionMask = 0x0000018F;
          } else {
            eNvStore->CpmAtpxSupportedFunctionMask = 0x00000003;
          }
        } else {
          eNvStore->CpmAtpxSupportedFunctionMask = 0;
        }
      } else {
        eNvStore->CpmAtpxSupportedFunctionMask = 0;
      }

      // AtpxFlags
      eNvStore->CpmAtpxFlags = 0;
      if (DisplayFeatureTablePtr->MuxFlag) {
        eNvStore->CpmAtpxFlags = DisplayFeatureTablePtr->MuxFlag;
      } else {
        if (MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode == 1) {
          eNvStore->CpmAtpxFlags = 0x00000680;                                                // PX Dynamic Mode Support Full dGPU Powerdown
        } else if (MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode == 2) {
          eNvStore->CpmAtpxFlags = 0x00000280;                                                // PX Dynamic Mode Support ULPS
        }
        if (MainTablePtr->DisplayFeature.Config.PowerXpressDynamicMode && MainTablePtr->DisplayFeature.Config.PowerXpressFixedMode) {
          eNvStore->CpmAtpxFlags &= 0xFFFFFDFF;
        }
        if (MainTablePtr->DisplayFeature.Config.DualGraphicsNotSupported) {
          eNvStore->CpmAtpxFlags |= 0x00000100;
        }
        if (MainTablePtr->DisplayFeature.Config.DgpuDisplayOutput) {
          if (MainTablePtr->DisplayFeature.Config.PulseGeneratorSupport) {
            eNvStore->CpmAtpxFlags |= 0x00001000;
          } else {
            eNvStore->CpmAtpxFlags |= 0x00001800;
          }
        }
      }

      // ATIF
      eNvStore->CpmAtifSupportedNotificationMask = DisplayFeatureTablePtr->AtifSupportedNotificationMask; // AtifSupportedNotificationMask
      if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress) {
        eNvStore->CpmAtifSupportedNotificationMask |= 0x40;
      } else {
        eNvStore->CpmAtifSupportedNotificationMask &= 0xDF;
      }
      if (MainTablePtr->DisplayFeature.Config.IsBrightnessByDriver) {
        eNvStore->CpmAtifSupportedNotificationMask |= 0x80;
      } else {
        eNvStore->CpmAtifSupportedNotificationMask &= 0x7F;
      }
      if (DisplayFeatureTablePtr->MxmOverTempEvent == 0xFF) {
        eNvStore->CpmAtifSupportedNotificationMask &= 0xFB;
      }
      if (MainTablePtr->DisplayFeature.Config.DgpuDisplayOutput) {
        eNvStore->CpmAtifSupportedNotificationMask |= 0x100;
      }

      eNvStore->CpmAtifFunctionBitVector = 0xC00F;                                                        // AtifFunctionBitVector
      eNvStore->CpmAtifFlags = 0x00000001;                                                                // AtifFlags

      GfxDetect.NumDevice         = 0;
      GfxDetect.DeviceStructSize  = 8;
      for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
        if (DevicePathTablePtr->Path[i].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
          if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress) {
            GfxDetect.Device[GfxDetect.NumDevice].Flags   = DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga << 2;
          } else {
            GfxDetect.Device[GfxDetect.NumDevice].Flags   = DevicePathTablePtr->Path[i].FeatureMask.Mask.Removable | (DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga << 2);
          }
          GfxDetect.Device[GfxDetect.NumDevice].Bus     = (UINT16)DevicePathTablePtr->Path[i].FeatureMask.Mask.Bus;
          GfxDetect.Device[GfxDetect.NumDevice].Device  = DevicePathTablePtr->Path[i].Device.Device;
          GfxDetect.NumDevice++;
        }
      }

      CpmTableProtocolPtr->CommonFunction.CopyMem (&eNvStore->CpmAtifDeviceListBuffer[0],
                      &GfxDetect,
                      sizeof(AMD_CPM_GFX_DETECT));                                                        // AtifDeviceListBuffer

      eNvStore->CpmiGpuP2pBridgePfa = DisplayFeatureDataPtr->GfxBridgePfa[0].Raw;                         // iGpuP2pBridgePfa
      eNvStore->CpmiGpuP2pDevicePfa = DisplayFeatureDataPtr->GfxDevicePfa[0].Raw;                         // iGpuP2pDevicePfa
      eNvStore->CpmdGpuP2pBridgePfa = DisplayFeatureDataPtr->GfxBridgePfa[1].Raw;                         // dGpuP2pBridgePfa
      eNvStore->CpmdGpuP2pDevicePfa = DisplayFeatureDataPtr->GfxDevicePfa[1].Raw;                         // dGpuP2pDevicePfa

      eNvStore->CpmdGpuAspmLxEnable = 0;                                                                  // dGpuAspmLxEnable
      if (PcieTopologyTablePtr && eNvStore->CpmdGpuP2pBridgePfa) {
        PciePortDescriptorPtr   = &PcieTopologyTablePtr->Port[0];
        while (TRUE) {
          if (PciePortDescriptorPtr->Port.DeviceNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device) {
            if (PciePortDescriptorPtr->Port.FunctionNumber == DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function) {
              eNvStore->CpmdGpuAspmLxEnable = CpmTableProtocolPtr->CommonFunction.PciRead8 (CpmTableProtocolPtr,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Bus,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Device,
                      DisplayFeatureDataPtr->GfxBridgePfa[1].Pfa.Function,
                      0x68);
              break;
            }
          }
          if (PciePortDescriptorPtr->Flags & DESCRIPTOR_TERMINATE_LIST) {
            break;
          }
          PciePortDescriptorPtr ++;
        }
      }

      eNvStore->CpmdGpuAudioDisable = 0;
      if (DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress && MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX && !MainTablePtr->DisplayFeature.Config.DgpuDisplayOutput) {
        eNvStore->CpmdGpuAudioDisable = 1;
      }

      Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                      NULL,
                                      &CpmTableProtocolPtr
                                      );
      if (EFI_ERROR (Status)) {
        return;
      }

      iGpuIndex = 0xFF;
      dGpuIndex = 0xFF;
      i = 0;
      while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
        if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
          break;
        }
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && !DevicePathTablePtr->Path[i].IsDgpu) {
          iGpuIndex = i;
        }
        if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[i].IsDgpu && DevicePathTablePtr->Path[i].FeatureMask.Mask.Vga) {
          dGpuIndex = i;
        }
        i++;
      }
      if (iGpuIndex == 0xFF) {
        MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 1;
      }
      if (dGpuIndex == 0xFF) {
        i = 0;
        while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
          if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
            break;
          }
          if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist && DevicePathTablePtr->Path[i].IsDgpu) {
            dGpuIndex = i;
            break;
          }
          i++;
        }
      }
      if (dGpuIndex == 0xFF) {
        MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 0;
        i = 0;
        while (i < AMD_DISPLAY_DEVICE_PATH_SIZE) {
          if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
            break;
          }
          if (DevicePathTablePtr->Path[i].IsDgpu) {
            dGpuIndex = i;
            break;
          }
          i++;
        }
      }

      eNvStore->CpmDisplayFeatureConfig = MainTablePtr->DisplayFeature.Raw;

      // Patch PX ASL SSDT Table
      if (!(DisplayFeatureTablePtr->FunctionDisableMask & BIT1)) {
        for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {

          if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
            if (DevicePathTablePtr->Path[i].IsDgpu) {
              Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                                CpmTableProtocolPtr,
                                                                DevicePathTablePtr->Path[i].Bridge.Device,
                                                                DevicePathTablePtr->Path[i].Bridge.Function
                                                                );
              CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                                  &DISPLAY_FEATURE_D_ACPI_DEVICE_GUID,
                                                                  NULL,
                                                                  CpmDGpuSsdtCallBack,
                                                                  &Buffer[0]
                                                                );
            } else {
              if ((DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress || DisplayFeatureDataPtr->CurrentFeature.Mask.HyperCrossFire) && (!MainTablePtr->DisplayFeature.Config.IsDgpuPrimary)) {
                Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                                CpmTableProtocolPtr,
                                                                DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                DevicePathTablePtr->Path[dGpuIndex].Bridge.Function
                                                                );
                if (DisplayFeatureDataPtr->VBiosFlag == 1 && DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress) {
                  Buffer[1] = 0;
                } else {
                  Buffer[1] = 1;
                }
                CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                                    &DISPLAY_FEATURE_I_ACPI_DEVICE_GUID,
                                                                    NULL,
                                                                    CpmIGpuSsdtCallBack,
                                                                    &Buffer[0]
                                                                 );
              } else {
                Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                                CpmTableProtocolPtr,
                                                                DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                                DevicePathTablePtr->Path[dGpuIndex].Bridge.Function
                                                                );
                Buffer[1] = 0;
                CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                                    &DISPLAY_FEATURE_I_ACPI_DEVICE_GUID,
                                                                    NULL,
                                                                    CpmIGpuSsdtCallBack,
                                                                    &Buffer[0]
                                                                  );
              }
              iGpuIndex = 0;
            }
          } else {
            if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
              break;
            }
          }
        }
        if (iGpuIndex == 0xFF && dGpuIndex != 0xFF) {
          Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                          CpmTableProtocolPtr,
                                                          DevicePathTablePtr->Path[dGpuIndex].Bridge.Device,
                                                          DevicePathTablePtr->Path[dGpuIndex].Bridge.Function
                                                          );
          Buffer[1] = 0;
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                              &DISPLAY_FEATURE_I_ACPI_DEVICE_GUID,
                                                              NULL,
                                                              CpmIGpuSsdtCallBack,
                                                              &Buffer[0]
                                                            );
        }
        if (DisplayFeatureTablePtr->MxmOverTempEvent != 0xFF) {
          SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, DisplayFeatureTablePtr->MxmOverTempEvent);
          if ((SciMap & 0xF) < 0xA) {
            Buffer[0] = 0x3000 + (((SciMap) & 0xF) << 8);
          } else {
            Buffer[0] = 0x4000 + ((((SciMap) & 0xF) - 0x9) << 8);
          }
          if (((SciMap >> 4) & 0xF) < 0xA) {
            Buffer[0] |= 0x30 + ((SciMap >> 4) & 0xF);
          } else {
            Buffer[0] |= 0x40 + (((SciMap >> 4) & 0xF) - 0x9);
          }
          Buffer[0] = (Buffer[0] << 16) + (((UINT32)'L') << 8) + (UINT32)'_';
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                              &DISPLAY_FEATURE_MXM_OVERT_ACPI_DEVICE_GUID,
                                                              NULL,
                                                              CpmMxmOverTSsdtCallBack,
                                                              &Buffer[0]
                                                            );
        }

        if (DisplayFeatureTablePtr->DisplayConnectEvent != 0xFF && DisplayFeatureDataPtr->CurrentFeature.Mask.PowerXpress && MainTablePtr->DisplayFeature.Config.DgpuDisplayOutput) {
          SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, DisplayFeatureTablePtr->DisplayConnectEvent);
          if ((SciMap & 0xF) < 0xA) {
            Buffer[0] = 0x3000 + (((SciMap) & 0xF) << 8);
          } else {
            Buffer[0] = 0x4000 + ((((SciMap) & 0xF) - 0x9) << 8);
          }
          if (((SciMap >> 4) & 0xF) < 0xA) {
            Buffer[0] |= 0x30 + ((SciMap >> 4) & 0xF);
          } else {
            Buffer[0] |= 0x40 + (((SciMap >> 4) & 0xF) - 0x9);
          }
          Buffer[0] = (Buffer[0] << 16) + (((UINT32)'L') << 8) + (UINT32)'_';
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                              &DISPLAY_FEATURE_DISPLAY_CONNECT_EVENT_ACPI_DEVICE_GUID,
                                                              NULL,
                                                              CpmDisplayConnectEventSsdtCallBack,
                                                              &Buffer[0]
                                                            );
        }
      }
    }

  }

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD703);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for iGPU
 *
 * This function is used to update the name of PCIe bridge for dGPU in ASL
 * and renames XTRM to ATRM if PowerXpress is enabled.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
CpmIGpuSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;

  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('X', 'T', 'R', 'M'):
    if (*(BufferPtr + 1)) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('A', 'T', 'R', 'M');
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):
    if (*BufferPtr) {
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for dGPU
 *
 * This function is used to update the name of PCIe bridge for dGPU in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
CpmDGpuSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('P', 'B', '2', '_'):
    if (*BufferPtr) {
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for MXM_OVERT
 *
 * This function is used to update the name of GPE method in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
CpmMxmOverTSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('M', '2', '1', '7'):     // CpmMxmOverTempEventMethod: M217
    if (*BufferPtr) {
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Display Feature SSDT table for Discrete GPU
 * connect/disconnect event
 *
 * This function is used to update the name of GPE method in ASL
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
CpmDisplayConnectEventSsdtCallBack (
  IN        AMD_CPM_TABLE_PROTOCOL  *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32    *BufferPtr;
  BufferPtr = (UINT32*)Context;
  switch ( *((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('M', '2', '1', '8'):     // CpmDisplayConnectEventMethod: M218
    if (*BufferPtr) {
      *((UINT32*)AmlObjPtr) = *BufferPtr;
    }
    break;
  }
  return FALSE;
}
