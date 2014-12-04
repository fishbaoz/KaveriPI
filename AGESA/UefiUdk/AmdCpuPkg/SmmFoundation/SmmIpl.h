/* $NoKeywords:$ */
/**
 * @file
 *
 * Smm Ipl header file
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   SMM Access DXE Driver
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

#ifndef _SMM_IPL_H_
#define _SMM_IPL_H_

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeCoffLib.h>
#include <Library/TimerLib.h>
#include <Protocol/Mpservice.h>
#include <Porting.h>

#include "SmmFoundation.h"

#define PAGE_PRESENT             0x01
#define PAGE_READ_WRITE          0x02
#define PAGE_USER_SUPERVISOR     0x04
#define PAGE_WRITE_THROUGH       0x08
#define PAGE_CACHE_DISABLE       0x010
#define PAGE_ACCESSED            0x020
#define PAGE_DIRTY               0x040
#define PAGE_PAT                 0x080
#define PAGE_GLOBAL              0x0100
#define PAGE_2M_MBO              0x080
#define PAGE_2M_PAT              0x01000ul

#define LOCAL_APIC_ADDR         0xFEE00000ul
#define INT_CMD_REG_LO          0x300
#define INT_CMD_REG_HI          0x310
#define APIC_ID_REG             0x20
#define INIT          (5 << 8)
#define SIPI          (6 << 8)
#define LEVEL_ASSERT      (1 << 14)


/// Address patch structure
typedef struct {
  UINT8       *SmmEntryOffset;          /// SmmEntryOffset
  UINTN       PModeEntryOffset;         /// PModeEntryOffset
  UINTN       FlatJumpOffset;           /// FlatJumpOffset
  UINTN       LModeEntryOffset;         /// LModeEntryOffset
  UINTN       LongJumpOffset;           /// LongJumpOffset
  UINTN       GdtOffset;                /// GdtOffset
  UINTN       GdtBaseOffset;            /// GdtBaseOffset
  UINTN       Cr3;                      /// Cr3
  UINTN       Size;                     /// Size
  UINTN       SmmSpinLock;              /// SmmSpinLock
  UINTN       IdtOffset;                /// IdtOffset
  UINTN       IdtBaseOffset;            /// IdtBaseOffset
} SMM_ADDRESS_PATCH_MAP;


#pragma pack (push, 1)

/// Relocation information structure
typedef struct {
  volatile UINT32 NewSmmBase;           /// New base
  UINT32          TsegBase;             /// Tseg base
  UINT32          TsegSize;             /// Tseg size
} SMM_RELOC_INFO;

/// Relocation address patch map
typedef struct {
  UINT8 *SmmRelocEntryOffset;           /// Offset
  UINTN SmmRelocInfoOffset;             /// Offset
  UINTN Size;                           /// Size
} RELOC_ADDRESS_PATCH_MAP;

/// Page-Map Level-4 Offset (PML4) and
/// Page-Directory-Pointer Offset (PDPE) entries 4K & 2MB
typedef union {
  struct {                              ///< Bits
    UINT64  Present:1;                  ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;                ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;           ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;             ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;            ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;                 ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Reserved:1;                 ///< Reserved
    UINT64  MustBeZero:2;               ///< Must Be Zero
    UINT64  Available:3;                ///< Available for use by system software
    UINT64  PageTableBaseAddress:40;    ///< Page Table Base Address
    UINT64  AvabilableHigh:11;          ///< Available for use by system software
    UINT64  Nx:1;                       ///< No Execute bit
  } Bits;
  UINT64    Uint64;                     /// Qword
} PAGE_MAP_AND_DIRECTORY_POINTER;

/// Page Table Entry 2MB
typedef union {
  struct {                              ///< Bits
    UINT64  Present:1;                  ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;                ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;           ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;             ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;            ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;                 ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                    ///< 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  MustBe1:1;                  ///< Must be 1
    UINT64  Global:1;                   ///< 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;                ///< Available for use by system software
    UINT64  PAT:1;                      ///< PAT
    UINT64  MustBeZero:8;               ///< Must be zero;
    UINT64  PageTableBaseAddress:31;    ///< Page Table Base Address
    UINT64  AvabilableHigh:11;          ///< Available for use by system software
    UINT64  Nx:1;                       ///< 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;                     /// Qword
} PAGE_TABLE_ENTRY;

/// Page Table Entry 1GB
typedef union {
  struct {                              ///< Bits
    UINT64  Present:1;                  ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;                ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;           ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;             ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;            ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;                 ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                    ///< 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  MustBe1:1;                  ///< Must be 1
    UINT64  Global:1;                   ///< 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;                ///< Available for use by system software
    UINT64  PAT:1;                      ///< PAT
    UINT64  MustBeZero:17;              ///< Must be zero;
    UINT64  PageTableBaseAddress:22;    ///< Page Table Base Address
    UINT64  AvabilableHigh:11;          ///< Available for use by system software
    UINT64  Nx:1;                       ///< 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;                     /// Qword
} PAGE_TABLE_1G_ENTRY;

extern VOID   AsmGetSmmPatchAddressMap (
  SMM_ADDRESS_PATCH_MAP   *AddressMap
  );

extern VOID   AsmGetSmmRelocPatchMap (
  RELOC_ADDRESS_PATCH_MAP   *AddressMap
  );

extern UINTN  AsmGetSmm16CodeSize ();

#pragma pack (pop)
#endif //_SMM_IPL_H_

