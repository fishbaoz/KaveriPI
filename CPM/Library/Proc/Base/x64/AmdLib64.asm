;/**
; * @file
; *
; * AMD CPM library 64bit for IO Access
; *
; * Contains AMD CPM Library
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      CPM
; * @e sub-project:  Library
; * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
; */
;*****************************************************************************
;
; Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
;/*++

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO byte
; *
; *  @param[in]   CX    IO port address
; *  @param[in]   DL    IO port Value
; */
PUBLIC  CpmIoWrite8
CpmIoWrite8     PROC
        mov     al, dl
        mov     dx, cx
        out     dx, al
        ret
CpmIoWrite8     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO word
; *
; *  @param[in]   CX      IO port address
; *  @param[in]   DX      IO port Value
; */
PUBLIC  CpmIoWrite16
CpmIoWrite16    PROC
        mov     ax, dx
        mov     dx, cx
        out     dx, ax
        ret
CpmIoWrite16    ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO dword
; *
; *  @param[in]   CX      IO port address
; *  @param[in]   EDX     IO port Value
; */
PUBLIC CpmIoWrite32
CpmIoWrite32    PROC
        mov     eax, edx
        mov     dx, cx
        out     dx, eax
        ret
CpmIoWrite32    ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO byte
; *
; *  @param[in] CX  IO port address
; *  @retval    AL  IO port Value
; */
PUBLIC CpmIoRead8
CpmIoRead8      PROC
        mov     dx, cx
        in      al, dx
        ret
CpmIoRead8      ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO word
; *
; *  @param[in]   CX  IO port address
; *  @retval      AX  IO port Value
; */
PUBLIC CpmIoRead16
CpmIoRead16     PROC
        mov     dx, cx
        in      ax, dx
        ret
CpmIoRead16     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO dword
; *
; *  @param[in]   CX  IO port address
; *  @retval      EAX IO port Value
; */
PUBLIC CpmIoRead32
CpmIoRead32     PROC
        mov     dx, cx
        in      eax, dx
        ret
CpmIoRead32     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read MSR
; *
; *  @param[in]  RCX      MSR Address
; *  @retval     RAX      MSR Register Value
; */
PUBLIC CpmMsrRead
CpmMsrRead      PROC
        rdmsr
        and     rax, 0ffffffffh
        shl     rdx, 32
        or      rax, rdx
        ret
CpmMsrRead      ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write MSR
; *
; *  @param[in]  RCX        MSR Address
; *  @param[in]  RDX        MSR Register Data
; */
PUBLIC CpmMsrWrite
CpmMsrWrite     PROC
        mov     rax, rdx
        and     rax, 0ffffffffh
        shr     rdx, 32
        wrmsr
        ret
CpmMsrWrite     ENDP


;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read CPUID
; *
; *  @param[in]  RCX    CPUID function
; *  @param[in]  RDX    Pointer to CPUID_DATA to save cpuid data
; */
PUBLIC CpmCpuidRead
CpmCpuidRead      PROC
    push rbx
    push rsi
    mov  rsi, rdx
    mov  rax, rcx
    cpuid
    mov [rsi],   eax
    mov [rsi+4], ebx
    mov [rsi+8], ecx
    mov [rsi+12],edx
    pop rsi
    pop rbx
    ret
CpmCpuidRead    ENDP


;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read TSC
; *
; *  @retval    RAX Time stamp counter value
; */

PUBLIC CpmReadTsc
CpmReadTsc      PROC
        rdtsc
        and     rax, 0ffffffffh
        shl     rdx, 32
        or      rax, rdx
        ret
CpmReadTsc      ENDP

END
