/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD binary block interface
 *
 * Contains the block entry function dispatcher
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Legacy
 * @e \$Revision: 192403 $   @e \$Date: 2012-12-12 15:46:25 -0600 (Wed, 12 Dec 2012) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2008 - 2013 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *           2013 - 2014 Sage Electronic Engineering, LLC
 * All Rights Reserved.
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
 */

/*----------------------------------------------------------------------------------------
 *                            M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Dispatcher.h"
#include "Options.h"
#include "Filecode.h"
#include <Ids.h>
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

extern const AMD_IMAGE_HEADER mImageID;

#define FILECODE LEGACY_PROC_DISPATCHER_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern const DISPATCH_TABLE DispatchTable[];
extern const AMD_MODULE_HEADER mCpuModuleID;

/*---------------------------------------------------------------------------------------*/
/**
 * The Dispatcher is the entry point into the AGESA software. It takes a function
 * number as entry parameter in order to invoke the published function
 *
 * @param[in,out]  ConfigPtr
 *
 * @return   AGESA Status.
 *
 */
AGESA_STATUS
CALLCONV
AmdAgesaDispatcher (
  IN OUT   VOID *ConfigPtrParm
  )
{
	AGESA_STATUS Status = AGESA_UNSUPPORTED;
	AMD_CONFIG_PARAMS *ConfigPtr = ConfigPtrParm;

	{
		DISPATCH_TABLE *Entry;
		Entry = (DISPATCH_TABLE *) DispatchTable;
		while (Entry->FunctionId != 0) {
			if (ConfigPtr->Func == Entry->FunctionId) {
				Status = Entry->EntryPoint (ConfigPtr);
				break;
			}
			Entry++;
		}
		ASSERT(Entry->EntryPoint != NULL);
	}

	// 2. Try next dispatcher if possible, and we have not already got status back
	if ((mCpuModuleID.NextBlock != NULL) && (Status == AGESA_UNSUPPORTED)) {
		MODULE_ENTRY  ModuleEntry = NULL;
		ModuleEntry = (MODULE_ENTRY) (UINT64) mCpuModuleID.NextBlock->ModuleDispatcher;
		if (ModuleEntry != NULL) {
		  Status = (*ModuleEntry) (ConfigPtr);
		}
	}


	// 3. If not this image specific function, see if we can find alternative image instead
	if (Status == AGESA_UNSUPPORTED) {
		IMAGE_ENTRY ImageEntry = NULL;
		UINT32 ImageStart = 0xFFE80000;
		UINT32 ImageEnd = 0xFFFC0000;
		AMD_IMAGE_HEADER* AltImagePtr = NULL;
		if ((((AMD_CONFIG_PARAMS *)ConfigPtr)->AltImageBasePtr != 0xFFFFFFFF  ) || (((AMD_CONFIG_PARAMS *)ConfigPtr)->AltImageBasePtr != 0)) {
			ImageStart = ((AMD_CONFIG_PARAMS *)ConfigPtr)->AltImageBasePtr;
			ImageEnd = ImageStart + 4;
			// Locate/test image base that matches this component
			AltImagePtr = LibAmdLocateImage (
					(VOID *)ImageStart,
					(VOID *)ImageEnd,
					4096,
					(CHAR8 *)AGESA_ID
				);
			if (AltImagePtr != NULL) {
				//Invoke alternative Image
				ImageEntry = (IMAGE_ENTRY) ((UINT64) AltImagePtr + AltImagePtr->EntryPointAddress);
				Status = (*ImageEntry) (ConfigPtr);
			}
		}
	}

	return (Status);
}

/*---------------------------------------------------------------------------------------*/
/**
 * The host environment interface of callout.
 *
 * @param[in]      Func
 * @param[in]      Data
 * @param[in,out]  ConfigPtr
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
CALLCONV
AmdAgesaCallout (
  IN       UINT32  Func,
  IN       UINT32  Data,
  IN OUT   VOID    *ConfigPtr
  )
{
  UINT32 Result;
  Result = AGESA_UNSUPPORTED;
  if (((AMD_CONFIG_PARAMS *) ConfigPtr)->CalloutPtr == NULL) {
    return Result;
  }

  Result = (((AMD_CONFIG_PARAMS *) ConfigPtr)->CalloutPtr) (Func, Data, ConfigPtr);
  return (Result);
}
