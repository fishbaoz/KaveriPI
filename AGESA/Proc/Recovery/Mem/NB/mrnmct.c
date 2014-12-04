/* $NoKeywords:$ */
/**
 * @file
 *
 * mrnmct.c
 *
 * Northbridge Common MCT supporting functions Recovery
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem/NB)
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
#include "mrport.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "cpuFamilyTranslation.h"
#include "cpuCacheInit.h"
#include "Filecode.h"
#define FILECODE PROC_RECOVERY_MEM_NB_MRNMCT_FILECODE

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

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function is the Recovery memory configuration function for Nb DDR3
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_ALERT
 *                          - AGESA_FATAL
 *                          - AGESA_SUCCESS
 *                          - AGESA_WARNING
 */

AGESA_STATUS
MemRecNMemInitNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  AGESA_STATUS Status;
  MEM_TECH_BLOCK *TechPtr;

  TechPtr = NBPtr->TechPtr;

  NBPtr->MemRecNInitializeMctNb (NBPtr);

  if (NBPtr->IsSupported[DramModeBeforeDimmPres]) {
    TechPtr->SetDramMode (TechPtr);
  }

  Status = AGESA_FATAL;
  if (TechPtr->DimmPresence (TechPtr)) {

    if (NBPtr->IsSupported[DramModeAfterDimmPres]) {
      TechPtr->SetDramMode (TechPtr);
    }

    if (MemRecNAutoConfigNb (NBPtr)) {

      AGESA_TESTPOINT (TpProcMemPlatformSpecificConfig, &(NBPtr->MemPtr->StdHeader));
      if (MemRecNPlatformSpecNb (NBPtr)) {
        AgesaHookBeforeDramInitRecovery (0, NBPtr->MemPtr);
        AGESA_TESTPOINT (TpProcMemStartDcts, &(NBPtr->MemPtr->StdHeader));
        MemRecNStartupDCTNb (NBPtr);

        AGESA_TESTPOINT (TpProcMemMtrrConfiguration, &(NBPtr->MemPtr->StdHeader));
        MemRecNCPUMemRecTypingNb (NBPtr);

        AGESA_TESTPOINT (TpProcMemDramTraining, &(NBPtr->MemPtr->StdHeader));
        NBPtr->TrainingFlow (NBPtr);

        Status = AGESA_SUCCESS;
      }
    }
  }

  NBPtr->MemRecNFinalizeMctNb (NBPtr);

  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function returns a physical address of a corresponding Chip select
 *
 *     @return      Addr -  System Address
 */

UINT32
MemRecNGetMCTSysAddrNb ( VOID )
{
  // Hardcode the base base address to 2M bytes.
  return (1ul << 21);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function runs on the BSP only, it sets the fixed MTRRs for common legacy ranges.
 *   It sets TOP_MEM and TOM2 and some variable MTRRs with WB Uncacheable type.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemRecNCPUMemRecTypingNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  S_UINT64 SMsr;

  MEM_DATA_STRUCT *MemPtr;
  MemPtr = NBPtr->MemPtr;

  //
  //======================================================================
  // Set default values for CPU registers
  //======================================================================
  //

  LibAmdMsrRead (SYS_CFG, (UINT64 *)&SMsr, &MemPtr->StdHeader);
  SMsr.lo |= 0x1C0000;                // turn on modification enable bit and
                                      // mtrr enable bits
  LibAmdMsrWrite (SYS_CFG, (UINT64 *)&SMsr, &MemPtr->StdHeader);

  SMsr.lo = SMsr.hi = 0x1E1E1E1E;
  LibAmdMsrWrite (0x250, (UINT64 *)&SMsr, &MemPtr->StdHeader);      // 0 - 512K = WB Mem
  LibAmdMsrWrite (0x258, (UINT64 *)&SMsr, &MemPtr->StdHeader);      // 512K - 640K = WB Mem

  LibAmdMsrRead (SYS_CFG, (UINT64 *)&SMsr, &MemPtr->StdHeader);
  SMsr.lo &= 0xFFF7FFFF;                // turn off modification enable bit
  LibAmdMsrWrite (SYS_CFG, (UINT64 *)&SMsr, &MemPtr->StdHeader);
  //
  //======================================================================
  // Set TOP_MEM and TOM2 CPU registers
  //======================================================================
  //
  SMsr.hi = 0;
  SMsr.lo = 0x08000000;
  LibAmdMsrWrite (TOP_MEM, (UINT64 *)&SMsr, &MemPtr->StdHeader);  // TOP_MEM

  // Set FS Base address for later memory accesses
  SMsr.lo = 0;
  LibAmdMsrWrite (FS_BASE, (UINT64 *)&SMsr, &MemPtr->StdHeader);

  //
  //======================================================================
  // Set variable MTRR values
  //======================================================================
  //
  SMsr.lo = 0x00000006;
  LibAmdMsrWrite (0x200, (UINT64 *)&SMsr, &MemPtr->StdHeader);      // MTRRPhysBase0

  SMsr.hi = NBPtr->VarMtrrHiMsk;
  SMsr.lo = 0xF8000800;
  LibAmdMsrWrite (0x201, (UINT64 *)&SMsr, &MemPtr->StdHeader);      // MTRRPhysMask0

}

/*-----------------------------------------------------------------------------*/
/**
 *
 *     This function returns the upper 32 bits mask for variable MTRR based on
 *     the CPU_LOGICAL_ID.
 *     @param[in]       *LogicalIdPtr - Pointer to the CPU_LOGICAL_ID
 *     @param[in]       StdHeader - Header for library and services
 *
 *     @return          UINT32 - MTRR mask for upper 32 bits
 *
 */
UINT32
MemRecGetVarMtrrHiMsk (
  IN       CPU_LOGICAL_ID *LogicalIdPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 TempNotCare;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;
  CACHE_INFO *CacheInfoPtr;

  GetCpuServicesFromLogicalId (LogicalIdPtr, (CONST CPU_SPECIFIC_SERVICES **)&FamilySpecificServices, StdHeader);
  FamilySpecificServices->GetCacheInfo (FamilySpecificServices, (CONST VOID **)&CacheInfoPtr, &TempNotCare, StdHeader);
  return (UINT32) (CacheInfoPtr->VariableMtrrMask >> 32);
}

/*-----------------------------------------------------------------------------
 *
 *
 *     This function re-enable phy compensation.
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  OptParam   - Optional parameter
 *
 *     @return    TRUE
 * ----------------------------------------------------------------------------
 */
BOOLEAN
MemRecNReEnablePhyCompNb (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 Dct;

  Dct = NBPtr->Dct;

  NBPtr->SwitchDCT (NBPtr, 0);
  // Clear DisableCal and set DisablePredriverCal
  MemRecNSetBitFieldNb (NBPtr, BFDisablePredriverCal, 0x2000);
  NBPtr->SwitchDCT (NBPtr, Dct);

  return TRUE;
}
/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
