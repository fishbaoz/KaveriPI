/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Reset API, and related functions and structures.
 *
 * Contains code that initialized the CPU after early reset.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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

#ifndef _CPU_POST_INIT_H_
#define _CPU_POST_INIT_H_


/*---------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *---------------------------------------------------------------------------------------
 */
//  Forward declaration needed for multi-structure mutual references
AGESA_FORWARD_DECLARATION (CPU_CFOH_FAMILY_SERVICES);

/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */
#define P_STATE_DATA_GATHER_TEMP_ADDR     0x200000ul ///< Fixed the row data at 2M memory address.
/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
//----------------------------------------------------------------------------
//                         POST INIT TYPEDEFS, STRUCTURES, ENUMS
//
//----------------------------------------------------------------------------
/// BSC to AP MSR sync up
typedef struct {
  UINT32 RegisterAddress;   ///< MSR Address
  UINT64 RegisterValue;     ///< BSC's MSR Value
} BSC_AP_MSR_SYNC;

/**
 *  Set Cache Flush On Halt Register.
 *
 *  @CpuServiceInstances
 *
 * @param[in]          FamilySpecificServices   The current Family Specific Services.
 * @param[in]          EntryPoint               Timepoint designator.
 * @param[in]          PlatformConfig           Contains the runtime modifiable feature input data.
 * @param[in]          StdHeader                Handle of Header for calling lib functions and services.
 *
 */
typedef VOID (F_CPU_SET_CFOH_REG) (
  IN       CPU_CFOH_FAMILY_SERVICES     *FamilySpecificServices,
  IN       UINT64                       EntryPoint,
  IN       PLATFORM_CONFIGURATION       *PlatformConfig,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  );

 /// Reference to a Method.
typedef F_CPU_SET_CFOH_REG *PF_CPU_SET_CFOH_REG;

/**
 * Provide the interface to the Cache Flush On Halt Family Specific Services.
 *
 * Use the methods or data in this struct to adapt the feature code to a specific cpu family or model (or stepping!).
 * Each supported Family must provide an implementation for all methods in this interface, even if the
 * implementation is a CommonReturn().
 */
struct _CPU_CFOH_FAMILY_SERVICES {                     // See forward reference above
  UINT16          Revision;                             ///< Interface version
  // Public Methods.
  PF_CPU_SET_CFOH_REG SetCacheFlushOnHaltRegister;    ///< Method: Set Cache Flush On Halt register.
};

/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */

//                   These are   P U B L I C   functions, used by IBVs
AGESA_STATUS
AmdCpuPost (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       PLATFORM_CONFIGURATION *PlatformConfig
  );

//                   These are   P U B L I C   functions, used by AGESA

AGESA_STATUS
CopyHeapToTempRamAtPost (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
SyncApMsrsToBsc (
  IN OUT   BSC_AP_MSR_SYNC    *ApMsrSync,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

AGESA_STATUS
FinalizeAtPost (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
GetPstateGatherDataAddressAtPost (
     OUT   UINT64            **Ptr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
SyncAllApMtrrToBsc (
  IN       VOID *MtrrTable,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );
#endif  // _CPU_POST_INIT_H_

