/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Kaveri MSR tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuF15KvPowerMgmt.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVMSRTABLES_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
NbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
SetTopologyExtensions (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

STATIC CONST MSR_TYPE_ENTRY_INITIALIZER ROMDATA F15KvMsrRegistersBeforeApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------

// MSR_NB_CFG (0xC001001F)
// bit[54] InitApicIdCpuIdLo = 1
// bit[18] DisCstateBoostBlockPstateUp = 1
// bit[16]                   = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_NB_CFG,                           // MSR Address
      0x0040000000050000,                   // OR Mask
      0x0040000000050000,                   // NAND Mask
    }}
  },
};

STATIC CONST MSR_TYPE_ENTRY_INITIALIZER ROMDATA F15KvMsrRegistersAfterApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------


// MSR_MC0_MISC (0x00000403)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC0_MISC,                         // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_MC1_MISC (0x00000407)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC1_MISC,                         // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_MC2_MISC (0x0000040B)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC2_MISC,                         // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_MC4_MISC0 (0x00000413)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC4_MISC0,                        // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_MC5_MISC (0x00000417)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC5_MISC,                         // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_MC4_MISC1 (0xC0000408)
// bits[52:55] LvtOffset = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC4_MISC1,                        // MSR Address
      0x0010000000000000,                   // OR Mask
      0x00F0000000000000,                   // NAND Mask
    }}
  },
// MSR_CU_SPMCTL (0xC0010281)
// bit[61:54] = 0x0C
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_CU_SPMCTL,                        // MSR Address
      0x0300000000000000,                   // OR Mask
      0x3FC0000000000000,                   // NAND Mask
    }}
  },
// MSR_HWCR (0xC0010015)
// bit[27] EffFreqReadOnlyLock = 1
// bit[12] HltXSpCycEn = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_HWCR,                             // MSR Address
      0x0000000008001000,                   // OR Mask
      0x0000000008001000,                   // NAND Mask
    }}
  },
// MSR_OSVW_ID_Length (0xC0010140)
// bit[15:0] = 4
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_OSVW_ID_Length,                   // MSR Address
      0x0000000000000004,                   // OR Mask
      0x000000000000FFFF,                   // NAND Mask
    }}
  },
// MSR_MC0_CTL_MASK
// bit[8] IntErrType2 = 1   ENH392804 workaround
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_MC0_CTL_MASK,                     // MSR Address
      0x0000000000000100,                   // OR Mask
      0x0000000000000100,                   // NAND Mask
    }}
  },
// MSR_HDT_CFG
// bit[20] = 1   ENH392804 workaround
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      MSR_HDT_CFG,                          // MSR Address
      0x0000000000100000,                   // OR Mask
      0x0000000000100000,                   // NAND Mask
    }}
  },
// CPU_WDT_CFG
// bit[0] CpuWdtEn = 0    ENH392804 workaround
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                     // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      CPU_WDT_CFG,                          // MSR Address
      0x0000000000000000,                   // OR Mask
      0x0000000000000001,                   // NAND Mask
    }}
  },
};

CONST REGISTER_TABLE ROMDATA F15KvMsrRegisterTableBeforeApLaunch = {
  AllCores,
  (sizeof (F15KvMsrRegistersBeforeApLaunch) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvMsrRegistersBeforeApLaunch,
};

CONST REGISTER_TABLE ROMDATA F15KvMsrRegisterTableAfterApLaunch = {
  AllCores,
  (sizeof (F15KvMsrRegistersAfterApLaunch) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvMsrRegistersAfterApLaunch,
};

// MSR with Special Programming Requirements Table

STATIC CONST FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_INITIALIZER ROMDATA F15KvMsrWorkarounds[] =
{
// MSR_0000_0413
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_15,                        // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // PlatformFeatures
    {{
      NbMcaLock,                            // Function call
      0x00000000,                           // Data
    }}
  },
// MSR_C001_1005
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_15,                        // CpuFamily
      AMD_F15_KV_ALL                        // CpuRevision
    },
    {AMD_PF_ALL},                           // platformFeatures
    {{
      SetTopologyExtensions,                // function call
      0x00000000,                           // data
    }}
  },
};

CONST REGISTER_TABLE ROMDATA F15KvMsrWorkaroundTable = {
  AllCores,
  (sizeof (F15KvMsrWorkarounds) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) F15KvMsrWorkarounds,
};

/*---------------------------------------------------------------------------------------*/
/**
 *  MSR special programming requirements for MSR_C001_1005
 *
 *  AGESA should program MSR_C001_1005[54, TopologyExtensions] as follows:
 *  IF (CPUID Fn8000_0001_EBX[PkgType]==001b) THEN 1 ELSE 0 ENDIF.
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
SetTopologyExtensions (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  CPUID_DATA CpuId;
  UINT64 CpuMsrData;

  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, StdHeader);
  LibAmdMsrRead (MSR_CPUID_EXT_FEATS, &CpuMsrData, StdHeader);
  CpuMsrData &= ~(BIT54);
  if (((CpuId.EBX_Reg >> 28) & 0xF) == KV_SOCKET_FM2r2) {
    CpuMsrData |= BIT54;
  }
  LibAmdMsrWrite (MSR_CPUID_EXT_FEATS, &CpuMsrData, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  MSR special programming requirements for MSR_0000_0413
 *
 *  AGESA should program MSR_0000_0413[61] dependant on the value read from MSR_0000_0413[50:49]
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
NbMcaLock (
  IN       UINT32            Data,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  MC4_MISC0_MSR Mc4Misc0;

  LibAmdMsrRead (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);

  if (Mc4Misc0.IntType == 0x2) {
    Mc4Misc0.Locked = 1;
  } else {
    Mc4Misc0.Locked = 0;
  }
  LibAmdMsrWrite (MSR_MC4_MISC0, (UINT64 *) &Mc4Misc0, StdHeader);

  return;
}


