;/**
; * @file
; *
; * AMD CPM library 32bit for IO Access
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

.586p
.model  flat
.code

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO byte
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */
public  CpmIoWrite8
CpmIoWrite8     PROC NEAR C USES DX AX Address:WORD, Data:Byte
        mov     dx, Address
        mov     al, Data
        out     dx, al
        ret
CpmIoWrite8     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO word
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */
public CpmIoWrite16
CpmIoWrite16    PROC NEAR C USES DX AX Address:WORD, Data:WORD
        mov     dx, Address
        mov     ax, Data
        out     dx, ax
        ret
CpmIoWrite16    ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write IO dword
; *
; *  @param[in]   Address IO port address
; *  @param[in]   Data    IO port Value
; */
public CpmIoWrite32
CpmIoWrite32    PROC NEAR C USES DX EAX Address:WORD, Data:DWORD
        mov     dx, Address
        mov     eax, Data
        out     dx, eax
        ret
CpmIoWrite32    ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO byte
; *
; *  @param[in] - IO port address
; *  @retval      IO port Value
; */
public CpmIoRead8
CpmIoRead8      PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      al, dx
        ret
CpmIoRead8      ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO word
; *
; *  @param[in]   Address IO port address
; *  @retval      IO port Value
; */
public CpmIoRead16
CpmIoRead16     PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      ax, dx
        ret
CpmIoRead16     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read IO dword
; *
; *  @param[in]   Address  IO port address
; *  @retval      IO port Value
; */
public CpmIoRead32
CpmIoRead32     PROC NEAR C USES DX Address:WORD
        mov     dx, Address
        in      eax, dx
        ret
CpmIoRead32     ENDP


;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read MSR
; *
; *  @param[in]  Address  MSR Address
; *  @retval     MSR register Value
; */
public CpmMsrRead
CpmMsrRead      PROC NEAR C USES ECX Address:DWORD
        mov     ecx, Address
        rdmsr
        ret
CpmMsrRead      ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Write MSR
; *
; *  @param[in]  Address    MSR Address
; *  @param[in]  Data       MSR Register Data
; */
public CpmMsrWrite
CpmMsrWrite     PROC NEAR C USES ECX Address:DWORD, Data:QWORD
        mov     ecx, Address
        mov     eax, DWORD PTR Data[0]
        mov     edx, DWORD PTR Data[4]
        wrmsr
        ret
CpmMsrWrite     ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read CPUID
; *
; *  @param[in]  Func   CPUID function
; *  @param[in]  DATA   Pointer to CPUID_DATA to save cpuid data
; */
public CpmCpuidRead
CpmCpuidRead    PROC NEAR C  Func:DWORD, DATA:PTR
        pushad
        mov     eax, Func
        cpuid
        mov     esi, DATA
        mov     [esi],   eax
        mov     [esi+4], ebx
        mov     [esi+8], ecx
        mov     [esi+12],edx
        popad
        ret
CpmCpuidRead    ENDP

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Read TSC
; *
; *  @retval     Time stamp counter value
; */
public CpmReadTsc
CpmReadTsc      PROC    NEAR C
        rdtsc
        ret
CpmReadTsc      ENDP

END
