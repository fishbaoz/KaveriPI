/* $NoKeywords:$ */
/**
 * @file
 *
 * mrp.c
 *
 * Common platform specific configuration.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem/Ps)
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
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "OptionMemory.h"
#include "ma.h"
#include "mp.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_PS_MRP_FILECODE


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
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
extern MEM_PSC_FLOW_BLOCK* memRecPlatSpecFlowArray[];

/* -----------------------------------------------------------------------------*/
/**
 *
 *    This function gets platform specific configuration such as Max Freq., Slow Mode, Dram Term,
 *    and so on.
 *
 *     @param[in]       *NBPtr       Pointer to MEM_NB_BLOCK
 *     @return          TRUE - Successfully execute platform specific configuration flow.
 *     @return          FALSE - Fail to execute platform specific configuration flow.
 *
 */
BOOLEAN
MemPRecPSCFlow (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;

  i = 0;
  while (memRecPlatSpecFlowArray[i] != NULL) {
    if ((memRecPlatSpecFlowArray[i])->DramTerm (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
      if ((memRecPlatSpecFlowArray[i])->ODTPattern (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
        if ((memRecPlatSpecFlowArray[i])->SAO (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
          if ((memRecPlatSpecFlowArray[i])->MR0WrCL (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
            if ((memRecPlatSpecFlowArray[i])->RC2IBT (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
              if ((memRecPlatSpecFlowArray[i])->RC10OpSpeed (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
                if ((memRecPlatSpecFlowArray[i])->LRIBT (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
                  if ((memRecPlatSpecFlowArray[i])->LRNPR (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
                    if ((memRecPlatSpecFlowArray[i])->LRNLR (NBPtr, (memRecPlatSpecFlowArray[i])->EntryOfTables)) {
                      return TRUE;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    i++;
  }
  return FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function constructs the rank type map of Dimm0, Dimm1, Dimm2. Also it counts the number
 *     of dimm in the table.
 *
 *     @param[in]       Dimm0       Rank type of Dimm0
 *     @param[in]       Dimm1       Rank type of Dimm1
 *     @param[in]       Dimm2       Rank type of Dimm2
 *     @param[in, out]       *RankTypeInTable       Pointer to RankTypeInTable variable
 *
 *
 */
VOID
MemPRecConstructRankTypeMap (
  IN       UINT16 Dimm0,
  IN       UINT16 Dimm1,
  IN       UINT16 Dimm2,
  IN OUT   UINT16 *RankTypeInTable
  )
{
  UINT8 i;
  UINT16 RT;
  UINT8 BitShift;

  *RankTypeInTable = 0;
  RT = 0;
  BitShift = 0;

  for (i = 0; i < MAX_DIMMS_PER_CHANNEL; i++) {
    switch (i) {
    case 0:
      RT = (Dimm0 == 0) ? NP : Dimm0;
      BitShift = 0;
      break;
    case 1:
      RT = (Dimm1 == 0) ? NP : Dimm1;
      BitShift = 4;
      break;
    case 2:
      RT = (Dimm2 == 0) ? NP : Dimm2;
      BitShift = 8;
      break;
    default:
      // dimm3 is not used, fills nibble3 with "NP"
      RT = NP;
      BitShift = 12;
    }
    *RankTypeInTable |= RT << BitShift;
  }
}

/*-----------------------------------------------------------------------------*/
/**
 *     MemPIsIdSupported
 *      This function matches the CPU_LOGICAL_ID and PackageType with certain criteria to
 *      determine if it is supported by this NB type.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       LogicalId - CPU_LOGICAL_ID
 *     @param[in]       PackageType - Package Type
 *
 *     @return          TRUE -  NB type is matched !
 *     @return          FALSE - NB type is not matched !
 *
 */
BOOLEAN
MemPRecIsIdSupported (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       CPU_LOGICAL_ID LogicalId,
  IN       UINT8 PackageType
  )
{
  CPUID_DATA CpuId;
  UINT8 PkgType;

  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, &(NBPtr->MemPtr->StdHeader));
  PkgType = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28

  if (((NBPtr->MCTPtr->LogicalCpuid.Family & LogicalId.Family) != 0)
      && ((NBPtr->MCTPtr->LogicalCpuid.Revision & LogicalId.Revision) != 0)) {
    if ((PackageType == PT_DONT_CARE) || (PackageType == PkgType)) {
      return TRUE;
    }
  }
  return FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    This function returns the rank type map of a channel.
 *
 *     @param[in]       *CurrentChannel       Pointer to CH_DEF_STRUCT
 *
 *     @return          UINT16 - The map of rank type.
 *
 */
UINT16
MemPRecGetPsRankType (
  IN       CH_DEF_STRUCT *CurrentChannel
  )
{
  UINT8 i;
  UINT16 DIMMRankType;

  DIMMRankType = 0;
  for (i = 0; i < MAX_DIMMS_PER_CHANNEL; i++) {
    if ((CurrentChannel->DimmQrPresent & (UINT8) 1 << i) != 0) {
      if (i < 2) {
        DIMMRankType |= (UINT16) 8 << (i << 2);
      }
    } else if ((CurrentChannel->DimmDrPresent & (UINT8) 1 << i) != 0) {
      DIMMRankType |= (UINT16) 4 << (i << 2);
    } else if ((CurrentChannel->DimmSRPresent & (UINT8) 1 << i) != 0) {
      DIMMRankType |= (UINT16) 2 << (i << 2);
    } else {
      DIMMRankType |= (UINT16) 1 << (i << 2);
    }
  }
  return DIMMRankType;
}
