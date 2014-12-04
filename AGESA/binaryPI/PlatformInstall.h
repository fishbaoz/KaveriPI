/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build options for a combination of package type, processor, and features.
 *
 * This file generates the defaults tables for the all platform solution
 * combinations.  The documented build options are imported from a user
 * controlled file for processing.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 290085 $   @e \$Date: 2014-04-18 13:53:57 -0500 (Fri, 18 Apr 2014) $
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
 *
 ***************************************************************************/

/*****************************************************************************
 *
 * Start processing the user options:  First, set default settings
 *
 ****************************************************************************/

/* Available options for image builds.
 *
 * As part of the image build for each image, define the options below to select the
 * AGESA entry points included in that image.  Turn these on in your option c file, not
 * here.
 */
// #define AGESA_ENTRY_INIT_RESET                    TRUE
// #define AGESA_ENTRY_INIT_RECOVERY                 TRUE
// #define AGESA_ENTRY_INIT_EARLY                    TRUE
// #define AGESA_ENTRY_INIT_POST                     TRUE
// #define AGESA_ENTRY_INIT_ENV                      TRUE
// #define AGESA_ENTRY_INIT_MID                      TRUE
// #define AGESA_ENTRY_INIT_LATE                     TRUE
// #define AGESA_ENTRY_INIT_S3SAVE                   TRUE
// #define AGESA_ENTRY_INIT_RESUME                   TRUE
// #define AGESA_ENTRY_INIT_LATE_RESTORE             TRUE
// #define AGESA_ENTRY_INIT_GENERAL_SERVICES         TRUE

/*  Defaults for private/internal build control settings  */
/* Available options for image builds.
 *
 * As part of the image build for each image, define the options below to select the
 * AGESA entry points included in that image.
 */

VOLATILE  AMD_MODULE_HEADER mCpuModuleID = {
  //ModuleHeaderSignature
  // Remove 'DOM$' as temp solution before update BinUtil.exe ,
  '0000',
  //ModuleIdentifier[8]
  AGESA_ID,
  //ModuleVersion[12]
  AGESA_VERSION_STRING,
  //ModuleDispatcher
  NULL,//(VOID *)(UINT64)((MODULE_ENTRY)AmdAgesaDispatcher),
  //NextBlock
  NULL
};

/*  Process user desired AGESA entry points  */
#ifndef AGESA_ENTRY_INIT_RESET
  #define AGESA_ENTRY_INIT_RESET                     FALSE
#endif

#ifndef AGESA_ENTRY_INIT_RECOVERY
  #define AGESA_ENTRY_INIT_RECOVERY                  FALSE
#endif

#ifndef AGESA_ENTRY_INIT_EARLY
  #define AGESA_ENTRY_INIT_EARLY                     FALSE
#endif

#ifndef AGESA_ENTRY_INIT_POST
  #define AGESA_ENTRY_INIT_POST                      FALSE
#endif

#ifndef AGESA_ENTRY_INIT_ENV
  #define AGESA_ENTRY_INIT_ENV                       FALSE
#endif

#ifndef AGESA_ENTRY_INIT_MID
  #define AGESA_ENTRY_INIT_MID                       FALSE
#endif

#ifndef AGESA_ENTRY_INIT_LATE
  #define AGESA_ENTRY_INIT_LATE                      FALSE
#endif

#ifndef AGESA_ENTRY_INIT_S3SAVE
  #define AGESA_ENTRY_INIT_S3SAVE                    FALSE
#endif

#ifndef AGESA_ENTRY_INIT_RESUME
  #define AGESA_ENTRY_INIT_RESUME                    FALSE
#endif

#ifndef AGESA_ENTRY_INIT_LATE_RESTORE
  #define AGESA_ENTRY_INIT_LATE_RESTORE              FALSE
#endif

#ifndef AGESA_ENTRY_INIT_GENERAL_SERVICES
  #define AGESA_ENTRY_INIT_GENERAL_SERVICES          FALSE
#endif

#ifndef AGESA_ENTRY_INIT_FINAL_RESTORE
  #define AGESA_ENTRY_INIT_FINAL_RESTORE             FALSE
#endif

/*  Default the late AP entry point to off.  It can be enabled
    by any family that may need the late AP functionality, or
    by any feature code that may need it.  The IBVs no longer
    have control over this entry point.  */
#ifdef AGESA_ENTRY_LATE_RUN_AP_TASK
  #undef AGESA_ENTRY_LATE_RUN_AP_TASK
#endif
#define AGESA_ENTRY_LATE_RUN_AP_TASK    FALSE



/* Process solution defined socket / family installations
 *
 * As part of the release package for each image, define the options below to select the
 * AGESA processor support included in that image.
 */

/*  Default sockets to off  */
#define OPTION_FS1_SOCKET_SUPPORT    FALSE
#define OPTION_FM2r2_SOCKET_SUPPORT  FALSE
#define OPTION_FM2_SOCKET_SUPPORT    FALSE
#define OPTION_FP2_SOCKET_SUPPORT    FALSE
#define OPTION_FP3_SOCKET_SUPPORT    FALSE

/*  Default families to off  */
#define OPTION_FAMILY15H_MODEL_1x    FALSE
#define OPTION_FAMILY15H_MODEL_3x    FALSE


/*  Enable the appropriate socket support  */
#ifdef INSTALL_FS1_SOCKET_SUPPORT
  #if  INSTALL_FS1_SOCKET_SUPPORT == TRUE
    #undef OPTION_FS1_SOCKET_SUPPORT
    #define OPTION_FS1_SOCKET_SUPPORT  TRUE
  #endif
#endif

#ifdef INSTALL_FM2r2_SOCKET_SUPPORT
  #if  INSTALL_FM2r2_SOCKET_SUPPORT == TRUE
    #undef OPTION_FM2r2_SOCKET_SUPPORT
    #define OPTION_FM2r2_SOCKET_SUPPORT  TRUE
  #endif
#endif

#ifdef INSTALL_FM2_SOCKET_SUPPORT
  #if  INSTALL_FM2_SOCKET_SUPPORT == TRUE
    #undef OPTION_FM2_SOCKET_SUPPORT
    #define OPTION_FM2_SOCKET_SUPPORT  TRUE
  #endif
#endif

#ifdef INSTALL_FP2_SOCKET_SUPPORT
  #if  INSTALL_FP2_SOCKET_SUPPORT == TRUE
    #undef OPTION_FP2_SOCKET_SUPPORT
    #define OPTION_FP2_SOCKET_SUPPORT  TRUE
  #endif
#endif

#ifdef INSTALL_FP3_SOCKET_SUPPORT
  #if  INSTALL_FP3_SOCKET_SUPPORT == TRUE
    #undef OPTION_FP3_SOCKET_SUPPORT
    #define OPTION_FP3_SOCKET_SUPPORT  TRUE
  #endif
#endif


/*  Enable the appropriate family support  */
// F15_1x is supported in FS1r2, FM2, FP2 & FM2r2
#ifdef INSTALL_FAMILY_15_MODEL_1x_SUPPORT
  #if  INSTALL_FAMILY_15_MODEL_1x_SUPPORT == TRUE
    #undef OPTION_FAMILY15H
    #define OPTION_FAMILY15H     TRUE
    #undef OPTION_FAMILY15H_MODEL_1x
    #define OPTION_FAMILY15H_MODEL_1x     TRUE
  #endif
#endif

// F15_3x is supported in FP3 & FM2r2
#ifdef INSTALL_FAMILY_15_MODEL_3x_SUPPORT
  #if  INSTALL_FAMILY_15_MODEL_3x_SUPPORT == TRUE
    #undef OPTION_FAMILY15H
    #define OPTION_FAMILY15H     TRUE
    #undef OPTION_FAMILY15H_MODEL_3x
    #define OPTION_FAMILY15H_MODEL_3x     TRUE
  #endif
#endif

/*  Turn off families not required by socket designations */
#if (OPTION_FAMILY15H_MODEL_1x == TRUE)
  #if (OPTION_FS1_SOCKET_SUPPORT == FALSE) && (OPTION_FM2_SOCKET_SUPPORT == FALSE) && (OPTION_FP2_SOCKET_SUPPORT == FALSE) \
      && (OPTION_FM2r2_SOCKET_SUPPORT == FALSE)
    #undef OPTION_FAMILY15H_MODEL_1x
    #define OPTION_FAMILY15H_MODEL_1x  FALSE
  #endif
#endif

#if (OPTION_FAMILY15H_MODEL_3x == TRUE)
  #if (OPTION_FP3_SOCKET_SUPPORT == FALSE) && (OPTION_FM2r2_SOCKET_SUPPORT == FALSE)
    #undef OPTION_FAMILY15H_MODEL_3x
    #define OPTION_FAMILY15H_MODEL_3x  FALSE
  #endif
#endif

#if (OPTION_FAMILY15H_MODEL_1x == FALSE) && (OPTION_FAMILY15H_MODEL_3x == FALSE)
  #undef OPTION_FAMILY15H
  #define OPTION_FAMILY15H     FALSE
#endif

/*  Check for invalid combinations of socket/family */
#if (OPTION_FM2r2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_3x == FALSE) && (OPTION_FAMILY15H_MODEL_1x == FALSE)
    #error No FM2r2 supported families included in the build
  #endif
#endif

#if (OPTION_FM2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_1x == FALSE)
    #error No FM2 supported families included in the build
  #endif
#endif

#if (OPTION_FP2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_1x == FALSE)
    #error No FP2 supported families included in the build
  #endif
#endif

#if (OPTION_FP3_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_3x == FALSE)
    #error No FP3 supported families included in the build
  #endif
#endif


/* Process AGESA private data
 *
 * Turn on appropriate CPU models and memory controllers,
 * as well as some other memory controls.
 */

/*  Default all models to off  */
#define OPTION_FAMILY15H_TN          FALSE
#define OPTION_FAMILY15H_KV          FALSE
#define OPTION_FAMILY15H_UNKNOWN     FALSE

/*  Default all memory controllers to off  */
#define OPTION_MEMCTLR_TN            FALSE
#define OPTION_MEMCTLR_KV            FALSE

/*  Default all memory controls to off  */
#define OPTION_HW_WRITE_LEV_TRAINING            FALSE
#define OPTION_SW_WRITE_LEV_TRAINING            FALSE
#define OPTION_CONTINOUS_PATTERN_GENERATION     FALSE
#define OPTION_HW_DQS_REC_EN_TRAINING           FALSE
#define OPTION_NON_OPT_SW_DQS_REC_EN_TRAINING   FALSE
#define OPTION_OPT_SW_DQS_REC_EN_TRAINING       FALSE
#define OPTION_NON_OPT_SW_RD_WR_POS_TRAINING    FALSE
#define OPTION_OPT_SW_RD_WR_POS_TRAINING        FALSE
#define OPTION_MAX_RD_LAT_TRAINING              FALSE
#define OPTION_HW_DRAM_INIT                     FALSE
#define OPTION_SW_DRAM_INIT                     FALSE
#define OPTION_S3_MEM_SUPPORT                   FALSE
#define OPTION_ADDR_TO_CS_TRANSLATOR            FALSE
#define OPTION_HW_DQS_REC_EN_SEED_TRAINING      FALSE
#define OPTION_RDDQS_2D_TRAINING                FALSE
#define OPTION_DLLPDBYPASS                      FALSE
#define OPTION_PRE_MEM_INIT                     FALSE
#define OPTION_POST_MEM_INIT                    FALSE

/*  Defaults for public user options  */
#define OPTION_UDIMMS                           FALSE
#define OPTION_RDIMMS                           FALSE
#define OPTION_SODIMMS                          FALSE
#define OPTION_LRDIMMS                          FALSE
#define OPTION_DDR2                             FALSE
#define OPTION_DDR3                             FALSE
#define OPTION_GDDR5                            FALSE
#define OPTION_ECC                              FALSE
#define OPTION_BANK_INTERLEAVE                  FALSE
#define OPTION_DCT_INTERLEAVE                   FALSE
#define OPTION_NODE_INTERLEAVE                  FALSE
#define OPTION_PARALLEL_TRAINING                FALSE
#define OPTION_ONLINE_SPARE                     FALSE
#define OPTION_MEM_RESTORE                      FALSE
#define OPTION_DIMM_EXCLUDE                     FALSE
#define OPTION_AMP                              FALSE
#define OPTION_DATA_EYE                         FALSE
#define OPTION_AGGRESSOR                        FALSE

/*  Default all CPU controls to off  */
#define OPTION_MULTISOCKET                      FALSE
#define OPTION_CRAT                             FALSE
#define OPTION_CDIT                             FALSE
#define OPTION_SRAT                             FALSE
#define OPTION_SLIT                             FALSE
#define OPTION_CPU_CORELEVELING                 FALSE
#define OPTION_CPU_CFOH                         FALSE
#define OPTION_C6_STATE                         FALSE
#define OPTION_IO_CSTATE                        FALSE
#define OPTION_CPB                              FALSE
#define OPTION_CPU_APM                          FALSE
#define OPTION_CPU_LOW_PWR_PSTATE_FOR_PROCHOT   FALSE
#define OPTION_CPU_PSI                          FALSE
#define OPTION_CPU_HTC                          FALSE
#define OPTION_S3SCRIPT                         FALSE
#define OPTION_GFX_RECOVERY                     FALSE
#define OPTION_CPU_SCS                          FALSE
#define OPTION_PREFETCH_MODE                    FALSE
#define OPTION_CONNECTED_STANDBY                FALSE
#define OPTION_CC6_EXIT_CONTROL                 FALSE

/*  Default FCH controls to off  */
#define FCH_SUPPORT                             FALSE

/*  Enable all private controls based on socket/family enables  */
#if (OPTION_FS1_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_1x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_TN
    #define OPTION_FAMILY15H_TN  TRUE
    #undef OPTION_MEMCTLR_TN
    #define OPTION_MEMCTLR_TN    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  TRUE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  TRUE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      TRUE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  TRUE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  FALSE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  TRUE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  TRUE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CPU_CORELEVELING
    #define OPTION_CPU_CORELEVELING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  TRUE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
  #endif
#endif

#if (OPTION_FM2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_1x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_TN
    #define OPTION_FAMILY15H_TN  TRUE
    #undef OPTION_MEMCTLR_TN
    #define OPTION_MEMCTLR_TN    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  TRUE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  TRUE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      TRUE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  TRUE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  TRUE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  TRUE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  TRUE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_CPU_CORELEVELING
    #define OPTION_CPU_CORELEVELING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  TRUE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
  #endif
#endif

#if (OPTION_FP2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_1x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_TN
    #define OPTION_FAMILY15H_TN  TRUE
    #undef OPTION_MEMCTLR_TN
    #define OPTION_MEMCTLR_TN    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  TRUE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  TRUE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      TRUE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  TRUE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  FALSE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  TRUE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  TRUE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_ADDR_TO_CS_TRANSLATOR
    #define OPTION_ADDR_TO_CS_TRANSLATOR  TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_CPU_CORELEVELING
    #define OPTION_CPU_CORELEVELING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  TRUE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
  #endif
#endif

#if (OPTION_FP3_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_3x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_KV
    #define OPTION_FAMILY15H_KV  TRUE
    #undef OPTION_MEMCTLR_KV
    #define OPTION_MEMCTLR_KV    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  FALSE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  FALSE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      FALSE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  FALSE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  FALSE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  FALSE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  FALSE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_PRE_MEM_INIT
    #define OPTION_PRE_MEM_INIT    TRUE
    #undef OPTION_POST_MEM_INIT
    #define OPTION_POST_MEM_INIT   TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CPU_CORELEVELING
    #define OPTION_CPU_CORELEVELING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_CPU_SCS
    #define OPTION_CPU_SCS TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_GDDR5
    #define OPTION_GDDR5  TRUE
    #undef OPTION_ECC
    #define OPTION_ECC  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  FALSE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_CC6_EXIT_CONTROL
    #define OPTION_CC6_EXIT_CONTROL TRUE
  #endif
#endif

#if (OPTION_FM2r2_SOCKET_SUPPORT == TRUE)
  #if (OPTION_FAMILY15H_MODEL_3x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_KV
    #define OPTION_FAMILY15H_KV  TRUE
    #undef OPTION_MEMCTLR_KV
    #define OPTION_MEMCTLR_KV    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  FALSE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  FALSE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      FALSE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  FALSE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  FALSE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  FALSE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  FALSE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_PRE_MEM_INIT
    #define OPTION_PRE_MEM_INIT    TRUE
    #undef OPTION_POST_MEM_INIT
    #define OPTION_POST_MEM_INIT   TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CPU_CORELEVELING
    #define OPTION_CPU_CORELEVELING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_CPU_SCS
    #define OPTION_CPU_SCS TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_GDDR5
    #define OPTION_GDDR5  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  FALSE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_CC6_EXIT_CONTROL
    #define OPTION_CC6_EXIT_CONTROL TRUE
  #endif
  #if (OPTION_FAMILY15H_MODEL_1x == TRUE)
    #undef FCH_SUPPORT
    #define FCH_SUPPORT          TRUE
    #undef OPTION_FAMILY15H_TN
    #define OPTION_FAMILY15H_TN  TRUE
    #undef OPTION_MEMCTLR_TN
    #define OPTION_MEMCTLR_TN    TRUE
    #undef OPTION_HW_WRITE_LEV_TRAINING
    #define OPTION_HW_WRITE_LEV_TRAINING  TRUE
    #undef OPTION_CONTINOUS_PATTERN_GENERATION
    #define OPTION_CONTINOUS_PATTERN_GENERATION  TRUE
    #undef OPTION_HW_DQS_REC_EN_TRAINING
    #define OPTION_HW_DQS_REC_EN_TRAINING  TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      TRUE
    #undef OPTION_OPT_SW_RD_WR_POS_TRAINING
    #define OPTION_OPT_SW_RD_WR_POS_TRAINING  TRUE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING  FALSE
    #undef OPTION_MAX_RD_LAT_TRAINING
    #define OPTION_MAX_RD_LAT_TRAINING  TRUE
    #undef OPTION_SW_DRAM_INIT
    #define OPTION_SW_DRAM_INIT  TRUE
    #undef OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT  TRUE
    #undef OPTION_ADDR_TO_CS_TRANSLATOR
    #define OPTION_ADDR_TO_CS_TRANSLATOR  TRUE
    #undef OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY  TRUE
    #undef OPTION_CRAT
    #define OPTION_CRAT  TRUE
    #undef OPTION_CDIT
    #define OPTION_CDIT  TRUE
    #undef OPTION_CPU_HTC
    #define OPTION_CPU_HTC TRUE
    #undef OPTION_CPU_CORELEVLING
    #define OPTION_CPU_CORELEVLING  TRUE
    #undef OPTION_C6_STATE
    #define OPTION_C6_STATE  TRUE
    #undef OPTION_IO_CSTATE
    #define OPTION_IO_CSTATE TRUE
    #undef OPTION_CPB
    #define OPTION_CPB  TRUE
    #undef OPTION_CPU_PSI
    #define OPTION_CPU_PSI TRUE
    #undef OPTION_PREFETCH_MODE
    #define OPTION_PREFETCH_MODE TRUE
    #undef OPTION_S3SCRIPT
    #define OPTION_S3SCRIPT  TRUE
    #undef OPTION_CPU_CFOH
    #define OPTION_CPU_CFOH  TRUE
    #undef OPTION_UDIMMS
    #define OPTION_UDIMMS  TRUE
    #undef OPTION_SODIMMS
    #define OPTION_SODIMMS  TRUE
    #undef OPTION_DDR3
    #define OPTION_DDR3  TRUE
    #undef OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE  TRUE
    #undef OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE  TRUE
    #undef OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE  TRUE
    #undef OPTION_DIMM_EXCLUDE
    #define OPTION_DIMM_EXCLUDE  TRUE
    #undef OPTION_AMP
    #define OPTION_AMP  TRUE
    #undef OPTION_CPU_APM
    #define OPTION_CPU_APM TRUE
  #endif
#endif

#if (OPTION_FAMILY15H_TN == TRUE) || (OPTION_FAMILY15H_KV == TRUE)
  #undef  GNB_SUPPORT
  #define GNB_SUPPORT   TRUE
#endif

#define OPTION_ACPI_PSTATES             TRUE
#define OPTION_WHEA                     TRUE
#define OPTION_DMI                      TRUE
#define OPTION_EARLY_SAMPLES            FALSE
#define CFG_ACPI_PSTATES_PPC            TRUE
#define CFG_ACPI_PSTATES_PCT            TRUE
#define CFG_ACPI_PSTATES_PSD            TRUE
#define CFG_ACPI_PSTATES_PSS            TRUE
#define CFG_ACPI_PSTATES_XPSS           TRUE
#define CFG_ACPI_PSTATE_PSD_INDPX       FALSE
#define CFG_VRM_HIGH_SPEED_ENABLE       FALSE
#define CFG_VRM_NB_HIGH_SPEED_ENABLE    FALSE
#define OPTION_ALIB                     TRUE
/*---------------------------------------------------------------------------
 *       Processing the options:  Second, process the user's selections
 *--------------------------------------------------------------------------*/
#ifdef BLDOPT_REMOVE_DDR3_SUPPORT
  #if  BLDOPT_REMOVE_DDR3_SUPPORT == TRUE
    #undef  OPTION_DDR3
    #define OPTION_DDR3               FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_GDDR5_SUPPORT
  #if  BLDOPT_REMOVE_GDDR5_SUPPORT == TRUE
    #undef  OPTION_GDDR5
    #define OPTION_GDDR5               FALSE
  #endif
#endif
#if ((OPTION_DDR3 == FALSE) && (OPTION_GDDR5 == FALSE))
  #error  BLDOPT: No DIMM type support selected. Either BLDOPT_REMOVE_DDR3_SUPPORT or BLDOPT_REMOVE_GDDR5_SUPPORT must be FALSE.
#endif
#ifdef BLDOPT_REMOVE_MULTISOCKET_SUPPORT
  #if  BLDOPT_REMOVE_MULTISOCKET_SUPPORT == TRUE
    #undef  OPTION_MULTISOCKET
    #define OPTION_MULTISOCKET          FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_ECC_SUPPORT
  #if  BLDOPT_REMOVE_ECC_SUPPORT == TRUE
    #undef  OPTION_ECC
    #define OPTION_ECC        FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_UDIMMS_SUPPORT
  #if  BLDOPT_REMOVE_UDIMMS_SUPPORT == TRUE
    #undef  OPTION_UDIMMS
    #define OPTION_UDIMMS               FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_RDIMMS_SUPPORT
  #if  BLDOPT_REMOVE_RDIMMS_SUPPORT == TRUE
    #undef  OPTION_RDIMMS
    #define OPTION_RDIMMS               FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_SODIMMS_SUPPORT
  #if  BLDOPT_REMOVE_SODIMMS_SUPPORT == TRUE
    #undef  OPTION_SODIMMS
    #define OPTION_SODIMMS               FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_LRDIMMS_SUPPORT
  #if  BLDOPT_REMOVE_LRDIMMS_SUPPORT == TRUE
    #undef  OPTION_LRDIMMS
    #define OPTION_LRDIMMS               FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_BANK_INTERLEAVE
  #if  BLDOPT_REMOVE_BANK_INTERLEAVE == TRUE
    #undef  OPTION_BANK_INTERLEAVE
    #define OPTION_BANK_INTERLEAVE      FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_DCT_INTERLEAVE
  #if  BLDOPT_REMOVE_DCT_INTERLEAVE == TRUE
    #undef  OPTION_DCT_INTERLEAVE
    #define OPTION_DCT_INTERLEAVE       FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_NODE_INTERLEAVE
  #if  BLDOPT_REMOVE_NODE_INTERLEAVE == TRUE
    #undef  OPTION_NODE_INTERLEAVE
    #define OPTION_NODE_INTERLEAVE      FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_PARALLEL_TRAINING
  #if  BLDOPT_REMOVE_PARALLEL_TRAINING == TRUE
    #undef  OPTION_PARALLEL_TRAINING
    #define OPTION_PARALLEL_TRAINING    FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT
  #if  BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT == TRUE
    #undef  OPTION_ONLINE_SPARE
    #define OPTION_ONLINE_SPARE         FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_MEM_RESTORE_SUPPORT
  #if  BLDOPT_REMOVE_MEM_RESTORE_SUPPORT == TRUE
    #undef  OPTION_MEM_RESTORE
    #define OPTION_MEM_RESTORE         FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_HW_DQS_REC_EN_SEED_TRAINING
  #if  BLDOPT_REMOVE_HW_DQS_REC_EN_SEED_TRAINING == TRUE
    #undef OPTION_HW_DQS_REC_EN_SEED_TRAINING
    #define OPTION_HW_DQS_REC_EN_SEED_TRAINING      FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_ACPI_PSTATES
  #if  BLDOPT_REMOVE_ACPI_PSTATES == TRUE
    #undef  OPTION_ACPI_PSTATES
    #define OPTION_ACPI_PSTATES         FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_CRAT
  #if  BLDOPT_REMOVE_CRAT == TRUE
    #undef  OPTION_CRAT
    #define OPTION_CRAT                 FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_CDIT
  #if  BLDOPT_REMOVE_CDIT == TRUE
    #undef  OPTION_CDIT
    #define OPTION_CDIT                 FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_SRAT
  #if  BLDOPT_REMOVE_SRAT == TRUE
    #undef  OPTION_SRAT
    #define OPTION_SRAT                 FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_SLIT
  #if  BLDOPT_REMOVE_SLIT == TRUE
    #undef  OPTION_SLIT
    #define OPTION_SLIT                 FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_WHEA
  #if  BLDOPT_REMOVE_WHEA == TRUE
    #undef  OPTION_WHEA
    #define OPTION_WHEA                 FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_DMI
  #if  BLDOPT_REMOVE_DMI == TRUE
    #undef  OPTION_DMI
    #define OPTION_DMI                  FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_ADDR_TO_CS_TRANSLATOR
  #if  BLDOPT_REMOVE_ADDR_TO_CS_TRANSLATOR == TRUE
    #undef  OPTION_ADDR_TO_CS_TRANSLATOR
    #define OPTION_ADDR_TO_CS_TRANSLATOR        FALSE
  #endif
#endif
#ifdef BLDOPT_REMOVE_AMP_SUPPORT
  #if  BLDOPT_REMOVE_AMP_SUPPORT == TRUE
    #undef  OPTION_AMP
    #define OPTION_AMP        FALSE
  #endif
#endif

#ifdef OPTION_RDDQS_2D_TRAINING
  #if OPTION_RDDQS_2D_TRAINING == FALSE
    #undef OPTION_DATA_EYE
    #define OPTION_DATA_EYE     FALSE
  #else
    #ifdef BLDOPT_REMOVE_DATA_EYE
      #if BLDOPT_REMOVE_DATA_EYE == TRUE
        #undef OPTION_DATA_EYE
        #define OPTION_DATA_EYE     FALSE
      #endif
    #endif
  #endif
#else
  #undef OPTION_DATA_EYE
  #define OPTION_DATA_EYE     FALSE
#endif

#ifdef BLDOPT_REMOVE_C6_STATE
  #if  BLDOPT_REMOVE_C6_STATE == TRUE
    #undef  OPTION_C6_STATE
    #define OPTION_C6_STATE      FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_CONNECTED_STANDBY
  #if  BLDOPT_REMOVE_CONNECTED_STANDBY == TRUE
    #undef  OPTION_CONNECTED_STANDBY
    #define OPTION_CONNECTED_STANDBY    FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_GFX_RECOVERY
  #if  BLDOPT_REMOVE_GFX_RECOVERY == TRUE
    #undef  OPTION_GFX_RECOVERY
    #define OPTION_GFX_RECOVERY         FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_HW_RDDQS_2D_TRAINING
  #if  BLDOPT_REMOVE_HW_RDDQS_2D_TRAINING == TRUE
    #undef OPTION_RDDQS_2D_TRAINING
    #define OPTION_RDDQS_2D_TRAINING      FALSE
  #endif
#endif

#ifdef BLDCFG_REMOVE_ACPI_PSTATES_PPC
  #if  BLDCFG_REMOVE_ACPI_PSTATES_PPC == TRUE
    #undef  CFG_ACPI_PSTATES_PPC
    #define CFG_ACPI_PSTATES_PPC        FALSE
  #endif
#endif

#ifdef BLDCFG_REMOVE_ACPI_PSTATES_PCT
  #if  BLDCFG_REMOVE_ACPI_PSTATES_PCT == TRUE
    #undef  CFG_ACPI_PSTATES_PCT
    #define CFG_ACPI_PSTATES_PCT        FALSE
  #endif
#endif

#ifdef BLDCFG_REMOVE_ACPI_PSTATES_PSD
  #if  BLDCFG_REMOVE_ACPI_PSTATES_PSD == TRUE
    #undef  CFG_ACPI_PSTATES_PSD
    #define CFG_ACPI_PSTATES_PSD        FALSE
  #endif
#endif

#ifdef BLDCFG_REMOVE_ACPI_PSTATES_PSS
  #if  BLDCFG_REMOVE_ACPI_PSTATES_PSS == TRUE
    #undef  CFG_ACPI_PSTATES_PSS
    #define CFG_ACPI_PSTATES_PSS        FALSE
  #endif
#endif

#ifdef BLDCFG_REMOVE_ACPI_PSTATES_XPSS
  #if  BLDCFG_REMOVE_ACPI_PSTATES_XPSS == TRUE
    #undef  CFG_ACPI_PSTATES_XPSS
    #define CFG_ACPI_PSTATES_XPSS       FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_LOW_POWER_STATE_FOR_PROCHOT
  #if  BLDOPT_REMOVE_LOW_POWER_STATE_FOR_PROCHOT  == TRUE
    #undef  OPTION_CPU_LOW_PWR_PSTATE_FOR_PROCHOT
    #define OPTION_CPU_LOW_PWR_PSTATE_FOR_PROCHOT    FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_AGGRESSOR
  #if  BLDOPT_REMOVE_AGGRESSOR == TRUE
    #undef  OPTION_AGGRESSOR
    #define OPTION_AGGRESSOR      FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_S3_MEM_SUPPORT
  #if  BLDOPT_REMOVE_S3_MEM_SUPPORT == TRUE
    #undef  OPTION_S3_MEM_SUPPORT
    #define OPTION_S3_MEM_SUPPORT      FALSE
  #endif
#endif

#ifdef BLDCFG_FORCE_INDEPENDENT_PSD_OBJECT
  #if  BLDCFG_FORCE_INDEPENDENT_PSD_OBJECT == TRUE
    #undef  CFG_ACPI_PSTATE_PSD_INDPX
    #define CFG_ACPI_PSTATE_PSD_INDPX       TRUE
  #endif
#endif

#ifdef BLDCFG_ACPI_PSTATES_PSD_POLICY
    #define CFG_ACPI_PSTATES_PSD_POLICY     (BLDCFG_ACPI_PSTATES_PSD_POLICY)
#else
    #define CFG_ACPI_PSTATES_PSD_POLICY     PsdPolicyProcessorDefault
#endif

#ifdef BLDCFG_VRM_HIGH_SPEED_ENABLE
  #if  BLDCFG_VRM_HIGH_SPEED_ENABLE == TRUE
    #undef  CFG_VRM_HIGH_SPEED_ENABLE
    #define CFG_VRM_HIGH_SPEED_ENABLE       TRUE
  #endif
#endif

#ifdef BLDCFG_VRM_NB_HIGH_SPEED_ENABLE
  #if BLDCFG_VRM_NB_HIGH_SPEED_ENABLE == TRUE
    #undef CFG_VRM_NB_HIGH_SPEED_ENABLE
    #define CFG_VRM_NB_HIGH_SPEED_ENABLE    TRUE
  #endif
#endif

#ifdef BLDCFG_AMD_PLATFORM_TYPE
  #define CFG_AMD_PLATFORM_TYPE              BLDCFG_AMD_PLATFORM_TYPE
#else
  #define CFG_AMD_PLATFORM_TYPE              0
#endif

CONST UINT32 ROMDATA AmdPlatformTypeCgf = CFG_AMD_PLATFORM_TYPE;

#ifdef BLDCFG_PLATFORM_CONTROL_FLOW_MODE
  #define CFG_PLATFORM_CONTROL_FLOW_MODE  (BLDCFG_PLATFORM_CONTROL_FLOW_MODE)
#else
  #define CFG_PLATFORM_CONTROL_FLOW_MODE  (Nfcm)
#endif

#ifdef BLDCFG_PERFORMANCE_HARDWARE_PREFETCHER
  #define CFG_PERFORMANCE_HARDWARE_PREFETCHER  (BLDCFG_PERFORMANCE_HARDWARE_PREFETCHER)
#else
  #define CFG_PERFORMANCE_HARDWARE_PREFETCHER  (HARDWARE_PREFETCHER_AUTO)
#endif

#ifdef BLDCFG_PERFORMANCE_SOFTWARE_PREFETCHES
  #define CFG_PERFORMANCE_SOFTWARE_PREFETCHES  (BLDCFG_PERFORMANCE_SOFTWARE_PREFETCHES)
#else
  #define CFG_PERFORMANCE_SOFTWARE_PREFETCHES  (SOFTWARE_PREFETCHES_AUTO)
#endif

#ifdef BLDCFG_PERFORMANCE_DRAM_PREFETCHER
  #define CFG_PERFORMANCE_DRAM_PREFETCHER  (BLDCFG_PERFORMANCE_DRAM_PREFETCHER)
#else
  #define CFG_PERFORMANCE_DRAM_PREFETCHER  (DRAM_PREFETCHER_AUTO)
#endif

#ifdef BLDCFG_VRM_CURRENT_LIMIT
  #define CFG_VRM_CURRENT_LIMIT            BLDCFG_VRM_CURRENT_LIMIT
#else
  #define CFG_VRM_CURRENT_LIMIT            0
#endif

#ifdef BLDCFG_VRM_LOW_POWER_THRESHOLD
  #define CFG_VRM_LOW_POWER_THRESHOLD      BLDCFG_VRM_LOW_POWER_THRESHOLD
#else
  #define CFG_VRM_LOW_POWER_THRESHOLD      0
#endif

#ifdef BLDCFG_VRM_SLEW_RATE
  #define CFG_VRM_SLEW_RATE                BLDCFG_VRM_SLEW_RATE
#else
  #define CFG_VRM_SLEW_RATE                DFLT_VRM_SLEW_RATE
#endif

#ifdef BLDCFG_VRM_MAXIMUM_CURRENT_LIMIT
  #define CFG_VRM_MAXIMUM_CURRENT_LIMIT  BLDCFG_VRM_MAXIMUM_CURRENT_LIMIT
#else
  #define CFG_VRM_MAXIMUM_CURRENT_LIMIT  (0)
#endif

#ifdef BLDCFG_VRM_NB_MAXIMUM_CURRENT_LIMIT
  #define CFG_VRM_NB_MAXIMUM_CURRENT_LIMIT  BLDCFG_VRM_NB_MAXIMUM_CURRENT_LIMIT
#else
  #define CFG_VRM_NB_MAXIMUM_CURRENT_LIMIT  (0)
#endif

#ifdef BLDCFG_VRM_SVI_OCP_LEVEL
  #define CFG_VRM_SVI_OCP_LEVEL     BLDCFG_VRM_SVI_OCP_LEVEL
#else
  #define CFG_VRM_SVI_OCP_LEVEL     0
#endif

#ifdef BLDCFG_VRM_NB_SVI_OCP_LEVEL
  #define CFG_VRM_NB_SVI_OCP_LEVEL  BLDCFG_VRM_NB_SVI_OCP_LEVEL
#else
  #define CFG_VRM_NB_SVI_OCP_LEVEL  0
#endif

#ifdef BLDCFG_VRM_NB_CURRENT_LIMIT
  #define CFG_VRM_NB_CURRENT_LIMIT         BLDCFG_VRM_NB_CURRENT_LIMIT
#else
  #define CFG_VRM_NB_CURRENT_LIMIT         (0)
#endif

#ifdef BLDCFG_VRM_NB_LOW_POWER_THRESHOLD
  #define CFG_VRM_NB_LOW_POWER_THRESHOLD   BLDCFG_VRM_NB_LOW_POWER_THRESHOLD
#else
  #define CFG_VRM_NB_LOW_POWER_THRESHOLD   (0)
#endif

#ifdef BLDCFG_VRM_NB_SLEW_RATE
  #define CFG_VRM_NB_SLEW_RATE             BLDCFG_VRM_NB_SLEW_RATE
#else
  #define CFG_VRM_NB_SLEW_RATE             DFLT_VRM_SLEW_RATE
#endif

#ifdef BLDCFG_CPU_FREQUENCY_LIMIT
  #define CFG_CPU_FREQUENCY_LIMIT         BLDCFG_CPU_FREQUENCY_LIMIT
#else
  #define CFG_CPU_FREQUENCY_LIMIT         0
#endif

#ifdef BLDCFG_CPU_CONNECTED_STANDBY_MODE
  #define CFG_CPU_CONNECTED_STANDBY_MODE  BLDCFG_CPU_CONNECTED_STANDBY_MODE
#else
  #define CFG_CPU_CONNECTED_STANDBY_MODE  ConnectedStandbyAuto
#endif

#ifdef BLDCFG_PLAT_NUM_IO_APICS
  #define CFG_PLAT_NUM_IO_APICS            BLDCFG_PLAT_NUM_IO_APICS
#else
  #define CFG_PLAT_NUM_IO_APICS            0
#endif

#ifdef BLDCFG_MEM_INIT_PSTATE
  #define CFG_MEM_INIT_PSTATE              BLDCFG_MEM_INIT_PSTATE
#else
  #define CFG_MEM_INIT_PSTATE              0
#endif

#ifdef BLDCFG_PLATFORM_CSTATE_MODE
  #define CFG_CSTATE_MODE                     BLDCFG_PLATFORM_CSTATE_MODE
#else
  #define CFG_CSTATE_MODE                     CStateModeC6
#endif

#ifdef BLDCFG_PLATFORM_CSTATE_OPDATA
  #define CFG_CSTATE_OPDATA                   BLDCFG_PLATFORM_CSTATE_OPDATA
#else
  #define CFG_CSTATE_OPDATA                   0
#endif

#ifdef BLDCFG_PLATFORM_CSTATE_IO_BASE_ADDRESS
  #define CFG_CSTATE_IO_BASE_ADDRESS       BLDCFG_PLATFORM_CSTATE_IO_BASE_ADDRESS
#else
  #define CFG_CSTATE_IO_BASE_ADDRESS       0
#endif

#ifdef BLDCFG_PLATFORM_CPB_MODE
  #define CFG_CPB_MODE                        BLDCFG_PLATFORM_CPB_MODE
#else
  #define CFG_CPB_MODE                        CpbModeAuto
#endif

#ifdef BLDCFG_CORE_LEVELING_MODE
  #define CFG_CORE_LEVELING_MODE           BLDCFG_CORE_LEVELING_MODE
#else
  #define CFG_CORE_LEVELING_MODE           0
#endif

#ifdef BLDCFG_AMD_TDP_LIMIT
  #define CFG_AMD_POWER_CEILING            BLDCFG_AMD_TDP_LIMIT
#else
  #define CFG_AMD_POWER_CEILING            0
#endif

#ifdef BLDCFG_HEAP_DRAM_ADDRESS
  #define CFG_HEAP_DRAM_ADDRESS                 BLDCFG_HEAP_DRAM_ADDRESS
#else
  #define CFG_HEAP_DRAM_ADDRESS                 AMD_HEAP_RAM_ADDRESS
#endif

#ifdef BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
  #define CFG_MEMORY_BUS_FREQUENCY_LIMIT        BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
#else
  #define CFG_MEMORY_BUS_FREQUENCY_LIMIT        DDR800_FREQUENCY
#endif

#ifdef BLDCFG_MEMORY_MODE_UNGANGED
  #define CFG_MEMORY_MODE_UNGANGED              BLDCFG_MEMORY_MODE_UNGANGED
#else
  #define CFG_MEMORY_MODE_UNGANGED              TRUE
#endif

#ifdef BLDCFG_MEMORY_QUAD_RANK_CAPABLE
  #define CFG_MEMORY_QUAD_RANK_CAPABLE          BLDCFG_MEMORY_QUAD_RANK_CAPABLE
#else
  #define CFG_MEMORY_QUAD_RANK_CAPABLE          TRUE
#endif

#ifdef BLDCFG_MEMORY_QUADRANK_TYPE
  #define CFG_MEMORY_QUADRANK_TYPE              BLDCFG_MEMORY_QUADRANK_TYPE
#else
  #define CFG_MEMORY_QUADRANK_TYPE              DFLT_MEMORY_QUADRANK_TYPE
#endif

#ifdef BLDCFG_MEMORY_RDIMM_CAPABLE
  #define CFG_MEMORY_RDIMM_CAPABLE              BLDCFG_MEMORY_RDIMM_CAPABLE
#else
  #define CFG_MEMORY_RDIMM_CAPABLE              TRUE
#endif

#ifdef BLDCFG_MEMORY_LRDIMM_CAPABLE
  #define CFG_MEMORY_LRDIMM_CAPABLE              BLDCFG_MEMORY_LRDIMM_CAPABLE
#else
  #define CFG_MEMORY_LRDIMM_CAPABLE              TRUE
#endif

#ifdef BLDCFG_MEMORY_UDIMM_CAPABLE
  #define CFG_MEMORY_UDIMM_CAPABLE              BLDCFG_MEMORY_UDIMM_CAPABLE
#else
  #define CFG_MEMORY_UDIMM_CAPABLE              TRUE
#endif

#ifdef BLDCFG_MEMORY_SODIMM_CAPABLE
  #define CFG_MEMORY_SODIMM_CAPABLE             BLDCFG_MEMORY_SODIMM_CAPABLE
#else
  #define CFG_MEMORY_SODIMM_CAPABLE             FALSE
#endif

#ifdef BLDCFG_LIMIT_MEMORY_TO_BELOW_1TB
  #define CFG_LIMIT_MEMORY_TO_BELOW_1TB         BLDCFG_LIMIT_MEMORY_TO_BELOW_1TB
#else
  #define CFG_LIMIT_MEMORY_TO_BELOW_1TB         TRUE
#endif

#ifdef BLDCFG_MEMORY_ENABLE_BANK_INTERLEAVING
  #define CFG_MEMORY_ENABLE_BANK_INTERLEAVING   BLDCFG_MEMORY_ENABLE_BANK_INTERLEAVING
#else
  #define CFG_MEMORY_ENABLE_BANK_INTERLEAVING   TRUE
#endif

#ifdef BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING
  #define CFG_MEMORY_ENABLE_NODE_INTERLEAVING   BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING
#else
  #define CFG_MEMORY_ENABLE_NODE_INTERLEAVING   FALSE
#endif

#ifdef BLDCFG_MEMORY_CHANNEL_INTERLEAVING
  #define CFG_MEMORY_CHANNEL_INTERLEAVING       BLDCFG_MEMORY_CHANNEL_INTERLEAVING
#else
  #define CFG_MEMORY_CHANNEL_INTERLEAVING       TRUE
#endif

#ifdef BLDCFG_MEMORY_POWER_DOWN
  #define CFG_MEMORY_POWER_DOWN                 BLDCFG_MEMORY_POWER_DOWN
#else
  #if (OPTION_FAMILY15H_KV == TRUE)
    #define CFG_MEMORY_POWER_DOWN               TRUE
  #else
    #define CFG_MEMORY_POWER_DOWN               FALSE
  #endif
#endif

#ifdef BLDCFG_POWER_DOWN_MODE
  #define CFG_POWER_DOWN_MODE                   BLDCFG_POWER_DOWN_MODE
#else
  #define CFG_POWER_DOWN_MODE                   POWER_DOWN_MODE_AUTO
#endif

#ifdef BLDCFG_ONLINE_SPARE
  #define CFG_ONLINE_SPARE                      BLDCFG_ONLINE_SPARE
#else
  #define CFG_ONLINE_SPARE                      FALSE
#endif

#ifdef BLDCFG_MEMORY_PARITY_ENABLE
  #define CFG_MEMORY_PARITY_ENABLE              BLDCFG_MEMORY_PARITY_ENABLE
#else
  #define CFG_MEMORY_PARITY_ENABLE              FALSE
#endif

#ifdef BLDCFG_BANK_SWIZZLE
  #define CFG_BANK_SWIZZLE                      BLDCFG_BANK_SWIZZLE
#else
  #define CFG_BANK_SWIZZLE                      TRUE
#endif

#ifdef BLDCFG_TIMING_MODE_SELECT
  #define CFG_TIMING_MODE_SELECT                BLDCFG_TIMING_MODE_SELECT
#else
  #define CFG_TIMING_MODE_SELECT                TIMING_MODE_AUTO
#endif

#ifdef BLDCFG_MEMORY_CLOCK_SELECT
  #define CFG_MEMORY_CLOCK_SELECT               BLDCFG_MEMORY_CLOCK_SELECT
#else
  #define CFG_MEMORY_CLOCK_SELECT               DDR800_FREQUENCY
#endif

#ifdef BLDCFG_DQS_TRAINING_CONTROL
  #define CFG_DQS_TRAINING_CONTROL              BLDCFG_DQS_TRAINING_CONTROL
#else
  #define CFG_DQS_TRAINING_CONTROL              TRUE
#endif

#ifdef BLDCFG_IGNORE_SPD_CHECKSUM
  #define CFG_IGNORE_SPD_CHECKSUM               BLDCFG_IGNORE_SPD_CHECKSUM
#else
  #define CFG_IGNORE_SPD_CHECKSUM               FALSE
#endif

#ifdef BLDCFG_USE_BURST_MODE
  #define CFG_USE_BURST_MODE                    BLDCFG_USE_BURST_MODE
#else
  #define CFG_USE_BURST_MODE                    FALSE
#endif

#ifdef BLDCFG_MEMORY_ALL_CLOCKS_ON
  #define CFG_MEMORY_ALL_CLOCKS_ON              BLDCFG_MEMORY_ALL_CLOCKS_ON
#else
  #define CFG_MEMORY_ALL_CLOCKS_ON              FALSE
#endif

#ifdef BLDCFG_DDR_PHY_DLL_BYPASS_MODE
  #define CFG_DDR_PHY_DLL_BYPASS_MODE           BLDCFG_DDR_PHY_DLL_BYPASS_MODE
#else
  #define CFG_DDR_PHY_DLL_BYPASS_MODE           TRUE
#endif

#ifdef BLDCFG_ENABLE_ECC_FEATURE
  #define CFG_ENABLE_ECC_FEATURE                BLDCFG_ENABLE_ECC_FEATURE
#else
  #define CFG_ENABLE_ECC_FEATURE                TRUE
#endif

#ifdef BLDCFG_ECC_REDIRECTION
  #define CFG_ECC_REDIRECTION                   BLDCFG_ECC_REDIRECTION
#else
  #define CFG_ECC_REDIRECTION                   FALSE
#endif

#ifdef BLDCFG_SCRUB_DRAM_RATE
  #define CFG_SCRUB_DRAM_RATE         BLDCFG_SCRUB_DRAM_RATE
#else
  #define CFG_SCRUB_DRAM_RATE         DFLT_SCRUB_DRAM_RATE
#endif

#ifdef BLDCFG_SCRUB_L2_RATE
  #define CFG_SCRUB_L2_RATE           BLDCFG_SCRUB_L2_RATE
#else
  #define CFG_SCRUB_L2_RATE           DFLT_SCRUB_L2_RATE
#endif

#ifdef BLDCFG_SCRUB_L3_RATE
  #define CFG_SCRUB_L3_RATE           BLDCFG_SCRUB_L3_RATE
#else
  #define CFG_SCRUB_L3_RATE           DFLT_SCRUB_L3_RATE
#endif

#ifdef BLDCFG_SCRUB_IC_RATE
  #define CFG_SCRUB_IC_RATE           BLDCFG_SCRUB_IC_RATE
#else
  #define CFG_SCRUB_IC_RATE           DFLT_SCRUB_IC_RATE
#endif

#ifdef BLDCFG_SCRUB_DC_RATE
  #define CFG_SCRUB_DC_RATE           BLDCFG_SCRUB_DC_RATE
#else
  #define CFG_SCRUB_DC_RATE           DFLT_SCRUB_DC_RATE
#endif

#ifdef BLDCFG_ECC_SYNC_FLOOD
  #define CFG_ECC_SYNC_FLOOD          BLDCFG_ECC_SYNC_FLOOD
#else
  #define CFG_ECC_SYNC_FLOOD          TRUE
#endif

#ifdef BLDCFG_ECC_SYMBOL_SIZE
  #define CFG_ECC_SYMBOL_SIZE         BLDCFG_ECC_SYMBOL_SIZE
#else
  #define CFG_ECC_SYMBOL_SIZE         0
#endif

#ifdef BLDCFG_1GB_ALIGN
  #define CFG_1GB_ALIGN               BLDCFG_1GB_ALIGN
#else
  #define CFG_1GB_ALIGN               FALSE
#endif

#ifdef BLDCFG_UMA_ALLOCATION_MODE
  #define CFG_UMA_MODE                BLDCFG_UMA_ALLOCATION_MODE
#else
  #define CFG_UMA_MODE                UMA_AUTO
#endif

#ifdef BLDCFG_FORCE_TRAINING_MODE
  #define CFG_FORCE_TRAIN_MODE                BLDCFG_FORCE_TRAINING_MODE
#else
  #define CFG_FORCE_TRAIN_MODE                FORCE_TRAIN_AUTO
#endif

#ifdef BLDCFG_PMU_TRAINING_MODE
  #define CFG_PMU_TRAIN_MODE                BLDCFG_PMU_TRAINING_MODE
#else
  #define CFG_PMU_TRAIN_MODE                PMU_TRAIN_AUTO
#endif

#ifdef BLDCFG_UMA_ALLOCATION_SIZE
  #define CFG_UMA_SIZE                BLDCFG_UMA_ALLOCATION_SIZE
#else
  #define CFG_UMA_SIZE                0
#endif

#ifdef BLDCFG_UMA_ABOVE4G_SUPPORT
  #define CFG_UMA_ABOVE4G           BLDCFG_UMA_ABOVE4G_SUPPORT
#else
  #define CFG_UMA_ABOVE4G           FALSE
#endif

#ifdef BLDCFG_UMA_ALIGNMENT
  #define CFG_UMA_ALIGNMENT           BLDCFG_UMA_ALIGNMENT
#else
  #define CFG_UMA_ALIGNMENT           NO_UMA_ALIGNED
#endif

#ifdef BLDCFG_DIMM_TYPE_USED_IN_MIXED_CONFIG
  #define CFG_DIMM_TYPE_USED_IN_MIXED_CONFIG      BLDCFG_DIMM_TYPE_USED_IN_MIXED_CONFIG
#else
  #define CFG_DIMM_TYPE_USED_IN_MIXED_CONFIG      DDR3_TECHNOLOGY
#endif

#ifdef BLDCFG_PROCESSOR_SCOPE_IN_SB
  #define CFG_PROCESSOR_SCOPE_IN_SB   BLDCFG_PROCESSOR_SCOPE_IN_SB
#else
  #define CFG_PROCESSOR_SCOPE_IN_SB   FALSE
#endif

#ifdef BLDCFG_S3_LATE_RESTORE
  #define CFG_S3_LATE_RESTORE         BLDCFG_S3_LATE_RESTORE
#else
  #define CFG_S3_LATE_RESTORE         TRUE
#endif

#ifdef BLDCFG_USE_32_BYTE_REFRESH
  #define CFG_USE_32_BYTE_REFRESH  (BLDCFG_USE_32_BYTE_REFRESH)
#else
  #define CFG_USE_32_BYTE_REFRESH  (FALSE)
#endif

#ifdef BLDCFG_USE_VARIABLE_MCT_ISOC_PRIORITY
  #define CFG_USE_VARIABLE_MCT_ISOC_PRIORITY  (BLDCFG_USE_VARIABLE_MCT_ISOC_PRIORITY)
#else
  #define CFG_USE_VARIABLE_MCT_ISOC_PRIORITY  (FALSE)
#endif

#ifdef BLDCFG_PROCESSOR_SCOPE_NAME0
  #define CFG_PROCESSOR_SCOPE_NAME0          BLDCFG_PROCESSOR_SCOPE_NAME0
#else
  #define CFG_PROCESSOR_SCOPE_NAME0          SCOPE_NAME_VALUE
#endif

#ifdef BLDCFG_PROCESSOR_SCOPE_NAME1
  #define CFG_PROCESSOR_SCOPE_NAME1          BLDCFG_PROCESSOR_SCOPE_NAME1
#else
  #define CFG_PROCESSOR_SCOPE_NAME1          SCOPE_NAME_VALUE1
#endif

#ifdef BLDCFG_CFG_GNB_HD_AUDIO
  #define CFG_GNB_HD_AUDIO                   BLDCFG_CFG_GNB_HD_AUDIO
#else
  #define CFG_GNB_HD_AUDIO                   TRUE
#endif

#ifdef BLDCFG_CFG_ABM_SUPPORT
  #define CFG_ABM_SUPPORT                    BLDCFG_CFG_ABM_SUPPORT
#else
  #define CFG_ABM_SUPPORT                    FALSE
#endif

#ifdef BLDCFG_CFG_DYNAMIC_REFRESH_RATE
  #define CFG_DYNAMIC_REFRESH_RATE           BLDCFG_CFG_DYNAMIC_REFRESH_RATE
#else
  #define CFG_DYNAMIC_REFRESH_RATE           0
#endif

#ifdef BLDCFG_CFG_LCD_BACK_LIGHT_CONTROL
  #define CFG_LCD_BACK_LIGHT_CONTROL         BLDCFG_CFG_LCD_BACK_LIGHT_CONTROL
#else
  #define CFG_LCD_BACK_LIGHT_CONTROL         200
#endif

#ifdef BLDCFG_STEREO_3D_PINOUT
  #define CFG_GNB_STEREO_3D_PINOUT           BLDCFG_STEREO_3D_PINOUT
#else
  #define CFG_GNB_STEREO_3D_PINOUT           0
#endif

#ifdef BLDCFG_REMOTE_DISPLAY_SUPPORT
  #define CFG_GNB_REMOTE_DISPLAY_SUPPORT     BLDCFG_REMOTE_DISPLAY_SUPPORT
#else
  #define CFG_GNB_REMOTE_DISPLAY_SUPPORT     FALSE
#endif

// Define pin configuration for SYNCFLOOD
// Default to FALSE (Use pin as SYNCFLOOD)
#ifdef BLDCFG_USE_SYNCFLOOD_AS_NMI
  #define CFG_GNB_SYNCFLOOD_PIN_AS_NMI       BLDCFG_USE_SYNCFLOOD_AS_NMI
#else
  #define CFG_GNB_SYNCFLOOD_PIN_AS_NMI       FALSE
#endif

#ifdef BLDCFG_GNB_THERMAL_SENSOR_CORRECTION
  #define CFG_GNB_THERMAL_SENSOR_CORRECTION  BLDCFG_GNB_THERMAL_SENSOR_CORRECTION
#else
  #define CFG_GNB_THERMAL_SENSOR_CORRECTION  0
#endif

#ifdef BLDCFG_IGPU_SUBSYSTEM_ID
  #define CFG_GNB_IGPU_SSID                  BLDCFG_IGPU_SUBSYSTEM_ID
#else
  #define CFG_GNB_IGPU_SSID                  0
#endif

#ifdef BLDCFG_IGPU_HD_AUDIO_SUBSYSTEM_ID
  #define CFG_GNB_HDAUDIO_SSID               BLDCFG_IGPU_HD_AUDIO_SUBSYSTEM_ID
#else
  #define CFG_GNB_HDAUDIO_SSID               0
#endif

#ifdef BLDCFG_IGPU_ENABLE_DISABLE_POLICY
  #define CFG_IGPU_ENABLE_DISABLE_POLICY    BLDCFG_IGPU_ENABLE_DISABLE_POLICY
#else
  #define CFG_IGPU_ENABLE_DISABLE_POLICY    IGPU_DISABLE_AUTO
#endif

#ifdef BLDCFG_ACP_SIZE
  #define CFG_ACP_SIZE                BLDCFG_ACP_SIZE
#else
  #define CFG_ACP_SIZE                NO_ACP_SIZE
#endif

#ifdef BLDCFG_APU_PCIE_PORTS_SUBSYSTEM_ID
  #define CFG_GNB_PCIE_SSID                  BLDCFG_APU_PCIE_PORTS_SUBSYSTEM_ID
#else
  #define CFG_GNB_PCIE_SSID                  0x12341022ul
#endif

#ifdef BLDCFG_GFX_LVDS_SPREAD_SPECTRUM
  #define CFG_GFX_LVDS_SPREAD_SPECTRUM               BLDCFG_GFX_LVDS_SPREAD_SPECTRUM
#else
  #define CFG_GFX_LVDS_SPREAD_SPECTRUM               0
#endif

#ifdef BLDCFG_GFX_LVDS_SPREAD_SPECTRUM_RATE
  #define CFG_GFX_LVDS_SPREAD_SPECTRUM_RATE               BLDCFG_GFX_LVDS_SPREAD_SPECTRUM_RATE
#else
  #define CFG_GFX_LVDS_SPREAD_SPECTRUM_RATE               0
#endif

#ifdef BLDCFG_CFG_TEMP_PCIE_MMIO_BASE_ADDRESS
  #define CFG_TEMP_PCIE_MMIO_BASE_ADDRESS    BLDCFG_CFG_TEMP_PCIE_MMIO_BASE_ADDRESS
#else
  #define CFG_TEMP_PCIE_MMIO_BASE_ADDRESS    0xD0000000ul
#endif

#ifdef BLDCFG_ENABLE_EXTERNAL_VREF_FEATURE
  #define CFG_ENABLE_EXTERNAL_VREF                BLDCFG_ENABLE_EXTERNAL_VREF_FEATURE
#else
  #define CFG_ENABLE_EXTERNAL_VREF                FALSE
#endif

#ifdef BLDOPT_REMOVE_EARLY_SAMPLES
  #if  BLDOPT_REMOVE_EARLY_SAMPLES == TRUE
    #undef  OPTION_EARLY_SAMPLES
    #define OPTION_EARLY_SAMPLES        FALSE
  #else
    #undef  OPTION_EARLY_SAMPLES
    #define OPTION_EARLY_SAMPLES        TRUE
  #endif
#endif

#ifdef BLDOPT_REMOVE_ALIB
  #if  BLDOPT_REMOVE_ALIB == TRUE
    #undef  OPTION_ALIB
    #define OPTION_ALIB                 FALSE
  #else
    #undef  OPTION_ALIB
    #define OPTION_ALIB                 TRUE
  #endif
#endif

#ifdef BLDOPT_REMOVE_FCH_COMPONENT
  #if BLDOPT_REMOVE_FCH_COMPONENT == TRUE
    #undef FCH_SUPPORT
    #define FCH_SUPPORT                            FALSE
  #endif
#endif

// PCIE_REFCLK_SPREAD_SPECTRUM has dependency with FCH, ASIC decides the percentage and should not change it
#ifdef BLDCFG_PCIE_REFCLK_SPREAD_SPECTRUM
  #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM               BLDCFG_PCIE_REFCLK_SPREAD_SPECTRUM
#else
  #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM               0
#endif


#ifdef BLDCFG_IOMMU_SUPPORT
  #define CFG_IOMMU_SUPPORT             BLDCFG_IOMMU_SUPPORT
#else
  #define CFG_IOMMU_SUPPORT             TRUE
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE
  #define CFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE             BLDCFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE
#else
  #define CFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE             0
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL
  #define CFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL           BLDCFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL
#else
  #define CFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL           0
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON
  #define CFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON             BLDCFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON
#else
  #define CFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON             0
#endif

#ifdef BLDCFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE
  #define CFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE          BLDCFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE
#else
  #define CFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE          0
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY
  #define CFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY         BLDCFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY
#else
  #define CFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY         0
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON
  #define CFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON         BLDCFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON
#else
  #define CFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON         0
#endif

#ifdef BLDCFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL
  #define CFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL         BLDCFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL
#else
  #define CFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL         0
#endif

#ifdef BLDCFG_LVDS_MAX_PIXEL_CLOCK_FREQ
  #define CFG_LVDS_MAX_PIXEL_CLOCK_FREQ             BLDCFG_LVDS_MAX_PIXEL_CLOCK_FREQ
#else
  #define CFG_LVDS_MAX_PIXEL_CLOCK_FREQ             0
#endif

#ifdef BLDCFG_LCD_BIT_DEPTH_CONTROL_VALUE
  #define CFG_LCD_BIT_DEPTH_CONTROL_VALUE           BLDCFG_LCD_BIT_DEPTH_CONTROL_VALUE
#else
  #define CFG_LCD_BIT_DEPTH_CONTROL_VALUE           0
#endif


// BLDCFG_LVDS_24BBP_PANEL_MODE
// This specifies the LVDS 24 BBP mode.
// 0 - Use LDI mode (default).
// 1 - Use FPDI mode.
#ifdef BLDCFG_LVDS_24BBP_PANEL_MODE
  #define CFG_LVDS_24BBP_PANEL_MODE                 BLDCFG_LVDS_24BBP_PANEL_MODE
#else
  #define CFG_LVDS_24BBP_PANEL_MODE                 0
#endif

#ifdef BLDCFG_LVDS_MISC_888_FPDI_MODE
  #define CFG_LVDS_MISC_888_FPDI_MODE                 BLDCFG_LVDS_MISC_888_FPDI_MODE
#else
  #define CFG_LVDS_MISC_888_FPDI_MODE                 FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_DL_CH_SWAP
  #define CFG_LVDS_MISC_DL_CH_SWAP                 BLDCFG_LVDS_MISC_DL_CH_SWAP
#else
  #define CFG_LVDS_MISC_DL_CH_SWAP                 FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_VSYNC_ACTIVE_LOW
  #define CFG_LVDS_MISC_VSYNC_ACTIVE_LOW                 BLDCFG_LVDS_MISC_VSYNC_ACTIVE_LOW
#else
  #define CFG_LVDS_MISC_VSYNC_ACTIVE_LOW                 FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_HSYNC_ACTIVE_LOW
  #define CFG_LVDS_MISC_HSYNC_ACTIVE_LOW                 BLDCFG_LVDS_MISC_HSYNC_ACTIVE_LOW
#else
  #define CFG_LVDS_MISC_HSYNC_ACTIVE_LOW                 FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_BLON_ACTIVE_LOW
  #define CFG_LVDS_MISC_BLON_ACTIVE_LOW                 BLDCFG_LVDS_MISC_BLON_ACTIVE_LOW
#else
  #define CFG_LVDS_MISC_BLON_ACTIVE_LOW                 FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_VOLT_OVERWRITE_ENABLE
  #define CFG_LVDS_MISC_VOLT_OVERWRITE_ENABLE            BLDCFG_LVDS_MISC_VOLT_OVERWRITE_ENABLE
#else
  #define CFG_LVDS_MISC_VOLT_OVERWRITE_ENABLE            FALSE
#endif

#ifdef BLDCFG_LVDS_MISC_VOLT_ADJUSTMENT
  #define CFG_LVDS_MISC_VOLT_ADJUSTMENT               BLDCFG_LVDS_MISC_VOLT_ADJUSTMENT
#else
  #define CFG_LVDS_MISC_VOLT_ADJUSTMENT               0
#endif

#ifdef BLDCFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE
  #define CFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE            BLDCFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE
#else
  #define CFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE            FALSE
#endif

#ifdef BLDCFG_DP_FIXED_VOLT_SWING
  #define CFG_DP_FIXED_VOLT_SWING               BLDCFG_DP_FIXED_VOLT_SWING
#else
  #define CFG_DP_FIXED_VOLT_SWING               0
#endif

#ifdef BLDCFG_PLATFORM_POWER_POLICY_MODE
  #define CFG_PLATFORM_POWER_POLICY_MODE  (BLDCFG_PLATFORM_POWER_POLICY_MODE)
#else
  #define CFG_PLATFORM_POWER_POLICY_MODE  (Auto)
#endif

#ifdef BLDCFG_NB_PSTATES_SUPPORTED
  #define CFG_NB_PSTATES_SUPPORTED        (BLDCFG_NB_PSTATES_SUPPORTED)
#else
  #define CFG_NB_PSTATES_SUPPORTED        (TRUE)
#endif

#ifdef BLDCFG_HTC_TEMPERATURE_LIMIT
  #define CFG_HTC_TEMPERATURE_LIMIT        (BLDCFG_HTC_TEMPERATURE_LIMIT)
#else
  #define CFG_HTC_TEMPERATURE_LIMIT        (0)
#endif

#ifdef BLDCFG_LHTC_TEMPERATURE_LIMIT
  #define CFG_LHTC_TEMPERATURE_LIMIT        (BLDCFG_LHTC_TEMPERATURE_LIMIT)
#else
  #define CFG_LHTC_TEMPERATURE_LIMIT        (0)
#endif

#ifdef BLDCFG_PCI_MMIO_BASE
  #define CFG_PCI_MMIO_BASE               (BLDCFG_PCI_MMIO_BASE)
#else
  #define CFG_PCI_MMIO_BASE               (0)
#endif

#ifdef BLDCFG_PCI_MMIO_SIZE
  #define CFG_PCI_MMIO_SIZE               (BLDCFG_PCI_MMIO_SIZE)
#else
  #define CFG_PCI_MMIO_SIZE               (0)
#endif

#ifdef BLDCFG_AP_MTRR_SETTINGS_LIST
  #define CFG_AP_MTRR_SETTINGS_LIST           (BLDCFG_AP_MTRR_SETTINGS_LIST)
#else
  #define CFG_AP_MTRR_SETTINGS_LIST           (NULL)
#endif

#ifdef BLDCFG_IOMMU_EXCLUSION_RANGE_LIST
  #define CFG_IOMMU_EXCLUSION_RANGE_LIST      (BLDCFG_IOMMU_EXCLUSION_RANGE_LIST)
#else
  #define CFG_IOMMU_EXCLUSION_RANGE_LIST      (NULL)
#endif

#ifdef BLDCFG_HYBRID_BOOST_ENABLE
  #define CFG_HYBRID_BOOST_ENABLE            BLDCFG_HYBRID_BOOST_ENABLE
#else
  #define CFG_HYBRID_BOOST_ENABLE            TRUE
#endif

#ifdef BLDCFG_GNB_IOAPIC_ADDRESS
  #define CFG_GNB_IOAPIC_ADDRESS             BLDCFG_GNB_IOAPIC_ADDRESS
#else
  #define CFG_GNB_IOAPIC_ADDRESS             NULL
#endif

#ifdef BLDCFG_GNB_IOMMU_ADDRESS
  #define CFG_GNB_IOMMU_ADDRESS             BLDCFG_GNB_IOMMU_ADDRESS
#else
  #define CFG_GNB_IOMMU_ADDRESS             NULL
#endif

#ifdef BLDCFG_GPU_FREQUENCY_LIMIT
  #define CFG_GPU_FREQUENCY_LIMIT           BLDCFG_GPU_FREQUENCY_LIMIT
#else
  #define CFG_GPU_FREQUENCY_LIMIT           0
#endif

#ifdef BLDCFG_BAPM_ENABLE
  #define CFG_BAPM_ENABLE                   BLDCFG_BAPM_ENABLE
#else
  #define CFG_BAPM_ENABLE                   TRUE
#endif

#ifdef BLDCFG_ENABLE_DATA_EYE
  #define CFG_ENABLE_DATA_EYE              BLDCFG_ENABLE_DATA_EYE
#else
  #define CFG_ENABLE_DATA_EYE              TRUE
#endif

#ifdef BLDCFG_ACPI_SET_OEM_ID
  #define CFG_ACPI_SET_OEM_ID             BLDCFG_ACPI_SET_OEM_ID
#else
  #define CFG_ACPI_SET_OEM_ID             'A','M','D',' ',' ',' '
#endif

#ifdef BLDCFG_ACPI_SET_OEM_TABLE_ID
  #define CFG_ACPI_SET_OEM_TABLE_ID       BLDCFG_ACPI_SET_OEM_TABLE_ID
#else
  #define CFG_ACPI_SET_OEM_TABLE_ID       'A','G','E','S','A',' ',' ',' '
#endif

#ifdef BLDCFG_DOCKED_TDP_HEADROOM
  #define CFG_DOCKED_TDP_HEADROOM         BLDCFG_DOCKED_TDP_HEADROOM
#else
  #define CFG_DOCKED_TDP_HEADROOM         TRUE
#endif

#ifdef BLDCFG_BATTERY_BOOST_EN
  #define CFG_BATTERY_BOOST_EN         BLDCFG_BATTERY_BOOST_EN
#else
  #define CFG_BATTERY_BOOST_EN         TRUE
#endif

#ifdef BLDCFG_TDP_AC_DOCKED
  #define CFG_TDP_AC_DOCKED         BLDCFG_TDP_AC_DOCKED
#else
  #define CFG_TDP_AC_DOCKED         0
#endif

#ifdef BLDCFG_TDP_DC_DOCKED
  #define CFG_TDP_DC_DOCKED         BLDCFG_TDP_DC_DOCKED
#else
  #define CFG_TDP_DC_DOCKED         0
#endif

#ifdef BLDCFG_TDP_AC_UNDOCKED
  #define CFG_TDP_AC_UNDOCKED         BLDCFG_TDP_AC_UNDOCKED
#else
  #define CFG_TDP_AC_UNDOCKED         0
#endif

#ifdef BLDCFG_TDP_DC_UNDOCKED
  #define CFG_TDP_DC_UNDOCKED         BLDCFG_TDP_DC_UNDOCKED
#else
  #define CFG_TDP_DC_UNDOCKED         0
#endif

#ifdef BLDCFG_STAPM_EN_AC_DOCKED
  #define CFG_STAPM_EN_AC_DOCKED         BLDCFG_STAPM_EN_AC_DOCKED
#else
  #define CFG_STAPM_EN_AC_DOCKED         TRUE
#endif

#ifdef BLDCFG_STAPM_EN_DC_DOCKED
  #define CFG_STAPM_EN_DC_DOCKED         BLDCFG_STAPM_EN_DC_DOCKED
#else
  #define CFG_STAPM_EN_DC_DOCKED         TRUE
#endif

#ifdef BLDCFG_STAPM_EN_AC_UNDOCKED
  #define CFG_STAPM_EN_AC_UNDOCKED       BLDCFG_STAPM_EN_AC_UNDOCKED
#else
  #define CFG_STAPM_EN_AC_UNDOCKED       TRUE
#endif

#ifdef BLDCFG_STAPM_EN_DC_UNDOCKED
  #define CFG_STAPM_EN_DC_UNDOCKED       BLDCFG_STAPM_EN_DC_UNDOCKED
#else
  #define CFG_STAPM_EN_DC_UNDOCKED       TRUE
#endif

#ifdef BLDCFG_STAPM_POWER_AC_DOCKED
  #define CFG_STAPM_POWER_AC_DOCKED      BLDCFG_STAPM_POWER_AC_DOCKED
#else
  #define CFG_STAPM_POWER_AC_DOCKED      0
#endif

#ifdef BLDCFG_STAPM_POWER_DC_DOCKED
  #define CFG_STAPM_POWER_DC_DOCKED      BLDCFG_STAPM_POWER_DC_DOCKED
#else
  #define CFG_STAPM_POWER_DC_DOCKED      0
#endif

#ifdef BLDCFG_STAPM_POWER_AC_UNDOCKED
  #define CFG_STAPM_POWER_AC_UNDOCKED    BLDCFG_STAPM_POWER_AC_UNDOCKED
#else
  #define CFG_STAPM_POWER_AC_UNDOCKED    0
#endif

#ifdef BLDCFG_STAPM_POWER_DC_UNDOCKED
  #define CFG_STAPM_POWER_DC_UNDOCKED    BLDCFG_STAPM_POWER_DC_UNDOCKED
#else
  #define CFG_STAPM_POWER_DC_UNDOCKED    0
#endif

#ifdef BLDCFG_DRAM_DOUBLE_REFRESH_RATE
  #define CFG_DRAM_DOUBLE_REFRESH_RATE             BLDCFG_DRAM_DOUBLE_REFRESH_RATE
#else
  #define CFG_DRAM_DOUBLE_REFRESH_RATE             FALSE
#endif

#ifdef BLDCFG_MEMORY_PHY_VOLTAGE
  #define CFG_MEMORY_PHY_VOLTAGE          BLDCFG_MEMORY_PHY_VOLTAGE
#else
  #define CFG_MEMORY_PHY_VOLTAGE          VOLT1_05
#endif

/*---------------------------------------------------------------------------
 *       Processing the options:  Third, perform the option cross checks
 *--------------------------------------------------------------------------*/
// Assure that at least one type of memory support is included
#if OPTION_UDIMMS == FALSE
  #if OPTION_RDIMMS == FALSE
    #if OPTION_SODIMMS == FALSE
      #if OPTION_LRDIMMS == FALSE
        #error  BLDOPT: No DIMM support selected. Either BLDOPT_REMOVE_UDIMMS_SUPPORT or BLDOPT_REMOVE_RDIMMS_SUPPORT or BLDOPT_REMOVE_SODIMMS_SUPPORT or BLDOPT_REMOVE_LRDIMMS_SUPPORT must be FALSE.
      #endif
    #endif
  #endif
#endif
// Ensure at least one dimm type is capable
#if CFG_MEMORY_RDIMM_CAPABLE == FALSE
  #if CFG_MEMORY_UDIMM_CAPABLE == FALSE
    #if CFG_MEMORY_SODIMM_CAPABLE == FALSE
      #if CFG_MEMORY_LRDIMM_CAPABLE == FALSE
        #error   BLDCFG: No dimm type is capable
      #endif
    #endif
  #endif
#endif
// Check LRDIMM CODE and LRDIMM CFG item
#if CFG_MEMORY_LRDIMM_CAPABLE == FALSE
  #if  BLDOPT_REMOVE_LRDIMMS_SUPPORT == TRUE
    #error   Warning: LRDIMM capability is false, but LRIDMM support code included
  #endif
#endif
//  Turn off multi-socket based features if only one node...
#if OPTION_MULTISOCKET == FALSE
  #undef  OPTION_PARALLEL_TRAINING
  #define OPTION_PARALLEL_TRAINING    FALSE
  #undef  OPTION_NODE_INTERLEAVE
  #define OPTION_NODE_INTERLEAVE      FALSE
#endif
// Ensure the frequency limit is valid
#if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR2400_FREQUENCY)
  #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR2133_FREQUENCY)
    #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR1866_FREQUENCY)
      #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR1600_FREQUENCY)
        #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR1333_FREQUENCY)
          #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR1066_FREQUENCY)
            #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR800_FREQUENCY)
              #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR667_FREQUENCY)
                #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR533_FREQUENCY)
                  #if (CFG_MEMORY_BUS_FREQUENCY_LIMIT != DDR400_FREQUENCY)
                    #error    BLDCFG: Unsupported memory bus frequency
                  #endif
                #endif
              #endif
            #endif
          #endif
        #endif
      #endif
    #endif
  #endif
#endif
// Ensure timing mode is valid
#if CFG_TIMING_MODE_SELECT != TIMING_MODE_SPECIFIC
  #if CFG_TIMING_MODE_SELECT != TIMING_MODE_LIMITED
    #if CFG_TIMING_MODE_SELECT != TIMING_MODE_AUTO
      #error    BLDCFG: Invalid timing mode is set
    #endif
  #endif
#endif
// Ensure the scrub rate is valid
#if ((CFG_SCRUB_DRAM_RATE > 0x16) && (CFG_SCRUB_DRAM_RATE != 0xFF))
  #error    BLDCFG: Unsupported dram scrub rate set
#endif
#if CFG_SCRUB_L2_RATE > 0x16
  #error   BLDCFG: Unsupported L2 scrubber rate set
#endif
#if CFG_SCRUB_L3_RATE > 0x16
  #error   BLDCFG: unsupported L3 scrubber rate set
#endif
#if CFG_SCRUB_IC_RATE > 0x16
  #error    BLDCFG: Unsupported Instruction cache scrub rate set
#endif
#if CFG_SCRUB_DC_RATE > 0x16
  #error   BLDCFG: Unsupported Dcache scrub rate set
#endif
// Ensure Quad rank dimm type is valid
#if CFG_MEMORY_QUADRANK_TYPE != QUADRANK_UNBUFFERED
  #if CFG_MEMORY_QUADRANK_TYPE != QUADRANK_REGISTERED
    #error     BLDCFG: Invalid quad rank dimm type set
  #endif
#endif
// Ensure ECC symbol size is valid
#if CFG_ECC_SYMBOL_SIZE != ECCSYMBOLSIZE_USE_BKDG
  #if CFG_ECC_SYMBOL_SIZE != ECCSYMBOLSIZE_FORCE_X4
    #if CFG_ECC_SYMBOL_SIZE != ECCSYMBOLSIZE_FORCE_X8
      #error   BLDCFG: Invalid Ecc symbol size set
    #endif
  #endif
#endif
// Ensure power down mode is valid
#if CFG_POWER_DOWN_MODE != POWER_DOWN_BY_CHIP_SELECT
  #if CFG_POWER_DOWN_MODE != POWER_DOWN_BY_CHANNEL
    #if AGESA_ENTRY_INIT_POST == TRUE
      #error     BLDCFG: Invalid power down mode set
    #endif
  #endif
#endif

// Ensure P-state dependence settings do not conflict
#if (CFG_ACPI_PSTATES_PSD_POLICY == PsdPolicyDependent) && (CFG_ACPI_PSTATE_PSD_INDPX == TRUE)
  #error     BLDCFG: Conflict P-state dependency settings between BLDCFG_FORCE_INDEPENDENT_PSD_OBJECT and BLDCFG_ACPI_PSTATES_PSD_POLICY.
#endif

#if ((CFG_HTC_TEMPERATURE_LIMIT == 0) && (CFG_LHTC_TEMPERATURE_LIMIT != 0))
  #error     BLDCFG: Cannot define BLDCFG_LHTC_TEMPERATURE_LIMIT unless BLDCFG_HTC_TEMPERATURE_LIMIT is also not zero.
#endif

#if ((CFG_LHTC_TEMPERATURE_LIMIT == 0) && (CFG_HTC_TEMPERATURE_LIMIT != 0))
  #error     BLDCFG: Cannot define BLDCFG_HTC_TEMPERATURE_LIMIT unless BLDCFG_LHTC_TEMPERATURE_LIMIT is also not zero.
#endif

#if (CFG_MEMORY_PHY_VOLTAGE != VOLT0_95)
  #if (CFG_MEMORY_PHY_VOLTAGE != VOLT1_05)
    #error     BLDCFG: Invalid Memory Phy Voltage (VDDR)
  #endif
#endif
// Ensure the GPU max frequency is valid
#if ((CFG_GNB_GPU_MAX_FREQUENCY > UNSUPPORTED_GPU_FREQUENCY))
  #error     BLDCFG: Unsupported GPU max frequency
#endif

/*****************************************************************************
 *
 * Process the option logic, setting local control variables
 *
 ****************************************************************************/
#if OPTION_ACPI_PSTATES == TRUE
  #define OPTFCN_ACPI_TABLES           CreateAcpiTablesMain
  #define OPTFCN_GATHER_DATA           PStateGatherData
  #if OPTION_MULTISOCKET == TRUE
    #define OPTFCN_PSTATE_LEVELING     PStateLeveling
  #else
    #define OPTFCN_PSTATE_LEVELING     CommonReturnAgesaSuccess
  #endif
#else
  #define OPTFCN_ACPI_TABLES           CommonReturnAgesaSuccess
  #define OPTFCN_GATHER_DATA           CommonReturnAgesaSuccess
  #define OPTFCN_PSTATE_LEVELING       CommonReturnAgesaSuccess
#endif

// Consolidate P-state dependence setings
#if (CFG_ACPI_PSTATES_PSD_POLICY == PsdPolicyProcessorDefault) && (CFG_ACPI_PSTATE_PSD_INDPX == TRUE)
  #undef CFG_ACPI_PSTATES_PSD_POLICY
  #define CFG_ACPI_PSTATES_PSD_POLICY  PsdPolicyIndependent
#endif

/*****************************************************************************
 *
 * Include the structure definitions for the defaults table structures
 *
 ****************************************************************************/
#include  "Options.h"
#include  "OptionCpuFamiliesInstall.h"
#include  "OptionsTopology.h"
#include  "OptionTopologyInstall.h"
#include  "OptionMemory.h"
#include  "PlatformMemoryConfiguration.h"
#include  "OptionMemoryInstall.h"
#include  "OptionMemoryRecovery.h"
#include  "OptionMemoryRecoveryInstall.h"
#include  "OptionCpuFeaturesInstall.h"
#include  "OptionDmi.h"
#include  "OptionDmiInstall.h"
#include  "OptionPstate.h"
#include  "OptionPstateInstall.h"
#include  "OptionWhea.h"
#include  "OptionWheaInstall.h"
#include  "OptionCrat.h"
#include  "OptionCratInstall.h"
#include  "OptionCdit.h"
#include  "OptionCditInstall.h"
#include  "OptionSrat.h"
#include  "OptionSratInstall.h"
#include  "OptionSlit.h"
#include  "OptionSlitInstall.h"
#include  "OptionMultiSocket.h"
#include  "OptionMultiSocketInstall.h"
#include  "OptionIdsInstall.h"
#include  "OptionGfxRecovery.h"
#include  "OptionGfxRecoveryInstall.h"
#include  "OptionGnb.h"
#include  "OptionGnbInstall.h"
#include  "OptionS3ScriptInstall.h"
#include  "OptionFchInstall.h"
#include  "OptionMmioMapInstall.h"
#include  "OptionPrefetchModeInstall.h"


/*****************************************************************************
 *
 *  Generate the output structures (defaults tables)
 *
 ****************************************************************************/

FCH_PLATFORM_POLICY  FchUserOptions = {
  CFG_SMBUS0_BASE_ADDRESS,              // CfgSmbus0BaseAddress
  CFG_SMBUS1_BASE_ADDRESS,              // CfgSmbus1BaseAddress
  CFG_SIO_PME_BASE_ADDRESS,             // CfgSioPmeBaseAddress
  CFG_ACPI_PM1_EVT_BLOCK_ADDRESS,       // CfgAcpiPm1EvtBlkAddr
  CFG_ACPI_PM1_CNT_BLOCK_ADDRESS,       // CfgAcpiPm1CntBlkAddr
  CFG_ACPI_PM_TMR_BLOCK_ADDRESS,        // CfgAcpiPmTmrBlkAddr
  CFG_ACPI_CPU_CNT_BLOCK_ADDRESS,       // CfgCpuControlBlkAddr
  CFG_ACPI_GPE0_BLOCK_ADDRESS,          // CfgAcpiGpe0BlkAddr
  CFG_SMI_CMD_PORT_ADDRESS,             // CfgSmiCmdPortAddr
  CFG_ACPI_PMA_CNTBLK_ADDRESS,          // CfgAcpiPmaCntBlkAddr
  CFG_GEC_SHADOW_ROM_BASE,              // CfgGecShadowRomBase
  CFG_WATCHDOG_TIMER_BASE,              // CfgWatchDogTimerBase
  CFG_SPI_ROM_BASE_ADDRESS,             // CfgSpiRomBaseAddress
  CFG_HPET_BASE_ADDRESS,                // CfgHpetBaseAddress
  CFG_AZALIA_SSID,                      // CfgAzaliaSsid
  CFG_SMBUS_SSID,                       // CfgSmbusSsid
  CFG_IDE_SSID,                         // CfgIdeSsid
  CFG_SATA_AHCI_SSID,                   // CfgSataAhciSsid
  CFG_SATA_IDE_SSID,                    // CfgSataIdeSsid
  CFG_SATA_RAID5_SSID,                  // CfgSataRaid5Ssid
  CFG_SATA_RAID_SSID,                   // CfgSataRaidSsid
  CFG_EHCI_SSID,                        // CfgEhcidSsid
  CFG_OHCI_SSID,                        // CfgOhcidSsid
  CFG_LPC_SSID,                         // CfgLpcSsid
  CFG_SD_SSID,                          // CfgSdSsid
  CFG_XHCI_SSID,                        // CfgXhciSsid
  CFG_FCH_PORT80_BEHIND_PCIB,           // CfgFchPort80BehindPcib
  CFG_FCH_ENABLE_ACPI_SLEEP_TRAP,       // CfgFchEnableAcpiSleepTrap
  CFG_FCH_GPP_LINK_CONFIG,              // CfgFchGppLinkConfig
  CFG_FCH_GPP_PORT0_PRESENT,            // CfgFchGppPort0Present
  CFG_FCH_GPP_PORT1_PRESENT,            // CfgFchGppPort1Present
  CFG_FCH_GPP_PORT2_PRESENT,            // CfgFchGppPort2Present
  CFG_FCH_GPP_PORT3_PRESENT,            // CfgFchGppPort3Present
  CFG_FCH_GPP_PORT0_HOTPLUG,            // CfgFchGppPort0HotPlug
  CFG_FCH_GPP_PORT1_HOTPLUG,            // CfgFchGppPort1HotPlug
  CFG_FCH_GPP_PORT2_HOTPLUG,            // CfgFchGppPort2HotPlug
  CFG_FCH_GPP_PORT3_HOTPLUG,            // CfgFchGppPort3HotPlug

  CFG_FCH_ESATA_PORT_BITMAP,            // CfgFchEsataPortBitMap
  CFG_FCH_IR_PIN_CONTROL,               // CfgFchIrPinControl
  CFG_FCH_SD_CLOCK_CONTROL,             // CfgFchSdClockControl
  CFG_FCH_SCI_MAP_LIST,                 // *CfgFchSciMapControl
  CFG_FCH_SATA_PHY_LIST,                // *CfgFchSataPhyControl
  CFG_FCH_GPIO_CONTROL_LIST,            // *CfgFchGpioControl
  CFG_FCH_RTC_WORKAROUND,               // CfgFchRtcWorkaround
  CFG_FCH_USB_PORT_DIS_WORKAROUND       // CfgFchUsbPortDisWorkAround
};

BUILD_OPT_CFG UserOptions = {
  {                                             // AGESA version string
    AGESA_CODE_SIGNATURE,                       //    code header Signature
    AGESA_PACKAGE_STRING,                       //    16 character ID
    AGESA_VERSION_STRING,                       //    12 character version string
    0                                           //    null string terminator
  },
  //Build Option Area
  OPTION_UDIMMS,                        //UDIMMS
  OPTION_RDIMMS,                        //RDIMMS
  OPTION_LRDIMMS,                       //LRDIMMS
  OPTION_ECC,                           //ECC
  OPTION_BANK_INTERLEAVE,               //BANK_INTERLEAVE
  OPTION_DCT_INTERLEAVE,                //DCT_INTERLEAVE
  OPTION_NODE_INTERLEAVE,               //NODE_INTERLEAVE
  OPTION_PARALLEL_TRAINING,             //PARALLEL_TRAINING
  OPTION_ONLINE_SPARE,                  //ONLINE_SPARE
  OPTION_MEM_RESTORE,                   //MEM CONTEXT RESTORE
  OPTION_MULTISOCKET,                   //MULTISOCKET
  OPTION_ACPI_PSTATES,                  //ACPI_PSTATES
  OPTION_CRAT,                          //CRAT
  OPTION_CDIT,                          //CDIT
  OPTION_SRAT,                          //SRAT
  OPTION_SLIT,                          //SLIT
  OPTION_WHEA,                          //WHEA
  OPTION_DMI,                           //DMI
  OPTION_EARLY_SAMPLES,                 //EARLY_SAMPLES
  OPTION_ADDR_TO_CS_TRANSLATOR,         //ADDR_TO_CS_TRANSLATOR

  //Build Configuration Area
  CFG_PCI_MMIO_BASE,
  CFG_PCI_MMIO_SIZE,
  {
    // CoreVrm
    {
      CFG_VRM_CURRENT_LIMIT,              // VrmCurrentLimit
      CFG_VRM_LOW_POWER_THRESHOLD,        // VrmLowPowerThershold
      CFG_VRM_SLEW_RATE,                  // VrmSlewRate
      CFG_VRM_HIGH_SPEED_ENABLE,          // VrmHiSpeedEnable
      CFG_VRM_MAXIMUM_CURRENT_LIMIT,      // VrmMaximumCurrentLimit
      CFG_VRM_SVI_OCP_LEVEL               // VrmSviOcpLevel
    },
    // NbVrm
    {
      CFG_VRM_NB_CURRENT_LIMIT,           // VrmNbCurrentLimit
      CFG_VRM_NB_LOW_POWER_THRESHOLD,     // VrmNbLowPowerThershold
      CFG_VRM_NB_SLEW_RATE,               // VrmNbSlewRate
      CFG_VRM_NB_HIGH_SPEED_ENABLE,       // VrmNbHiSpeedEnable
      CFG_VRM_NB_MAXIMUM_CURRENT_LIMIT,   // VrmNbMaximumCurrentLimit
      CFG_VRM_NB_SVI_OCP_LEVEL            // VrmNbSviOcpLevel
    }
  },
  CFG_CPU_FREQUENCY_LIMIT,              //CfgCpuFrequencyLimit
  CFG_CPU_CONNECTED_STANDBY_MODE,       //CfgPlatformConnectedStandbyMode
  CFG_PLAT_NUM_IO_APICS,                //PlatformApicIoNumber
  CFG_MEM_INIT_PSTATE,                  //MemoryInitPstate
  CFG_CSTATE_MODE,                      //CStateMode
  CFG_CSTATE_OPDATA,                    //CStatePlatformData
  CFG_CSTATE_IO_BASE_ADDRESS,           //CStateIoBaseAddress
  CFG_CPB_MODE,                         //CpbMode
  LOW_POWER_PSTATE_FOR_PROCHOT_AUTO,    //Low power Pstate for PROCHOT, it's always set to 'AUTO'
  CFG_CORE_LEVELING_MODE,               //CoreLevelingCofig
  {
    CFG_PLATFORM_CONTROL_FLOW_MODE,       // The platform's control flow mode.
    CFG_USE_32_BYTE_REFRESH,              // Display Refresh uses 32 byte packets.
    CFG_USE_VARIABLE_MCT_ISOC_PRIORITY,   // The Memory controller will be set to Variable Isoc Priority.
    // ADVANCED_PERFORMANCE_PROFILE
    {
      CFG_PERFORMANCE_HARDWARE_PREFETCHER,  // Hardware prefetcher mode
      CFG_PERFORMANCE_SOFTWARE_PREFETCHES,  // Software prefetcher mode
      CFG_PERFORMANCE_DRAM_PREFETCHER       // Dram prefetcher mode
    },
    CFG_PLATFORM_POWER_POLICY_MODE,       // The platform's power policy mode.
    CFG_NB_PSTATES_SUPPORTED              // The Nb-Pstates is supported or not
  },
  CFG_AMD_PLATFORM_TYPE,                // CfgAmdPlatformType
  CFG_AMD_POWER_CEILING,                // CfgAmdPowerCeiling
  CFG_HTC_TEMPERATURE_LIMIT,            // CfgHtcTemperatureLimit
  CFG_LHTC_TEMPERATURE_LIMIT,           // CfgLhtcTemperatureLimit

  CFG_MEMORY_BUS_FREQUENCY_LIMIT,       // CfgMemoryBusFrequencyLimit
  CFG_MEMORY_MODE_UNGANGED,             // CfgMemoryModeUnganged
  CFG_MEMORY_QUAD_RANK_CAPABLE,         // CfgMemoryQuadRankCapable
  CFG_MEMORY_QUADRANK_TYPE,             // CfgMemoryQuadrankType
  CFG_MEMORY_RDIMM_CAPABLE,             // CfgMemoryRDimmCapable
  CFG_MEMORY_LRDIMM_CAPABLE,            // CfgMemoryLRDimmCapable
  CFG_MEMORY_UDIMM_CAPABLE,             // CfgMemoryUDimmCapable
  CFG_MEMORY_SODIMM_CAPABLE,            // CfgMemorySodimmCapable
  CFG_LIMIT_MEMORY_TO_BELOW_1TB,        // CfgLimitMemoryToBelow1Tb
  CFG_MEMORY_ENABLE_BANK_INTERLEAVING,  // CfgMemoryEnableBankInterleaving
  CFG_MEMORY_ENABLE_NODE_INTERLEAVING,  // CfgMemoryEnableNodeInterleaving
  CFG_MEMORY_CHANNEL_INTERLEAVING,      // CfgMemoryChannelInterleaving
  CFG_MEMORY_POWER_DOWN,                // CfgMemoryPowerDown
  CFG_POWER_DOWN_MODE,                  // CfgPowerDownMode
  CFG_ONLINE_SPARE,                     // CfgOnlineSpare
  CFG_MEMORY_PARITY_ENABLE,             // CfgMemoryParityEnable
  CFG_BANK_SWIZZLE,                     // CfgBankSwizzle
  CFG_TIMING_MODE_SELECT,               // CfgTimingModeSelect
  CFG_MEMORY_CLOCK_SELECT,              // CfgMemoryClockSelect
  CFG_DQS_TRAINING_CONTROL,             // CfgDqsTrainingControl
  CFG_IGNORE_SPD_CHECKSUM,              // CfgIgnoreSpdChecksum
  CFG_USE_BURST_MODE,                   // CfgUseBurstMode
  CFG_MEMORY_ALL_CLOCKS_ON,             // CfgMemoryAllClocksOn
  CFG_DDR_PHY_DLL_BYPASS_MODE,          // CfgDdrPhyDllBypassMode
  CFG_ENABLE_ECC_FEATURE,               // CfgEnableEccFeature
  CFG_ECC_REDIRECTION,                  // CfgEccRedirection
  CFG_SCRUB_DRAM_RATE,                  // CfgScrubDramRate
  CFG_SCRUB_L2_RATE,                    // CfgScrubL2Rate
  CFG_SCRUB_L3_RATE,                    // CfgScrubL3Rate
  CFG_SCRUB_IC_RATE,                    // CfgScrubIcRate
  CFG_SCRUB_DC_RATE,                    // CfgScrubDcRate
  CFG_ECC_SYNC_FLOOD,                   // CfgEccSyncFlood
  CFG_ECC_SYMBOL_SIZE,                  // CfgEccSymbolSize
  CFG_HEAP_DRAM_ADDRESS,                // CfgHeapDramAddress
  CFG_1GB_ALIGN,                        // CfgNodeMem1GBAlign
  CFG_S3_LATE_RESTORE,                  // CfgS3LateRestore
  CFG_ACPI_PSTATE_PSD_INDPX,            // CfgAcpiPstateIndependent
  CFG_ACPI_PSTATES_PSD_POLICY,          // CfgAcpiPstatesPsdPolicy
  (AP_MTRR_SETTINGS *) CFG_AP_MTRR_SETTINGS_LIST, // CfgApMtrrSettingsList
  CFG_UMA_MODE,                         // CfgUmaMode
  CFG_UMA_SIZE,                         // CfgUmaSize
  CFG_UMA_ABOVE4G,                      // CfgUmaAbove4G
  CFG_UMA_ALIGNMENT,                    // CfgUmaAlignment
  CFG_PROCESSOR_SCOPE_IN_SB,            // CfgProcessorScopeInSb
  CFG_PROCESSOR_SCOPE_NAME0,            // CfgProcessorScopeName0
  CFG_PROCESSOR_SCOPE_NAME1,            // CfgProcessorScopeName1
  CFG_GNB_HD_AUDIO,                     // CfgGnbHdAudio
  CFG_ABM_SUPPORT,                      // CfgAbmSupport
  CFG_DYNAMIC_REFRESH_RATE,             // CfgDynamicRefreshRate
  CFG_LCD_BACK_LIGHT_CONTROL,           // CfgLcdBackLightControl
  CFG_GNB_STEREO_3D_PINOUT,             // CfgGnb3dStereoPinIndex
  CFG_TEMP_PCIE_MMIO_BASE_ADDRESS,      // CfgTempPcieMmioBaseAddress
  CFG_GNB_IGPU_SSID,                    // CfgGnbIGPUSSID
  CFG_GNB_HDAUDIO_SSID,                 // CfgGnbHDAudioSSID
  CFG_GNB_PCIE_SSID,                    // CfgGnbPcieSSID
  CFG_GFX_LVDS_SPREAD_SPECTRUM,         // CfgLvdsSpreadSpectrum
  CFG_GFX_LVDS_SPREAD_SPECTRUM_RATE,    // CfgLvdsSpreadSpectrumRate

  &FchUserOptions,                      // FchBldCfg

  CFG_IOMMU_SUPPORT,                    // CfgIommuSupport
  CFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE,    // CfgLvdsPowerOnSeqDigonToDe
  CFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL,  // CfgLvdsPowerOnSeqDeToVaryBl
  CFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON,    // CfgLvdsPowerOnSeqDeToDigon
  CFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE, // CfgLvdsPowerOnSeqVaryBlToDe
  CFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY,// CfgLvdsPowerOnSeqOnToOffDelay
  CFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON,// CfgLvdsPowerOnSeqVaryBlToBlon
  CFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL,// CfgLvdsPowerOnSeqBlonToVaryBl
  CFG_LVDS_MAX_PIXEL_CLOCK_FREQ,        // CfgLvdsMaxPixelClockFreq
  CFG_LCD_BIT_DEPTH_CONTROL_VALUE,      // CfgLcdBitDepthControlValue
  CFG_LVDS_24BBP_PANEL_MODE,            // CfgLvds24bbpPanelMode
  {
    CFG_LVDS_MISC_888_FPDI_MODE,          // CfgLvdsMiscControl
    CFG_LVDS_MISC_DL_CH_SWAP,             // CfgLvdsMiscControl
    CFG_LVDS_MISC_VSYNC_ACTIVE_LOW,       // CfgLvdsMiscControl
    CFG_LVDS_MISC_HSYNC_ACTIVE_LOW,       // CfgLvdsMiscControl
    CFG_LVDS_MISC_BLON_ACTIVE_LOW,        // CfgLvdsMiscControl
    CFG_LVDS_MISC_VOLT_OVERWRITE_ENABLE,  // CfgLvdsMiscControl
  },
  CFG_PCIE_REFCLK_SPREAD_SPECTRUM,      // CfgPcieRefClkSpreadSpectrum
  CFG_ENABLE_EXTERNAL_VREF,             // CfgExternalVrefCtlFeature
  CFG_FORCE_TRAIN_MODE,                 // CfgForceTrainMode
  CFG_GNB_REMOTE_DISPLAY_SUPPORT,       // CfgGnbRemoteDisplaySupport
  (IOMMU_EXCLUSION_RANGE_DESCRIPTOR *) CFG_IOMMU_EXCLUSION_RANGE_LIST, // CfgIvrsExclusionRangeList
  CFG_GNB_SYNCFLOOD_PIN_AS_NMI,         // CfgGnbSyncFloodPinAsNmi
  CFG_IGPU_ENABLE_DISABLE_POLICY,       // CfgIgpuEnableDisablePolicy
  CFG_GNB_THERMAL_SENSOR_CORRECTION,    // CfgGnbSwTjOffset
  CFG_LVDS_MISC_VOLT_ADJUSTMENT,        // CfgLvdsMiscVoltAdjustment
  {
    0,                                       // Reserved
    CFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE, // CfgDisplayMiscControl.VbiosFastBootEn
    0,                                       // Reserved
  },
  CFG_DP_FIXED_VOLT_SWING,              // CfgDpFixedVoltSwingType
  CFG_DIMM_TYPE_USED_IN_MIXED_CONFIG,   // CfgDimmTypeUsedInMixedConfig
  CFG_HYBRID_BOOST_ENABLE,              // CfgHybridBoostEnable
  CFG_GNB_IOAPIC_ADDRESS,               // CfgGnbIoapicAddress
  CFG_ENABLE_DATA_EYE,                  // CfgDataEyeEn
  CFG_DOCKED_TDP_HEADROOM,              // CfgDockedTdpHeadroom
  CFG_BATTERY_BOOST_EN,                 // CfgBatteryBoostEn
  CFG_TDP_AC_DOCKED,                    // CfgTdpAcDocked
  CFG_TDP_DC_DOCKED,                    // CfgTdpDcDocked
  CFG_TDP_AC_UNDOCKED,                  // CfgTdpAcUnDocked
  CFG_TDP_DC_UNDOCKED,                  // CfgTdpDcUnDocked
  CFG_STAPM_EN_AC_DOCKED,               // CfgStapmEnAcDocked
  CFG_STAPM_EN_DC_DOCKED,               // CfgStapmEnDcDocked
  CFG_STAPM_EN_AC_UNDOCKED,             // CfgStapmEnAcUnDocked
  CFG_STAPM_EN_DC_UNDOCKED,             // CfgStapmEnDcUnDocked
  CFG_STAPM_POWER_AC_DOCKED,            // CfgStapmPowerAcDocked
  CFG_STAPM_POWER_DC_DOCKED,            // CfgStapmPowerDcDocked
  CFG_STAPM_POWER_AC_UNDOCKED,          // CfgStapmPowerAcUnDocked
  CFG_STAPM_POWER_DC_UNDOCKED,          // CfgStapmPowerDcUnDocked
  CFG_DRAM_DOUBLE_REFRESH_RATE,         // CfgDramDoubleRefreshRateEn
  CFG_ACP_SIZE,                         // CfgGnbAcpSize
  CFG_PMU_TRAIN_MODE,                   // CfgPmuTrainMode
  CFG_MEMORY_PHY_VOLTAGE,               // CfgMemoryPhyVoltage
  CFG_GPU_FREQUENCY_LIMIT,              // CfgGpuFrequencyLimit
  CFG_BAPM_ENABLE,                      // CfgBapmEnable
  0,                                    //reserved...
};

CONST FUNCTION_PARAMS_INFO ROMDATA FuncParamsInfo[] =
{
  #if AGESA_ENTRY_INIT_RESET == TRUE
    { AMD_INIT_RESET,
      sizeof (AMD_RESET_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitResetConstructor,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_INIT_RESET_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_RECOVERY == TRUE
    { AMD_INIT_RECOVERY,
      sizeof (AMD_RECOVERY_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitRecoveryInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_INIT_POST_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_EARLY == TRUE
    { AMD_INIT_EARLY,
      sizeof (AMD_EARLY_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitEarlyInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_INIT_EARLY_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_ENV == TRUE
    { AMD_INIT_ENV,
      sizeof (AMD_ENV_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitEnvInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_INIT_ENV_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_LATE == TRUE
    { AMD_INIT_LATE,
      sizeof (AMD_LATE_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitLateInitializer,
      (PF_AGESA_DESTRUCTOR) AmdInitLateDestructor,
      AMD_INIT_LATE_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_MID == TRUE
    { AMD_INIT_MID,
      sizeof (AMD_MID_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitMidInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_INIT_MID_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_POST == TRUE
    { AMD_INIT_POST,
      sizeof (AMD_POST_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitPostInitializer,
      (PF_AGESA_DESTRUCTOR) AmdInitPostDestructor,
      AMD_INIT_POST_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_RESUME == TRUE
    { AMD_INIT_RESUME,
      sizeof (AMD_RESUME_PARAMS),
      (PF_AGESA_FUNCTION) AmdInitResumeInitializer,
      (PF_AGESA_DESTRUCTOR) AmdInitResumeDestructor,
      AMD_INIT_RESUME_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_LATE_RESTORE == TRUE
    { AMD_S3LATE_RESTORE,
      sizeof (AMD_S3LATE_PARAMS),
      (PF_AGESA_FUNCTION) AmdS3LateRestoreInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_S3_LATE_RESTORE_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_FINAL_RESTORE == TRUE
    { AMD_S3FINAL_RESTORE,
      sizeof (AMD_S3FINAL_PARAMS),
      (PF_AGESA_FUNCTION) AmdS3FinalRestoreInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_S3_FINAL_RESTORE_HANDLE
    },
  #endif

  #if AGESA_ENTRY_INIT_S3SAVE == TRUE
    { AMD_S3_SAVE,
      sizeof (AMD_S3SAVE_PARAMS),
      (PF_AGESA_FUNCTION) AmdS3SaveInitializer,
      (PF_AGESA_DESTRUCTOR) AmdS3SaveDestructor,
      AMD_S3_SAVE_HANDLE
    },
  #endif

  #if AGESA_ENTRY_LATE_RUN_AP_TASK == TRUE
    { AMD_LATE_RUN_AP_TASK,
      sizeof (AP_EXE_PARAMS),
      (PF_AGESA_FUNCTION) AmdLateRunApTaskInitializer,
      (PF_AGESA_DESTRUCTOR) CommonReturnAgesaSuccess,
      AMD_LATE_RUN_AP_TASK_HANDLE
    },
  #endif
  { 0, 0, NULL, NULL, 0 }
};

CONST UINTN InitializerCount = ((sizeof (FuncParamsInfo)) / (sizeof (FuncParamsInfo[0])));

CONST DISPATCH_TABLE ROMDATA DispatchTable[] =
{
  { AMD_CREATE_STRUCT, (IMAGE_ENTRY)AmdCreateStruct },
  { AMD_RELEASE_STRUCT, (IMAGE_ENTRY)AmdReleaseStruct },

  #if AGESA_ENTRY_INIT_RESET == TRUE
    { AMD_INIT_RESET, (IMAGE_ENTRY)AmdInitReset },
  #endif

  #if AGESA_ENTRY_INIT_RECOVERY == TRUE
    { AMD_INIT_RECOVERY, (IMAGE_ENTRY)AmdInitRecovery },
  #endif

  #if AGESA_ENTRY_INIT_EARLY == TRUE
    { AMD_INIT_EARLY, (IMAGE_ENTRY)AmdInitEarly },
  #endif

  #if AGESA_ENTRY_INIT_POST == TRUE
    { AMD_INIT_POST, (IMAGE_ENTRY)AmdInitPost },
    #if OPTION_DATA_EYE == TRUE
      { AMD_GET_2D_DATA_EYE, (IMAGE_ENTRY)AmdGet2DDataEye },
    #endif
  #endif

  #if AGESA_ENTRY_INIT_ENV == TRUE
    { AMD_INIT_ENV, (IMAGE_ENTRY)AmdInitEnv },
  #endif

  #if AGESA_ENTRY_INIT_MID == TRUE
    { AMD_INIT_MID, (IMAGE_ENTRY)AmdInitMid },
  #endif

  #if AGESA_ENTRY_INIT_LATE == TRUE
    { AMD_INIT_LATE, (IMAGE_ENTRY)AmdInitLate },
  #endif

  #if AGESA_ENTRY_INIT_S3SAVE == TRUE
    { AMD_S3_SAVE, (IMAGE_ENTRY)AmdS3Save },
  #endif

  #if AGESA_ENTRY_INIT_RESUME == TRUE
    { AMD_INIT_RESUME, (IMAGE_ENTRY)AmdInitResume },
  #endif

  #if AGESA_ENTRY_INIT_LATE_RESTORE == TRUE
    { AMD_S3LATE_RESTORE, (IMAGE_ENTRY)AmdS3LateRestore },
  #endif

  #if AGESA_ENTRY_INIT_GENERAL_SERVICES == TRUE
    { AMD_GET_APIC_ID, (IMAGE_ENTRY)AmdGetApicId },
    { AMD_GET_PCI_ADDRESS, (IMAGE_ENTRY)AmdGetPciAddress },
    { AMD_IDENTIFY_CORE, (IMAGE_ENTRY)AmdIdentifyCore },
    { AMD_READ_EVENT_LOG, (IMAGE_ENTRY)AmdReadEventLog },
    { AMD_IDENTIFY_DIMMS, (IMAGE_ENTRY)AmdIdentifyDimm },
    { AMD_GET_EXECACHE_SIZE, (IMAGE_ENTRY)AmdGetAvailableExeCacheSize },
  #endif

  #if AGESA_ENTRY_LATE_RUN_AP_TASK == TRUE
    { AMD_LATE_RUN_AP_TASK, (IMAGE_ENTRY)AmdLateRunApTask },
  #endif

  { AMD_HEAP_ALLOCATE_BUFFER, (IMAGE_ENTRY)HeapAllocateBuffer },
  { AMD_HEAP_DEALLOCATE_BUFFER, (IMAGE_ENTRY)HeapDeallocateBuffer },

  { FCH_INIT_RESET, (IMAGE_ENTRY)FchInitReset },
  { FCH_INIT_ENV, (IMAGE_ENTRY)FchInitEnv },
  { FCH_INIT_MID, (IMAGE_ENTRY)FchInitMid },
  { FCH_INIT_LATE, (IMAGE_ENTRY)FchInitLate },
//  { FCH_INIT_S3_EARLY_RESTORE, (IMAGE_ENTRY)FchInitS3EarlyRestore },
//  { FCH_INIT_S3_LATE_RESTORE, (IMAGE_ENTRY)FchInitS3LateRestore },

  { AMD_GET_VALUE, (IMAGE_ENTRY)AmdGetValueDispatch },
  { AMD_SET_VALUE, (IMAGE_ENTRY)AmdSetValueDispatch },

  { 0, NULL }
};

CONST DISPATCH_TABLE ROMDATA ApDispatchTable[] =
{
  IDS_LATE_RUN_AP_TASK
  // Get DMI info
  CPU_DMI_AP_GET_TYPE4_TYPE7
  // Cpu Prefetch Mode
  CPU_PREFETCH_MODE_AP_TASK
  { 0, NULL }
};

#if AGESA_ENTRY_INIT_EARLY == TRUE
  #if IDSOPT_IDS_ENABLED == TRUE
    #if IDSOPT_TRACING_ENABLED == TRUE
      #define MAKE_DBG_STR(x, y)  MAKE_AS_A_STRING(x : y)
      CONST CHAR8 *BldOptDebugOutput[] = {
        #if IDS_TRACE_SHOW_BLD_OPT_CFG == TRUE
          //Build Option Area
          MAKE_DBG_STR (\nOptUDIMM,             OPTION_UDIMMS)
          MAKE_DBG_STR (\nOptRDIMM,             OPTION_RDIMMS)
          MAKE_DBG_STR (\nOptLRDIMM,            OPTION_LRDIMMS)
          MAKE_DBG_STR (\nOptECC,               OPTION_ECC)
          MAKE_DBG_STR (\nOptCsIntlv,           OPTION_BANK_INTERLEAVE)
          MAKE_DBG_STR (\nOptDctIntlv,          OPTION_DCT_INTERLEAVE)
          MAKE_DBG_STR (\nOptNodeIntlv,         OPTION_NODE_INTERLEAVE)
          //MAKE_DBG_STR (\nOptParallelTraining,  OPTION_PARALLEL_TRAINING)
          MAKE_DBG_STR (\nOptOnlineSpare,       OPTION_ONLINE_SPARE)
          MAKE_DBG_STR (\nOptAddr2CsTranslator, OPTION_ADDR_TO_CS_TRANSLATOR)
          MAKE_DBG_STR (\nOptMemRestore,        OPTION_MEM_RESTORE)
          MAKE_DBG_STR (\nOptMultiSocket,       OPTION_MULTISOCKET)
          MAKE_DBG_STR (\nOptPstates,           OPTION_ACPI_PSTATES)
          MAKE_DBG_STR (\nOptSRAT,              OPTION_SRAT)
          MAKE_DBG_STR (\nOptSLIT,              OPTION_SLIT)
          MAKE_DBG_STR (\nOptWHEA,              OPTION_WHEA)
          MAKE_DBG_STR (\nOptDMI,               OPTION_DMI)
          MAKE_DBG_STR (\nOptEarlySamples,      OPTION_EARLY_SAMPLES),

          //Build Configuration Area
          // CoreVrm
          MAKE_DBG_STR (\nVrmCurrentLimit      , CFG_VRM_CURRENT_LIMIT)
          MAKE_DBG_STR (\nVrmLowPowerThreshold , CFG_VRM_LOW_POWER_THRESHOLD)
          MAKE_DBG_STR (\nVrmSlewRate          , CFG_VRM_SLEW_RATE)
          MAKE_DBG_STR (\nVrmHiSpeedEnable     , CFG_VRM_HIGH_SPEED_ENABLE)
          MAKE_DBG_STR (\nVrmMaximumCurrentLimit, CFG_VRM_MAXIMUM_CURRENT_LIMIT)
          MAKE_DBG_STR (\nVrmSviOcpLevel,        CFG_VRM_SVI_OCP_LEVEL)
          // NbVrm
          MAKE_DBG_STR (\nNbVrmCurrentLimit      , CFG_VRM_NB_CURRENT_LIMIT)
          MAKE_DBG_STR (\nNbVrmLowPowerThreshold , CFG_VRM_NB_LOW_POWER_THRESHOLD)
          MAKE_DBG_STR (\nNbVrmSlewRate          , CFG_VRM_NB_SLEW_RATE)
          MAKE_DBG_STR (\nNbVrmHiSpeedEnable     , CFG_VRM_NB_HIGH_SPEED_ENABLE)
          MAKE_DBG_STR (\nNbVrmMaximumCurrentLimit, CFG_VRM_NB_MAXIMUM_CURRENT_LIMIT),
          MAKE_DBG_STR (\nNbVrmSviOcpLevel,        CFG_VRM_NB_SVI_OCP_LEVEL)

          MAKE_DBG_STR (\nCfgCpuFrequencyLimit  , CFG_CPU_FREQUENCY_LIMIT),
          MAKE_DBG_STR (\nNumIoApics            , CFG_PLAT_NUM_IO_APICS)
          MAKE_DBG_STR (\nMemInitPstate         , CFG_MEM_INIT_PSTATE)
          MAKE_DBG_STR (\nCStateMode            , CFG_CSTATE_MODE)
          MAKE_DBG_STR (\nCStateOpData          , CFG_CSTATE_OPDATA)
          MAKE_DBG_STR (\nCStateIoBaseAddr      , CFG_CSTATE_IO_BASE_ADDRESS)
          MAKE_DBG_STR (\nCpbMode               , CFG_CPB_MODE)
          MAKE_DBG_STR (\nCoreLevelingMode      , CFG_CORE_LEVELING_MODE),

          MAKE_DBG_STR (\nControlFlowMode       , CFG_PLATFORM_CONTROL_FLOW_MODE)
          MAKE_DBG_STR (\nUse32ByteRefresh      , CFG_USE_32_BYTE_REFRESH)
          MAKE_DBG_STR (\nUseVarMctIsocPriority , CFG_USE_VARIABLE_MCT_ISOC_PRIORITY)
          MAKE_DBG_STR (\nPowerPolicy           , CFG_PLATFORM_POWER_POLICY_MOD)

          MAKE_DBG_STR (\nPciMmioAddr           , CFG_PCI_MMIO_BASE)
          MAKE_DBG_STR (\nPciMmioSize           , CFG_PCI_MMIO_SIZE)
          MAKE_DBG_STR (\nPlatformType          , CFG_AMD_PLATFORM_TYPE)
          MAKE_DBG_STR (\nPowerCeiling          , CFG_AMD_POWER_CEILING),
          MAKE_DBG_STR (\nHtcTempLimit          , CFG_HTC_TEMPERATURE_LIMIT)
          MAKE_DBG_STR (\nLhtcTempLimit         , CFG_LHTC_TEMPERATURE_LIMIT)

          MAKE_DBG_STR (\nMemBusFreqLimit       , CFG_MEMORY_BUS_FREQUENCY_LIMIT)
          MAKE_DBG_STR (\nTimingModeSelect      , CFG_TIMING_MODE_SELECT)
          MAKE_DBG_STR (\nMemoryClockSelect     , CFG_MEMORY_CLOCK_SELECT)

          MAKE_DBG_STR (\nMemUnganged           , CFG_MEMORY_MODE_UNGANGED)
          MAKE_DBG_STR (\nQRCap                 , CFG_MEMORY_QUAD_RANK_CAPABLE)
          MAKE_DBG_STR (\nQRType                , CFG_MEMORY_QUADRANK_TYPE)
          MAKE_DBG_STR (\nRDimmCap              , CFG_MEMORY_RDIMM_CAPABLE)
          MAKE_DBG_STR (\nLRDimmCap             , CFG_MEMORY_LRDIMM_CAPABLE)
          MAKE_DBG_STR (\nUDimmCap              , CFG_MEMORY_UDIMM_CAPABLE)
          MAKE_DBG_STR (\nSODimmCap             , CFG_MEMORY_SODIMM_CAPABLE)
          MAKE_DBG_STR (\nDqsTrainingControl    , CFG_DQS_TRAINING_CONTROL)
          MAKE_DBG_STR (\nIgnoreSpdChecksum     , CFG_IGNORE_SPD_CHECKSUM)
          MAKE_DBG_STR (\nUseBurstMode          , CFG_USE_BURST_MODE)
          MAKE_DBG_STR (\nAllMemClkOn           , CFG_MEMORY_ALL_CLOCKS_ON),

          MAKE_DBG_STR (\nPowerDownEn           , CFG_MEMORY_POWER_DOWN)
          MAKE_DBG_STR (\nPowerDownMode         , CFG_POWER_DOWN_MODE)
          MAKE_DBG_STR (\nOnlineSpare           , CFG_ONLINE_SPARE)
          MAKE_DBG_STR (\nAddrParityEn          , CFG_MEMORY_PARITY_ENABLE)
          MAKE_DBG_STR (\nBankSwizzle           , CFG_BANK_SWIZZLE)
          MAKE_DBG_STR (\nLimitBelow1TB         , CFG_LIMIT_MEMORY_TO_BELOW_1TB)
          MAKE_DBG_STR (\nCsIntlvEn             , CFG_MEMORY_ENABLE_BANK_INTERLEAVING)
          MAKE_DBG_STR (\nNodeIntlvEn           , CFG_MEMORY_ENABLE_NODE_INTERLEAVING)
          MAKE_DBG_STR (\nDctIntlvEn            , CFG_MEMORY_CHANNEL_INTERLEAVING),

          MAKE_DBG_STR (\nUmaMode                , CFG_UMA_MODE)
          MAKE_DBG_STR (\nUmaSize                , CFG_UMA_SIZE)
          MAKE_DBG_STR (\nUmaAbove4G             , CFG_UMA_ABOVE4G)
          MAKE_DBG_STR (\nUmaAlignment           , CFG_UMA_ALIGNMENT)

          MAKE_DBG_STR (\nEccEn                 , CFG_ENABLE_ECC_FEATURE)
          MAKE_DBG_STR (\nEccRedirect           , CFG_ECC_REDIRECTION)
          MAKE_DBG_STR (\nScrubDramRate         , CFG_SCRUB_DRAM_RATE)
          MAKE_DBG_STR (\nScrubL2Rate           , CFG_SCRUB_L2_RATE)
          MAKE_DBG_STR (\nScrubL3Rate           , CFG_SCRUB_L3_RATE)
          MAKE_DBG_STR (\nScrubIcRate           , CFG_SCRUB_IC_RATE)
          MAKE_DBG_STR (\nScrubDcRate           , CFG_SCRUB_DC_RATE)
          MAKE_DBG_STR (\nEccSyncFlood          , CFG_ECC_SYNC_FLOOD)
          MAKE_DBG_STR (\nEccSymbolSize         , CFG_ECC_SYMBOL_SIZE)
          MAKE_DBG_STR (\nHeapDramAddress       , CFG_HEAP_DRAM_ADDRESS)
          MAKE_DBG_STR (\nNodeMem1GBAlign       , CFG_1GB_ALIGN),

          MAKE_DBG_STR (\nS3LateRestore         , CFG_S3_LATE_RESTORE)
          MAKE_DBG_STR (\nAcpiPstateIndependent , CFG_ACPI_PSTATE_PSD_INDPX)
          MAKE_DBG_STR (\nAcpiPstatesPsdPolicy  , CFG_ACPI_PSTATES_PSD_POLICY)

          MAKE_DBG_STR (\nApMtrrSettingsList     , CFG_AP_MTRR_SETTINGS_LIST)

          MAKE_DBG_STR (\nProcessorScopeInSb     , CFG_PROCESSOR_SCOPE_IN_SB)
          MAKE_DBG_STR (\nProcessorScopeName0    , CFG_PROCESSOR_SCOPE_NAME0)
          MAKE_DBG_STR (\nProcessorScopeName1    , CFG_PROCESSOR_SCOPE_NAME1)
          MAKE_DBG_STR (\nGnbHdAudio             , CFG_GNB_HD_AUDIO)
          MAKE_DBG_STR (\nAbmSupport             , CFG_ABM_SUPPORT)
          MAKE_DBG_STR (\nDynamicRefreshRate     , CFG_DYNAMIC_REFRESH_RATE)
          MAKE_DBG_STR (\nLcdBackLightControl    , CFG_LCD_BACK_LIGHT_CONTROL)
          MAKE_DBG_STR (\nGnb3dStereoPinIndex    , CFG_GNB_STEREO_3D_PINOUT)
          MAKE_DBG_STR (\nTempPcieMmioBaseAddress, CFG_TEMP_PCIE_MMIO_BASE_ADDRESS),
          MAKE_DBG_STR (\nCfgGnbIGPUSSID         , CFG_GNB_IGPU_SSID)
          MAKE_DBG_STR (\nCfgGnbHDAudioSSID      , CFG_GNB_HDAUDIO_SSID)
          MAKE_DBG_STR (\nCfgGnbPcieSSID         , CFG_GNB_PCIE_SSID)
          MAKE_DBG_STR (\nCfgIommuSupport        , CFG_IOMMU_SUPPORT)
          MAKE_DBG_STR (\nCfgLvdsSpreadSpectrum  , CFG_GFX_LVDS_SPREAD_SPECTRUM)
          MAKE_DBG_STR (\nCfgLvdsSpreadSpectrumRate , CFG_GFX_LVDS_SPREAD_SPECTRUM_RATE)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqDigonToDe    , CFG_LVDS_POWER_ON_SEQ_DIGON_TO_DE)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqDeToVaryBl   , CFG_LVDS_POWER_ON_SEQ_DE_TO_VARY_BL)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqDeToDigon    , CFG_LVDS_POWER_ON_SEQ_DE_TO_DIGON)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqVaryBlToDe   , CFG_LVDS_POWERS_ON_SEQ_VARY_BL_TO_DE)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqOnToOffDelay , CFG_LVDS_POWER_ON_SEQ_ON_TO_OFF_DELAY)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqVaryBlToBlon , CFG_LVDS_POWER_ON_SEQ_VARY_BL_TO_BLON)
          MAKE_DBG_STR (\nCfgLvdsPowerOnSeqBlonToVaryBl , CFG_LVDS_POWER_ON_SEQ_BLON_TO_VARY_BL)
          MAKE_DBG_STR (\nCfgLvdsMaxPixelClockFreq      , CFG_LVDS_MAX_PIXEL_CLOCK_FREQ)
          MAKE_DBG_STR (\nCfgLcdBitDepthControlValue    , CFG_LCD_BIT_DEPTH_CONTROL_VALUE)
          MAKE_DBG_STR (\nCfgLvds24bbpPanelMode         , CFG_LVDS_24BBP_PANEL_MODE),
          MAKE_DBG_STR (\nCfgLvdsMiscControl.FpdiMode   , CFG_LVDS_MISC_888_FPDI_MODE),
          MAKE_DBG_STR (\nCfgLvdsMiscControl.DlChSwap           , CFG_LVDS_MISC_DL_CH_SWAP),
          MAKE_DBG_STR (\nCfgLvdsMiscControl.VsyncActiveLow     , CFG_LVDS_MISC_VSYNC_ACTIVE_LOW),
          MAKE_DBG_STR (\nCfgLvdsMiscControl.HsyncActiveLow     , CFG_LVDS_MISC_HSYNC_ACTIVE_LOW),
          MAKE_DBG_STR (\nCfgLvdsMiscControl.BLONActiveLow      , CFG_LVDS_MISC_BLON_ACTIVE_LOW),
          MAKE_DBG_STR (\nCfgPcieRefClkSpreadSpectrum   , CFG_PCIE_REFCLK_SPREAD_SPECTRUM),
          MAKE_DBG_STR (\nCfgExtVref                    , CFG_ENABLE_EXTERNAL_VREF),
          MAKE_DBG_STR (\nCfgForceTrainMode             , CFG_FORCE_TRAIN_MODE),
          MAKE_DBG_STR (\nCfgGnbRemoteDisplaySupport    , CFG_GNB_REMOTE_DISPLAY_CONFIG),
          MAKE_DBG_STR (\nCfgIvrsExclusionRangeList     , CFG_IOMMU_EXCLUSION_RANGE_LIST),
          MAKE_DBG_STR (\nCfgGnbSyncFloodPinAsNmi       , CFG_GNB_SYNCFLOOD_PIN_AS_NMI),
          MAKE_DBG_STR (\nCfgIgpuEnableDisablePolicy    , CFG_IGPU_ENABLE_DISABLE_POLICY),
          MAKE_DBG_STR (\nCfgGnbSwTjOffset               , CFG_GNB_THERMAL_SENSOR_CORRECTION),
          MAKE_DBG_STR (\nCfgDisplayMiscControl.VbiosFastBootEn   , CFG_DISPLAY_MISC_VBIOS_FAST_BOOT_ENABLE),
          MAKE_DBG_STR (\nCfgDimmTypeUsedInMixedConfig  , CFG_DIMM_TYPE_USED_IN_MIXED_CONFIG),
          MAKE_DBG_STR (\nCfgDataEyeEn                  , CFG_ENABLE_DATA_EYE),
          MAKE_DBG_STR (\nCfgBatteryBoostEn             , CFG_BATTERY_BOOST_EN),
          MAKE_DBG_STR (\nCfgTdpAcDocked                , CFG_TDP_AC_DOCKED),
          MAKE_DBG_STR (\nCfgTdpDcDocked                , CFG_TDP_DC_DOCKED),
          MAKE_DBG_STR (\nCfgTdpAcUnDocked              , CFG_TDP_AC_UNDOCKED),
          MAKE_DBG_STR (\nCfgTdpDcUnDocked              , CFG_TDP_DC_UNDOCKED),
          MAKE_DBG_STR (\nCfgStapmEnAcDocked            , CFG_STAPM_EN_AC_DOCKED),
          MAKE_DBG_STR (\nCfgStapmEnDcDocked            , CFG_STAPM_EN_DC_DOCKED),
          MAKE_DBG_STR (\nCfgStapmEnAcUnDocked          , CFG_STAPM_EN_AC_UNDOCKED),
          MAKE_DBG_STR (\nCfgStapmEnDcUnDocked          , CFG_STAPM_EN_DC_UNDOCKED),
          MAKE_DBG_STR (\nCfgStapmPowerAcDocked         , CFG_STAPM_POWER_AC_DOCKED),
          MAKE_DBG_STR (\nCfgStapmPowerDcDocked         , CFG_STAPM_POWER_DC_DOCKED),
          MAKE_DBG_STR (\nCfgStapmPowerAcUnDocked       , CFG_STAPM_POWER_AC_UNDOCKED),
          MAKE_DBG_STR (\nCfgStapmPowerDcUnDocked       , CFG_STAPM_POWER_DC_UNDOCKED),
          MAKE_DBG_STR (\nCfgDramDoubleRefreshRateEn    , CFG_DRAM_DOUBLE_REFRESH_RATE),
          MAKE_DBG_STR (\nCfgGnbAcpSize                 , CFG_ACP_SIZE),
          MAKE_DBG_STR (\nCfgPmuTrainMode               , CFG_PMU_TRAIN_MODE),
          MAKE_DBG_STR (\nCfgGpuFrequencyLimit          , CFG_GPU_FREQUENCY_LIMIT),
          MAKE_DBG_STR (\nCfgBapmEnable                 , CFG_BAPM_ENABLE),
        #endif
        NULL
      };
    #endif
  #endif
#endif

// Needed for floating point support, linker expects this symbol to be defined.
#if (OPTION_CPU_SCS == TRUE) || (CFG_GNB_BAPM_SUPPORT_TN == TRUE)
  CONST INT32 _fltused = 0;
#endif
