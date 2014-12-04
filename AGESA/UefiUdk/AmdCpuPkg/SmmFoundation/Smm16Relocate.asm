;/**
; * @file
; *
; * Amd Smm relocate code
; *
; * Contains Smm relocate code
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

;
; The below code will be copied into 3000:800 by SmmIpl to set relocate the Smmbase of BSP/AP
SmmRelocate16Entry   PROC  PUBLIC
SmmProcReloc16Start::

 db 0E9h, 019h, 00h                         ;3000:8000    jmp loc_801c

 Align 16
SmmRelocInfo::
 dd 0                                       ; 3000:8010   dd 0 ; New Smmbase
 dd 0                                       ; 3000:8014   dd 0 ; TsegBase
 dd 0                                       ; 3000:8018   dd 0 ; Tseg Size

 db 0FAh                                    ; 3000:801C   cli
 db 08Ch, 0C8h                              ; 3000:801D   mov ax,cs
 db 08Eh, 0D8h                              ; 3000:801F   mov ds,ax

 db 0BEh, 014h, 080h                        ; 3000:8021   mov si,8014   ; offset TsegBase
 db 066h, 08Bh, 004h                        ; 3000:8024   mov eax,[si]  ; eax = Tseg
 db 066h, 031h, 0D2h                        ; 3000:8027   xor edx,edx
 db 066h, 0B9h, 012h, 001h, 001h, 0C0h      ; 3000:802A   mov ecx,c0010112 ; Set MSR with TsegBase
 db 00Fh, 030h                              ; 3000:8030   wrmsr

 db 0BEh, 018h, 080h                        ; 3000:8032   mov si,8018     ; offset TsegSize
 db 066h, 031h, 0C0h                        ; 3000:8035   xor eax,eax
 db 066h, 08Bh, 014h                        ; 3000:8038   mov edx,[si]    ; edx = TsegSize
 db 066h, 029h, 0D0h                        ; 3000:803B   sub eax,edx
 db 066h, 00Dh, 003h, 060h, 000h, 000h      ; 3000:803E   or eax,00006003 ; Tseg attibute
 db 066h, 0BAh, 0FFh, 0FFh, 000h, 000h      ; 3000:8044   mov edx,0000ffff; edx = Tsegmask
 db 066h, 0B9h, 013h, 001h, 001h, 0C0h      ; 3000:804A   mov ecx,c0010113  ;Set MSR with Tseg mask
 db 00Fh, 030h                              ; 3000:8050   wrmsr

 db 0BFh, 0C9h, 0FEh                        ; 3000:8052   mov di,fec9
 db 031h, 0C0h                              ; 3000:8055   xor ax,ax
 db 088h, 005h                              ; 3000:8057   mov [di],al   ; Set AutoHaltRestart in SmmSaveArea
 db 0BEh, 010h, 080h                        ; 3000:8059   mov si,8010   ; Offset New SmmBase
 db 0BFh, 000h, 0FFh                        ; 3000:805C   mov di,ff00   ; SMMBase in save area
 db 066h, 08Bh, 004h                        ; 3000:805F   mov eax,[si]  ; eax = New Smmbase
 db 066h, 089h, 005h                        ; 3000:8062   mov [di],eax  ; Update new smm base

 db 066h, 031h, 0C0h                        ; 3000:8065   xor eax,eax   ; Clear SmmSave area
 db 066h, 089h, 004h                        ; 3000:8068   mov [si],eax  ; Clear to indicate done

 db 00Fh, 0AAh                              ; 3000:806B   rsm           ; Bail out
 db 0F4h                                    ; 3000:806D   hlt

 ret
SmmRelocate16Entry   ENDP
SmmProcReloc16End::

;-------------------------------------------------------------------------------------
;  AsmGetSmmPatchAddressMap (&AddressMap);
;-------------------------------------------------------------------------------------
AsmGetSmmRelocPatchMap   PROC

        mov         rax, offset SmmProcReloc16Start
        mov         qword ptr [rcx], rax
        mov         qword ptr [rcx+08h], SmmRelocInfo - SmmProcReloc16Start
        mov         qword ptr [rcx+10h], SmmProcReloc16End - SmmProcReloc16Start


        ret

AsmGetSmmRelocPatchMap   ENDP

END
