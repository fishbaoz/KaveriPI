/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Kaveri PCI tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 285517 $   @e \$Date: 2014-02-21 14:15:42 -0600 (Fri, 21 Feb 2014) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "Ids.h"
#include "amdlib.h"
#include "cpuServices.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "cpuPstateTables.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15KvPowerMgmt.h"
#include "Table.h"
#include "Gnb.h"
#include "GnbGfx.h"
#include "GnbRegistersKV.h"
#include "GnbPcieConfig.h"
#include "GnbRegisterAccKV.h"
#include "GnbHandleLib.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVPCITABLES_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE            PstateFamilyServiceTable;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
F15KvNbSynPtrAdj (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

//  P C I    T a b l e s
// ----------------------

STATIC CONST TABLE_ENTRY_FIELDS ROMDATA F15KvPciRegistersBeforeApLaunch[] =
{
// D18F0x68 - Link Transaction Control
// bits[22:21] DsNpReqLmt = 10b
// bit [19]    ApicExtSpur = 1
// bit [18]    ApicExtId = 1
// bit [17]    ApicExtBrdCst = 1
// bit [15]    LimitCldtCfg = 1
// bit [11]    RespPassPW = 1
// bit [10]    DisFillP = 0
// bit [4]     DisMTS = 1
// bit [3]     DisWrDwP = 0
// bit [2]     DisWrBP = 0
// bit [1]     DisRdDwP = 0
// bit [0]     DisRdBP = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x68),  // Address
      0x004E8810,                           // regData
      0x006E8C1F,                           // regMask
    }}
  }
};

STATIC CONST TABLE_ENTRY_FIELDS ROMDATA F15KvPciRegistersAfterApLaunch[] =
{
// D18F0x6C - Link Initialization Control
// bit[0] RouteTblDis = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C),  // Address
      0x00000000,                           // regData
      0x00000001,                           // regMask
    }}
  },
// D18F0x84 - Link Control
// bits[12]       IsocEn             = 0x1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x84),  // Address
      0x00001000,                           // RegData
      0x00001000,                           // RegMask
    }}
  },
// D18F0x90 - ONION Link Base Channel Buffer Count
// bit[30]     PReq[3] = 1
// bits[29:28] NpReqData[3:2] = 0
// bits[27:25] FreeData = 3
// bits[24:20] FreeCmd = 1
// bits[19:18] RspData = 1
// bits[17:16] NpReqData[1:0] = 1
// bits[15:12] ProbeCmd = 0
// bits[11:8]  RspCmd = 2
// bits[7:5]   PReq[2:0] = 2
// bits[4:0]   NpReqCmd = 0xA
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x90),  // Address
      0x4615024A,                           // regData
      0x7FFFFFFF,                           // regMask
    }}
  },
// D18F0xB0 - ONIONPlus Link Base Channel Buffer Count
// bit[30]     PReq[3] = 0
// bits[29:28] NpReqData[3:2] = 3
// bits[27:25] FreeData = 1
// bits[24:20] FreeCmd = 0
// bits[19:18] RspData = 0
// bits[17:16] NpReqData[1:0] = 3
// bits[15:12] ProbeCmd = 0
// bits[11:8]  RspCmd = 0
// bits[7:5]   PReq[2:0] = 0
// bits[4:0]   NpReqCmd = 0x18
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0xB0),  // Address
      0x32030018,                           // regData
      0x7FFFFFFF,                           // regMask
    }}
  },
// D18F0x94 - ONION Link Isochronous Channel Buffer Count
// bits[28:27] IsocRspData = 0
// bits[26:25] IsocNpReqData = 1
// bits[24:22] IsocRspCmd = 0
// bits[21:19] IsocPReq = 0
// bits[18:16] IsocNpReqCmd = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x94),  // Address
      0x02010000,                           // regData
      0x1FFF0000,                           // regMask
    }}
  },
// D18F0xB4 - ONIONPlus Link Isochronous Channel Buffer Count
// bits[28:27] IsocRspData = 0
// bits[26:25] IsocNpReqData = 0
// bits[24:22] IsocRspCmd = 0
// bits[21:19] IsocPReq = 0
// bits[18:16] IsocNpReqCmd = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0xB4),  // Address
      0x00000000,                           // regData
      0x1FFF0000,                           // regMask
    }}
  },
// D18F0x110 - Link Clumping Enable for ONION link.
// bits [31:1]   ClumpEn             = 0x1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x110), // Address
      0x00000002,                           // regData
      0xFFFFFFFE,                           // regMask
    }}
  },
// D18F0x168
// bit [20]                           = 1
// bit [15]                           = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
        MAKE_SBDFO (0, 0, 24, FUNC_0, 0x168),// Address
        0x00100000,                         // regData
        0x00108000,                         // regMask
    }}
  },

// D18F1xE0 - Configuration Map
// bits[31:24] BusNumLimit = F8
// bits[23:16] BusNumBase = 0
// bit [1]     WE = 1
// bit [0]     RE = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
        MAKE_SBDFO (0, 0, 24, FUNC_1, 0xE0),// Address
        0xF8000003,                         // regData
        0xFFFF0003,                         // regMask
    }}
  },
// D18F2x118 - Memory Controller Configuration Low Register
// bits[31:28] MctVarPriCntLmt = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                        // CpuFamily
      AMD_F15_KV_ALL                           // CpuRevision
    },
    {AMD_PF_ALL},                              // platform Features
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_2, 0x118),    // Address
      0x10000000,                              // regData
      0xF0000000,                              // regMask
    }}
  },
// D18F2x1B0 - Extended Memory Controller Configuration Low
// bits[11], DisIoCohPref = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15,                      // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_2, 0x1B0),  // Address
      0x00000800,                           // regData
      0x00000800,                           // regMask
    }}
  },
// D18F3x44 - MCA NB Configuration
//
// bit[30] SyncFloodOnDramAdrParErr = 1
// bit[27] NbMcaToMstCpuEn = 1
// bit[21] SyncFloodOnAnyUcErr = 1
// bit[20] SyncFloodOnWDT = 1
// bit[8]  WDTDis = 1             ENH392804 workaround
// bit[4]  SyncPktPropDis = 0
// bit[3]  SyncPktGenDis = 0
// bit[2]  SyncFloodOnDramUcEcc = 1

  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x44),  // Address
      0x48300104,                           // regData
      0x4830011C,                           // regMask
    }}
  },
// D18F3x70 - SRI_to_XBAR Command Buffer Count
// bits[30:28] IsocRspCBC = 1
// bits[26:24] IsocPreqCBC = 0
// bits[22:20] IsocReqCBC = 1
// bits[18:16] UpRspCBC = 7
// bits[14:12] DnPreqCBC = 1
// bits[10:8]  UpPreqCBC = 1
// bits[7:6]   DnRspCBC = 1
// bits[5:4]   DnReqCBC = 1
// bits[2:0]   UpReqCBC = 7
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x70),  // Address
      0x10171157,                           // regData
      0x777777F7,                           // regMask
    }}
  },
// D18F3x74 - XBAR_to_SRI Command Buffer Count
// bits[31:28] DRReqCBC = 0
// bits[26:24] IsocPreqCBC = 1
// bits[23:20] IsocReqCBC = 1
// bits[19:16] ProbeCBC = 0xC
// bits[14:12] DnPreqCBC = 0
// bits[10:8]  UpPreqCBC = 1
// bits[6:4]   DnReqCBC = 0
// bits[2:0]   UpReqCBC = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x74),  // Address
      0x011C0101,                           // regData
      0xF7FF7777,                           // regMask
    }}
  },
// D18F3x78 - MCT to XBAR Buffer Count
// bits[21:16]   RspDBC              = 0x20
// bits[12:8]    ProbeCBC            = 0x11
// bits[5:0]     RspCBC              = 0x1F
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x78),  // Address
      0x0020111F,                           // RegData
      0x003F1F3F,                           // RegMask
    }}
  },
// D18F3x7C - Free List Buffer Count
// bits[22:20] Sri2XbarFreeRspDBC = 0
// bits[19:16] Sri2XbarFreeXreqDBC = 5
// bits[15:12] Sri2XbarFreeRspCBC = 0
// bits[11:8]  Sri2XbarFreeXreqCBC = 0xE
// bits[5:0]   Xbar2SriFreeListCBC = 0x2A
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x7C),  // Address
      0x00050E2A,                           // regData
      0x007FFF3F,                           // regMask
    }}
  },
// D18F3x84 - ACPI Power State Control High
// ACPI State S3
// bits[7:5] ClkDivisorSmafAct4 = 7
// bit [1] NbLowPwrEnSmafAct4   = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x84),  // Address
      0x000000E2,                           // regData
      0x000000E2,                           // regMask
    }}
  },
// D18F3xA0 - Power Control Miscellaneous
// bits[13:11] PllLockTime = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA0),  // Address
      0x00000800,                           // regData
      0x00003800,                           // regMask
    }}
  },
// D18F3xD4 - Clock Power Timing Control 0
// bit [31]    NbClkDivApplyAll = 1
// bits[30:28] NbClkDiv = 4
// bits[27:24] PowerStepUp = 0x8
// bits[23:20] PowerStepDown = 0x8
// bit [14]    CacheFlushImmOnAllHalt = 0
// bits[11:8]  ClkRampHystSel = 0xF
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xD4),  // Address
      0xC8800F00,                           // regData
      0xFFF04F00,                           // regMask
    }}
  },
// D18F3xD8 - Clock Power/Timing Control 1
// bits[6:4]   VSRampSlamTime = 0x4
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xD8),  // Address
      0x00000040,                           // RegData
      0x00000070,                           // RegMask
    }}
  },
// D18F3xDC - Clock Power/Timing Control 2
// bit [26]    IgnCpuPrbEn = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                      // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0xDC),  // Address
      0x04000000,                           // regData
      0x04000000,                           // regMask
    }}
  },
// D18F3x140 - SRI_to_XCS Token Count
// bits[24:20] FreeTok = 0xC
// bits[17:16] IsocRspTok = 1
// bits[15:14] IsocPreqTok = 0
// bits[13:12] IsocReqTok = 1
// bits[11:10] DnRspTok = 2
// bits[9:8]   UpRspTok = 1
// bits[7:6]   DnPreqTok = 1
// bits[5:4]   UpPreqTok = 1
// bits[3:2]   DnReqTok = 1
// bits[1:0]   UpReqTok = 3
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platform Features
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x140), // Address
      0x00C11957,                           // regData
      0x01F3FFFF,                           // regMask
    }}
  },
// D18F3x144 - MCT_to_XCS Token Count
// bits[7:4] ProbeTok = 4
// bits[3:0] RspTok = 0xB
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platform Features
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x144), // Address
      0x0000004B,                           // regData
      0x000000FF,                           // regMask
    }}
  },
// D18F3x148 - ONION Link_to_XCS Token Count
// bits[31:30] FreeTok[3:2]       = 0
// bits[28]    IsocRspTok1        = 0
// bits[26]    IsocPreqTok1       = 0
// bits[24]    IsocReqTok1        = 0
// bits[23:22] ProbeTok1          = 0
// bits[21:20] RspTok1            = 0
// bits[19:18] PReqTok1           = 0
// bits[17:16] ReqTok1            = 0
// bits[15:14] FreeTok[1:0]       = 1
// bits[13:12] IsocRspTok0        = 0
// bits[11:10] IsocPreqTok0       = 1
// bits[9:8]   IsocReqTok0        = 1
// bits[7:6]   ProbeTok0          = 0
// bits[5:4]   RspTok0            = 2
// bits[3:2]   PReqTok0           = 2
// bits[1:0]   ReqTok0            = 2
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x148), // Address
      0x0000452A,                           // regData
      0xD5FFFFFF,                           // regMask
    }}
  },
// D18F3x14C - ONIONPlus Link to XCS Token Count
// bits[31:30] FreeTok[3:2]       = 0
// bits[28]    IsocRspTok1        = 0
// bits[26]    IsocPreqTok1       = 0
// bits[24]    IsocReqTok1        = 0
// bits[23:22] ProbeTok1          = 0
// bits[21:20] RspTok1            = 0
// bits[19:18] PReqTok1           = 0
// bits[17:16] ReqTok1            = 0
// bits[15:14] FreeTok[1:0]       = 2
// bits[13:12] IsocRspTok0        = 0
// bits[11:10] IsocPreqTok0       = 0
// bits[9:8]   IsocReqTok0        = 0
// bits[7:6]   ProbeTok0          = 0
// bits[5:4]   RspTok0            = 0
// bits[3:2]   PReqTok0           = 0
// bits[1:0]   ReqTok0            = 3
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x14C), // Address
      0x00008003,                           // regData
      0xD5FFFFFF,                           // regMask
    }}
  },
// D18F3x160 - NB Machine Check Misc 0
// bits[23:20] LvtOffset = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x160), // Address
      0x00100000,                           // regData
      0x00F00000,                           // regMask
    }}
  },
// D18F3x168 - NB Machine Check Misc 1
// bits[23:20] LvtOffset = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x168), // Address
      0x00100000,                           // regData
      0x00F00000,                           // regMask
    }}
  },
// D18F3x17C - Extended Freelist Buffer Count
// bits[3:0] SPQPrbFreeCBC = 8
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platform Features
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x17C), // Address
      0x00000008,                           // regData
      0x0000000F                            // regMask
    }}
  },
// D18F3x180 - NB Extended Configuration
// bit [28] SyncFloodOnCC6DramUcErr = 1
// bit [24] McaLogErrAddrWdtErr = 1
// bit [21] SyncFloodOnCpuLeakErr = 1
// bit [17] SyncFloodOnDeferErrToIO = 1
// bit [9]  SyncFloodOnUCNbAry = 1
// bit [8]  SyncFloodOnProtErr = 1
// bit [7]  SyncFloodOnTgtAbortErr = 1
// bit [6]  SyncFloodOnDatErr = 1
// bit [5]  DisPciCfgCpuMstAbortRsp = 1
// bit [1]  SyncFloodOnUsPwDatErr = 1
// bit [0]  McaLogUsPwDatErrEn = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x180), // Address
      0x112203E3,                           // regData
      0x112203E3,                           // regMask
    }}
  },
// D18F3x188 - NB Configuration 2 Register
// bit[27] DisCpuWrSzDw64ReOrd     = 1
// bit [9] DisL3HiPriFreeListAlloc = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x188), // Address
      0x08000200,                           // regData
      0x08000200,                           // regMask
    }}
  },
// D18F3x1A0 - Core to NB Buffer Count
// bits[30:26] NbToCpuPrbLmt = 0xC
// bits[18:16] CpuToNbFreeBufCnt = 3
// bits[2:0]   CpuCmdBufCnt = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1A0), // Address
      0x30030001,                           // regData
      0x7C070007,                           // regMask
    }}
  },
// D18F3x1E4 - SBI Control
// bits[11:8] LvtOffset = 3
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1E4), // Address
      0x00000300,                           // regData
      0x00000F00,                           // regMask
    }}
  },
// D18F3x200 - Performance Mode Control Register
// bit [3] EnMcqPrbPickThrottle = 1
// bit [2] EnDctOddToNcLnkDatXfr = 1
// bit [1] EnDctEvnToNcLnkDatXfr = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_3, 0x200), // Address
      0x0000000E,                           // regData
      0x0000000E,                           // regMask
    }}
  },
// D18F4x110 - Sample and Residency Timer
// bits[11:0] CSampleTimer = 2
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x110), // Address
      0x00000002,                           // regData
      0x00000FFF,                           // regMask
    }}
  },
// D18F4x15C - Core Performance Boost Control
// bit [8] CstatePowerEn = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x15C), // Address
      0x00000100,                           // regData
      0x00000100,                           // regMask
    }}
  },
// D18F4x16C - APM TDP Control
// bit [4]       ApmTdpLimitIntEn    = 0x1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x16C), // Address
      0x00000010,                           // RegData
      0x00000010,                           // RegMask
    }}
  },
// D18F5x88 - Northbridge Configuration 4
// bit [24] DisHbNpReqBusLock = 1
// bit [20] DisSraCamXbarAddrMatch = 1
// bit [14] DisHldRegRdRspChk = 1
// bit [6]  = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x88),  // Address
      0x01104000,                           // regData
      0x01104040,                           // regMask
    }}
  },
// D18F5x8C
// bit [15] EnSrqAllocGt31       = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x8C),  // Address
      0x00008000,                           // RegData
      0x00008000,                           // RegMask
    }}
  },
// D18F5xE0 - Processor TDP Running Average
// bits[3:0] RunAvgRange = 0x2
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0xE0),  // Address
      0x00000002,                           // regData
      0x0000000F,                           // regMask
    }}
  },
// D18F5x128 - Clock Power/Timing Control 3
// bit [30]    NbFidChgCpuOpEn = 1
// bits[13:12] PwrGateTmr = 1
// bit [9]     FastSlamTimeDown = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x128), // Address
      0x40001200,                           // regData
      0x40003200,                           // regMask
    }}
  },
// D18F5x12C - Clock Power/Timing Control 4
// bit [5] CorePsi1En = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x12C), // Address
      0x00000020,                           // regData
      0x00000020,                           // regMask
    }}
  },
// D18F5x170 - Northbridge P-state Control
// bits[12:9]   NbPstateThreshold = 1, if 1 compute unit is enabled
  {
    CompUnitCountsPciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {
      PERFORMANCE_PROFILE_ALL,              // PerformanceFeatures
      (COMPUTE_UNIT_RANGE_0 (1, 1) | COUNT_RANGE_NONE), // 1 compute units
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x170), // Address
      0x00000200,                           // regData
      0x00001E00,                           // regMask
    }
  },
// D18F5x170 - Northbridge P-state Control
// bits[12:9]   NbPstateThreshold = 2, if 2 compute unit is enabled
  {
    CompUnitCountsPciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {
      PERFORMANCE_PROFILE_ALL,              // PerformanceFeatures
      (COMPUTE_UNIT_RANGE_0 (2, 2) | COUNT_RANGE_NONE), // 2 compute units
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x170), // Address
      0x00000400,                           // regData
      0x00001E00,                           // regMask
    }
  },
// D18F5x178 - Northbridge Fusion Configuration
// bit [18]      CstateFusionHsDis    = 1
// bit [17]      Dis2ndGnbAllowPsWait = 1
// bit [16]      ProcHotToGnbEn       = 1
// bit [11]      AllowSelfRefrS3Dis   = 1
// bit [10]      InbWakeS3Dis         = 1
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x178), // Address
      0x00070C00,                           // RegData
      0x00070C00,                           // RegMask
    }}
  },
// D18F5x18C - Clock Power/Timing Control 6
// bit [1]       CoreTfn              = 1
// bit [0]       NbTfn                = 0
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_5, 0x18C), // Address
      0x00000002,                           // RegData
      0x00000003,                           // RegMask
    }}
  },
// D18F0x90 - Upstream Base Channel Buffer Count
// bit [31] LockBc = 1
//
// NOTE: The entry is intended to be programmed after other bits of D18F0x[90, 94] is programmed and before D18F0x6C[30] is programmed.
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x90),  // Address
      0x80000000,                           // regData
      0x80000000,                           // regMask
    }}
  },
// D18F0xB0 - Upstream Base Channel Buffer Count - Link 1
// bit [31] LockBc = 1
//
// NOTE: The entry is intended to be programmed after other bits of D18F0x[B0, B4] is programmed and before D18F0x6C[30] is programmed.
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0xB0),  // Address
      0x80000000,                           // regData
      0x80000000,                           // regMask
    }}
  },
// D18F0x6C - Link Initialization Control
// bit [30] RlsLnkFullTokCntImm = 1
// bit [28] RlsIntFullTokCntImm = 1
//
// NOTE: The entry is intended to be after D18F0x[90, 94] and D18F3x[6C, 70, 74, 78, 7C, 140, 144, 148, 17C, 1A0] are programmed.
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C),  // Address
      0x50000000,                           // regData
      0x50000000,                           // regMask
    }}
  },
// D18F0x6C - Link Initialization Control
// bit [27] ApplyIsocModeEnNow = 1
//
// NOTE: The entry is intended to be after D18F0x6C[30, 28] are programmed.
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C),  // Address
      0x08000000,                           // regData
      0x08000000,                           // regMask
    }}
  },
// D18F4x16C - APM TDP Control
// bit [3] TdpLimitDis = 1   ENH392804 workaround
  {
    PciRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MAKE_SBDFO (0, 0, 24, FUNC_4, 0x16c), // Address
      0x00000008,                           // regData
      0x00000008,                           // regMask
    }}
  },
};

// PCI registers with Special Programming Requirements Table

STATIC CONST FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_INITIALIZER ROMDATA F15KvPciWorkarounds[] =
{
// D18F3xDC   - Clock Power/Timing Control 2
// bits[29:27]   NbsynPtrAdjLo
// bits[14:12]   NbsynPtrAdj
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_15_KV,
      AMD_F15_KV_ALL
    },
    {AMD_PF_ALL},
    {
      F15KvNbSynPtrAdj,
      0x00000000
    }
  },
};

CONST REGISTER_TABLE ROMDATA F15KvPciRegisterTableBeforeApLaunch = {
  PrimaryCores,
  (sizeof (F15KvPciRegistersBeforeApLaunch) / sizeof (TABLE_ENTRY_FIELDS)),
  F15KvPciRegistersBeforeApLaunch,
};

CONST REGISTER_TABLE ROMDATA F15KvPciRegisterTableAfterApLaunch = {
  PrimaryCores,
  (sizeof (F15KvPciRegistersAfterApLaunch) / sizeof (TABLE_ENTRY_FIELDS)),
  F15KvPciRegistersAfterApLaunch,
};

CONST REGISTER_TABLE ROMDATA F15KvPciWorkaroundTable = {
  PrimaryCores,
  (sizeof (F15KvPciWorkarounds) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvPciWorkarounds,
};


/*---------------------------------------------------------------------------------------*/
/**
 * Update the NB/code syschronization FIFO pointer adjust based on fuse values for
 * stretch enable and stretch amount.
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F15KvNbSynPtrAdj (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  PCI_ADDR                              PciAddress;
  F15_KV_CLK_PWR_TIMING_CTRL2_REGISTER  ClockPowerTimingControl2;
  CLK_PWR_TIMING_CTRL_6_REGISTER        ClockPowerTimingControl6;
  D0F0xBC_xC01070D0_STRUCT              D0F0xBC_xC01070D0;
  PSTATE_CPU_FAMILY_SERVICES            *FamilyServices;
  UINT32                                NumPStates;
  UINT8                                 BoostStates;
  UINT32                                i;
  UINT32                                CoreFrequency;
  UINT64                                LocalMsrRegister;
  CPU_LOGICAL_ID                        LogicalId;
  BOOLEAN                               ClockStretchEnabled;

  PciAddress.AddressValue = CPTC2_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &ClockPowerTimingControl2, StdHeader);
  ClockPowerTimingControl2.NbsynPtrAdj = 2;
  ClockPowerTimingControl2.NbsynPtrAdjLo = 2;
  ClockPowerTimingControl2.NbsynPtrAdjPstate_0 = 0;
  ClockPowerTimingControl2.NbsynPtrAdjPstate_2_1 = 0;

  GetLogicalIdOfCurrentCore (&LogicalId, StdHeader);
  if ((LogicalId.Revision & AMD_F15_KV_A0) != 0) {
    // On A0, get fuse value from SMU
    GnbRegisterReadKV (GnbGetHandle (StdHeader), TYPE_D0F0xBC, D0F0xBC_xC01070D0_ADDRESS,
                       &D0F0xBC_xC01070D0.Value, 0, StdHeader);
    ClockStretchEnabled = (BOOLEAN) D0F0xBC_xC01070D0.Field.StretchEn;
  } else {
    // On all other revs, get fuse value from shadow register
    PciAddress.AddressValue = CPTC6_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &ClockPowerTimingControl6, StdHeader);
    ClockStretchEnabled = (BOOLEAN) ClockPowerTimingControl6.ClkStretchEn;
  }
  if (ClockStretchEnabled) {
    GetFeatureServicesOfCurrentCore (&PstateFamilyServiceTable, (CONST VOID **)&FamilyServices, StdHeader);
    ASSERT (FamilyServices != NULL);
    FamilyServices->GetPstateMaxState (FamilyServices, &NumPStates, &BoostStates, StdHeader);
    for (i = NumPStates; i != -1; i--) {
      // Check if PState is enabled
      LibAmdMsrRead (PS_REG_BASE + i, &LocalMsrRegister, StdHeader);

      if (((F15_PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1) {
        FamilyServices->GetPstateFrequency (FamilyServices, (UINT8) i, &CoreFrequency, StdHeader);
        if (CoreFrequency >= TWO_GHZ_IN_MHZ) {
          ClockPowerTimingControl2.NbsynPtrAdjPstate_0 = (i & 1);
          ClockPowerTimingControl2.NbsynPtrAdjPstate_2_1 = ((i >> 1) & 3);
          break;
        }
      }
    }
  }
  PciAddress.AddressValue = CPTC2_PCI_ADDR;
  LibAmdPciWrite (AccessWidth32, PciAddress, &ClockPowerTimingControl2, StdHeader);
}
