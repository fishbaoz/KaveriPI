/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CRAT option API.
 *
 * Contains structures and values used to control the CRAT option code.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  OPTION
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
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
 ******************************************************************************
 */

#ifndef _OPTION_CRAT_H_
#define _OPTION_CRAT_H_

#include "cpuLateInit.h"
#include "cpuCrat.h"

/*----------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *----------------------------------------------------------------------------------------
 */

typedef AGESA_STATUS OPTION_CRAT_FEATURE (
  IN OUT   AMD_CONFIG_PARAMS     *StdHeader,
  IN OUT   VOID                  **CratPtr
  );

#define CRAT_STRUCT_VERSION  0x01

/// The Option Configuration of CRAT
typedef struct  {
  UINT16              OptCratVersion;       ///< The version number of CRAT
  OPTION_CRAT_FEATURE  *CratFeature;        ///< The Option Feature of CRAT
  UINT8               OemIdString[6];       ///< Configurable OEM Id
  UINT8               OemTableIdString[8];  ///< Configurable OEM Table Id
} OPTION_CRAT_CONFIGURATION;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to generat CRAT cache affinity structure.
 *
 *    @param[in]      CratHeaderStructPtr  CRAT header pointer
 *    @param[in, out] TableEnd             The end of CRAT
 *    @param[in, out] StdHeader            Standard Head Pointer
 *
 *
 */

typedef VOID F_GENERATE_CRAT_CACHE (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

/// Reference to a Method.
typedef F_GENERATE_CRAT_CACHE         *PF_GENERATE_CRAT_CACHE;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to generat CRAT TLB affinity structure.
 *
 *    @param[in]      CratHeaderStructPtr  CRAT header pointer
 *    @param[in, out] TableEnd             The end of CRAT
 *    @param[in, out] StdHeader            Standard Head Pointer
 *
 *
 */

typedef VOID F_GENERATE_CRAT_TLB (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

/// Reference to a Method.
typedef F_GENERATE_CRAT_TLB         *PF_GENERATE_CRAT_TLB;

/**
 * Provide the interface to the CRAT Family Specific Services.
 *
 * Use the methods or data in this struct to adapt the feature code to a specific cpu family or model (or stepping!).
 * Each supported Family must provide an implementation for all methods in this interface, even if the
 * implementation is a CommonReturn().
 */
typedef struct _CRAT_FAMILY_SERVICES {
  UINT16          Revision;                                        ///< Interface version
  // Public Methods.
  PF_GENERATE_CRAT_CACHE   generateCratCacheEntry;                 ///< Method: Family specific call to generat CRAT cache affinity structure
  PF_GENERATE_CRAT_TLB     generateCratTLBEntry;                   ///< Method: Family specific call to generat CRAT TLB affinity structure

} CRAT_FAMILY_SERVICES;

/*----------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *----------------------------------------------------------------------------------------
 */
VOID
MakeHSAProcUnitEntry (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
MakeMemoryEntry (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
MakeCacheEntry (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
MakeTLBEntry (
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/// @todo
//VOID
//MakeFPUEntry (
//  IN       CRAT_HEADER *CratHeaderStructPtr,
//  IN OUT   UINT8       **TableEnd,
//  IN OUT   AMD_CONFIG_PARAMS *StdHeader
//  );

/// @todo
//VOID
//MakeIOEntry (
//  IN       CRAT_HEADER *CratHeaderStructPtr,
//  IN OUT   UINT8       **TableEnd,
//  IN OUT   AMD_CONFIG_PARAMS *StdHeader
//  );

UINT8 *
AddOneCratEntry (
  IN       CRAT_ENTRY_TYPE CratEntryType,
  IN       CRAT_HEADER *CratHeaderStructPtr,
  IN OUT   UINT8       **TableEnd,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

#endif  // _OPTION_CRAT_H_

