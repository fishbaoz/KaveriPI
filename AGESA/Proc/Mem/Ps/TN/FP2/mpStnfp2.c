/* $NoKeywords:$ */
/**
 * @file
 *
 * mpStnfp2.c
 *
 * Platform specific settings for TN DDR3 SO-DIMM FP2 system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Ps/TN/FP2)
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

#include "AGESA.h"
#include "AdvancedApi.h"
#include "mport.h"
#include "ma.h"
#include "Ids.h"
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "mm.h"
#include "mn.h"
#include "mp.h"
#include "mu.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_PS_TN_FP2_MPSTNFP2_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*
 *-----------------------------------------------------------------------------
 *                                EXPORTED FUNCTIONS
 *
 *-----------------------------------------------------------------------------
 */
// Slow mode, Address timing and Output drive compensation for soldered down SODIMM configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   SlowMode,   AddTmgCtl,   ODC
//
STATIC CONST PSCFG_SAO_ENTRY TNSODWNSODdr3SAO[] = {
  {_DIMM_NONE, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 0, 0x00000000, 0x00002222},
  {_DIMM_NONE, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000035, 0x30222323},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x10002222},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00000000, 0x10002222},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000035, 0x30222323},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR, NP, NP, 0, 0x003D3D3D, 0x20002222},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_DR, NP, NP, 0, 0x00003D3D, 0x20002222},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1, 0x00000035, 0x30222323},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30112222},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30112222},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_SR, NP, NP, 0, 0x003C3C3C, 0x30112222},
  {_DIMM_NONE, DDR1866, V1_5, DIMM_DR, NP, NP, 1, 0x00003C3C, 0x30112222},
  {_DIMM_NONE, DDR2133, V1_5, DIMM_SR, NP, NP, 0, 0x003B3B3B, 0x30112222},
  {_DIMM_NONE, DDR2133, V1_5, DIMM_DR, NP, NP, 1, 0x00003B3B, 0x30112222},
  {_DIMM_NONE, DDR2400, V1_5, DIMM_SR, NP, NP, 0, 0x003A3A3A, 0x30112222},
  {_DIMM_NONE, DDR2400, V1_5, DIMM_DR, NP, NP, 1, 0x00003A3A, 0x30112222}
};
CONST PSC_TBL_ENTRY TNSAOTblEntSODWNSO3 = {
   {PSCFG_SAO, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_15_TN, AMD_F15_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY},
   sizeof (TNSODWNSODdr3SAO) / sizeof (PSCFG_SAO_ENTRY),
   (VOID *)&TNSODWNSODdr3SAO
};

// Dram Term and Dynamic Dram Term for soldered down SODIMM configuration
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   Dimm,   Rank,   RttNom,    RttWr
//
// RttNom:
// 0 On die termination disabled
// 1 60ohms
// 2 120ohms
// 3 40ohms
// 4 20ohms
// 5 30ohms
// RttWr:
// 0 Dynamic termination for writes disabled.
// 1 60ohms
// 2 120ohms
STATIC CONST PSCFG_RTT_ENTRY DramTermTNSODWNSODIMM[] = {
  {_DIMM_NONE, DDR667 + DDR800 + DDR1066, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, DIMM_SR + DIMM_DR, R0 + R1, 2, 0},
  {_DIMM_NONE, DDR667 + DDR800, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_SR + DIMM_DR, R0 + R1, 3, 2},
  {_DIMM_NONE, DDR1066, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_SR + DIMM_DR, R0 + R1, 5, 2},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, DIMM_SR + DIMM_DR, R0 + R1, 1, 0},
  {_DIMM_NONE, DDR1333, VOLT_ALL, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, DIMM_SR + DIMM_DR, R0 + R1, 5, 2},
  {_DIMM_NONE, DDR1600, V1_5 + V1_35, DIMM_SR + DIMM_DR, NP, NP, DIMM_SR + DIMM_DR, R0 + R1, 3, 0},
  {_DIMM_NONE, DDR1866 + DDR2133 + DDR2400, V1_5, DIMM_SR + DIMM_DR, NP, NP, DIMM_SR + DIMM_DR, R0 + R1, 3, 0}
};
CONST PSC_TBL_ENTRY TNDramTermTblEntSODWNSO = {
   {PSCFG_RTT, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_15_TN, AMD_F15_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY},
   sizeof (DramTermTNSODWNSODIMM) / sizeof (PSCFG_RTT_ENTRY),
   (VOID *)&DramTermTNSODWNSODIMM
};

// Max Freq. for TN FP2 SODIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MaxFreqTNSODIMMFP2[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY}
};
CONST PSC_TBL_ENTRY TNMaxFreqTblEntSOFP2 = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_TN, AMD_F15_TN_ONLY}, TN_SOCKET_FP2, DDR3_TECHNOLOGY},
   sizeof (MaxFreqTNSODIMMFP2) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxFreqTNSODIMMFP2
};

// Max Freq. for RL FP2 SODIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MaxFreqRLSODIMMFP2[] = {
  {_1DIMM, 1, 1, 0, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1066_FREQUENCY}
};
CONST PSC_TBL_ENTRY RLMaxFreqTblEntSOFP2 = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_RL, AMD_F15_RL_ALL}, TN_SOCKET_FP2, DDR3_TECHNOLOGY},
   sizeof (MaxFreqRLSODIMMFP2) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxFreqRLSODIMMFP2
};

// Max Freq. for soldered down SODIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MaxFreqTNSODWNSODIMM[] = {
  {_DIMM_NONE, 1, 1, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_DIMM_NONE, 1, 0, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_DIMM_NONE, 2, 2, 0, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_DIMM_NONE, 2, 1, 1, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY},
  {_DIMM_NONE, 2, 0, 2, 0, DDR1333_FREQUENCY, DDR1333_FREQUENCY, DDR1333_FREQUENCY}
};
CONST PSC_TBL_ENTRY TNMaxFreqTblEntSODWNSO = {
   {PSCFG_MAXFREQ, SODWN_SODIMM_TYPE, _DIMM_NONE, {AMD_FAMILY_15_TN, AMD_F15_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY},
   sizeof (MaxFreqTNSODWNSODIMM) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxFreqTNSODWNSODIMM
};

//
// MemClkDis
//
STATIC CONST UINT8 ROMDATA TNSODdr3CLKDisFP2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY TNClkDisMapEntSOFP2 = {
   {PSCFG_CLKDIS, SODIMM_TYPE + UDIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FP2, DDR3_TECHNOLOGY},
   sizeof (TNSODdr3CLKDisFP2) / sizeof (UINT8),
   (VOID *)&TNSODdr3CLKDisFP2
};

//
// ODT tri-state
//
STATIC CONST UINT8 ROMDATA TNSODdr3ODTTriFP2[] = {0xFF, 0xFF, 0xFF, 0xFF};
CONST PSC_TBL_ENTRY TNSODdr3ODTTriEntFP2 = {
   {PSCFG_ODTTRI, SODIMM_TYPE + UDIMM_TYPE,  _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FP2, DDR3_TECHNOLOGY},
   sizeof (TNSODdr3ODTTriFP2) / sizeof (UINT8),
   (VOID *)&TNSODdr3ODTTriFP2
};

//
// ChipSel tri-state
//
STATIC CONST UINT8 ROMDATA TNSODdr3CSTriFP2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY TNSODdr3CSTriEntFP2 = {
   {PSCFG_CSTRI, SODIMM_TYPE + UDIMM_TYPE,  _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FP2, DDR3_TECHNOLOGY},
   sizeof (TNSODdr3CSTriFP2) / sizeof (UINT8),
   (VOID *)&TNSODdr3CSTriFP2
};