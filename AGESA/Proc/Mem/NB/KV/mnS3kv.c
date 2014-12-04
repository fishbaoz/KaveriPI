/* $NoKeywords:$ */
/**
 * @file
 *
 * mns3kv.c
 *
 * KV memory specific function to support S3 resume
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
/*****************************************************************************
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
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "mport.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "S3.h"
#include "mfs3.h"
#include "mnkv.h"
#include "mnreg.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "mnS3kv.h"
#include "heapManager.h"
#include "merrhdl.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_KV_MNS3KV_FILECODE
#define DCT0_MEMPSTATE_MASK 0x10
#define DCT1_MEMPSTATE_MASK 0x20
#define DO_NOT_CARE 0
/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
/// Define S3 fields to reduce the number of encoding bits
typedef enum {
  S3RegCadTxImpedance,             ///< Register CAD Tx Impedance
  S3RegDataTxImpedance,            ///< Register Data Tx Impedance
  S3RegDataRxImpedance,            ///< Register Data Rx Impedance
  S3RegDataTxSlewRate,             ///< Register Data Tx Slew rate
  S3RegCadTxSlewRate,              ///< Register CAD Tx Slew rate
  S3RegDataRdPtrInitVal,           ///< Register Data Read Pointer Init
  S3RegRdPtrInitVal,               ///< Register Read Pointer Init
  S3RegTxDly,                      ///< Register TX delay
  S3RegDataRxDly,                  ///< Register Data Rx Delay
  S3RegDataTxDly,                  ///< Register Data Tx Delay
  S3RegDataTxEqHiImp,              ///< Register Data Tx EQ Hi Impedence
  S3RegDataTxEqLoImp,              ///< Register Data Tx EQ Lo Impedence
  S3RegDataTxEqBoostImp,           ///< Register Data Tx EQ Boost Impedence

  MaxS3Field
} S3_FIELD;

STATIC CONST BIT_FIELD_NAME ROMDATA S3FieldTransTab[] = {
  RegCadTxImpedance,
  RegDataTxImpedance,
  RegDataRxImpedance,
  RegDataTxSlewRate,
  RegCadTxSlewRate,
  RegDataRdPtrInitVal,
  RegRdPtrInitVal,
  RegTxDly,
  RegDataRxDly,
  RegDataTxDly,
  RegDataTxEqHiImp,
  RegDataTxEqLoImp,
  RegDataTxEqBoostImp,
};

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
STATIC
MemNS3ResumeKV (
  IN OUT   S3_MEM_NB_BLOCK *S3NBPtr,
  IN       UINT8 NodeID
  );

UINT16
STATIC
MemNS3GetRegLstPtrKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  );

AGESA_STATUS
STATIC
MemNS3GetDeviceRegLstKV (
  IN       UINT32 RegisterLstID,
  OUT   VOID **RegisterHeader,
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3SetDfltPhyRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetPhyFreqKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetConPCIMaskKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  );

VOID
STATIC
MemNS3ChangeMemPStateContextAndFlowNb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetCSRKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetCSRKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetBitFieldKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetBitFieldKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3ChangeMemPStateContextKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetNBPStateDepRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SetNBPStateDepRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3SaveNBRegisterKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3RestoreNBRegisterKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

BOOLEAN
MemNS3DctCfgSelectKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  );

VOID
STATIC
MemNS3DisableChannelKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );


VOID
STATIC
MemNS3SetInstanceKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3GetInstanceKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
STATIC
MemNS3EnablePhyCalibrationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3PendOnPhyCalibrateCompletionKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3RateOfPhyCalibrateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3PhyPowerSavingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNS3SwitchMemPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  );
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define S3_NB_PS(Dct)     ((1 << 1) | (Dct))
#define S3_CAD(Dct)       ((2 << 1) | (Dct))
#define S3_DATA(Dct)      ((3 << 1) | (Dct))

#define S3_PER_NB_PS(p, bf) ((((p) & 0xFF) << 4) | bf)
#define S3_PER_CAD_TYPE(t, bf) ((((t) & 0xFF) << 4) | bf)
#define S3_PER_DQ(d, b, bf) ((0 << 11) | (((d) & 0x3) << 9) | (((b) & 0x1F) << 4) | bf)
#define S3_PER_DQS(d, b, bf) ((1 << 11) | (((d) & 0x3) << 9) | (((b) & 0x1F) << 4) | bf)

PCI_SPECIAL_CASE PciSpecialCaseFuncKV[] = {
  {MemNS3GetCSRKV, MemNS3SetCSRKV},
  {MemNS3GetBitFieldKV, MemNS3SetBitFieldKV},
  {MemNS3GetNBPStateDepRegKV, MemNS3SetNBPStateDepRegKV},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3SetDfltPhyRegKV},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3SetPhyFreqKV},
  {MemNS3ChangeMemPStateContextKV, MemNS3ChangeMemPStateContextKV},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ForceNBP0Unb},
  {MemNS3SaveNBRegisterKV, MemNS3RestoreNBRegisterKV},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ReleaseNBPSUnb},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3DisableChannelKV},
  { (VOID (*) (ACCESS_WIDTH, PCI_ADDR, VOID *, VOID *)) memDefRet, MemNS3ConfigureDisDllShutdownSrKV},
  {MemNS3GetInstanceKV, MemNS3SetInstanceKV}
};

PCI_REG_DESCRIPTOR ROMDATA S3PciPreSelfRefDescriptorKV[] = {
  {{10, 3, 1}, DO_NOT_CARE, 0, 0},             // Clear DisDllShutdownSR prior any pstate changes
  {{6, 3, 1},  DO_NOT_CARE, 0, 0},             // Force NBP0
  {{0, 0, 0},  FUNC_2, 0x110, 0x000007EC},
  {{0, 0, 0},  FUNC_1, 0x40,  0xFFFF0003},
  {{0, 1, 0},  FUNC_1, 0x140, 0x000000FF},
  {{0, 0, 0},  FUNC_1, 0x44,  0xFFFF0007},
  {{0, 1, 0},  FUNC_1, 0x144, 0x000000FF},
  {{0, 0, 0},  FUNC_1, 0xF0,  0xFF00FF02},
  {{0, 0, 0},  FUNC_1, 0x120, 0x001FFFFF},
  {{0, 0, 0},  FUNC_1, 0x124, 0x001FFFFF},
  {{0, 0, 0},  FUNC_1, 0x200, 0xFFFFF87F},
  {{0, 0, 0},  FUNC_1, 0x204, 0xFFFFF80F},
  {{0, 0, 0},  FUNC_1, 0x208, 0xFFFFF87F},
  {{0, 0, 0},  FUNC_1, 0x20C, 0xFFFFF80F},
  {{0, 0, 0},  FUNC_1, 0x210, 0xFFFFF87F},
  {{0, 0, 0},  FUNC_1, 0x214, 0xFFFFF80F},
  {{0, 0, 0},  FUNC_1, 0x218, 0xFFFFF87F},
  {{0, 0, 0},  FUNC_1, 0x21C, 0xFFFFF80F},
  {{0, 0, 0},  FUNC_1, 0x240, 0x007FF800},
  {{0, 0, 0},  FUNC_1, 0x244, 0x007FF800},
  {{0, 0, 0},  FUNC_1, 0x248, 0x007FF800},
  {{0, 0, 0},  FUNC_1, 0x24C, 0x007FF800},

  {{0, 0, 0},  FUNC_2, 0x114, 0x00000200},
  {{0, 0, 0},  FUNC_2, 0x118, 0xFF77FFFF},
  {{0, 0, 0},  FUNC_2, 0x11C, 0xFFFFFFFF},
  {{0, 0, 0},  FUNC_2, 0x1B0, 0xFFD01F3F},
  {{0, 0, 0},  FUNC_2, 0x1B4, 0xFE7FFFFF},
  {{0, 0, 0},  FUNC_2, 0xA4,  0x00F07900},
  {{0, 0, 0},  FUNC_3, 0x2B4, 0x00000F0F},
  {{0, 0, 0},  FUNC_3, 0x88,  0x08000000}
};

CONST PCI_REGISTER_BLOCK_HEADER ROMDATA S3PciPreSelfRefKV = {
  0,
  (sizeof (S3PciPreSelfRefDescriptorKV) / sizeof (PCI_REG_DESCRIPTOR)),
  S3PciPreSelfRefDescriptorKV,
  PciSpecialCaseFuncKV
};

CONDITIONAL_PCI_REG_DESCRIPTOR ROMDATA S3CPciPreSelfDescriptorKV[] = {
   // DCT 0
  {{7, 0, 1}, DCT0,   0x40,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x44,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x48,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x4C,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x50,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x54,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x58,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x5C,  0x7FF8FFED, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x60,  0x7FF8FFE0, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x64,  0x7FF8FFE0, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x68,  0x7FF8FFE0, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x6C,  0x7FF8FFE0, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x78,  0x0002075E, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x80,  0x000000FF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x84,  0x00800003, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x88,  0x3F000000, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x8C,  0x00070000, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x90,  0xFBFF0000, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x94,  0x9FF1CC9F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0xA8,  0xB07BFF00, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x1BC, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x200, 0x3F1F1F1F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x204, 0x0F3F0F3F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x208, 0x07070707, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x20C, 0x00030F1F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT0,   SET_S3_NB_PSTATE_OFFSET (0x210, 0), 0xFFC70000, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT0,   SET_S3_NB_PSTATE_OFFSET (0x210, 1), 0xFFC70000, DCT0_NBPSTATE_SUPPORT_MASK, DCT0_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT0,   SET_S3_NB_PSTATE_OFFSET (0x210, 2), 0xFFC70000, DCT0_NBPSTATE_SUPPORT_MASK, DCT0_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT0,   SET_S3_NB_PSTATE_OFFSET (0x210, 3), 0xFFC70000, DCT0_NBPSTATE_SUPPORT_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x214, 0x000F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x218, 0xCF0F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x21C, 0x001F1F00, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x220, 0x00001F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x224, 0x0000070F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x228, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x22C, 0x0000001F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x230, 0x0F0F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x234, 0x0F0F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x238, 0x0F0F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x23C, 0x0F0F0F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x240, 0x000077FF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x244, 0x0000000F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x248, 0x3F3F1F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x24C, 0x3F3F3F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x2E0, 0x5F700000, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x2E8, 0xFFFFFFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x2EC, 0x0000FFFF, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x2F0, 0x00000001, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x400, 0x00000F0F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x404, 0x8F1F0F1F, DCT0_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x408, 0x1F000007, DCT0_MASK, DCT0_ANY_DIMM_MASK},

  // DCT 1
  {{7, 0, 1}, DCT1,   0x40,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x44,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x48,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x4C,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x50,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x54,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x58,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x5C,  0x7FF8FFED, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x60,  0x7FF8FFE0, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x64,  0x7FF8FFE0, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x68,  0x7FF8FFE0, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x6C,  0x7FF8FFE0, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x78,  0x0002075E, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x80,  0x000000FF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x84,  0x00800003, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x88,  0x3F000000, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x8C,  0x00070000, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x90,  0xFBFF0000, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x94,  0x9FF1CC9F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0xA8,  0xB07BFF00, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x1BC, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x200, 0x3F1F1F1F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x204, 0x0F3F0F3F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x208, 0x07070707, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x20C, 0x00030F1F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT1,   SET_S3_NB_PSTATE_OFFSET (0x210, 0), 0xFFC70000, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT1,   SET_S3_NB_PSTATE_OFFSET (0x210, 1), 0xFFC70000, DCT1_NBPSTATE_SUPPORT_MASK, DCT1_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT1,   SET_S3_NB_PSTATE_OFFSET (0x210, 2), 0xFFC70000, DCT1_NBPSTATE_SUPPORT_MASK, DCT1_ANY_DIMM_MASK},
  {{2, 0, 1}, DCT1,   SET_S3_NB_PSTATE_OFFSET (0x210, 3), 0xFFC70000, DCT1_NBPSTATE_SUPPORT_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x214, 0x000F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x218, 0xCF0F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x21C, 0x001F1F00, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x220, 0x00001F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x224, 0x0000070F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x228, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x22C, 0x0000001F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x230, 0x0F0F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x234, 0x0F0F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x238, 0x0F0F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x23C, 0x0F0F0F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x240, 0x000077FF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x244, 0x0000000F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x248, 0x3F3F1F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x24C, 0x3F3F3F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x2E0, 0x5F700000, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x2E8, 0xFFFFFFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x2EC, 0x0000FFFF, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x2F0, 0x00000001, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x400, 0x00000F0F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x404, 0x8F1F0F1F, DCT1_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x408, 0x1F000007, DCT1_MASK, DCT1_ANY_DIMM_MASK},

  // DRAM channel disable
  {{9, 3, 1}, DCT0,   0, 0, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK},
  {{9, 3, 1}, DCT1,   0, 0, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK},

  // Phy Initialization
  // 1. Set DDR3 mode
  {{3, 3, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK},

  // 2. Phy general config
  {{1, 2, 1}, DCT0,   RegDctPhy0x00F0F04E,       0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegDctPhy0x00F1F04E,       0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegLaneToCrcMap0,  0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegLaneToCrcMap1,  0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl0,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl1,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl2,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl3,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl4,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl5,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl6,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegAcsmOdtCtrl7,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegVrefByteMaster, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegVrefByteAbyte,  0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegVrefByteDbyte,  0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegProcOdtTmg,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegClkTreeTermCntrMstr,    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegDataDqDqsRcvCntrl3,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegMasterD3EvntMerr,       0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDctPhy0x00F0F04E,       0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDctPhy0x00F1F04E,       0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegLaneToCrcMap0,  0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegLaneToCrcMap1,  0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl0,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl1,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl2,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl3,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl4,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl5,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl6,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegAcsmOdtCtrl7,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegVrefByteMaster, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegVrefByteAbyte,  0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegVrefByteDbyte,  0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegProcOdtTmg,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegClkTreeTermCntrMstr,    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDataDqDqsRcvCntrl3,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegMasterD3EvntMerr,       0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},

  // 3. Phy voltage
  {{1, 1, 1}, DCT0,   BFCalCmptrResTrim, 0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, DCT1,   BFCalCmptrResTrim, 0x000000FF, DCT1_MASK, ANY_DIMM_MASK},

  // 4. Frequency Change
  {{4, 3, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK},

  // 5. CAD/Data bus config
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance00,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance01,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance02,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance03,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance04,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance05,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance06,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance07,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance08,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance09,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance0a,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance0b,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance10,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance11,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance12,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance13,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance14,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance15,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance16,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance17,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance18,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance19,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance1a,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance1b,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance20,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance21,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance22,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance23,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance24,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance25,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance26,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance27,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance28,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance29,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance2a,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance2b,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance30,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance31,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance32,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance33,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance34,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance35,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance36,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance37,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance38,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance39,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance3a,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance3b,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset0,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset1,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset2,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset3,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset4,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset5,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset6,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset7,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset8,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group0,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group1,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group2,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group3,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group0,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group1,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group2,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group3,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group0,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group1,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group2,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group3,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group0,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group1,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group2,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group3,         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group0,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group1,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group2,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group0,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group1,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group2,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group0,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group1,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group2,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group0,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group1,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group2,     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group0, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group1, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group2, 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly000,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly001,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly010,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly011,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly020,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly021,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly030,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly031,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly100,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly101,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly110,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly111,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly120,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly121,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly130,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly131,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly200,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly201,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly210,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly211,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly220,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly221,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly230,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly231,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly300,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly301,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly310,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly311,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly320,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly321,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly330,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly331,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly400,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly401,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly410,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly411,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly420,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly421,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly430,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly431,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly500,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly501,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly510,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly511,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly520,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly521,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly530,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly531,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly600,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly601,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly610,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly611,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly620,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly621,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly630,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly631,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly700,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly701,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly710,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly711,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly720,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly721,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly730,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly731,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly800,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly801,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly810,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly811,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly820,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly821,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly830,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly831,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly000,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly001,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly010,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly011,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly020,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly021,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly030,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly031,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly100,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly101,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly110,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly111,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly120,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly121,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly130,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly131,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly200,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly201,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly210,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly211,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly220,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly221,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly230,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly231,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly300,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly301,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly310,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly311,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly320,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly321,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly330,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly331,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly400,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly401,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly410,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly411,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly420,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly421,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly430,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly431,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly500,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly501,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly510,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly511,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly520,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly521,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly530,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly531,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly600,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly601,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly610,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly611,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly620,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly621,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly630,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly631,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly700,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly701,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly710,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly711,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly720,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly721,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly730,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly731,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly800,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly801,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly810,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly811,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly820,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly821,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly830,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly831,            0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance00,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance01,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance02,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance03,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance04,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance05,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance06,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance07,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance08,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance09,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance0a,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance0b,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance10,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance11,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance12,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance13,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance14,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance15,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance16,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance17,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance18,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance19,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance1a,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance1b,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance20,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance21,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance22,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance23,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance24,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance25,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance26,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance27,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance28,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance29,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance2a,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance2b,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance30,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance31,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance32,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance33,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance34,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance35,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance36,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance37,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance38,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance39,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance3a,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance3b,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset0,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset1,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset2,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset3,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset4,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset5,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset6,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset7,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset8,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group0,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group1,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group2,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group3,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group0,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group1,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group2,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group3,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group0,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group1,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group2,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group3,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group0,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group1,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group2,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group3,         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad0Group0,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad0Group1,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad0Group2,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group0,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group1,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group2,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group0,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group1,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group2,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group0,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group1,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group2,     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group0, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group1, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group2, 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly000,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly001,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly010,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly011,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly020,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly021,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly030,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly031,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly100,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly101,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly110,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly111,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly120,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly121,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly130,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly131,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly200,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly201,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly210,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly211,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly220,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly221,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly230,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly231,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly300,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly301,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly310,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly311,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly320,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly321,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly330,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly331,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly400,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly401,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly410,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly411,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly420,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly421,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly430,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly431,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly500,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly501,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly510,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly511,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly520,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly521,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly530,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly531,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly600,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly601,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly610,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly611,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly620,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly621,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly630,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly631,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly700,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly701,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly710,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly711,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly720,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly721,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly730,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly731,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly800,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly801,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly810,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly811,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly820,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly821,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly830,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly831,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly000,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly001,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly010,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly011,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly020,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly021,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly030,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly031,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly100,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly101,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly110,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly111,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly120,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly121,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly130,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly131,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly200,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly201,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly210,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly211,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly220,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly221,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly230,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly231,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly300,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly301,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly310,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly311,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly320,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly321,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly330,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly331,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly400,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly401,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly410,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly411,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly420,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly421,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly430,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly431,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly500,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly501,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly510,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly511,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly520,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly521,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly530,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly531,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly600,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly601,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly610,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly611,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly620,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly621,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly630,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly631,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly700,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly701,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly710,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly711,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly720,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly721,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly730,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly731,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly800,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly801,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly810,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly811,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly820,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly821,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly830,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly831,            0x000000FF, DCT1_MASK, ANY_DIMM_MASK},

  // 6. Phy FIFO Config
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (0, S3RegRdPtrInitVal),             0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (0, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (1, S3RegRdPtrInitVal),             0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (1, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (2, S3RegRdPtrInitVal),             0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (2, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (3, S3RegRdPtrInitVal),             0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (3, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (NB_PS_PMU, S3RegRdPtrInitVal),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT0), S3_PER_NB_PS (NB_PS_PMU, S3RegDataRdPtrInitVal), 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (0, S3RegRdPtrInitVal),             0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (0, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (1, S3RegRdPtrInitVal),             0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (1, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (2, S3RegRdPtrInitVal),             0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (2, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (3, S3RegRdPtrInitVal),             0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (3, S3RegDataRdPtrInitVal),         0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (NB_PS_PMU, S3RegRdPtrInitVal),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_NB_PS (DCT1), S3_PER_NB_PS (NB_PS_PMU, S3RegDataRdPtrInitVal), 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},

  // 7. Predriver
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CS, S3RegCadTxSlewRate),       0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 0, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 1, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 2, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 3, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 4, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 5, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 6, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 7, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 8, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 0, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 1, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 2, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 3, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 4, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 5, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 6, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 7, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 8, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CS, S3RegCadTxSlewRate),       0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 0, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 1, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 2, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 3, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 4, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 5, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 6, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 7, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 8, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 0, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 1, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 2, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 3, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 4, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 5, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 6, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 7, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 8, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},

  // DRAM PHY Power Savings
  {{1, 2, 1}, DCT0,   RegCadDllLockMaintenance,  0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegDataDllLockMaintenance,     0x000000FF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegCadDllLockMaintenance,  0x000000FF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDataDllLockMaintenance,     0x000000FF, DCT1_MASK, ANY_DIMM_MASK},

  // Program MemPstate 1 registers
  // Switch to MemPstate context 1
  {{5, 3, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  // DRAM PHY Power Savings
  {{1, 2, 1}, DCT0,   RegCadDllLockMaintenance,  0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT0,   RegDataDllLockMaintenance,     0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegCadDllLockMaintenance,  0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDataDllLockMaintenance,     0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  {{7, 0, 1}, DCT0,   0x200, 0x3F1F1F1F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x204, 0x0F3F0F3F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x20C, 0x00030F1F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x214, 0x000F0F0F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x218, 0xCF0F0F0F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x21C, 0x001F1F00, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x22C, 0x0000001F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x240, 0x000077FF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x248, 0x3F3F1F0F, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT0,   0x2E8, 0xFFFFFFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT0,   0x2EC, 0x0000FFFF, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT0,   0x2F0, 0x00000001, DCT0_MEMPSTATE_MASK, DCT0_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x200, 0x3F1F1F1F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x204, 0x0F3F0F3F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x20C, 0x00030F1F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x214, 0x000F0F0F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x218, 0xCF0F0F0F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x21C, 0x001F1F00, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x22C, 0x0000001F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x240, 0x000077FF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x248, 0x3F3F1F0F, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 0, 1}, DCT1,   0x2E8, 0xFFFFFFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 2, 1}, DCT1,   0x2EC, 0x0000FFFF, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},
  {{7, 1, 1}, DCT1,   0x2F0, 0x00000001, DCT1_MEMPSTATE_MASK, DCT1_ANY_DIMM_MASK},

  // Genernal Phy Config for M1 only
  {{1, 2, 1}, DCT0,   RegDctPhy0x01F1F045,        0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, DCT1,   RegDctPhy0x01F1F045,        0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataTxImpedance),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, ALL_BYTES, S3RegDataRxImpedance),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  {{11, 2, 1}, DCT0,   RegCadTxImpedance00,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance01,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance02,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance03,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance04,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance05,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance06,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance07,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance08,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance09,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance0a,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance0b,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance10,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance11,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance12,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance13,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance14,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance15,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance16,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance17,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance18,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance19,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance1a,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance1b,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance20,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance21,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance22,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance23,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance24,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance25,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance26,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance27,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance28,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance29,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance2a,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance2b,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance30,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance31,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance32,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance33,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance34,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance35,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance36,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance37,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance38,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance39,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance3a,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegCadTxImpedance3b,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset0,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset1,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset2,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset3,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset4,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset5,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset6,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset7,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataRdPtrOffset8,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad0Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad1Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad2Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group0,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group1,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group2,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegTxDlyCad3Group3,         0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad0Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad1Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad2Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group0,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group1,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDllTuningCad3Group2,     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad0Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad1Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad2Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad3Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad4Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad5Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad6Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad7Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group0, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group1, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDllTuningCad8Group2, 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT0,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly000,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly001,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly010,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly011,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly020,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly021,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly030,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly031,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly100,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly101,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly110,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly111,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly120,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly121,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly130,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly131,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly200,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly201,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly210,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly211,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly220,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly221,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly230,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly231,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly300,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly301,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly310,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly311,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly320,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly321,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly330,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly331,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly400,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly401,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly410,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly411,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly420,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly421,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly430,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly431,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly500,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly501,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly510,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly511,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly520,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly521,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly530,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly531,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly600,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly601,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly610,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly611,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly620,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly621,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly630,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly631,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly700,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly701,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly710,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly711,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly720,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly721,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly730,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly731,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly800,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly801,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly810,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly811,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly820,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly821,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly830,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataRxDly831,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly000,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly001,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly010,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly011,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly020,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly021,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly030,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly031,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly100,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly101,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly110,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly111,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly120,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly121,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly130,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly131,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly200,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly201,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly210,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly211,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly220,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly221,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly230,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly231,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly300,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly301,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly310,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly311,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly320,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly321,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly330,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly331,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly400,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly401,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly410,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly411,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly420,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly421,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly430,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly431,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly500,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly501,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly510,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly511,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly520,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly521,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly530,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly531,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly600,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly601,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly610,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly611,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly620,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly621,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly630,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly631,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly700,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly701,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly710,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly711,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly720,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly721,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly730,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly731,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly800,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly801,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly810,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly811,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly820,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly821,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly830,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT0,   RegDataTxDly831,            0x000000FF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance00,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance01,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance02,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance03,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance04,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance05,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance06,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance07,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance08,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance09,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance0a,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance0b,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance10,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance11,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance12,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance13,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance14,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance15,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance16,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance17,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance18,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance19,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance1a,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance1b,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance20,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance21,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance22,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance23,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance24,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance25,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance26,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance27,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance28,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance29,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance2a,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance2b,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance30,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance31,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance32,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance33,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance34,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance35,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance36,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance37,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance38,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance39,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance3a,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegCadTxImpedance3b,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad0Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad1Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad2Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group0,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group1,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group2,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegRdPtrOffsetCad3Group3,   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset0,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset1,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset2,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset3,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset4,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset5,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset6,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset7,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataRdPtrOffset8,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad0Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad1Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad2Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group0,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group1,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group2,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegTxDlyCad3Group3,         0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad0Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad0Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad1Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad2Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group0,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group1,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDllTuningCad3Group2,     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad0Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad1Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad2Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad3Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad4Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad5Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad6Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad7Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group0, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group1, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDllTuningCad8Group2, 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl00,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl01,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl02,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl03,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl04,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl05,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl06,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl07,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl08,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl09,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl0a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl0b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl10,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl11,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl12,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl13,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl14,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl15,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl16,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl17,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl18,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl19,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl1a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl1b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl20,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl21,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl22,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl23,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl24,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl25,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl26,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl27,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl28,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl29,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl2a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl2b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl30,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl31,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl32,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl33,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl34,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl35,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl36,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl37,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl38,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl39,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl3a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl3b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl40,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl41,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl42,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl43,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl44,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl45,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl46,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl47,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl48,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl49,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl4a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl4b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl50,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl51,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl52,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl53,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl54,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl55,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl56,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl57,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl58,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl59,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl5a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl5b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl60,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl61,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl62,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl63,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl64,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl65,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl66,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl67,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl68,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl69,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl6a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl6b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl70,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl71,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl72,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl73,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl74,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl75,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl76,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl77,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl78,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl79,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl7a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl7b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl80,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl81,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl82,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl83,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl84,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl85,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl86,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl87,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl88,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl89,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl8a,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 2, 1}, DCT1,   RegDataDqDqsRxCtl8b,        0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly000,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly001,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly010,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly011,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly020,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly021,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly030,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly031,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly100,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly101,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly110,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly111,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly120,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly121,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly130,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly131,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly200,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly201,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly210,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly211,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly220,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly221,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly230,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly231,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly300,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly301,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly310,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly311,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly320,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly321,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly330,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly331,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly400,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly401,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly410,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly411,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly420,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly421,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly430,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly431,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly500,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly501,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly510,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly511,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly520,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly521,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly530,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly531,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly600,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly601,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly610,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly611,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly620,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly621,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly630,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly631,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly700,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly701,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly710,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly711,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly720,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly721,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly730,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly731,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly800,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly801,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly810,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly811,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly820,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly821,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly830,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataRxDly831,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly000,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly001,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly010,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly011,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly020,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly021,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly030,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly031,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly100,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly101,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly110,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly111,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly120,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly121,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly130,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly131,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly200,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly201,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly210,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly211,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly220,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly221,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly230,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly231,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly300,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly301,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly310,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly311,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly320,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly321,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly330,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly331,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly400,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly401,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly410,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly411,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly420,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly421,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly430,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly431,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly500,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly501,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly510,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly511,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly520,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly521,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly530,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly531,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly600,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly601,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly610,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly611,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly620,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly621,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly630,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly631,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly700,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly701,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly710,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly711,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly720,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly721,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly730,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly731,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly800,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly801,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly810,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly811,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly820,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly821,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly830,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{11, 1, 1}, DCT1,   RegDataTxDly831,            0x000000FF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CS, S3RegCadTxSlewRate),       0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT0), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 0, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 1, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 2, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 3, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 4, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 5, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 6, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 7, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQ (ALL_DIMMS, 8, S3RegDataTxSlewRate),     0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 0, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 1, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 2, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 3, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 4, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 5, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 6, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 7, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT0), S3_PER_DQS (ALL_DIMMS, 8, S3RegDataTxSlewRate),    0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CKE, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CS, S3RegCadTxSlewRate),       0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ODT, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_ADDR_CMD, S3RegCadTxSlewRate), 0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_CAD (DCT1), S3_PER_CAD_TYPE (PAD_CLK, S3RegCadTxSlewRate),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 0, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 1, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 2, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 3, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 4, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 5, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 6, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 7, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQ (ALL_DIMMS, 8, S3RegDataTxSlewRate),     0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 0, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 1, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 2, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 3, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 4, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 5, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 6, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 7, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 2, 1}, S3_DATA (DCT1), S3_PER_DQS (ALL_DIMMS, 8, S3RegDataTxSlewRate),    0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  // Switch back to MemPstate context 0
  {{5, 3, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, DCT0_MASK + DCT1_MASK, ANY_DIMM_MASK},

  // DCT0
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MASK, ANY_DIMM_MASK},

  // DCT1
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MASK, ANY_DIMM_MASK},

  // MemPstate 1
  {{5, 3, 1}, DO_NOT_CARE, 1, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  // DCT0
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQ  (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT0), S3_PER_DQS (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT0_MEMPSTATE_MASK, ANY_DIMM_MASK},

  // DCT1
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqHiImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqLoImp),      0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQ  (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 0, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 1, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 2, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 3, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 4, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 5, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 6, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 7, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},
  {{1, 1, 1}, S3_DATA (DCT1), S3_PER_DQS (0, 8, S3RegDataTxEqBoostImp),   0x0000FFFF, DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

  {{5, 3, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, DCT0_MEMPSTATE_MASK + DCT1_MEMPSTATE_MASK, ANY_DIMM_MASK},

};

CONST CPCI_REGISTER_BLOCK_HEADER ROMDATA S3CPciPreSelfRefKV = {
  0,
  (sizeof (S3CPciPreSelfDescriptorKV) / sizeof (CONDITIONAL_PCI_REG_DESCRIPTOR)),
  S3CPciPreSelfDescriptorKV,
  PciSpecialCaseFuncKV
};

CONDITIONAL_PCI_REG_DESCRIPTOR ROMDATA S3CPciPostSelfDescriptorKV[] = {
  {{0, 0, 0}, FUNC_5, 0x240, 0xFFFFFFC1, ANY_DIMM_MASK, ANY_DIMM_MASK},
  {{0, 0, 0}, FUNC_5, 0x244, 0x0000FFFF, ANY_DIMM_MASK, ANY_DIMM_MASK},
  {{0, 0, 0}, FUNC_5, 0x248, 0xFFFFFFC0, ANY_DIMM_MASK, ANY_DIMM_MASK},
  {{0, 0, 0}, FUNC_5, 0x24C, 0x0000FFFF, ANY_DIMM_MASK, ANY_DIMM_MASK},

  // LockDramCfg
  {{0, 0, 0}, FUNC_2, 0x118, 0x00040000, ANY_DIMM_MASK, ANY_DIMM_MASK},
  {{0, 0, 0}, FUNC_2, 0x118, 0x00080000, ANY_DIMM_MASK, ANY_DIMM_MASK},

  // Release NBPstate
  {{8, 3, 1}, DO_NOT_CARE, 0, DO_NOT_CARE, ANY_DIMM_MASK, ANY_DIMM_MASK}
};

CONST CPCI_REGISTER_BLOCK_HEADER ROMDATA S3CPciPostSelfRefKV = {
  0,
  (sizeof (S3CPciPostSelfDescriptorKV) / sizeof (CONDITIONAL_PCI_REG_DESCRIPTOR)),
  S3CPciPostSelfDescriptorKV,
  PciSpecialCaseFuncKV
};

MSR_REG_DESCRIPTOR ROMDATA S3MSRPreSelfRefDescriptorKV[] = {
  {{0, 0, 0}, 0xC0010010, 0x00000000007F0000},
  {{0, 0, 0}, 0xC001001A, 0x0000FFFFFF800000},
  {{0, 0, 0}, 0xC001001D, 0x0000FFFFFF800000},
  {{0, 0, 0}, 0xC001001F, 0x0044601080000600}
};

CONST MSR_REGISTER_BLOCK_HEADER ROMDATA S3MSRPreSelfRefKV = {
  0,
  (sizeof (S3MSRPreSelfRefDescriptorKV) / sizeof (MSR_REG_DESCRIPTOR)),
  S3MSRPreSelfRefDescriptorKV,
  NULL
};

VOID *MemS3RegListKV[] = {
  (VOID *)&S3PciPreSelfRefKV,
  NULL,
  (VOID *)&S3CPciPreSelfRefKV,
  (VOID *)&S3CPciPostSelfRefKV,
  (VOID *)&S3MSRPreSelfRefKV,
  NULL,
  NULL,
  NULL
};

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block for S3 resume
 *
 *     @param[in,out]   *S3NBPtr   - Pointer to MEM_NB_BLOCK.
 *     @param[in,out]   *MemPtr  - Pointer to MEM_DATA_STRUCT.
 *     @param[in]       NodeID   - Node ID of the target node.
 *
 *      @return         BOOLEAN
 *                         TRUE - This is the correct constructor for the targeted node.
 *                         FALSE - This isn't the correct constructor for the targeted node.
 */
BOOLEAN
MemS3ResumeConstructNBBlockKV (
  IN OUT   VOID *S3NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  INT32 i;
  MEM_NB_BLOCK *NBPtr;
  UINT8 Dct;

  NBPtr = ((S3_MEM_NB_BLOCK *)S3NBPtr)->NBPtr;

  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[NodeID].SocketId, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedKV (&(MemPtr->DiesPerSystem[NodeID].LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->MemPtr = MemPtr;
  NBPtr->MCTPtr = &(MemPtr->DiesPerSystem[NodeID]);
  NBPtr->PciAddr.AddressValue = MemPtr->DiesPerSystem[NodeID].PciAddr.AddressValue;
  MemNInitNBRegTableKV (NBPtr, NBPtr->NBRegTable);
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->Ganged = FALSE;
  NBPtr->NodeCount = MAX_NODES_SUPPORTED_KV;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_KV;
  NBPtr->MemPstate = MEMORY_PSTATE0;
  NBPtr->MemPstateStage = 0;
  NBPtr->NbPsCtlReg = 0;

  for (i = 0; i < EnumSize; i++) {
    NBPtr->IsSupported[i] = FALSE;
  }

  for (i = 0; i < NumberOfHooks; i++) {
    NBPtr->FamilySpecificHook[i] = (BOOLEAN (*) (MEM_NB_BLOCK *, VOID *)) memDefTrue;
  }

  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &MemPtr->StdHeader);

  NBPtr->SwitchDCT = MemNSwitchDCTNb;
  NBPtr->SwitchChannel = MemNSwitchChannelNb;
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldKV;
  NBPtr->GetBitField = MemNGetBitFieldNb;
  NBPtr->SetBitField = MemNSetBitFieldNb;
  NBPtr->MemNIsIdSupportedNb = MemNIsIdSupportedKV;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3ExitSelfRefReg = (VOID (*) (MEM_NB_BLOCK *, AMD_CONFIG_PARAMS *)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetConPCIMask = MemNS3GetConPCIMaskKV;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetConMSRMask = (VOID (*) (MEM_NB_BLOCK *, DESCRIPTOR_GROUP *)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3Resume = MemNS3ResumeKV;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3RestoreScrub = (VOID (*) (MEM_NB_BLOCK *, UINT8)) memDefRet;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetRegLstPtr = MemNS3GetRegLstPtrKV;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3GetDeviceRegLst = MemNS3GetDeviceRegLstKV;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3SpecialCaseHeapSize = 0;
  ((S3_MEM_NB_BLOCK *)S3NBPtr)->MemS3PspDetect = MemS3PspNotDetected;
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNS3DctCfgSelectKV;

  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, 0);
  MemNSwitchDCTNb (NBPtr, 0);
  MemNSetBitFieldNb (NBPtr, BFMemPsSel, 0);

  // Set DramType and disable unused DCTs
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    MemNSetBitFieldNb (NBPtr, BFDramType, DRAM_TYPE_DDR3_KV);

    if ((Dct == 0) || (Dct == 3)) {
      MemNSetBitFieldNb (NBPtr, BFChannelEn, 1);
    } else {
      MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);
    }
  }

  MemNSwitchDCTNb (NBPtr, 0);
  NBPtr->DctCount = MAX_D3_DCTS_PER_NODE_KV;
  NBPtr->IsSupported[MasterDct1Relocate] = TRUE;

  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function brings the DIMMs out of self refresh for KV
 *
 *     @param[in,out]   *S3NBPtr - Pointer to the S3_MEM_NB_BLOCK
 *     @param[in]       NodeID   - The Node id of the target die
 *
 *     @return         TRUE  - Successful
 *                     FALSE - Fail
 */
BOOLEAN
STATIC
MemNS3ResumeKV (
  IN OUT   S3_MEM_NB_BLOCK *S3NBPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 Dct;
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;

  NBPtr = S3NBPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;

  // Platform hook before exit self refresh
  AGESA_TESTPOINT (TpProcMemBeforeAgesaHookBeforeExitSelfRef, &MemPtr->StdHeader);
  if (AgesaHookBeforeExitSelfRefresh (0, MemPtr) == AGESA_SUCCESS) {
  }
  AGESA_TESTPOINT (TpProcMemAfterAgesaHookBeforeExitSelfRef, &MemPtr->StdHeader);

  //Override the NB Pstate if needed
  IDS_OPTION_HOOK (IDS_NB_PSTATE_DIDVID, S3NBPtr->NBPtr, &MemPtr->StdHeader);

  // Clear DisAutoComp to complete predriver programming
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
      MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);
      // Enable PHY Calibration
      MemNS3EnablePhyCalibrationKV (NBPtr);

      // PHY Power Saving
      MemNS3PhyPowerSavingKV (NBPtr);
    }

    // Program D18F2x9C_x0002_0080[3:0][PmuClkDiv]]=7 (PMU CLK turned off).
    MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv), 7);
  }

  // Change power state
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
      MemNSetBitFieldNb (NBPtr, BFMemResetL, 1);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 0);
    }
  }

  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
      // Query for the calibrate completion.
      MemNS3PendOnPhyCalibrateCompletionKV (NBPtr);
      // Set calibration rate.
      MemNS3RateOfPhyCalibrateKV (NBPtr);
    }
  }

  // Sync channel init
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
      MemNSetBitFieldNb (NBPtr, BFPtrInitReq, 1);
      while (MemNGetBitFieldNb (NBPtr, BFPtrInitReq) != 0) {}
    }
  }

  // Exit self refresh
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
      MemNSetBitFieldNb (NBPtr, BFExitSelfRef, 1);
      while (MemNGetBitFieldNb (NBPtr, BFExitSelfRef) != 0) {}
      MemUWait10ns ((7800 * 2) / 10, MemPtr);  // wait Tref * 2
      // Program D18F2x78_dct[3:0][ChanVal]=1.
      MemNSetBitFieldNb (NBPtr, BFChanVal, 1);
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function returns the register list for each device for KV
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in, out]  *DescriptPtr - Pointer to DESCRIPTOR_GROUP
 *     @return          UINT16 - size of the device descriptor on the target node.
 */
UINT16
STATIC
MemNS3GetRegLstPtrKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  )
{
  UINT8 i;
  UINT16 Size;
  Size = 0;
  for (i = PRESELFREF; i <= POSTSELFREF; i ++) {
    DescriptPtr->PCIDevice[i].Type = (UINT8) (DEV_TYPE_PCI_PRE_ESR + i);
    DescriptPtr->PCIDevice[i].Node = NBPtr->Node;
    DescriptPtr->PCIDevice[i].RegisterListID = 0xFFFFFFFF;
    if ((PCI_REGISTER_BLOCK_HEADER *) MemS3RegListKV[PCI_LST_ESR_KV - PCI_LST_ESR_KV + i] != NULL) {
      DescriptPtr->PCIDevice[i].RegisterListID = PCI_LST_ESR_KV + i;
      Size += sizeof (PCI_DEVICE_DESCRIPTOR);
    }
    DescriptPtr->CPCIDevice[i].Type = (UINT8) (DEV_TYPE_CPCI_PRE_ESR + i);
    DescriptPtr->CPCIDevice[i].Node = NBPtr->Node;
    DescriptPtr->CPCIDevice[i].RegisterListID = 0xFFFFFFFF;
    if ((CPCI_REGISTER_BLOCK_HEADER *) MemS3RegListKV[CPCI_LST_ESR_KV - PCI_LST_ESR_KV + i] != NULL) {
      DescriptPtr->CPCIDevice[i].RegisterListID = CPCI_LST_ESR_KV + i;
      Size += sizeof (CONDITIONAL_PCI_DEVICE_DESCRIPTOR);
    }
    DescriptPtr->MSRDevice[i].Type = (UINT8) (DEV_TYPE_MSR_PRE_ESR + i);
    DescriptPtr->MSRDevice[i].RegisterListID = 0xFFFFFFFF;
    if ((MSR_REGISTER_BLOCK_HEADER *) MemS3RegListKV[MSR_LST_ESR_KV - PCI_LST_ESR_KV + i] != NULL) {
      DescriptPtr->MSRDevice[i].RegisterListID = MSR_LST_ESR_KV + i;
      Size += sizeof (MSR_DEVICE_DESCRIPTOR);
    }
    DescriptPtr->CMSRDevice[i].Type = (UINT8) (DEV_TYPE_CMSR_PRE_ESR + i);
    DescriptPtr->CMSRDevice[i].RegisterListID = 0xFFFFFFFF;
    if ((CMSR_REGISTER_BLOCK_HEADER *) MemS3RegListKV[CMSR_LST_ESR_KV - PCI_LST_ESR_KV + i] != NULL) {
      DescriptPtr->CMSRDevice[i].RegisterListID = CMSR_LST_ESR_KV + i;
      Size += sizeof (CONDITIONAL_MSR_DEVICE_DESCRIPTOR);
    }
  }
  return Size;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function return the register list according to the register ID.
 *
 *     @param[in]   RegisterLstID - value of the Register list ID.
 *     @param[out]  **RegisterHeader - pointer to the address of the register list.
  *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @return      AGESA_STATUS
 *                          - AGESA_FATAL
 *                          - AGESA_SUCCESS
 */
AGESA_STATUS
STATIC
MemNS3GetDeviceRegLstKV (
  IN       UINT32 RegisterLstID,
     OUT   VOID **RegisterHeader,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  if (RegisterLstID >= (sizeof (MemS3RegListKV) / sizeof (VOID *))) {
    ASSERT(FALSE); // RegisterListID exceeded size of Register list
    return AGESA_FATAL;
  }
  if (MemS3RegListKV[RegisterLstID] != NULL) {
    *RegisterHeader = MemS3RegListKV[RegisterLstID];
    return AGESA_SUCCESS;
  }
  ASSERT(FALSE); // Device register list error
  return AGESA_FATAL;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function setups default phy registers eg. PmuReset, PmuStall, MajorMode...
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetDfltPhyRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT8 Mode;
  UINT8 Dct;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {
        // 1. Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
        MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);
        MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);

        // 2. Program D18F2x9C_x0002_000E_dct[3:0][PhyDisable]=0.
        MemNSetBitFieldNb (NBPtr, BFPhyDisable, 0);

        // 3. According to the type of DRAM attached, program D18F2x9C_x00FFF04A_dct[3:0][MajorMode],
        //    D18F2x9C_x0002_000E_dct[3:0][G5_Mode], and D18F2x9C_x0002_0098_dct[3:0][CalG5D3].
        Mode = DRAM_TYPE_DDR3_KV;
        MemNSetBitFieldNb (NBPtr, RegRxCtl1, Mode << 4);
        // Mem P-state 0, normal D3 data/dqs receiver
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, RegDctPhy0x01F1F043), 0x0);
        // Mem P-state 1, low-power D3 data/dqs receiver
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDctPhy0x01F1F043), 0x4);

        MemNSetBitFieldNb (NBPtr, BFG5Mode, Mode);
        MemNSetBitFieldNb (NBPtr, BFG5ModeAbyte, Mode);
        MemNSetBitFieldNb (NBPtr, BFG5ModeDbyte, Mode);
        MemNSetBitFieldNb (NBPtr, BFG5ModeAbit, Mode);
        MemNSetBitFieldNb (NBPtr, BFCalG5D3, Mode);

        // Program D18F2x9C_x0002_0087_dct[3:0][DisAutoComp, DisPredriverCal] = {1,1}.
        MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
        MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);
      }
    }
    MemNSwitchDCTNb  (NBPtr, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function sets Phy frequency
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetPhyFreqKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT16 Freq;
  UINT16 PllMultDiv;
  UINT16 PMUClkDiv;
  UINT8 Dct;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
      MemNSwitchDCTNb  (NBPtr, Dct);
      if (MemNGetBitFieldNb (NBPtr, BFDisDramInterface) == 0) {

        // 1. Program D18F2x9C_x0002_0093_dct[3:0][PllRegWaitTime] = 4Bh.
        MemNSetBitFieldNb (NBPtr, RegPllRegWaitTime, 0x4B);

        // 2. Program D18F2x9C_x0002_0089_dct[3:0][PllLockTime] = 190h. Restore the default PLL lock time.
        MemNSetBitFieldNb (NBPtr, RegPllLockTime, 0x190);

        // 3. Program D18F2x9C_x0002_0000_dct[3:0][PllMultDiv]. PMU Reset must be high.
        Freq = (UINT16) MEMCLK_ID_TO_FREQ (MemNGetBitFieldNb (NBPtr, BFMemClkFreq)) * 2;
        PllMultDiv = ((Freq / 100) * 0x100) + ((Freq % 100) / 20);

        // 4. Program D18F2x9C_x0002_0080_dct[3:0][PMUClkDiv].
        // PMU must operate no faster than 533MHz
        PMUClkDiv = CEIL_DIV (((CEIL_DIV (Freq, 533) < 1) ? 0 : CEIL_DIV (Freq, 533)), 2);

        MemNSetBitFieldNb (NBPtr, RegPllMemPs0, PllMultDiv);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv),  PMUClkDiv);

        MemNSetBitFieldNb (NBPtr, RegPllMemPs1, 0x0603);
        MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFPMUClkDiv), 1);
      }
    }
    MemNSwitchDCTNb  (NBPtr, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function does the channel disable sequence
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3DisableChannelKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  BIT_FIELD_NAME bitfield;
  UINT32 RegVal;
  UINT8 DimmMask;
  UINT8 Die;

  DimmMask = 0;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    // Function field contains the DCT number
    NBPtr->SwitchDCT (NBPtr, (UINT8) Address.Address.Function);
    //RegValue = MemNGetBitFieldNb (NBPtr, RegPwrStateCmd);

    for (bitfield = BFCSBaseAddr0Reg; bitfield <= BFCSBaseAddr3Reg; ++bitfield) {
      RegVal = MemNGetBitFieldNb (NBPtr, bitfield);
      if (RegVal & 0x1) {
        DimmMask |= (UINT8) (1 << (bitfield - BFCSBaseAddr0Reg));
      }
    }

    if (DimmMask == 0) {
      // Clear this bit before phy access
      MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);

      //  Tristate Vref on unused channel
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 2);
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut0Sel, 0);
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut1Sel, 0);
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut2Sel, 0);
      MemNSetBitFieldNb (NBPtr, BFGlobalVrefOut3Sel, 0);

      // Tri-state drivers
      MemNSetBitFieldNb (NBPtr, RegTxImpedance, 0);

      // Program D18F2x9C_x00FA_F04A_dct[3:03:03:0] = 0080h.  Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
      MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

      // Phy LP2 state
      MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);

      //Disable powergating the ss channel when the Dcc is powergated
      MemNSetBitFieldNb (NBPtr, BFDisDllShutdownSR, 1);

      // Disable DCT
      MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);

      // Shut down the power to the SRAM array for maximum power savings.
      MemNSetBitFieldNb (NBPtr, BFPmuSramSd, 1);
    }
  }
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function changes memory Pstate context
 *
 *     @param[in]   AccessWidth - Access width of the register.
 *     @param[in]   Address - address in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *
 *     @return    TRUE
 * ----------------------------------------------------------------------------
 */
VOID
STATIC
MemNS3ChangeMemPStateContextAndFlowNb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;

  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;

  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;
    if (NBPtr->MemPstate == MEMORY_PSTATE0) {
      // If MemoryPstate is not disabled, switch to MemPState 1 context, and reprocess the register list
      MemNChangeMemPStateContextNb (NBPtr, 1);
      *(UINT32 *) Value = RESTART_FROM_BEGINNING_LIST;
    } else {
      // Switch back to MemPstate0 Context
      MemNChangeMemPStateContextNb (NBPtr, 0);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function returns the conditional PCI device mask
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in, out]  *DescriptPtr - Pointer to DESCRIPTOR_GROUP
 *      @return         none
 */
VOID
STATIC
MemNS3GetConPCIMaskKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   DESCRIPTOR_GROUP *DescriptPtr
  )
{
  BIT_FIELD_NAME bitfield;
  UINT32 RegVal;
  UINT8 DCT;
  UINT8 DimmMask;
  UINT8 BadDimmMask;
  UINT8 NbPsCapMsk;
  UINT8 MemPstateMsk;
  UINT8 CsPerDelay;

  NbPsCapMsk = 0;
  MemPstateMsk = 0;
  DimmMask = 0;
  BadDimmMask = 0;
  CsPerDelay = 1;

  for (DCT = 0; DCT < NBPtr->DctCount; DCT ++) {
    MemNSwitchDCTNb (NBPtr, DCT);
    if (MemNGetBitFieldNb (NBPtr, BFMemClkFreqVal)) {
      if (MemNGetBitFieldNb (NBPtr, BFPerRankTimingEn) == 0) {
        CsPerDelay = 2;
      }
      for (bitfield = BFCSBaseAddr0Reg; bitfield <= BFCSBaseAddr3Reg; bitfield ++) {
        RegVal = MemNGetBitFieldNb (NBPtr, bitfield);
        if (RegVal & 0x1) {
          DimmMask |= (UINT8) (1 << ((((bitfield - BFCSBaseAddr0Reg) / CsPerDelay) << 1) + DCT));
        } else if (RegVal & 0x4) {
          BadDimmMask |= (UINT8) (1 << ((((bitfield - BFCSBaseAddr0Reg) / CsPerDelay) << 1) + DCT));
        }
      }
    }
  }
  // Check if the system is capable of doing NB Pstate change
  if (MemNGetBitFieldNb (NBPtr, BFNbPstateDis) == 0) {
    NbPsCapMsk = DCT0_NBPSTATE_SUPPORT_MASK;
  }
  if (MemNGetBitFieldNb (NBPtr, BFMemPstateDis) == 0) {
    MemPstateMsk = DCT0_MEMPSTATE_MASK;
  }

  MemNSwitchDCTNb (NBPtr, 0);
  // Set channel mask
  DescriptPtr->CPCIDevice[PRESELFREF].Mask1 = 0;
  DescriptPtr->CPCIDevice[POSTSELFREF].Mask1 = 0;
  for (DCT = 0; DCT < NBPtr->DctCount; DCT ++) {
    if (DimmMask & (0x55 << DCT)) {
      // Set mask before exit self refresh
      DescriptPtr->CPCIDevice[PRESELFREF].Mask1 |= (NbPsCapMsk | MemPstateMsk | 1) << DCT;
      // Set mask after exit self refresh
      DescriptPtr->CPCIDevice[POSTSELFREF].Mask1 |= (NbPsCapMsk | MemPstateMsk | 1) << DCT;
    } else if (BadDimmMask & (0x55 << DCT)) {
      // Need to save function 2 registers for bad dimm
      DescriptPtr->CPCIDevice[PRESELFREF].Mask1 |= 1 << DCT;
    }
  }

  // Set dimm mask
  DescriptPtr->CPCIDevice[PRESELFREF].Mask2 = DimmMask;
  DescriptPtr->CPCIDevice[POSTSELFREF].Mask2 = DimmMask;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of CSR register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetCSRKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 ExtendOffset;
  UINT32 TempFunc;

  ExtendOffset = Address.Address.Register;
  TempFunc = Address.Address.Function;

  // Switch Dct
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  Dct = 0;
  if (ExtendOffset & 0x400) {
    Dct = 3;
  }
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  Address.Address.Function = TempFunc;
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Register = 0x98;
  ExtendOffset &= 0x3FF;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ExtendOffset, ConfigPtr);
  IDS_OPTION_HOOK (IDS_AFTER_DCT_PHY_ACCESS, NULL, ConfigPtr);
  Address.Address.Register = 0x9C;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a CSR register
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetCSRKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 ExtendOffset;
  UINT32 ValueWrite;
  UINT32 TempFunc;

  ExtendOffset = Address.Address.Register;

  TempFunc = Address.Address.Function;
  // Switch Dct
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  Dct = 0;
  if (ExtendOffset & 0x400) {
    Dct = 3;
  }
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Function = TempFunc;
  Address.Address.Register = 0x9C;

  ExtendOffset &= 0x3FF;
  ExtendOffset |= 0x40000000;
  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    ValueWrite = *(UINT8 *) Value;
    break;
  case AccessS3SaveWidth16:
    ValueWrite = *(UINT16 *) Value;
    break;
  case AccessS3SaveWidth32:
    ValueWrite = *(UINT32 *) Value;
    break;
  default:
    ASSERT (FALSE);
  }
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ValueWrite, ConfigPtr);
  Address.Address.Register = 0x98;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &ExtendOffset, ConfigPtr);
  IDS_OPTION_HOOK (IDS_AFTER_DCT_PHY_ACCESS, NULL, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads and writes register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in]   IsSet - if this is a register read or write
 *     @param[in, out]  *Value - Pointer to the value be read or  written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetSetBitFieldKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN       BOOLEAN IsSet,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  BIT_FIELD_NAME BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;
  UINT8 InstNum;
  UINT8 Byte;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
    S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
    NBPtr = S3NBPtr[Die].NBPtr;

    // Sync DCT
    MemNSetBitFieldNb (NBPtr, BFDctCfgSel, FIX_DCT (NBPtr->Dct));

    // Function field:
    //   Bit 0:     DCT number
    NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

    // Bit/Register field:
    BitField = (BIT_FIELD_NAME) (Address.Address.Register);

    ASSERT (MaxS3Field <= 16);  // Only 16 S3 fields are supported for chiplet select

    // Function field:
    //   Bit [2:1]: If non-zero, support SW broadcast for phy registers
    switch ((Address.Address.Function >> 1)) {
    case 0:
      // Non-chiplet-select registers, no translation needed
      break;
    case 1:
      // Select per NB Pstate
      InstNum = (UINT8) (BitField >> 4);
      BitField = PER_NB_PS (InstNum, S3FieldTransTab[BitField & 0xF]);
      break;
    case 2:
      // Select per CAD pad name
      InstNum = (UINT8) (BitField >> 4);
      BitField = PER_CAD_PAD (InstNum, ALL_PADS, S3FieldTransTab[BitField & 0xF]);
      break;
    case 3:
      InstNum = ((BitField & 0x800) != 0) ? PAD_DQS : PAD_DQ;
      Byte = (UINT8) (BitField >> 4) & 0x1F;
      if (Byte == 0x1F) {
        Byte = ALL_BYTES;
      }

      BitField = PER_DATA_BYTE (InstNum, Byte, S3FieldTransTab[BitField & 0xF]);
      break;
    default:
      ASSERT (FALSE);
    }

    if (IsSet) {
      switch (AccessWidth) {
      case AccessS3SaveWidth8:
        RegValue = *(UINT8 *) Value;
        break;
      case AccessS3SaveWidth16:
        RegValue = *(UINT16 *) Value;
        break;
      case AccessS3SaveWidth32:
        RegValue = *(UINT32 *) Value;
        break;
      default:
        ASSERT (FALSE);
      }
      MemNSetBitFieldNb (NBPtr, BitField, RegValue);
    } else {
      RegValue = MemNGetBitFieldNb (NBPtr, BitField);

      switch (AccessWidth) {
      case AccessS3SaveWidth8:
        *(UINT8 *) Value = (UINT8) RegValue;
        break;
      case AccessS3SaveWidth16:
        *(UINT16 *) Value = (UINT16) RegValue;
        break;
      case AccessS3SaveWidth32:
        *(UINT32 *) Value = RegValue;
        break;
      default:
        ASSERT (FALSE);
      }
    }
  } else {
    ASSERT (FALSE);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads PHY register by instance
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetInstanceKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  BIT_FIELD_NAME BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) != AGESA_SUCCESS) {
    ASSERT (FALSE);
  }

  S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
  NBPtr = S3NBPtr[Die].NBPtr;

  // Sync DCT
  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, FIX_DCT (NBPtr->Dct));

  // Function field:
  //   Bit 0:     DCT number
  NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

  // Bit/Register field:
  BitField = (BIT_FIELD_NAME) (Address.Address.Register);

  RegValue = MemNGetBitFieldNb (NBPtr, PER_INSTANCE (BitField));

  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    *(UINT8 *) Value = (UINT8) RegValue;
    break;
  case AccessS3SaveWidth16:
    *(UINT16 *) Value = (UINT16) RegValue;
    break;
  case AccessS3SaveWidth32:
    *(UINT32 *) Value = RegValue;
    break;
  default:
    ASSERT (FALSE);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function writes PHY register by instance
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetInstanceKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  BIT_FIELD_NAME BitField;
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT32 RegValue;
  UINT8 Die;

  RegValue = 0;
  // See which Node should be accessed
  Die = (UINT8) (Address.Address.Device - 24);

  LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
  if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) != AGESA_SUCCESS) {
    ASSERT (FALSE);
  }

  S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
  NBPtr = S3NBPtr[Die].NBPtr;

  // Sync DCT
  MemNSetBitFieldNb (NBPtr, BFDctCfgSel, FIX_DCT (NBPtr->Dct));

  // Function field:
  //   Bit 0:     DCT number
  NBPtr->SwitchDCT (NBPtr, (UINT8) (Address.Address.Function & 1));

  // Bit/Register field:
  BitField = (BIT_FIELD_NAME) (Address.Address.Register);

  switch (AccessWidth) {
  case AccessS3SaveWidth8:
    RegValue = *(UINT8 *) Value;
    break;
  case AccessS3SaveWidth16:
    RegValue = *(UINT16 *) Value;
    break;
  case AccessS3SaveWidth32:
    RegValue = *(UINT32 *) Value;
    break;
  default:
    ASSERT (FALSE);
  }

  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (BitField), RegValue);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function reads register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetBitFieldKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MemNS3GetSetBitFieldKV (AccessWidth, Address, FALSE, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function writes register bitfield
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value to be written.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetBitFieldKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MemNS3GetSetBitFieldKV (AccessWidth, Address, TRUE, Value, ConfigPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function changes memory Pstate context
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 * ----------------------------------------------------------------------------
 */
VOID
STATIC
MemNS3ChangeMemPStateContextKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  LOCATE_HEAP_PTR LocateBufferPtr;
  S3_MEM_NB_BLOCK *S3NBPtr;
  UINT8 Die;
  UINT8 MemPstate;

  MemPstate = (UINT8) Address.Address.Register;

  ASSERT (MemPstate < 2);   // Only support 2 memory pstates

  IDS_SKIP_HOOK (IDS_BEFORE_S3_SPECIAL, &Address, ConfigPtr) {
    // See which Node should be accessed
    Die = (UINT8) (Address.Address.Device - 24);

    LocateBufferPtr.BufferHandle = AMD_MEM_S3_NB_HANDLE;
    if (HeapLocateBuffer (&LocateBufferPtr, ConfigPtr) == AGESA_SUCCESS) {
      S3NBPtr = (S3_MEM_NB_BLOCK *) LocateBufferPtr.BufferPtr;
      NBPtr = S3NBPtr[Die].NBPtr;
      MemNSetBitFieldNb (NBPtr, BFMemPsSel, MemPstate);
      NBPtr->MemPstate = MemPstate;
      IDS_HDT_CONSOLE (MEM_FLOW, "Changing Memory Pstate to %x\n", NBPtr->MemPstate);
    } else {
      IDS_HDT_CONSOLE (MEM_FLOW, "Can not find the S3 handle\n");
    }
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a register that has one copy for each NB Pstate
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3GetNBPStateDepRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 NBPstate;
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  NBPstate = (UINT8) (Temp >> 10);
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 3);
  Temp &= 0x3FF;

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xC8) | ((NBPstate << 4) | Dct);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Function = FUNC_2;
  Address.Address.Register = Temp;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);

  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  TempValue = 0;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function write to a register that has one copy for each NB Pstate
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the CSR register in PCI_ADDR format.
 *     @param[in, out]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SetNBPStateDepRegKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 NBPstate;
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  NBPstate = (UINT8) (Temp >> 10);
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 3);
  Temp &= 0x3FF;

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xCE) | ((NBPstate << 4) | Dct);
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Function = FUNC_2;
  Address.Address.Register = Temp;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);

  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  TempValue = 0;
  LibAmdPciWrite (AccessS3SaveWidth32, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function read the value of Function 2 PCI register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the NB register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be read.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3SaveNBRegisterKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 3);

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Register = Temp;
  Address.Address.Function = FUNC_2;
  LibAmdPciRead (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_GETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function set the value of Function 2 PCI register.
 *
 *     @param[in]   AccessWidth - Access width of the register
 *     @param[in]   Address - address of the NB register in PCI_ADDR format.
 *     @param[in]  *Value - Pointer to the value be write.
 *     @param[in, out]  *ConfigPtr - Pointer to Config handle.
 *     @return         none
 */
VOID
STATIC
MemNS3RestoreNBRegisterKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  )
{
  UINT8 TempValue;
  UINT8 Dct;
  UINT32 Temp;

  Temp = Address.Address.Register;
  Dct = (UINT8) ((Address.Address.Function == 0) ? 0 : 3);

  // Switch Dct
  // Function field contains DCT value
  Address.Address.Function = FUNC_1;
  Address.Address.Register = 0x10C;
  LibAmdPciRead (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  TempValue = (TempValue & 0xFC) | Dct;
  LibAmdPciWrite (AccessS3SaveWidth8, Address, &TempValue, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, Dct);

  Address.Address.Register = Temp;
  Address.Address.Function = FUNC_2;
  LibAmdPciWrite (AccessWidth, Address, Value, ConfigPtr);
  IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x = %x\n", Address.Address.Function, Address.Address.Register, *(UINT32 *)Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function is used by families that use a separate DctCfgSel bit to
 *     select the current DCT which will be accessed by function 2.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *Dct     - Pointer to ID of the target DCT
 *
 */

BOOLEAN
MemNS3DctCfgSelectKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  )
{
  //
  // Set the DctCfgSel to new DCT
  // In the case DDR3 with Graphics downcore disabled, channel B is controlled by DCT3
  //
  MemNCmnGetSetFieldKV (NBPtr, 1, BFDctCfgSel, FIX_DCT (*(UINT8 *) Dct));

  return TRUE;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *     Enable DDR3 PHY calibrating
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3EnablePhyCalibrationKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xA3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xB3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 0);
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x83);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Waits for PHY calibrating to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3PendOnPhyCalibrateCompletionKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // read CalBusy and discard result - this is needed to latch the
  // initial 'busy' status
  MemNGetBitFieldNb (NBPtr, RegCalBusy);

  // Wait until D18F2x9C_x20097 == 0.
  MemNPollBitFieldNb (NBPtr, RegCalBusy, 0, PCI_ACCESS_TIMEOUT, FALSE);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Set the rate for PHY calibrate
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3RateOfPhyCalibrateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalInitMode, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x03);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x13);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function tri-state unused pads.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNS3TriStateUnusedPadKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  //===================================================================
  // Tristate DATA[8] if ECC is not supported
  //===================================================================
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state ECC Pad\n");
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDataTxImpedance), 0);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, RegDataTxImpedance), 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     PHY Power Saving
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNS3PhyPowerSavingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{

  CONST UINT8 DLL_RESET_RELOCK_SHIFT = 7;
  INT8 MemPstate;
  UINT8 LowestMemPstate;
  UINT16 Speed;

  LowestMemPstate = 1;

  //----------------------------------------------------------------
  //  Tri-State unused pads for Power Saving
  //----------------------------------------------------------------
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tS3 DRAM PHY Power Savings\n");
  for (MemPstate = LowestMemPstate; MemPstate >= 0; MemPstate--) {
    // When memory pstate is enabled, this loop will goes through M1 first then M0
    // Otherwise, this loop only goes through M0.
    MemNS3SwitchMemPstateKV (NBPtr, MemPstate);

    if (NBPtr->MemPstate == 0) {
      Speed = (UINT16) MEMCLK_ID_TO_FREQ (MemNGetBitFieldNb (NBPtr, BFMemClkFreq));
    } else {
      Speed = DDR667_FREQUENCY;
    }

    // 1. Program DctPhyA41 as follows to disable unused pads.
    // DrvStrenP = DrvStrenP = 0.
    // See 2.10.4. Software does this for each unconnected pad in the package or each pad
    // connected to unused pins
    MemNS3TriStateUnusedPadKV (NBPtr);

    // 2. Program D18F2x9C_x0[3,1:0][F,3:0]0_0014_dct[3:0][MaxDurDllNoLock] = 0.
    // DctPhyA78[DllResetRelock] must be set to 1 prior to writing this register
    // and then DctPhyA78[DllResetRelock] must be cleared after the register
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
    MemNSetBitFieldNb (NBPtr, BFCadMaxDurDllNoLock, 0);
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, 0);

    // 3. Program D18F2x9C_x0[3,1:0][F,3:0]1_0014_dct[3:0][MaxDurDllNoLock] as follows:
    // If (DDR rate <= 1066) MaxDurDllNoLock = 9h
    // else MaxDurDllNoLock = 7h
    // DctPhyD78[DllResetRelock] must be set to 1 prior to writing this register
    // and then DctPhyD78[DllResetRelock] must be cleared after the register
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tData DLL standby\n");
    MemNSetBitFieldNb (NBPtr, RegDctPhyD78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
    if (Speed <= DDR1066_FREQUENCY) {
      MemNSetBitFieldNb (NBPtr, BFDataMaxDurDllNoLock, 9);
    } else {
      MemNSetBitFieldNb (NBPtr, BFDataMaxDurDllNoLock, 7);
    }
    MemNSetBitFieldNb (NBPtr, RegDctPhyD78DllControl, 0);

    // Program static value for DllPumpPeriod
    MemNSetBitFieldNb (NBPtr, BFDataDllPumpPeriod, 3);

    if (NBPtr->MemPstate == 1) {
      // 4. Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[3:0] as follows:
      // For M1 context program ODTStrenN = ODTStrenP = 0
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), 0);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), 0);

      // 5. Program D18F2x9C_x01F1_F043_dct[3:0] = 0004h.
      // bc to all ccccmmmm in M1. Rcvrs in low power mode
      MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDataRcvMajorMode), 0x0004);
    }
  }

  // 6. Program D18F2x9C_x00FA_F04A_dct[3:0] = 0080h.
  // Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
  MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

  // 7. Power down unused DBYTE 9 (undocumented spare) as follows:
  // Program D18F2x9C_x0091_F04A_dct[] = 0280h
  // Program D18F2x9C_x0091_0F77_dct[] = 07CFh. Turn off all DLLs
  // Program D18F2x9C_x0091_0000_dct[] = 00004. Turn Dbyte off
  // Program D18F2x9C_x0091_0F77_dct[] = 07EFh. Turn off XCLK
  // Program D18F2x9C_x0391_F04D_dct[] = 0000h. Disable Odt
  // Program D18F2x9C_x0391_F041_dct[] = 0000h. Disable output driver impedance
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0091F04A), 0x280);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910F77), 0x07CF);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910000), 0x0004);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD00910F77), 0x07EF);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0391F04D), 0x0000);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, UNUSED_BYTE, RegDctPhyD0391F041), 0x0000);

  // 8. If ECC memory is not connected or is disabled, or the package does not support ECC,
  // then power down data chiplet 8 with the following:
  // Write to all memory P-state instances if available.
  // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04A_dct[3:0] = 0280h.
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[3:0] = 07CFh.  Turn off all DLLs
  // Program D18F2x9C_x00[F,8:0]1_0000_dct[3:0] = 00004.  Turn Dbyte off
  // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[3:0] = 07EFh.  Turn off XCLK
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[3:0] = 0000h.  Disable Odt
  // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]041_dct[3:0] = 0000h.  Disable output driver impedance
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0091F04A), 0x280);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07CF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910000), 0x0004);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07EF);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F04D), 0x0000);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F041), 0x0000);
  }

  // 9. Power down unused receivers in data chips as follows:
  // Program D18F2x9C_x00F1_804A_dct[] = 280h.  EDC pad.
  // Program D18F2x9C_x00F1_904A_dct[] = 280h.  DBI pad.
  // If x4 DIMMs are not present then program D18F2x9C_x00F1_B04A_dct[] = 280h.  MEMDQSU_H and MEMDQSU_L pads
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1804A), 0x0280);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1904A), 0x0280);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1B04A), 0x0280);

  // 10. Power down the PMU
  MemNDisablePmuKV (NBPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function switches memory pstate for KV
 *
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in]       MemPstate - Mem Pstate
 *
 * ----------------------------------------------------------------------------
 */
VOID
STATIC
MemNS3SwitchMemPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  )
{
  ASSERT (MemPstate < 2);   // Only support 2 memory pstates
  IDS_HDT_CONSOLE (MEM_FLOW, "MemPsSel = %d\n", MemPstate);
  MemNSetBitFieldNb (NBPtr, BFMemPsSel, MemPstate);
  NBPtr->MemPstate = MemPstate;
}

