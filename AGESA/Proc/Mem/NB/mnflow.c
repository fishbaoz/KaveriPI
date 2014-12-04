/* $NoKeywords:$ */
/**
 * @file
 *
 * mnflow.c
 *
 * Common Northbridge initializer flow for MCT and DCT
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB)
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
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_NB_MNFLOW_FILECODE
/* features */
#include "mftds.h"

extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];
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

BOOLEAN
STATIC
MemNInitDCTNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNCleanupDctRegsNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNGetPORFreqLimitTblDrvNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function programs the MCT with initial values
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - AGESA_FATAL error did not occur (it is possible to have an Error that is not AGESA_SUCCESS)
 *     @return          FALSE - AGESA_FATAL error occurred
 */

BOOLEAN
MemNInitMCTNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MEM_TECH_BLOCK *TechPtr;
  UINT8 Dct;
  BOOLEAN Flag;
  ID_INFO CallOutIdInfo;

  TechPtr = NBPtr->TechPtr;
  // Switch Tech functions for Nb
  NBPtr->TechBlockSwitch (NBPtr);
  // Start Memory controller initialization sequence
  Flag = FALSE;
  if (TechPtr->DimmPresence (TechPtr)) {
    AGESA_TESTPOINT (TpProcMemPlatformSpecificInit, &(NBPtr->MemPtr->StdHeader));
    if (NBPtr->MemNPlatformSpecificFormFactorInitNb (NBPtr)) {
      AGESA_TESTPOINT (TpProcMemSpdTiming, &(NBPtr->MemPtr->StdHeader));
      if (TechPtr->SpdCalcWidth (TechPtr)) {
        AGESA_TESTPOINT (TpProcMemSpeedTclConfig, &(NBPtr->MemPtr->StdHeader));
        if (TechPtr->SpdGetTargetSpeed (TechPtr)) {
          for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
            NBPtr->SwitchDCT (NBPtr, Dct);

            Flag |= MemNInitDCTNb (NBPtr);
          }

          if (Flag && !NBPtr->IsSupported[TwoStageDramInit] && (NBPtr->MCTPtr->ErrCode != AGESA_FATAL)) {
            MemFInitTableDrive (NBPtr, MTBeforeDInit);
            AGESA_TESTPOINT (TpProcMemBeforeAgesaHookBeforeDramInit, &(NBPtr->MemPtr->StdHeader));
            IDS_PERF_TIMESTAMP (TP_BEGINAGESAHOOKBEFOREDRAMINIT, &(NBPtr->MemPtr->StdHeader));
            CallOutIdInfo.IdField.SocketId = NBPtr->MCTPtr->SocketId;
            CallOutIdInfo.IdField.ModuleId = NBPtr->MCTPtr->DieId;
            IDS_HDT_CONSOLE (MEM_FLOW, "\nCalling out to Platform BIOS on Socket %d Module %d...\n", CallOutIdInfo.IdField.SocketId, CallOutIdInfo.IdField.ModuleId);
            AgesaHookBeforeDramInit ((UINTN) CallOutIdInfo.IdInformation, NBPtr->MemPtr);
            IDS_HDT_CONSOLE (MEM_FLOW, "\nVDDIO = 1.%dV\n", (NBPtr->RefPtr->DDR3Voltage == VOLT1_5) ? 5 :
                                                  (NBPtr->RefPtr->DDR3Voltage == VOLT1_35) ? 35 :
                                                  (NBPtr->RefPtr->DDR3Voltage == VOLT1_25) ? 25 : 999);
            AGESA_TESTPOINT (TpProcMemAfterAgesaHookBeforeDramInit, &(NBPtr->MemPtr->StdHeader));
            IDS_PERF_TIMESTAMP (TP_ENDAGESAHOOKBEFOREDRAMINIT, &(NBPtr->MemPtr->StdHeader));
            IDS_OPTION_HOOK (IDS_BEFORE_DRAM_INIT, NBPtr, &(NBPtr->MemPtr->StdHeader));
            NBPtr->InitMCT (NBPtr);
            NBPtr->StartupDCT (NBPtr);
          }
        }
      }
    }
  }
  return (BOOLEAN) (NBPtr->MCTPtr->ErrCode != AGESA_FATAL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function initializes the platform specific block for families that support
 *      table driven form factor
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - AGESA_SUCCESS
 */

BOOLEAN
MemNPlatformSpecificFormFactorInitTblDrvNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    NBPtr->SwitchDCT (NBPtr, Dct);
    NBPtr->PsPtr->MemPDoPs = MemPPSCFlow;
    NBPtr->PsPtr->MemPGetPORFreqLimit = MemNGetPORFreqLimitTblDrvNb;
    NBPtr->PsPtr->MemPGetPass1Seeds = MemPGetPSCPass1Seed;
  }

  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function programs the DCT with initial values
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - Error did not occur
 *     @return          FALSE - Error occurred
 */

BOOLEAN
STATIC
MemNInitDCTNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MEM_TECH_BLOCK *TechPtr;
  TechPtr = NBPtr->TechPtr;
  TechPtr->SetDramMode (TechPtr);

  if (!NBPtr->MCTPtr->GangedMode || (NBPtr->MCTPtr->Dct == 0)) {
    if (NBPtr->DCTPtr->Timings.DctDimmValid == 0) {
      NBPtr->DisableDCT (NBPtr);
    } else {
      MemNCleanupDctRegsNb (NBPtr);
      if (TechPtr->AutoCycTiming (TechPtr)) {
        if (TechPtr->SpdSetBanks (TechPtr)) {
          if (NBPtr->StitchMemory (NBPtr)) {
            // if all dimms on a DCT are disabled, the DCT needs to be disabled.
            if (NBPtr->DCTPtr->Timings.CsEnabled != 0) {
              if (NBPtr->AutoConfig (NBPtr)) {
                if (NBPtr->PlatformSpec (NBPtr)) {
                  return TRUE;
                }
              }
            } else {
              NBPtr->DisableDCT (NBPtr);
            }
          }
        }
      }
    }
  }
  return FALSE;
}

/*-----------------------------------------------------------------------------*/
/**
 *
 *      This function clears DCT registers
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNCleanupDctRegsNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  BIT_FIELD_NAME BitField;

  for (BitField = BFCSBaseAddr0Reg; BitField <= BFCSBaseAddr7Reg; BitField++) {
    MemNSetBitFieldNb (NBPtr, BitField, 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This is function gets the POR speed limit for families supports table driven form factor
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNGetPORFreqLimitTblDrvNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;

  i = 0;
  while (memPlatSpecFlowArray[i] != NULL) {
    if ((memPlatSpecFlowArray[i])->MaxFrequency (NBPtr, (memPlatSpecFlowArray[i])->EntryOfTables)) {
      break;
    }
    i++;
  }
  // Check if there is no table found across CPU families. If so, disable channels.
  if (memPlatSpecFlowArray[i] == NULL) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\nDCT %d: No MaxFreq table. This channel will be disabled.\n", NBPtr->Dct);
    NBPtr->DCTPtr->Timings.DimmExclude |= NBPtr->DCTPtr->Timings.DctDimmValid;
    PutEventLog (AGESA_ERROR, MEM_ERROR_UNSUPPORTED_DIMM_CONFIG, NBPtr->Node, NBPtr->Dct, NBPtr->Channel, 0, &NBPtr->MemPtr->StdHeader);
    SetMemError (AGESA_ERROR, NBPtr->MCTPtr);
    // Change target speed to highest value so it won't affect other channels when leveling frequency across the node.
    NBPtr->DCTPtr->Timings.TargetSpeed = UNSUPPORTED_DDR_FREQUENCY;
  }
}
