;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
;  Workfile: ueficar.inc    $Revision:: 281181  $    $Date:: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
;
; Description: Code to setup and break down UEFI cache-as-stack and heap and
; execution cache map.
;
;*****************************************************************************
;
; Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software (the Materials)
; pursuant to the terms and conditions of your Software License Agreement
; with AMD.  This header does *NOT* give you permission to use the Materials
; or any rights under AMD's intellectual property.  Your use of any portion
; of these Materials shall constitute your acceptance of those terms and
; conditions.  If you do not agree to the terms and conditions of the Software
; License Agreement, please do not use any portion of these Materials.
;
; CONFIDENTIALITY:  The Materials and all other information, identified as
; confidential and provided to you by AMD shall be kept confidential in
; accordance with the terms and conditions of the Software License Agreement.
;
; LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
; PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
; WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
; MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
; OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
; IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
; (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
; INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
; GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
; RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
; THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
; EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
; THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
;
; AMD does not assume any responsibility for any errors which may appear in
; the Materials or any other related information provided to you by AMD, or
; result from use of the Materials or any related information.
;
; You agree that you will not reverse engineer or decompile the Materials.
;
; NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
; further information, software, technical information, know-how, or show-how
; available to you.  Additionally, AMD retains the right to modify the
; Materials at any time, without notice, and is not obligated to provide such
; modified Materials to you.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgement of AMD's proprietary rights in them.
;
; EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
; direct product thereof will be exported directly or indirectly, into any
; country prohibited by the United States Export Administration Act and the
; regulations thereunder, without the required authorization from the U.S.
; government nor will be used for any purpose prohibited by the same.
;*****************************************************************************

    .XLIST
    INCLUDE cpcarmac.inc
    .LIST
    .586P
    .mmx

;======================================================================
;   Reference: UEFI PI v1.2 definition:
;
;   typedef struct _UEFI_SEC_PEI_HAND_OFF {
;       UINT16  DataSize;
;       VOID    *BootFirmwareVolumeBase;
;       UINTN   BootFirmwareVolumeSize;
;       VOID    *TemporaryRamBase;
;       UINTN   TemporaryRamSize;
;       VOID    *PeiTemporaryRamBase;
;       UINTN   PeiTemporaryRamSize;
;       VOID    *StackBase;
;       UINTN   StackSize;
;   } UEFI_SEC_PEI_HAND_OFF;
;
UEFI_SEC_PEI_HAND_OFF STRUC 4
        DATA_SIZE                   WORD    ?
        BOOT_FIRMWARE_VOLUME_BASE   DWORD   ?
        BOOT_FIRMWARE_VOLUME_SIZE   DWORD   ?
        TEMPORARY_RAM_BASE          DWORD   ?
        TEMPORARY_RAM_SIZE          DWORD   ?
        PEI_TEMPORARY_RAM_BASE      DWORD   ?
        PEI_TEMPORARY_RAM_SIZE      DWORD   ?
        STACK_BASE                  DWORD   ?
        STACK_SIZE                  DWORD   ?
UEFI_SEC_PEI_HAND_OFF ENDS

; Assure build option is defined, default is BIST only storage
IFNDEF AMD_CAR_STACK_FRAME_PAD
    AMD_CAR_STACK_FRAME_PAD     EQU     0
ENDIF

;======================================================================
;======================================================================
; AMD_ENABLE_UEFI_STACK:  Setup a stack, heap & UEFI stack frame
;
;   Input condition requirements:
;       32bit protected 'flat addressing' mode
;       SS, DS, ES = segment descriptor defining 0x00000000 as the base.
;
;   Build time options:
;       AMD_CAR_STACK_FRAME_PAD EQU 00h
;              used to create a Host Env stack frame for pseudo
;              global variables - a build time option. Incremented
;              by 4 to cover the BIST storage.
;
;   Input Parameter:
;       STACK_AT_TOP
;              Indicate stack is on the top of cache as RAM.
;       STACK_AT_BOTTOM (default)
;              Indicate stack is at the bottom of cache as RAM.
;
;   In:
;       EAX  = BIST value collected after reset by host env
;       EBX  = Return address (preserved)
;       ECX  = size, in bytes, of the region to cache for execution.
;
;   Out:
;       SS:ESP - Our new private stack location
;
;       EAX = AGESA_STATUS
;       EDX = Return status code if EAX contains a return code of higher
;             severity than AGESA_SUCCESS
;       ECX = Stack size in bytes
;       EDI  = pointer to stack frame location. Points to the
;               beginning of the UEFI structure defined by the
;               PI v1.2 spec. The Host Env stack frame follows
;               this structure.
;               [EDI]UEFI_SEC_PEI_HAND_OFF.BOOT_FIRMWARE_VOLUME_BASE = OEM_BFV_BASE
;               [EDI]UEFI_SEC_PEI_HAND_OFF.BOOT_FIRMWARE_VOLUME_SIZE = OEM_BFV_SIZE
;               [EDI+sizeof(UEFI_SEC_PEI_HAND_OFF)].OEM_DATA_DWORD[0] = BIST
;
;   Preserved:
;       EBX, EBP, DS, ES, SS
;
;   Destroyed:
;       EAX, ECX, EDX, EDI, ESI, ESP
;       MMX0, MMX1, MMX2, MMX3, MMX4, MMX5  ... MMX[0..7] are used as save/restore storage
;
;   Known Limitations:
;       *!* This routine presently is limited to a max of 64 processor cores
;
;   Description:
;       This procedure will do the following:
;       - allocate pre-defined address space for use as a stack for C code
;       - allocate pre-defined address space for use as a UEFI heap
;       - enable execution cache for a specified region
;       - create an instance of the UEFI structure UEFI_SEC_PEI_HAND_OFF on the
;           stack and populate it with values.
;
;     Stack Allocation:
;       Note: At present, the stack allocation is the same as described in cpCarMac.inc
;           In fact, this macro uses that macro to perform the allocation.
;           The same 64 core limit applies to this implementation.
;       Future versions of this macro will expand support to 80+ cores.
;           Stack allocation will be 64k for the BSP, 16K for all APs.
;       ESP is set to point below the HostEnv stack frame.
;
;     Heap Allocation:
;       Note: At present, only the BSP will be allocated space for a UEFI heap.
;       Future versions of this macro will allcate 48K for each AP and the
;           allocation for the BSP will vary for the size of the L2 present and
;           the number of cores sharing the L2; maximizing the BSP allocation.
;
;     Execution cache:
;       The code will use Variable MTRRs 6 and 7 to define an area of memory
;       enabled for execution cache. This is presumed to include the PEI core
;       code area. The allocation is presummed to be at top-of-4G address range
;       so the smaller block, if one exists, will be allocated at the base
;       parameter (edx) and the larger block just after (edx+sizeof(smaller block))
;
;      HostEnv UEFI stack frame:
;       The code will create a stack data frame containing:
;       * a Host Env specific area for pseudo global variables.
;           o This area is at 'bottom (defalult)' so as to be retained if the PEI core decides
;               to reset the stack.
;           o The first entry in this area is the BIST value.
;       * an SEC-PEI hand-off structure (PI v1.2)
;           o populated with the stack and Temporary RAM entries.
;           o A processor's stack and heap are contiguous, with stack on 'top'.
;
;======================================================================
AMD_ENABLE_UEFI_STACK MACRO StackPosition
    ;local   AmdEnableUefiStackAbort    ; not needed IFF macro (correctly) only used once

    movd    mm1, ebp                    ; Save user requested register
    movd    mm0, ebx                    ; Logically 'push' the input parameters ( return address )
    movd    mm2, eax                    ;   ( BIST )
    movd    mm3, ecx                    ;   ( cache zone size )

    ; Short term method - need to accomodate existance of UEFI heap AND the AGESA heap.
    ;   So, use the old stack allocation process for stack, then mimick current UEFI (~v0.9)
    ;   operation to fill in the data stack frame.
    IFNB <StackPosition>
        AMD_ENABLE_STACK StackPosition
    ELSE
        AMD_ENABLE_STACK STACK_AT_BOTTOM
    ENDIF
    cmp     eax, AGESA_SUCCESS          ; Abort if not first entry; future versions will not allow multi-entry
    jne     AmdEnableUefiStackAbort

    ; review:
    ;       EAX = AGESA_STATUS
    ;       EDX = Return status code if EAX contains a return code of higher
    ;             severity than AGESA_SUCCESS
    ;       ECX = Stack size in bytes
    ;       ebx - return address parameter
    ;       ebp - user preserved register
    ;       ss:esp - stack pointer
    ;
    ;       esi -  address of start of stack block
    ;       [esp] - stack base address
    ;       [esp+4] - size of stack
    ;       [esp+8] - Marker for top-of-stack
    ;       mm0 - user return address
    ;       mm1 - user saved EBP register content
    ;       mm2 - BIST value
    ;       mm3 - cache zone size
    ;       mm5 - 32b pointer to family info struc. Set by GET_NODE_ID_CORE_ID_Fxx macros

    ; calculate stack frame pointer
    mov     ebp, [esp]
    mov     edx, ebp                            ; save stack base to edx

    ; for BSC, we divide the CAR zone in half and allocate 1/2 to each of stack & UEFI heap
    ; for APs, we allocate whole CAR to stack
    IS_BSC
    .if (carry?)
        IFNB <StackPosition>
            IF (StackPosition EQ STACK_AT_BOTTOM)
                shr ecx, 1
                add ebp, ecx
                shl ecx, 1
            ELSE
                add ebp, ecx
            ENDIF
        ELSE
            shr ecx, 1
            add ebp, ecx
            shl ecx, 1
        ENDIF
    .else
        add ebp, ecx
    .endif
    sub     ebp, (4 + AMD_CAR_STACK_FRAME_PAD)  ; space for BIST and additional OEM data
    movd    eax, mm2                            ; retrieve BIST data OEM acquired after reset
    mov     [ebp], eax                          ; place BIST data into first OEM data DWORD
    sub     ebp, SIZEOF UEFI_SEC_PEI_HAND_OFF   ; space for UEFI structure storage
    mov     eax, edx                            ; retrieve CAR base address for passing on
    mov     esp, ebp                            ; now can update the esp
    ; fill the UEFI stack frame
    mov     [ebp].UEFI_SEC_PEI_HAND_OFF.TEMPORARY_RAM_BASE, eax
    mov     [ebp].UEFI_SEC_PEI_HAND_OFF.TEMPORARY_RAM_SIZE, ecx
    mov     [ebp].UEFI_SEC_PEI_HAND_OFF.DATA_SIZE, SIZEOF UEFI_SEC_PEI_HAND_OFF

    ; for BSC, we divide the CAR zone in half and allocate 1/2 to each of stack & UEFI heap
    IS_BSC
    .if (carry?)
        push    ecx
        shr     ecx, 1                          ; divide the CAR zone in half and allocate 1/2 to each of stack & UEFI heap
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_SIZE, ecx
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_SIZE, ecx

        IFNB <StackPosition>
            IF (StackPosition EQ STACK_AT_BOTTOM)
                mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_BASE, eax
                add     eax, ecx                ; put PEI temporary RAM base in upper half
                mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_BASE, eax
            ELSE
                mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_BASE, eax
                add     eax, ecx                ; put stack base in upper half
                mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_BASE, eax
            ENDIF
        ELSE
            mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_BASE, eax
            add     eax, ecx                    ; put PEI temporary RAM base in upper half
            mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_BASE, eax
        ENDIF
        pop     ecx
    .else
    ; for APs, we allocate whole CAR to stack
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_BASE, 0
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.PEI_TEMPORARY_RAM_SIZE, 0
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_SIZE, ecx
        mov     [ebp].UEFI_SEC_PEI_HAND_OFF.STACK_BASE, eax
    .endif

    ; we will use the cache zone as implied BFV,
    ; The OEM is free to override this from their code that follows
    movd    eax, mm3                            ; cache zone size
    mov     [ebp].UEFI_SEC_PEI_HAND_OFF.BOOT_FIRMWARE_VOLUME_SIZE, eax

    ; calculate the base from size
    xor     ebx, ebx                            ; cache zone base
    sub     ebx, eax
    mov     [ebp].UEFI_SEC_PEI_HAND_OFF.BOOT_FIRMWARE_VOLUME_BASE, ebx
    movd    mm4, ebx

    ; Round up the size if there are more than 2 bits set in the given cache zone size
    push    edx
    push    ecx
    push    eax

    bsr     ecx, eax
    .if (!ZERO?)
        btr     eax, ecx                        ; there is one bit set in the given cache zone size
        bsr     ecx, eax
        .if (!ZERO?)
            push    ecx
            btr     eax, ecx                    ; there are two bits set in the given cache zone size
            bsr     ecx, eax
            .if (!ZERO?)
                pop     ecx                     ; ecx is the index of second bit set from most-significant
                pop     eax                     ; eax is the given cache zone size

                xor     edx, edx
                bts     edx, ecx
                add     eax, edx                ; round up the size
                dec     edx
                bts     edx, ecx                ; former 2nd bit spot should now be =0, clear it also
                not     edx
                and     eax, edx                ; now, eax has two bits set at most, could have only one

                push    eax
            .else
                pop     ecx                     ; balance the stack
            .endif
        .endif
    .endif

    pop     eax
    pop     ecx
    pop     edx
    movd    mm3, eax                            ; update cache zone size

    ; Check for and apply any family size limits.
    movd    edi, mm5
    mov     bx, [edi].CPU_FAMILY_INFO.L2_ALLOC_EXE

    .if (bx > 0)                                ; if there is a family limit
        ; CPUID will destroyed EAX, EBX, ECX, EDX
        ; but we only want to preserve EAX, ECX, EDX
        push    eax
        push    ecx
        push    edx

        ; get L2 allocate execution cache = CPU_FAMILY_INFO.L2_ALLOC_EXE + (L2 cache size - CPU_FAMILY_INFO.L2_MIN_SIZE)
        AMD_CPUID   AMD_CPUID_L2Cache
        shr     ecx, 16                         ; CX = L2 cache size
        sub     cx, [edi].CPU_FAMILY_INFO.L2_MIN_SIZE  ; CX = additional L2 size to the family limit
        mov     bx, [edi].CPU_FAMILY_INFO.L2_ALLOC_EXE ; use the additional L2 for exe cache
        add     bx,  cx

        ; restore EAX, ECX, EDX
        pop     edx
        pop     ecx
        pop     eax

        movzx   ebx, bx                         ;   convert the limit from K to Bytes
        shl     ebx, 10
        .if     (eax > ebx)                     ;   enforce the family limit
            ; note: SEC-PEI data is NOT updated on purpose, to allow the PEI
            ;   to see the full intended zone as the BFV


            mov     eax, ebx                    ;   set size to family limit
            movd    mm3, eax                    ;   update cache zone size
        .endif
    .endif

    ; base = 4G - size
    push    edx
    xor     edx, edx
    sub     edx, eax
    movd    mm4, edx
    pop     edx
    ; review:
    ;       eax - Cache zone size
    ;       ebx -
    ;       ecx - Stack size in bytes
    ;       edx - Return status code if EAX contains a return code of higher
    ;             severity than AGESA_SUCCESS
    ;       ebp - Stack Frame pointer
    ;
    ;       esi -  address of start of stack block
    ;       mm0 - user return address
    ;       mm1 - user saved EBP register content
    ;       mm3 - cache zone size
    ;       mm4 - cache zone base
    ;       mm5 - 32b pointer to family info struc. Set by GET_NODE_ID_CORE_ID_Fxx macros

    ; Cross check the cache zone for proper base/length values,
    push    edx
    push    ecx

    and     eax, 0FFFF8000h                     ; size must be >= 32K

    ; Size a Power of Two? We can pull the two largest blocks from the size
    ; then set vMTRR[6..7] to cover those blocks of the zone. The zone is presumed
    ; to be at the top of 4G memory space, so the blocks are allocated in a
    ; 'top down' manner, smaller first at base address then the larger.
    bsr     ecx, eax
    .if (!ZERO?)                                ; Is parameter non-zero?
        push    ecx                             ; save size of larger block
        btr     eax, ecx                        ; reduce zone size by 1st 2**N
        push    eax
        ; calculate upper mask value - needs to match the CPU address bus size
        movzx   ax, [edi].CPU_FAMILY_INFO.SIZE_ADDRESS_BUS
        movzx   eax, ax
        xor     edx, edx
        .if (al <= 64)
            bts     edx, eax
        .endif
        dec     edx                             ; edx = upper mask (e.g. 0x000FFFFF)
        pop     eax                             ; retrieve zone size (minus large block)
        bsr     ecx, eax
        .if (!zero?)
            push    edx                             ; save upper mask, make room to calc new base
            ; set vMTRR[6] for Smaller block, if it exists
            xor     ebx, ebx
            dec     ebx                             ; ebx = all ones
            btr     ebx, ecx
            inc     ebx                             ; ebx = MTRR mask ( e.g 0xFFF80000)
            movd    eax, mm4                        ; cache zone base
            and     eax, ebx                        ; use mask to align base
            xor     edx, edx
            bts     edx, ecx                        ; edx = block size
            add     edx, eax                        ; add block size to base - for next block's base
            movd    mm4, edx                        ; update stored base value
            mov     al,  MTRR_TYPE_WP
            mov     ecx, AMD_MTRR_VARIABLE_BASE6
            xor     edx, edx                        ; clear upper base
            wrmsr                                   ; set the vMTRR[6] Base
            mov     eax, ebx                        ; now build the mask
            pop     edx                             ; retrieve upper mask value
            bts     eax, VMTRR_VALID
            inc     ecx
            wrmsr                                   ; set the vMTRR[6] Mask + Valid
        .endif                                  ; Any remaining size is abandoned. We can only use 2 vMTRRs
        pop     ecx                             ; retrieve size of larger block
        push    edx                             ; save upper mask value
        ; set vMTRR[7] for Larger block, if it exists
        xor     ebx, ebx
        dec     ebx                             ; ebx=all ones
        btr     ebx, ecx
        inc     ebx                             ; ebx = MTRR mask ( e.g 0xFFF00000)
        movd    eax, mm4                        ; cache zone base
        and     eax, ebx                        ; use mask to align base
        xor     edx, edx                        ; clear upper base
        mov     al,  MTRR_TYPE_WP
        mov     ecx, AMD_MTRR_VARIABLE_BASE7
        wrmsr                                   ; set the vMTRR[7] Base
        mov     eax, ebx                        ; now build the mask
        bts     eax, VMTRR_VALID
        pop     edx                             ; retrieve upper mask value
        inc     ecx
        wrmsr                                   ; set the vMTRR[7] Mask + Valid
    .endif

    ; prepare to exit
    mov     edi, ebp                    ; place stack frame pointer for return
    movd    ebp, mm1                    ; Restore saved user requested register
    movd    ebx, mm0                    ;   and the return address

    pop     ecx
    pop     edx
    mov     eax, AGESA_SUCCESS
AmdEnableUefiStackAbort:

ENDM


;======================================================================
; AMD_DISABLE_UEFI_STACK:  Dismantle the pre-memory cache-as-RAM mode.
;
;   In:
;       EBX  = Return address (preserved)
;
;   Out:
;       EAX = AGESA_SUCCESS
;
;   Description:
;       It is expected that the UEFI PEI core has reloacted the stack to main
;       RAM by this time and the MTRR map has been sync'd. Therefore, this
;       routine will not modify the MTRR settings; but rather, just disable
;       the CAR mode. Cache tags will be invalidated.
;
;   Preserved:
;       ebx, esp
;   Destroyed:
;       eax, ebx, ecx, edx, esi, ebp
;======================================================================
AMD_DISABLE_UEFI_STACK MACRO

    mov     ebp, ebx                    ; Save return address

    ; get node/core/flags of current executing core
    GET_NODE_ID_CORE_ID                 ; Sets ESI[15,8]= Node#; ESI[7,0]= core# (relative to node); flags

    AMD_DISABLE_STACK_FAMILY_HOOK       ; Re-Enable 'normal' cache operations

    mov     ebx, ebp                    ; restore return address (ebx)
    xor     eax, AGESA_SUCCESS

ENDM

;======================================================================
; IS_BSC:  Determine if this is Boot Strap Core
;
;   In:
;       NULL
;
;   Out:
;       CF = 1, it's BSC
;       CF = 0, it's AP
;
;   Destroyed:
;       CF
;======================================================================
IS_BSC MACRO
    pushad
    mov     ecx, APIC_BASE_ADDRESS      ; MSR:0000_001B
    _RDMSR
    bt      eax, APIC_BSC               ; Is this the BSC?
    popad

ENDM
