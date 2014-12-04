/*****************************************************************************
 *
 * Copyright (c) 2013 - 2014, Sage Electronic Engineering, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Advanced Micro Devices, Inc. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***************************************************************************/
#include "AGESA.h"
#include "FieldAccessors.h"

extern CONST ACCESSOR_DISPATCH_TABLE ROMDATA AccessorTable[];

/**
 * Find the Setter/Getter dispatch entry in the dispatch table.
 *
 * @param[in]  Field Number
 *
 * @return   A pointer to the dispatch entry for the field number specified.  NULL is
 *           returned if no dispatch entry is found.
 *
 */
STATIC
CONST ACCESSOR_DISPATCH_TABLE*
FindAccessorEntry (
  IN       UINT32 field
  )
{
  CONST ACCESSOR_DISPATCH_TABLE *Entry;

  for (Entry = &AccessorTable;
		  (Entry->FunctionId != 0) && (Entry->FunctionId != field);
		  ++Entry)
  {}
  return ((field == Entry->FunctionId) ? Entry : NULL);
}

/**
 * The Dispatcher is the entry point into the AGESA software. It takes a function
 * number as entry parameter in order to invoke the published function
 *
 * @param[in,out]  ConfigPtr
 *
 * @return   AGESA Status.
 *
 */
/**********************************************************************
 * Interface call:  AmdSetValue
 **********************************************************************/
AGESA_STATUS
AmdSetValueDispatch (
  IN OUT   AMD_ACCESSOR_PARAMS *AccessorParams
  )
{
	AGESA_STATUS retval = AGESA_UNSUPPORTED;
	CONST ACCESSOR_DISPATCH_TABLE* Entry = FindAccessorEntry(AccessorParams->FieldName);
	if ((Entry != NULL) && (Entry->SetValueEntryPoint != NULL)) {
		retval = Entry->SetValueEntryPoint(
				AccessorParams->FieldValue,
				AccessorParams->FieldSize
			);
	}
	return retval;
}

/**
 * The Dispatcher is the entry point into the AGESA software. It takes a function
 * number as entry parameter in order to invoke the published function
 *
 * @param[in,out]  ConfigPtr
 *
 * @return   AGESA Status.
 *
 */
/**********************************************************************
 * Interface call:  AmdSetValue
 **********************************************************************/
AGESA_STATUS
AmdGetValueDispatch (
  IN OUT   AMD_ACCESSOR_PARAMS *AccessorParams
  )
{
	AGESA_STATUS retval = AGESA_UNSUPPORTED;
	CONST ACCESSOR_DISPATCH_TABLE* Entry = FindAccessorEntry(AccessorParams->FieldName);
	if ((Entry != NULL) && ((VOID*)(Entry->GetValueEntryPoint) != NULL)) {
		retval = Entry->GetValueEntryPoint(
				&(AccessorParams->FieldValue),
				AccessorParams->FieldSize
			);
	}
	return retval;
}
