;/**
; * @file
; *
; * Amd Smm entry code
; *
; * Contains 16 bit Smm entry code
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AMD Udk
; * @e sub-project:  Smm
; * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
; */
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
.code

include  Smm16Include.inc

;/*---------------------------------------------------------------------------------------*/
;/**
; *  SMM 16 bit entry proc
; *
; */

Smm16FunnelProc   PROC  PUBLIC
Smm16FunnelProcStart::
        ;db 0ebh, 0feh; jmp $
        db 66h,  0BAh                 ; mov       edx,dword ptr [si]
        dd  0                         ; EDX       keep Smm16Base       ;<Patched by SmmIPL>

; At this point CS = 0x(vv00) and ip= 0x0.

        db 8ch,  0c8h                 ; mov        ax,  cs
        db 8eh,  0d8h                 ; mov        ds,  ax
        db 8eh,  0c0h                 ; mov        es,  ax
        db 8eh,  0d0h                 ; mov        ss,  ax
        db 33h,  0c0h                 ; xor        ax,  ax
        db 8eh,  0e0h                 ; mov        fs,  ax
        db 8eh,  0e8h                 ; mov        gs,  ax

; Switch to flat mode.

        db 0BEh
        dw gdtr-  Smm16FunnelProcStart ; mov        si, gdtr
        db 081h, 0c6h, 000h, 080h     ; add        si, 08000                     ; Adjust for SMBase since the code will start smbase+8000 (i.e. ip added 8000h)
        db 66h                        ; db         66h
        db 2Eh,  0Fh, 01h, 14h        ; lgdt       fword ptr cs:[si]

        db 0BEh
        dw idtr- Smm16FunnelProcStart ; mov        si, IdtrProfile
        db 081h, 0c6h, 000h, 080h     ; add        si, 08000                     ; Adjust for SMBase since the code will start smbase+8000 (i.e. ip added 8000h)
        db 66h                        ; db         66h
        db 2Eh,  0Fh, 01h, 1Ch        ; lidt       fword ptr cs:[si]

        db 33h,  0C0h                 ; xor        ax,  ax
        db 8Eh,  0D8h                 ; mov        ds,  ax

        db 0Fh,  20h, 0C0h            ; mov        eax, cr0                    ; Get control register 0
        db 66h,  83h, 0C8h, 01h       ; or         eax, 000000001h             ; Set PE bit (bit #0)
        db 0Fh,  22h, 0C0h            ; mov        cr0, eax

SmmFLAT32_JUMP::

        db 66h,  67h, 0EAh            ; far jump
        dd 0h                         ; 32-bit offset                          ;<Patched by SmmIPL>
        dw LINEAR_CODE_SEL            ; 16-bit selector

SmmProtectedModeStart::

        db 66h,  0B8h, 08h,  00h      ; mov        ax,  08h
        db 66h,  8Eh,  0D8h           ; mov        ds,  ax
        db 66h,  8Eh,  0C0h           ; mov        es,  ax
        db 66h,  8Eh,  0E0h           ; mov        fs,  ax
        db 66h,  8Eh,  0E8h           ; mov        gs,  ax
        db 66h,  8Eh,  0D0h           ; mov        ss,  ax                     ; Flat mode setup.

Cr3Offset::
        db 0B8h                       ; mov        eax, Cr3Value
        dd 0                                                                  ;<Patched by SmmIPL>
        db 00fh, 022h, 0D8h,          ; mov        cr3, eax

        db 0Fh,  20h,  0E0h           ; mov        eax, cr4

        db 00Dh
        dd 00000660h                  ; or         eax, 0660h                 ; Enable PE and XMM
        ;db 0Fh,  0BAh, 0E8h, 05h     ; bts        eax, 5

        db 0Fh,  22h,  0E0h           ; mov        cr4, eax

        db 8Bh,  0F2h                 ; mov        esi, edx                    ; Save wakeup buffer address

        db 0B9h
        dd 0C0000080h                 ; mov        ecx, 0c0000080h             ; EFER MSR number.
        db 0Fh,  32h                  ; rdmsr                                  ; Read EFER.
        db 0Fh,  0BAh, 0E8h, 08h      ; bts        eax, 8                      ; Set LME=1.
        db 0Fh,  30h                  ; wrmsr                                  ; Write EFER.

        db 0Fh,  20h,  0C0h           ; mov        eax, cr0                    ; Read CR0.
        db 0Fh,  0BAh, 0E8h, 1Fh      ; bts        eax, 31                     ; Set PG=1.
        db 0Fh,  22h,  0C0h           ; mov        cr0, eax                    ; Write CR0.

LONG_JUMP::

        db 67h,  0EAh                 ; far jump
        dd 0h                         ; 32-bit offset                          ;<Patched by SmmIPL>
        dw LINEAR_CODE64_SEL          ; 16-bit selector

SmmLongModeStart::

        mov         ax,  08h
        mov         ds,  ax
        mov         es,  ax
        mov         ss,  ax

        ;
        ; ProgramStack
        ;
        mov         edi, esi
        add         edi, StackStartAddressLocation
        mov         rsp, qword ptr [edi]

        ;
        ; Call Smm64 Foundation code
        ;
        mov         edi, esi
        add         edi, Smm64EntryOffset
        mov         rax, qword ptr [edi]

        test        rax, rax
        jz          GoToSleep

        sub         rsp, 20h
        xor         rcx, rcx
        xor         rdx, rdx
        mov         ecx, esi
        add         ecx, SmmAddressMap

        push        rcx
        push        rdx
        call        rax           ;Call SmmEntry64 entry
        add         rsp, 10h

GoToSleep::
        ;We should never come here
        rsm

Smm16FunnelProc   ENDP

ALIGN   2

gdtr:
    dw      GDT_END - GDT_BASE - 1            ; GDT limit
    dd      0                                 ; GDT_BASE        ; <Patched by SmmIPL>

idtr:
    dw      IDT_END - IDT_BASE - 1            ; IDT limit
    dd      0                                 ; IDT_BASE        ; <Patched by SmmIPL>

ALIGN   16

;
; The Global Descriptor Table (GDT)
;

GDT_BASE:
; null descriptor
; null descriptor
NULL_SEL            equ $-GDT_BASE
    DW      0            ; limit 15:0
    DW      0            ; base 15:0
    DB      0            ; base 23:16
    DB      0            ; sys flag, dpl, type
    DB      0            ; limit 19:16, flags
    DB      0            ; base 31:24

; linear data segment descriptor
LINEAR_SEL          equ $-GDT_BASE
    DW      0ffffh       ; limit 15:0
    DW      0            ; base 15:0
    DB      0            ; base 23:16
    DB      93h          ; PRESENT_FLAG(1)|DPL(0)|SYSTEM_FLAG(1)|DESC_TYPE(DATA32_TYPE)
    DB      0CFh         ; GRANULARITY_FLAG(1)|DEFAULT_SIZE32(1)|CODE64_FLAG(0)|UPPER_LIMIT(0xf)
    DB      0            ; base 31:24

; linear code segment descriptor
LINEAR_CODE_SEL     equ $-GDT_BASE
    DW      0ffffh       ; limit 15:0
    DW      0            ; base 15:0
    DB      0            ; base 23:16
    DB      09Bh         ; PRESENT_FLAG(1)|DPL(0)|SYSTEM_FLAG(1)|DESC_TYPE(CODE32_TYPE)
    DB      0CFh         ; GRANULARITY_FLAG(1)|DEFAULT_SIZE32(1)|CODE64_FLAG(0)|UPPER_LIMIT(0xf)
    DB      0            ; base 31:24

; linear 64 bit code segment descriptor
LINEAR_CODE64_SEL     equ $-GDT_BASE
    DW      0ffffh       ; limit 15:0
    DW      0            ; base 15:0
    DB      0            ; base 23:16
    DB      09Bh         ; PRESENT_FLAG(1)|DPL(0)|SYSTEM_FLAG(1)|DESC_TYPE(CODE32_TYPE)
    DB      0AFh         ; GRANULARITY_FLAG(1)|DEFAULT_SIZE32(0)|CODE64_FLAG(1)|UPPER_LIMIT(0xf)
    DB      0            ; base 31:24
GDT_END:

;
; The Global Descriptor Table (GDT)
;

IDT_BASE:
; null descriptor
; null descriptor
NULL_SEL            equ $-IDT_BASE
    DW      0            ; limit 15:0
    DW      0            ; base 15:0
    DB      0            ; base 23:16
    DB      0            ; sys flag, dpl, type
    DB      0            ; limit 19:16, flags
    DB      0            ; base 31:24
IDT_END:

SmmSpinLock::
    DQ      0
Smm16FunnelProcEnd::

;-------------------------------------------------------------------------------------
;  AsmGetSmmPatchAddressMap (&AddressMap);
;-------------------------------------------------------------------------------------
AsmGetSmmPatchAddressMap   PROC

        mov         rax, offset Smm16FunnelProcStart
        mov         qword ptr [rcx], rax

        mov         qword ptr [rcx+08h], SmmProtectedModeStart - Smm16FunnelProcStart
        mov         qword ptr [rcx+10h], SmmFLAT32_JUMP - Smm16FunnelProcStart

        mov         qword ptr [rcx+18h], SmmLongModeStart - Smm16FunnelProcStart
        mov         qword ptr [rcx+20h], LONG_JUMP - Smm16FunnelProcStart

        mov         qword ptr [rcx+28h], gdtr - Smm16FunnelProcStart
        mov         qword ptr [rcx+30h], GDT_BASE - Smm16FunnelProcStart

        mov         qword ptr [rcx+38h], Cr3Offset - Smm16FunnelProcStart
        mov         qword ptr [rcx+40h], Smm16FunnelProcEnd - Smm16FunnelProcStart
        mov         qword ptr [rcx+48h], SmmSpinLock - Smm16FunnelProcStart
        mov         qword ptr [rcx+50h], idtr - Smm16FunnelProcStart
        mov         qword ptr [rcx+58h], IDT_BASE - Smm16FunnelProcStart

        ret

AsmGetSmmPatchAddressMap   ENDP

;-------------------------------------------------------------------------------------
; return AsmGetSmm16Size ();
;-------------------------------------------------------------------------------------
AsmGetSmm16CodeSize PROC

        push        rbx
        mov         rax, offset Smm16FunnelProcEnd
        mov         rbx, offset Smm16FunnelProcStart
        sub         rax, rbx
        pop         rbx

        ret
AsmGetSmm16CodeSize ENDP

END

