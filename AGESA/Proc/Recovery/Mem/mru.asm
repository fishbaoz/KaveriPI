;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
;  $Workfile:: mu.asm   $ $Revision:: 281#$  $Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
; Description: Main memory controller system configuration for AGESA DDR 2
;
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
;============================================================================


    .XLIST
    .LIST
    .686p
    .MODEL FLAT
    .CODE
     ASSUME FS: NOTHING


; Define the calling convention used for the C library modules
;@attention - This should be in a central include file
CALLCONV    EQU     NEAR C


;===============================================================================
;MemRecUOutPort:
;
; Do a 32 Bit IO Out operation using edx.
; NOTE: This function will be obsolete in the future.
;
;             In: Port - port number
;                 Value - value to be written to port
;
;            Out:
;
;All registers preserved except for "Out:"
;===============================================================================
MemRecUOutPort PROC CALLCONV PUBLIC Port:DWORD, Value:DWORD
    pushad
    mov edx,Port
    mov eax,Value
    out dx,al
    popad
    ret
MemRecUOutPort ENDP



;----------------------------------------------------------------------------
; _MFENCE();
;
_MFENCE macro
    db  0Fh,0AEh,0F0h
    endm

;----------------------------------------------------------------------------
; _EXECFENCE();
;
_EXECFENCE macro
    out 0EDh,al             ;prevent speculative execution of following instructions
    endm

;===============================================================================
;MemRecUWrite1CL:
;
;   Write data from buffer to a system address
;
;             In: Address - System address to read from
;                 Pattern - pointer pattern.
;
;            Out:
;
;All registers preserved except for "Out:"
;===============================================================================
MemRecUWrite1CL PROC CALLCONV PUBLIC Address:DWORD, Pattern:NEAR PTR DWORD
        pushad
        push ds

        mov eax,Address
        push ss
        pop ds
        xor edx,edx
        mov edx, DWORD PTR Pattern
        mov esi,edx
        mov edx,16
        _EXECFENCE
        mov ecx,4
        @@:
        db 66h,0Fh,6Fh,06           ;MOVDQA xmm0,[esi]
        db 64h,66h,0Fh,0E7h,00      ;MOVNTDQ fs:[eax],xmm0  (xmm0 is 128 bits)
        add eax,edx
        add esi,edx
        loop @B

        pop ds
        popad
        ret
MemRecUWrite1CL ENDP

;===============================================================================
;MemRecURead1CL:
;
; Read one cacheline to buffer
;
;             In: Buffer - pointer buffer.
;               : Address - System address to read from
;
;            Out:
;
;All registers preserved except for "Out:"
;===============================================================================
MemRecURead1CL PROC CALLCONV PUBLIC Buffer:NEAR PTR DWORD, Address:DWORD

        pushad

        mov esi,Address
        xor edx,edx
        mov edx,DWORD PTR Buffer
        mov edi,edx
        mov ecx,64
        @@:
        mov al,fs:[esi]
        mov ss:[edi],al
        inc esi
        inc edi
        loop @B

        popad
        ret
MemRecURead1CL ENDP


;===============================================================================
;MemRecUFlushPattern:
;
; Flush one cache line
;
;             In:   Address - System address [31:0]
;            Out:
;
;All registers preserved except for "Out:"
;===============================================================================
MemRecUFlushPattern PROC CALLCONV PUBLIC Address:DWORD
        pushad
        mov eax,Address
        _EXECFENCE
                                        ;clflush fs:[eax]
        db  064h                        ;access relative to FS BASE prefix
        db  00Fh                        ;opcode
        db  0AEh                        ;opcode
        db  038h                        ;eax indirect addressing
        _MFENCE
        popad
        ret
MemRecUFlushPattern ENDP



    END

