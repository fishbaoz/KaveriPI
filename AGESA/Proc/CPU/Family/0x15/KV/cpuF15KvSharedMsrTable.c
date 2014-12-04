/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Kaveri Shared MSR table with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 286107 $   @e \$Date: 2014-02-27 13:45:40 -0600 (Thu, 27 Feb 2014) $
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
#include "Table.h"
#include "cpuF15KvPowerMgmt.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVSHAREDMSRTABLE_FILECODE


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
F15KvFpCfgInit (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
STATIC CONST MSR_TYPE_ENTRY_INITIALIZER ROMDATA F15KvSharedMsrRegisters[] =
{
//  M S R    T a b l e s
// ----------------------

// MSR_TOM2 (0xC001001D)
// bits[63:0] TOP_MEM2 = 0
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_TOM2,                           // MSR Address - Shared
      0x0000000000000000,                 // OR Mask
      0xFFFFFFFFFFFFFFFF,                 // NAND Mask
    }
  },

// MSR_SYS_CFG (0xC0010010)
// bit[21] MtrrTom2En = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_SYS_CFG,                        // MSR Address - Shared
      (1 << 21),                          // OR Mask
      (1 << 21),                          // NAND Mask
    }
  },

// MSR_IC_CFG (0xC0011021)
// bit[43] = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_IC_CFG,                         // MSR Address - Shared
      (1ull << 43),                       // OR Mask
      (1ull << 43),                       // NAND Mask
    }
  },

// MSR_CU_CFG (0xC0011023)
// bit[49] ProcFeedbackEn = 1
// bit[34] WbinvdFlushClean = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_CU_CFG,                         // MSR Address - Shared
      (0x0002000400000000),               // OR Mask
      (0x0002000400000000),               // NAND Mask
    }
  },

// MSR_CU_CFG2 (0xC001102A)
// bits[56:52] L2UpsizeCUCT = 0x19
// bit [50] RdMmExtCfgQwEn = 1
// bit [38] = 1
// bits[21:18] L2UpsizeCSWT[4:1] = 0
// bit [10] VicResyncChkEn = 1
// bit [9] L2UpsizeCSWT[0] = 0
// bits[4:0] L2UpsizeERT = 0x11

  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_CU_CFG2,                        // MSR Address - Shared
      0x0194004000000411,                 // OR Mask
      0x01F40040003C061F,                 // NAND Mask
    }
  },

// MSR_CU_CFG2 (0xC001102A)
// bit [8] SpecNbReqDis = 1

  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_A0                       // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_CU_CFG2,                        // MSR Address - Shared
      0x0000000000000100,                 // OR Mask
      0x0000000000000100,                 // NAND Mask
    }
  },

// MSR_CU_CFG3 (0xC001102B)
// bit[42] PwcDisableWalkerSharing = 0
// bit[22] PfcDoubleStride = 1
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_CU_CFG3,                        // MSR Address
      0x0000000000400000,                 // OR Mask
      0x0000040000400000,                 // NAND Mask
    }
  },
// MSR_CU_PFTCFG (0xC001102F)
// bits[36]      PrefetchThrottlingEn = 0x0
// bits[35:30]   ThrottleLevel        = 0x0
// bits[29:24]   AccThresh2           = 0x0
// bits[23:18]   AccThresh1           = 0x0
// bits[17:12]   AccThresh0           = 0x0
// bits[11:6]    TrackThreshold       = 0x0F
// bits[5:0]     CaptureThreshold     = 0x0F
  {
    MsrRegister,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      MSR_CU_PFTCFG,                      // MSR Address
      0x00000000000003CF,                 // OR Mask
      0x0000001FFFFFFFFF,                 // NAND Mask
    }
  },
};


// Compute Unit Count Dependent MSR Table

STATIC CONST MSR_CU_TYPE_ENTRY_INITIALIZER ROMDATA F15KvSharedMsrCuRegisters[] =
{
//  M S R    T a b l e s
// ----------------------

  // MSR_CU_CFG2 (0xC001102A)
  // bits[37:36] - ThrottleNbInterface[3:2] = 0
  // bits[7:6] - ThrottleNbInterface[1:0] = 0
  {
    CompUnitCountsMsr,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      (COMPUTE_UNIT_RANGE_0 (1, 1) | COUNT_RANGE_NONE), // 1 compute unit
      {
        MSR_CU_CFG2,                      // MSR Address - Shared
        0x0000000000000000,               // OR Mask
        0x00000030000000C0,               // NAND Mask
      }
    }
  },

  // MSR_CU_CFG2 (0xC001102A)
  // bits[37:36] - ThrottleNbInterface[3:2] = 0
  // bits[7:6] - ThrottleNbInterface[1:0] = 1
  {
    CompUnitCountsMsr,
    {
      AMD_FAMILY_15_KV,                   // CpuFamily
      AMD_F15_KV_ALL                      // CpuRevision
    },
    AMD_PF_ALL,                           // platformFeatures
    {
      (COMPUTE_UNIT_RANGE_0 (2, 2) | COUNT_RANGE_NONE), // 2 compute units
      {
        MSR_CU_CFG2,                      // MSR Address - Shared
        0x0000000000000040,               // OR Mask
        0x00000030000000C0,               // NAND Mask
      }
    }
  }

};


// Shared MSRs with Special Programming Requirements Table

STATIC CONST FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_INITIALIZER ROMDATA F15KvSharedMsrWorkarounds[] =
{
  // MSR_FP_CFG (0xC0011028)
  // bit [16]    - DiDtMode = F3x1FC[0]
  // bits[22:18] - DiDtCfg0 = F3x1FC[5:1]
  // bits[34:27] - DiDtCfg1 = F3x1FC[13:6]
  // bits[26:25] - DiDtCfg2 = F3x1FC[15:14]
  // bit [40]    - DiDtCfg3 = F3x1FC[16]
  // bits[44:41] - DiDtCfg4 = F3x1FC[20:17]
  {
    FamSpecificWorkaround,
    {
      AMD_FAMILY_15_KV,
      AMD_F15_KV_ALL
    },
    {AMD_PF_ALL},
    {
      F15KvFpCfgInit,
      0x00000000
    }
  },
};


CONST REGISTER_TABLE ROMDATA F15KvSharedMsrRegisterTable = {
  ComputeUnitPrimary,
  (sizeof (F15KvSharedMsrRegisters) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvSharedMsrRegisters,
};


CONST REGISTER_TABLE ROMDATA F15KvSharedMsrCuRegisterTable = {
  ComputeUnitPrimary,
  (sizeof (F15KvSharedMsrCuRegisters) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvSharedMsrCuRegisters,
};

CONST REGISTER_TABLE ROMDATA F15KvSharedMsrWorkaroundTable = {
  ComputeUnitPrimary,
  (sizeof (F15KvSharedMsrWorkarounds) / sizeof (TABLE_ENTRY_FIELDS)),
  (TABLE_ENTRY_FIELDS *) &F15KvSharedMsrWorkarounds,
};

/*---------------------------------------------------------------------------------------*/
/**
 * Update the FP_CFG MSR in current processor for Family15h KV.
 *
 * This function satisfies the programming requirements for the FP_CFG MSR.
 *
 * @param[in]   Data         The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
F15KvFpCfgInit (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32       ProductInfo;
  UINT64       FpCfg;
  PCI_ADDR     PciAddress;

  PciAddress.AddressValue = PRCT_INFO_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &ProductInfo, StdHeader);

  LibAmdMsrRead (MSR_FP_CFG, &FpCfg, StdHeader);
  ((FP_CFG_MSR *) &FpCfg)->DiDtMode = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtMode;
  ((FP_CFG_MSR *) &FpCfg)->DiDtCfg0 = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtCfg0;
  ((FP_CFG_MSR *) &FpCfg)->DiDtCfg1 = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtCfg1;
  ((FP_CFG_MSR *) &FpCfg)->DiDtCfg3 = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtCfg3;
  ((FP_CFG_MSR *) &FpCfg)->DiDtCfg2 = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtCfg2;
  ((FP_CFG_MSR *) &FpCfg)->DiDtCfg4 = ((PRODUCT_INFO_REGISTER *) &ProductInfo)->DiDtCfg4;
  LibAmdMsrWrite (MSR_FP_CFG, &FpCfg, StdHeader);
}


