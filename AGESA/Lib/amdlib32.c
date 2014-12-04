/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains interface to the AMD AGESA library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 48409 $   @e \$Date: 2011-03-08 11:19:40 -0600 (Tue, 08 Mar 2011) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright (c) 2008 - 2013, Advanced Micro Devices, Inc.
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "amdlib.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE LIB_AMDLIB_FILECODE

#if defined(__GNUC__)
/*---------------------------------------------------------------------------------------*/
/**
 *  FPU Clear Exceptions
 *
 *
 */

VOID
LibAmdFnclex (
  VOID)
{
	__asm__("fnclex");
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Read MXCSR register
 *
 * @param[in]  Value   Value to write
 */

VOID
LibAmdReadMxcsr (
     OUT   UINT32 *Value
  )
{
	__asm__("mov %[value], %%edi\n stmxcsr (%%edi)" :: [value] "irm" (Value) : "edi");
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Write MXCSR register
 *
 * @param[in]  Value   Value to write
 */

VOID
LibAmdWriteMxcsr (
  IN       UINT32 *Value
  )
{
	__asm__("mov %[value], %%edi\n ldmxcsr (%%edi)" :: [value] "irm" (Value) : "edi");
}
#endif

