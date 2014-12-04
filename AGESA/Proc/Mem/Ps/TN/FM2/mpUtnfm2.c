/* $NoKeywords:$ */
/**
 * @file
 *
 * mpUtnfm2.c
 *
 * Platform specific settings for TN DDR3 UDIMM FM2 system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Ps/TN/FM2)
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

#define FILECODE PROC_MEM_PS_TN_FM2_MPUTNFM2_FILECODE
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
//
// MemClkDis
//
STATIC CONST UINT8 ROMDATA TNUDdr3CLKDisFM2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY TNClkDisMapEntUFM2 = {
   {PSCFG_CLKDIS, UDIMM_TYPE + SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (TNUDdr3CLKDisFM2) / sizeof (UINT8),
   (VOID *)&TNUDdr3CLKDisFM2
};

//
// ODT tri-state
//
STATIC CONST UINT8 ROMDATA TNUDdr3ODTTriFM2[] = {0xFF, 0xFF, 0xFF, 0xFF};
CONST PSC_TBL_ENTRY TNUDdr3ODTTriEntFM2 = {
   {PSCFG_ODTTRI, UDIMM_TYPE + SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (TNUDdr3ODTTriFM2) / sizeof (UINT8),
   (VOID *)&TNUDdr3ODTTriFM2
};

//
// ChipSel tri-state [UDIMM]
//
STATIC CONST UINT8 ROMDATA TNUDdr3CSTriFM2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY TNUDdr3CSTriEntFM2 = {
   {PSCFG_CSTRI, UDIMM_TYPE + SODIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (TNUDdr3CSTriFM2) / sizeof (UINT8),
   (VOID *)&TNUDdr3CSTriFM2
};

//
// Max Freq. Of TN
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MaxFreqTNUDIMMFM2[] = {
  {_2DIMM, 1, 1, 0, 0, DDR2133_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, DDR1066_FREQUENCY}
};
CONST PSC_TBL_ENTRY TNMaxFreqTblEntUFM2 = {
   {PSCFG_MAXFREQ, UDIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_TN, AMD_F15_TN_ONLY}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (MaxFreqTNUDIMMFM2) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxFreqTNUDIMMFM2
};

//
// Max Freq. Of RL
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC CONST PSCFG_MAXFREQ_ENTRY ROMDATA MaxFreqRLUDIMMFM2[] = {
  {_2DIMM, 1, 1, 0, 0, DDR2133_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1333_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, DDR1066_FREQUENCY}
};
CONST PSC_TBL_ENTRY RLMaxFreqTblEntUFM2 = {
   {PSCFG_MAXFREQ, UDIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_RL, AMD_F15_RL_ALL}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (MaxFreqRLUDIMMFM2) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxFreqRLUDIMMFM2
};

// 2D training configuratrions
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   2D
//
STATIC CONST PSCFG_S2D_ENTRY TNUDdr3S2DFM2[] = {
  // DimmPerCh,Frequency,VDDIO,DIMM0,DIMM1,DIMM2,Enable2DTraining
  {1, DDR1333 + DDR1600 + DDR1866 + DDR2133, VOLT_ALL, DIMM_SR + DIMM_DR, NP, NP, 1},
  {2, DDR1333 + DDR1600 + DDR1866 + DDR2133, VOLT_ALL, NP + DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, NP, 1}
  };
CONST PSC_TBL_ENTRY S2DTblEntUFM2 = {
   {PSCFG_S2D, UDIMM_TYPE + SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_TN, AMD_F15_ALL}, TN_SOCKET_FM2, DDR3_TECHNOLOGY},
   sizeof (TNUDdr3S2DFM2) / sizeof (PSCFG_S2D_ENTRY),
   (VOID *)&TNUDdr3S2DFM2
};