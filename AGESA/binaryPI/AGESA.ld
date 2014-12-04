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

/**
 *	AGESA ROM Layout
 *	: image header (ImageHeader.o)
 *	: executable code (.text)
 *	: constructors (.ctors)
 *	: data (.rodata, .data)
 */

BASE = 0xFFE00000;
SIZE = 0x140000;

/**************************************************************************/

ENTRY(_start)

SECTIONS
{
	. = BASE;
	_rom = .;

	.header : {
		. = ALIGN(0x1000);
		agesa_signature = .;
		*ImageHeader.o( .rodata )
		*ImageHeader.o( .data )
	}

	.text : {
		. = ALIGN(0x400);
		*(.textfirst);
		*(.text);
		*(.ctors);
		. = ALIGN(0x400);
	}

	.rodata : {
		. = ALIGN(0x400);
		*(.bs_init)
		*(.rodata)
		*(.data)
		 . = ALIGN(0x400);
	}

	_rom_size = . - BASE;
	/*
	 * TODO:  Figure out a good way to protest if _rom_size exceeds SIZE
	 * TODO:  Figure out a way to protest if any global variables are used but not initialized (.bss, .sbss)
	 */

	/DISCARD/ : {
		*(.comment)
		*(.note)
		*(.bss)
		*(.sbss)
		*(COMMON)
	}
}
