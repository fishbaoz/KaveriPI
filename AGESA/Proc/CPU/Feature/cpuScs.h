/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA CPU Software Configuration Settings (SCS) feature support code.
 *
 * Contains code that declares the AGESA CPU SCS related APIs
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Feature
 * @e \$Revision: 284995 $   @e \$Date: 2014-02-17 11:17:41 -0600 (Mon, 17 Feb 2014) $
 *
 */
/*
 ******************************************************************************
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

#ifndef _CPU_SCS_H_
#define _CPU_SCS_H_

/*----------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *----------------------------------------------------------------------------------------
 */
//  Forward declaration needed for multi-structure mutual references
AGESA_FORWARD_DECLARATION (SCS_FAMILY_SERVICES);

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */

typedef double DOUBLE;

#define AGESA_SCS_HEAP_ENTRY_MISSING          1
#define AGESA_SCS_BUFFER_EMPTY                2
#define AGESA_SCS_WEIGHTS_MISMATCH            3

#define HEADROOM    25

/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 *
 * This structure is used by the SMU to load the x86 core data into SMU RAM.
 *  The SMU will scan the SCS tables block passed from the BIOS ROM image for
 *  the table(s) that match the installed processor, then load that table(s)
 *  into this SMU-RAM based structure. The x86 will receive a pointer to this
 *  structure via an SMU command-response sequence.
 *
 * The Normalizer values will be generated by the SMU firmware.
 * The APMWeights values are passed from the BIOS ROM image SCS tables.
 ******************************************************************************
 */

/// Packed Weights
typedef struct {
  UINT32      Weight;                          ///< weight
} PACKED_WEIGHTS;

/// Smu Cpu Information
typedef struct {
  DOUBLE          CountConstant;               ///< copied from SCS table
  DOUBLE          BoostConstant;               ///< copied from SCS table
  UINT32          CountApmWeights;             ///< length of APMWeights array
  PACKED_WEIGHTS  MSRApmWeights[1];            ///< Weights from SCS table, in MSR packed format
} SMU_RAM_CPU_INFO;

/// Smu Cpu Weights Information
typedef struct {
  UINT32          CountApmWeights;             ///< length of APMWeights array
  PACKED_WEIGHTS  MSRApmWeights[1];            ///< Weights from SCS table, in MSR packed format
} SMU_RAM_CPU_WEIGHTS_INFO;

/// Exponent and Mantissa to be passed to cores
typedef struct {
  UINT32          Exponent:8;                  ///< Exponent
  UINT32          Mantissa:8;                  ///< Mantissa
} NORMALIZER;

/* MSR Register 0xC0011073 */
#define MSR_C001_1073 0xC0011073ul

/// C001_1073 MSR Register
typedef struct {
  UINT64 :24;                        ///< Reserved
  UINT64 Base:8;                     ///< Base
  UINT64 :10;                        ///< Reserved
  UINT64 Mantissa:5;                 ///< Mantissa
  UINT64 Exponent:5;                 ///< Exponent
  UINT64 :10;                        ///< Reserved
  UINT64 Lock:1;                     ///< Lock
  UINT64 Ready:1;                    ///< Ready
} C001_1073_MSR;

/* Sample and Residency Timers Register D18F4x110 */
#define SAMPLE_AND_RESIDENCY_TIMER_REG 0x110
#define SAMPLE_AND_RESIDENCY_TIMER_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, SAMPLE_AND_RESIDENCY_TIMER_REG))

/// Sample and Residency Timers Register
typedef struct {
  UINT32 CSampleTimer:12;            ///< CSampleTimer
  UINT32 :20;                        ///< reserved
} SAMPLE_AND_RESIDENCY_TIMER_REGISTER;

/* Kv fuse register for CSampleTimer */
#define EXTENDED_PRODUCT_INFORMATION_REG 0x1D8
#define EXTENDED_PRODUCT_INFORMATION_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, EXTENDED_PRODUCT_INFORMATION_REG))
/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to check if Software Configuration Settings (SCS) is supported.
 *
 * @param[in]    ScsServices        SCS services.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               SCS is supported.
 * @retval       FALSE              SCS is not supported.
 *
 */
typedef BOOLEAN F_SCS_IS_SUPPORTED (
  IN       SCS_FAMILY_SERVICES    *ScsServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_SCS_IS_SUPPORTED *PF_SCS_IS_SUPPORTED;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to program the weights.
 *
 * @param[in]    ScsServices        SCS services.
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Family specific error value.
 *
 */
typedef AGESA_STATUS F_SCS_P_WEIGHTS (
  IN       SMU_RAM_CPU_INFO       *SmuCpuInfoPtr,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_SCS_P_WEIGHTS *PF_SCS_P_WEIGHTS;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call get P0 frequency in MHz
 *
 * @param[out]   P0FrequencyInMhz   Frequency return value.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 *
 */
typedef VOID F_SCS_G_P0_FREQ (
     OUT   UINT32                 *P0FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/// Reference to a Method.
typedef F_SCS_G_P0_FREQ *PF_SCS_G_P0_FREQ;

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to write the normalizer.
 *
 * @param[in]    ScsServices        SCS services.
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @return       Family specific error value.
 *
 */
typedef VOID F_SCS_W_NORMALIZER (
  IN       UINT8               FiveBitExponent,
  IN       UINT8               FiveBitMantissa,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

/// Reference to a Method.
typedef F_SCS_W_NORMALIZER *PF_SCS_W_NORMALIZER;

/**
 * Provide the interface to the SCS Family Specific Services.
 *
 * Use the methods or data in this struct to adapt the feature code to a specific cpu family or model (or stepping!).
 * Each supported Family must provide an implementation for all methods in this interface, even if the
 * implementation is a CommonReturn().
 */
typedef struct _SCS_FAMILY_SERVICES {
  UINT16          Revision;                                             ///< Interface version
  // Public Methods.
  PF_SCS_IS_SUPPORTED IsScsSupported;                                   ///< Method: Family specific call to check if SCS is supported.
  PF_SCS_G_P0_FREQ    GetP0Frequency;                                   ///< Method: Family specific call to return the frequency of P-state 0.
  PF_SCS_P_WEIGHTS    ProgramWeights;                                   ///< Method: Family specific call to program weights.
  PF_SCS_W_NORMALIZER WriteNormalizer;                                  ///< Method: Family specific call to write normalizer.
} SCS_FAMILY_SERVICES;


/*----------------------------------------------------------------------------------------
 *                          F U N C T I O N S     P R O T O T Y P E
 *----------------------------------------------------------------------------------------
 */

#endif  // _CPU_SCS_H_
