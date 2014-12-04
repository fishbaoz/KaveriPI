/* $NoKeywords:$ */
/**
 * @file
 *
 * Amd Smm IPL code
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
#include "SmmIpl.h"
#include "AmdSmmAccess2.h"
#include "AmdSmmConfigure.h"
//#include "SmmIpl.h"

EFI_MP_SERVICES_PROTOCOL      *mMpService;
EFI_SMM_ACCESS2_PROTOCOL      *mSmmAccess;
LIST_ENTRY                    *mSmmInfoListHead = NULL;

// {5C337A89-C708-4b03-9485-C08BCA6FA8FA}
EFI_GUID SMM64EntryFileGuid =  { 0x5c337a89, 0xc708, 0x4b03, 0x94, 0x85, 0xc0, 0x8b, 0xca, 0x6f, 0xa8, 0xfa };


/*---------------------------------------------------------------------------------------*/
/**
 *  Find Total Cpu cores in the platform
 *
 *  This function finds total cpu cores in the system
 *
 *  @return  TotalCore
 *
**/
/*---------------------------------------------------------------------------------------*/
UINTN
GetTotalCpuCores ()
{
  IN       UINTN       NumberOfProcessors;
  IN       UINTN       NumberOfEnabledProcessors;
  IN       EFI_STATUS  Status;

  Status = mMpService->GetNumberOfProcessors (
                          mMpService,
                          &NumberOfProcessors,
                          &NumberOfEnabledProcessors
                          );
  if (EFI_ERROR (Status)) {
    return (0);
  }
  return (NumberOfEnabledProcessors);

}

/*---------------------------------------------------------------------------------------*/
/**
 *  Find ApicId of each cpu cores
 *
 *  This function return Apic id for corrosponding Cpu core.
 *
 *  @param[in] CoreId Cpu core number (zero base)
 *
 *  @return  corrosponding ApicId
 *
**/
/*---------------------------------------------------------------------------------------*/

UINT8
GetCoreApicId (
  IN       UINTN CoreId
  )
{
  IN       EFI_PROCESSOR_INFORMATION   ProcessorInfo;
  IN       EFI_STATUS                  Status;

  Status = mMpService->GetProcessorInfo (
                          mMpService,
                          CoreId,
                          &ProcessorInfo
                          );
  if (EFI_ERROR (Status)) {
    return (0);
  }

  return ((UINT8)ProcessorInfo.ProcessorId);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Send SMI to corrosponding cpu core using Smi IPL message via local APIC
 *
 *  This function trigger SMI to respective ApicId (0xff means to all cores)
 *
 *  @param[in]  ApicId Cpu core number (zero base)
 *
 *  @return  None
 *
**/
/*---------------------------------------------------------------------------------------*/
VOID
SendSmiIpl (
  IN       UINT8 ApicId
  )
{
  UINT32          val;
  volatile UINT32 *MemIo;

  // FMOo move to seperate library
  DEBUG ((DEBUG_INFO, "SendSmiIpl to ApicId %x\n", ApicId ));
  val = (UINT8) ApicId;

  MemIo = (volatile UINT32 *) (UINTN) (LOCAL_APIC_ADDR + INT_CMD_REG_HI);
  *MemIo &= 0x00ffffff;
  *MemIo |= (val << 24);

  MemIo = (volatile UINT32 *) (UINTN) (LOCAL_APIC_ADDR + INT_CMD_REG_LO);
  *MemIo = BIT9;

}

/*---------------------------------------------------------------------------------------*/
/**
 *  Get the size of Smm foundation code that need to be carved from Smm space
 *
 *  This function check platform config to set-aside Smm foundation code
 *
 *  @return  Total size of Smm foundation code
 *
**/
/*---------------------------------------------------------------------------------------*/
UINTN
GetSmmFoundationSize ()
{
   return (UINTN)PcdGet32 (PcdSmmFoundationTotalSize);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Get the size of SMM Stack for each core
 *
 *  This function return Stack size
 *
 *  @return  Total size of Smm stack
 *
**/
/*---------------------------------------------------------------------------------------*/
UINTN
GetSmmStackSize ()
{
  // Use PCD to find the stack size to set for each core
  return (UINTN)PcdGet32 (PcdSmmStackSize);
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Load Efi (exe) file in memory
 *
 *  This function is used to load SMM64 SMM foundation code in memory
 *
 *  @param[in]  File          : EFI guid of file in FV
 *  @param[in]  SectionType   : Section of the FFS that need to be extracted
 *  @param[in]  Memory        : Physical location of memory where this section need to be loaded
 *  @param[out] ImageSize     : the function will update this variable with the size of section
 *  @param[out] ExeEntryPoint : entry point (for EFI_SECTION_PE32)
 *
 *  @return  Update ImageSize and ExeEntryPoint and return success if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
LoadExeFileInMemory (
  IN       EFI_GUID                  *File,
  IN       EFI_SECTION_TYPE          SectionType,
  IN       EFI_PHYSICAL_ADDRESS      Memory,
     OUT   UINTN                     *ImageSize,
     OUT   EFI_IMAGE_ENTRY_POINT     *ExeEntryPoint
  )
{
  EFI_STATUS                    Status;
  VOID                          *SourceBuffer;
  PE_COFF_LOADER_IMAGE_CONTEXT  ImageContext;

  *ExeEntryPoint  = NULL;
  SourceBuffer = NULL;
  *ImageSize = 0;

  // Look for SMM foundation FFS FV
  Status = GetSectionFromAnyFv (File, SectionType, 0, (VOID **) &SourceBuffer, ImageSize);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  ImageContext.Handle    = SourceBuffer;
  ImageContext.ImageRead = PeCoffLoaderImageReadFromMemory;

  // Gather SMM foundation Coff image information
  Status = PeCoffLoaderGetImageInfo (&ImageContext);


  if (EFI_ERROR (Status)) {
    FreePool (SourceBuffer);
    return Status;
  }

  ImageContext.ImageAddress = Memory;
  ImageContext.ImageAddress += ImageContext.SectionAlignment - 1;
  ImageContext.ImageAddress &= ~(ImageContext.SectionAlignment - 1);

  // Load SMM foundation FFS in SMM space
  Status = PeCoffLoaderLoadImage (&ImageContext);
  if (!EFI_ERROR (Status)) {
    //
    // Relocate the image in our new buffer
    //
    Status = PeCoffLoaderRelocateImage (&ImageContext);
    if (!EFI_ERROR (Status)) {
      *ExeEntryPoint = (EFI_IMAGE_ENTRY_POINT)(UINTN)ImageContext.EntryPoint;
      //Status = (*ExeEntryPoint) (NULL, gST);
    }
  }

  FreePool (SourceBuffer);
  return Status;
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Locate the next available space in SMM space to copy the SMMEntry code for each core
 *
 *  This function scans the SMM space to find the suitable place to copy SMMentry code for each core
 *
 *  The algorith to find the next available SmmEntry is simple. SmmBase for the next one starts after
 *  the last one ends (0x200 aligned); except when the SmmBase overlap with other Smm's save area, in
 *  that case we will start from new region where the save state of previous SMM save area
 *  ends
 *
 *  @param  Smm16Base               : Start of Smm space where Smm16code can be copied
 *  @param  SmmEnd                  : End of Tseg area
 *  @param  Smm16Size               : Size of each SMM16 code
 *  @param  NextAvailableSmm16Entry : The function will update this param to reflect the next available
 *                                    SmmEnty address
 *
 *  @return  Update NextAvailableSmm16Entry and return success if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
FindNextAvailableSmm16EntryBase (
  IN       EFI_PHYSICAL_ADDRESS     Smm16Base,
  IN       EFI_PHYSICAL_ADDRESS     SmmEnd,
  IN       UINTN                    Smm16Size,
  IN OUT   EFI_PHYSICAL_ADDRESS     *NextAvailableSmm16Entry
  )
{
  // keep track of next SmmBase
  STATIC EFI_PHYSICAL_ADDRESS NextSmmBase;

  // keep track of SmmSaveArea
  STATIC EFI_PHYSICAL_ADDRESS SmmSaveAreaStart;
  STATIC EFI_PHYSICAL_ADDRESS SmmSaveAreaEnd;

  // The algorith to find the next available SmmEntry is simple. Next smmentry starts where the last
  // one ends (0x200 aligned); except when the Next Smm AP area step on the Smm Save area of any AP, in
  // that case we wiill start from new region which start at address of end of smm save state of
  // last AP
  if (*NextAvailableSmm16Entry == NULL) {
  //For very first time init the local static variable
    NextSmmBase = Smm16Base;
    SmmSaveAreaStart  = NextSmmBase + 0x7e00;
    SmmSaveAreaEnd    = SmmSaveAreaStart + 0x200;
  }

  // Check if Smmbase (& Smmsave area from 0x7e00-0x8000) can fit in Smm space
  if (NextSmmBase + 0x8000 > SmmEnd) {
    return (EFI_OUT_OF_RESOURCES);
  }

  //Align size to 0x200 boundry the size is allocated in 0x200 block size
  Smm16Size = ((Smm16Size + 0x1ff) / 0x200) * 0x200;

  //If this area does not step over SmmSave we are done
  if (NextSmmBase + Smm16Size < SmmSaveAreaStart) {
    *NextAvailableSmm16Entry = NextSmmBase;
  } else {
    //Else start from where last AP SMM Save area ends
    *NextAvailableSmm16Entry = SmmSaveAreaEnd;
  }

  //Update The SMM Save area pointers
  SmmSaveAreaStart  = *NextAvailableSmm16Entry + 0x7e00;
  SmmSaveAreaEnd    = *NextAvailableSmm16Entry + 0x8000;

  NextSmmBase = *NextAvailableSmm16Entry  + Smm16Size;

  return (EFI_SUCCESS);
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Allocates and fills in the Page Directory and Page Table Entries to
 *  establish a 1:1 Virtual to Physical mapping.
 *
 *  @param[in]  PageTablesAddress  The base address of page table.
 *
 *  @retval Offset to end of page table
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_PHYSICAL_ADDRESS
CreateIdentityMappingPageTables (
  IN       EFI_PHYSICAL_ADDRESS  PageTablesAddress
  )
{
  UINT32                                        RegEax;
  UINT32                                        RegEdx;
  UINT8                                         PhysicalAddressBits;
  EFI_PHYSICAL_ADDRESS                          PageAddress;
  UINTN                                         IndexOfPml4Entries;
  UINTN                                         IndexOfPdpEntries;
  UINTN                                         IndexOfPageDirectoryEntries;
  UINT32                                        NumberOfPml4EntriesNeeded;
  UINT32                                        NumberOfPdpEntriesNeeded;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageMapLevel4Entry;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageMap;
  PAGE_MAP_AND_DIRECTORY_POINTER                *PageDirectoryPointerEntry;
  PAGE_TABLE_ENTRY                              *PageDirectoryEntry;
  UINTN                                         TotalPagesNum;
  UINTN                                         BigPageAddress;
  VOID                                          *Hob;
  BOOLEAN                                       Page1GSupport;
  PAGE_TABLE_1G_ENTRY                           *PageDirectory1GEntry;

  // Always force a 1GB Page Table in X64 SMM otherwise page tables can get
  // very large.  At this time the processor supports 48 bits which makes
  // the 1GB page table 2MB + 64KB in size. 2MB page table would be > 256MB
  Page1GSupport = FALSE;
  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000001) {
    AsmCpuid (0x80000001, NULL, NULL, NULL, &RegEdx);
    if ((RegEdx & BIT26) != 0) {
      Page1GSupport = TRUE;
    }
  }
  if (!Page1GSupport) {
    DEBUG ((DEBUG_ERROR, "SmmFoundation driver requires 1 GB Page table support.\n"));
    ASSERT (FALSE);
  }

  //
  // Get physical address bits supported.
  //
  Hob = GetFirstHob (EFI_HOB_TYPE_CPU);
  if (Hob != NULL) {
    PhysicalAddressBits = ((EFI_HOB_CPU *) Hob)->SizeOfMemorySpace;
  } else {
    AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
    if (RegEax >= 0x80000008) {
      AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
      PhysicalAddressBits = (UINT8) RegEax;
    } else {
      PhysicalAddressBits = 36;
    }
  }

  //
  // IA-32e paging translates 48-bit linear addresses to 52-bit physical addresses.
  //
  ASSERT (PhysicalAddressBits <= 52);
  if (PhysicalAddressBits > 48) {
    PhysicalAddressBits = 48;
  }

  //
  // Calculate the table entries needed.
  //
  if (PhysicalAddressBits <= 39 ) {
    NumberOfPml4EntriesNeeded = 1;
    NumberOfPdpEntriesNeeded = (UINT32)LShiftU64 (1, (PhysicalAddressBits - 30));
  } else {
    NumberOfPml4EntriesNeeded = (UINT32)LShiftU64 (1, (PhysicalAddressBits - 39));
    NumberOfPdpEntriesNeeded = 512;
  }

  //
  // Pre-allocate big pages to avoid later allocations.
  //
  if (!Page1GSupport) {
    TotalPagesNum = (NumberOfPdpEntriesNeeded + 1) * NumberOfPml4EntriesNeeded + 1;
  } else {
    TotalPagesNum = NumberOfPml4EntriesNeeded + 1;
  }
  BigPageAddress = (UINTN) PageTablesAddress;

  //
  // By architecture only one PageMapLevel4 exists - so lets allocate storage for it.
  //
  PageMap         = (VOID *) BigPageAddress;
  BigPageAddress += SIZE_4KB;

  PageMapLevel4Entry = PageMap;
  PageAddress        = 0;
  for (IndexOfPml4Entries = 0; IndexOfPml4Entries < NumberOfPml4EntriesNeeded; IndexOfPml4Entries++, PageMapLevel4Entry++) {
    //
    // Each PML4 entry points to a page of Page Directory Pointer entires.
    // So lets allocate space for them and fill them in in the IndexOfPdpEntries loop.
    //
    PageDirectoryPointerEntry = (VOID *) BigPageAddress;
    BigPageAddress += SIZE_4KB;

    //
    // Make a PML4 Entry
    //
    PageMapLevel4Entry->Uint64 = (UINT64) (UINTN)PageDirectoryPointerEntry;
    PageMapLevel4Entry->Bits.ReadWrite = 1;
    PageMapLevel4Entry->Bits.Present = 1;

    if (Page1GSupport) {
      PageDirectory1GEntry = (VOID *) PageDirectoryPointerEntry;

      for (IndexOfPageDirectoryEntries = 0; IndexOfPageDirectoryEntries < 512; IndexOfPageDirectoryEntries++, PageDirectory1GEntry++, PageAddress += SIZE_1GB) {
        //
        // Fill in the Page Directory entries
        //
        PageDirectory1GEntry->Uint64 = (UINT64)PageAddress;
        PageDirectory1GEntry->Bits.ReadWrite = 1;
        PageDirectory1GEntry->Bits.Present = 1;
        PageDirectory1GEntry->Bits.MustBe1 = 1;
      }
    } else {
      for (IndexOfPdpEntries = 0; IndexOfPdpEntries < NumberOfPdpEntriesNeeded; IndexOfPdpEntries++, PageDirectoryPointerEntry++) {
        //
        // Each Directory Pointer entries points to a page of Page Directory entires.
        // So allocate space for them and fill them in in the IndexOfPageDirectoryEntries loop.
        //
        PageDirectoryEntry = (VOID *) BigPageAddress;
        BigPageAddress += SIZE_4KB;

        //
        // Fill in a Page Directory Pointer Entries
        //
        PageDirectoryPointerEntry->Uint64 = (UINT64) (UINTN)PageDirectoryEntry;
        PageDirectoryPointerEntry->Bits.ReadWrite = 1;
        PageDirectoryPointerEntry->Bits.Present = 1;

        for (IndexOfPageDirectoryEntries = 0; IndexOfPageDirectoryEntries < 512; IndexOfPageDirectoryEntries++, PageDirectoryEntry++, PageAddress += SIZE_2MB) {
          //
          // Fill in the Page Directory entries
          //
          PageDirectoryEntry->Uint64 = (UINT64)PageAddress;
          PageDirectoryEntry->Bits.ReadWrite = 1;
          PageDirectoryEntry->Bits.Present = 1;
          PageDirectoryEntry->Bits.MustBe1 = 1;
        }
      }

      for (; IndexOfPdpEntries < 512; IndexOfPdpEntries++, PageDirectoryPointerEntry++) {
        ZeroMem (
          PageDirectoryPointerEntry,
          sizeof (PAGE_MAP_AND_DIRECTORY_POINTER)
          );
      }
    }
  }

  //
  // For the PML4 entries we are not using fill in a null entry.
  //
  for (; IndexOfPml4Entries < 512; IndexOfPml4Entries++, PageMapLevel4Entry++) {
    ZeroMem (
      PageMapLevel4Entry,
      sizeof (PAGE_MAP_AND_DIRECTORY_POINTER)
      );
  }

  return ((EFI_PHYSICAL_ADDRESS)BigPageAddress);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Update the SMM core entry point in SMM foundation code so that for all future SMM
 *  the foundation code will handoff to SMM core
 *
 *  @param[in]  SmmCoreEntry    : Address in SMM core kernal
 *
 *  @retval EFI_SUCCSS if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
RegisterSmmCoreEntry (
  IN       UINTN  SmmCoreEntry
  )
{
  EFI_STATUS            Status;
  SMM_FOUNDATION_TABLE  *SmmInfo;
  LIST_ENTRY            *Link;

  // First open the Smm space
  Status = mSmmAccess->Open (mSmmAccess);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  // Parse to SMM_INFO structure of all SMM cores
  Link = mSmmInfoListHead;

  do {
      //And update the SmmCore entry in the structure
    SmmInfo = SMM_FOUNDATION_FROM_LINK (Link);
    SmmInfo->UefiSmmCoreBase = SmmCoreEntry;

    Link = GetNextNode (mSmmInfoListHead, Link);

  } while (Link != mSmmInfoListHead);

  // Finally close the Smm space
  Status = mSmmAccess->Close (mSmmAccess);
  return Status;

}

/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare Smm foundation (SMM16 entry code)
 *
 *  This function copied the SMM16 bit code to SMM space and update the SMM_FOUNDATON_TABLE
 *  structure that SMM foundation code will use on SMM entry
 *
 *  @param[in]  Smm16Entry    : Address in SMM space where SMM code is copied
 *  @param[in]  PageTableBase : Address in PageTable that SMM foundation code will use when
 *                              switching to 64 bit mode
 *  @param[in]  SmmEntryInfo  : SmmEntry stucuture for each core/Smm entry that it will use
 *                              for execution
 *
 *  @retval EFI_SUCCSS if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
PrepareSmmStartupVector (
  IN       EFI_PHYSICAL_ADDRESS     Smm16Entry,
  IN       EFI_PHYSICAL_ADDRESS     PageTableBase,
  IN       SMM_FOUNDATION_TABLE     *SmmEntryInfo
  )
{
  SMM_ADDRESS_PATCH_MAP       AddressMap;
  EFI_STATUS                  Status = EFI_SUCCESS;
  SMM_FOUNDATION_TABLE        *Info;
  STATIC SPIN_LOCK            *SmmSpinLock = NULL;

  AsmGetSmmPatchAddressMap  (&AddressMap);

  //
  // Copy AP startup code to startup vector, and then redirect the long jump
  // instructions for mode switching.
  //
  CopyMem ((VOID *) (UINTN) Smm16Entry, AddressMap.SmmEntryOffset, AddressMap.Size);

  // Some patchup is requred in SmmEntry code to adjust for this relocation operation (i.e. copy from DXE
  // driver to SMM space). Look for SMM_ADDRESS_PATCH_MAP for those patch offsets

  // Perform patchup on SmmEntry based on the relocation
  *(UINT32 *) (UINTN) (Smm16Entry + 2) = (UINT32) (Smm16Entry);

  *(UINT32 *) (UINTN) (Smm16Entry + AddressMap.FlatJumpOffset + 3) = (UINT32) (Smm16Entry + AddressMap.PModeEntryOffset);
  //
  // For IA32 mode, LongJumpOffset is filled with zero. If non-zero, then we are in
  // X64 mode, so further redirect for long mode switch.
  //
  if (AddressMap.LongJumpOffset != 0) {
    *(UINT32 *) (UINTN) (Smm16Entry + AddressMap.LongJumpOffset + 2) = (UINT32) (Smm16Entry + AddressMap.LModeEntryOffset);
  }

  // Fix the GDT base
  *(UINT32 *) (UINTN) (Smm16Entry + AddressMap.GdtOffset + 2)  = (UINT32) (Smm16Entry + AddressMap.GdtBaseOffset);

  *(UINT32 *) (UINTN) (Smm16Entry + AddressMap.IdtOffset + 2)  = (UINT32) (Smm16Entry + AddressMap.IdtBaseOffset);
  *(UINT32 *) (UINTN) (Smm16Entry + AddressMap.Cr3 + 1)  = (UINT32)PageTableBase;


  if (!SmmSpinLock) {
    SmmSpinLock =  (SPIN_LOCK *) (UINTN) (Smm16Entry + AddressMap.SmmSpinLock);
    InitializeSpinLock (SmmSpinLock);
  }
  SmmEntryInfo->SmmSpinLock = SmmSpinLock;

  //
  // Update the SmmInfo structure of each core
  //
  Info = (SMM_FOUNDATION_TABLE *) (UINTN) (Smm16Entry + AddressMap.Size);
  CopyMem (Info, SmmEntryInfo, sizeof (SMM_FOUNDATION_TABLE));

  Info->CoreSmmBase = (UINT32) Smm16Entry;

  //Add this to link list

  if (mSmmInfoListHead == NULL) {
    mSmmInfoListHead = &Info->Link;
    InitializeListHead (mSmmInfoListHead);
  } else {
    // Create a link list so that each core can parse to other cores Smm info
    InsertTailList (mSmmInfoListHead, &Info->Link);
  }

  return (Status);
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Prepare Smm relocate vector
 *
 *  This function copied the AP SMM16 bit code to SMM space
 *
 *  @param[in]  SmmRelocBase    Relocation base
 *  @param[out] RelocInfo       Pointer to information.
 *
 *  @retval EFI_SUCCSS if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
PrepareSmmRelocateVector (
  IN       EFI_PHYSICAL_ADDRESS   SmmRelocBase,
     OUT   SMM_RELOC_INFO         **RelocInfo
  )
{
  RELOC_ADDRESS_PATCH_MAP       AddressMap;

  AsmGetSmmRelocPatchMap (&AddressMap);

  //
  // Copy AP startup code to startup vector, and then redirect the long jump
  // instructions for mode switching.
  //
  CopyMem ((VOID *) (UINTN) SmmRelocBase, AddressMap.SmmRelocEntryOffset, AddressMap.Size);

  // Get the location of SmmReloc info in the AP space
  *RelocInfo = (SMM_RELOC_INFO *) (UINTN) (SmmRelocBase + AddressMap.SmmRelocInfoOffset);

  return (EFI_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Relocate SMMbase of each core from default address (0x38000) to Tseg location
 *
 *  This function copied the smm relocation code to 0x38000 i.e. default SMM base and trigger
 *  SMI for each core to run the relocation code. The relocation code will update the MSR and
 *  change the SMM base in the SMM save area to new address
 *
 *  @param[in]  Core          : Logical CPU core id
 *  @param[in]  NewSmmBase    : The new SMM base after the relocation
 *  @param[in]  TsegBase      : Start of SMM area
 *  @param[in]  TsegSize      : Total size of SMM area
 *
 *  @retval EFI_SUCCSS if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
RelocateSmmBase (
  IN       UINTN                      Core,
  IN       EFI_PHYSICAL_ADDRESS       NewSmmBase,
  IN       EFI_PHYSICAL_ADDRESS       TsegBase,
  IN       UINTN                      TsegSize
  )
{
  EFI_PHYSICAL_ADDRESS    SmmRelocBase;
  SMM_RELOC_INFO          *RelocInfo;
  EFI_STATUS              Status;
  UINTN                   Timeout;

  Timeout = 0;
  //Allocate memory to relocate SMM base
  SmmRelocBase = SMM_DEFAULT_BASE;
  Status = gBS->AllocatePages (
                  AllocateAddress,
                  EfiBootServicesData,
                  1,
                  &SmmRelocBase
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy SMM relocate code in this allocated memory. This is Smm16Relocate.asm copied to SmmRelocBase
  Status = PrepareSmmRelocateVector (
              SmmRelocBase,
              &RelocInfo
              );


  RelocInfo->NewSmmBase  = (UINT32) NewSmmBase;
  RelocInfo->TsegBase    = (UINT32) TsegBase;
  RelocInfo->TsegSize    = (UINT32) TsegSize;

  // Send SIPI to relocate SMI. Smm16Relocate.asm code will get SMM control, perform necessary SMM relocation
  // and set the flag to indicate completion
  SendSmiIpl (GetCoreApicId (Core));
  while (RelocInfo->NewSmmBase) {
  // Wait for AP to run and clear this value to indicate compition
    MicroSecondDelay (1);
    Timeout++;
    if (Timeout > 10000) {
    // If > 10 sec. bailout. Enough??
      Status = EFI_UNSUPPORTED;
      break;
    }
  }

  // Release the memory
  gBS->FreePages (SmmRelocBase, 1);

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Initialize the SMM foundation code
 *
 *  This function is the higher level function that calls other lower level routines to
 *  build the page table in SMM space, copy the SMM foundation code in SMM space and
 *  relocate each core SMM base to Tseg area where SMM founction code is copied
 *
 *
 *  @param[in]  SmmAccess         : Handle for SmmAccess protocol to open/close SMM area
 *  @param[in]  TsegBase          : Start of SMM area
 *  @param[in]  TsegSize          : Total size of SMM area
 *  @param[in]  SmmFoundationBase : Location in SMM space where Smm Foundation code + data will be copied
 *
 *  @retval EFI_SUCCSS if all goes well
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS InitSmmFoundationCode (
  IN       EFI_SMM_ACCESS2_PROTOCOL   *SmmAccess,
  IN       UINTN                      TsegBase,
  IN       UINTN                      TsegSize,
  IN       UINTN                      SmmFoundationBase
  )
{

  EFI_STATUS                    Status;
  UINTN                         ImageSize;
  EFI_IMAGE_ENTRY_POINT         Smm64Entry;
  UINTN                         i;
  EFI_PHYSICAL_ADDRESS          Smm16Entry;
  EFI_PHYSICAL_ADDRESS          Smm16Size;
  EFI_PHYSICAL_ADDRESS          Smm16Start;
  UINT8                         CoreCount;
  SMM_FOUNDATION_TABLE          SmmInfo;
  EFI_PHYSICAL_ADDRESS          PageTableEnd;
  UINTN                         SmmStackSize;

  //PageTable+Smm64Entry+Smm16Entry are placed towards the end of Tseg. Carve the reserve space at the end

  // find the total no. of AP's in the system
  CoreCount = (UINT8)GetTotalCpuCores ();
  if (!CoreCount) {
    return (EFI_NOT_FOUND);
  }

  DEBUG ((DEBUG_INFO, "InitSmmFoundationCode:: TsegBase %08x TsegSize %08x SmmFoundationBase %08x\n", TsegBase, TsegSize, SmmFoundationBase));


  // First create PageTable at SmmFoundationBase. Needed for 16->64 bit CPU mode
  PageTableEnd = CreateIdentityMappingPageTables (SmmFoundationBase);
  DEBUG ((DEBUG_INFO, "Loaded PageTable at %016x\n", SmmFoundationBase));
  DEBUG ((DEBUG_INFO, "PageTable ends at %016x\n", PageTableEnd));
  // Make sure Page Table doesn't overrun size allotted for it.  Leave 1MB for
  // rest of SmmFoundation code.
  if (PageTableEnd > (EFI_PHYSICAL_ADDRESS) (TsegBase + TsegSize - 0x100000)) {
    DEBUG ((DEBUG_ERROR, "Page Table overflowed allotted space\n"));
    ASSERT (FALSE);
  }
  // Next load Smm64 entry point after PageTable ends
  Status = LoadExeFileInMemory (&SMM64EntryFileGuid ,
                  EFI_SECTION_PE32,
                  (EFI_PHYSICAL_ADDRESS)PageTableEnd,
                  &ImageSize,
                 &Smm64Entry);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Loaded Smm64Foundation at %08x\n", Smm64Entry));

  // PageTable and Smm64 foundation code is in Tseg. This is shared by all core. Next build Smm16Entry
  // for each core
  Smm16Size = AsmGetSmm16CodeSize ();
  SmmStackSize = GetSmmStackSize ();

  // Smm16Start after PageTable, Smm64 code. Align to 0x200
  Smm16Start = ((PageTableEnd + ImageSize + 0x1ff) / 0x200) * 0x200,

  ZeroMem (&SmmInfo, sizeof (SmmInfo));

  // Indicate no SMM Entry has been identified yet, and need to initialize everything.
  Smm16Entry = NULL;

  // Relocate all the CPU cores
  for (i = 0; i < CoreCount; i++) {

    // Open the SMM area to build SMM PageTable and copy Smm Foundation code
    // After CPU rsm the SMM area may be closed so make sure to enable again
    Status = SmmAccess->Open (mSmmAccess);

    // Find the next available SMM base for Smm16Entry code for each core and update Smm16Entry variable
    // Smm16 code for each core is after Smm64 foundation code
    Status = FindNextAvailableSmm16EntryBase (
                    Smm16Start,
                    (EFI_PHYSICAL_ADDRESS) TsegBase + TsegSize,
                    Smm16Size,
                    &Smm16Entry
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }


    // Update SmmInfo foundation table
    SmmInfo.StackBase             = TsegBase + TsegSize - (i * SmmStackSize);
    SmmInfo.StackSize             = SmmStackSize;
    SmmInfo.UefiSmmCoreBase       = 0;
    SmmInfo.TotalCores            = CoreCount;
    SmmInfo.CoreSmmBase           = Smm16Entry;
    SmmInfo.Smm64Entry            = Smm64Entry;
    SmmInfo.CpuNumber             = (UINT8) i;
    SmmInfo.ApicId                = GetCoreApicId (SmmInfo.CpuNumber);


    // Check Smm code and Stack are not overlapping
    if (SmmInfo.CoreSmmBase + Smm16Size > SmmInfo.StackBase - SmmStackSize) {
      return EFI_OUT_OF_RESOURCES;
    }

    DEBUG ((DEBUG_INFO, "Core %x Smm16Foundation at %08x Stack at %08x \n", i, SmmInfo.CoreSmmBase, SmmInfo.StackBase));
    // Prepare Smm16 Startup code at Tseg ie. update the SmmInfo structure in Smm16Entry location of each core
    Status = PrepareSmmStartupVector (
                Smm16Entry,
                SmmFoundationBase,
                &SmmInfo
                );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Send SIPI to each core to set new SmmBase at Tseg (default 0x38000)
    Status = RelocateSmmBase (
                i,
                Smm16Entry - 0x8000,              // newSmmBase
                TsegBase,                         // TsegBase
                (UINT32)TsegSize                  // TsegSize
                );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    SmmAccess->Close (mSmmAccess);
  }


  // Finally send Sipi to all core to test if responding. Add correction if necessary - FMO
  SendSmiIpl (0xff);

  DEBUG ((DEBUG_INFO, "Unused SmmFoundation space %08x \n", (SmmInfo.StackBase - SmmStackSize) - (SmmInfo.CoreSmmBase + Smm16Size)));

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Entrypoint of SmmIpl driver.
 *
 *  @param  ImageHandle
 *  @param  SystemTable
 *
 *  @return EFI_SUCCESS
 *  @return EFI_LOAD_ERROR
 *  @return EFI_OUT_OF_RESOURCES
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
SmmIplEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_SMRAM_DESCRIPTOR      *SmramMap = NULL;
  UINT64                    SmmReservedSize;
  UINTN                     Size;

  // Install instance of SmmAccess2 protocol
  InstallSmmAccess2 (ImageHandle, SystemTable);

  // Locate SmmAccess2 protocol
  Status = gBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **)&mSmmAccess);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Locate SmmAccess2 protocol
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mMpService);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Size = 0;
  Status = mSmmAccess->GetCapabilities (mSmmAccess, &Size, NULL);
  if (EFI_ERROR (Status) && (Status != EFI_BUFFER_TOO_SMALL)) {
    return Status;
  }

  SmramMap = (EFI_SMRAM_DESCRIPTOR *)AllocatePool (Size);
  if (SmramMap == NULL) {
    return Status;
  }

  // Find the size and start of Smm space in Tseg
  Status = mSmmAccess->GetCapabilities (mSmmAccess, &Size, SmramMap);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Get total SMM size
  SmmReservedSize = GetSmmFoundationSize ();
  if (!SmmReservedSize) {
    return EFI_NOT_FOUND;
  }

  // Open the SMM area to build SMM PageTable and copy Smm Foundation code
  Status = mSmmAccess->Open (mSmmAccess);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InitSmmFoundationCode (
                mSmmAccess,
                SmramMap->PhysicalStart,
                SmramMap->PhysicalSize,
                SmramMap->PhysicalStart + SmramMap->PhysicalSize - SmmReservedSize
                );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // All well. Now install SmmConfig protocol
  InstallSmmConfiguration (
    SmramMap->PhysicalStart + SmramMap->PhysicalSize - SmmReservedSize,
    SmmReservedSize,
    RegisterSmmCoreEntry
    );

  FreePool (SmramMap);

  mSmmAccess->Close (mSmmAccess);

  return Status;
}

//FMO patch IDT of SMM code
