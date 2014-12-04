/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM structures and definitions
 *
 * Contains AMD CPM CPU/APU Option Definitions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Include
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

#ifndef _OPTION_CPU_INSTALL_H_
#define _OPTION_CPU_INSTALL_H_

#ifdef AMD_CPM_CPU_FAMILY_0x14_ON_SUPPORT
  #ifndef AMD_CPM_PCIE_GPP0_NAME_ON
    #define AMD_CPM_PCIE_GPP0_NAME_ON               AMD_CPM_PCIE_GPP0_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_ON
    #define AMD_CPM_PCIE_GPP1_NAME_ON               AMD_CPM_PCIE_GPP1_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_ON
    #define AMD_CPM_PCIE_GPP2_NAME_ON               AMD_CPM_PCIE_GPP2_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_ON
    #define AMD_CPM_PCIE_GPP3_NAME_ON               AMD_CPM_PCIE_GPP3_NAME_1
  #endif

  CPU_REVISION_ITEM CpuRevisionTableON[] = {
    { 0x0FFF0F00, 0x00510F00, CPM_CPU_REVISION_ID_ON, CPM_PCIE_REVISION_ID_ON},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameON[] = {
    {CPM_PCIE_REVISION_ID_ON, 4, 0, 0x20, AMD_CPM_PCIE_GPP0_NAME_ON},
    {CPM_PCIE_REVISION_ID_ON, 5, 0, 0x21, AMD_CPM_PCIE_GPP1_NAME_ON},
    {CPM_PCIE_REVISION_ID_ON, 6, 0, 0x22, AMD_CPM_PCIE_GPP2_NAME_ON},
    {CPM_PCIE_REVISION_ID_ON, 7, 0, 0x23, AMD_CPM_PCIE_GPP3_NAME_ON},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_ON           CpuRevisionTableON,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_ON          PcieBridgeNameON,
#else
  #define OPTION_CPM_CPU_REVISION_ID_ON
  #define OPTION_CPM_PCIE_BRIDGE_NAME_ON
#endif

#ifdef AMD_CPM_CPU_FAMILY_0x15_TN_SUPPORT
  #ifndef AMD_CPM_PCIE_GFX0_NAME_TN
    #define AMD_CPM_PCIE_GFX0_NAME_TN               AMD_CPM_PCIE_GFX0_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GFX1_NAME_TN
    #define AMD_CPM_PCIE_GFX1_NAME_TN               AMD_CPM_PCIE_GFX1_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP0_NAME_TN
    #define AMD_CPM_PCIE_GPP0_NAME_TN               AMD_CPM_PCIE_GPP0_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_TN
    #define AMD_CPM_PCIE_GPP1_NAME_TN               AMD_CPM_PCIE_GPP1_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_TN
    #define AMD_CPM_PCIE_GPP2_NAME_TN               AMD_CPM_PCIE_GPP2_NAME_1
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_TN
    #define AMD_CPM_PCIE_GPP3_NAME_TN               AMD_CPM_PCIE_GPP3_NAME_1
  #endif

  CPU_REVISION_ITEM CpuRevisionTableTN[] = {
    { 0x0FFF0F00, 0x00610F00, CPM_CPU_REVISION_ID_TN, CPM_PCIE_REVISION_ID_TN},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameTN[] = {
    {CPM_PCIE_REVISION_ID_TN, 2, 0, 0x10, AMD_CPM_PCIE_GFX0_NAME_TN},
    {CPM_PCIE_REVISION_ID_TN, 3, 0, 0x11, AMD_CPM_PCIE_GFX1_NAME_TN},
    {CPM_PCIE_REVISION_ID_TN, 4, 0, 0x20, AMD_CPM_PCIE_GPP0_NAME_TN},
    {CPM_PCIE_REVISION_ID_TN, 5, 0, 0x21, AMD_CPM_PCIE_GPP1_NAME_TN},
    {CPM_PCIE_REVISION_ID_TN, 6, 0, 0x22, AMD_CPM_PCIE_GPP2_NAME_TN},
    {CPM_PCIE_REVISION_ID_TN, 7, 0, 0x23, AMD_CPM_PCIE_GPP3_NAME_TN},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_TN           CpuRevisionTableTN,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_TN          PcieBridgeNameTN,
#else
  #define OPTION_CPM_CPU_REVISION_ID_TN
  #define OPTION_CPM_PCIE_BRIDGE_NAME_TN
#endif

#ifdef AMD_CPM_CPU_FAMILY_0x15_KV_SUPPORT
  #ifndef AMD_CPM_PCIE_GFX0_NAME_KV
    #define AMD_CPM_PCIE_GFX0_NAME_KV               AMD_CPM_PCIE_GFX0_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GFX1_NAME_KV
    #define AMD_CPM_PCIE_GFX1_NAME_KV               AMD_CPM_PCIE_GFX1_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GPP0_NAME_KV
    #define AMD_CPM_PCIE_GPP0_NAME_KV               AMD_CPM_PCIE_GPP0_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_KV
    #define AMD_CPM_PCIE_GPP1_NAME_KV               AMD_CPM_PCIE_GPP1_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_KV
    #define AMD_CPM_PCIE_GPP2_NAME_KV               AMD_CPM_PCIE_GPP2_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_KV
    #define AMD_CPM_PCIE_GPP3_NAME_KV               AMD_CPM_PCIE_GPP3_NAME_2
  #endif
  #ifndef AMD_CPM_PCIE_GPP4_NAME_KV
    #define AMD_CPM_PCIE_GPP4_NAME_KV               AMD_CPM_PCIE_GPP4_NAME_2
  #endif

  CPU_REVISION_ITEM CpuRevisionTableKV[] = {
    { 0x0FFF0F00, 0x00630F00, CPM_CPU_REVISION_ID_KV, CPM_PCIE_REVISION_ID_KV},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameKV[] = {
    {CPM_PCIE_REVISION_ID_KV, 2, 1, 0x10, AMD_CPM_PCIE_GFX0_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 2, 2, 0x11, AMD_CPM_PCIE_GFX1_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 3, 1, 0x20, AMD_CPM_PCIE_GPP0_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 3, 2, 0x21, AMD_CPM_PCIE_GPP1_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 3, 3, 0x22, AMD_CPM_PCIE_GPP2_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 3, 4, 0x23, AMD_CPM_PCIE_GPP3_NAME_KV},
    {CPM_PCIE_REVISION_ID_KV, 3, 5, 0x24, AMD_CPM_PCIE_GPP4_NAME_KV},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_KV           CpuRevisionTableKV,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_KV          PcieBridgeNameKV,
#else
  #define OPTION_CPM_CPU_REVISION_ID_KV
  #define OPTION_CPM_PCIE_BRIDGE_NAME_KV
#endif

#ifdef AMD_CPM_CPU_FAMILY_0x15_CZ_SUPPORT
  #ifndef AMD_CPM_PCIE_GFX0_NAME_CZ
    #define AMD_CPM_PCIE_GFX0_NAME_CZ               AMD_CPM_PCIE_GFX0_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GFX1_NAME_CZ
    #define AMD_CPM_PCIE_GFX1_NAME_CZ               AMD_CPM_PCIE_GFX1_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GFX2_NAME_CZ
    #define AMD_CPM_PCIE_GFX2_NAME_CZ               AMD_CPM_PCIE_GFX2_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GFX3_NAME_CZ
    #define AMD_CPM_PCIE_GFX3_NAME_CZ               AMD_CPM_PCIE_GFX3_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GFX4_NAME_CZ
    #define AMD_CPM_PCIE_GFX4_NAME_CZ               AMD_CPM_PCIE_GFX4_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GPP0_NAME_CZ
    #define AMD_CPM_PCIE_GPP0_NAME_CZ               AMD_CPM_PCIE_GPP0_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_CZ
    #define AMD_CPM_PCIE_GPP1_NAME_CZ               AMD_CPM_PCIE_GPP1_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_CZ
    #define AMD_CPM_PCIE_GPP2_NAME_CZ               AMD_CPM_PCIE_GPP2_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_CZ
    #define AMD_CPM_PCIE_GPP3_NAME_CZ               AMD_CPM_PCIE_GPP3_NAME_4
  #endif
  #ifndef AMD_CPM_PCIE_GPP4_NAME_CZ
    #define AMD_CPM_PCIE_GPP4_NAME_CZ               AMD_CPM_PCIE_GPP4_NAME_4
  #endif

  CPU_REVISION_ITEM CpuRevisionTableCZ[] = {
    { 0x0FFF0F00, 0x00660F00, CPM_CPU_REVISION_ID_CZ, CPM_PCIE_REVISION_ID_CZ},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameCZ[] = {
    {CPM_PCIE_REVISION_ID_CZ, 3, 1, 0x10, AMD_CPM_PCIE_GFX0_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 3, 2, 0x11, AMD_CPM_PCIE_GFX1_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 3, 3, 0x12, AMD_CPM_PCIE_GFX2_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 3, 4, 0x13, AMD_CPM_PCIE_GFX3_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 3, 5, 0x14, AMD_CPM_PCIE_GFX4_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 2, 1, 0x20, AMD_CPM_PCIE_GPP0_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 2, 2, 0x21, AMD_CPM_PCIE_GPP1_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 2, 3, 0x22, AMD_CPM_PCIE_GPP2_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 2, 4, 0x23, AMD_CPM_PCIE_GPP3_NAME_CZ},
    {CPM_PCIE_REVISION_ID_CZ, 2, 5, 0x24, AMD_CPM_PCIE_GPP4_NAME_CZ},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_CZ           CpuRevisionTableCZ,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_CZ          PcieBridgeNameCZ,
#else
  #define OPTION_CPM_CPU_REVISION_ID_CZ
  #define OPTION_CPM_PCIE_BRIDGE_NAME_CZ
#endif

#ifdef AMD_CPM_CPU_FAMILY_0x16_KB_SUPPORT
  #ifndef AMD_CPM_PCIE_GPP0_NAME_KB
    #define AMD_CPM_PCIE_GPP0_NAME_KB               AMD_CPM_PCIE_GPP0_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_KB
    #define AMD_CPM_PCIE_GPP1_NAME_KB               AMD_CPM_PCIE_GPP1_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_KB
    #define AMD_CPM_PCIE_GPP2_NAME_KB               AMD_CPM_PCIE_GPP2_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_KB
    #define AMD_CPM_PCIE_GPP3_NAME_KB               AMD_CPM_PCIE_GPP3_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GFX_NAME_KB
    #define AMD_CPM_PCIE_GFX_NAME_KB                AMD_CPM_PCIE_GFX_NAME_3
  #endif

  CPU_REVISION_ITEM CpuRevisionTableKB[] = {
    { 0x0FFF0F00, 0x00700F00, CPM_CPU_REVISION_ID_KB, CPM_PCIE_REVISION_ID_KB},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameKB[] = {
    {CPM_PCIE_REVISION_ID_KB, 2, 1, 0x10, AMD_CPM_PCIE_GFX_NAME_KB},
    {CPM_PCIE_REVISION_ID_KB, 2, 2, 0x20, AMD_CPM_PCIE_GPP0_NAME_KB},
    {CPM_PCIE_REVISION_ID_KB, 2, 3, 0x21, AMD_CPM_PCIE_GPP1_NAME_KB},
    {CPM_PCIE_REVISION_ID_KB, 2, 4, 0x22, AMD_CPM_PCIE_GPP2_NAME_KB},
    {CPM_PCIE_REVISION_ID_KB, 2, 5, 0x23, AMD_CPM_PCIE_GPP3_NAME_KB},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_KB           CpuRevisionTableKB,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_KB          PcieBridgeNameKB,
#else
  #define OPTION_CPM_CPU_REVISION_ID_KB
  #define OPTION_CPM_PCIE_BRIDGE_NAME_KB
#endif

#ifdef AMD_CPM_CPU_FAMILY_0x16_ML_SUPPORT
  #ifndef AMD_CPM_PCIE_GPP0_NAME_ML
    #define AMD_CPM_PCIE_GPP0_NAME_ML               AMD_CPM_PCIE_GPP0_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP1_NAME_ML
    #define AMD_CPM_PCIE_GPP1_NAME_ML               AMD_CPM_PCIE_GPP1_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP2_NAME_ML
    #define AMD_CPM_PCIE_GPP2_NAME_ML               AMD_CPM_PCIE_GPP2_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GPP3_NAME_ML
    #define AMD_CPM_PCIE_GPP3_NAME_ML               AMD_CPM_PCIE_GPP3_NAME_3
  #endif
  #ifndef AMD_CPM_PCIE_GFX_NAME_ML
    #define AMD_CPM_PCIE_GFX_NAME_ML                AMD_CPM_PCIE_GFX_NAME_3
  #endif

  CPU_REVISION_ITEM CpuRevisionTableML[] = {
    { 0x0FFF0F00, 0x00730F00, CPM_CPU_REVISION_ID_ML, CPM_PCIE_REVISION_ID_ML},
    0
  };

  PCIE_BRIDGE_NAME PcieBridgeNameML[] = {
    {CPM_PCIE_REVISION_ID_ML, 2, 1, 0x10, AMD_CPM_PCIE_GFX_NAME_ML},
    {CPM_PCIE_REVISION_ID_ML, 2, 2, 0x20, AMD_CPM_PCIE_GPP0_NAME_ML},
    {CPM_PCIE_REVISION_ID_ML, 2, 3, 0x21, AMD_CPM_PCIE_GPP1_NAME_ML},
    {CPM_PCIE_REVISION_ID_ML, 2, 4, 0x22, AMD_CPM_PCIE_GPP2_NAME_ML},
    {CPM_PCIE_REVISION_ID_ML, 2, 5, 0x23, AMD_CPM_PCIE_GPP3_NAME_ML},
    0
  };

  #define OPTION_CPM_CPU_REVISION_ID_ML           CpuRevisionTableML,
  #define OPTION_CPM_PCIE_BRIDGE_NAME_ML          PcieBridgeNameML,
#else
  #define OPTION_CPM_CPU_REVISION_ID_ML
  #define OPTION_CPM_PCIE_BRIDGE_NAME_ML
#endif

#endif
