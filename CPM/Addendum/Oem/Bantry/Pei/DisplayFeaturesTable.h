/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for display feature.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
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

#ifndef _DISPLAY_FEATURE_TABLES_H_
#define _DISPLAY_FEATURE_TABLES_H_
#include "AmdCpmOemInitPeim.h"

//
// CPM Display Feature Module
//
//
// CPM Device Path Table
//
AMD_CPM_DEVICE_PATH_TABLE gCpmDevicePathTable = {
  {CPM_SIGNATURE_DEVICE_PATH, sizeof (gCpmDevicePathTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    {DFS_VALID + DFS_POWER_EXPRESS + DFS_SURROUND_VIEW + DFS_HYPER_CROSS_FIRE, DFS_iGPU, {0, 0}, {1, 0}},                                     // PowerXpress|HCF, iGPU, (0,0,0)/(1,0)
    {DFS_VALID + DFS_POWER_EXPRESS + DFS_SURROUND_VIEW + DFS_HYPER_CROSS_FIRE, DFS_dGPU, {APU_GFX0_DEV_TN, APU_GFX0_FUNC_TN}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,2,0)/(0,0)
    {DFS_VALID + DFS_POWER_EXPRESS + DFS_SURROUND_VIEW + DFS_HYPER_CROSS_FIRE, DFS_dGPU, {APU_GFX1_DEV_TN, APU_GFX1_FUNC_TN}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {DFS_VALID + DFS_POWER_EXPRESS + DFS_SURROUND_VIEW + DFS_HYPER_CROSS_FIRE, DFS_dGPU, {APU_GFX0_DEV_KV, APU_GFX0_FUNC_KV}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {DFS_VALID + DFS_POWER_EXPRESS + DFS_SURROUND_VIEW + DFS_HYPER_CROSS_FIRE, DFS_dGPU, {APU_GFX1_DEV_KV, APU_GFX1_FUNC_KV}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {0x00000000, 0x00, {0, 0}, {0, 0}},
  }
};


//
// CPM Display Feature Table
//
AMD_CPM_DISPLAY_FEATURE_TABLE gCpmDisplayFeatureTable = {
  {CPM_SIGNATURE_DISPLAY_FEATURE, sizeof (gCpmDisplayFeatureTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0,                                          // FunctionDisableMask
  DEVICE_ID_GFX0,                             // MXM Device Id
  0xFF,                                       // MXM_OVERT#
  0,                                          // Forced Power State Id if MXM_OVERT# != 0xFF
  0xFF,                                       // Display Connect Event
  0,                                          // Device Id for Docking Pin
  0,                                          // MuxFlag
  0,                                          // No Display Mux
  0,                                          //
  10,                                         // AtpxConnector8Number
  {                                           // AtpxConnector8
    {0x05, 0x00, 0x00, 0x0110},                 // Connector #0: LCD1 on iGPU
    {0x05, 0x01, 0x00, 0x0100},                 // Connector #1: CRT1 on iGPU
    {0x07, 0x03, 0x00, 0x0210},                 // Connector #2: DFP1 on iGPU
    {0x07, 0x07, 0x00, 0x0220},                 // Connector #3: DFP2 on iGPU
    {0x00, 0x09, 0x00, 0x0230},                 // Connector #4: DFP3 on iGPU
    {0x01, 0x00, 0x01, 0x0110},                 // Connector #5: LCD1 on dGPU
    {0x01, 0x01, 0x01, 0x0100},                 // Connector #6: CRT1 on dGPU
    {0x03, 0x03, 0x01, 0x0210},                 // Connector #7: DFP1 on dGPU
    {0x03, 0x07, 0x01, 0x0220},                 // Connector #8: DFP2 on dGPU
    {0x00, 0x09, 0x01, 0x0230},                 // Connector #9: DFP3 on dGPU
  },
  0,                                          // AtpxConnector9Number
  {                                           // AtpxConnector9
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
  },
  DISPLAY_SWITCH_REQUEST,                     // AtifSupportedNotificationMask;
  7,                                          // AtifDeviceCombinationNumber;
  {
    0x01, 0x02, 0x08, 0x80, 0x03, 0x09, 0x81    // AtifDeviceCombinationBuffer[20];
  },
  {
    0x6C, 0x00,                               // WORD Structure Size : 0x6A
    0x00, 0x00,                               // WORD Flags          : Reserved
    0x00,                                     // BYTE        Error Code     : 0x00
    0x64,                                     // BYTE        AC Level       : 100%
    0x20,                                     // BYTE        DC Level       : 32%
    0x0C,                                     // BYTE        Minimum signal : 12
    0xFF,                                     // BYTE        Maximum signal : 255
    0x31,                                     // BYTE        Count          : 49
    0x02, 0x0E,                               // BYTE-BYTE    First Lumi/Signal: 2% - 14
    0x04, 0x10,                               // BYTE-BYTE
    0x06, 0x12,                               // BYTE-BYTE
    0x08, 0x15,                               // BYTE-BYTE
    0x0A, 0x17,                               // BYTE-BYTE
    0x0C, 0x1A,                               // BYTE-BYTE
    0x0E, 0x1D,                               // BYTE-BYTE
    0x10, 0x20,                               // BYTE-BYTE
    0x12, 0x23,                               // BYTE-BYTE
    0x14, 0x26,                               // BYTE-BYTE
    0x16, 0x29,                               // BYTE-BYTE
    0x18, 0x2C,                               // BYTE-BYTE
    0x1A, 0x30,                               // BYTE-BYTE
    0x1C, 0x34,                               // BYTE-BYTE
    0x1E, 0x37,                               // BYTE-BYTE
    0x20, 0x3B,                               // BYTE-BYTE
    0x22, 0x3E,                               // BYTE-BYTE
    0x24, 0x43,                               // BYTE-BYTE
    0x26, 0x47,                               // BYTE-BYTE
    0x28, 0x4B,                               // BYTE-BYTE
    0x2A, 0x50,                               // BYTE-BYTE
    0x2C, 0x54,                               // BYTE-BYTE
    0x2E, 0x58,                               // BYTE-BYTE
    0x30, 0x5D,                               // BYTE-BYTE
    0x32, 0x62,                               // BYTE-BYTE
    0x34, 0x67,                               // BYTE-BYTE
    0x36, 0x6C,                               // BYTE-BYTE
    0x38, 0x71,                               // BYTE-BYTE
    0x3A, 0x76,                               // BYTE-BYTE
    0x3C, 0x7B,                               // BYTE-BYTE
    0x3E, 0x81,                               // BYTE-BYTE
    0x40, 0x87,                               // BYTE-BYTE
    0x42, 0x8C,                               // BYTE-BYTE
    0x44, 0x92,                               // BYTE-BYTE
    0x46, 0x98,                               // BYTE-BYTE
    0x48, 0x9E,                               // BYTE-BYTE
    0x4A, 0xA4,                               // BYTE-BYTE
    0x4C, 0xAB,                               // BYTE-BYTE
    0x4E, 0xB1,                               // BYTE-BYTE
    0x50, 0xB7,                               // BYTE-BYTE
    0x52, 0xBE,                               // BYTE-BYTE
    0x54, 0xC5,                               // BYTE-BYTE
    0x56, 0xCC,                               // BYTE-BYTE
    0x58, 0xD3,                               // BYTE-BYTE
    0x5A, 0xDA,                               // BYTE-BYTE
    0x5C, 0xE1,                               // BYTE-BYTE
    0x5E, 0xE8,                               // BYTE-BYTE
    0x60, 0xF0,                               // BYTE-BYTE
    0x62, 0xF7                                // BYTE-BYTE   Last Lumi/Signal: 98% - 250
  }
};
#endif