;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
; $Workfile:: cpcar.inc
;
; Description: CPCAR.INC - AGESA cache-as-RAM setup Include File
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
    .LIST
    .mmx

BSP_STACK_BASE_ADDR     EQU 30000h      ; Base address for primary cores stack
BSP_STACK_SIZE_64K      EQU 10000h      ; 64KB for BSP core
BSP_STACK_SIZE_32K      EQU 8000h       ; 32KB for BSP core
CORE0_STACK_BASE_ADDR   EQU 80000h      ; Base address for primary cores stack
CORE0_STACK_SIZE        EQU 4000h       ; 16KB for primary cores
CORE1_STACK_BASE_ADDR   EQU 40000h      ; Base address for AP cores
CORE1_STACK_SIZE        EQU 1000h       ; 4KB for each AP cores

APIC_BASE_ADDRESS       EQU 0000001Bh
    APIC_BSC                EQU 8       ; Boot Strap Core

APIC_MSG_REG            EQU 380h        ; Location of BSC message
    APIC_MSG            EQU 00DE00ADh   ; Message data
    APIC_INVD_ALL_DONE_MSG  EQU     00AD00DEh ; Indicate all cores have invalidated
APIC_CMD_LO_REG         EQU 300h        ; APIC command low
APIC_CMD_HI_REG         EQU 310h        ; APIC command high
    CMD_REG_TO_READ_DATA EQU 00000338h  ; APIC command for remote read of APIC_MSG_REG
    REMOTE_READ_STS       EQU 00030000h ; Remote read status mask
    REMOTE_DELIVERY_PEND  EQU 00010000h ; Remote read is pending
    REMOTE_DELIVERY_DONE  EQU 00020000h ; Remote read is complete
    DELIVERY_STS_BIT    EQU 12          ; Delivery status valid bit
APIC_ID_REG             EQU 0020h       ; Local APIC ID offset
    APIC20_APICID       EQU 24
APIC_REMOTE_READ_REG    EQU 00C0h       ; Remote read offset

; Flags can only run from bits 31 to 24.  Bits 23:0 are in use.
AMD_CU_NEED_TO_WAIT     EQU 31
AMD_CU_SEND_INVD_MSG    EQU 30
AMD_CU_RESTORE_ES       EQU 29

AMD_MTRR_VARIABLE_BASE0 EQU 0200h
AMD_MTRR_VARIABLE_BASE6 EQU 020Ch
AMD_MTRR_VARIABLE_BASE7 EQU 020Eh
    VMTRR_VALID             EQU     11
    MTRR_TYPE_WB            EQU     06h
    MTRR_TYPE_WP            EQU     05h
    MTRR_TYPE_WT            EQU     04h
    MTRR_TYPE_UC            EQU     00h
AMD_MTRR_VARIABLE_MASK7 EQU 020Fh
AMD_MTRR_FIX64k_00000   EQU 0250h
AMD_MTRR_FIX16k_80000   EQU 0258h
AMD_MTRR_FIX16k_A0000   EQU 0259h
AMD_MTRR_FIX4k_C0000    EQU 0268h
AMD_MTRR_FIX4k_C8000    EQU 0269h
AMD_MTRR_FIX4k_D0000    EQU 026Ah
AMD_MTRR_FIX4k_D8000    EQU 026Bh
AMD_MTRR_FIX4k_E0000    EQU 026Ch
AMD_MTRR_FIX4k_E8000    EQU 026Dh
AMD_MTRR_FIX4k_F0000    EQU 026Eh
AMD_MTRR_FIX4k_F8000    EQU 026Fh

AMD_MTRR_DEFTYPE        EQU 02FFh
    WB_DRAM_TYPE            EQU 1Eh     ; MemType - memory type
    MTRR_DEF_TYPE_EN        EQU 11      ; MtrrDefTypeEn - variable and fixed MTRRs default enabled
    MTRR_DEF_TYPE_FIX_EN    EQU 10      ; MtrrDefTypeEn - fixed MTRRs default enabled

HWCR                    EQU 0C0010015h  ; Hardware Configuration
    INVD_WBINVD             EQU 4       ;   INVD to WBINVD conversion

IORR_BASE               EQU 0C0010016h  ; IO Range Regusters Base/Mask, 2 pairs
                                        ;   uses 16h - 19h
TOP_MEM                 EQU 0C001001Ah  ; Top of Memory
TOP_MEM2                EQU 0C001001Dh  ; Top of Memory2

LS_CFG3                 EQU 0C001101Ch  ; Load-Store Configuration 3
    DIS_SS_F15CZ            EQU 7       ;   Disable Streaming Store functionality
    DC_DIS_SPEC_TLB_RLD_F15CZ   EQU 20  ;   Disable speculative TLB reloads
    DC_DIS_HW_PF_F15CZ      EQU 23      ;   Hardware prefetches bit

LS_CFG                  EQU 0C0011020h  ; Load-Store Configuration
    DIS_SS                  EQU 28      ;   Family 15h:Disable Streaming Store functionality

IC_CFG                  EQU 0C0011021h  ; Instruction Cache Config Register
    IC_DIS_SPEC_TLB_RLD     EQU 9       ;   Disable speculative TLB reloads

DC_CFG                  EQU 0C0011022h  ; Data Cache Configuration
    DC_DIS_SPEC_TLB_RLD     EQU 4       ;   Disable speculative TLB reloads
    DC_DIS_SPEC_TLB_WALK    EQU 4       ;   Disable speculative table-walks
    DIS_HW_PF               EQU 13      ;   Hardware prefetches bit

CU_CFG                  EQU 0C0011023h  ; Family 15h: Combined Unit Configuration
    L2_WAY_LOCK_EN          EQU 23      ;   L2WayLock - L2 way lock enable
    L2_FIRST_LOCKED_WAY     EQU 19      ;   L2FirstLockedWay - first L2 way lockedh
    L2_FIRST_LOCKED_WAY_OR_MASK  EQU 000780000h

BU_CFG2                 EQU 0C001102Ah  ; Family 16h: Bus Unit Configuration 2
CU_CFG2                 EQU 0C001102Ah  ; Family 15h: Combined Unit Configuration 2

CU_CFG3                 EQU 0C001102Bh  ; Combined Unit Configuration 3
    COMBINE_CR0_CD          EQU 49      ;   Combine CR0.CD for both cores of a compute unit

L2I_CFG                 EQU 0C00110A0h  ; L2I Configuration
    L2_RINSER_DIS       EQU 20          ;   L2 rinser disable
    PREFETCHER_DIS      EQU 7           ;   L2 prefetcher disable
    CACHE_IC_ATTR_DIS   EQU 3           ;   Inserting IC attributes into the L2 disable

CR0_PE                  EQU 0           ; Protection Enable
CR0_NW                  EQU 29          ; Not Write-through
CR0_CD                  EQU 30          ; Cache Disable
CR0_PG                  EQU 31          ; Paging Enable

; CPUID Functions

CPUID_MODEL             EQU 1
AMD_CPUID_L2Cache       EQU 80000006h   ; L2/L3 cache info
AMD_CPUID_APIC          EQU 80000008h   ; Long Mode and APIC info., core count
    APIC_ID_CORE_ID_SIZE     EQU 12     ; ApicIdCoreIdSize bit position

NB_CFG                  EQU 0C001001Fh  ; Northbridge Configuration Register
    INIT_APIC_ID_CPU_ID_LO   EQU 54     ;   InitApicIdCpuIdLo - is core# in high or low half of APIC ID?
    ENABLE_CF8_EXT_CFG       EQU 46     ;   EnableCf8ExtCfg - enable CF8 extended configuration cycles

MTRR_SYS_CFG            EQU 0C0010010h  ; System Configuration Register
  SYS_UC_LOCK_EN            EQU 17      ;   SysUcLockEn      System lock command enable
  MTRR_FIX_DRAM_EN          EQU 18      ;   MtrrFixDramEn    MTRR fixed RdDram and WrDram attributes enable
  MTRR_FIX_DRAM_MOD_EN      EQU 19      ;   MtrrFixDramModEn MTRR fixed RdDram and WrDram modification enable
  MTRR_VAR_DRAM_EN          EQU 20      ;   MtrrVarDramEn    MTRR variable DRAM enable

COMPUTE_UNIT_STATUS     EQU 08000C580h  ; Compute Unit Status Register
  QUAD_CORE                 EQU 24      ;   QuadCore         four cores of a compute unit are enabled
  DUAL_CORE                 EQU 16      ;   DualCore         two cores of a compute unit are enabled
  TRIPLE_CORE               EQU 8       ;   TripleCore       three cores of a compute unit are enabled
  CU_ENABLED                EQU 0       ;   Enabled          at least one core of a compute unit is enabled

; PCI Registers
FUNC_3                      EQU 3
MCA_NB_CFG                  EQU 44h     ; MCA NB Configuration
  CPU_ERR_DIS               EQU 6       ;   CPU error response disable

PRODUCT_INFO_REG1           EQU 1FCh    ; Product Information Register 1

; Local use flags, in upper most byte of ESI
FLAG_UNKNOWN_FAMILY               EQU 24    ; Signals that the family# of the installed processor is not recognized
FLAG_STACK_REENTRY                EQU 25    ; Signals that the environment has made a re-entry (2nd) call to set up the stack
FLAG_IS_PRIMARY                   EQU 26    ; Signals that this core is the primary within the compute unit
FLAG_CORE_NOT_IDENTIFIED          EQU 27    ; Signals that the cores/compute units of the installed processor is not recognized
FLAG_FORCE_32K_STACK              EQU 28    ; Signals that to force 32KB stack size for BSP core

; Error code returned in EDX by AMD_ENABLE_STACK
IFNDEF CPU_EVENT_UNKNOWN_PROCESSOR_FAMILY
       CPU_EVENT_UNKNOWN_PROCESSOR_FAMILY     EQU   008010500h
ENDIF
IFNDEF CPU_EVENT_STACK_REENTRY
       CPU_EVENT_STACK_REENTRY                EQU   008020500h
ENDIF
IFNDEF CPU_EVENT_CORE_NOT_IDENTIFIED
       CPU_EVENT_CORE_NOT_IDENTIFIED          EQU   008030500h
ENDIF

; AGESA_STATUS values
IFNDEF AGESA_SUCCESS
       AGESA_SUCCESS  EQU 0
ENDIF
IFNDEF AGESA_WARNING
       AGESA_WARNING  EQU 4
ENDIF
IFNDEF AGESA_FATAL
       AGESA_FATAL    EQU 7
ENDIF
;;***************************************************************************
;;
;;                      CPU MACROS - PUBLIC
;;
;;***************************************************************************
_WRMSR macro
  db  0Fh, 30h
  endm

_RDMSR macro
  db  0Fh, 32h
  endm

AMD_CPUID MACRO arg0
  IFB <arg0>
    mov   eax, 1
    db    0Fh, 0A2h                     ; Execute instruction
    bswap eax
    xchg  al, ah                        ; Ext model in al now
    rol   eax, 8                        ; Ext model in ah, model in al
    and   ax, 0FFCFh                    ; Keep 23:16, 7:6, 3:0
  ELSE
    mov   eax, arg0
    db    0Fh, 0A2h
  ENDIF
ENDM

MAKE_EXT_PCI_ADDR MACRO Seg, Bus, Dev, Func, Offset
    mov   eax, ((1 SHL 31) OR (Seg SHL 28) OR (((Offset AND 0F00h) SHR 8) SHL 24) OR (Bus SHL 16) OR (Dev SHL 11) OR (Func SHL 8) OR (Offset AND 0FCh))
  ENDM

;---------------------------------------------------
; LoadTableAddress
;       Due to the various assembly methodologies used by BIOS vendors, this macro is needed to abstract the
;       loading of the address of a table. The default is the standard LEA instruction with table address.
;       The IBV that needs to use an alternative method can define their version of the macro prior to including
;       this file into their source.
;       An alternative example:
;         LoadTableAddress  MACRO MyTable
;           LEA   eax, -(LAST_ADDRESS - MyTable)
;---------------------------------------------------
IFNDEF LoadTableAddress
  LoadTableAddress        Macro TargetTable
       LEA   eax, TargetTable
  ENDM
ENDIF


;;***************************************************************************
;;
;;                      CPU STRUCTURES - PUBLIC
;;
;;***************************************************************************
CPU_FAMILY_INFO     STRUC
    L2_MIN_SIZE         WORD    ?       ; Minimum size of the L2 cache for this family, in K
    NUM_SHARED_CORES    BYTE    ?       ; Number of cores sharing an L2 cache
    L2_ALLOC_MEM        BYTE    ?       ; L2 space reserved for memory training, in K
    L2_ALLOC_EXE        WORD    ?       ; L2 space reserved for EXE CACHE, in K. 0 means unlimited.
    SIZE_ADDRESS_BUS    BYTE    ?       ; Number of address bits supported by this family
    FAMILY_RESERVED     BYTE    ?       ; reserved, pad to DWORD size
CPU_FAMILY_INFO     ENDS

;============================================================================
;
; Define a  macro that allow the exclusion of processor families from
; the cache-as-ram code. This will reduce the size of the assembled file.
;
;============================================================================


NoFamilyDefined   = 1
CHECK_FOR_FAMILY_DEFINITIONS MACRO
    IFDEF OPTION_FAMILY15H
        NoFamilyDefined = 0
    ENDIF
    IFDEF OPTION_FAMILY16H
        NoFamilyDefined = 0
    ENDIF
ENDM


;---------------------------------------------------
;
; AMD_ENABLE_STACK_FAMILY_HOOK Macro - Stackless
;
;   Set any family specific controls needed to enable the use of
;   cache as general storage before main memory is available.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;---------------------------------------------------
AMD_ENABLE_STACK_FAMILY_HOOK MACRO

    CHECK_FOR_FAMILY_DEFINITIONS

IF NoFamilyDefined
    AMD_ENABLE_STACK_FAMILY_HOOK_F15
ELSE
    IFDEF   OPTION_FAMILY15H
        AMD_ENABLE_STACK_FAMILY_HOOK_F15
    ENDIF
ENDIF

ENDM

;----------------------------------------------
;
; AMD_DISABLE_STACK_FAMILY_HOOK Macro - Stackless
;
;   Return any family specific controls to their 'standard'
;   settings for using cache with main memory.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;----------------------------------------------
AMD_DISABLE_STACK_FAMILY_HOOK MACRO

    CHECK_FOR_FAMILY_DEFINITIONS

IF NoFamilyDefined
    AMD_DISABLE_STACK_FAMILY_HOOK_F15
ELSE
    IFDEF   OPTION_FAMILY15H
        AMD_DISABLE_STACK_FAMILY_HOOK_F15
    ENDIF
ENDIF

ENDM

;---------------------------------------------------
;
; GET_NODE_ID_CORE_ID Macro - Stackless
;
;   Read family specific values to determine the node and core
;   numbers for the core executing this code.
;
; Inputs:
;     none
; Outputs:
;     SI[7:0] = Core# (0..N, relative to node)
;     SI[15:8]= Node# (0..N)
;     SI[23:16]= reserved
;     SI[24]=   flag: 1=Family Unrecognized
;     SI[25]=   flag: 1=Interface re-entry call
;     SI[26]=   flag: 1=Core is primary of compute unit
;     SI[31:27]= reserved, =0
;
; Destroyed:
;       eax, ebx, ecx, edx, esi
;---------------------------------------------------
GET_NODE_ID_CORE_ID MACRO

    CHECK_FOR_FAMILY_DEFINITIONS

    mov     si, -1
IF NoFamilyDefined
    GET_NODE_ID_CORE_ID_F15
ELSE
    IFDEF   OPTION_FAMILY15H
        GET_NODE_ID_CORE_ID_F15
    ENDIF
ENDIF
      ;
      ; Check for unrecognized Family
      ;
    .if (si == -1)                      ; Has family (node/core) been discovered?
        mov     esi, ( (1 SHL FLAG_UNKNOWN_FAMILY)+(1 SHL FLAG_IS_PRIMARY) ) ; No, Set error code, Only let BSP continue
        mov     ecx, APIC_BASE_ADDRESS  ; MSR:0000_001B
        _RDMSR
        bt      eax, APIC_BSC           ;   Is this the BSC?
        .if (!carry?)
            ; No, this is an AP
            hlt                         ;       Kill APs
        .endif
    .endif
ENDM


;;***************************************************************************
;;                      Family 15h MACROS
;;***************************************************************************
;---------------------------------------------------
;
; AMD_ENABLE_STACK_FAMILY_HOOK_F15 Macro - Stackless
;
;   Set any family specific controls needed to enable the use of
;   cache as general storage before main memory is available.
;
; Inputs:
;       ESI - node#, core#, flags from GET_NODE_ID_CORE_ID
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;
; Family 15h TN/KV requirements:
;   * Paging must be disabled.
;   * MSRC001_0015[INVDWBINVD]=0
;   * MSRC001_1020[DisSS]=1
;   * MSRC001_1021[DisSpecTlb]=1
;   * MSRC001_1022[DisSpecTlb]=1
;   * MSRC001_1022[DisHwPf]=1
;   * No INVD or WBINVD, no exceptions, page faults or interrupts
;
; Family 15h CZ requirements:
;   * Paging must be disabled.
;   * MSRC001_0015[INVDWBINVD]=0
;   * MSRC001_101C[DisSS]=1
;   * MSRC001_1021[DisSpecTlb]=1
;   * MSRC001_101C[DisSpecTlb]=1
;   * MSRC001_101C[DisHwPf]=1
;   * No INVD or WBINVD, no exceptions, page faults or interrupts
;---------------------------------------------------
AMD_ENABLE_STACK_FAMILY_HOOK_F15 MACRO
    local   fam15_enable_stack_hook_exit

    AMD_CPUID   CPUID_MODEL
    mov     ebx, eax                    ; Save revision info to EBX
    shr     eax, 20                     ; AL = cpu extended family
    cmp     al, 06h                     ; Is this family 15h?
    jnz     fam15_enable_stack_hook_exit ; Br if no

    bt      esi, FLAG_STACK_REENTRY     ; Check if stack has already been set
    .if (!carry?)
        mov     ecx, HWCR               ; MSR C001_0015
        _RDMSR
        btr     eax, INVD_WBINVD        ; disable INVD -> WBINVD conversion
        _WRMSR
    .endif

    mov     eax, ebx                    ; Restore revision info to EAX
    shr     eax, 16
    and     al, 0Fh                     ; AL = cpu extended model
    .if (al == 06h)                     ; Is this CZ?
        mov     ecx, LS_CFG3                ; MSR:C001_101C
        _RDMSR
        bts     eax, DIS_SS_F15CZ           ; Turn on Streaming store functionality disabled bit
        _WRMSR

        mov     ecx, IC_CFG                 ; MSR:C001_1021
        _RDMSR
        bts     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on Disable speculative IC-TLB reloads bit
        _WRMSR

        mov     ecx, LS_CFG3                ; MSR:C001_101C
        _RDMSR
        bts     eax, DC_DIS_SPEC_TLB_RLD_F15CZ  ; Turn on Disable speculative DC-TLB reloads bit
        bts     eax, DC_DIS_HW_PF_F15CZ         ; Turn on Disable hardware prefetches bit
        _WRMSR
    .else
        mov     ecx, LS_CFG                 ; MSR:C001_1020
        _RDMSR
        bts     eax, DIS_SS                 ; Turn on Streaming store functionality disabled bit
        _WRMSR

        mov     ecx, IC_CFG                 ; MSR:C001_1021
        _RDMSR
        bts     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on Disable speculative IC-TLB reloads bit
        _WRMSR

        inc     ecx  ;DC_CFG                ; MSR:C001_1022

        _RDMSR
        bts     eax, DC_DIS_SPEC_TLB_RLD    ; Turn on Disable speculative DC-TLB reloads bit
        bts     eax, DIS_HW_PF              ; Turn on Disable hardware prefetches bit
        _WRMSR
    .endif

    mov     eax, ebx                    ; Restore revision info to EAX
    shr     eax, 16
    and     al, 0Fh                     ; AL = cpu extended model
    .if (al == 01h)                     ; Is this TN
        ; Enable MSRC001_001F[EnableCf8ExtCfg]
        mov     ecx, NB_CFG             ; MSR:C001_001F
        _RDMSR                          ; EDX has EnableCf8ExtCfg bit
        bts     edx, (ENABLE_CF8_EXT_CFG - 32)
        _WRMSR
    .endif

    ; Do TN/KV/CZ enable stack special
    mov     ecx, CU_CFG             ; MSR:C001_1023
    _RDMSR
    bt      eax, L2_WAY_LOCK_EN     ; Check if way 15 of the L2 needs to be reserved
    .if (!carry?)
        bts     eax, L2_WAY_LOCK_EN
        or      eax, L2_FIRST_LOCKED_WAY_OR_MASK
        _WRMSR
    .endif

    mov     eax, ebx                    ; Restore revision info to EAX
    xchg    al, ah
    shr     eax, 8                      ; AX = {ExtFamily, ExtModel, BaseModel, Stepping}
    .if (ax == 6100h)                   ; Is this TN-A0?
        ; Check if F3x1FC[21] is 0
        MAKE_EXT_PCI_ADDR   0, 0, 24, FUNC_3, PRODUCT_INFO_REG1
        mov     dx, 0CF8h               ; PCI Read
        out     dx, eax
        add     dx, 4
        in      eax, dx
        bt      eax, 21
        .if (!carry?)                   ; if F3x1FC[21] == 0?
            mov     ecx, CU_CFG2        ; MSR:C001_102A
            _RDMSR
            bts     eax, 8              ; Erratum#684. This bit should be set for TN-A0 with D18F3x1FC[21] = 0.
            _WRMSR
        .endif
    .endif

    ; Do Standard Family 15 work
    mov     ecx, CU_CFG3                ; MSR:C001_102B
    _RDMSR
    btr     edx, (COMBINE_CR0_CD - 32)  ; Clear CombineCr0Cd bit
    _WRMSR

fam15_enable_stack_hook_exit:
ENDM


;----------------------------------------------
;
; AMD_DISABLE_STACK_FAMILY_HOOK_F15 Macro - Stackless
;
;   Return any family specific controls to their 'standard'
;   settings for using cache with main memory.
;
; Inputs:
;       ESI - [31:24] flags; [15:8]= Node#; [7:0]= core#
; Outputs:
;       none
; Destroyed:
;       eax, ebx, ecx, edx
;
; Family 15h TN/KV requirements:
;   * INVD or WBINVD
;   * MSRC001_0015[INVD_WBINVD]=1
;   * MSRC001_1020[DisSS]=0
;   * MSRC001_1021[DisSpecTlb]=0
;   * MSRC001_1022[DisSpecTlb]=0
;   * MSRC001_1022[DisHwPf]=0
;
; Family 15h CZ requirements:
;   * INVD or WBINVD
;   * MSRC001_0015[INVD_WBINVD]=1
;   * MSRC001_101C[DisSS]=0
;   * MSRC001_1021[DisSpecTlb]=0
;   * MSRC001_101C[DisSpecTlb]=0
;   * MSRC001_101C[DisHwPf]=0
;---------------------------------------------------
AMD_DISABLE_STACK_FAMILY_HOOK_F15 MACRO
    local   fam15_disable_stack_hook_exit
    local   fam15_disable_stack_remote_read_exit

    AMD_CPUID   CPUID_MODEL
    mov     ebx, eax                    ; Save revision info to EBX
    shr     eax, 20                     ; AL = cpu extended family
    cmp     al, 06h                     ; Is this family 15h?
    jnz     fam15_disable_stack_hook_exit ; Br if no

    mov     edi, ebx                    ; Save revision info to EDI
    AMD_CPUID   AMD_CPUID_APIC
    mov     al, cl                      ; AL = number of cores - 1
    shr     cx, APIC_ID_CORE_ID_SIZE    ; CL = ApicIdCoreIdSize
    mov     bx, 1
    shl     bl, cl                      ; BL = theoretical number of cores on socket
    dec     bx                          ; BL = core number on socket mask
    mov     ah, bl                      ; AH = core number on socket mask
    mov     ebx, edi                    ; Restore revision info to EBX
    mov     di, ax                      ; DI[15:8] = core number mask, DI[7:0] = number of cores - 1

    and     ebx, 0F00FFh
    mov     eax, ebx
    shr     eax, 8
    or      bx, ax                      ; Save Extended Model, Model and Stepping to BX
                                        ; [11:8] = Extended Model, [7:4] = Model, [3:0] = Stepping

    ;; A handshake is required to ensure that all cores on a node invalidate in sync.
    mov     ecx, APIC_BASE_ADDRESS
    _RDMSR
    mov     dx, bx                      ; Save Extended Model, Model and Stepping to DX
    shl     edx, 16                     ; EDX[31:16] = Extended Model, Model and Stepping
    mov     ebx, eax                    ; EBX = LAPIC base
    xor     ecx, ecx                    ; Zero out CU flags
    bts     ecx, AMD_CU_NEED_TO_WAIT    ; Default to waiting
    bts     ecx, AMD_CU_SEND_INVD_MSG   ; Default to signaling
    mov     eax, CR0
    bt      ax, CR0_PE                  ; Are we in protected mode?
    .if (!carry?)
        bts     ecx, AMD_CU_RESTORE_ES  ; Indicate ES restore is required
        mov     cx, es                  ; Save ES segment register to CX
        xor     ax, ax
        mov     es, ax                  ; Set ES to big real mode selector for 4GB access
    .endif

    and     bx, 0F000h                  ; EBX = LAPIC base, offset 0
    or      bl, APIC_ID_REG
    mov     eax, es:[ebx]               ; EAX[31:24] = APIC ID
    shr     eax, APIC20_APICID          ; AL = APIC ID
    mov     ah, al                      ; AH = APIC ID
    mov     dx, di                      ; DH = core mask
    and     ah, dh                      ; AH = core number
    .if (zero?)
        ;; Core 0 of a socket
        btr     ecx, AMD_CU_SEND_INVD_MSG ; No need to signal after INVD
        .if (dl != 0)
            ;; This socket has multiple cores
            and     bx, 0F000h          ; EBX = LAPIC base, offset 0
            or      bx, APIC_MSG_REG
            mov     edi, APIC_MSG
            mov     es:[ebx], edi       ; Signal for non core 0s to complete CAR breakdown
        .else
            btr     ecx, AMD_CU_NEED_TO_WAIT ; No need to wait on a single core CPU
        .endif
    .endif

    bt     ecx, AMD_CU_NEED_TO_WAIT
    .if (carry?)
        .if (ah == dl)
            ;; This is the highest numbered core on this socket -- wait on core 0
            not     dh                  ; Flip the mask to determine local core 0's APIC ID
            and     al, dh              ; AL = target APIC ID
        .else
            ;; All other cores (including core 0) wait on the next highest core.
            ;; In this way, cores will halt in a cascading fashion down to 0.
            inc     al
        .endif

        shl     eax, APIC20_APICID
        and     bx, 0F000h
        or      bx, APIC_CMD_HI_REG
        mov     es:[ebx], eax           ; Set target APIC ID

        ;; Use bits 23:16 as a timeout for unresponsive cores
        ror     ecx, 8
        mov     ch, 0FFh
        stc
        .while (carry?)
            and     bx, 0F000h          ; EBX = LAPIC base, offset 0
            or      bx, APIC_CMD_LO_REG
            mov     eax, CMD_REG_TO_READ_DATA
            mov     es:[ebx], eax       ; Fire remote read IPI
            inc     ch                  ; Pre increment the timeout
            stc
            .while (carry?)
                dec     ch              ; Check the timeout
                jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                mov     eax, es:[ebx]
                bt      eax, DELIVERY_STS_BIT
            .endw
            stc
            .while (carry?)
                mov     eax, es:[ebx]
                and     eax, REMOTE_READ_STS
                .if (eax == REMOTE_DELIVERY_PEND)
                    dec     ch          ; Check the timeout
                    jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                    stc
                .else
                    clc
                .endif
            .endw
            .if (eax == REMOTE_DELIVERY_DONE)
                and     bx, 0F000h      ; EBX = LAPIC base, offset 0
                or      bl, APIC_REMOTE_READ_REG
                mov     eax, es:[ebx]
                .if (eax == APIC_MSG)
                    clc
                .else
                    stc
                .endif
            .else
                dec     ch              ; Check the timeout
                jz      fam15_disable_stack_remote_read_exit   ; Branch if there is an unresponsive core
                stc
            .endif
        .endw

fam15_disable_stack_remote_read_exit:
        rol     ecx, 8                  ; Restore ECX

    .endif
    bt      ecx, AMD_CU_RESTORE_ES
    .if (carry?)
        mov     es, cx
    .endif
    mov     edi, ecx                    ; EDI = CU flags
    shr     edx, 16
    mov     bx, dx                      ; Restore Extended Model, Model and Stepping

    ;; Handshaking complete.  Continue tearing down CAR.
    .if (bh == 06h)                     ; Is this CZ?
        mov     ecx, LS_CFG3                ; MSR:C001_101C
        _RDMSR
        btr     eax, DIS_SS_F15CZ           ; Turn on Streaming store functionality
        _WRMSR

        mov     ecx, IC_CFG                 ; MSR:C001_1021
        _RDMSR
        btr     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on speculative TLB reloads
        _WRMSR

        mov     ecx, LS_CFG3                ; MSR:C001_101C
        _RDMSR
        btr     eax, DC_DIS_SPEC_TLB_RLD_F15CZ  ; Turn on speculative TLB reloads
        btr     eax, DC_DIS_HW_PF_F15CZ         ; Turn on hardware prefetches
        _WRMSR
    .else
        mov     ecx, LS_CFG                 ; MSR:C001_1020
        _RDMSR
        btr     eax, DIS_SS                 ; Turn on Streaming store functionality
        _WRMSR

        mov     ecx, IC_CFG                 ; MSR:C001_1021
        _RDMSR
        btr     eax, IC_DIS_SPEC_TLB_RLD    ; Turn on speculative TLB reloads
        _WRMSR

        inc     ecx  ;DC_CFG                ; MSR:C001_1022
        _RDMSR
        btr     eax, DC_DIS_SPEC_TLB_RLD    ; Turn on speculative TLB reloads
        btr     eax, DIS_HW_PF              ; Turn on hardware prefetches
        _WRMSR
    .endif

    mov     ecx, HWCR                   ; MSR:C001_0015h
    _RDMSR
    btr     eax, INVD_WBINVD            ; Disable INVD -> WBINVD conversion
    _WRMSR
    invd                                ; Clear the cache tag RAMs

    ; Do TN/KV/CZ disable stack special
    mov     ecx, CU_CFG             ; MSR:C001_1023
    _RDMSR
    shr      eax, L2_FIRST_LOCKED_WAY
    and      eax, 01Fh
    .if (eax == 01Fh)               ; Check if way 15 of the L2 needs to be reserved
        _RDMSR                      ; ECX = CU_CFG, MSR:C001_1023
        btr     eax, L2_WAY_LOCK_EN
        _WRMSR
    .endif

    ; Do Standard Family 15 work
    mov     ecx, HWCR                   ; MSR:C001_0015h
    _RDMSR
    bts     eax, INVD_WBINVD            ; Turn on INVD -> WBINVD conversion
    _WRMSR

    mov     ecx, CU_CFG3                ; MSR:C001_102B
    _RDMSR
    bts     edx, (COMBINE_CR0_CD - 32)  ; Set CombineCr0Cd bit
    _WRMSR

    bt      edi, AMD_CU_SEND_INVD_MSG
    .if (carry?)
        ;; Non core zero needs to signal to core 0 to proceed
        mov     ecx, APIC_BASE_ADDRESS
        _RDMSR
        mov     ebx, eax                ; EBX = LAPIC base
        and     bx, 0F000h              ; EBX = LAPIC base, offset 0
        or      bx, APIC_MSG_REG
        mov     eax, APIC_MSG
        mov     es:[ebx], eax           ; Signal for core 0 to complete CAR breakdown
    .endif

fam15_disable_stack_hook_exit:
ENDM


;---------------------------------------------------
;
; GET_NODE_ID_CORE_ID_F15 Macro - Stackless
;
;   Read family specific values to determine the node and core
;   numbers for the core executing this code.
;
; Inputs:
;     none
; Outputs:
;     ESI = core#, node# & flags (see GET_NODE_ID_CORE_ID macro above)
;     MM5 = 32b pointer to family info structure
; Destroyed:
;       eax, ebx, ecx, edx, esi, mm5
;---------------------------------------------------
GET_NODE_ID_CORE_ID_F15 MACRO

    local   node_core_f15_exit
    local   end_of_f15h_data

    IFNDEF FAM15H_INFO_STRUCT
        jmp     end_of_f15h_data
        ; Family 15h Info Structure:          L2Size,  #SharedCores, AllocMem, AllocExe, SzAddrBus, pad
        FAM15H_INFO_STRUCT  CPU_FAMILY_INFO {  512,         2,          0,     0,         48,         0    }
end_of_f15h_data:
    ENDIF

    cmp     si, -1                      ; Has node/core already been discovered?
    jnz     node_core_f15_exit          ; Br if yes

    AMD_CPUID   CPUID_MODEL
    shr     eax, 12                     ; AH = cpu extended family
    cmp     ah, 06h                     ; Is this family 15h?
    jnz     node_core_f15_exit          ; Br if no
    shr     al, 4                       ; AL = cpu extended model
    shr     ebx, 16                     ; BH = LocalApicId
    mov     bl, al                      ; BL = cpu extended model

    LoadTableAddress(FAM15H_INFO_STRUCT)
    movd    mm5, eax                    ; load pointer to Family Info Struc

    xor     esi, esi                    ; Assume BSC, clear local flags
    mov     ecx, APIC_BASE_ADDRESS      ; MSR:0000_001B
    _RDMSR
    bt      eax, APIC_BSC               ; Is this the BSC?
    .if (carry?)
        ; This is the BSP.
        ; Enable routing tables on BSP (just in case the topology init code has not yet enabled them)
        mov     eax, 8000C06Ch          ;   PCI address for D18F0x6C Link Initialization Control Register
        mov     dx, 0CF8h
        out     dx, eax
        add     dx, 4
        in      eax, dx
        btr     eax, 0                  ;   Set LinkInitializationControl[RouteTblDis] = 0
        out     dx, eax
    .else                               ;
        mov     al, bl                  ;   AL = cpu extended model
        shr     bx, 8                   ;   BL = CPUID Fn0000_0001_EBX[LocalApicId]
        mov si, bx                      ;   SI = [15:8]= Node# = 0; [7:0]= core#
    .endif                              ; end

    ;
    ;   determine if this core shares MTRRs
    ;
    mov     eax, 8000C580h              ; Compute Unit Status
    mov     bx, si                      ; load node#(bh), core#(bl)
    shl     bh, 3                       ; Move node# to PCI Dev# field
    add     ah, bh                      ; Adjust PCI address for node number
    mov     dx, 0CF8h
    out     dx, eax
    add     dx, 4
    in      eax, dx                     ; [3:0]=Enabled; [19:16]=DualCore

                                        ; BL is MyCore#  , BH is primary flag
    mov     cx, 08h                     ; Use CH as 'first of pair' core#
    .while (cl > 0)
        bt      eax, 0                  ;   Is pair enabled?
        .if (carry?)                    ;
            mov     bh, 01h             ;   flag core as primary
            bt      eax, 16             ;   Is there a 2nd in the pair?
            .if (carry?)                ;
                .break .if (ch == bl)   ;     Does 1st match MyCore#?
                inc     ch
                xor     bh, bh          ;     flag core as NOT primary
                .break .if (ch == bl)   ;     Does 2nd match MyCore#?
            .else                       ;   No 2nd core
                .break .if (ch == bl)   ;     Does 1st match MyCore#?
            .endif
            inc     ch
        .endif
        shr     eax, 1
        dec     cl
    .endw
    .if (cl == 0)
        ;Error - core# didn't match Compute Unit Status content
        bts     esi, FLAG_CORE_NOT_IDENTIFIED
        bts     esi, FLAG_IS_PRIMARY    ;   Set Is_Primary for unknowns
    .endif
    .if (bh != 0)                       ; Check state of primary for the matched core
        bts     esi, FLAG_IS_PRIMARY    ;   Set shared flag into return value
    .endif
node_core_f15_exit:
ENDM
