/* $NoKeywords:$ */
/**
 * @file
 *
 * AGESA gnb file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  GNB
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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
#ifndef _GNBREGISTERSKV_H_
#define _GNBREGISTERSKV_H_
#define  TYPE_D0F0                  0x1
#define  TYPE_D0F0x64               0x2
#define  TYPE_D0F0x98               0x3
#define  TYPE_D0F0xBC               0x4
#define  TYPE_D0F0xE4               0x5
#define  TYPE_DxF0                  0x6
#define  TYPE_DxF0xE4               0x7
#define  TYPE_D0F2                  0x8
#define  TYPE_D0F2xF4               0x9
#define  TYPE_D0F2xFC               0xa
#define  TYPE_D18F1                 0xb
#define  TYPE_D18F2                 0xc
#define  TYPE_D18F3                 0xd
#define  TYPE_D18F4                 0xe
#define  TYPE_D18F5                 0xf
#define  TYPE_MSR                   0x10
#define  TYPE_D1F0                  0x11
#define  TYPE_GMM                   0x12
#define  TYPE_D1F1                  0x13
#define  TYPE_D18F2_dct0            0x14
#define  TYPE_D18F2_dct1            0x15
#define  TYPE_D18F2_dct2            0x16
#define  TYPE_D18F2_dct3            0x17
#define  TYPE_D18F2_dct0_mp0        0x18
#define  TYPE_D18F2_dct0_mp1        0x19
#define  TYPE_D18F2_dct1_mp0        0x1a
#define  TYPE_D18F2_dct1_mp1        0x1b
#define  TYPE_D18F2_dct2_mp0        0x1c
#define  TYPE_D18F2_dct2_mp1        0x1d
#define  TYPE_D18F2_dct3_mp0        0x1e
#define  TYPE_D18F2_dct3_mp1        0x1f
#define  TYPE_CGIND                 0x20
#define  TYPE_SMU_MSG               0x21
#define  TYPE_D0F0xD4               0x22
#define  TYPE_D0F0xFC               0x23
#define  TYPE_D18F0                 0x24

#ifndef WRAP_SPACE
  #define  WRAP_SPACE(w, x)   (0x01300000 | (w << 16) | (x))
#endif
#ifndef CORE_SPACE
  #define  CORE_SPACE(c, x)   (0x00010000 | (c << 24) | (x))
#endif
#ifndef PHY_SPACE
  #define  PHY_SPACE(w, p, x) (0x00200000 | ((p + 1) << 24) | (w << 16) | (x))
#endif
#ifndef PIF_SPACE
  #define  PIF_SPACE(w, p, x) (0x00100000 | ((p + 1) << 24) | (w << 16) | (x))
#endif

#define  SMU_MSG_TYPE                       TYPE_SMU_MSG
#define  SMC_MSG_TEST                       0x1
#define  SMC_MSG_PHY_LN_OFF                 2
#define  SMC_MSG_PHY_LN_ON                  3
#define  SMC_MSG_DDI_PHY_OFF                4
#define  SMC_MSG_DDI_PHY_ON                 5
#define  SMC_MSG_RECONFIGURE_SB             0x19
#define  SMC_MSG_FORCE_LCLK_DPM_STATE       0x1c
#define  SMC_MSG_UNFORCE_LCLK_DPM_STATE     0x1d
#define  SMC_MSG_DSMU                       0x24
#define  SMC_MSG_RESET                      0x29
#define  SMC_MSG_VDDNB_REQUEST              0x3a

#define  SMC_MSG_VOLTAGE_CNTL_ENABLE        59
#define  SMC_MSG_VOLTAGE_CNTL_DISABLE       60

#define  SMC_MSG_PWR_OFF_IOMMU              0x48
#define  SMC_MSG_PWR_ON_IOMMU               0x49
#define  SMC_MSG_READ_SCS                   0x50
#define  SMC_MSG_ENABLE_ALL_SMU_FEATURES    0x5f
#define  SMC_MSG_DISABLE_ALL_SMU_FEATURES   0x60
#define  SMC_MSG_POWER_SOURCE_AC            0x61
#define  SMC_MSG_POWER_SOURCE_DC            0x62

#define  BAPM_MASK                          0x0001
#define  POWER_ESTIMATOR_MASK               0x0002
#define  THERMAL_CONTROLLER_MASK            0x0004
#define  TDC_LIMITING_MASK                  0x0008
#define  PKG_PWR_LIMITING_MASK              0x0010
#define  LCLK_DPM_MASK                      0x0020
#define  VOLTAGE_CONTROLLER_MASK            0x0040
#define  EAPM_MASK                          0x0080
#define  C7_MASK                            0x0100
#define  IBS_MASK                           0x0200
#define  PSTATE_ARBITER_MASK                0x0400
#define  NB_DPM_MASK                        0x0800
#define  LPMX_MASK                          0x1000
#define  WHISPER_MODE_MASK                  0x2000
#define  GFX_CU_PG_MASK                     0x4000
#define  AVS_MASK                           0x8000
#define  MULTI_VID_MASK                     0x10000
#define  CHTC_MASK                          0x20000
#define  HYBRID_BOOST_MASK                  0x40000
#define  SCLK_DPM_MASK                      0x80000
#define  PRE_FETCH_DATA_MASK                0x100000
#define  LCLK_DEEP_SLEEP_MASK               0x200000
#define  CLK_MONITOR_MASK                   0x400000

#define  L1_SEL_GFX                         0
#define  L1_SEL_GPP                         1
#define  L1_SEL_GPPSB                       2
#define  L1_SEL_GBIF                        3
#define  L1_SEL_INTGEN                      4

// **** D0F0x04 Register Definition ****
// Address
#define D0F0x04_ADDRESS                                         0x4
// Type
#define D0F0x04_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x04_IoAccessEn_OFFSET                               0
#define D0F0x04_IoAccessEn_WIDTH                                1
#define D0F0x04_IoAccessEn_MASK                                 0x1
#define D0F0x04_MemAccessEn_OFFSET                              1
#define D0F0x04_MemAccessEn_WIDTH                               1
#define D0F0x04_MemAccessEn_MASK                                0x2
#define D0F0x04_BusMasterEn_OFFSET                              2
#define D0F0x04_BusMasterEn_WIDTH                               1
#define D0F0x04_BusMasterEn_MASK                                0x4
#define D0F0x04_SpecialCycleEn_OFFSET                           3
#define D0F0x04_SpecialCycleEn_WIDTH                            1
#define D0F0x04_SpecialCycleEn_MASK                             0x8
#define D0F0x04_MemWriteInvalidateEn_OFFSET                     4
#define D0F0x04_MemWriteInvalidateEn_WIDTH                      1
#define D0F0x04_MemWriteInvalidateEn_MASK                       0x10
#define D0F0x04_PalSnoopEn_OFFSET                               5
#define D0F0x04_PalSnoopEn_WIDTH                                1
#define D0F0x04_PalSnoopEn_MASK                                 0x20
#define D0F0x04_ParityErrorEn_OFFSET                            6
#define D0F0x04_ParityErrorEn_WIDTH                             1
#define D0F0x04_ParityErrorEn_MASK                              0x40
#define D0F0x04_Reserved_7_7_OFFSET                             7
#define D0F0x04_Reserved_7_7_WIDTH                              1
#define D0F0x04_Reserved_7_7_MASK                               0x80
#define D0F0x04_SerrEn_OFFSET                                   8
#define D0F0x04_SerrEn_WIDTH                                    1
#define D0F0x04_SerrEn_MASK                                     0x100
#define D0F0x04_FastB2BEn_OFFSET                                9
#define D0F0x04_FastB2BEn_WIDTH                                 1
#define D0F0x04_FastB2BEn_MASK                                  0x200
#define D0F0x04_Reserved_19_10_OFFSET                           10
#define D0F0x04_Reserved_19_10_WIDTH                            10
#define D0F0x04_Reserved_19_10_MASK                             0xFFC00
#define D0F0x04_CapList_OFFSET                                  20
#define D0F0x04_CapList_WIDTH                                   1
#define D0F0x04_CapList_MASK                                    0x100000
#define D0F0x04_PCI66En_OFFSET                                  21
#define D0F0x04_PCI66En_WIDTH                                   1
#define D0F0x04_PCI66En_MASK                                    0x200000
#define D0F0x04_Reserved_22_22_OFFSET                           22
#define D0F0x04_Reserved_22_22_WIDTH                            1
#define D0F0x04_Reserved_22_22_MASK                             0x400000
#define D0F0x04_FastBackCapable_OFFSET                          23
#define D0F0x04_FastBackCapable_WIDTH                           1
#define D0F0x04_FastBackCapable_MASK                            0x800000
#define D0F0x04_Reserved_24_24_OFFSET                           24
#define D0F0x04_Reserved_24_24_WIDTH                            1
#define D0F0x04_Reserved_24_24_MASK                             0x1000000
#define D0F0x04_DevselTiming_OFFSET                             25
#define D0F0x04_DevselTiming_WIDTH                              2
#define D0F0x04_DevselTiming_MASK                               0x6000000
#define D0F0x04_SignalTargetAbort_OFFSET                        27
#define D0F0x04_SignalTargetAbort_WIDTH                         1
#define D0F0x04_SignalTargetAbort_MASK                          0x8000000
#define D0F0x04_ReceivedTargetAbort_OFFSET                      28
#define D0F0x04_ReceivedTargetAbort_WIDTH                       1
#define D0F0x04_ReceivedTargetAbort_MASK                        0x10000000
#define D0F0x04_ReceivedMasterAbort_OFFSET                      29
#define D0F0x04_ReceivedMasterAbort_WIDTH                       1
#define D0F0x04_ReceivedMasterAbort_MASK                        0x20000000
#define D0F0x04_SignaledSystemError_OFFSET                      30
#define D0F0x04_SignaledSystemError_WIDTH                       1
#define D0F0x04_SignaledSystemError_MASK                        0x40000000
#define D0F0x04_ParityErrorDetected_OFFSET                      31
#define D0F0x04_ParityErrorDetected_WIDTH                       1
#define D0F0x04_ParityErrorDetected_MASK                        0x80000000

/// D0F0x04
typedef union {
  struct {                                                            ///<
    UINT32                                              IoAccessEn:1; ///<
    UINT32                                             MemAccessEn:1; ///<
    UINT32                                             BusMasterEn:1; ///<
    UINT32                                          SpecialCycleEn:1; ///<
    UINT32                                    MemWriteInvalidateEn:1; ///<
    UINT32                                              PalSnoopEn:1; ///<
    UINT32                                           ParityErrorEn:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                                  SerrEn:1; ///<
    UINT32                                               FastB2BEn:1; ///<
    UINT32                                          Reserved_19_10:10; ///<
    UINT32                                                 CapList:1; ///<
    UINT32                                                 PCI66En:1; ///<
    UINT32                                          Reserved_22_22:1; ///<
    UINT32                                         FastBackCapable:1; ///<
    UINT32                                          Reserved_24_24:1; ///<
    UINT32                                            DevselTiming:2; ///<
    UINT32                                       SignalTargetAbort:1; ///<
    UINT32                                     ReceivedTargetAbort:1; ///<
    UINT32                                     ReceivedMasterAbort:1; ///<
    UINT32                                     SignaledSystemError:1; ///<
    UINT32                                     ParityErrorDetected:1; ///<

  } Field;

  UINT32 Value;
} D0F0x04_STRUCT;

// **** D0F0x64 Register Definition ****
// Address
#define D0F0x64_ADDRESS                                         0x64
// Type
#define D0F0x64_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x64_MiscIndData_OFFSET                              0
#define D0F0x64_MiscIndData_WIDTH                               32
#define D0F0x64_MiscIndData_MASK                                0xFFFFFFFF

/// D0F0x64
typedef union {
  struct {                                                            ///<
    UINT32                                             MiscIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0x64_STRUCT;

// **** D0F0x7C Register Definition ****
// Address
#define D0F0x7C_ADDRESS                                         0x7C
// Type
#define D0F0x7C_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x7C_ForceIntGfxDisable_OFFSET                       0
#define D0F0x7C_ForceIntGfxDisable_WIDTH                        1
#define D0F0x7C_ForceIntGfxDisable_MASK                         0x1
#define D0F0x7C_Reserved_31_1_OFFSET                            1
#define D0F0x7C_Reserved_31_1_WIDTH                             31
#define D0F0x7C_Reserved_31_1_MASK                              0xFFFFFFFE

/// D0F0x7C
typedef union {
  struct {                                                            ///<
    UINT32                                      ForceIntGfxDisable:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0x7C_STRUCT;

// **** D0F0x98 Register Definition ****
// Address
#define D0F0x98_ADDRESS                                         0x98
// Type
#define D0F0x98_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0x98_OrbIndData_OFFSET                               0
#define D0F0x98_OrbIndData_WIDTH                                32
#define D0F0x98_OrbIndData_MASK                                 0xFFFFFFFF

/// D0F0x98
typedef union {
  struct {                                                            ///<
    UINT32                                              OrbIndData:32; ///<

  } Field;

  UINT32 Value;
} D0F0x98_STRUCT;

// **** D0F0xE4 Register Definition ****
// Address
#define D0F0xE4_ADDRESS                                         0xE4
// Type
#define D0F0xE4_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xE4_PcieIndxData_OFFSET                             0
#define D0F0xE4_PcieIndxData_WIDTH                              32
#define D0F0xE4_PcieIndxData_MASK                               0xFFFFFFFF

/// D0F0xE4
typedef union {
  struct {                                                            ///<
    UINT32                                            PcieIndxData:32; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_STRUCT;

// **** D0F2x70 Register Definition ****
// Address
#define D0F2x70_ADDRESS                                         0x70
// Type
#define D0F2x70_TYPE                                            TYPE_D0F2

// Field Data
#define D0F2x70_PrefSupW_OFFSET                                 0
#define D0F2x70_PrefSupW_WIDTH                                  1
#define D0F2x70_PrefSupW_MASK                                   0x1
#define D0F2x70_PprSupW_OFFSET                                  1
#define D0F2x70_PprSupW_WIDTH                                   1
#define D0F2x70_PprSupW_MASK                                    0x2
#define D0F2x70_Reserved_2_2_OFFSET                             2
#define D0F2x70_Reserved_2_2_WIDTH                              1
#define D0F2x70_Reserved_2_2_MASK                               0x4
#define D0F2x70_NxSupW_OFFSET                                   3
#define D0F2x70_NxSupW_WIDTH                                    1
#define D0F2x70_NxSupW_MASK                                     0x8
#define D0F2x70_GtSupW_OFFSET                                   4
#define D0F2x70_GtSupW_WIDTH                                    1
#define D0F2x70_GtSupW_MASK                                     0x10
#define D0F2x70_Reserved_5_5_OFFSET                             5
#define D0F2x70_Reserved_5_5_WIDTH                              1
#define D0F2x70_Reserved_5_5_MASK                               0x20
#define D0F2x70_IaSupW_OFFSET                                   6
#define D0F2x70_IaSupW_WIDTH                                    1
#define D0F2x70_IaSupW_MASK                                     0x40
#define D0F2x70_Reserved_7_7_OFFSET                             7
#define D0F2x70_Reserved_7_7_WIDTH                              1
#define D0F2x70_Reserved_7_7_MASK                               0x80
#define D0F2x70_Reserved_8_8_OFFSET                             8
#define D0F2x70_Reserved_8_8_WIDTH                              1
#define D0F2x70_Reserved_8_8_MASK                               0x100
#define D0F2x70_PcSupW_OFFSET                                   9
#define D0F2x70_PcSupW_WIDTH                                    1
#define D0F2x70_PcSupW_MASK                                     0x200
#define D0F2x70_HatsW_OFFSET                                    10
#define D0F2x70_HatsW_WIDTH                                     2
#define D0F2x70_HatsW_MASK                                      0xC00
#define D0F2x70_Reserved_15_12_OFFSET                           12
#define D0F2x70_Reserved_15_12_WIDTH                            4
#define D0F2x70_Reserved_15_12_MASK                             0xf000
#define D0F2x70_SmifSupW_OFFSET                                 16
#define D0F2x70_SmifSupW_WIDTH                                  2
#define D0F2x70_SmifSupW_MASK                                   0x30000
#define D0F2x70_Reserved_31_18_OFFSET                           18
#define D0F2x70_Reserved_31_18_WIDTH                            14
#define D0F2x70_Reserved_31_18_MASK                             0xfffc0000

/// D0F2x70
typedef union {
  struct {                                                            ///<
    UINT32                                                PrefSupW:1; ///<
    UINT32                                                 PprSupW:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                                  NxSupW:1; ///<
    UINT32                                                  GtSupW:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                                  IaSupW:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                                  PcSupW:1; ///<
    UINT32                                                   HatsW:2; ///<
    UINT32                                          Reserved_15_12:4 ; ///<
    UINT32                                                SmifSupW:2 ; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F2x70_STRUCT;

// **** D0F2xF4 Register Definition ****
// Address
#define D0F2xF4_ADDRESS                                         0xF4
// Type
#define D0F2xF4_TYPE                                            TYPE_D0F2

// Field Data
#define D0F2xF4_L2cfgData_OFFSET                                0
#define D0F2xF4_L2cfgData_WIDTH                                 32
#define D0F2xF4_L2cfgData_MASK                                  0xFFFFFFFF

/// D0F2xF4
typedef union {
  struct {                                                            ///<
    UINT32                                               L2cfgData:32; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_STRUCT;

// **** D18F0x110 Register Definition ****
// Address
#define D18F0x110_ADDRESS                                       0x110

// Type
#define D18F0x110_TYPE                                          TYPE_D18F0

// **** D18F0x114 Register Definition ****
// Address
#define D18F0x114_ADDRESS                                       0x114

// Type
#define D18F0x114_TYPE                                          TYPE_D18F0
// Field Data
#define D18F0x114_Reserved_0_0_OFFSET                           0
#define D18F0x114_Reserved_0_0_WIDTH                            1
#define D18F0x114_Reserved_0_0_MASK                             0x1
#define D18F0x114_ClumpEn_OFFSET                                1
#define D18F0x114_ClumpEn_WIDTH                                 31
#define D18F0x114_ClumpEn_MASK                                  0xfffffffe

/// D18F0x114
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                                  ClumpEn:31; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F0x114_STRUCT;

// **** D18F1x44 Register Definition ****
// Address
#define D18F1x44_ADDRESS                                        0x44
// Type
#define D18F1x44_TYPE                                           TYPE_D18F1

// Field Data
#define D18F1x44_DstNode_OFFSET                                 0
#define D18F1x44_DstNode_WIDTH                                  3
#define D18F1x44_DstNode_MASK                                   0x7
#define D18F1x44_Reserved_7_3_OFFSET                            3
#define D18F1x44_Reserved_7_3_WIDTH                             5
#define D18F1x44_Reserved_7_3_MASK                              0xF8
#define D18F1x44_Reserved_10_8_OFFSET                           8
#define D18F1x44_Reserved_10_8_WIDTH                            3
#define D18F1x44_Reserved_10_8_MASK                             0x700
#define D18F1x44_Reserved_15_11_OFFSET                          11
#define D18F1x44_Reserved_15_11_WIDTH                           5
#define D18F1x44_Reserved_15_11_MASK                            0xF800
#define D18F1x44_DramLimit_39_24_OFFSET                         16
#define D18F1x44_DramLimit_39_24_WIDTH                          16
#define D18F1x44_DramLimit_39_24_MASK                           0xFFFF0000

/// D18F1x44
typedef union {
  struct {                                                            ///<
    UINT32                                                 DstNode:3; ///<
    UINT32                                            Reserved_7_3:5; ///<
    UINT32                                           Reserved_10_8:3; ///<
    UINT32                                          Reserved_15_11:5; ///<
    UINT32                                         DramLimit_39_24:16; ///<

  } Field;

  UINT32 Value;
} D18F1x44_STRUCT;

// **** D18F1x10C Register Definition ****
// Address
#define D18F1x10C_ADDRESS                                       0x10C
// Type
#define D18F1x10C_TYPE                                          TYPE_D18F1

// Field Data
#define D18F1x10C_DctCfgSel_OFFSET                              0
#define D18F1x10C_DctCfgSel_WIDTH                               3
#define D18F1x10C_DctCfgSel_MASK                                0x7
#define D18F1x10C_MemPsSel_OFFSET                               3
#define D18F1x10C_MemPsSel_WIDTH                                1
#define D18F1x10C_MemPsSel_MASK                                 0x8
#define D18F1x10C_NbPsSel_OFFSET                                4
#define D18F1x10C_NbPsSel_WIDTH                                 2
#define D18F1x10C_NbPsSel_MASK                                  0x30
#define D18F1x10C_Reserved_31_6_OFFSET                          6
#define D18F1x10C_Reserved_31_6_WIDTH                           26
#define D18F1x10C_Reserved_31_6_MASK                            0xFFFFFFC0

/// D18F1x10C
typedef union {
  struct {                                                            ///<
    UINT32                                               DctCfgSel:3; ///<
    UINT32                                                MemPsSel:1; ///<
    UINT32                                                 NbPsSel:2; ///<
    UINT32                                           Reserved_31_6:26; ///<

  } Field;

  UINT32 Value;
} D18F1x10C_STRUCT;

// **** D18F2x60_dct0 Register Definition ****
// Address
#define D18F2x60_dct0_ADDRESS                                   0x60
// Type
#define D18F2x60_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x64_dct0 Register Definition ****
// Address
#define D18F2x64_dct0_ADDRESS                                   0x64
// Type
#define D18F2x64_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x68_dct0 Register Definition ****
// Address
#define D18F2x68_dct0_ADDRESS                                   0x68
// Type
#define D18F2x68_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x6C_dct0 Register Definition ****
// Address
#define D18F2x6C_dct0_ADDRESS                                   0x6C
// Type
#define D18F2x6C_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x60_dct1 Register Definition ****
// Address
#define D18F2x60_dct1_ADDRESS                                   0x60
// Type
#define D18F2x60_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x64_dct1 Register Definition ****
// Address
#define D18F2x64_dct1_ADDRESS                                   0x64
// Type
#define D18F2x64_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x68_dct1 Register Definition ****
// Address
#define D18F2x68_dct1_ADDRESS                                   0x68
// Type
#define D18F2x68_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x6C_dct1 Register Definition ****
// Address
#define D18F2x6C_dct1_ADDRESS                                   0x6C
// Type
#define D18F2x6C_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x60_dct2 Register Definition ****
// Address
#define D18F2x60_dct2_ADDRESS                                   0x60
// Type
#define D18F2x60_dct2_TYPE                                      TYPE_D18F2_dct2

// **** D18F2x64_dct2 Register Definition ****
// Address
#define D18F2x64_dct2_ADDRESS                                   0x64
// Type
#define D18F2x64_dct2_TYPE                                      TYPE_D18F2_dct2

// **** D18F2x68_dct2 Register Definition ****
// Address
#define D18F2x68_dct2_ADDRESS                                   0x68
// Type
#define D18F2x68_dct2_TYPE                                      TYPE_D18F2_dct2

// **** D18F2x6C_dct2 Register Definition ****
// Address
#define D18F2x6C_dct2_ADDRESS                                   0x6C
// Type
#define D18F2x6C_dct2_TYPE                                      TYPE_D18F2_dct2

// **** D18F2x60_dct3 Register Definition ****
// Address
#define D18F2x60_dct3_ADDRESS                                   0x60
// Type
#define D18F2x60_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x64_dct3 Register Definition ****
// Address
#define D18F2x64_dct3_ADDRESS                                   0x64
// Type
#define D18F2x64_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x68_dct3 Register Definition ****
// Address
#define D18F2x68_dct3_ADDRESS                                   0x68
// Type
#define D18F2x68_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x6C_dct3 Register Definition ****
// Address
#define D18F2x6C_dct3_ADDRESS                                   0x6C
// Type
#define D18F2x6C_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x78_dct0 Register Definition ****
// Address
#define D18F2x78_dct0_ADDRESS                                   0x78
// Type
#define D18F2x78_dct0_TYPE                                      TYPE_D18F2_dct0

// Field Data
#define D18F2x78_dct0_ChanVal_OFFSET                            0
#define D18F2x78_dct0_ChanVal_WIDTH                             1
#define D18F2x78_dct0_ChanVal_MASK                              0x1
#define D18F2x78_dct0_ChannelEn_OFFSET                          1
#define D18F2x78_dct0_ChannelEn_WIDTH                           1
#define D18F2x78_dct0_ChannelEn_MASK                            0x2
#define D18F2x78_dct0_GsyncDis_OFFSET                           2
#define D18F2x78_dct0_GsyncDis_WIDTH                            1
#define D18F2x78_dct0_GsyncDis_MASK                             0x4
#define D18F2x78_dct0_Reserved_3_3_OFFSET                       3
#define D18F2x78_dct0_Reserved_3_3_WIDTH                        1
#define D18F2x78_dct0_Reserved_3_3_MASK                         0x8
#define D18F2x78_dct0_PtrInitReq_OFFSET                         4
#define D18F2x78_dct0_PtrInitReq_WIDTH                          1
#define D18F2x78_dct0_PtrInitReq_MASK                           0x10
#define D18F2x78_dct0_Reserved_5_5_OFFSET                       5
#define D18F2x78_dct0_Reserved_5_5_WIDTH                        1
#define D18F2x78_dct0_Reserved_5_5_MASK                         0x20
#define D18F2x78_dct0_RxPtrInitReq_OFFSET                       6
#define D18F2x78_dct0_RxPtrInitReq_WIDTH                        1
#define D18F2x78_dct0_RxPtrInitReq_MASK                         0x40
#define D18F2x78_dct0_Reserved_7_7_OFFSET                       7
#define D18F2x78_dct0_Reserved_7_7_WIDTH                        1
#define D18F2x78_dct0_Reserved_7_7_MASK                         0x80
#define D18F2x78_dct0_DramType_OFFSET                           8
#define D18F2x78_dct0_DramType_WIDTH                            3
#define D18F2x78_dct0_DramType_MASK                             0x700
#define D18F2x78_dct0_Reserved_16_11_OFFSET                     11
#define D18F2x78_dct0_Reserved_16_11_WIDTH                      6
#define D18F2x78_dct0_Reserved_16_11_MASK                       0x1F800
#define D18F2x78_dct0_AddrCmdTriEn_OFFSET                       17
#define D18F2x78_dct0_AddrCmdTriEn_WIDTH                        1
#define D18F2x78_dct0_AddrCmdTriEn_MASK                         0x20000
#define D18F2x78_dct0_Reserved_31_18_OFFSET                     18
#define D18F2x78_dct0_Reserved_31_18_WIDTH                      13
#define D18F2x78_dct0_Reserved_31_18_MASK                       0x7FFC0000
#define D18F2x78_dct0_Reserved_31_31_OFFSET                     31
#define D18F2x78_dct0_Reserved_31_31_WIDTH                      1
#define D18F2x78_dct0_Reserved_31_31_MASK                       0x80000000

/// D18F2x78_dct0
typedef union {
  struct {                                                            ///<
    UINT32                                                 ChanVal:1; ///<
    UINT32                                               ChannelEn:1; ///<
    UINT32                                                GsyncDis:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                              PtrInitReq:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            RxPtrInitReq:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                                DramType:3; ///<
    UINT32                                          Reserved_16_11:6; ///<
    UINT32                                            AddrCmdTriEn:1; ///<
    UINT32                                          Reserved_31_18:13; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x78_dct0_STRUCT;

// **** D18F2x78_dct1 Register Definition ****
// Address
#define D18F2x78_dct1_ADDRESS                                   0x78
// Type
#define D18F2x78_dct1_TYPE                                      TYPE_D18F2_dct1

// Field Data
#define D18F2x78_dct1_ChanVal_OFFSET                            0
#define D18F2x78_dct1_ChanVal_WIDTH                             1
#define D18F2x78_dct1_ChanVal_MASK                              0x1
#define D18F2x78_dct1_ChannelEn_OFFSET                          1
#define D18F2x78_dct1_ChannelEn_WIDTH                           1
#define D18F2x78_dct1_ChannelEn_MASK                            0x2
#define D18F2x78_dct1_GsyncDis_OFFSET                           2
#define D18F2x78_dct1_GsyncDis_WIDTH                            1
#define D18F2x78_dct1_GsyncDis_MASK                             0x4
#define D18F2x78_dct1_Reserved_3_3_OFFSET                       3
#define D18F2x78_dct1_Reserved_3_3_WIDTH                        1
#define D18F2x78_dct1_Reserved_3_3_MASK                         0x8
#define D18F2x78_dct1_PtrInitReq_OFFSET                         4
#define D18F2x78_dct1_PtrInitReq_WIDTH                          1
#define D18F2x78_dct1_PtrInitReq_MASK                           0x10
#define D18F2x78_dct1_Reserved_5_5_OFFSET                       5
#define D18F2x78_dct1_Reserved_5_5_WIDTH                        1
#define D18F2x78_dct1_Reserved_5_5_MASK                         0x20
#define D18F2x78_dct1_RxPtrInitReq_OFFSET                       6
#define D18F2x78_dct1_RxPtrInitReq_WIDTH                        1
#define D18F2x78_dct1_RxPtrInitReq_MASK                         0x40
#define D18F2x78_dct1_Reserved_7_7_OFFSET                       7
#define D18F2x78_dct1_Reserved_7_7_WIDTH                        1
#define D18F2x78_dct1_Reserved_7_7_MASK                         0x80
#define D18F2x78_dct1_DramType_OFFSET                           8
#define D18F2x78_dct1_DramType_WIDTH                            3
#define D18F2x78_dct1_DramType_MASK                             0x700
#define D18F2x78_dct1_Reserved_16_11_OFFSET                     11
#define D18F2x78_dct1_Reserved_16_11_WIDTH                      6
#define D18F2x78_dct1_Reserved_16_11_MASK                       0x1F800
#define D18F2x78_dct1_AddrCmdTriEn_OFFSET                       17
#define D18F2x78_dct1_AddrCmdTriEn_WIDTH                        1
#define D18F2x78_dct1_AddrCmdTriEn_MASK                         0x20000
#define D18F2x78_dct1_Reserved_31_18_OFFSET                     18
#define D18F2x78_dct1_Reserved_31_18_WIDTH                      13
#define D18F2x78_dct1_Reserved_31_18_MASK                       0x7FFC0000
#define D18F2x78_dct1_Reserved_31_31_OFFSET                     31
#define D18F2x78_dct1_Reserved_31_31_WIDTH                      1
#define D18F2x78_dct1_Reserved_31_31_MASK                       0x80000000

/// D18F2x78_dct1
typedef union {
  struct {                                                            ///<
    UINT32                                                 ChanVal:1; ///<
    UINT32                                               ChannelEn:1; ///<
    UINT32                                                GsyncDis:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                              PtrInitReq:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            RxPtrInitReq:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                                DramType:3; ///<
    UINT32                                          Reserved_16_11:6; ///<
    UINT32                                            AddrCmdTriEn:1; ///<
    UINT32                                          Reserved_31_18:13; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x78_dct1_STRUCT;

// **** D18F2x78_dct3 Register Definition ****
// Address
#define D18F2x78_dct3_ADDRESS                                   0x78
// Type
#define D18F2x78_dct3_TYPE                                      TYPE_D18F2_dct3

// Field Data
#define D18F2x78_dct3_ChanVal_OFFSET                            0
#define D18F2x78_dct3_ChanVal_WIDTH                             1
#define D18F2x78_dct3_ChanVal_MASK                              0x1
#define D18F2x78_dct3_ChannelEn_OFFSET                          1
#define D18F2x78_dct3_ChannelEn_WIDTH                           1
#define D18F2x78_dct3_ChannelEn_MASK                            0x2
#define D18F2x78_dct3_GsyncDis_OFFSET                           2
#define D18F2x78_dct3_GsyncDis_WIDTH                            1
#define D18F2x78_dct3_GsyncDis_MASK                             0x4
#define D18F2x78_dct3_Reserved_3_3_OFFSET                       3
#define D18F2x78_dct3_Reserved_3_3_WIDTH                        1
#define D18F2x78_dct3_Reserved_3_3_MASK                         0x8
#define D18F2x78_dct3_PtrInitReq_OFFSET                         4
#define D18F2x78_dct3_PtrInitReq_WIDTH                          1
#define D18F2x78_dct3_PtrInitReq_MASK                           0x10
#define D18F2x78_dct3_Reserved_5_5_OFFSET                       5
#define D18F2x78_dct3_Reserved_5_5_WIDTH                        1
#define D18F2x78_dct3_Reserved_5_5_MASK                         0x20
#define D18F2x78_dct3_RxPtrInitReq_OFFSET                       6
#define D18F2x78_dct3_RxPtrInitReq_WIDTH                        1
#define D18F2x78_dct3_RxPtrInitReq_MASK                         0x40
#define D18F2x78_dct3_Reserved_7_7_OFFSET                       7
#define D18F2x78_dct3_Reserved_7_7_WIDTH                        1
#define D18F2x78_dct3_Reserved_7_7_MASK                         0x80
#define D18F2x78_dct3_DramType_OFFSET                           8
#define D18F2x78_dct3_DramType_WIDTH                            3
#define D18F2x78_dct3_DramType_MASK                             0x700
#define D18F2x78_dct3_Reserved_16_11_OFFSET                     11
#define D18F2x78_dct3_Reserved_16_11_WIDTH                      6
#define D18F2x78_dct3_Reserved_16_11_MASK                       0x1F800
#define D18F2x78_dct3_AddrCmdTriEn_OFFSET                       17
#define D18F2x78_dct3_AddrCmdTriEn_WIDTH                        1
#define D18F2x78_dct3_AddrCmdTriEn_MASK                         0x20000
#define D18F2x78_dct3_Reserved_31_18_OFFSET                     18
#define D18F2x78_dct3_Reserved_31_18_WIDTH                      13
#define D18F2x78_dct3_Reserved_31_18_MASK                       0x7FFC0000
#define D18F2x78_dct3_Reserved_31_31_OFFSET                     31
#define D18F2x78_dct3_Reserved_31_31_WIDTH                      1
#define D18F2x78_dct3_Reserved_31_31_MASK                       0x80000000

/// D18F2x78_dct3
typedef union {
  struct {                                                            ///<
    UINT32                                                 ChanVal:1; ///<
    UINT32                                               ChannelEn:1; ///<
    UINT32                                                GsyncDis:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                              PtrInitReq:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            RxPtrInitReq:1; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                                DramType:3; ///<
    UINT32                                          Reserved_16_11:6; ///<
    UINT32                                            AddrCmdTriEn:1; ///<
    UINT32                                          Reserved_31_18:13; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x78_dct3_STRUCT;

// **** D18F2x90_dct0 Register Definition ****
// Address
#define D18F2x90_dct0_ADDRESS                                   0x90
// Type
#define D18F2x90_dct0_TYPE                                      TYPE_D18F2_dct0

// Field Data
#define D18F2x90_dct0_Reserved_0_0_OFFSET                       0
#define D18F2x90_dct0_Reserved_0_0_WIDTH                        1
#define D18F2x90_dct0_Reserved_0_0_MASK                         0x1
#define D18F2x90_dct0_ExitSelfRef_OFFSET                        1
#define D18F2x90_dct0_ExitSelfRef_WIDTH                         1
#define D18F2x90_dct0_ExitSelfRef_MASK                          0x2
#define D18F2x90_dct0_Reserved_7_2_OFFSET                       2
#define D18F2x90_dct0_Reserved_7_2_WIDTH                        6
#define D18F2x90_dct0_Reserved_7_2_MASK                         0xFC
#define D18F2x90_dct0_Reserved_8_8_OFFSET                       8
#define D18F2x90_dct0_Reserved_8_8_WIDTH                        1
#define D18F2x90_dct0_Reserved_8_8_MASK                         0x100
#define D18F2x90_dct0_Reserved_11_9_OFFSET                      9
#define D18F2x90_dct0_Reserved_11_9_WIDTH                       3
#define D18F2x90_dct0_Reserved_11_9_MASK                        0xE00
#define D18F2x90_dct0_Reserved_15_12_OFFSET                     12
#define D18F2x90_dct0_Reserved_15_12_WIDTH                      4
#define D18F2x90_dct0_Reserved_15_12_MASK                       0xF000
#define D18F2x90_dct0_UnbuffDimm_OFFSET                         16
#define D18F2x90_dct0_UnbuffDimm_WIDTH                          1
#define D18F2x90_dct0_UnbuffDimm_MASK                           0x10000
#define D18F2x90_dct0_EnterSelfRef_OFFSET                       17
#define D18F2x90_dct0_EnterSelfRef_WIDTH                        1
#define D18F2x90_dct0_EnterSelfRef_MASK                         0x20000
#define D18F2x90_dct0_PendRefPayback_OFFSET                     18
#define D18F2x90_dct0_PendRefPayback_WIDTH                      1
#define D18F2x90_dct0_PendRefPayback_MASK                       0x40000
#define D18F2x90_dct0_Reserved_19_19_OFFSET                     19
#define D18F2x90_dct0_Reserved_19_19_WIDTH                      1
#define D18F2x90_dct0_Reserved_19_19_MASK                       0x80000
#define D18F2x90_dct0_DynPageCloseEn_OFFSET                     20
#define D18F2x90_dct0_DynPageCloseEn_WIDTH                      1
#define D18F2x90_dct0_DynPageCloseEn_MASK                       0x100000
#define D18F2x90_dct0_IdleCycLowLimit_OFFSET                    21
#define D18F2x90_dct0_IdleCycLowLimit_WIDTH                     2
#define D18F2x90_dct0_IdleCycLowLimit_MASK                      0x600000
#define D18F2x90_dct0_ForceAutoPchg_OFFSET                      23
#define D18F2x90_dct0_ForceAutoPchg_WIDTH                       1
#define D18F2x90_dct0_ForceAutoPchg_MASK                        0x800000
#define D18F2x90_dct0_StagRefEn_OFFSET                          24
#define D18F2x90_dct0_StagRefEn_WIDTH                           1
#define D18F2x90_dct0_StagRefEn_MASK                            0x1000000
#define D18F2x90_dct0_PendRefPaybackS3En_OFFSET                 25
#define D18F2x90_dct0_PendRefPaybackS3En_WIDTH                  1
#define D18F2x90_dct0_PendRefPaybackS3En_MASK                   0x2000000
#define D18F2x90_dct0_PhaseAlignSRExitEn_OFFSET                 26
#define D18F2x90_dct0_PhaseAlignSRExitEn_WIDTH                  1
#define D18F2x90_dct0_PhaseAlignSRExitEn_MASK                   0x4000000
#define D18F2x90_dct0_DisDllShutdownSR_OFFSET                   27
#define D18F2x90_dct0_DisDllShutdownSR_WIDTH                    1
#define D18F2x90_dct0_DisDllShutdownSR_MASK                     0x8000000
#define D18F2x90_dct0_IdleCycLimit_OFFSET                       28
#define D18F2x90_dct0_IdleCycLimit_WIDTH                        4
#define D18F2x90_dct0_IdleCycLimit_MASK                         0xF0000000

/// D18F2x90_dct0
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                             ExitSelfRef:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                           Reserved_11_9:3; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                              UnbuffDimm:1; ///<
    UINT32                                            EnterSelfRef:1; ///<
    UINT32                                          PendRefPayback:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          DynPageCloseEn:1; ///<
    UINT32                                         IdleCycLowLimit:2; ///<
    UINT32                                           ForceAutoPchg:1; ///<
    UINT32                                               StagRefEn:1; ///<
    UINT32                                      PendRefPaybackS3En:1; ///<
    UINT32                                      PhaseAlignSRExitEn:1; ///<
    UINT32                                        DisDllShutdownSR:1; ///<
    UINT32                                            IdleCycLimit:4; ///<

  } Field;

  UINT32 Value;
} D18F2x90_dct0_STRUCT;

// **** D18F2x90_dct3 Register Definition ****
// Address
#define D18F2x90_dct3_ADDRESS                                   0x90
// Type
#define D18F2x90_dct3_TYPE                                      TYPE_D18F2_dct3

// Field Data
#define D18F2x90_dct3_Reserved_0_0_OFFSET                       0
#define D18F2x90_dct3_Reserved_0_0_WIDTH                        1
#define D18F2x90_dct3_Reserved_0_0_MASK                         0x1
#define D18F2x90_dct3_ExitSelfRef_OFFSET                        1
#define D18F2x90_dct3_ExitSelfRef_WIDTH                         1
#define D18F2x90_dct3_ExitSelfRef_MASK                          0x2
#define D18F2x90_dct3_Reserved_7_2_OFFSET                       2
#define D18F2x90_dct3_Reserved_7_2_WIDTH                        6
#define D18F2x90_dct3_Reserved_7_2_MASK                         0xFC
#define D18F2x90_dct3_Reserved_8_8_OFFSET                       8
#define D18F2x90_dct3_Reserved_8_8_WIDTH                        1
#define D18F2x90_dct3_Reserved_8_8_MASK                         0x100
#define D18F2x90_dct3_Reserved_11_9_OFFSET                      9
#define D18F2x90_dct3_Reserved_11_9_WIDTH                       3
#define D18F2x90_dct3_Reserved_11_9_MASK                        0xE00
#define D18F2x90_dct3_Reserved_15_12_OFFSET                     12
#define D18F2x90_dct3_Reserved_15_12_WIDTH                      4
#define D18F2x90_dct3_Reserved_15_12_MASK                       0xF000
#define D18F2x90_dct3_UnbuffDimm_OFFSET                         16
#define D18F2x90_dct3_UnbuffDimm_WIDTH                          1
#define D18F2x90_dct3_UnbuffDimm_MASK                           0x10000
#define D18F2x90_dct3_EnterSelfRef_OFFSET                       17
#define D18F2x90_dct3_EnterSelfRef_WIDTH                        1
#define D18F2x90_dct3_EnterSelfRef_MASK                         0x20000
#define D18F2x90_dct3_PendRefPayback_OFFSET                     18
#define D18F2x90_dct3_PendRefPayback_WIDTH                      1
#define D18F2x90_dct3_PendRefPayback_MASK                       0x40000
#define D18F2x90_dct3_Reserved_19_19_OFFSET                     19
#define D18F2x90_dct3_Reserved_19_19_WIDTH                      1
#define D18F2x90_dct3_Reserved_19_19_MASK                       0x80000
#define D18F2x90_dct3_DynPageCloseEn_OFFSET                     20
#define D18F2x90_dct3_DynPageCloseEn_WIDTH                      1
#define D18F2x90_dct3_DynPageCloseEn_MASK                       0x100000
#define D18F2x90_dct3_IdleCycLowLimit_OFFSET                    21
#define D18F2x90_dct3_IdleCycLowLimit_WIDTH                     2
#define D18F2x90_dct3_IdleCycLowLimit_MASK                      0x600000
#define D18F2x90_dct3_ForceAutoPchg_OFFSET                      23
#define D18F2x90_dct3_ForceAutoPchg_WIDTH                       1
#define D18F2x90_dct3_ForceAutoPchg_MASK                        0x800000
#define D18F2x90_dct3_StagRefEn_OFFSET                          24
#define D18F2x90_dct3_StagRefEn_WIDTH                           1
#define D18F2x90_dct3_StagRefEn_MASK                            0x1000000
#define D18F2x90_dct3_PendRefPaybackS3En_OFFSET                 25
#define D18F2x90_dct3_PendRefPaybackS3En_WIDTH                  1
#define D18F2x90_dct3_PendRefPaybackS3En_MASK                   0x2000000
#define D18F2x90_dct3_PhaseAlignSRExitEn_OFFSET                 26
#define D18F2x90_dct3_PhaseAlignSRExitEn_WIDTH                  1
#define D18F2x90_dct3_PhaseAlignSRExitEn_MASK                   0x4000000
#define D18F2x90_dct3_DisDllShutdownSR_OFFSET                   27
#define D18F2x90_dct3_DisDllShutdownSR_WIDTH                    1
#define D18F2x90_dct3_DisDllShutdownSR_MASK                     0x8000000
#define D18F2x90_dct3_IdleCycLimit_OFFSET                       28
#define D18F2x90_dct3_IdleCycLimit_WIDTH                        4
#define D18F2x90_dct3_IdleCycLimit_MASK                         0xF0000000

/// D18F2x90_dct3
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                             ExitSelfRef:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                           Reserved_11_9:3; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                              UnbuffDimm:1; ///<
    UINT32                                            EnterSelfRef:1; ///<
    UINT32                                          PendRefPayback:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          DynPageCloseEn:1; ///<
    UINT32                                         IdleCycLowLimit:2; ///<
    UINT32                                           ForceAutoPchg:1; ///<
    UINT32                                               StagRefEn:1; ///<
    UINT32                                      PendRefPaybackS3En:1; ///<
    UINT32                                      PhaseAlignSRExitEn:1; ///<
    UINT32                                        DisDllShutdownSR:1; ///<
    UINT32                                            IdleCycLimit:4; ///<

  } Field;

  UINT32 Value;
} D18F2x90_dct3_STRUCT;

// **** D18F2x90_dct1 Register Definition ****
// Address
#define D18F2x90_dct1_ADDRESS                                   0x90
// Type
#define D18F2x90_dct1_TYPE                                      TYPE_D18F2_dct1

// Field Data
#define D18F2x90_dct1_Reserved_0_0_OFFSET                       0
#define D18F2x90_dct1_Reserved_0_0_WIDTH                        1
#define D18F2x90_dct1_Reserved_0_0_MASK                         0x1
#define D18F2x90_dct1_ExitSelfRef_OFFSET                        1
#define D18F2x90_dct1_ExitSelfRef_WIDTH                         1
#define D18F2x90_dct1_ExitSelfRef_MASK                          0x2
#define D18F2x90_dct1_Reserved_7_2_OFFSET                       2
#define D18F2x90_dct1_Reserved_7_2_WIDTH                        6
#define D18F2x90_dct1_Reserved_7_2_MASK                         0xFC
#define D18F2x90_dct1_Reserved_8_8_OFFSET                       8
#define D18F2x90_dct1_Reserved_8_8_WIDTH                        1
#define D18F2x90_dct1_Reserved_8_8_MASK                         0x100
#define D18F2x90_dct1_Reserved_11_9_OFFSET                      9
#define D18F2x90_dct1_Reserved_11_9_WIDTH                       3
#define D18F2x90_dct1_Reserved_11_9_MASK                        0xE00
#define D18F2x90_dct1_Reserved_15_12_OFFSET                     12
#define D18F2x90_dct1_Reserved_15_12_WIDTH                      4
#define D18F2x90_dct1_Reserved_15_12_MASK                       0xF000
#define D18F2x90_dct1_UnbuffDimm_OFFSET                         16
#define D18F2x90_dct1_UnbuffDimm_WIDTH                          1
#define D18F2x90_dct1_UnbuffDimm_MASK                           0x10000
#define D18F2x90_dct1_EnterSelfRef_OFFSET                       17
#define D18F2x90_dct1_EnterSelfRef_WIDTH                        1
#define D18F2x90_dct1_EnterSelfRef_MASK                         0x20000
#define D18F2x90_dct1_PendRefPayback_OFFSET                     18
#define D18F2x90_dct1_PendRefPayback_WIDTH                      1
#define D18F2x90_dct1_PendRefPayback_MASK                       0x40000
#define D18F2x90_dct1_Reserved_19_19_OFFSET                     19
#define D18F2x90_dct1_Reserved_19_19_WIDTH                      1
#define D18F2x90_dct1_Reserved_19_19_MASK                       0x80000
#define D18F2x90_dct1_DynPageCloseEn_OFFSET                     20
#define D18F2x90_dct1_DynPageCloseEn_WIDTH                      1
#define D18F2x90_dct1_DynPageCloseEn_MASK                       0x100000
#define D18F2x90_dct1_IdleCycLowLimit_OFFSET                    21
#define D18F2x90_dct1_IdleCycLowLimit_WIDTH                     2
#define D18F2x90_dct1_IdleCycLowLimit_MASK                      0x600000
#define D18F2x90_dct1_ForceAutoPchg_OFFSET                      23
#define D18F2x90_dct1_ForceAutoPchg_WIDTH                       1
#define D18F2x90_dct1_ForceAutoPchg_MASK                        0x800000
#define D18F2x90_dct1_StagRefEn_OFFSET                          24
#define D18F2x90_dct1_StagRefEn_WIDTH                           1
#define D18F2x90_dct1_StagRefEn_MASK                            0x1000000
#define D18F2x90_dct1_PendRefPaybackS3En_OFFSET                 25
#define D18F2x90_dct1_PendRefPaybackS3En_WIDTH                  1
#define D18F2x90_dct1_PendRefPaybackS3En_MASK                   0x2000000
#define D18F2x90_dct1_PhaseAlignSRExitEn_OFFSET                 26
#define D18F2x90_dct1_PhaseAlignSRExitEn_WIDTH                  1
#define D18F2x90_dct1_PhaseAlignSRExitEn_MASK                   0x4000000
#define D18F2x90_dct1_DisDllShutdownSR_OFFSET                   27
#define D18F2x90_dct1_DisDllShutdownSR_WIDTH                    1
#define D18F2x90_dct1_DisDllShutdownSR_MASK                     0x8000000
#define D18F2x90_dct1_IdleCycLimit_OFFSET                       28
#define D18F2x90_dct1_IdleCycLimit_WIDTH                        4
#define D18F2x90_dct1_IdleCycLimit_MASK                         0xF0000000

/// D18F2x90_dct1
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                             ExitSelfRef:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                           Reserved_11_9:3; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                              UnbuffDimm:1; ///<
    UINT32                                            EnterSelfRef:1; ///<
    UINT32                                          PendRefPayback:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          DynPageCloseEn:1; ///<
    UINT32                                         IdleCycLowLimit:2; ///<
    UINT32                                           ForceAutoPchg:1; ///<
    UINT32                                               StagRefEn:1; ///<
    UINT32                                      PendRefPaybackS3En:1; ///<
    UINT32                                      PhaseAlignSRExitEn:1; ///<
    UINT32                                        DisDllShutdownSR:1; ///<
    UINT32                                            IdleCycLimit:4; ///<

  } Field;

  UINT32 Value;
} D18F2x90_dct1_STRUCT;

// **** D18F2x90_dct2 Register Definition ****
// Address
#define D18F2x90_dct2_ADDRESS                                   0x90
// Type
#define D18F2x90_dct2_TYPE                                      TYPE_D18F2_dct2

// Field Data
#define D18F2x90_dct2_Reserved_0_0_OFFSET                       0
#define D18F2x90_dct2_Reserved_0_0_WIDTH                        1
#define D18F2x90_dct2_Reserved_0_0_MASK                         0x1
#define D18F2x90_dct2_ExitSelfRef_OFFSET                        1
#define D18F2x90_dct2_ExitSelfRef_WIDTH                         1
#define D18F2x90_dct2_ExitSelfRef_MASK                          0x2
#define D18F2x90_dct2_Reserved_7_2_OFFSET                       2
#define D18F2x90_dct2_Reserved_7_2_WIDTH                        6
#define D18F2x90_dct2_Reserved_7_2_MASK                         0xFC
#define D18F2x90_dct2_Reserved_8_8_OFFSET                       8
#define D18F2x90_dct2_Reserved_8_8_WIDTH                        1
#define D18F2x90_dct2_Reserved_8_8_MASK                         0x100
#define D18F2x90_dct2_Reserved_11_9_OFFSET                      9
#define D18F2x90_dct2_Reserved_11_9_WIDTH                       3
#define D18F2x90_dct2_Reserved_11_9_MASK                        0xE00
#define D18F2x90_dct2_Reserved_15_12_OFFSET                     12
#define D18F2x90_dct2_Reserved_15_12_WIDTH                      4
#define D18F2x90_dct2_Reserved_15_12_MASK                       0xF000
#define D18F2x90_dct2_UnbuffDimm_OFFSET                         16
#define D18F2x90_dct2_UnbuffDimm_WIDTH                          1
#define D18F2x90_dct2_UnbuffDimm_MASK                           0x10000
#define D18F2x90_dct2_EnterSelfRef_OFFSET                       17
#define D18F2x90_dct2_EnterSelfRef_WIDTH                        1
#define D18F2x90_dct2_EnterSelfRef_MASK                         0x20000
#define D18F2x90_dct2_PendRefPayback_OFFSET                     18
#define D18F2x90_dct2_PendRefPayback_WIDTH                      1
#define D18F2x90_dct2_PendRefPayback_MASK                       0x40000
#define D18F2x90_dct2_Reserved_19_19_OFFSET                     19
#define D18F2x90_dct2_Reserved_19_19_WIDTH                      1
#define D18F2x90_dct2_Reserved_19_19_MASK                       0x80000
#define D18F2x90_dct2_DynPageCloseEn_OFFSET                     20
#define D18F2x90_dct2_DynPageCloseEn_WIDTH                      1
#define D18F2x90_dct2_DynPageCloseEn_MASK                       0x100000
#define D18F2x90_dct2_IdleCycLowLimit_OFFSET                    21
#define D18F2x90_dct2_IdleCycLowLimit_WIDTH                     2
#define D18F2x90_dct2_IdleCycLowLimit_MASK                      0x600000
#define D18F2x90_dct2_ForceAutoPchg_OFFSET                      23
#define D18F2x90_dct2_ForceAutoPchg_WIDTH                       1
#define D18F2x90_dct2_ForceAutoPchg_MASK                        0x800000
#define D18F2x90_dct2_StagRefEn_OFFSET                          24
#define D18F2x90_dct2_StagRefEn_WIDTH                           1
#define D18F2x90_dct2_StagRefEn_MASK                            0x1000000
#define D18F2x90_dct2_PendRefPaybackS3En_OFFSET                 25
#define D18F2x90_dct2_PendRefPaybackS3En_WIDTH                  1
#define D18F2x90_dct2_PendRefPaybackS3En_MASK                   0x2000000
#define D18F2x90_dct2_PhaseAlignSRExitEn_OFFSET                 26
#define D18F2x90_dct2_PhaseAlignSRExitEn_WIDTH                  1
#define D18F2x90_dct2_PhaseAlignSRExitEn_MASK                   0x4000000
#define D18F2x90_dct2_DisDllShutdownSR_OFFSET                   27
#define D18F2x90_dct2_DisDllShutdownSR_WIDTH                    1
#define D18F2x90_dct2_DisDllShutdownSR_MASK                     0x8000000
#define D18F2x90_dct2_IdleCycLimit_OFFSET                       28
#define D18F2x90_dct2_IdleCycLimit_WIDTH                        4
#define D18F2x90_dct2_IdleCycLimit_MASK                         0xF0000000

/// D18F2x90_dct2
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                             ExitSelfRef:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                           Reserved_11_9:3; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                              UnbuffDimm:1; ///<
    UINT32                                            EnterSelfRef:1; ///<
    UINT32                                          PendRefPayback:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          DynPageCloseEn:1; ///<
    UINT32                                         IdleCycLowLimit:2; ///<
    UINT32                                           ForceAutoPchg:1; ///<
    UINT32                                               StagRefEn:1; ///<
    UINT32                                      PendRefPaybackS3En:1; ///<
    UINT32                                      PhaseAlignSRExitEn:1; ///<
    UINT32                                        DisDllShutdownSR:1; ///<
    UINT32                                            IdleCycLimit:4; ///<

  } Field;

  UINT32 Value;
} D18F2x90_dct2_STRUCT;

// **** D18F2x94_dct3 Register Definition ****
// Address
#define D18F2x94_dct3_ADDRESS                                   0x94
// Type
#define D18F2x94_dct3_TYPE                                      TYPE_D18F2_dct3

// Field Data
#define D18F2x94_dct3_MemClkFreq_OFFSET                         0
#define D18F2x94_dct3_MemClkFreq_WIDTH                          5
#define D18F2x94_dct3_MemClkFreq_MASK                           0x1F
#define D18F2x94_dct3_Reserved_6_5_OFFSET                       5
#define D18F2x94_dct3_Reserved_6_5_WIDTH                        2
#define D18F2x94_dct3_Reserved_6_5_MASK                         0x60
#define D18F2x94_dct3_MemClkFreqVal_OFFSET                      7
#define D18F2x94_dct3_MemClkFreqVal_WIDTH                       1
#define D18F2x94_dct3_MemClkFreqVal_MASK                        0x80
#define D18F2x94_dct3_Reserved_9_8_OFFSET                       8
#define D18F2x94_dct3_Reserved_9_8_WIDTH                        2
#define D18F2x94_dct3_Reserved_9_8_MASK                         0x300
#define D18F2x94_dct3_ZqcsInterval_OFFSET                       10
#define D18F2x94_dct3_ZqcsInterval_WIDTH                        2
#define D18F2x94_dct3_ZqcsInterval_MASK                         0xC00
#define D18F2x94_dct3_Reserved_12_12_OFFSET                     12
#define D18F2x94_dct3_Reserved_12_12_WIDTH                      1
#define D18F2x94_dct3_Reserved_12_12_MASK                       0x1000
#define D18F2x94_dct3_Reserved_13_13_OFFSET                     13
#define D18F2x94_dct3_Reserved_13_13_WIDTH                      1
#define D18F2x94_dct3_Reserved_13_13_MASK                       0x2000
#define D18F2x94_dct3_DisDramInterface_OFFSET                   14
#define D18F2x94_dct3_DisDramInterface_WIDTH                    1
#define D18F2x94_dct3_DisDramInterface_MASK                     0x4000
#define D18F2x94_dct3_PowerDownEn_OFFSET                        15
#define D18F2x94_dct3_PowerDownEn_WIDTH                         1
#define D18F2x94_dct3_PowerDownEn_MASK                          0x8000
#define D18F2x94_dct3_PowerDownMode_OFFSET                      16
#define D18F2x94_dct3_PowerDownMode_WIDTH                       1
#define D18F2x94_dct3_PowerDownMode_MASK                        0x10000
#define D18F2x94_dct3_Reserved_18_17_OFFSET                     17
#define D18F2x94_dct3_Reserved_18_17_WIDTH                      2
#define D18F2x94_dct3_Reserved_18_17_MASK                       0x60000
#define D18F2x94_dct3_Reserved_19_19_OFFSET                     19
#define D18F2x94_dct3_Reserved_19_19_WIDTH                      1
#define D18F2x94_dct3_Reserved_19_19_MASK                       0x80000
#define D18F2x94_dct3_SlowAccessMode_OFFSET                     20
#define D18F2x94_dct3_SlowAccessMode_WIDTH                      1
#define D18F2x94_dct3_SlowAccessMode_MASK                       0x100000
#define D18F2x94_dct3_FreqChgInProg_OFFSET                      21
#define D18F2x94_dct3_FreqChgInProg_WIDTH                       1
#define D18F2x94_dct3_FreqChgInProg_MASK                        0x200000
#define D18F2x94_dct3_BankSwizzleMode_OFFSET                    22
#define D18F2x94_dct3_BankSwizzleMode_WIDTH                     1
#define D18F2x94_dct3_BankSwizzleMode_MASK                      0x400000
#define D18F2x94_dct3_ProcOdtDis_OFFSET                         23
#define D18F2x94_dct3_ProcOdtDis_WIDTH                          1
#define D18F2x94_dct3_ProcOdtDis_MASK                           0x800000
#define D18F2x94_dct3_DcqBypassMax_OFFSET                       24
#define D18F2x94_dct3_DcqBypassMax_WIDTH                        5
#define D18F2x94_dct3_DcqBypassMax_MASK                         0x1F000000
#define D18F2x94_dct3_Reserved_30_29_OFFSET                     29
#define D18F2x94_dct3_Reserved_30_29_WIDTH                      2
#define D18F2x94_dct3_Reserved_30_29_MASK                       0x60000000
#define D18F2x94_dct3_DphyMemPsSelEn_OFFSET                     31
#define D18F2x94_dct3_DphyMemPsSelEn_WIDTH                      1
#define D18F2x94_dct3_DphyMemPsSelEn_MASK                       0x80000000

/// D18F2x94_dct3
typedef union {
  struct {                                                            ///<
    UINT32                                              MemClkFreq:5; ///<
    UINT32                                            Reserved_6_5:2; ///<
    UINT32                                           MemClkFreqVal:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                            ZqcsInterval:2; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                        DisDramInterface:1; ///<
    UINT32                                             PowerDownEn:1; ///<
    UINT32                                           PowerDownMode:1; ///<
    UINT32                                          Reserved_18_17:2; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          SlowAccessMode:1; ///<
    UINT32                                           FreqChgInProg:1; ///<
    UINT32                                         BankSwizzleMode:1; ///<
    UINT32                                              ProcOdtDis:1; ///<
    UINT32                                            DcqBypassMax:5; ///<
    UINT32                                          Reserved_30_29:2; ///<
    UINT32                                          DphyMemPsSelEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2x94_dct3_STRUCT;

// **** D18F2x94_dct0 Register Definition ****
// Address
#define D18F2x94_dct0_ADDRESS                                   0x94
// Type
#define D18F2x94_dct0_TYPE                                      TYPE_D18F2_dct0

// Field Data
#define D18F2x94_dct0_MemClkFreq_OFFSET                         0
#define D18F2x94_dct0_MemClkFreq_WIDTH                          5
#define D18F2x94_dct0_MemClkFreq_MASK                           0x1F
#define D18F2x94_dct0_Reserved_6_5_OFFSET                       5
#define D18F2x94_dct0_Reserved_6_5_WIDTH                        2
#define D18F2x94_dct0_Reserved_6_5_MASK                         0x60
#define D18F2x94_dct0_MemClkFreqVal_OFFSET                      7
#define D18F2x94_dct0_MemClkFreqVal_WIDTH                       1
#define D18F2x94_dct0_MemClkFreqVal_MASK                        0x80
#define D18F2x94_dct0_Reserved_9_8_OFFSET                       8
#define D18F2x94_dct0_Reserved_9_8_WIDTH                        2
#define D18F2x94_dct0_Reserved_9_8_MASK                         0x300
#define D18F2x94_dct0_ZqcsInterval_OFFSET                       10
#define D18F2x94_dct0_ZqcsInterval_WIDTH                        2
#define D18F2x94_dct0_ZqcsInterval_MASK                         0xC00
#define D18F2x94_dct0_Reserved_12_12_OFFSET                     12
#define D18F2x94_dct0_Reserved_12_12_WIDTH                      1
#define D18F2x94_dct0_Reserved_12_12_MASK                       0x1000
#define D18F2x94_dct0_Reserved_13_13_OFFSET                     13
#define D18F2x94_dct0_Reserved_13_13_WIDTH                      1
#define D18F2x94_dct0_Reserved_13_13_MASK                       0x2000
#define D18F2x94_dct0_DisDramInterface_OFFSET                   14
#define D18F2x94_dct0_DisDramInterface_WIDTH                    1
#define D18F2x94_dct0_DisDramInterface_MASK                     0x4000
#define D18F2x94_dct0_PowerDownEn_OFFSET                        15
#define D18F2x94_dct0_PowerDownEn_WIDTH                         1
#define D18F2x94_dct0_PowerDownEn_MASK                          0x8000
#define D18F2x94_dct0_PowerDownMode_OFFSET                      16
#define D18F2x94_dct0_PowerDownMode_WIDTH                       1
#define D18F2x94_dct0_PowerDownMode_MASK                        0x10000
#define D18F2x94_dct0_Reserved_18_17_OFFSET                     17
#define D18F2x94_dct0_Reserved_18_17_WIDTH                      2
#define D18F2x94_dct0_Reserved_18_17_MASK                       0x60000
#define D18F2x94_dct0_Reserved_19_19_OFFSET                     19
#define D18F2x94_dct0_Reserved_19_19_WIDTH                      1
#define D18F2x94_dct0_Reserved_19_19_MASK                       0x80000
#define D18F2x94_dct0_SlowAccessMode_OFFSET                     20
#define D18F2x94_dct0_SlowAccessMode_WIDTH                      1
#define D18F2x94_dct0_SlowAccessMode_MASK                       0x100000
#define D18F2x94_dct0_FreqChgInProg_OFFSET                      21
#define D18F2x94_dct0_FreqChgInProg_WIDTH                       1
#define D18F2x94_dct0_FreqChgInProg_MASK                        0x200000
#define D18F2x94_dct0_BankSwizzleMode_OFFSET                    22
#define D18F2x94_dct0_BankSwizzleMode_WIDTH                     1
#define D18F2x94_dct0_BankSwizzleMode_MASK                      0x400000
#define D18F2x94_dct0_ProcOdtDis_OFFSET                         23
#define D18F2x94_dct0_ProcOdtDis_WIDTH                          1
#define D18F2x94_dct0_ProcOdtDis_MASK                           0x800000
#define D18F2x94_dct0_DcqBypassMax_OFFSET                       24
#define D18F2x94_dct0_DcqBypassMax_WIDTH                        5
#define D18F2x94_dct0_DcqBypassMax_MASK                         0x1F000000
#define D18F2x94_dct0_Reserved_30_29_OFFSET                     29
#define D18F2x94_dct0_Reserved_30_29_WIDTH                      2
#define D18F2x94_dct0_Reserved_30_29_MASK                       0x60000000
#define D18F2x94_dct0_DphyMemPsSelEn_OFFSET                     31
#define D18F2x94_dct0_DphyMemPsSelEn_WIDTH                      1
#define D18F2x94_dct0_DphyMemPsSelEn_MASK                       0x80000000

/// D18F2x94_dct0
typedef union {
  struct {                                                            ///<
    UINT32                                              MemClkFreq:5; ///<
    UINT32                                            Reserved_6_5:2; ///<
    UINT32                                           MemClkFreqVal:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                            ZqcsInterval:2; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                        DisDramInterface:1; ///<
    UINT32                                             PowerDownEn:1; ///<
    UINT32                                           PowerDownMode:1; ///<
    UINT32                                          Reserved_18_17:2; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          SlowAccessMode:1; ///<
    UINT32                                           FreqChgInProg:1; ///<
    UINT32                                         BankSwizzleMode:1; ///<
    UINT32                                              ProcOdtDis:1; ///<
    UINT32                                            DcqBypassMax:5; ///<
    UINT32                                          Reserved_30_29:2; ///<
    UINT32                                          DphyMemPsSelEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2x94_dct0_STRUCT;

// **** D18F2x94_dct1 Register Definition ****
// Address
#define D18F2x94_dct1_ADDRESS                                   0x94
// Type
#define D18F2x94_dct1_TYPE                                      TYPE_D18F2_dct1

// Field Data
#define D18F2x94_dct1_MemClkFreq_OFFSET                         0
#define D18F2x94_dct1_MemClkFreq_WIDTH                          5
#define D18F2x94_dct1_MemClkFreq_MASK                           0x1F
#define D18F2x94_dct1_Reserved_6_5_OFFSET                       5
#define D18F2x94_dct1_Reserved_6_5_WIDTH                        2
#define D18F2x94_dct1_Reserved_6_5_MASK                         0x60
#define D18F2x94_dct1_MemClkFreqVal_OFFSET                      7
#define D18F2x94_dct1_MemClkFreqVal_WIDTH                       1
#define D18F2x94_dct1_MemClkFreqVal_MASK                        0x80
#define D18F2x94_dct1_Reserved_9_8_OFFSET                       8
#define D18F2x94_dct1_Reserved_9_8_WIDTH                        2
#define D18F2x94_dct1_Reserved_9_8_MASK                         0x300
#define D18F2x94_dct1_ZqcsInterval_OFFSET                       10
#define D18F2x94_dct1_ZqcsInterval_WIDTH                        2
#define D18F2x94_dct1_ZqcsInterval_MASK                         0xC00
#define D18F2x94_dct1_Reserved_12_12_OFFSET                     12
#define D18F2x94_dct1_Reserved_12_12_WIDTH                      1
#define D18F2x94_dct1_Reserved_12_12_MASK                       0x1000
#define D18F2x94_dct1_Reserved_13_13_OFFSET                     13
#define D18F2x94_dct1_Reserved_13_13_WIDTH                      1
#define D18F2x94_dct1_Reserved_13_13_MASK                       0x2000
#define D18F2x94_dct1_DisDramInterface_OFFSET                   14
#define D18F2x94_dct1_DisDramInterface_WIDTH                    1
#define D18F2x94_dct1_DisDramInterface_MASK                     0x4000
#define D18F2x94_dct1_PowerDownEn_OFFSET                        15
#define D18F2x94_dct1_PowerDownEn_WIDTH                         1
#define D18F2x94_dct1_PowerDownEn_MASK                          0x8000
#define D18F2x94_dct1_PowerDownMode_OFFSET                      16
#define D18F2x94_dct1_PowerDownMode_WIDTH                       1
#define D18F2x94_dct1_PowerDownMode_MASK                        0x10000
#define D18F2x94_dct1_Reserved_18_17_OFFSET                     17
#define D18F2x94_dct1_Reserved_18_17_WIDTH                      2
#define D18F2x94_dct1_Reserved_18_17_MASK                       0x60000
#define D18F2x94_dct1_Reserved_19_19_OFFSET                     19
#define D18F2x94_dct1_Reserved_19_19_WIDTH                      1
#define D18F2x94_dct1_Reserved_19_19_MASK                       0x80000
#define D18F2x94_dct1_SlowAccessMode_OFFSET                     20
#define D18F2x94_dct1_SlowAccessMode_WIDTH                      1
#define D18F2x94_dct1_SlowAccessMode_MASK                       0x100000
#define D18F2x94_dct1_FreqChgInProg_OFFSET                      21
#define D18F2x94_dct1_FreqChgInProg_WIDTH                       1
#define D18F2x94_dct1_FreqChgInProg_MASK                        0x200000
#define D18F2x94_dct1_BankSwizzleMode_OFFSET                    22
#define D18F2x94_dct1_BankSwizzleMode_WIDTH                     1
#define D18F2x94_dct1_BankSwizzleMode_MASK                      0x400000
#define D18F2x94_dct1_ProcOdtDis_OFFSET                         23
#define D18F2x94_dct1_ProcOdtDis_WIDTH                          1
#define D18F2x94_dct1_ProcOdtDis_MASK                           0x800000
#define D18F2x94_dct1_DcqBypassMax_OFFSET                       24
#define D18F2x94_dct1_DcqBypassMax_WIDTH                        5
#define D18F2x94_dct1_DcqBypassMax_MASK                         0x1F000000
#define D18F2x94_dct1_Reserved_30_29_OFFSET                     29
#define D18F2x94_dct1_Reserved_30_29_WIDTH                      2
#define D18F2x94_dct1_Reserved_30_29_MASK                       0x60000000
#define D18F2x94_dct1_DphyMemPsSelEn_OFFSET                     31
#define D18F2x94_dct1_DphyMemPsSelEn_WIDTH                      1
#define D18F2x94_dct1_DphyMemPsSelEn_MASK                       0x80000000

/// D18F2x94_dct1
typedef union {
  struct {                                                            ///<
    UINT32                                              MemClkFreq:5; ///<
    UINT32                                            Reserved_6_5:2; ///<
    UINT32                                           MemClkFreqVal:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                            ZqcsInterval:2; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                        DisDramInterface:1; ///<
    UINT32                                             PowerDownEn:1; ///<
    UINT32                                           PowerDownMode:1; ///<
    UINT32                                          Reserved_18_17:2; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          SlowAccessMode:1; ///<
    UINT32                                           FreqChgInProg:1; ///<
    UINT32                                         BankSwizzleMode:1; ///<
    UINT32                                              ProcOdtDis:1; ///<
    UINT32                                            DcqBypassMax:5; ///<
    UINT32                                          Reserved_30_29:2; ///<
    UINT32                                          DphyMemPsSelEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2x94_dct1_STRUCT;

// **** D18F2x94_dct2 Register Definition ****
// Address
#define D18F2x94_dct2_ADDRESS                                   0x94
// Type
#define D18F2x94_dct2_TYPE                                      TYPE_D18F2_dct2

// Field Data
#define D18F2x94_dct2_MemClkFreq_OFFSET                         0
#define D18F2x94_dct2_MemClkFreq_WIDTH                          5
#define D18F2x94_dct2_MemClkFreq_MASK                           0x1F
#define D18F2x94_dct2_Reserved_6_5_OFFSET                       5
#define D18F2x94_dct2_Reserved_6_5_WIDTH                        2
#define D18F2x94_dct2_Reserved_6_5_MASK                         0x60
#define D18F2x94_dct2_MemClkFreqVal_OFFSET                      7
#define D18F2x94_dct2_MemClkFreqVal_WIDTH                       1
#define D18F2x94_dct2_MemClkFreqVal_MASK                        0x80
#define D18F2x94_dct2_Reserved_9_8_OFFSET                       8
#define D18F2x94_dct2_Reserved_9_8_WIDTH                        2
#define D18F2x94_dct2_Reserved_9_8_MASK                         0x300
#define D18F2x94_dct2_ZqcsInterval_OFFSET                       10
#define D18F2x94_dct2_ZqcsInterval_WIDTH                        2
#define D18F2x94_dct2_ZqcsInterval_MASK                         0xC00
#define D18F2x94_dct2_Reserved_12_12_OFFSET                     12
#define D18F2x94_dct2_Reserved_12_12_WIDTH                      1
#define D18F2x94_dct2_Reserved_12_12_MASK                       0x1000
#define D18F2x94_dct2_Reserved_13_13_OFFSET                     13
#define D18F2x94_dct2_Reserved_13_13_WIDTH                      1
#define D18F2x94_dct2_Reserved_13_13_MASK                       0x2000
#define D18F2x94_dct2_DisDramInterface_OFFSET                   14
#define D18F2x94_dct2_DisDramInterface_WIDTH                    1
#define D18F2x94_dct2_DisDramInterface_MASK                     0x4000
#define D18F2x94_dct2_PowerDownEn_OFFSET                        15
#define D18F2x94_dct2_PowerDownEn_WIDTH                         1
#define D18F2x94_dct2_PowerDownEn_MASK                          0x8000
#define D18F2x94_dct2_PowerDownMode_OFFSET                      16
#define D18F2x94_dct2_PowerDownMode_WIDTH                       1
#define D18F2x94_dct2_PowerDownMode_MASK                        0x10000
#define D18F2x94_dct2_Reserved_18_17_OFFSET                     17
#define D18F2x94_dct2_Reserved_18_17_WIDTH                      2
#define D18F2x94_dct2_Reserved_18_17_MASK                       0x60000
#define D18F2x94_dct2_Reserved_19_19_OFFSET                     19
#define D18F2x94_dct2_Reserved_19_19_WIDTH                      1
#define D18F2x94_dct2_Reserved_19_19_MASK                       0x80000
#define D18F2x94_dct2_SlowAccessMode_OFFSET                     20
#define D18F2x94_dct2_SlowAccessMode_WIDTH                      1
#define D18F2x94_dct2_SlowAccessMode_MASK                       0x100000
#define D18F2x94_dct2_FreqChgInProg_OFFSET                      21
#define D18F2x94_dct2_FreqChgInProg_WIDTH                       1
#define D18F2x94_dct2_FreqChgInProg_MASK                        0x200000
#define D18F2x94_dct2_BankSwizzleMode_OFFSET                    22
#define D18F2x94_dct2_BankSwizzleMode_WIDTH                     1
#define D18F2x94_dct2_BankSwizzleMode_MASK                      0x400000
#define D18F2x94_dct2_ProcOdtDis_OFFSET                         23
#define D18F2x94_dct2_ProcOdtDis_WIDTH                          1
#define D18F2x94_dct2_ProcOdtDis_MASK                           0x800000
#define D18F2x94_dct2_DcqBypassMax_OFFSET                       24
#define D18F2x94_dct2_DcqBypassMax_WIDTH                        5
#define D18F2x94_dct2_DcqBypassMax_MASK                         0x1F000000
#define D18F2x94_dct2_Reserved_30_29_OFFSET                     29
#define D18F2x94_dct2_Reserved_30_29_WIDTH                      2
#define D18F2x94_dct2_Reserved_30_29_MASK                       0x60000000
#define D18F2x94_dct2_DphyMemPsSelEn_OFFSET                     31
#define D18F2x94_dct2_DphyMemPsSelEn_WIDTH                      1
#define D18F2x94_dct2_DphyMemPsSelEn_MASK                       0x80000000

/// D18F2x94_dct2
typedef union {
  struct {                                                            ///<
    UINT32                                              MemClkFreq:5; ///<
    UINT32                                            Reserved_6_5:2; ///<
    UINT32                                           MemClkFreqVal:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                            ZqcsInterval:2; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                        DisDramInterface:1; ///<
    UINT32                                             PowerDownEn:1; ///<
    UINT32                                           PowerDownMode:1; ///<
    UINT32                                          Reserved_18_17:2; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                          SlowAccessMode:1; ///<
    UINT32                                           FreqChgInProg:1; ///<
    UINT32                                         BankSwizzleMode:1; ///<
    UINT32                                              ProcOdtDis:1; ///<
    UINT32                                            DcqBypassMax:5; ///<
    UINT32                                          Reserved_30_29:2; ///<
    UINT32                                          DphyMemPsSelEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2x94_dct2_STRUCT;

// **** D18F2xA8_dct2 Register Definition ****
// Address
#define D18F2xA8_dct2_ADDRESS                                   0xA8
// Type
#define D18F2xA8_dct2_TYPE                                      TYPE_D18F2_dct2

// Field Data
#define D18F2xA8_dct2_Reserved_1_0_OFFSET                       0
#define D18F2xA8_dct2_Reserved_1_0_WIDTH                        2
#define D18F2xA8_dct2_Reserved_1_0_MASK                         0x3
#define D18F2xA8_dct2_Reserved_3_2_OFFSET                       2
#define D18F2xA8_dct2_Reserved_3_2_WIDTH                        2
#define D18F2xA8_dct2_Reserved_3_2_MASK                         0xC
#define D18F2xA8_dct2_Reserved_4_4_OFFSET                       4
#define D18F2xA8_dct2_Reserved_4_4_WIDTH                        1
#define D18F2xA8_dct2_Reserved_4_4_MASK                         0x10
#define D18F2xA8_dct2_Reserved_5_5_OFFSET                       5
#define D18F2xA8_dct2_Reserved_5_5_WIDTH                        1
#define D18F2xA8_dct2_Reserved_5_5_MASK                         0x20
#define D18F2xA8_dct2_Reserved_7_6_OFFSET                       6
#define D18F2xA8_dct2_Reserved_7_6_WIDTH                        2
#define D18F2xA8_dct2_Reserved_7_6_MASK                         0xC0
#define D18F2xA8_dct2_MrsCtrlWordCS_7_0_OFFSET                  8
#define D18F2xA8_dct2_MrsCtrlWordCS_7_0_WIDTH                   8
#define D18F2xA8_dct2_MrsCtrlWordCS_7_0_MASK                    0xFF00
#define D18F2xA8_dct2_MemPhyPllPdMode_OFFSET                    16
#define D18F2xA8_dct2_MemPhyPllPdMode_WIDTH                     2
#define D18F2xA8_dct2_MemPhyPllPdMode_MASK                      0x30000
#define D18F2xA8_dct2_Reserved_18_18_OFFSET                     18
#define D18F2xA8_dct2_Reserved_18_18_WIDTH                      1
#define D18F2xA8_dct2_Reserved_18_18_MASK                       0x40000
#define D18F2xA8_dct2_Reserved_19_19_OFFSET                     19
#define D18F2xA8_dct2_Reserved_19_19_WIDTH                      1
#define D18F2xA8_dct2_Reserved_19_19_MASK                       0x80000
#define D18F2xA8_dct2_BankSwap_OFFSET                           20
#define D18F2xA8_dct2_BankSwap_WIDTH                            1
#define D18F2xA8_dct2_BankSwap_MASK                             0x100000
#define D18F2xA8_dct2_AggrPDEn_OFFSET                           21
#define D18F2xA8_dct2_AggrPDEn_WIDTH                            1
#define D18F2xA8_dct2_AggrPDEn_MASK                             0x200000
#define D18F2xA8_dct2_PrtlChPDEnhEn_OFFSET                      22
#define D18F2xA8_dct2_PrtlChPDEnhEn_WIDTH                       1
#define D18F2xA8_dct2_PrtlChPDEnhEn_MASK                        0x400000
#define D18F2xA8_dct2_Reserved_23_23_OFFSET                     23
#define D18F2xA8_dct2_Reserved_23_23_WIDTH                      1
#define D18F2xA8_dct2_Reserved_23_23_MASK                       0x800000
#define D18F2xA8_dct2_Reserved_25_24_OFFSET                     24
#define D18F2xA8_dct2_Reserved_25_24_WIDTH                      2
#define D18F2xA8_dct2_Reserved_25_24_MASK                       0x3000000
#define D18F2xA8_dct2_Reserved_27_26_OFFSET                     26
#define D18F2xA8_dct2_Reserved_27_26_WIDTH                      2
#define D18F2xA8_dct2_Reserved_27_26_MASK                       0xC000000
#define D18F2xA8_dct2_FastSelfRefEntryDis_OFFSET                28
#define D18F2xA8_dct2_FastSelfRefEntryDis_WIDTH                 1
#define D18F2xA8_dct2_FastSelfRefEntryDis_MASK                  0x10000000
#define D18F2xA8_dct2_RefChCmdMgtDis_OFFSET                     29
#define D18F2xA8_dct2_RefChCmdMgtDis_WIDTH                      1
#define D18F2xA8_dct2_RefChCmdMgtDis_MASK                       0x20000000
#define D18F2xA8_dct2_Reserved_30_30_OFFSET                     30
#define D18F2xA8_dct2_Reserved_30_30_WIDTH                      1
#define D18F2xA8_dct2_Reserved_30_30_MASK                       0x40000000
#define D18F2xA8_dct2_PerRankTimingEn_OFFSET                    31
#define D18F2xA8_dct2_PerRankTimingEn_WIDTH                     1
#define D18F2xA8_dct2_PerRankTimingEn_MASK                      0x80000000

/// D18F2xA8_dct2
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                       MrsCtrlWordCS_7_0:8; ///<
    UINT32                                         MemPhyPllPdMode:2; ///<
    UINT32                                          Reserved_18_18:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                                BankSwap:1; ///<
    UINT32                                                AggrPDEn:1; ///<
    UINT32                                           PrtlChPDEnhEn:1; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                          Reserved_25_24:2; ///<
    UINT32                                          Reserved_27_26:2; ///<
    UINT32                                     FastSelfRefEntryDis:1; ///<
    UINT32                                          RefChCmdMgtDis:1; ///<
    UINT32                                          Reserved_30_30:1; ///<
    UINT32                                         PerRankTimingEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2xA8_dct2_STRUCT;

// **** D18F2xA8_dct3 Register Definition ****
// Address
#define D18F2xA8_dct3_ADDRESS                                   0xA8
// Type
#define D18F2xA8_dct3_TYPE                                      TYPE_D18F2_dct3

// Field Data
#define D18F2xA8_dct3_Reserved_1_0_OFFSET                       0
#define D18F2xA8_dct3_Reserved_1_0_WIDTH                        2
#define D18F2xA8_dct3_Reserved_1_0_MASK                         0x3
#define D18F2xA8_dct3_Reserved_3_2_OFFSET                       2
#define D18F2xA8_dct3_Reserved_3_2_WIDTH                        2
#define D18F2xA8_dct3_Reserved_3_2_MASK                         0xC
#define D18F2xA8_dct3_Reserved_4_4_OFFSET                       4
#define D18F2xA8_dct3_Reserved_4_4_WIDTH                        1
#define D18F2xA8_dct3_Reserved_4_4_MASK                         0x10
#define D18F2xA8_dct3_Reserved_5_5_OFFSET                       5
#define D18F2xA8_dct3_Reserved_5_5_WIDTH                        1
#define D18F2xA8_dct3_Reserved_5_5_MASK                         0x20
#define D18F2xA8_dct3_Reserved_7_6_OFFSET                       6
#define D18F2xA8_dct3_Reserved_7_6_WIDTH                        2
#define D18F2xA8_dct3_Reserved_7_6_MASK                         0xC0
#define D18F2xA8_dct3_MrsCtrlWordCS_7_0_OFFSET                  8
#define D18F2xA8_dct3_MrsCtrlWordCS_7_0_WIDTH                   8
#define D18F2xA8_dct3_MrsCtrlWordCS_7_0_MASK                    0xFF00
#define D18F2xA8_dct3_MemPhyPllPdMode_OFFSET                    16
#define D18F2xA8_dct3_MemPhyPllPdMode_WIDTH                     2
#define D18F2xA8_dct3_MemPhyPllPdMode_MASK                      0x30000
#define D18F2xA8_dct3_Reserved_18_18_OFFSET                     18
#define D18F2xA8_dct3_Reserved_18_18_WIDTH                      1
#define D18F2xA8_dct3_Reserved_18_18_MASK                       0x40000
#define D18F2xA8_dct3_Reserved_19_19_OFFSET                     19
#define D18F2xA8_dct3_Reserved_19_19_WIDTH                      1
#define D18F2xA8_dct3_Reserved_19_19_MASK                       0x80000
#define D18F2xA8_dct3_BankSwap_OFFSET                           20
#define D18F2xA8_dct3_BankSwap_WIDTH                            1
#define D18F2xA8_dct3_BankSwap_MASK                             0x100000
#define D18F2xA8_dct3_AggrPDEn_OFFSET                           21
#define D18F2xA8_dct3_AggrPDEn_WIDTH                            1
#define D18F2xA8_dct3_AggrPDEn_MASK                             0x200000
#define D18F2xA8_dct3_PrtlChPDEnhEn_OFFSET                      22
#define D18F2xA8_dct3_PrtlChPDEnhEn_WIDTH                       1
#define D18F2xA8_dct3_PrtlChPDEnhEn_MASK                        0x400000
#define D18F2xA8_dct3_Reserved_23_23_OFFSET                     23
#define D18F2xA8_dct3_Reserved_23_23_WIDTH                      1
#define D18F2xA8_dct3_Reserved_23_23_MASK                       0x800000
#define D18F2xA8_dct3_Reserved_25_24_OFFSET                     24
#define D18F2xA8_dct3_Reserved_25_24_WIDTH                      2
#define D18F2xA8_dct3_Reserved_25_24_MASK                       0x3000000
#define D18F2xA8_dct3_Reserved_27_26_OFFSET                     26
#define D18F2xA8_dct3_Reserved_27_26_WIDTH                      2
#define D18F2xA8_dct3_Reserved_27_26_MASK                       0xC000000
#define D18F2xA8_dct3_FastSelfRefEntryDis_OFFSET                28
#define D18F2xA8_dct3_FastSelfRefEntryDis_WIDTH                 1
#define D18F2xA8_dct3_FastSelfRefEntryDis_MASK                  0x10000000
#define D18F2xA8_dct3_RefChCmdMgtDis_OFFSET                     29
#define D18F2xA8_dct3_RefChCmdMgtDis_WIDTH                      1
#define D18F2xA8_dct3_RefChCmdMgtDis_MASK                       0x20000000
#define D18F2xA8_dct3_Reserved_30_30_OFFSET                     30
#define D18F2xA8_dct3_Reserved_30_30_WIDTH                      1
#define D18F2xA8_dct3_Reserved_30_30_MASK                       0x40000000
#define D18F2xA8_dct3_PerRankTimingEn_OFFSET                    31
#define D18F2xA8_dct3_PerRankTimingEn_WIDTH                     1
#define D18F2xA8_dct3_PerRankTimingEn_MASK                      0x80000000

/// D18F2xA8_dct3
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                       MrsCtrlWordCS_7_0:8; ///<
    UINT32                                         MemPhyPllPdMode:2; ///<
    UINT32                                          Reserved_18_18:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                                BankSwap:1; ///<
    UINT32                                                AggrPDEn:1; ///<
    UINT32                                           PrtlChPDEnhEn:1; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                          Reserved_25_24:2; ///<
    UINT32                                          Reserved_27_26:2; ///<
    UINT32                                     FastSelfRefEntryDis:1; ///<
    UINT32                                          RefChCmdMgtDis:1; ///<
    UINT32                                          Reserved_30_30:1; ///<
    UINT32                                         PerRankTimingEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2xA8_dct3_STRUCT;

// **** D18F2xA8_dct0 Register Definition ****
// Address
#define D18F2xA8_dct0_ADDRESS                                   0xA8
// Type
#define D18F2xA8_dct0_TYPE                                      TYPE_D18F2_dct0

// Field Data
#define D18F2xA8_dct0_Reserved_1_0_OFFSET                       0
#define D18F2xA8_dct0_Reserved_1_0_WIDTH                        2
#define D18F2xA8_dct0_Reserved_1_0_MASK                         0x3
#define D18F2xA8_dct0_Reserved_3_2_OFFSET                       2
#define D18F2xA8_dct0_Reserved_3_2_WIDTH                        2
#define D18F2xA8_dct0_Reserved_3_2_MASK                         0xC
#define D18F2xA8_dct0_Reserved_4_4_OFFSET                       4
#define D18F2xA8_dct0_Reserved_4_4_WIDTH                        1
#define D18F2xA8_dct0_Reserved_4_4_MASK                         0x10
#define D18F2xA8_dct0_Reserved_5_5_OFFSET                       5
#define D18F2xA8_dct0_Reserved_5_5_WIDTH                        1
#define D18F2xA8_dct0_Reserved_5_5_MASK                         0x20
#define D18F2xA8_dct0_Reserved_7_6_OFFSET                       6
#define D18F2xA8_dct0_Reserved_7_6_WIDTH                        2
#define D18F2xA8_dct0_Reserved_7_6_MASK                         0xC0
#define D18F2xA8_dct0_MrsCtrlWordCS_7_0_OFFSET                  8
#define D18F2xA8_dct0_MrsCtrlWordCS_7_0_WIDTH                   8
#define D18F2xA8_dct0_MrsCtrlWordCS_7_0_MASK                    0xFF00
#define D18F2xA8_dct0_MemPhyPllPdMode_OFFSET                    16
#define D18F2xA8_dct0_MemPhyPllPdMode_WIDTH                     2
#define D18F2xA8_dct0_MemPhyPllPdMode_MASK                      0x30000
#define D18F2xA8_dct0_Reserved_18_18_OFFSET                     18
#define D18F2xA8_dct0_Reserved_18_18_WIDTH                      1
#define D18F2xA8_dct0_Reserved_18_18_MASK                       0x40000
#define D18F2xA8_dct0_Reserved_19_19_OFFSET                     19
#define D18F2xA8_dct0_Reserved_19_19_WIDTH                      1
#define D18F2xA8_dct0_Reserved_19_19_MASK                       0x80000
#define D18F2xA8_dct0_BankSwap_OFFSET                           20
#define D18F2xA8_dct0_BankSwap_WIDTH                            1
#define D18F2xA8_dct0_BankSwap_MASK                             0x100000
#define D18F2xA8_dct0_AggrPDEn_OFFSET                           21
#define D18F2xA8_dct0_AggrPDEn_WIDTH                            1
#define D18F2xA8_dct0_AggrPDEn_MASK                             0x200000
#define D18F2xA8_dct0_PrtlChPDEnhEn_OFFSET                      22
#define D18F2xA8_dct0_PrtlChPDEnhEn_WIDTH                       1
#define D18F2xA8_dct0_PrtlChPDEnhEn_MASK                        0x400000
#define D18F2xA8_dct0_Reserved_23_23_OFFSET                     23
#define D18F2xA8_dct0_Reserved_23_23_WIDTH                      1
#define D18F2xA8_dct0_Reserved_23_23_MASK                       0x800000
#define D18F2xA8_dct0_Reserved_25_24_OFFSET                     24
#define D18F2xA8_dct0_Reserved_25_24_WIDTH                      2
#define D18F2xA8_dct0_Reserved_25_24_MASK                       0x3000000
#define D18F2xA8_dct0_Reserved_27_26_OFFSET                     26
#define D18F2xA8_dct0_Reserved_27_26_WIDTH                      2
#define D18F2xA8_dct0_Reserved_27_26_MASK                       0xC000000
#define D18F2xA8_dct0_FastSelfRefEntryDis_OFFSET                28
#define D18F2xA8_dct0_FastSelfRefEntryDis_WIDTH                 1
#define D18F2xA8_dct0_FastSelfRefEntryDis_MASK                  0x10000000
#define D18F2xA8_dct0_RefChCmdMgtDis_OFFSET                     29
#define D18F2xA8_dct0_RefChCmdMgtDis_WIDTH                      1
#define D18F2xA8_dct0_RefChCmdMgtDis_MASK                       0x20000000
#define D18F2xA8_dct0_Reserved_30_30_OFFSET                     30
#define D18F2xA8_dct0_Reserved_30_30_WIDTH                      1
#define D18F2xA8_dct0_Reserved_30_30_MASK                       0x40000000
#define D18F2xA8_dct0_PerRankTimingEn_OFFSET                    31
#define D18F2xA8_dct0_PerRankTimingEn_WIDTH                     1
#define D18F2xA8_dct0_PerRankTimingEn_MASK                      0x80000000

/// D18F2xA8_dct0
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                       MrsCtrlWordCS_7_0:8; ///<
    UINT32                                         MemPhyPllPdMode:2; ///<
    UINT32                                          Reserved_18_18:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                                BankSwap:1; ///<
    UINT32                                                AggrPDEn:1; ///<
    UINT32                                           PrtlChPDEnhEn:1; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                          Reserved_25_24:2; ///<
    UINT32                                          Reserved_27_26:2; ///<
    UINT32                                     FastSelfRefEntryDis:1; ///<
    UINT32                                          RefChCmdMgtDis:1; ///<
    UINT32                                          Reserved_30_30:1; ///<
    UINT32                                         PerRankTimingEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2xA8_dct0_STRUCT;

// **** D18F2xA8_dct1 Register Definition ****
// Address
#define D18F2xA8_dct1_ADDRESS                                   0xA8
// Type
#define D18F2xA8_dct1_TYPE                                      TYPE_D18F2_dct1

// Field Data
#define D18F2xA8_dct1_Reserved_1_0_OFFSET                       0
#define D18F2xA8_dct1_Reserved_1_0_WIDTH                        2
#define D18F2xA8_dct1_Reserved_1_0_MASK                         0x3
#define D18F2xA8_dct1_Reserved_3_2_OFFSET                       2
#define D18F2xA8_dct1_Reserved_3_2_WIDTH                        2
#define D18F2xA8_dct1_Reserved_3_2_MASK                         0xC
#define D18F2xA8_dct1_Reserved_4_4_OFFSET                       4
#define D18F2xA8_dct1_Reserved_4_4_WIDTH                        1
#define D18F2xA8_dct1_Reserved_4_4_MASK                         0x10
#define D18F2xA8_dct1_Reserved_5_5_OFFSET                       5
#define D18F2xA8_dct1_Reserved_5_5_WIDTH                        1
#define D18F2xA8_dct1_Reserved_5_5_MASK                         0x20
#define D18F2xA8_dct1_Reserved_7_6_OFFSET                       6
#define D18F2xA8_dct1_Reserved_7_6_WIDTH                        2
#define D18F2xA8_dct1_Reserved_7_6_MASK                         0xC0
#define D18F2xA8_dct1_MrsCtrlWordCS_7_0_OFFSET                  8
#define D18F2xA8_dct1_MrsCtrlWordCS_7_0_WIDTH                   8
#define D18F2xA8_dct1_MrsCtrlWordCS_7_0_MASK                    0xFF00
#define D18F2xA8_dct1_MemPhyPllPdMode_OFFSET                    16
#define D18F2xA8_dct1_MemPhyPllPdMode_WIDTH                     2
#define D18F2xA8_dct1_MemPhyPllPdMode_MASK                      0x30000
#define D18F2xA8_dct1_Reserved_18_18_OFFSET                     18
#define D18F2xA8_dct1_Reserved_18_18_WIDTH                      1
#define D18F2xA8_dct1_Reserved_18_18_MASK                       0x40000
#define D18F2xA8_dct1_Reserved_19_19_OFFSET                     19
#define D18F2xA8_dct1_Reserved_19_19_WIDTH                      1
#define D18F2xA8_dct1_Reserved_19_19_MASK                       0x80000
#define D18F2xA8_dct1_BankSwap_OFFSET                           20
#define D18F2xA8_dct1_BankSwap_WIDTH                            1
#define D18F2xA8_dct1_BankSwap_MASK                             0x100000
#define D18F2xA8_dct1_AggrPDEn_OFFSET                           21
#define D18F2xA8_dct1_AggrPDEn_WIDTH                            1
#define D18F2xA8_dct1_AggrPDEn_MASK                             0x200000
#define D18F2xA8_dct1_PrtlChPDEnhEn_OFFSET                      22
#define D18F2xA8_dct1_PrtlChPDEnhEn_WIDTH                       1
#define D18F2xA8_dct1_PrtlChPDEnhEn_MASK                        0x400000
#define D18F2xA8_dct1_Reserved_23_23_OFFSET                     23
#define D18F2xA8_dct1_Reserved_23_23_WIDTH                      1
#define D18F2xA8_dct1_Reserved_23_23_MASK                       0x800000
#define D18F2xA8_dct1_Reserved_25_24_OFFSET                     24
#define D18F2xA8_dct1_Reserved_25_24_WIDTH                      2
#define D18F2xA8_dct1_Reserved_25_24_MASK                       0x3000000
#define D18F2xA8_dct1_Reserved_27_26_OFFSET                     26
#define D18F2xA8_dct1_Reserved_27_26_WIDTH                      2
#define D18F2xA8_dct1_Reserved_27_26_MASK                       0xC000000
#define D18F2xA8_dct1_FastSelfRefEntryDis_OFFSET                28
#define D18F2xA8_dct1_FastSelfRefEntryDis_WIDTH                 1
#define D18F2xA8_dct1_FastSelfRefEntryDis_MASK                  0x10000000
#define D18F2xA8_dct1_RefChCmdMgtDis_OFFSET                     29
#define D18F2xA8_dct1_RefChCmdMgtDis_WIDTH                      1
#define D18F2xA8_dct1_RefChCmdMgtDis_MASK                       0x20000000
#define D18F2xA8_dct1_Reserved_30_30_OFFSET                     30
#define D18F2xA8_dct1_Reserved_30_30_WIDTH                      1
#define D18F2xA8_dct1_Reserved_30_30_MASK                       0x40000000
#define D18F2xA8_dct1_PerRankTimingEn_OFFSET                    31
#define D18F2xA8_dct1_PerRankTimingEn_WIDTH                     1
#define D18F2xA8_dct1_PerRankTimingEn_MASK                      0x80000000

/// D18F2xA8_dct1
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                       MrsCtrlWordCS_7_0:8; ///<
    UINT32                                         MemPhyPllPdMode:2; ///<
    UINT32                                          Reserved_18_18:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                                BankSwap:1; ///<
    UINT32                                                AggrPDEn:1; ///<
    UINT32                                           PrtlChPDEnhEn:1; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                          Reserved_25_24:2; ///<
    UINT32                                          Reserved_27_26:2; ///<
    UINT32                                     FastSelfRefEntryDis:1; ///<
    UINT32                                          RefChCmdMgtDis:1; ///<
    UINT32                                          Reserved_30_30:1; ///<
    UINT32                                         PerRankTimingEn:1; ///<

  } Field;

  UINT32 Value;
} D18F2xA8_dct1_STRUCT;

// **** D18F2x110 Register Definition ****
// Address
#define D18F2x110_ADDRESS                                       0x110

// Type
#define D18F2x110_TYPE                                          TYPE_D18F2
// Field Data
#define D18F2x110_Reserved_1_0_OFFSET                           0
#define D18F2x110_Reserved_1_0_WIDTH                            2
#define D18F2x110_Reserved_1_0_MASK                             0x3
#define D18F2x110_BankSwapAddr8En_OFFSET                        2
#define D18F2x110_BankSwapAddr8En_WIDTH                         1
#define D18F2x110_BankSwapAddr8En_MASK                          0x4
#define D18F2x110_MemClrInit_OFFSET                             3
#define D18F2x110_MemClrInit_WIDTH                              1
#define D18F2x110_MemClrInit_MASK                               0x8
#define D18F2x110_Reserved_4_4_OFFSET                           4
#define D18F2x110_Reserved_4_4_WIDTH                            1
#define D18F2x110_Reserved_4_4_MASK                             0x10
#define D18F2x110_DctDatIntLv_OFFSET                            5
#define D18F2x110_DctDatIntLv_WIDTH                             1
#define D18F2x110_DctDatIntLv_MASK                              0x20
#define D18F2x110_DctSelIntLvAddr_1_0__OFFSET                   6
#define D18F2x110_DctSelIntLvAddr_1_0__WIDTH                    2
#define D18F2x110_DctSelIntLvAddr_1_0__MASK                     0xc0
#define D18F2x110_DctSelIntLvAddr_1_0__VALUE                    0x0
#define D18F2x110_DramEnable_OFFSET                             8
#define D18F2x110_DramEnable_WIDTH                              1
#define D18F2x110_DramEnable_MASK                               0x100
#define D18F2x110_MemClrBusy_OFFSET                             9
#define D18F2x110_MemClrBusy_WIDTH                              1
#define D18F2x110_MemClrBusy_MASK                               0x200
#define D18F2x110_MemCleared_OFFSET                             10
#define D18F2x110_MemCleared_WIDTH                              1
#define D18F2x110_MemCleared_MASK                               0x400
#define D18F2x110_Reserved_31_11_OFFSET                         11
#define D18F2x110_Reserved_31_11_WIDTH                          21
#define D18F2x110_Reserved_31_11_MASK                           0xfffff800

/// D18F2x110
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_1_0:2 ; ///<
    UINT32                                          BankSwapAddr8En:1 ; ///<
    UINT32                                               MemClrInit:1 ; ///<
    UINT32                                             Reserved_4_4:1 ; ///<
    UINT32                                              DctDatIntLv:1 ; ///<
    UINT32                                     DctSelIntLvAddr_1_0_:2 ; ///<
    UINT32                                               DramEnable:1 ; ///<
    UINT32                                               MemClrBusy:1 ; ///<
    UINT32                                               MemCleared:1 ; ///<
    UINT32                                           Reserved_31_11:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F2x110_STRUCT;

// **** D18F2x118 Register Definition ****
// Address
#define D18F2x118_ADDRESS                                       0x118
// Type
#define D18F2x118_TYPE                                          TYPE_D18F2

// Field Data
#define D18F2x118_MctPriCpuRd_OFFSET                            0
#define D18F2x118_MctPriCpuRd_WIDTH                             2
#define D18F2x118_MctPriCpuRd_MASK                              0x3
#define D18F2x118_MctPriCpuWr_OFFSET                            2
#define D18F2x118_MctPriCpuWr_WIDTH                             2
#define D18F2x118_MctPriCpuWr_MASK                              0xC
#define D18F2x118_MctPriIsocRd_OFFSET                           4
#define D18F2x118_MctPriIsocRd_WIDTH                            2
#define D18F2x118_MctPriIsocRd_MASK                             0x30
#define D18F2x118_MctPriIsocWr_OFFSET                           6
#define D18F2x118_MctPriIsocWr_WIDTH                            2
#define D18F2x118_MctPriIsocWr_MASK                             0xC0
#define D18F2x118_MctPriDefault_OFFSET                          8
#define D18F2x118_MctPriDefault_WIDTH                           2
#define D18F2x118_MctPriDefault_MASK                            0x300
#define D18F2x118_MctPriWr_OFFSET                               10
#define D18F2x118_MctPriWr_WIDTH                                2
#define D18F2x118_MctPriWr_MASK                                 0xC00
#define D18F2x118_MctPriIsoc_OFFSET                             12
#define D18F2x118_MctPriIsoc_WIDTH                              2
#define D18F2x118_MctPriIsoc_MASK                               0x3000
#define D18F2x118_MctPriTrace_OFFSET                            14
#define D18F2x118_MctPriTrace_WIDTH                             2
#define D18F2x118_MctPriTrace_MASK                              0xC000
#define D18F2x118_Reserved_17_16_OFFSET                         16
#define D18F2x118_Reserved_17_16_WIDTH                          2
#define D18F2x118_Reserved_17_16_MASK                           0x30000
#define D18F2x118_CC6SaveEn_OFFSET                              18
#define D18F2x118_CC6SaveEn_WIDTH                               1
#define D18F2x118_CC6SaveEn_MASK                                0x40000
#define D18F2x118_LockDramCfg_OFFSET                            19
#define D18F2x118_LockDramCfg_WIDTH                             1
#define D18F2x118_LockDramCfg_MASK                              0x80000
#define D18F2x118_McqMedPriByPassMax_OFFSET                     20
#define D18F2x118_McqMedPriByPassMax_WIDTH                      3
#define D18F2x118_McqMedPriByPassMax_MASK                       0x700000
#define D18F2x118_Reserved_23_23_OFFSET                         23
#define D18F2x118_Reserved_23_23_WIDTH                          1
#define D18F2x118_Reserved_23_23_MASK                           0x800000
#define D18F2x118_McqHiPriByPassMax_OFFSET                      24
#define D18F2x118_McqHiPriByPassMax_WIDTH                       3
#define D18F2x118_McqHiPriByPassMax_MASK                        0x7000000
#define D18F2x118_MctEccDisLatOptEn_OFFSET                      27
#define D18F2x118_MctEccDisLatOptEn_WIDTH                       1
#define D18F2x118_MctEccDisLatOptEn_MASK                        0x8000000
#define D18F2x118_MctVarPriCntLmt_OFFSET                        28
#define D18F2x118_MctVarPriCntLmt_WIDTH                         4
#define D18F2x118_MctVarPriCntLmt_MASK                          0xF0000000

/// D18F2x118
typedef union {
  struct {                                                            ///<
    UINT32                                             MctPriCpuRd:2; ///<
    UINT32                                             MctPriCpuWr:2; ///<
    UINT32                                            MctPriIsocRd:2; ///<
    UINT32                                            MctPriIsocWr:2; ///<
    UINT32                                           MctPriDefault:2; ///<
    UINT32                                                MctPriWr:2; ///<
    UINT32                                              MctPriIsoc:2; ///<
    UINT32                                             MctPriTrace:2; ///<
    UINT32                                          Reserved_17_16:2; ///<
    UINT32                                               CC6SaveEn:1; ///<
    UINT32                                             LockDramCfg:1; ///<
    UINT32                                      McqMedPriByPassMax:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                       McqHiPriByPassMax:3; ///<
    UINT32                                       MctEccDisLatOptEn:1; ///<
    UINT32                                         MctVarPriCntLmt:4; ///<

  } Field;

  UINT32 Value;
} D18F2x118_STRUCT;

// **** D18F2x2E0_dct2 Register Definition ****
// Address
#define D18F2x2E0_dct2_ADDRESS                                  0x2E0
// Type
#define D18F2x2E0_dct2_TYPE                                     TYPE_D18F2_dct2

// Field Data
#define D18F2x2E0_dct2_CurMemPstate_OFFSET                      0
#define D18F2x2E0_dct2_CurMemPstate_WIDTH                       1
#define D18F2x2E0_dct2_CurMemPstate_MASK                        0x1
#define D18F2x2E0_dct2_Reserved_19_1_OFFSET                     1
#define D18F2x2E0_dct2_Reserved_19_1_WIDTH                      19
#define D18F2x2E0_dct2_Reserved_19_1_MASK                       0xFFFFE
#define D18F2x2E0_dct2_MxMrsEn_OFFSET                           20
#define D18F2x2E0_dct2_MxMrsEn_WIDTH                            3
#define D18F2x2E0_dct2_MxMrsEn_MASK                             0x700000
#define D18F2x2E0_dct2_Reserved_23_23_OFFSET                    23
#define D18F2x2E0_dct2_Reserved_23_23_WIDTH                     1
#define D18F2x2E0_dct2_Reserved_23_23_MASK                      0x800000
#define D18F2x2E0_dct2_M1MemClkFreq_OFFSET                      24
#define D18F2x2E0_dct2_M1MemClkFreq_WIDTH                       5
#define D18F2x2E0_dct2_M1MemClkFreq_MASK                        0x1F000000
#define D18F2x2E0_dct2_Reserved_29_29_OFFSET                    29
#define D18F2x2E0_dct2_Reserved_29_29_WIDTH                     1
#define D18F2x2E0_dct2_Reserved_29_29_MASK                      0x20000000
#define D18F2x2E0_dct2_FastMstateDis_OFFSET                     30
#define D18F2x2E0_dct2_FastMstateDis_WIDTH                      1
#define D18F2x2E0_dct2_FastMstateDis_MASK                       0x40000000
#define D18F2x2E0_dct2_Reserved_31_31_OFFSET                    31
#define D18F2x2E0_dct2_Reserved_31_31_WIDTH                     1
#define D18F2x2E0_dct2_Reserved_31_31_MASK                      0x80000000

/// D18F2x2E0_dct2
typedef union {
  struct {                                                            ///<
    UINT32                                            CurMemPstate:1; ///<
    UINT32                                           Reserved_19_1:19; ///<
    UINT32                                                 MxMrsEn:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                            M1MemClkFreq:5; ///<
    UINT32                                          Reserved_29_29:1; ///<
    UINT32                                           FastMstateDis:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x2E0_dct2_STRUCT;

// **** D18F2x2E0_dct1 Register Definition ****
// Address
#define D18F2x2E0_dct1_ADDRESS                                  0x2E0
// Type
#define D18F2x2E0_dct1_TYPE                                     TYPE_D18F2_dct1

// Field Data
#define D18F2x2E0_dct1_CurMemPstate_OFFSET                      0
#define D18F2x2E0_dct1_CurMemPstate_WIDTH                       1
#define D18F2x2E0_dct1_CurMemPstate_MASK                        0x1
#define D18F2x2E0_dct1_Reserved_19_1_OFFSET                     1
#define D18F2x2E0_dct1_Reserved_19_1_WIDTH                      19
#define D18F2x2E0_dct1_Reserved_19_1_MASK                       0xFFFFE
#define D18F2x2E0_dct1_MxMrsEn_OFFSET                           20
#define D18F2x2E0_dct1_MxMrsEn_WIDTH                            3
#define D18F2x2E0_dct1_MxMrsEn_MASK                             0x700000
#define D18F2x2E0_dct1_Reserved_23_23_OFFSET                    23
#define D18F2x2E0_dct1_Reserved_23_23_WIDTH                     1
#define D18F2x2E0_dct1_Reserved_23_23_MASK                      0x800000
#define D18F2x2E0_dct1_M1MemClkFreq_OFFSET                      24
#define D18F2x2E0_dct1_M1MemClkFreq_WIDTH                       5
#define D18F2x2E0_dct1_M1MemClkFreq_MASK                        0x1F000000
#define D18F2x2E0_dct1_Reserved_29_29_OFFSET                    29
#define D18F2x2E0_dct1_Reserved_29_29_WIDTH                     1
#define D18F2x2E0_dct1_Reserved_29_29_MASK                      0x20000000
#define D18F2x2E0_dct1_FastMstateDis_OFFSET                     30
#define D18F2x2E0_dct1_FastMstateDis_WIDTH                      1
#define D18F2x2E0_dct1_FastMstateDis_MASK                       0x40000000
#define D18F2x2E0_dct1_Reserved_31_31_OFFSET                    31
#define D18F2x2E0_dct1_Reserved_31_31_WIDTH                     1
#define D18F2x2E0_dct1_Reserved_31_31_MASK                      0x80000000

/// D18F2x2E0_dct1
typedef union {
  struct {                                                            ///<
    UINT32                                            CurMemPstate:1; ///<
    UINT32                                           Reserved_19_1:19; ///<
    UINT32                                                 MxMrsEn:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                            M1MemClkFreq:5; ///<
    UINT32                                          Reserved_29_29:1; ///<
    UINT32                                           FastMstateDis:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x2E0_dct1_STRUCT;

// **** D18F2x2E0_dct3 Register Definition ****
// Address
#define D18F2x2E0_dct3_ADDRESS                                  0x2E0
// Type
#define D18F2x2E0_dct3_TYPE                                     TYPE_D18F2_dct3

// Field Data
#define D18F2x2E0_dct3_CurMemPstate_OFFSET                      0
#define D18F2x2E0_dct3_CurMemPstate_WIDTH                       1
#define D18F2x2E0_dct3_CurMemPstate_MASK                        0x1
#define D18F2x2E0_dct3_Reserved_19_1_OFFSET                     1
#define D18F2x2E0_dct3_Reserved_19_1_WIDTH                      19
#define D18F2x2E0_dct3_Reserved_19_1_MASK                       0xFFFFE
#define D18F2x2E0_dct3_MxMrsEn_OFFSET                           20
#define D18F2x2E0_dct3_MxMrsEn_WIDTH                            3
#define D18F2x2E0_dct3_MxMrsEn_MASK                             0x700000
#define D18F2x2E0_dct3_Reserved_23_23_OFFSET                    23
#define D18F2x2E0_dct3_Reserved_23_23_WIDTH                     1
#define D18F2x2E0_dct3_Reserved_23_23_MASK                      0x800000
#define D18F2x2E0_dct3_M1MemClkFreq_OFFSET                      24
#define D18F2x2E0_dct3_M1MemClkFreq_WIDTH                       5
#define D18F2x2E0_dct3_M1MemClkFreq_MASK                        0x1F000000
#define D18F2x2E0_dct3_Reserved_29_29_OFFSET                    29
#define D18F2x2E0_dct3_Reserved_29_29_WIDTH                     1
#define D18F2x2E0_dct3_Reserved_29_29_MASK                      0x20000000
#define D18F2x2E0_dct3_FastMstateDis_OFFSET                     30
#define D18F2x2E0_dct3_FastMstateDis_WIDTH                      1
#define D18F2x2E0_dct3_FastMstateDis_MASK                       0x40000000
#define D18F2x2E0_dct3_Reserved_31_31_OFFSET                    31
#define D18F2x2E0_dct3_Reserved_31_31_WIDTH                     1
#define D18F2x2E0_dct3_Reserved_31_31_MASK                      0x80000000

/// D18F2x2E0_dct3
typedef union {
  struct {                                                            ///<
    UINT32                                            CurMemPstate:1; ///<
    UINT32                                           Reserved_19_1:19; ///<
    UINT32                                                 MxMrsEn:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                            M1MemClkFreq:5; ///<
    UINT32                                          Reserved_29_29:1; ///<
    UINT32                                           FastMstateDis:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x2E0_dct3_STRUCT;

// **** D18F2x2E0_dct0 Register Definition ****
// Address
#define D18F2x2E0_dct0_ADDRESS                                  0x2E0
// Type
#define D18F2x2E0_dct0_TYPE                                     TYPE_D18F2_dct0

// Field Data
#define D18F2x2E0_dct0_CurMemPstate_OFFSET                      0
#define D18F2x2E0_dct0_CurMemPstate_WIDTH                       1
#define D18F2x2E0_dct0_CurMemPstate_MASK                        0x1
#define D18F2x2E0_dct0_Reserved_19_1_OFFSET                     1
#define D18F2x2E0_dct0_Reserved_19_1_WIDTH                      19
#define D18F2x2E0_dct0_Reserved_19_1_MASK                       0xFFFFE
#define D18F2x2E0_dct0_MxMrsEn_OFFSET                           20
#define D18F2x2E0_dct0_MxMrsEn_WIDTH                            3
#define D18F2x2E0_dct0_MxMrsEn_MASK                             0x700000
#define D18F2x2E0_dct0_Reserved_23_23_OFFSET                    23
#define D18F2x2E0_dct0_Reserved_23_23_WIDTH                     1
#define D18F2x2E0_dct0_Reserved_23_23_MASK                      0x800000
#define D18F2x2E0_dct0_M1MemClkFreq_OFFSET                      24
#define D18F2x2E0_dct0_M1MemClkFreq_WIDTH                       5
#define D18F2x2E0_dct0_M1MemClkFreq_MASK                        0x1F000000
#define D18F2x2E0_dct0_Reserved_29_29_OFFSET                    29
#define D18F2x2E0_dct0_Reserved_29_29_WIDTH                     1
#define D18F2x2E0_dct0_Reserved_29_29_MASK                      0x20000000
#define D18F2x2E0_dct0_FastMstateDis_OFFSET                     30
#define D18F2x2E0_dct0_FastMstateDis_WIDTH                      1
#define D18F2x2E0_dct0_FastMstateDis_MASK                       0x40000000
#define D18F2x2E0_dct0_Reserved_31_31_OFFSET                    31
#define D18F2x2E0_dct0_Reserved_31_31_WIDTH                     1
#define D18F2x2E0_dct0_Reserved_31_31_MASK                      0x80000000

/// D18F2x2E0_dct0
typedef union {
  struct {                                                            ///<
    UINT32                                            CurMemPstate:1; ///<
    UINT32                                           Reserved_19_1:19; ///<
    UINT32                                                 MxMrsEn:3; ///<
    UINT32                                          Reserved_23_23:1; ///<
    UINT32                                            M1MemClkFreq:5; ///<
    UINT32                                          Reserved_29_29:1; ///<
    UINT32                                           FastMstateDis:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F2x2E0_dct0_STRUCT;

// **** D18F3x44 Register Definition ****
// Address
#define D18F3x44_ADDRESS                                        0x44

// Type
#define D18F3x44_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3x44_Reserved_0_0_OFFSET                            0
#define D18F3x44_Reserved_0_0_WIDTH                             1
#define D18F3x44_Reserved_0_0_MASK                              0x1
#define D18F3x44_CpuRdDatErrEn_OFFSET                           1
#define D18F3x44_CpuRdDatErrEn_WIDTH                            1
#define D18F3x44_CpuRdDatErrEn_MASK                             0x2
#define D18F3x44_SyncFloodOnDramUcEcc_OFFSET                    2
#define D18F3x44_SyncFloodOnDramUcEcc_WIDTH                     1
#define D18F3x44_SyncFloodOnDramUcEcc_MASK                      0x4
#define D18F3x44_SyncFloodOnDramUcEcc_VALUE                     0x1
#define D18F3x44_SyncPktGenDis_OFFSET                           3
#define D18F3x44_SyncPktGenDis_WIDTH                            1
#define D18F3x44_SyncPktGenDis_MASK                             0x8
#define D18F3x44_SyncPktGenDis_VALUE                            0x0
#define D18F3x44_SyncPktPropDis_OFFSET                          4
#define D18F3x44_SyncPktPropDis_WIDTH                           1
#define D18F3x44_SyncPktPropDis_MASK                            0x10
#define D18F3x44_SyncPktPropDis_VALUE                           0x0
#define D18F3x44_IoMstAbortDis_OFFSET                           5
#define D18F3x44_IoMstAbortDis_WIDTH                            1
#define D18F3x44_IoMstAbortDis_MASK                             0x20
#define D18F3x44_CpuErrDis_OFFSET                               6
#define D18F3x44_CpuErrDis_WIDTH                                1
#define D18F3x44_CpuErrDis_MASK                                 0x40
#define D18F3x44_CpuErrDis_VALUE                                0x1
#define D18F3x44_IoErrDis_OFFSET                                7
#define D18F3x44_IoErrDis_WIDTH                                 1
#define D18F3x44_IoErrDis_MASK                                  0x80
#define D18F3x44_WDTDis_OFFSET                                  8
#define D18F3x44_WDTDis_WIDTH                                   1
#define D18F3x44_WDTDis_MASK                                    0x100
#define D18F3x44_WDTCntSel_2_0__OFFSET                          9
#define D18F3x44_WDTCntSel_2_0__WIDTH                           3
#define D18F3x44_WDTCntSel_2_0__MASK                            0xe00
#define D18F3x44_WDTBaseSel_OFFSET                              12
#define D18F3x44_WDTBaseSel_WIDTH                               2
#define D18F3x44_WDTBaseSel_MASK                                0x3000
#define D18F3x44_GenLinkSel_OFFSET                              14
#define D18F3x44_GenLinkSel_WIDTH                               2
#define D18F3x44_GenLinkSel_MASK                                0xc000
#define D18F3x44_GenCrcErrByte0_OFFSET                          16
#define D18F3x44_GenCrcErrByte0_WIDTH                           1
#define D18F3x44_GenCrcErrByte0_MASK                            0x10000
#define D18F3x44_GenCrcErrByte1_OFFSET                          17
#define D18F3x44_GenCrcErrByte1_WIDTH                           1
#define D18F3x44_GenCrcErrByte1_MASK                            0x20000
#define D18F3x44_GenSubLinkSel_OFFSET                           18
#define D18F3x44_GenSubLinkSel_WIDTH                            2
#define D18F3x44_GenSubLinkSel_MASK                             0xc0000
#define D18F3x44_SyncFloodOnWDT_OFFSET                          20
#define D18F3x44_SyncFloodOnWDT_WIDTH                           1
#define D18F3x44_SyncFloodOnWDT_MASK                            0x100000
#define D18F3x44_SyncFloodOnWDT_VALUE                           0x1
#define D18F3x44_SyncFloodOnAnyUcErr_OFFSET                     21
#define D18F3x44_SyncFloodOnAnyUcErr_WIDTH                      1
#define D18F3x44_SyncFloodOnAnyUcErr_MASK                       0x200000
#define D18F3x44_SyncFloodOnAnyUcErr_VALUE                      0x1
#define D18F3x44_DramEccEn_OFFSET                               22
#define D18F3x44_DramEccEn_WIDTH                                1
#define D18F3x44_DramEccEn_MASK                                 0x400000
#define D18F3x44_ChipKillEccCap_OFFSET                          23
#define D18F3x44_ChipKillEccCap_WIDTH                           1
#define D18F3x44_ChipKillEccCap_MASK                            0x800000
#define D18F3x44_IoRdDatErrEn_OFFSET                            24
#define D18F3x44_IoRdDatErrEn_WIDTH                             1
#define D18F3x44_IoRdDatErrEn_MASK                              0x1000000
#define D18F3x44_DisPciCfgCpuErrRsp_OFFSET                      25
#define D18F3x44_DisPciCfgCpuErrRsp_WIDTH                       1
#define D18F3x44_DisPciCfgCpuErrRsp_MASK                        0x2000000
#define D18F3x44_FlagMcaCorrErr_OFFSET                          26
#define D18F3x44_FlagMcaCorrErr_WIDTH                           1
#define D18F3x44_FlagMcaCorrErr_MASK                            0x4000000
#define D18F3x44_NbMcaToMstCpuEn_OFFSET                         27
#define D18F3x44_NbMcaToMstCpuEn_WIDTH                          1
#define D18F3x44_NbMcaToMstCpuEn_MASK                           0x8000000
#define D18F3x44_NbMcaToMstCpuEn_VALUE                          0x1
#define D18F3x44_DisTgtAbortCpuErrRsp_OFFSET                    28
#define D18F3x44_DisTgtAbortCpuErrRsp_WIDTH                     1
#define D18F3x44_DisTgtAbortCpuErrRsp_MASK                      0x10000000
#define D18F3x44_DisMstAbortCpuErrRsp_OFFSET                    29
#define D18F3x44_DisMstAbortCpuErrRsp_WIDTH                     1
#define D18F3x44_DisMstAbortCpuErrRsp_MASK                      0x20000000
#define D18F3x44_SyncFloodOnDramAdrParErr_OFFSET                30
#define D18F3x44_SyncFloodOnDramAdrParErr_WIDTH                 1
#define D18F3x44_SyncFloodOnDramAdrParErr_MASK                  0x40000000
#define D18F3x44_SyncFloodOnDramAdrParErr_VALUE                 0x1
#define D18F3x44_NbMcaLogEn_OFFSET                              31
#define D18F3x44_NbMcaLogEn_WIDTH                               1
#define D18F3x44_NbMcaLogEn_MASK                                0x80000000

/// D18F3x44
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                            CpuRdDatErrEn:1 ; ///<
    UINT32                                     SyncFloodOnDramUcEcc:1 ; ///<
    UINT32                                            SyncPktGenDis:1 ; ///<
    UINT32                                           SyncPktPropDis:1 ; ///<
    UINT32                                            IoMstAbortDis:1 ; ///<
    UINT32                                                CpuErrDis:1 ; ///<
    UINT32                                                 IoErrDis:1 ; ///<
    UINT32                                                   WDTDis:1 ; ///<
    UINT32                                           WDTCntSel_2_0_:3 ; ///<
    UINT32                                               WDTBaseSel:2 ; ///<
    UINT32                                               GenLinkSel:2 ; ///<
    UINT32                                           GenCrcErrByte0:1 ; ///<
    UINT32                                           GenCrcErrByte1:1 ; ///<
    UINT32                                            GenSubLinkSel:2 ; ///<
    UINT32                                           SyncFloodOnWDT:1 ; ///<
    UINT32                                      SyncFloodOnAnyUcErr:1 ; ///<
    UINT32                                                DramEccEn:1 ; ///<
    UINT32                                           ChipKillEccCap:1 ; ///<
    UINT32                                             IoRdDatErrEn:1 ; ///<
    UINT32                                       DisPciCfgCpuErrRsp:1 ; ///<
    UINT32                                           FlagMcaCorrErr:1 ; ///<
    UINT32                                          NbMcaToMstCpuEn:1 ; ///<
    UINT32                                     DisTgtAbortCpuErrRsp:1 ; ///<
    UINT32                                     DisMstAbortCpuErrRsp:1 ; ///<
    UINT32                                 SyncFloodOnDramAdrParErr:1 ; ///<
    UINT32                                               NbMcaLogEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x44_STRUCT;

// **** D18F3x64 Register Definition ****
// Address
#define D18F3x64_ADDRESS                                        0x64
// Type
#define D18F3x64_TYPE                                           TYPE_D18F3

// Field Data
#define D18F3x64_HtcEn_OFFSET                                   0
#define D18F3x64_HtcEn_WIDTH                                    1
#define D18F3x64_HtcEn_MASK                                     0x1
#define D18F3x64_Reserved_3_1_OFFSET                            1
#define D18F3x64_Reserved_3_1_WIDTH                             3
#define D18F3x64_Reserved_3_1_MASK                              0xE
#define D18F3x64_HtcAct_OFFSET                                  4
#define D18F3x64_HtcAct_WIDTH                                   1
#define D18F3x64_HtcAct_MASK                                    0x10
#define D18F3x64_HtcActSts_OFFSET                               5
#define D18F3x64_HtcActSts_WIDTH                                1
#define D18F3x64_HtcActSts_MASK                                 0x20
#define D18F3x64_PslApicHiEn_OFFSET                             6
#define D18F3x64_PslApicHiEn_WIDTH                              1
#define D18F3x64_PslApicHiEn_MASK                               0x40
#define D18F3x64_PslApicLoEn_OFFSET                             7
#define D18F3x64_PslApicLoEn_WIDTH                              1
#define D18F3x64_PslApicLoEn_MASK                               0x80
#define D18F3x64_Reserved_15_8_OFFSET                           8
#define D18F3x64_Reserved_15_8_WIDTH                            8
#define D18F3x64_Reserved_15_8_MASK                             0xFF00
#define D18F3x64_HtcTmpLmt_OFFSET                               16
#define D18F3x64_HtcTmpLmt_WIDTH                                7
#define D18F3x64_HtcTmpLmt_MASK                                 0x7F0000
#define D18F3x64_HtcSlewSel_OFFSET                              23
#define D18F3x64_HtcSlewSel_WIDTH                               1
#define D18F3x64_HtcSlewSel_MASK                                0x800000
#define D18F3x64_HtcHystLmt_OFFSET                              24
#define D18F3x64_HtcHystLmt_WIDTH                               4
#define D18F3x64_HtcHystLmt_MASK                                0xF000000
#define D18F3x64_HtcPstateLimit_OFFSET                          28
#define D18F3x64_HtcPstateLimit_WIDTH                           3
#define D18F3x64_HtcPstateLimit_MASK                            0x70000000
#define D18F3x64_Reserved_31_31_OFFSET                          31
#define D18F3x64_Reserved_31_31_WIDTH                           1
#define D18F3x64_Reserved_31_31_MASK                            0x80000000

/// D18F3x64
typedef union {
  struct {                                                            ///<
    UINT32                                                   HtcEn:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                                  HtcAct:1; ///<
    UINT32                                               HtcActSts:1; ///<
    UINT32                                             PslApicHiEn:1; ///<
    UINT32                                             PslApicLoEn:1; ///<
    UINT32                                           Reserved_15_8:8; ///<
    UINT32                                               HtcTmpLmt:7; ///<
    UINT32                                              HtcSlewSel:1; ///<
    UINT32                                              HtcHystLmt:4; ///<
    UINT32                                          HtcPstateLimit:3; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D18F3x64_STRUCT;

// **** D18F3xA0 Register Definition ****
// Address
#define D18F3xA0_ADDRESS                                        0xA0
// Type
#define D18F3xA0_TYPE                                           TYPE_D18F3

// Field Data
#define D18F3xA0_PsiVid_6_0_OFFSET                              0
#define D18F3xA0_PsiVid_6_0_WIDTH                               7
#define D18F3xA0_PsiVid_6_0_MASK                                0x7F
#define D18F3xA0_PsiVidEn_OFFSET                                7
#define D18F3xA0_PsiVidEn_WIDTH                                 1
#define D18F3xA0_PsiVidEn_MASK                                  0x80
#define D18F3xA0_PsiVid_7_OFFSET                                8
#define D18F3xA0_PsiVid_7_WIDTH                                 1
#define D18F3xA0_PsiVid_7_MASK                                  0x100
#define D18F3xA0_Reserved_9_9_OFFSET                            9
#define D18F3xA0_Reserved_9_9_WIDTH                             1
#define D18F3xA0_Reserved_9_9_MASK                              0x200
#define D18F3xA0_Reserved_10_10_OFFSET                          10
#define D18F3xA0_Reserved_10_10_WIDTH                           1
#define D18F3xA0_Reserved_10_10_MASK                            0x400
#define D18F3xA0_PllLockTime_OFFSET                             11
#define D18F3xA0_PllLockTime_WIDTH                              3
#define D18F3xA0_PllLockTime_MASK                               0x3800
#define D18F3xA0_Svi2HighFreqSel_OFFSET                         14
#define D18F3xA0_Svi2HighFreqSel_WIDTH                          1
#define D18F3xA0_Svi2HighFreqSel_MASK                           0x4000
#define D18F3xA0_Reserved_15_15_OFFSET                          15
#define D18F3xA0_Reserved_15_15_WIDTH                           1
#define D18F3xA0_Reserved_15_15_MASK                            0x8000
#define D18F3xA0_ConfigId_OFFSET                                16
#define D18F3xA0_ConfigId_WIDTH                                 12
#define D18F3xA0_ConfigId_MASK                                  0xFFF0000
#define D18F3xA0_Reserved_30_28_OFFSET                          28
#define D18F3xA0_Reserved_30_28_WIDTH                           3
#define D18F3xA0_Reserved_30_28_MASK                            0x70000000
#define D18F3xA0_CofVidProg_OFFSET                              31
#define D18F3xA0_CofVidProg_WIDTH                               1
#define D18F3xA0_CofVidProg_MASK                                0x80000000

/// D18F3xA0
typedef union {
  struct {                                                            ///<
    UINT32                                              PsiVid_6_0:7; ///<
    UINT32                                                PsiVidEn:1; ///<
    UINT32                                                PsiVid_7:1; ///<
    UINT32                                            Reserved_9_9:1; ///<
    UINT32                                          Reserved_10_10:1; ///<
    UINT32                                             PllLockTime:3; ///<
    UINT32                                         Svi2HighFreqSel:1; ///<
    UINT32                                          Reserved_15_15:1; ///<
    UINT32                                                ConfigId:12; ///<
    UINT32                                          Reserved_30_28:3; ///<
    UINT32                                              CofVidProg:1; ///<

  } Field;

  UINT32 Value;
} D18F3xA0_STRUCT;

// **** D18F3xA8 Register Definition ****
// Address
#define D18F3xA8_ADDRESS                                        0xA8
// Type
#define D18F3xA8_TYPE                                           TYPE_D18F3

// Field Data
#define D18F3xA8_Reserved_28_0_OFFSET                           0
#define D18F3xA8_Reserved_28_0_WIDTH                            29
#define D18F3xA8_Reserved_28_0_MASK                             0x1FFFFFFF
#define D18F3xA8_PopDownPstate_OFFSET                           29
#define D18F3xA8_PopDownPstate_WIDTH                            3
#define D18F3xA8_PopDownPstate_MASK                             0xE0000000

/// D18F3xA8
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_28_0:29; ///<
    UINT32                                           PopDownPstate:3; ///<

  } Field;

  UINT32 Value;
} D18F3xA8_STRUCT;

// **** D18F3x1FC Register Definition ****
// Address
#define D18F3x1FC_ADDRESS                                       0x1fc

// Type
#define D18F3x1FC_TYPE                                          TYPE_D18F3
// Field Data
#define D18F3x1FC_DiDtMode_OFFSET                               0
#define D18F3x1FC_DiDtMode_WIDTH                                1
#define D18F3x1FC_DiDtMode_MASK                                 0x1
#define D18F3x1FC_DiDtCfg0_OFFSET                               1
#define D18F3x1FC_DiDtCfg0_WIDTH                                5
#define D18F3x1FC_DiDtCfg0_MASK                                 0x3e
#define D18F3x1FC_DiDtCfg1_OFFSET                               6
#define D18F3x1FC_DiDtCfg1_WIDTH                                8
#define D18F3x1FC_DiDtCfg1_MASK                                 0x3fc0
#define D18F3x1FC_DiDtCfg2_OFFSET                               14
#define D18F3x1FC_DiDtCfg2_WIDTH                                2
#define D18F3x1FC_DiDtCfg2_MASK                                 0xc000
#define D18F3x1FC_DiDtCfg3_OFFSET                               16
#define D18F3x1FC_DiDtCfg3_WIDTH                                1
#define D18F3x1FC_DiDtCfg3_MASK                                 0x10000
#define D18F3x1FC_DiDtCfg4_OFFSET                               17
#define D18F3x1FC_DiDtCfg4_WIDTH                                4
#define D18F3x1FC_DiDtCfg4_MASK                                 0x1e0000
#define D18F3x1FC_VddrLowVoltageSupport_OFFSET                  21
#define D18F3x1FC_VddrLowVoltageSupport_WIDTH                   1
#define D18F3x1FC_VddrLowVoltageSupport_MASK                    0x200000
#define D18F3x1FC_Bitfield_22_22_OFFSET                         22
#define D18F3x1FC_Bitfield_22_22_WIDTH                          1
#define D18F3x1FC_Bitfield_22_22_MASK                           0x400000
#define D18F3x1FC_Reserved_31_23_OFFSET                         23
#define D18F3x1FC_Reserved_31_23_WIDTH                          9
#define D18F3x1FC_Reserved_31_23_MASK                           0xff800000

/// D18F3x1FC
typedef union {
  struct {                                                              ///<
    UINT32                                                 DiDtMode:1 ; ///<
    UINT32                                                 DiDtCfg0:5 ; ///<
    UINT32                                                 DiDtCfg1:8 ; ///<
    UINT32                                                 DiDtCfg2:2 ; ///<
    UINT32                                                 DiDtCfg3:1 ; ///<
    UINT32                                                 DiDtCfg4:4 ; ///<
    UINT32                                    VddrLowVoltageSupport:1 ; ///<
    UINT32                                           Bitfield_22_22:1 ; ///<
    UINT32                                           Reserved_31_23:9 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x1FC_STRUCT;

// **** D18F5xE8 Register Definition ****
// Address
#define D18F5xE8_ADDRESS                                        0xe8

// Type
#define D18F5xE8_TYPE                                           TYPE_D18F5
// Field Data
#define D18F5xE8_Tdp2Watt_OFFSET                                0
#define D18F5xE8_Tdp2Watt_WIDTH                                 10
#define D18F5xE8_Tdp2Watt_MASK                                  0x3ff
#define D18F5xE8_Reserved_5_0_OFFSET                            10
#define D18F5xE8_Reserved_5_0_WIDTH                             6
#define D18F5xE8_Reserved_5_0_MASK                              0xfc00
#define D18F5xE8_ApmTdpLimit_OFFSET                             16
#define D18F5xE8_ApmTdpLimit_WIDTH                              13
#define D18F5xE8_ApmTdpLimit_MASK                               0x1fff0000
#define D18F5xE8_Reserved_31_29_OFFSET                          29
#define D18F5xE8_Reserved_31_29_WIDTH                           3
#define D18F5xE8_Reserved_31_29_MASK                            0xe0000000

/// D18F5xE8
typedef union {
  struct {                                                              ///<
    UINT32                                                 Tdp2Watt:10; ///<
    UINT32                                             Reserved_5_0:6 ; ///<
    UINT32                                              ApmTdpLimit:13; ///<
    UINT32                                           Reserved_31_29:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F5xE8_STRUCT;

// **** D18F5x12C Register Definition ****
// Address
#define D18F5x12C_ADDRESS                                       0x12C
// Type
#define D18F5x12C_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x12C_CoreOffsetTrim_OFFSET                         0
#define D18F5x12C_CoreOffsetTrim_WIDTH                          2
#define D18F5x12C_CoreOffsetTrim_MASK                           0x3
#define D18F5x12C_CoreLoadLineTrim_OFFSET                       2
#define D18F5x12C_CoreLoadLineTrim_WIDTH                        3
#define D18F5x12C_CoreLoadLineTrim_MASK                         0x1C
#define D18F5x12C_CorePsi1En_OFFSET                             5
#define D18F5x12C_CorePsi1En_WIDTH                              1
#define D18F5x12C_CorePsi1En_MASK                               0x20
#define D18F5x12C_CoreTfn_OFFSET                                6
#define D18F5x12C_CoreTfn_WIDTH                                 1
#define D18F5x12C_CoreTfn_MASK                                  0x40
#define D18F5x12C_RAZ_29_7_OFFSET                               7
#define D18F5x12C_RAZ_29_7_WIDTH                                23
#define D18F5x12C_RAZ_29_7_MASK                                 0x3FFFFF80
#define D18F5x12C_WaitVidCompDis_OFFSET                         30
#define D18F5x12C_WaitVidCompDis_WIDTH                          1
#define D18F5x12C_WaitVidCompDis_MASK                           0x40000000
#define D18F5x12C_Svi2CmdBusy_OFFSET                            31
#define D18F5x12C_Svi2CmdBusy_WIDTH                             1
#define D18F5x12C_Svi2CmdBusy_MASK                              0x80000000

/// D18F5x12C
typedef union {
  struct {                                                            ///<
    UINT32                                          CoreOffsetTrim:2; ///<
    UINT32                                        CoreLoadLineTrim:3; ///<
    UINT32                                              CorePsi1En:1; ///<
    UINT32                                                 CoreTfn:1; ///<
    UINT32                                                RAZ_29_7:23; ///<
    UINT32                                          WaitVidCompDis:1; ///<
    UINT32                                             Svi2CmdBusy:1; ///<

  } Field;

  UINT32 Value;
} D18F5x12C_STRUCT;

// **** D18F5x160 Register Definition ****
// Address
#define D18F5x160_ADDRESS                                       0x160
// Type
#define D18F5x160_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x160_NbPstateEn_OFFSET                             0
#define D18F5x160_NbPstateEn_WIDTH                              1
#define D18F5x160_NbPstateEn_MASK                               0x1
#define D18F5x160_NbFid_OFFSET                                  1
#define D18F5x160_NbFid_WIDTH                                   6
#define D18F5x160_NbFid_MASK                                    0x7E
#define D18F5x160_NbDid_OFFSET                                  7
#define D18F5x160_NbDid_WIDTH                                   1
#define D18F5x160_NbDid_MASK                                    0x80
#define D18F5x160_Reserved_9_8_OFFSET                           8
#define D18F5x160_Reserved_9_8_WIDTH                            2
#define D18F5x160_Reserved_9_8_MASK                             0x300
#define D18F5x160_NbVid_6_0_OFFSET                              10
#define D18F5x160_NbVid_6_0_WIDTH                               7
#define D18F5x160_NbVid_6_0_MASK                                0x1FC00
#define D18F5x160_Reserved_17_17_OFFSET                         17
#define D18F5x160_Reserved_17_17_WIDTH                          1
#define D18F5x160_Reserved_17_17_MASK                           0x20000
#define D18F5x160_MemPstate_OFFSET                              18
#define D18F5x160_MemPstate_WIDTH                               1
#define D18F5x160_MemPstate_MASK                                0x40000
#define D18F5x160_Reserved_20_19_OFFSET                         19
#define D18F5x160_Reserved_20_19_WIDTH                          2
#define D18F5x160_Reserved_20_19_MASK                           0x180000
#define D18F5x160_NbVid_7_OFFSET                                21
#define D18F5x160_NbVid_7_WIDTH                                 1
#define D18F5x160_NbVid_7_MASK                                  0x200000
#define D18F5x160_NbIddDiv_OFFSET                               22
#define D18F5x160_NbIddDiv_WIDTH                                2
#define D18F5x160_NbIddDiv_MASK                                 0xC00000
#define D18F5x160_NbIddValue_OFFSET                             24
#define D18F5x160_NbIddValue_WIDTH                              8
#define D18F5x160_NbIddValue_MASK                               0xFF000000

/// D18F5x160
typedef union {
  struct {                                                            ///<
    UINT32                                              NbPstateEn:1; ///<
    UINT32                                                   NbFid:6; ///<
    UINT32                                                   NbDid:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                               NbVid_6_0:7; ///<
    UINT32                                          Reserved_17_17:1; ///<
    UINT32                                               MemPstate:1; ///<
    UINT32                                          Reserved_20_19:2; ///<
    UINT32                                                 NbVid_7:1; ///<
    UINT32                                                NbIddDiv:2; ///<
    UINT32                                              NbIddValue:8; ///<

  } Field;

  UINT32 Value;
} D18F5x160_STRUCT;

// **** D18F5x164 Register Definition ****
// Address
#define D18F5x164_ADDRESS                                       0x164
// Type
#define D18F5x164_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x164_NbPstateEn_OFFSET                             0
#define D18F5x164_NbPstateEn_WIDTH                              1
#define D18F5x164_NbPstateEn_MASK                               0x1
#define D18F5x164_NbFid_OFFSET                                  1
#define D18F5x164_NbFid_WIDTH                                   6
#define D18F5x164_NbFid_MASK                                    0x7E
#define D18F5x164_NbDid_OFFSET                                  7
#define D18F5x164_NbDid_WIDTH                                   1
#define D18F5x164_NbDid_MASK                                    0x80
#define D18F5x164_Reserved_9_8_OFFSET                           8
#define D18F5x164_Reserved_9_8_WIDTH                            2
#define D18F5x164_Reserved_9_8_MASK                             0x300
#define D18F5x164_NbVid_6_0_OFFSET                              10
#define D18F5x164_NbVid_6_0_WIDTH                               7
#define D18F5x164_NbVid_6_0_MASK                                0x1FC00
#define D18F5x164_Reserved_17_17_OFFSET                         17
#define D18F5x164_Reserved_17_17_WIDTH                          1
#define D18F5x164_Reserved_17_17_MASK                           0x20000
#define D18F5x164_MemPstate_OFFSET                              18
#define D18F5x164_MemPstate_WIDTH                               1
#define D18F5x164_MemPstate_MASK                                0x40000
#define D18F5x164_Reserved_20_19_OFFSET                         19
#define D18F5x164_Reserved_20_19_WIDTH                          2
#define D18F5x164_Reserved_20_19_MASK                           0x180000
#define D18F5x164_NbVid_7_OFFSET                                21
#define D18F5x164_NbVid_7_WIDTH                                 1
#define D18F5x164_NbVid_7_MASK                                  0x200000
#define D18F5x164_NbIddDiv_OFFSET                               22
#define D18F5x164_NbIddDiv_WIDTH                                2
#define D18F5x164_NbIddDiv_MASK                                 0xC00000
#define D18F5x164_NbIddValue_OFFSET                             24
#define D18F5x164_NbIddValue_WIDTH                              8
#define D18F5x164_NbIddValue_MASK                               0xFF000000

/// D18F5x164
typedef union {
  struct {                                                            ///<
    UINT32                                              NbPstateEn:1; ///<
    UINT32                                                   NbFid:6; ///<
    UINT32                                                   NbDid:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                               NbVid_6_0:7; ///<
    UINT32                                          Reserved_17_17:1; ///<
    UINT32                                               MemPstate:1; ///<
    UINT32                                          Reserved_20_19:2; ///<
    UINT32                                                 NbVid_7:1; ///<
    UINT32                                                NbIddDiv:2; ///<
    UINT32                                              NbIddValue:8; ///<

  } Field;

  UINT32 Value;
} D18F5x164_STRUCT;

// **** D18F5x168 Register Definition ****
// Address
#define D18F5x168_ADDRESS                                       0x168
// Type
#define D18F5x168_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x168_NbPstateEn_OFFSET                             0
#define D18F5x168_NbPstateEn_WIDTH                              1
#define D18F5x168_NbPstateEn_MASK                               0x1
#define D18F5x168_NbFid_OFFSET                                  1
#define D18F5x168_NbFid_WIDTH                                   6
#define D18F5x168_NbFid_MASK                                    0x7E
#define D18F5x168_NbDid_OFFSET                                  7
#define D18F5x168_NbDid_WIDTH                                   1
#define D18F5x168_NbDid_MASK                                    0x80
#define D18F5x168_Reserved_9_8_OFFSET                           8
#define D18F5x168_Reserved_9_8_WIDTH                            2
#define D18F5x168_Reserved_9_8_MASK                             0x300
#define D18F5x168_NbVid_6_0_OFFSET                              10
#define D18F5x168_NbVid_6_0_WIDTH                               7
#define D18F5x168_NbVid_6_0_MASK                                0x1FC00
#define D18F5x168_Reserved_17_17_OFFSET                         17
#define D18F5x168_Reserved_17_17_WIDTH                          1
#define D18F5x168_Reserved_17_17_MASK                           0x20000
#define D18F5x168_MemPstate_OFFSET                              18
#define D18F5x168_MemPstate_WIDTH                               1
#define D18F5x168_MemPstate_MASK                                0x40000
#define D18F5x168_Reserved_20_19_OFFSET                         19
#define D18F5x168_Reserved_20_19_WIDTH                          2
#define D18F5x168_Reserved_20_19_MASK                           0x180000
#define D18F5x168_NbVid_7_OFFSET                                21
#define D18F5x168_NbVid_7_WIDTH                                 1
#define D18F5x168_NbVid_7_MASK                                  0x200000
#define D18F5x168_NbIddDiv_OFFSET                               22
#define D18F5x168_NbIddDiv_WIDTH                                2
#define D18F5x168_NbIddDiv_MASK                                 0xC00000
#define D18F5x168_NbIddValue_OFFSET                             24
#define D18F5x168_NbIddValue_WIDTH                              8
#define D18F5x168_NbIddValue_MASK                               0xFF000000

/// D18F5x168
typedef union {
  struct {                                                            ///<
    UINT32                                              NbPstateEn:1; ///<
    UINT32                                                   NbFid:6; ///<
    UINT32                                                   NbDid:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                               NbVid_6_0:7; ///<
    UINT32                                          Reserved_17_17:1; ///<
    UINT32                                               MemPstate:1; ///<
    UINT32                                          Reserved_20_19:2; ///<
    UINT32                                                 NbVid_7:1; ///<
    UINT32                                                NbIddDiv:2; ///<
    UINT32                                              NbIddValue:8; ///<

  } Field;

  UINT32 Value;
} D18F5x168_STRUCT;

// **** D18F5x16C Register Definition ****
// Address
#define D18F5x16C_ADDRESS                                       0x16C
// Type
#define D18F5x16C_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x16C_NbPstateEn_OFFSET                             0
#define D18F5x16C_NbPstateEn_WIDTH                              1
#define D18F5x16C_NbPstateEn_MASK                               0x1
#define D18F5x16C_NbFid_OFFSET                                  1
#define D18F5x16C_NbFid_WIDTH                                   6
#define D18F5x16C_NbFid_MASK                                    0x7E
#define D18F5x16C_NbDid_OFFSET                                  7
#define D18F5x16C_NbDid_WIDTH                                   1
#define D18F5x16C_NbDid_MASK                                    0x80
#define D18F5x16C_Reserved_9_8_OFFSET                           8
#define D18F5x16C_Reserved_9_8_WIDTH                            2
#define D18F5x16C_Reserved_9_8_MASK                             0x300
#define D18F5x16C_NbVid_6_0_OFFSET                              10
#define D18F5x16C_NbVid_6_0_WIDTH                               7
#define D18F5x16C_NbVid_6_0_MASK                                0x1FC00
#define D18F5x16C_Reserved_17_17_OFFSET                         17
#define D18F5x16C_Reserved_17_17_WIDTH                          1
#define D18F5x16C_Reserved_17_17_MASK                           0x20000
#define D18F5x16C_MemPstate_OFFSET                              18
#define D18F5x16C_MemPstate_WIDTH                               1
#define D18F5x16C_MemPstate_MASK                                0x40000
#define D18F5x16C_Reserved_20_19_OFFSET                         19
#define D18F5x16C_Reserved_20_19_WIDTH                          2
#define D18F5x16C_Reserved_20_19_MASK                           0x180000
#define D18F5x16C_NbVid_7_OFFSET                                21
#define D18F5x16C_NbVid_7_WIDTH                                 1
#define D18F5x16C_NbVid_7_MASK                                  0x200000
#define D18F5x16C_NbIddDiv_OFFSET                               22
#define D18F5x16C_NbIddDiv_WIDTH                                2
#define D18F5x16C_NbIddDiv_MASK                                 0xC00000
#define D18F5x16C_NbIddValue_OFFSET                             24
#define D18F5x16C_NbIddValue_WIDTH                              8
#define D18F5x16C_NbIddValue_MASK                               0xFF000000

/// D18F5x16C
typedef union {
  struct {                                                            ///<
    UINT32                                              NbPstateEn:1; ///<
    UINT32                                                   NbFid:6; ///<
    UINT32                                                   NbDid:1; ///<
    UINT32                                            Reserved_9_8:2; ///<
    UINT32                                               NbVid_6_0:7; ///<
    UINT32                                          Reserved_17_17:1; ///<
    UINT32                                               MemPstate:1; ///<
    UINT32                                          Reserved_20_19:2; ///<
    UINT32                                                 NbVid_7:1; ///<
    UINT32                                                NbIddDiv:2; ///<
    UINT32                                              NbIddValue:8; ///<

  } Field;

  UINT32 Value;
} D18F5x16C_STRUCT;

// **** D18F5x170 Register Definition ****
// Address
#define D18F5x170_ADDRESS                                       0x170
// Type
#define D18F5x170_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x170_NbPstateMaxVal_OFFSET                         0
#define D18F5x170_NbPstateMaxVal_WIDTH                          2
#define D18F5x170_NbPstateMaxVal_MASK                           0x3
#define D18F5x170_Reserved_2_2_OFFSET                           2
#define D18F5x170_Reserved_2_2_WIDTH                            1
#define D18F5x170_Reserved_2_2_MASK                             0x4
#define D18F5x170_NbPstateLo_OFFSET                             3
#define D18F5x170_NbPstateLo_WIDTH                              2
#define D18F5x170_NbPstateLo_MASK                               0x18
#define D18F5x170_Reserved_5_5_OFFSET                           5
#define D18F5x170_Reserved_5_5_WIDTH                            1
#define D18F5x170_Reserved_5_5_MASK                             0x20
#define D18F5x170_NbPstateHi_OFFSET                             6
#define D18F5x170_NbPstateHi_WIDTH                              2
#define D18F5x170_NbPstateHi_MASK                               0xC0
#define D18F5x170_Reserved_8_8_OFFSET                           8
#define D18F5x170_Reserved_8_8_WIDTH                            1
#define D18F5x170_Reserved_8_8_MASK                             0x100
#define D18F5x170_NbPstateThreshold_OFFSET                      9
#define D18F5x170_NbPstateThreshold_WIDTH                       3
#define D18F5x170_NbPstateThreshold_MASK                        0xE00
#define D18F5x170_Reserved_12_12_OFFSET                         12
#define D18F5x170_Reserved_12_12_WIDTH                          1
#define D18F5x170_Reserved_12_12_MASK                           0x1000
#define D18F5x170_NbPstateDisOnP0_OFFSET                        13
#define D18F5x170_NbPstateDisOnP0_WIDTH                         1
#define D18F5x170_NbPstateDisOnP0_MASK                          0x2000
#define D18F5x170_SwNbPstateLoDis_OFFSET                        14
#define D18F5x170_SwNbPstateLoDis_WIDTH                         1
#define D18F5x170_SwNbPstateLoDis_MASK                          0x4000
#define D18F5x170_Reserved_23_15_OFFSET                         15
#define D18F5x170_Reserved_23_15_WIDTH                          9
#define D18F5x170_Reserved_23_15_MASK                           0xFF8000
#define D18F5x170_NbPstateLoRes_OFFSET                          24
#define D18F5x170_NbPstateLoRes_WIDTH                           3
#define D18F5x170_NbPstateLoRes_MASK                            0x7000000
#define D18F5x170_NbPstateHiRes_OFFSET                          27
#define D18F5x170_NbPstateHiRes_WIDTH                           3
#define D18F5x170_NbPstateHiRes_MASK                            0x38000000
#define D18F5x170_Reserved_30_30_OFFSET                         30
#define D18F5x170_Reserved_30_30_WIDTH                          1
#define D18F5x170_Reserved_30_30_MASK                           0x40000000
#define D18F5x170_MemPstateDis_OFFSET                           31
#define D18F5x170_MemPstateDis_WIDTH                            1
#define D18F5x170_MemPstateDis_MASK                             0x80000000

/// D18F5x170
typedef union {
  struct {                                                            ///<
    UINT32                                          NbPstateMaxVal:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                              NbPstateLo:2; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                              NbPstateHi:2; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                       NbPstateThreshold:3; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                         NbPstateDisOnP0:1; ///<
    UINT32                                         SwNbPstateLoDis:1; ///<
    UINT32                                          Reserved_23_15:9; ///<
    UINT32                                           NbPstateLoRes:3; ///<
    UINT32                                           NbPstateHiRes:3; ///<
    UINT32                                          Reserved_30_30:1; ///<
    UINT32                                            MemPstateDis:1; ///<

  } Field;

  UINT32 Value;
} D18F5x170_STRUCT;

// **** D18F5x178 Register Definition ****
// Address
#define D18F5x178_ADDRESS                                       0x178
// Type
#define D18F5x178_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x178_Reserved_1_0_OFFSET                           0
#define D18F5x178_Reserved_1_0_WIDTH                            2
#define D18F5x178_Reserved_1_0_MASK                             0x3
#define D18F5x178_CstateFusionDis_OFFSET                        2
#define D18F5x178_CstateFusionDis_WIDTH                         1
#define D18F5x178_CstateFusionDis_MASK                          0x4
#define D18F5x178_CstateThreeWayHsEn_OFFSET                     3
#define D18F5x178_CstateThreeWayHsEn_WIDTH                      1
#define D18F5x178_CstateThreeWayHsEn_MASK                       0x8
#define D18F5x178_Reserved_17_4_OFFSET                          4
#define D18F5x178_Reserved_17_4_WIDTH                           14
#define D18F5x178_Reserved_17_4_MASK                            0x3FFF0
#define D18F5x178_CstateFusionHsDis_OFFSET                      18
#define D18F5x178_CstateFusionHsDis_WIDTH                       1
#define D18F5x178_CstateFusionHsDis_MASK                        0x40000
#define D18F5x178_SwGfxDis_OFFSET                               19
#define D18F5x178_SwGfxDis_WIDTH                                1
#define D18F5x178_SwGfxDis_MASK                                 0x80000
#define D18F5x178_Reserved_31_20_OFFSET                         20
#define D18F5x178_Reserved_31_20_WIDTH                          12
#define D18F5x178_Reserved_31_20_MASK                           0xFFF00000

/// D18F5x178
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                         CstateFusionDis:1; ///<
    UINT32                                      CstateThreeWayHsEn:1; ///<
    UINT32                                           Reserved_17_4:14; ///<
    UINT32                                       CstateFusionHsDis:1; ///<
    UINT32                                                SwGfxDis:1; ///<
    UINT32                                          Reserved_31_20:12; ///<

  } Field;

  UINT32 Value;
} D18F5x178_STRUCT;

// **** D18F5x188 Register Definition ****
// Address
#define D18F5x188_ADDRESS                                       0x188
// Type
#define D18F5x188_TYPE                                          TYPE_D18F5

// Field Data
#define D18F5x188_NbOffsetTrim_OFFSET                           0
#define D18F5x188_NbOffsetTrim_WIDTH                            2
#define D18F5x188_NbOffsetTrim_MASK                             0x3
#define D18F5x188_NbLoadLineTrim_OFFSET                         2
#define D18F5x188_NbLoadLineTrim_WIDTH                          3
#define D18F5x188_NbLoadLineTrim_MASK                           0x1C
#define D18F5x188_NbPsi1_OFFSET                                 5
#define D18F5x188_NbPsi1_WIDTH                                  1
#define D18F5x188_NbPsi1_MASK                                   0x20
#define D18F5x188_NbTfn_OFFSET                                  6
#define D18F5x188_NbTfn_WIDTH                                   1
#define D18F5x188_NbTfn_MASK                                    0x40
#define D18F5x188_RAZ_31_7_OFFSET                               7
#define D18F5x188_RAZ_31_7_WIDTH                                25
#define D18F5x188_RAZ_31_7_MASK                                 0xFFFFFF80

/// D18F5x188
typedef union {
  struct {                                                            ///<
    UINT32                                            NbOffsetTrim:2; ///<
    UINT32                                          NbLoadLineTrim:3; ///<
    UINT32                                                  NbPsi1:1; ///<
    UINT32                                                   NbTfn:1; ///<
    UINT32                                                RAZ_31_7:25; ///<

  } Field;

  UINT32 Value;
} D18F5x188_STRUCT;

// **** D0F0x64_x0D Register Definition ****
// Address
#define D0F0x64_x0D_ADDRESS                                     0xD
// Type
#define D0F0x64_x0D_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x0D_PciDev0Fn2RegEn_OFFSET                      0
#define D0F0x64_x0D_PciDev0Fn2RegEn_WIDTH                       1
#define D0F0x64_x0D_PciDev0Fn2RegEn_MASK                        0x1
#define D0F0x64_x0D_Reserved_30_1_OFFSET                        1
#define D0F0x64_x0D_Reserved_30_1_WIDTH                         30
#define D0F0x64_x0D_Reserved_30_1_MASK                          0x7FFFFFFE
#define D0F0x64_x0D_IommuDis_OFFSET                             31
#define D0F0x64_x0D_IommuDis_WIDTH                              1
#define D0F0x64_x0D_IommuDis_MASK                               0x80000000

/// D0F0x64_x0D
typedef union {
  struct {                                                            ///<
    UINT32                                         PciDev0Fn2RegEn:1; ///<
    UINT32                                           Reserved_30_1:30; ///<
    UINT32                                                IommuDis:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x0D_STRUCT;

// **** D0F0x64_x16 Register Definition ****
// Address
#define D0F0x64_x16_ADDRESS                                     0x16

// Type
#define D0F0x64_x16_TYPE                                        TYPE_D0F0x64
// Field Data
#define D0F0x64_x16_AerUrMsgEn_OFFSET                           0
#define D0F0x64_x16_AerUrMsgEn_WIDTH                            1
#define D0F0x64_x16_AerUrMsgEn_MASK                             0x1
#define D0F0x64_x16_Reserved_31_1_OFFSET                        1
#define D0F0x64_x16_Reserved_31_1_WIDTH                         31
#define D0F0x64_x16_Reserved_31_1_MASK                          0xfffffffe

/// D0F0x64_x16
typedef union {
  struct {                                                              ///<
    UINT32                                               AerUrMsgEn:1 ; ///<
    UINT32                                            Reserved_31_1:31; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x64_x16_STRUCT;

// **** D0F0x64_x1D Register Definition ****
// Address
#define D0F0x64_x1D_ADDRESS                                     0x1D
// Type
#define D0F0x64_x1D_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x1D_Reserved_0_0_OFFSET                         0
#define D0F0x64_x1D_Reserved_0_0_WIDTH                          1
#define D0F0x64_x1D_Reserved_0_0_MASK                           0x1
#define D0F0x64_x1D_VgaEn_OFFSET                                1
#define D0F0x64_x1D_VgaEn_WIDTH                                 1
#define D0F0x64_x1D_VgaEn_MASK                                  0x2
#define D0F0x64_x1D_Reserved_2_2_OFFSET                         2
#define D0F0x64_x1D_Reserved_2_2_WIDTH                          1
#define D0F0x64_x1D_Reserved_2_2_MASK                           0x4
#define D0F0x64_x1D_Vga16En_OFFSET                              3
#define D0F0x64_x1D_Vga16En_WIDTH                               1
#define D0F0x64_x1D_Vga16En_MASK                                0x8
#define D0F0x64_x1D_Reserved_31_4_OFFSET                        4
#define D0F0x64_x1D_Reserved_31_4_WIDTH                         28
#define D0F0x64_x1D_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F0x64_x1D
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                   VgaEn:1; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                                 Vga16En:1; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x1D_STRUCT;

// **** D0F0x64_x22 Register Definition ****
// Address
#define D0F0x64_x22_ADDRESS                                     0x22
// Type
#define D0F0x64_x22_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x22_Reserved_25_0_OFFSET                        0
#define D0F0x64_x22_Reserved_25_0_WIDTH                         26
#define D0F0x64_x22_Reserved_25_0_MASK                          0x3FFFFFF
#define D0F0x64_x22_SoftOverrideClk4_OFFSET                     26
#define D0F0x64_x22_SoftOverrideClk4_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x64_x22_SoftOverrideClk3_OFFSET                     27
#define D0F0x64_x22_SoftOverrideClk3_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x64_x22_SoftOverrideClk2_OFFSET                     28
#define D0F0x64_x22_SoftOverrideClk2_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x64_x22_SoftOverrideClk1_OFFSET                     29
#define D0F0x64_x22_SoftOverrideClk1_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x64_x22_SoftOverrideClk0_OFFSET                     30
#define D0F0x64_x22_SoftOverrideClk0_WIDTH                      1
#define D0F0x64_x22_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x64_x22_Reserved_31_31_OFFSET                       31
#define D0F0x64_x22_Reserved_31_31_WIDTH                        1
#define D0F0x64_x22_Reserved_31_31_MASK                         0x80000000

/// D0F0x64_x22
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_25_0:26; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x22_STRUCT;

// **** D0F0x64_x23 Register Definition ****
// Address
#define D0F0x64_x23_ADDRESS                                     0x23
// Type
#define D0F0x64_x23_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x23_Reserved_25_0_OFFSET                        0
#define D0F0x64_x23_Reserved_25_0_WIDTH                         26
#define D0F0x64_x23_Reserved_25_0_MASK                          0x3FFFFFF
#define D0F0x64_x23_SoftOverrideClk4_OFFSET                     26
#define D0F0x64_x23_SoftOverrideClk4_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x64_x23_SoftOverrideClk3_OFFSET                     27
#define D0F0x64_x23_SoftOverrideClk3_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x64_x23_SoftOverrideClk2_OFFSET                     28
#define D0F0x64_x23_SoftOverrideClk2_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x64_x23_SoftOverrideClk1_OFFSET                     29
#define D0F0x64_x23_SoftOverrideClk1_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x64_x23_SoftOverrideClk0_OFFSET                     30
#define D0F0x64_x23_SoftOverrideClk0_WIDTH                      1
#define D0F0x64_x23_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x64_x23_Reserved_31_31_OFFSET                       31
#define D0F0x64_x23_Reserved_31_31_WIDTH                        1
#define D0F0x64_x23_Reserved_31_31_MASK                         0x80000000

/// D0F0x64_x23
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_25_0:26; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x23_STRUCT;

// **** D0F0x64_x30 Register Definition ****
// Address
#define D0F0x64_x30_ADDRESS                                     0x30
// Type
#define D0F0x64_x30_TYPE                                        TYPE_D0F0x64

// Field Data
#define D0F0x64_x30_DevFnMap_OFFSET                             0
#define D0F0x64_x30_DevFnMap_WIDTH                              8
#define D0F0x64_x30_DevFnMap_MASK                               0xFF
#define D0F0x64_x30_Reserved_31_8_OFFSET                        8
#define D0F0x64_x30_Reserved_31_8_WIDTH                         24
#define D0F0x64_x30_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F0x64_x30
typedef union {
  struct {                                                            ///<
    UINT32                                                DevFnMap:8; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F0x64_x30_STRUCT;

// **** D0F0x98_x07 Register Definition ****
// Address
#define D0F0x98_x07_ADDRESS                                     0x7
// Type
#define D0F0x98_x07_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x07_IocBwOptEn_OFFSET                           0
#define D0F0x98_x07_IocBwOptEn_WIDTH                            1
#define D0F0x98_x07_IocBwOptEn_MASK                             0x1
#define D0F0x98_x07_Reserved_3_1_OFFSET                         1
#define D0F0x98_x07_Reserved_3_1_WIDTH                          3
#define D0F0x98_x07_Reserved_3_1_MASK                           0xE
#define D0F0x98_x07_IommuBwOptEn_OFFSET                         4
#define D0F0x98_x07_IommuBwOptEn_WIDTH                          1
#define D0F0x98_x07_IommuBwOptEn_MASK                           0x10
#define D0F0x98_x07_Reserved_5_5_OFFSET                         5
#define D0F0x98_x07_Reserved_5_5_WIDTH                          1
#define D0F0x98_x07_Reserved_5_5_MASK                           0x20
#define D0F0x98_x07_DmaReqRespPassPWMode_OFFSET                 6
#define D0F0x98_x07_DmaReqRespPassPWMode_WIDTH                  1
#define D0F0x98_x07_DmaReqRespPassPWMode_MASK                   0x40
#define D0F0x98_x07_IommuIsocPassPWMode_OFFSET                  7
#define D0F0x98_x07_IommuIsocPassPWMode_WIDTH                   1
#define D0F0x98_x07_IommuIsocPassPWMode_MASK                    0x80
#define D0F0x98_x07_Reserved_13_8_OFFSET                        8
#define D0F0x98_x07_Reserved_13_8_WIDTH                         6
#define D0F0x98_x07_Reserved_13_8_MASK                          0x3F00
#define D0F0x98_x07_MSIHTIntConversionEn_OFFSET                 14
#define D0F0x98_x07_MSIHTIntConversionEn_WIDTH                  1
#define D0F0x98_x07_MSIHTIntConversionEn_MASK                   0x4000
#define D0F0x98_x07_DropZeroMaskWrEn_OFFSET                     15
#define D0F0x98_x07_DropZeroMaskWrEn_WIDTH                      1
#define D0F0x98_x07_DropZeroMaskWrEn_MASK                       0x8000
#define D0F0x98_x07_Reserved_16_16_OFFSET                       16
#define D0F0x98_x07_Reserved_16_16_WIDTH                        1
#define D0F0x98_x07_Reserved_16_16_MASK                         0x10000
#define D0F0x98_x07_Reserved_29_17_OFFSET                       17
#define D0F0x98_x07_Reserved_29_17_WIDTH                        13
#define D0F0x98_x07_Reserved_29_17_MASK                         0x3FFE0000
#define D0F0x98_x07_UnadjustThrottlingStpclk_OFFSET             30
#define D0F0x98_x07_UnadjustThrottlingStpclk_WIDTH              1
#define D0F0x98_x07_UnadjustThrottlingStpclk_MASK               0x40000000
#define D0F0x98_x07_SMUCsrIsocEn_OFFSET                         31
#define D0F0x98_x07_SMUCsrIsocEn_WIDTH                          1
#define D0F0x98_x07_SMUCsrIsocEn_MASK                           0x80000000

/// D0F0x98_x07
typedef union {
  struct {                                                            ///<
    UINT32                                              IocBwOptEn:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                            IommuBwOptEn:1; ///<
    UINT32                                            Reserved_5_5:1; ///<
    UINT32                                    DmaReqRespPassPWMode:1; ///<
    UINT32                                     IommuIsocPassPWMode:1; ///<
    UINT32                                           Reserved_13_8:6; ///<
    UINT32                                    MSIHTIntConversionEn:1; ///<
    UINT32                                        DropZeroMaskWrEn:1; ///<
    UINT32                                          Reserved_16_16:1; ///<
    UINT32                                          Reserved_29_17:13; ///<
    UINT32                                UnadjustThrottlingStpclk:1; ///<
    UINT32                                            SMUCsrIsocEn:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x07_STRUCT;

// **** D0F0x98_x08 Register Definition ****
// Address
#define D0F0x98_x08_ADDRESS                                     0x8
// Type
#define D0F0x98_x08_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x08_NpWrrLenA_OFFSET                            0
#define D0F0x98_x08_NpWrrLenA_WIDTH                             8
#define D0F0x98_x08_NpWrrLenA_MASK                              0xFF
#define D0F0x98_x08_NpWrrLenB_OFFSET                            8
#define D0F0x98_x08_NpWrrLenB_WIDTH                             8
#define D0F0x98_x08_NpWrrLenB_MASK                              0xFF00
#define D0F0x98_x08_NpWrrLenC_OFFSET                            16
#define D0F0x98_x08_NpWrrLenC_WIDTH                             8
#define D0F0x98_x08_NpWrrLenC_MASK                              0xFF0000
#define D0F0x98_x08_Reserved_31_24_OFFSET                       24
#define D0F0x98_x08_Reserved_31_24_WIDTH                        8
#define D0F0x98_x08_Reserved_31_24_MASK                         0xFF000000

/// D0F0x98_x08
typedef union {
  struct {                                                            ///<
    UINT32                                               NpWrrLenA:8; ///<
    UINT32                                               NpWrrLenB:8; ///<
    UINT32                                               NpWrrLenC:8; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x08_STRUCT;

// **** D0F0x98_x0C Register Definition ****
// Address
#define D0F0x98_x0C_ADDRESS                                     0xC
// Type
#define D0F0x98_x0C_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x0C_GcmWrrLenA_OFFSET                           0
#define D0F0x98_x0C_GcmWrrLenA_WIDTH                            8
#define D0F0x98_x0C_GcmWrrLenA_MASK                             0xFF
#define D0F0x98_x0C_GcmWrrLenB_OFFSET                           8
#define D0F0x98_x0C_GcmWrrLenB_WIDTH                            8
#define D0F0x98_x0C_GcmWrrLenB_MASK                             0xFF00
#define D0F0x98_x0C_Reserved_29_16_OFFSET                       16
#define D0F0x98_x0C_Reserved_29_16_WIDTH                        14
#define D0F0x98_x0C_Reserved_29_16_MASK                         0x3FFF0000
#define D0F0x98_x0C_StrictSelWinnerEn_OFFSET                    30
#define D0F0x98_x0C_StrictSelWinnerEn_WIDTH                     1
#define D0F0x98_x0C_StrictSelWinnerEn_MASK                      0x40000000
#define D0F0x98_x0C_Reserved_31_31_OFFSET                       31
#define D0F0x98_x0C_Reserved_31_31_WIDTH                        1
#define D0F0x98_x0C_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x0C
typedef union {
  struct {                                                            ///<
    UINT32                                              GcmWrrLenA:8; ///<
    UINT32                                              GcmWrrLenB:8; ///<
    UINT32                                          Reserved_29_16:14; ///<
    UINT32                                       StrictSelWinnerEn:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x0C_STRUCT;

// **** D0F0x98_x1E Register Definition ****
// Address
#define D0F0x98_x1E_ADDRESS                                     0x1E
// Type
#define D0F0x98_x1E_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x1E_Reserved_0_0_OFFSET                         0
#define D0F0x98_x1E_Reserved_0_0_WIDTH                          1
#define D0F0x98_x1E_Reserved_0_0_MASK                           0x1
#define D0F0x98_x1E_HiPriEn_OFFSET                              1
#define D0F0x98_x1E_HiPriEn_WIDTH                               1
#define D0F0x98_x1E_HiPriEn_MASK                                0x2
#define D0F0x98_x1E_Reserved_23_2_OFFSET                        2
#define D0F0x98_x1E_Reserved_23_2_WIDTH                         22
#define D0F0x98_x1E_Reserved_23_2_MASK                          0xFFFFFC
#define D0F0x98_x1E_Reserved_31_24_OFFSET                       24
#define D0F0x98_x1E_Reserved_31_24_WIDTH                        8
#define D0F0x98_x1E_Reserved_31_24_MASK                         0xFF000000

/// D0F0x98_x1E
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                 HiPriEn:1; ///<
    UINT32                                           Reserved_23_2:22; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x1E_STRUCT;

// **** D0F0x98_x28 Register Definition ****
// Address
#define D0F0x98_x28_ADDRESS                                     0x28
// Type
#define D0F0x98_x28_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x28_Reserved_0_0_OFFSET                         0
#define D0F0x98_x28_Reserved_0_0_WIDTH                          1
#define D0F0x98_x28_Reserved_0_0_MASK                           0x1
#define D0F0x98_x28_ForceCoherentIntr_OFFSET                    1
#define D0F0x98_x28_ForceCoherentIntr_WIDTH                     1
#define D0F0x98_x28_ForceCoherentIntr_MASK                      0x2
#define D0F0x98_x28_Reserved_31_2_OFFSET                        2
#define D0F0x98_x28_Reserved_31_2_WIDTH                         30
#define D0F0x98_x28_Reserved_31_2_MASK                          0xFFFFFFFC

/// D0F0x98_x28
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                       ForceCoherentIntr:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x28_STRUCT;

// **** D0F0x98_x2C Register Definition ****
// Address
#define D0F0x98_x2C_ADDRESS                                     0x2C
// Type
#define D0F0x98_x2C_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x2C_Reserved_0_0_OFFSET                         0
#define D0F0x98_x2C_Reserved_0_0_WIDTH                          1
#define D0F0x98_x2C_Reserved_0_0_MASK                           0x1
#define D0F0x98_x2C_DynWakeEn_OFFSET                            1
#define D0F0x98_x2C_DynWakeEn_WIDTH                             1
#define D0F0x98_x2C_DynWakeEn_MASK                              0x2
#define D0F0x98_x2C_Reserved_7_2_OFFSET                         2
#define D0F0x98_x2C_Reserved_7_2_WIDTH                          6
#define D0F0x98_x2C_Reserved_7_2_MASK                           0xFC
#define D0F0x98_x2C_OrbRxIdlesMask_OFFSET                       8
#define D0F0x98_x2C_OrbRxIdlesMask_WIDTH                        1
#define D0F0x98_x2C_OrbRxIdlesMask_MASK                         0x100
#define D0F0x98_x2C_SBDmaActiveMask_OFFSET                      9
#define D0F0x98_x2C_SBDmaActiveMask_WIDTH                       1
#define D0F0x98_x2C_SBDmaActiveMask_MASK                        0x200
#define D0F0x98_x2C_NBOutbWakeMask_OFFSET                       10
#define D0F0x98_x2C_NBOutbWakeMask_WIDTH                        1
#define D0F0x98_x2C_NBOutbWakeMask_MASK                         0x400
#define D0F0x98_x2C_Reserved_15_11_OFFSET                       11
#define D0F0x98_x2C_Reserved_15_11_WIDTH                        5
#define D0F0x98_x2C_Reserved_15_11_MASK                         0xF800
#define D0F0x98_x2C_WakeHysteresis_OFFSET                       16
#define D0F0x98_x2C_WakeHysteresis_WIDTH                        16
#define D0F0x98_x2C_WakeHysteresis_MASK                         0xFFFF0000

/// D0F0x98_x2C
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                               DynWakeEn:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                          OrbRxIdlesMask:1; ///<
    UINT32                                         SBDmaActiveMask:1; ///<
    UINT32                                          NBOutbWakeMask:1; ///<
    UINT32                                          Reserved_15_11:5; ///<
    UINT32                                          WakeHysteresis:16; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x2C_STRUCT;

// **** D0F0x98_x3A Register Definition ****
// Address
#define D0F0x98_x3A_ADDRESS                                     0x3a

// Type
#define D0F0x98_x3A_TYPE                                        TYPE_D0F0x98
// Field Data
#define D0F0x98_x3A_Reserved_3_0_OFFSET                         0
#define D0F0x98_x3A_Reserved_3_0_WIDTH                          4
#define D0F0x98_x3A_Reserved_3_0_MASK                           0xf
#define D0F0x98_x3A_PGDClumpingEn_OFFSET                        4
#define D0F0x98_x3A_PGDClumpingEn_WIDTH                         4
#define D0F0x98_x3A_PGDClumpingEn_MASK                          0xf0
#define D0F0x98_x3A_PPDClumpingEn_OFFSET                        8
#define D0F0x98_x3A_PPDClumpingEn_WIDTH                         5
#define D0F0x98_x3A_PPDClumpingEn_MASK                          0x1f00
#define D0F0x98_x3A_Reserved_14_13_OFFSET                       13
#define D0F0x98_x3A_Reserved_14_13_WIDTH                        2
#define D0F0x98_x3A_Reserved_14_13_MASK                         0x6000
#define D0F0x98_x3A_PSDClumpingEn_OFFSET                        15
#define D0F0x98_x3A_PSDClumpingEn_WIDTH                         5
#define D0F0x98_x3A_PSDClumpingEn_MASK                          0xf8000
#define D0F0x98_x3A_GBIFClumpingEn_OFFSET                       20
#define D0F0x98_x3A_GBIFClumpingEn_WIDTH                        4
#define D0F0x98_x3A_GBIFClumpingEn_MASK                         0xf00000
#define D0F0x98_x3A_Reserved_31_24_OFFSET                       24
#define D0F0x98_x3A_Reserved_31_24_WIDTH                        8
#define D0F0x98_x3A_Reserved_31_24_MASK                         0xff000000

/// D0F0x98_x3A
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_3_0:4 ; ///<
    UINT32                                            PGDClumpingEn:4 ; ///<
    UINT32                                            PPDClumpingEn:5 ; ///<
    UINT32                                           Reserved_14_13:2 ; ///<
    UINT32                                            PSDClumpingEn:5 ; ///<
    UINT32                                           GBIFClumpingEn:4 ; ///<
    UINT32                                           Reserved_31_24:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0x98_x3A_STRUCT;

// **** D0F0x98_x49 Register Definition ****
// Address
#define D0F0x98_x49_ADDRESS                                     0x49
// Type
#define D0F0x98_x49_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x49_Reserved_23_0_OFFSET                        0
#define D0F0x98_x49_Reserved_23_0_WIDTH                         24
#define D0F0x98_x49_Reserved_23_0_MASK                          0xFFFFFF
#define D0F0x98_x49_SoftOverrideClk6_OFFSET                     24
#define D0F0x98_x49_SoftOverrideClk6_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk6_MASK                       0x1000000
#define D0F0x98_x49_SoftOverrideClk5_OFFSET                     25
#define D0F0x98_x49_SoftOverrideClk5_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk5_MASK                       0x2000000
#define D0F0x98_x49_SoftOverrideClk4_OFFSET                     26
#define D0F0x98_x49_SoftOverrideClk4_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x98_x49_SoftOverrideClk3_OFFSET                     27
#define D0F0x98_x49_SoftOverrideClk3_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x98_x49_SoftOverrideClk2_OFFSET                     28
#define D0F0x98_x49_SoftOverrideClk2_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x98_x49_SoftOverrideClk1_OFFSET                     29
#define D0F0x98_x49_SoftOverrideClk1_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x98_x49_SoftOverrideClk0_OFFSET                     30
#define D0F0x98_x49_SoftOverrideClk0_WIDTH                      1
#define D0F0x98_x49_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x98_x49_Reserved_31_31_OFFSET                       31
#define D0F0x98_x49_Reserved_31_31_WIDTH                        1
#define D0F0x98_x49_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x49
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                        SoftOverrideClk6:1; ///<
    UINT32                                        SoftOverrideClk5:1; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x49_STRUCT;

// **** D0F0x98_x4A Register Definition ****
// Address
#define D0F0x98_x4A_ADDRESS                                     0x4A
// Type
#define D0F0x98_x4A_TYPE                                        TYPE_D0F0x98

// Field Data
#define D0F0x98_x4A_Reserved_23_0_OFFSET                        0
#define D0F0x98_x4A_Reserved_23_0_WIDTH                         24
#define D0F0x98_x4A_Reserved_23_0_MASK                          0xFFFFFF
#define D0F0x98_x4A_SoftOverrideClk6_OFFSET                     24
#define D0F0x98_x4A_SoftOverrideClk6_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk6_MASK                       0x1000000
#define D0F0x98_x4A_SoftOverrideClk5_OFFSET                     25
#define D0F0x98_x4A_SoftOverrideClk5_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk5_MASK                       0x2000000
#define D0F0x98_x4A_SoftOverrideClk4_OFFSET                     26
#define D0F0x98_x4A_SoftOverrideClk4_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk4_MASK                       0x4000000
#define D0F0x98_x4A_SoftOverrideClk3_OFFSET                     27
#define D0F0x98_x4A_SoftOverrideClk3_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk3_MASK                       0x8000000
#define D0F0x98_x4A_SoftOverrideClk2_OFFSET                     28
#define D0F0x98_x4A_SoftOverrideClk2_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk2_MASK                       0x10000000
#define D0F0x98_x4A_SoftOverrideClk1_OFFSET                     29
#define D0F0x98_x4A_SoftOverrideClk1_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk1_MASK                       0x20000000
#define D0F0x98_x4A_SoftOverrideClk0_OFFSET                     30
#define D0F0x98_x4A_SoftOverrideClk0_WIDTH                      1
#define D0F0x98_x4A_SoftOverrideClk0_MASK                       0x40000000
#define D0F0x98_x4A_Reserved_31_31_OFFSET                       31
#define D0F0x98_x4A_Reserved_31_31_WIDTH                        1
#define D0F0x98_x4A_Reserved_31_31_MASK                         0x80000000

/// D0F0x98_x4A
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                        SoftOverrideClk6:1; ///<
    UINT32                                        SoftOverrideClk5:1; ///<
    UINT32                                        SoftOverrideClk4:1; ///<
    UINT32                                        SoftOverrideClk3:1; ///<
    UINT32                                        SoftOverrideClk2:1; ///<
    UINT32                                        SoftOverrideClk1:1; ///<
    UINT32                                        SoftOverrideClk0:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0x98_x4A_STRUCT;

// **** D0F0xBC_x20054 Register Definition ****
// Address
#define D0F0xBC_x20054_ADDRESS                                  0x20054

// Type
#define D0F0xBC_x20054_TYPE                                     TYPE_D0F0xBC
// Field Data
#define D0F0xBC_x20054_BiosTableAddr_OFFSET                    0
#define D0F0xBC_x20054_BiosTableAddr_WIDTH                     32
#define D0F0xBC_x20054_BiosTableAddr_MASK                      0xFFFFFFFF

/// D0F0xBC_x3F9F4
typedef union {
  struct {                                                              ///<
    UINT32                                           BiosTableAddr:32 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_x20054_STRUCT;

// **** D0F0xBC_xC0200090 Register Definition ****
// Address
#define D0F0xBC_xC0200090_ADDRESS                               0xc0200090

// Type
#define D0F0xBC_xC0200090_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200090_SMU_VOLTAGE_EN_OFFSET                 0
#define D0F0xBC_xC0200090_SMU_VOLTAGE_EN_WIDTH                  1
#define D0F0xBC_xC0200090_SMU_VOLTAGE_EN_MASK                   0x1
#define D0F0xBC_xC0200090_SMU_VOLTAGE_LVL_OFFSET                1
#define D0F0xBC_xC0200090_SMU_VOLTAGE_LVL_WIDTH                 8
#define D0F0xBC_xC0200090_SMU_VOLTAGE_LVL_MASK                  0x1FE

/// D0F0xBC_xC0200090
typedef union {
  struct {                                                              ///<
    UINT32                                             SmuVoltageEn:1 ; ///<
    UINT32                                            SmuVoltageLvl:8 ; ///<
    UINT32                                           Reserved_31_9:23 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200090_STRUCT;

// **** D0F0xBC_xC0200094 Register Definition ****
// Address
#define D0F0xBC_xC0200094_ADDRESS                               0xc0200094

// Type
#define D0F0xBC_xC0200094_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200094_SMU_VOLTAGE_STS_OFFSET                0
#define D0F0xBC_xC0200094_SMU_VOLTAGE_STS_WIDTH                 1
#define D0F0xBC_xC0200094_SMU_VOLTAGE_STS_MASK                  0x1
#define D0F0xBC_xC0200094_SMU_VOLTAGE_LVL_OFFSET                1
#define D0F0xBC_xC0200094_SMU_VOLTAGE_LVL_WIDTH                 8
#define D0F0xBC_xC0200094_SMU_VOLTAGE_LVL_MASK                  0x1FE

/// D0F0xBC_xC0200094
typedef union {
  struct {                                                              ///<
    UINT32                                            SmuVoltageSts:1 ; ///<
    UINT32                                            SmuVoltageLvl:8 ; ///<
    UINT32                                           Reserved_31_9:23 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200094_STRUCT;

// **** D0F0xBC_xC050008C Register Definition ****
// Address
#define D0F0xBC_xC050008C_ADDRESS                               0xC050008C

// Type
#define D0F0xBC_xC050008C_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC050008C_SCLK_DIVIDER_OFFSET                   0
#define D0F0xBC_xC050008C_SCLK_DIVIDER_WIDTH                    7
#define D0F0xBC_xC050008C_SCLK_DIVIDER_MASK                     0x7F

/// D0F0xBC_xC050008C
typedef union {
  struct {                                                              ///<
    UINT32                                             SclkDivider:7  ; ///<
    UINT32                                           Reserved_31_7:25 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC050008C_STRUCT;

// **** D0F0xBC_xC0500090 Register Definition ****
// Address
#define D0F0xBC_xC0500090_ADDRESS                               0xC0500090

// Type
#define D0F0xBC_xC0500090_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0500090_SCLK_STATUS_OFFSET                    0
#define D0F0xBC_xC0500090_SCLK_STATUS_WIDTH                     1
#define D0F0xBC_xC0500090_SCLK_STATUS_MASK                      0x1

/// D0F0xBC_xC05000D0
typedef union {
  struct {                                                              ///<
    UINT32                                              SclkStatus:1  ; ///<
    UINT32                                           Reserved_31_2:31 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0500090_STRUCT;

// **** D0F0xBC_xC05000CC Register Definition ****
// Address
#define D0F0xBC_xC05000CC_ADDRESS                               0xC05000CC

// Type
#define D0F0xBC_xC05000CC_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC05000CC_SCLK_DIVIDER_OFFSET                   0
#define D0F0xBC_xC05000CC_SCLK_DIVIDER_WIDTH                    7
#define D0F0xBC_xC05000CC_SCLK_DIVIDER_MASK                     0x7F

/// D0F0xBC_xC05000CC
typedef union {
  struct {                                                              ///<
    UINT32                                             SclkDivider:7  ; ///<
    UINT32                                           Reserved_31_7:25 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC05000CC_STRUCT;

// **** D0F0xBC_xC05000D0 Register Definition ****
// Address
#define D0F0xBC_xC05000D0_ADDRESS                               0xC05000D0

// Type
#define D0F0xBC_xC05000D0_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC05000D0_SCLK_STATUS_OFFSET                    0
#define D0F0xBC_xC05000D0_SCLK_STATUS_WIDTH                     1
#define D0F0xBC_xC05000D0_SCLK_STATUS_MASK                      0x1

/// D0F0xBC_xC05000D0
typedef union {
  struct {                                                              ///<
    UINT32                                              SclkStatus:1  ; ///<
    UINT32                                           Reserved_31_2:31 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC05000D0_STRUCT;

// **** D0F0xBC_xC0200310 Register Definition ****
// Address
#define D0F0xBC_xC0200310_ADDRESS                               0xc0200310

// Type
#define D0F0xBC_xC0200310_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_OFFSET                  0
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_RFE_BUSY_MASK_MASK                    0x1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_OFFSET          1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_WIDTH           1
#define D0F0xBC_xC0200310_BIF_CG_LCLK_BUSY_MASK_MASK            0x2
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_OFFSET            2
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_WIDTH             1
#define D0F0xBC_xC0200310_L1IMU_SMU_IDLE_MASK_MASK              0x4
#define D0F0xBC_xC0200310_RESERVED_BIT3_OFFSET                  3
#define D0F0xBC_xC0200310_RESERVED_BIT3_WIDTH                   1
#define D0F0xBC_xC0200310_RESERVED_BIT3_MASK                    0x8
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_OFFSET              4
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_WIDTH               1
#define D0F0xBC_xC0200310_SCLK_RUNNING_MASK_MASK                0x10
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_OFFSET                  5
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_SMU_BUSY_MASK_MASK                    0x20
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_OFFSET           6
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE1_MASK_MASK             0x40
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_OFFSET           7
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE2_MASK_MASK             0x80
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_OFFSET           8
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE3_MASK_MASK             0x100
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_OFFSET           9
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_WIDTH            1
#define D0F0xBC_xC0200310_PCIE_LCLK_IDLE4_MASK_MASK             0x200
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_OFFSET             10
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_WIDTH              1
#define D0F0xBC_xC0200310_L1IMUGPP_IDLE_MASK_MASK               0x400
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_OFFSET           11
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_WIDTH            1
#define D0F0xBC_xC0200310_L1IMUGPPSB_IDLE_MASK_MASK             0x800
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_OFFSET             12
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_WIDTH              1
#define D0F0xBC_xC0200310_L1IMUBIF_IDLE_MASK_MASK               0x1000
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_OFFSET          13
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_WIDTH           1
#define D0F0xBC_xC0200310_L1IMUINTGEN_IDLE_MASK_MASK            0x2000
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_OFFSET                14
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_WIDTH                 1
#define D0F0xBC_xC0200310_L2IMU_IDLE_MASK_MASK                  0x4000
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_OFFSET                  15
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_WIDTH                   1
#define D0F0xBC_xC0200310_ORB_IDLE_MASK_MASK                    0x8000
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_OFFSET               16
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_WIDTH                1
#define D0F0xBC_xC0200310_ON_INB_WAKE_MASK_MASK                 0x10000
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_OFFSET           17
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_WIDTH            1
#define D0F0xBC_xC0200310_ON_INB_WAKE_ACK_MASK_MASK             0x20000
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_OFFSET              18
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_WIDTH               1
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_MASK_MASK                0x40000
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_OFFSET          19
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_WIDTH           1
#define D0F0xBC_xC0200310_ON_OUTB_WAKE_ACK_MASK_MASK            0x80000
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_OFFSET                 20
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_WIDTH                  1
#define D0F0xBC_xC0200310_DMAACTIVE_MASK_MASK                   0x100000
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_OFFSET        21
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_WIDTH         1
#define D0F0xBC_xC0200310_RLC_SMU_GFXCLK_OFF_MASK_MASK          0x200000
#define D0F0xBC_xC0200310_RESERVED_OFFSET                       22
#define D0F0xBC_xC0200310_RESERVED_WIDTH                        10
#define D0F0xBC_xC0200310_RESERVED_MASK                         0xffc00000

/// D0F0xBC_xC0200310
typedef union {
  struct {                                                              ///<
    UINT32                                            RFE_BUSY_MASK:1 ; ///<
    UINT32                                    BIF_CG_LCLK_BUSY_MASK:1 ; ///<
    UINT32                                      L1IMU_SMU_IDLE_MASK:1 ; ///<
    UINT32                                            RESERVED_BIT3:1 ; ///<
    UINT32                                        SCLK_RUNNING_MASK:1 ; ///<
    UINT32                                            SMU_BUSY_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE1_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE2_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE3_MASK:1 ; ///<
    UINT32                                     PCIE_LCLK_IDLE4_MASK:1 ; ///<
    UINT32                                       L1IMUGPP_IDLE_MASK:1 ; ///<
    UINT32                                     L1IMUGPPSB_IDLE_MASK:1 ; ///<
    UINT32                                       L1IMUBIF_IDLE_MASK:1 ; ///<
    UINT32                                    L1IMUINTGEN_IDLE_MASK:1 ; ///<
    UINT32                                          L2IMU_IDLE_MASK:1 ; ///<
    UINT32                                            ORB_IDLE_MASK:1 ; ///<
    UINT32                                         ON_INB_WAKE_MASK:1 ; ///<
    UINT32                                     ON_INB_WAKE_ACK_MASK:1 ; ///<
    UINT32                                        ON_OUTB_WAKE_MASK:1 ; ///<
    UINT32                                    ON_OUTB_WAKE_ACK_MASK:1 ; ///<
    UINT32                                           DMAACTIVE_MASK:1 ; ///<
    UINT32                                  RLC_SMU_GFXCLK_OFF_MASK:1 ; ///<
    UINT32                                                 RESERVED:10; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0200310_STRUCT;

// **** D0F0xD4_x010905D7 Register Definition ****
// Address
#define D0F0xD4_x010905D7_ADDRESS                               0x10905D7
// Type
#define D0F0xD4_x010905D7_TYPE                                  TYPE_D0F0xD4

// Field Data

/// D0F0xD4_x010905D7
typedef struct {
  UINT32 Value;
} D0F0xD4_x010905D7_STRUCT;

// **** D0F0xD4_x010905D8 Register Definition ****
// Address
#define D0F0xD4_x010905D8_ADDRESS                               0x10905D8
// Type
#define D0F0xD4_x010905D8_TYPE                                  TYPE_D0F0xD4

// Field Data

/// D0F0xD4_x010905D8
typedef struct {
  UINT32 Value;
} D0F0xD4_x010905D8_STRUCT;

// **** D0F0xD4_x010914C3 Register Definition ****
// Address
#define D0F0xD4_x010914C3_ADDRESS                               0x010914C3
// Type
#define D0F0xD4_x010914C3_TYPE                                  TYPE_D0F0xD4

// **** D0F0xD4_x010914E1 Register Definition ****
// Address
#define D0F0xD4_x010914E1_ADDRESS                               0x10914E1
// Type
#define D0F0xD4_x010914E1_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x010914E1_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x010914E1_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x010914E1_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x010914E1_StrapBifRegApSize_OFFSET              1
#define D0F0xD4_x010914E1_StrapBifRegApSize_WIDTH               2
#define D0F0xD4_x010914E1_StrapBifRegApSize_MASK                0x6
#define D0F0xD4_x010914E1_StrapBifMemApSize_OFFSET              3
#define D0F0xD4_x010914E1_StrapBifMemApSize_WIDTH               3
#define D0F0xD4_x010914E1_StrapBifMemApSize_MASK                0x38
#define D0F0xD4_x010914E1_StrapBifRomApSize_OFFSET              6
#define D0F0xD4_x010914E1_StrapBifRomApSize_WIDTH               2
#define D0F0xD4_x010914E1_StrapBifRomApSize_MASK                0xC0
#define D0F0xD4_x010914E1_StrapBifFbAlwaysOn_OFFSET             8
#define D0F0xD4_x010914E1_StrapBifFbAlwaysOn_WIDTH              1
#define D0F0xD4_x010914E1_StrapBifFbAlwaysOn_MASK               0x100
#define D0F0xD4_x010914E1_StrapBifFbCplTypeSel_OFFSET           9
#define D0F0xD4_x010914E1_StrapBifFbCplTypeSel_WIDTH            2
#define D0F0xD4_x010914E1_StrapBifFbCplTypeSel_MASK             0x600
#define D0F0xD4_x010914E1_StrapBifDebugAccess_OFFSET            11
#define D0F0xD4_x010914E1_StrapBifDebugAccess_WIDTH             1
#define D0F0xD4_x010914E1_StrapBifDebugAccess_MASK              0x800
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_OFFSET         12
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_WIDTH          1
#define D0F0xD4_x010914E1_StrapBifDoorbellBarDis_MASK           0x1000
#define D0F0xD4_x010914E1_StrapBifBarComplianceEn_OFFSET        13
#define D0F0xD4_x010914E1_StrapBifBarComplianceEn_WIDTH         1
#define D0F0xD4_x010914E1_StrapBifBarComplianceEn_MASK          0x2000
#define D0F0xD4_x010914E1_StrapBifBarDoorbellAperSize_OFFSET    14
#define D0F0xD4_x010914E1_StrapBifBarDoorbellAperSize_WIDTH     2
#define D0F0xD4_x010914E1_StrapBifBarDoorbellAperSize_MASK      0xC000
#define D0F0xD4_x010914E1_StrapReserved_OFFSET                  16
#define D0F0xD4_x010914E1_StrapReserved_WIDTH                   11
#define D0F0xD4_x010914E1_StrapReserved_MASK                    0x7FF0000
#define D0F0xD4_x010914E1_Reserved_31_27_OFFSET                 27
#define D0F0xD4_x010914E1_Reserved_31_27_WIDTH                  5
#define D0F0xD4_x010914E1_Reserved_31_27_MASK                   0xF8000000

/// D0F0xD4_x010914E1
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                       StrapBifRegApSize:2; ///<
    UINT32                                       StrapBifMemApSize:3; ///<
    UINT32                                       StrapBifRomApSize:2; ///<
    UINT32                                      StrapBifFbAlwaysOn:1; ///<
    UINT32                                    StrapBifFbCplTypeSel:2; ///<
    UINT32                                     StrapBifDebugAccess:1; ///<
    UINT32                                  StrapBifDoorbellBarDis:1; ///<
    UINT32                                 StrapBifBarComplianceEn:1; ///<
    UINT32                             StrapBifBarDoorbellAperSize:2; ///<
    UINT32                                           StrapReserved:11; ///<
    UINT32                                          Reserved_31_27:5; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x010914E1_STRUCT;

// **** D0F0xD4_x010914E2 Register Definition ****
// Address
#define D0F0xD4_x010914E2_ADDRESS                               0x10914E2
// Type
#define D0F0xD4_x010914E2_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x010914E2_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x010914E2_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x010914E2_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_OFFSET               1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_WIDTH                1
#define D0F0xD4_x010914E2_StrapBifIoBarDis_MASK                 0x2
#define D0F0xD4_x010914E2_StrapBifVgaDis_OFFSET                 2
#define D0F0xD4_x010914E2_StrapBifVgaDis_WIDTH                  1
#define D0F0xD4_x010914E2_StrapBifVgaDis_MASK                   0x4
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_OFFSET            3
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_WIDTH             1
#define D0F0xD4_x010914E2_StrapBifF064BarDisA_MASK              0x8
#define D0F0xD4_x010914E2_StrapCec64BarDis_OFFSET               4
#define D0F0xD4_x010914E2_StrapCec64BarDis_WIDTH                1
#define D0F0xD4_x010914E2_StrapCec64BarDis_MASK                 0x10
#define D0F0xD4_x010914E2_StrapBifAz64BarDisA_OFFSET            5
#define D0F0xD4_x010914E2_StrapBifAz64BarDisA_WIDTH             1
#define D0F0xD4_x010914E2_StrapBifAz64BarDisA_MASK              0x20
#define D0F0xD4_x010914E2_StrapBifF0BarEn_OFFSET                6
#define D0F0xD4_x010914E2_StrapBifF0BarEn_WIDTH                 1
#define D0F0xD4_x010914E2_StrapBifF0BarEn_MASK                  0x40
#define D0F0xD4_x010914E2_StrapBifRxIgnoreMsgErr_OFFSET         7
#define D0F0xD4_x010914E2_StrapBifRxIgnoreMsgErr_WIDTH          1
#define D0F0xD4_x010914E2_StrapBifRxIgnoreMsgErr_MASK           0x80
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_OFFSET  8
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x010914E2_StrapBifF0LegacyDeviceTypeDis_MASK    0x100
#define D0F0xD4_x010914E2_StrapBifF2LegacyDeviceTypeDis_OFFSET  9
#define D0F0xD4_x010914E2_StrapBifF2LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x010914E2_StrapBifF2LegacyDeviceTypeDis_MASK    0x200
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_OFFSET  10
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x010914E2_StrapBifAzLegacyDeviceTypeDis_MASK    0x400
#define D0F0xD4_x010914E2_StrapBifRxIgnoreEpErr_OFFSET          11
#define D0F0xD4_x010914E2_StrapBifRxIgnoreEpErr_WIDTH           1
#define D0F0xD4_x010914E2_StrapBifRxIgnoreEpErr_MASK            0x800
#define D0F0xD4_x010914E2_StrapCecPmeSupportComplianceDis_OFFSET 12
#define D0F0xD4_x010914E2_StrapCecPmeSupportComplianceDis_WIDTH 1
#define D0F0xD4_x010914E2_StrapCecPmeSupportComplianceDis_MASK  0x1000
#define D0F0xD4_x010914E2_StrapBifAudioEn_OFFSET                13
#define D0F0xD4_x010914E2_StrapBifAudioEn_WIDTH                 1
#define D0F0xD4_x010914E2_StrapBifAudioEn_MASK                  0x2000
#define D0F0xD4_x010914E2_StrapReserved_OFFSET                  14
#define D0F0xD4_x010914E2_StrapReserved_WIDTH                   8
#define D0F0xD4_x010914E2_StrapReserved_MASK                    0x3FC000
#define D0F0xD4_x010914E2_Reserved_31_22_OFFSET                 22
#define D0F0xD4_x010914E2_Reserved_31_22_WIDTH                  10
#define D0F0xD4_x010914E2_Reserved_31_22_MASK                   0xFFC00000

/// D0F0xD4_x010914E2
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                        StrapBifIoBarDis:1; ///<
    UINT32                                          StrapBifVgaDis:1; ///<
    UINT32                                     StrapBifF064BarDisA:1; ///<
    UINT32                                        StrapCec64BarDis:1; ///<
    UINT32                                     StrapBifAz64BarDisA:1; ///<
    UINT32                                         StrapBifF0BarEn:1; ///<
    UINT32                                  StrapBifRxIgnoreMsgErr:1; ///<
    UINT32                           StrapBifF0LegacyDeviceTypeDis:1; ///<
    UINT32                           StrapBifF2LegacyDeviceTypeDis:1; ///<
    UINT32                           StrapBifAzLegacyDeviceTypeDis:1; ///<
    UINT32                                   StrapBifRxIgnoreEpErr:1; ///<
    UINT32                         StrapCecPmeSupportComplianceDis:1; ///<
    UINT32                                         StrapBifAudioEn:1; ///<
    UINT32                                           StrapReserved:8; ///<
    UINT32                                          Reserved_31_22:10; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x010914E2_STRUCT;

// **** D0F0xD4_x01091507 Register Definition ****
// Address
#define D0F0xD4_x01091507_ADDRESS                               0x1091507
// Type
#define D0F0xD4_x01091507_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01091507_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x01091507_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x01091507_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x01091507_StrapBifGen3EnA_OFFSET                1
#define D0F0xD4_x01091507_StrapBifGen3EnA_WIDTH                 1
#define D0F0xD4_x01091507_StrapBifGen3EnA_MASK                  0x2
#define D0F0xD4_x01091507_StrapBifClkPmEn_OFFSET                2
#define D0F0xD4_x01091507_StrapBifClkPmEn_WIDTH                 1
#define D0F0xD4_x01091507_StrapBifClkPmEn_MASK                  0x4
#define D0F0xD4_x01091507_StrapBifBiosRomEn_OFFSET              3
#define D0F0xD4_x01091507_StrapBifBiosRomEn_WIDTH               1
#define D0F0xD4_x01091507_StrapBifBiosRomEn_MASK                0x8
#define D0F0xD4_x01091507_StrapBifSmbusDis_OFFSET               4
#define D0F0xD4_x01091507_StrapBifSmbusDis_WIDTH                1
#define D0F0xD4_x01091507_StrapBifSmbusDis_MASK                 0x10
#define D0F0xD4_x01091507_StrapBifMemApSizePin_OFFSET           5
#define D0F0xD4_x01091507_StrapBifMemApSizePin_WIDTH            3
#define D0F0xD4_x01091507_StrapBifMemApSizePin_MASK             0xE0
#define D0F0xD4_x01091507_Reserved_8_8_OFFSET                   8
#define D0F0xD4_x01091507_Reserved_8_8_WIDTH                    1
#define D0F0xD4_x01091507_Reserved_8_8_MASK                     0x100
#define D0F0xD4_x01091507_StrapBifVgaDisPin_OFFSET              9
#define D0F0xD4_x01091507_StrapBifVgaDisPin_WIDTH               1
#define D0F0xD4_x01091507_StrapBifVgaDisPin_MASK                0x200
#define D0F0xD4_x01091507_Reserved_10_10_OFFSET                 10
#define D0F0xD4_x01091507_Reserved_10_10_WIDTH                  1
#define D0F0xD4_x01091507_Reserved_10_10_MASK                   0x400
#define D0F0xD4_x01091507_StrapTxCfgDrvFullSwing_OFFSET         11
#define D0F0xD4_x01091507_StrapTxCfgDrvFullSwing_WIDTH          1
#define D0F0xD4_x01091507_StrapTxCfgDrvFullSwing_MASK           0x800
#define D0F0xD4_x01091507_Reserved_15_12_OFFSET                 12
#define D0F0xD4_x01091507_Reserved_15_12_WIDTH                  4
#define D0F0xD4_x01091507_Reserved_15_12_MASK                   0xF000
#define D0F0xD4_x01091507_StrapBifAudioEnPin_OFFSET             16
#define D0F0xD4_x01091507_StrapBifAudioEnPin_WIDTH              1
#define D0F0xD4_x01091507_StrapBifAudioEnPin_MASK               0x10000
#define D0F0xD4_x01091507_StrapBifCecEnPin_OFFSET               17
#define D0F0xD4_x01091507_StrapBifCecEnPin_WIDTH                1
#define D0F0xD4_x01091507_StrapBifCecEnPin_MASK                 0x20000
#define D0F0xD4_x01091507_Reserved_31_18_OFFSET                 18
#define D0F0xD4_x01091507_Reserved_31_18_WIDTH                  14
#define D0F0xD4_x01091507_Reserved_31_18_MASK                   0xFFFC0000

/// D0F0xD4_x01091507
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                         StrapBifGen3EnA:1; ///<
    UINT32                                         StrapBifClkPmEn:1; ///<
    UINT32                                       StrapBifBiosRomEn:1; ///<
    UINT32                                        StrapBifSmbusDis:1; ///<
    UINT32                                    StrapBifMemApSizePin:3; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                       StrapBifVgaDisPin:1; ///<
    UINT32                                          Reserved_10_10:1; ///<
    UINT32                                  StrapTxCfgDrvFullSwing:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                      StrapBifAudioEnPin:1; ///<
    UINT32                                        StrapBifCecEnPin:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01091507_STRUCT;

// **** D0F0xD4_x01091518 Register Definition ****
// Address
#define D0F0xD4_x01091518_ADDRESS                               0x1091518
// Type
#define D0F0xD4_x01091518_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01091518_LinkTrainEn_OFFSET                    0
#define D0F0xD4_x01091518_LinkTrainEn_WIDTH                     1
#define D0F0xD4_x01091518_LinkTrainEn_MASK                      0x1
#define D0F0xD4_x01091518_Reserved_31_1_OFFSET                  1
#define D0F0xD4_x01091518_Reserved_31_1_WIDTH                   31
#define D0F0xD4_x01091518_Reserved_31_1_MASK                    0xFFFFFFFE

/// D0F0xD4_x01091518
typedef union {
  struct {                                                            ///<
    UINT32                                             LinkTrainEn:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01091518_STRUCT;

// **** D0F0xD4_x01301486 Register Definition ****
// Address
#define D0F0xD4_x01301486_ADDRESS                               0x1301486
// Type
#define D0F0xD4_x01301486_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x01301486_StrapEn_OFFSET                        0
#define D0F0xD4_x01301486_StrapEn_WIDTH                         1
#define D0F0xD4_x01301486_StrapEn_MASK                          0x1
#define D0F0xD4_x01301486_RstDone_OFFSET                        1
#define D0F0xD4_x01301486_RstDone_WIDTH                         1
#define D0F0xD4_x01301486_RstDone_MASK                          0x2
#define D0F0xD4_x01301486_LinkTrainEn_OFFSET                    2
#define D0F0xD4_x01301486_LinkTrainEn_WIDTH                     1
#define D0F0xD4_x01301486_LinkTrainEn_MASK                      0x4
#define D0F0xD4_x01301486_StrapAllValid_OFFSET                  3
#define D0F0xD4_x01301486_StrapAllValid_WIDTH                   1
#define D0F0xD4_x01301486_StrapAllValid_MASK                    0x8
#define D0F0xD4_x01301486_Reserved_7_4_OFFSET                   4
#define D0F0xD4_x01301486_Reserved_7_4_WIDTH                    4
#define D0F0xD4_x01301486_Reserved_7_4_MASK                     0xF0
#define D0F0xD4_x01301486_RecapStrapWarmrst_OFFSET              8
#define D0F0xD4_x01301486_RecapStrapWarmrst_WIDTH               1
#define D0F0xD4_x01301486_RecapStrapWarmrst_MASK                0x100
#define D0F0xD4_x01301486_Reserved_31_9_OFFSET                  9
#define D0F0xD4_x01301486_Reserved_31_9_WIDTH                   23
#define D0F0xD4_x01301486_Reserved_31_9_MASK                    0xFFFFFE00

/// D0F0xD4_x01301486
typedef union {
  struct {                                                            ///<
    UINT32                                                 StrapEn:1; ///<
    UINT32                                                 RstDone:1; ///<
    UINT32                                             LinkTrainEn:1; ///<
    UINT32                                           StrapAllValid:1; ///<
    UINT32                                            Reserved_7_4:4; ///<
    UINT32                                       RecapStrapWarmrst:1; ///<
    UINT32                                           Reserved_31_9:23; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x01301486_STRUCT;

// **** D0F0xD4_x013014AC Register Definition ****
// Address
#define D0F0xD4_x013014AC_ADDRESS                               0x13014AC
// Type
#define D0F0xD4_x013014AC_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014AC_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x013014AC_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x013014AC_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x013014AC_StrapBifGen3EnA_OFFSET                1
#define D0F0xD4_x013014AC_StrapBifGen3EnA_WIDTH                 1
#define D0F0xD4_x013014AC_StrapBifGen3EnA_MASK                  0x2
#define D0F0xD4_x013014AC_StrapBifClkPmEn_OFFSET                2
#define D0F0xD4_x013014AC_StrapBifClkPmEn_WIDTH                 1
#define D0F0xD4_x013014AC_StrapBifClkPmEn_MASK                  0x4
#define D0F0xD4_x013014AC_StrapBifBiosRomEn_OFFSET              3
#define D0F0xD4_x013014AC_StrapBifBiosRomEn_WIDTH               1
#define D0F0xD4_x013014AC_StrapBifBiosRomEn_MASK                0x8
#define D0F0xD4_x013014AC_StrapBifSmbusDis_OFFSET               4
#define D0F0xD4_x013014AC_StrapBifSmbusDis_WIDTH                1
#define D0F0xD4_x013014AC_StrapBifSmbusDis_MASK                 0x10
#define D0F0xD4_x013014AC_StrapBifMemApSizePin_OFFSET           5
#define D0F0xD4_x013014AC_StrapBifMemApSizePin_WIDTH            3
#define D0F0xD4_x013014AC_StrapBifMemApSizePin_MASK             0xE0
#define D0F0xD4_x013014AC_Reserved_8_8_OFFSET                   8
#define D0F0xD4_x013014AC_Reserved_8_8_WIDTH                    1
#define D0F0xD4_x013014AC_Reserved_8_8_MASK                     0x100
#define D0F0xD4_x013014AC_StrapBifVgaDisPin_OFFSET              9
#define D0F0xD4_x013014AC_StrapBifVgaDisPin_WIDTH               1
#define D0F0xD4_x013014AC_StrapBifVgaDisPin_MASK                0x200
#define D0F0xD4_x013014AC_Reserved_10_10_OFFSET                 10
#define D0F0xD4_x013014AC_Reserved_10_10_WIDTH                  1
#define D0F0xD4_x013014AC_Reserved_10_10_MASK                   0x400
#define D0F0xD4_x013014AC_StrapTxCfgDrvFullSwing_OFFSET         11
#define D0F0xD4_x013014AC_StrapTxCfgDrvFullSwing_WIDTH          1
#define D0F0xD4_x013014AC_StrapTxCfgDrvFullSwing_MASK           0x800
#define D0F0xD4_x013014AC_Reserved_15_12_OFFSET                 12
#define D0F0xD4_x013014AC_Reserved_15_12_WIDTH                  4
#define D0F0xD4_x013014AC_Reserved_15_12_MASK                   0xF000
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_OFFSET             16
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_WIDTH              1
#define D0F0xD4_x013014AC_StrapBifAudioEnPin_MASK               0x10000
#define D0F0xD4_x013014AC_StrapBifCecEnPin_OFFSET               17
#define D0F0xD4_x013014AC_StrapBifCecEnPin_WIDTH                1
#define D0F0xD4_x013014AC_StrapBifCecEnPin_MASK                 0x20000
#define D0F0xD4_x013014AC_Reserved_31_18_OFFSET                 18
#define D0F0xD4_x013014AC_Reserved_31_18_WIDTH                  14
#define D0F0xD4_x013014AC_Reserved_31_18_MASK                   0xFFFC0000

/// D0F0xD4_x013014AC
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                         StrapBifGen3EnA:1; ///<
    UINT32                                         StrapBifClkPmEn:1; ///<
    UINT32                                       StrapBifBiosRomEn:1; ///<
    UINT32                                        StrapBifSmbusDis:1; ///<
    UINT32                                    StrapBifMemApSizePin:3; ///<
    UINT32                                            Reserved_8_8:1; ///<
    UINT32                                       StrapBifVgaDisPin:1; ///<
    UINT32                                          Reserved_10_10:1; ///<
    UINT32                                  StrapTxCfgDrvFullSwing:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                      StrapBifAudioEnPin:1; ///<
    UINT32                                        StrapBifCecEnPin:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014AC_STRUCT;

// **** D0F0xD4_x013014B6 Register Definition ****
// Address
#define D0F0xD4_x013014B6_ADDRESS                               0x13014B6
// Type
#define D0F0xD4_x013014B6_TYPE                                  TYPE_D0F0xD4

// Field Data
#define D0F0xD4_x013014B6_Reserved_0_0_OFFSET                   0
#define D0F0xD4_x013014B6_Reserved_0_0_WIDTH                    1
#define D0F0xD4_x013014B6_Reserved_0_0_MASK                     0x1
#define D0F0xD4_x013014B6_StrapBifIoBarDis_OFFSET               1
#define D0F0xD4_x013014B6_StrapBifIoBarDis_WIDTH                1
#define D0F0xD4_x013014B6_StrapBifIoBarDis_MASK                 0x2
#define D0F0xD4_x013014B6_StrapBifVgaDis_OFFSET                 2
#define D0F0xD4_x013014B6_StrapBifVgaDis_WIDTH                  1
#define D0F0xD4_x013014B6_StrapBifVgaDis_MASK                   0x4
#define D0F0xD4_x013014B6_StrapBifF064BarDisA_OFFSET            3
#define D0F0xD4_x013014B6_StrapBifF064BarDisA_WIDTH             1
#define D0F0xD4_x013014B6_StrapBifF064BarDisA_MASK              0x8
#define D0F0xD4_x013014B6_StrapCec64BarDis_OFFSET               4
#define D0F0xD4_x013014B6_StrapCec64BarDis_WIDTH                1
#define D0F0xD4_x013014B6_StrapCec64BarDis_MASK                 0x10
#define D0F0xD4_x013014B6_StrapBifAz64BarDisA_OFFSET            5
#define D0F0xD4_x013014B6_StrapBifAz64BarDisA_WIDTH             1
#define D0F0xD4_x013014B6_StrapBifAz64BarDisA_MASK              0x20
#define D0F0xD4_x013014B6_StrapBifF0BarEn_OFFSET                6
#define D0F0xD4_x013014B6_StrapBifF0BarEn_WIDTH                 1
#define D0F0xD4_x013014B6_StrapBifF0BarEn_MASK                  0x40
#define D0F0xD4_x013014B6_StrapBifRxIgnoreMsgErr_OFFSET         7
#define D0F0xD4_x013014B6_StrapBifRxIgnoreMsgErr_WIDTH          1
#define D0F0xD4_x013014B6_StrapBifRxIgnoreMsgErr_MASK           0x80
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_OFFSET  8
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x013014B6_StrapBifF0LegacyDeviceTypeDis_MASK    0x100
#define D0F0xD4_x013014B6_StrapBifF2LegacyDeviceTypeDis_OFFSET  9
#define D0F0xD4_x013014B6_StrapBifF2LegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x013014B6_StrapBifF2LegacyDeviceTypeDis_MASK    0x200
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_OFFSET  10
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_WIDTH   1
#define D0F0xD4_x013014B6_StrapBifAzLegacyDeviceTypeDis_MASK    0x400
#define D0F0xD4_x013014B6_StrapBifRxIgnoreEpErr_OFFSET          11
#define D0F0xD4_x013014B6_StrapBifRxIgnoreEpErr_WIDTH           1
#define D0F0xD4_x013014B6_StrapBifRxIgnoreEpErr_MASK            0x800
#define D0F0xD4_x013014B6_StrapCecPmeSupportComplianceDis_OFFSET 12
#define D0F0xD4_x013014B6_StrapCecPmeSupportComplianceDis_WIDTH 1
#define D0F0xD4_x013014B6_StrapCecPmeSupportComplianceDis_MASK  0x1000
#define D0F0xD4_x013014B6_StrapBifAudioEn_OFFSET                13
#define D0F0xD4_x013014B6_StrapBifAudioEn_WIDTH                 1
#define D0F0xD4_x013014B6_StrapBifAudioEn_MASK                  0x2000
#define D0F0xD4_x013014B6_StrapReserved_OFFSET                  14
#define D0F0xD4_x013014B6_StrapReserved_WIDTH                   8
#define D0F0xD4_x013014B6_StrapReserved_MASK                    0x3FC000
#define D0F0xD4_x013014B6_Reserved_31_22_OFFSET                 22
#define D0F0xD4_x013014B6_Reserved_31_22_WIDTH                  10
#define D0F0xD4_x013014B6_Reserved_31_22_MASK                   0xFFC00000

/// D0F0xD4_x013014B6
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                        StrapBifIoBarDis:1; ///<
    UINT32                                          StrapBifVgaDis:1; ///<
    UINT32                                     StrapBifF064BarDisA:1; ///<
    UINT32                                        StrapCec64BarDis:1; ///<
    UINT32                                     StrapBifAz64BarDisA:1; ///<
    UINT32                                         StrapBifF0BarEn:1; ///<
    UINT32                                  StrapBifRxIgnoreMsgErr:1; ///<
    UINT32                           StrapBifF0LegacyDeviceTypeDis:1; ///<
    UINT32                           StrapBifF2LegacyDeviceTypeDis:1; ///<
    UINT32                           StrapBifAzLegacyDeviceTypeDis:1; ///<
    UINT32                                   StrapBifRxIgnoreEpErr:1; ///<
    UINT32                         StrapCecPmeSupportComplianceDis:1; ///<
    UINT32                                         StrapBifAudioEn:1; ///<
    UINT32                                           StrapReserved:8; ///<
    UINT32                                          Reserved_31_22:10; ///<

  } Field;

  UINT32 Value;
} D0F0xD4_x013014B6_STRUCT;

// **** D0F0xD4_x013014BE Register Definition ****
// Address
#define D0F0xD4_x013014BE_ADDRESS                                     0x013014BE

// Type
#define D0F0xD4_x013014BE_TYPE                                        TYPE_D0F0xD4
// Field Data
#define D0F0xD4_x013014BE_Reserved_19_0_OFFSET                        0
#define D0F0xD4_x013014BE_Reserved_19_0_WIDTH                         20
#define D0F0xD4_x013014BE_Reserved_19_0_MASK                          0xfffff
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_OFFSET              20
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_WIDTH               1
#define D0F0xD4_x013014BE_StrapBifSymalignHwDebug_MASK                0x100000
#define D0F0xD4_x013014BE_Reserved_31_21_OFFSET                       21
#define D0F0xD4_x013014BE_Reserved_31_21_WIDTH                        11
#define D0F0xD4_x013014BE_Reserved_31_21_MASK                         0xffe00000

/// D0F0xD4_x013014BE
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_19_0:20; ///<
    UINT32                                  StrapBifSymalignHwDebug:1 ; ///<
    UINT32                                           Reserved_31_21:11; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xD4_x013014BE_STRUCT;

// **** D0F0xE4_WRAP_0050 Register Definition ****
// Address
#define D0F0xE4_WRAP_0050_ADDRESS                               0x50

// Type
#define D0F0xE4_WRAP_0050_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0050_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0050_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0050_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0050
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0050_STRUCT;

// **** D0F0xE4_WRAP_0051 Register Definition ****
// Address
#define D0F0xE4_WRAP_0051_ADDRESS                               0x51

// Type
#define D0F0xE4_WRAP_0051_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0051_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0051_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0051_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0051_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0051
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0051_STRUCT;

// **** D0F0xE4_WRAP_0052 Register Definition ****
// Address
#define D0F0xE4_WRAP_0052_ADDRESS                               0x52

// Type
#define D0F0xE4_WRAP_0052_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0052_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0052_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0052_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0052_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0052
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0052_STRUCT;

// **** D0F0xE4_WRAP_0053 Register Definition ****
// Address
#define D0F0xE4_WRAP_0053_ADDRESS                               0x53

// Type
#define D0F0xE4_WRAP_0053_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0053_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0053_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0053_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0053_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0053
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0053_STRUCT;

// **** D0F0xE4_WRAP_0054 Register Definition ****
// Address
#define D0F0xE4_WRAP_0054_ADDRESS                               0x54

// Type
#define D0F0xE4_WRAP_0054_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0054_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0054_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0054_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0054_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0054
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0054_STRUCT;

// **** D0F0xE4_WRAP_0055 Register Definition ****
// Address
#define D0F0xE4_WRAP_0055_ADDRESS                               0x55

// Type
#define D0F0xE4_WRAP_0055_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0055_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0055_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0055_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0055_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0055
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0055_STRUCT;

// **** D0F0xE4_WRAP_0056 Register Definition ****
// Address
#define D0F0xE4_WRAP_0056_ADDRESS                               0x56

// Type
#define D0F0xE4_WRAP_0056_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0056_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0056_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0056_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0056_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0056
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0056_STRUCT;

// **** D0F0xE4_WRAP_0057 Register Definition ****
// Address
#define D0F0xE4_WRAP_0057_ADDRESS                               0x57

// Type
#define D0F0xE4_WRAP_0057_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0057_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0057_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0057_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0057_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0057
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0057_STRUCT;

// **** D0F0xE4_WRAP_0058 Register Definition ****
// Address
#define D0F0xE4_WRAP_0058_ADDRESS                               0x58

// Type
#define D0F0xE4_WRAP_0058_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0058_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0058_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0058_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0058_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0058
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0058_STRUCT;

// **** D0F0xE4_WRAP_0059 Register Definition ****
// Address
#define D0F0xE4_WRAP_0059_ADDRESS                               0x59

// Type
#define D0F0xE4_WRAP_0059_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_0059_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_0059_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_0059_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_0059_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_0059
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0059_STRUCT;

// **** D0F0xE4_WRAP_005A Register Definition ****
// Address
#define D0F0xE4_WRAP_005A_ADDRESS                               0x5a

// Type
#define D0F0xE4_WRAP_005A_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005A_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005A_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005A_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005A_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005A
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005A_STRUCT;

// **** D0F0xE4_WRAP_005B Register Definition ****
// Address
#define D0F0xE4_WRAP_005B_ADDRESS                               0x5b

// Type
#define D0F0xE4_WRAP_005B_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005B_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005B_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005B_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005B_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005B
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005B_STRUCT;

// **** D0F0xE4_WRAP_005C Register Definition ****
// Address
#define D0F0xE4_WRAP_005C_ADDRESS                               0x5c

// Type
#define D0F0xE4_WRAP_005C_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005C_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005C_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005C_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005C_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005C
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005C_STRUCT;

// **** D0F0xE4_WRAP_005D Register Definition ****
// Address
#define D0F0xE4_WRAP_005D_ADDRESS                               0x5d

// Type
#define D0F0xE4_WRAP_005D_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005D_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005D_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005D_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005D_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005D
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005D_STRUCT;

// **** D0F0xE4_WRAP_005E Register Definition ****
// Address
#define D0F0xE4_WRAP_005E_ADDRESS                               0x5e

// Type
#define D0F0xE4_WRAP_005E_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005E_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005E_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005E_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005E_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005E
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005E_STRUCT;

// **** D0F0xE4_WRAP_005F Register Definition ****
// Address
#define D0F0xE4_WRAP_005F_ADDRESS                               0x5f

// Type
#define D0F0xE4_WRAP_005F_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET 0
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK 0x7
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET 3
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortRxPresetHint_WIDTH 3
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK 0x38
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET 6
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK 0x3c0
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET 10
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortTxPreset_WIDTH 4
#define D0F0xE4_WRAP_005F_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK 0x3c00
#define D0F0xE4_WRAP_005F_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_005F_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_005F_Reserved_31_14_MASK                   0xffffc000

/// D0F0xE4_WRAP_005F
typedef union {
  struct {                                                              ///<
    UINT32                 StrapBifPcieLaneEqCntlDsPortRxPresetHint:3 ; ///<
    UINT32                 StrapBifPcieLaneEqCntlUsPortRxPresetHint:3 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlDsPortTxPreset:4 ; ///<
    UINT32                     StrapBifPcieLaneEqCntlUsPortTxPreset:4 ; ///<
    UINT32                                           Reserved_31_14:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_005F_STRUCT;

// **** D0F0xE4_WRAP_0080 Register Definition ****
// Address
#define D0F0xE4_WRAP_0080_ADDRESS                               0x80

// Type
#define D0F0xE4_WRAP_0080_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0080_StrapBifLinkConfig_OFFSET             0
#define D0F0xE4_WRAP_0080_StrapBifLinkConfig_WIDTH              4
#define D0F0xE4_WRAP_0080_StrapBifLinkConfig_MASK               0xf
#define D0F0xE4_WRAP_0080_Reserved_31_4_OFFSET                  4
#define D0F0xE4_WRAP_0080_Reserved_31_4_WIDTH                   28
#define D0F0xE4_WRAP_0080_Reserved_31_4_MASK                    0xfffffff0

/// D0F0xE4_WRAP_0080
typedef union {
  struct {                                                              ///<
    UINT32                                       StrapBifLinkConfig:4 ; ///<
    UINT32                                            Reserved_31_4:28; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0080_STRUCT;

// **** D0F0xE4_WRAP_0801 Register Definition ****
// Address
#define D0F0xE4_WRAP_0801_ADDRESS                               0x801

// Type
#define D0F0xE4_WRAP_0801_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0801_Reserved_6_0_OFFSET                   0
#define D0F0xE4_WRAP_0801_Reserved_6_0_WIDTH                    7
#define D0F0xE4_WRAP_0801_Reserved_6_0_MASK                     0x7f
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEq_OFFSET             7
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEq_WIDTH              1
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEq_MASK               0x80
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEqReqPhase_OFFSET     8
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEqReqPhase_WIDTH      1
#define D0F0xE4_WRAP_0801_StrapBifLcBypassEqReqPhase_MASK       0x100
#define D0F0xE4_WRAP_0801_StrapBifLcEqSearchMode_OFFSET         9
#define D0F0xE4_WRAP_0801_StrapBifLcEqSearchMode_WIDTH          2
#define D0F0xE4_WRAP_0801_StrapBifLcEqSearchMode_MASK           0x600
#define D0F0xE4_WRAP_0801_StrapBifGen3En_OFFSET                 11
#define D0F0xE4_WRAP_0801_StrapBifGen3En_WIDTH                  1
#define D0F0xE4_WRAP_0801_StrapBifGen3En_MASK                   0x800
#define D0F0xE4_WRAP_0801_StrapBifTargetLinkSpeed_OFFSET        12
#define D0F0xE4_WRAP_0801_StrapBifTargetLinkSpeed_WIDTH         2
#define D0F0xE4_WRAP_0801_StrapBifTargetLinkSpeed_MASK          0x3000
#define D0F0xE4_WRAP_0801_Reserved_15_14_OFFSET                 14
#define D0F0xE4_WRAP_0801_Reserved_15_14_WIDTH                  2
#define D0F0xE4_WRAP_0801_Reserved_15_14_MASK                   0xc000
#define D0F0xE4_WRAP_0801_StrapBifLcEqFs_OFFSET                 16
#define D0F0xE4_WRAP_0801_StrapBifLcEqFs_WIDTH                  6
#define D0F0xE4_WRAP_0801_StrapBifLcEqFs_MASK                   0x3f0000
#define D0F0xE4_WRAP_0801_Reserved_23_22_OFFSET                 22
#define D0F0xE4_WRAP_0801_Reserved_23_22_WIDTH                  2
#define D0F0xE4_WRAP_0801_Reserved_23_22_MASK                   0xc00000
#define D0F0xE4_WRAP_0801_StrapBifLcEqLf_OFFSET                 24
#define D0F0xE4_WRAP_0801_StrapBifLcEqLf_WIDTH                  6
#define D0F0xE4_WRAP_0801_StrapBifLcEqLf_MASK                   0x3f000000
#define D0F0xE4_WRAP_0801_Reserved_31_30_OFFSET                 30
#define D0F0xE4_WRAP_0801_Reserved_31_30_WIDTH                  2
#define D0F0xE4_WRAP_0801_Reserved_31_30_MASK                   0xc0000000

/// D0F0xE4_WRAP_0801
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_6_0:7 ; ///<
    UINT32                                       StrapBifLcBypassEq:1 ; ///<
    UINT32                               StrapBifLcBypassEqReqPhase:1 ; ///<
    UINT32                                   StrapBifLcEqSearchMode:2 ; ///<
    UINT32                                           StrapBifGen3En:1 ; ///<
    UINT32                                  StrapBifTargetLinkSpeed:2 ; ///<
    UINT32                                           Reserved_15_14:2 ; ///<
    UINT32                                           StrapBifLcEqFs:6 ; ///<
    UINT32                                           Reserved_23_22:2 ; ///<
    UINT32                                           StrapBifLcEqLf:6 ; ///<
    UINT32                                           Reserved_31_30:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0801_STRUCT;

// **** D0F0xE4_WRAP_0802 Register Definition ****
// Address
#define D0F0xE4_WRAP_0802_ADDRESS                               0x802

// Type
#define D0F0xE4_WRAP_0802_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0802_Reserved_13_0_OFFSET                  0
#define D0F0xE4_WRAP_0802_Reserved_13_0_WIDTH                   14
#define D0F0xE4_WRAP_0802_Reserved_13_0_MASK                    0x3fff
#define D0F0xE4_WRAP_0802_StrapBifPMSupport_OFFSET              14
#define D0F0xE4_WRAP_0802_StrapBifPMSupport_WIDTH               2
#define D0F0xE4_WRAP_0802_StrapBifPMSupport_MASK                0xc000
#define D0F0xE4_WRAP_0802_StrapBifL1ExitLatency_OFFSET          16
#define D0F0xE4_WRAP_0802_StrapBifL1ExitLatency_WIDTH           3
#define D0F0xE4_WRAP_0802_StrapBifL1ExitLatency_MASK            0x70000
#define D0F0xE4_WRAP_0802_StrapBifL0sExitLatency_OFFSET         19
#define D0F0xE4_WRAP_0802_StrapBifL0sExitLatency_WIDTH          3
#define D0F0xE4_WRAP_0802_StrapBifL0sExitLatency_MASK           0x380000
#define D0F0xE4_WRAP_0802_StrapEnableSignalExitL1_OFFSET        22
#define D0F0xE4_WRAP_0802_StrapEnableSignalExitL1_WIDTH         1
#define D0F0xE4_WRAP_0802_StrapEnableSignalExitL1_MASK          0x400000
#define D0F0xE4_WRAP_0802_Reserved_31_23_OFFSET                 23
#define D0F0xE4_WRAP_0802_Reserved_31_23_WIDTH                  9
#define D0F0xE4_WRAP_0802_Reserved_31_23_MASK                   0xff800000

/// D0F0xE4_WRAP_0802
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_13_0:14; ///<
    UINT32                                        StrapBifPMSupport:2 ; ///<
    UINT32                                    StrapBifL1ExitLatency:3 ; ///<
    UINT32                                   StrapBifL0sExitLatency:3 ; ///<
    UINT32                                  StrapEnableSignalExitL1:1 ; ///<
    UINT32                                           Reserved_31_23:9 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0802_STRUCT;

// **** D0F0xE4_WRAP_0901 Register Definition ****
// Address
#define D0F0xE4_WRAP_0901_ADDRESS                               0x901

// Type
#define D0F0xE4_WRAP_0901_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_0901_Reserved_6_0_OFFSET                   0
#define D0F0xE4_WRAP_0901_Reserved_6_0_WIDTH                    7
#define D0F0xE4_WRAP_0901_Reserved_6_0_MASK                     0x7f
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEq_OFFSET             7
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEq_WIDTH              1
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEq_MASK               0x80
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEqReqPhase_OFFSET     8
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEqReqPhase_WIDTH      1
#define D0F0xE4_WRAP_0901_StrapBifLcBypassEqReqPhase_MASK       0x100
#define D0F0xE4_WRAP_0901_StrapBifLcEqSearchMode_OFFSET         9
#define D0F0xE4_WRAP_0901_StrapBifLcEqSearchMode_WIDTH          2
#define D0F0xE4_WRAP_0901_StrapBifLcEqSearchMode_MASK           0x600
#define D0F0xE4_WRAP_0901_StrapBifGen3En_OFFSET                 11
#define D0F0xE4_WRAP_0901_StrapBifGen3En_WIDTH                  1
#define D0F0xE4_WRAP_0901_StrapBifGen3En_MASK                   0x800
#define D0F0xE4_WRAP_0901_StrapBifTargetLinkSpeed_OFFSET        12
#define D0F0xE4_WRAP_0901_StrapBifTargetLinkSpeed_WIDTH         2
#define D0F0xE4_WRAP_0901_StrapBifTargetLinkSpeed_MASK          0x3000
#define D0F0xE4_WRAP_0901_Reserved_15_14_OFFSET                 14
#define D0F0xE4_WRAP_0901_Reserved_15_14_WIDTH                  2
#define D0F0xE4_WRAP_0901_Reserved_15_14_MASK                   0xc000
#define D0F0xE4_WRAP_0901_StrapBifLcEqFs_OFFSET                 16
#define D0F0xE4_WRAP_0901_StrapBifLcEqFs_WIDTH                  6
#define D0F0xE4_WRAP_0901_StrapBifLcEqFs_MASK                   0x3f0000
#define D0F0xE4_WRAP_0901_Reserved_23_22_OFFSET                 22
#define D0F0xE4_WRAP_0901_Reserved_23_22_WIDTH                  2
#define D0F0xE4_WRAP_0901_Reserved_23_22_MASK                   0xc00000
#define D0F0xE4_WRAP_0901_StrapBifLcEqLf_OFFSET                 24
#define D0F0xE4_WRAP_0901_StrapBifLcEqLf_WIDTH                  6
#define D0F0xE4_WRAP_0901_StrapBifLcEqLf_MASK                   0x3f000000
#define D0F0xE4_WRAP_0901_Reserved_31_30_OFFSET                 30
#define D0F0xE4_WRAP_0901_Reserved_31_30_WIDTH                  2
#define D0F0xE4_WRAP_0901_Reserved_31_30_MASK                   0xc0000000

/// D0F0xE4_WRAP_0901
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_6_0:7 ; ///<
    UINT32                                       StrapBifLcBypassEq:1 ; ///<
    UINT32                               StrapBifLcBypassEqReqPhase:1 ; ///<
    UINT32                                   StrapBifLcEqSearchMode:2 ; ///<
    UINT32                                           StrapBifGen3En:1 ; ///<
    UINT32                                  StrapBifTargetLinkSpeed:2 ; ///<
    UINT32                                           Reserved_15_14:2 ; ///<
    UINT32                                           StrapBifLcEqFs:6 ; ///<
    UINT32                                           Reserved_23_22:2 ; ///<
    UINT32                                           StrapBifLcEqLf:6 ; ///<
    UINT32                                           Reserved_31_30:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_0901_STRUCT;

// **** D0F0xE4_WRAP_8011 Register Definition ****
// Address
#define D0F0xE4_WRAP_8011_ADDRESS                               0x1308011
// Type
#define D0F0xE4_WRAP_8011_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_OFFSET            0
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_WIDTH             6
#define D0F0xE4_WRAP_8011_TxclkDynGateLatency_MASK              0x3F
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_OFFSET              6
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_WIDTH               1
#define D0F0xE4_WRAP_8011_TxclkPermGateEven_MASK                0x40
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_OFFSET             7
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_WIDTH              1
#define D0F0xE4_WRAP_8011_TxclkDynGateEnable_MASK               0x80
#define D0F0xE4_WRAP_8011_TxclkPermStop_OFFSET                  8
#define D0F0xE4_WRAP_8011_TxclkPermStop_WIDTH                   1
#define D0F0xE4_WRAP_8011_TxclkPermStop_MASK                    0x100
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_OFFSET            9
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8011_TxclkRegsGateEnable_MASK              0x200
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_OFFSET           10
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8011_TxclkRegsGateLatency_MASK             0xFC00
#define D0F0xE4_WRAP_8011_Bitfield_16_16_OFFSET                 16
#define D0F0xE4_WRAP_8011_Bitfield_16_16_WIDTH                  1
#define D0F0xE4_WRAP_8011_Bitfield_16_16_MASK                   0x10000
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_OFFSET           17
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8011_TxclkPermGateLatency_MASK             0x7E0000
#define D0F0xE4_WRAP_8011_Bitfield_23_23_OFFSET                 23
#define D0F0xE4_WRAP_8011_Bitfield_23_23_WIDTH                  1
#define D0F0xE4_WRAP_8011_Bitfield_23_23_MASK                   0x800000
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_OFFSET            24
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8011_TxclkLcntGateEnable_MASK              0x1000000
#define D0F0xE4_WRAP_8011_Reserved_25_25_OFFSET                 25
#define D0F0xE4_WRAP_8011_Reserved_25_25_WIDTH                  1
#define D0F0xE4_WRAP_8011_Reserved_25_25_MASK                   0x2000000
#define D0F0xE4_WRAP_8011_Reserved_31_26_OFFSET                 26
#define D0F0xE4_WRAP_8011_Reserved_31_26_WIDTH                  6
#define D0F0xE4_WRAP_8011_Reserved_31_26_MASK                   0xFC000000

/// D0F0xE4_WRAP_8011
typedef union {
  struct {                                                            ///<
    UINT32                                     TxclkDynGateLatency:6; ///<
    UINT32                                       TxclkPermGateEven:1; ///<
    UINT32                                      TxclkDynGateEnable:1; ///<
    UINT32                                           TxclkPermStop:1; ///<
    UINT32                                     TxclkRegsGateEnable:1; ///<
    UINT32                                    TxclkRegsGateLatency:6; ///<
    UINT32                                          Bitfield_16_16:1; ///<
    UINT32                                    TxclkPermGateLatency:6; ///<
    UINT32                                          Bitfield_23_23:1; ///<
    UINT32                                     TxclkLcntGateEnable:1; ///<
    UINT32                                          Reserved_25_25:1; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8011_STRUCT;

// **** D0F0xE4_WRAP_8012 Register Definition ****
// Address
#define D0F0xE4_WRAP_8012_ADDRESS                               0x8012
// Type
#define D0F0xE4_WRAP_8012_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_OFFSET           0
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_WIDTH            6
#define D0F0xE4_WRAP_8012_Pif1xIdleGateLatency_MASK             0x3F
#define D0F0xE4_WRAP_8012_Reserved_6_6_OFFSET                   6
#define D0F0xE4_WRAP_8012_Reserved_6_6_WIDTH                    1
#define D0F0xE4_WRAP_8012_Reserved_6_6_MASK                     0x40
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_OFFSET            7
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8012_Pif1xIdleGateEnable_MASK              0x80
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_OFFSET         8
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_WIDTH          6
#define D0F0xE4_WRAP_8012_Pif1xIdleResumeLatency_MASK           0x3F00
#define D0F0xE4_WRAP_8012_Reserved_31_14_OFFSET                 14
#define D0F0xE4_WRAP_8012_Reserved_31_14_WIDTH                  18
#define D0F0xE4_WRAP_8012_Reserved_31_14_MASK                   0xFFFFC000

/// D0F0xE4_WRAP_8012
typedef union {
  struct {                                                            ///<
    UINT32                                    Pif1xIdleGateLatency:6; ///<
    UINT32                                            Reserved_6_6:1; ///<
    UINT32                                     Pif1xIdleGateEnable:1; ///<
    UINT32                                  Pif1xIdleResumeLatency:6; ///<
    UINT32                                          Reserved_31_14:18; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8012_STRUCT;

// **** D0F0xE4_WRAP_8014 Register Definition ****
// Address
#define D0F0xE4_WRAP_8014_ADDRESS                               0x8014
// Type
#define D0F0xE4_WRAP_8014_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_OFFSET            0
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_TxclkPermGateEnable_MASK              0x1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_OFFSET            1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_TxclkPrbsGateEnable_MASK              0x2
#define D0F0xE4_WRAP_8014_DdiPifa1xGateEnable_OFFSET            2
#define D0F0xE4_WRAP_8014_DdiPifa1xGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_DdiPifa1xGateEnable_MASK              0x4
#define D0F0xE4_WRAP_8014_DdiPifb1xGateEnable_OFFSET            3
#define D0F0xE4_WRAP_8014_DdiPifb1xGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_DdiPifb1xGateEnable_MASK              0x8
#define D0F0xE4_WRAP_8014_DdiPifc1xGateEnable_OFFSET            4
#define D0F0xE4_WRAP_8014_DdiPifc1xGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_DdiPifc1xGateEnable_MASK              0x10
#define D0F0xE4_WRAP_8014_DdiPifd1xGateEnable_OFFSET            5
#define D0F0xE4_WRAP_8014_DdiPifd1xGateEnable_WIDTH             1
#define D0F0xE4_WRAP_8014_DdiPifd1xGateEnable_MASK              0x20
#define D0F0xE4_WRAP_8014_Reserved_11_6_OFFSET                  6
#define D0F0xE4_WRAP_8014_Reserved_11_6_WIDTH                   6
#define D0F0xE4_WRAP_8014_Reserved_11_6_MASK                    0xFC0
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_OFFSET           12
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifA1xEnable_MASK             0x1000
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_OFFSET           13
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifB1xEnable_MASK             0x2000
#define D0F0xE4_WRAP_8014_PcieGatePifC1xEnable_OFFSET           14
#define D0F0xE4_WRAP_8014_PcieGatePifC1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifC1xEnable_MASK             0x4000
#define D0F0xE4_WRAP_8014_PcieGatePifD1xEnable_OFFSET           15
#define D0F0xE4_WRAP_8014_PcieGatePifD1xEnable_WIDTH            1
#define D0F0xE4_WRAP_8014_PcieGatePifD1xEnable_MASK             0x8000
#define D0F0xE4_WRAP_8014_Reserved_19_16_OFFSET                 16
#define D0F0xE4_WRAP_8014_Reserved_19_16_WIDTH                  4
#define D0F0xE4_WRAP_8014_Reserved_19_16_MASK                   0xF0000
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_OFFSET  20
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_WIDTH   1
#define D0F0xE4_WRAP_8014_TxclkPermGateOnlyWhenPllPwrDn_MASK    0x100000
#define D0F0xE4_WRAP_8014_Reserved_23_21_OFFSET                 21
#define D0F0xE4_WRAP_8014_Reserved_23_21_WIDTH                  3
#define D0F0xE4_WRAP_8014_Reserved_23_21_MASK                   0xE00000
#define D0F0xE4_WRAP_8014_DdiDigaGateEnable_OFFSET              24
#define D0F0xE4_WRAP_8014_DdiDigaGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8014_DdiDigaGateEnable_MASK                0x1000000
#define D0F0xE4_WRAP_8014_DdiDigbGateEnable_OFFSET              25
#define D0F0xE4_WRAP_8014_DdiDigbGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8014_DdiDigbGateEnable_MASK                0x2000000
#define D0F0xE4_WRAP_8014_DdiDigcGateEnable_OFFSET              26
#define D0F0xE4_WRAP_8014_DdiDigcGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8014_DdiDigcGateEnable_MASK                0x4000000
#define D0F0xE4_WRAP_8014_DdiDigdGateEnable_OFFSET              27
#define D0F0xE4_WRAP_8014_DdiDigdGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8014_DdiDigdGateEnable_MASK                0x8000000
#define D0F0xE4_WRAP_8014_SpareRegRw_OFFSET                     28
#define D0F0xE4_WRAP_8014_SpareRegRw_WIDTH                      4
#define D0F0xE4_WRAP_8014_SpareRegRw_MASK                       0xF0000000

/// D0F0xE4_WRAP_8014
typedef union {
  struct {                                                            ///<
    UINT32                                     TxclkPermGateEnable:1; ///<
    UINT32                                     TxclkPrbsGateEnable:1; ///<
    UINT32                                     DdiPifa1xGateEnable:1; ///<
    UINT32                                     DdiPifb1xGateEnable:1; ///<
    UINT32                                     DdiPifc1xGateEnable:1; ///<
    UINT32                                     DdiPifd1xGateEnable:1; ///<
    UINT32                                           Reserved_11_6:6; ///<
    UINT32                                    PcieGatePifA1xEnable:1; ///<
    UINT32                                    PcieGatePifB1xEnable:1; ///<
    UINT32                                    PcieGatePifC1xEnable:1; ///<
    UINT32                                    PcieGatePifD1xEnable:1; ///<
    UINT32                                          Reserved_19_16:4; ///<
    UINT32                           TxclkPermGateOnlyWhenPllPwrDn:1; ///<
    UINT32                                          Reserved_23_21:3; ///<
    UINT32                                       DdiDigaGateEnable:1; ///<
    UINT32                                       DdiDigbGateEnable:1; ///<
    UINT32                                       DdiDigcGateEnable:1; ///<
    UINT32                                       DdiDigdGateEnable:1; ///<
    UINT32                                              SpareRegRw:4; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8014_STRUCT;

// **** D0F0xE4_WRAP_8015 Register Definition ****
// Address
#define D0F0xE4_WRAP_8015_ADDRESS                               0x1308015
// Type
#define D0F0xE4_WRAP_8015_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8015_Reserved_8_0_OFFSET                   0
#define D0F0xE4_WRAP_8015_Reserved_8_0_WIDTH                    9
#define D0F0xE4_WRAP_8015_Reserved_8_0_MASK                     0x1FF
#define D0F0xE4_WRAP_8015_Bitfield_9_9_OFFSET                   9
#define D0F0xE4_WRAP_8015_Bitfield_9_9_WIDTH                    1
#define D0F0xE4_WRAP_8015_Bitfield_9_9_MASK                     0x200
#define D0F0xE4_WRAP_8015_Bitfield_10_10_OFFSET                 10
#define D0F0xE4_WRAP_8015_Bitfield_10_10_WIDTH                  1
#define D0F0xE4_WRAP_8015_Bitfield_10_10_MASK                   0x400
#define D0F0xE4_WRAP_8015_Bitfield_11_11_OFFSET                 11
#define D0F0xE4_WRAP_8015_Bitfield_11_11_WIDTH                  1
#define D0F0xE4_WRAP_8015_Bitfield_11_11_MASK                   0x800
#define D0F0xE4_WRAP_8015_Reserved_13_12_OFFSET                 12
#define D0F0xE4_WRAP_8015_Reserved_13_12_WIDTH                  2
#define D0F0xE4_WRAP_8015_Reserved_13_12_MASK                   0x3000
#define D0F0xE4_WRAP_8015_Bitfield_15_14_OFFSET                 14
#define D0F0xE4_WRAP_8015_Bitfield_15_14_WIDTH                  2
#define D0F0xE4_WRAP_8015_Bitfield_15_14_MASK                   0xC000
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_OFFSET          16
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_WIDTH           6
#define D0F0xE4_WRAP_8015_RefclkRegsGateLatency_MASK            0x3F0000
#define D0F0xE4_WRAP_8015_Reserved_22_22_OFFSET                 22
#define D0F0xE4_WRAP_8015_Reserved_22_22_WIDTH                  1
#define D0F0xE4_WRAP_8015_Reserved_22_22_MASK                   0x400000
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_OFFSET           23
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_WIDTH            1
#define D0F0xE4_WRAP_8015_RefclkRegsGateEnable_MASK             0x800000
#define D0F0xE4_WRAP_8015_Reserved_31_24_OFFSET                 24
#define D0F0xE4_WRAP_8015_Reserved_31_24_WIDTH                  8
#define D0F0xE4_WRAP_8015_Reserved_31_24_MASK                   0xFF000000

/// D0F0xE4_WRAP_8015
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_8_0:9; ///<
    UINT32                                            Bitfield_9_9:1; ///<
    UINT32                                          Bitfield_10_10:1; ///<
    UINT32                                          Bitfield_11_11:1; ///<
    UINT32                                          Reserved_13_12:2; ///<
    UINT32                                          Bitfield_15_14:2; ///<
    UINT32                                   RefclkRegsGateLatency:6; ///<
    UINT32                                          Reserved_22_22:1; ///<
    UINT32                                    RefclkRegsGateEnable:1; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8015_STRUCT;

// **** D0F0xE4_WRAP_8016 Register Definition ****
// Address
#define D0F0xE4_WRAP_8016_ADDRESS                               0x8016
// Type
#define D0F0xE4_WRAP_8016_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8016_CalibAckLatency_OFFSET                0
#define D0F0xE4_WRAP_8016_CalibAckLatency_WIDTH                 6
#define D0F0xE4_WRAP_8016_CalibAckLatency_MASK                  0x3F
#define D0F0xE4_WRAP_8016_Reserved_15_6_OFFSET                  6
#define D0F0xE4_WRAP_8016_Reserved_15_6_WIDTH                   10
#define D0F0xE4_WRAP_8016_Reserved_15_6_MASK                    0xFFC0
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_OFFSET             16
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_WIDTH              6
#define D0F0xE4_WRAP_8016_LclkDynGateLatency_MASK               0x3F0000
#define D0F0xE4_WRAP_8016_LclkGateFree_OFFSET                   22
#define D0F0xE4_WRAP_8016_LclkGateFree_WIDTH                    1
#define D0F0xE4_WRAP_8016_LclkGateFree_MASK                     0x400000
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_OFFSET              23
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_WIDTH               1
#define D0F0xE4_WRAP_8016_LclkDynGateEnable_MASK                0x800000
#define D0F0xE4_WRAP_8016_Reserved_31_24_OFFSET                 24
#define D0F0xE4_WRAP_8016_Reserved_31_24_WIDTH                  8
#define D0F0xE4_WRAP_8016_Reserved_31_24_MASK                   0xFF000000

/// D0F0xE4_WRAP_8016
typedef union {
  struct {                                                            ///<
    UINT32                                         CalibAckLatency:6; ///<
    UINT32                                           Reserved_15_6:10; ///<
    UINT32                                      LclkDynGateLatency:6; ///<
    UINT32                                            LclkGateFree:1; ///<
    UINT32                                       LclkDynGateEnable:1; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8016_STRUCT;

// **** D0F0xE4_WRAP_8020 Register Definition ****
// Address
#define D0F0xE4_WRAP_8020_ADDRESS                               0x8020

// Type
#define D0F0xE4_WRAP_8020_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_8020_Reserved_0_0_OFFSET                   0
#define D0F0xE4_WRAP_8020_Reserved_0_0_WIDTH                    1
#define D0F0xE4_WRAP_8020_Reserved_0_0_MASK                     0x1
#define D0F0xE4_WRAP_8020_LoopbackSelect_OFFSET                 1
#define D0F0xE4_WRAP_8020_LoopbackSelect_WIDTH                  4
#define D0F0xE4_WRAP_8020_LoopbackSelect_MASK                   0x1e
#define D0F0xE4_WRAP_8020_PrbsPcieLbSelect_OFFSET               5
#define D0F0xE4_WRAP_8020_PrbsPcieLbSelect_WIDTH                1
#define D0F0xE4_WRAP_8020_PrbsPcieLbSelect_MASK                 0x20
#define D0F0xE4_WRAP_8020_LoopbackHalfRate_OFFSET               6
#define D0F0xE4_WRAP_8020_LoopbackHalfRate_WIDTH                2
#define D0F0xE4_WRAP_8020_LoopbackHalfRate_MASK                 0xc0
#define D0F0xE4_WRAP_8020_LoopbackFifoPtr_OFFSET                8
#define D0F0xE4_WRAP_8020_LoopbackFifoPtr_WIDTH                 3
#define D0F0xE4_WRAP_8020_LoopbackFifoPtr_MASK                  0x700
#define D0F0xE4_WRAP_8020_Reserved_31_11_OFFSET                 11
#define D0F0xE4_WRAP_8020_Reserved_31_11_WIDTH                  21
#define D0F0xE4_WRAP_8020_Reserved_31_11_MASK                   0xfffff800

/// D0F0xE4_WRAP_8020
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                           LoopbackSelect:4 ; ///<
    UINT32                                         PrbsPcieLbSelect:1 ; ///<
    UINT32                                         LoopbackHalfRate:2 ; ///<
    UINT32                                          LoopbackFifoPtr:3 ; ///<
    UINT32                                           Reserved_31_11:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_8020_STRUCT;

// **** D0F0xE4_WRAP_802A Register Definition ****
// Address
#define D0F0xE4_WRAP_802A_ADDRESS                               0x802a

// Type
#define D0F0xE4_WRAP_802A_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_802A_PrbsPcieSelect_OFFSET                 0
#define D0F0xE4_WRAP_802A_PrbsPcieSelect_WIDTH                  16
#define D0F0xE4_WRAP_802A_PrbsPcieSelect_MASK                   0xffff
#define D0F0xE4_WRAP_802A_Reserved_31_16_OFFSET                 16
#define D0F0xE4_WRAP_802A_Reserved_31_16_WIDTH                  16
#define D0F0xE4_WRAP_802A_Reserved_31_16_MASK                   0xffff0000

/// D0F0xE4_WRAP_802A
typedef union {
  struct {                                                              ///<
    UINT32                                           PrbsPcieSelect:16; ///<
    UINT32                                           Reserved_31_16:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_802A_STRUCT;

// **** D0F0xE4_WRAP_802B Register Definition ****
// Address
#define D0F0xE4_WRAP_802B_ADDRESS                               0x802b

// Type
#define D0F0xE4_WRAP_802B_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_WRAP_802B_LMTxPhyCmd0_OFFSET                    0
#define D0F0xE4_WRAP_802B_LMTxPhyCmd0_WIDTH                     3
#define D0F0xE4_WRAP_802B_LMTxPhyCmd0_MASK                      0x7
#define D0F0xE4_WRAP_802B_LMRxPhyCmd0_OFFSET                    3
#define D0F0xE4_WRAP_802B_LMRxPhyCmd0_WIDTH                     2
#define D0F0xE4_WRAP_802B_LMRxPhyCmd0_MASK                      0x18
#define D0F0xE4_WRAP_802B_LMLinkSpeed0_OFFSET                   5
#define D0F0xE4_WRAP_802B_LMLinkSpeed0_WIDTH                    2
#define D0F0xE4_WRAP_802B_LMLinkSpeed0_MASK                     0x60
#define D0F0xE4_WRAP_802B_Reserved_7_7_OFFSET                   7
#define D0F0xE4_WRAP_802B_Reserved_7_7_WIDTH                    1
#define D0F0xE4_WRAP_802B_Reserved_7_7_MASK                     0x80
#define D0F0xE4_WRAP_802B_LMTxPhyCmd1_OFFSET                    8
#define D0F0xE4_WRAP_802B_LMTxPhyCmd1_WIDTH                     3
#define D0F0xE4_WRAP_802B_LMTxPhyCmd1_MASK                      0x700
#define D0F0xE4_WRAP_802B_LMRxPhyCmd1_OFFSET                    11
#define D0F0xE4_WRAP_802B_LMRxPhyCmd1_WIDTH                     2
#define D0F0xE4_WRAP_802B_LMRxPhyCmd1_MASK                      0x1800
#define D0F0xE4_WRAP_802B_LMLinkSpeed1_OFFSET                   13
#define D0F0xE4_WRAP_802B_LMLinkSpeed1_WIDTH                    2
#define D0F0xE4_WRAP_802B_LMLinkSpeed1_MASK                     0x6000
#define D0F0xE4_WRAP_802B_Reserved_15_15_OFFSET                 15
#define D0F0xE4_WRAP_802B_Reserved_15_15_WIDTH                  1
#define D0F0xE4_WRAP_802B_Reserved_15_15_MASK                   0x8000
#define D0F0xE4_WRAP_802B_LMTxPhyCmd2_OFFSET                    16
#define D0F0xE4_WRAP_802B_LMTxPhyCmd2_WIDTH                     3
#define D0F0xE4_WRAP_802B_LMTxPhyCmd2_MASK                      0x70000
#define D0F0xE4_WRAP_802B_LMRxPhyCmd2_OFFSET                    19
#define D0F0xE4_WRAP_802B_LMRxPhyCmd2_WIDTH                     2
#define D0F0xE4_WRAP_802B_LMRxPhyCmd2_MASK                      0x180000
#define D0F0xE4_WRAP_802B_LMLinkSpeed2_OFFSET                   21
#define D0F0xE4_WRAP_802B_LMLinkSpeed2_WIDTH                    2
#define D0F0xE4_WRAP_802B_LMLinkSpeed2_MASK                     0x600000
#define D0F0xE4_WRAP_802B_Reserved_23_23_OFFSET                 23
#define D0F0xE4_WRAP_802B_Reserved_23_23_WIDTH                  1
#define D0F0xE4_WRAP_802B_Reserved_23_23_MASK                   0x800000
#define D0F0xE4_WRAP_802B_LMTxPhyCmd3_OFFSET                    24
#define D0F0xE4_WRAP_802B_LMTxPhyCmd3_WIDTH                     3
#define D0F0xE4_WRAP_802B_LMTxPhyCmd3_MASK                      0x7000000
#define D0F0xE4_WRAP_802B_LMRxPhyCmd3_OFFSET                    27
#define D0F0xE4_WRAP_802B_LMRxPhyCmd3_WIDTH                     2
#define D0F0xE4_WRAP_802B_LMRxPhyCmd3_MASK                      0x18000000
#define D0F0xE4_WRAP_802B_LMLinkSpeed3_OFFSET                   29
#define D0F0xE4_WRAP_802B_LMLinkSpeed3_WIDTH                    2
#define D0F0xE4_WRAP_802B_LMLinkSpeed3_MASK                     0x60000000
#define D0F0xE4_WRAP_802B_Reserved_31_31_OFFSET                 31
#define D0F0xE4_WRAP_802B_Reserved_31_31_WIDTH                  1
#define D0F0xE4_WRAP_802B_Reserved_31_31_MASK                   0x80000000

/// D0F0xE4_WRAP_802B
typedef union {
  struct {                                                              ///<
    UINT32                                              LMTxPhyCmd0:3 ; ///<
    UINT32                                              LMRxPhyCmd0:2 ; ///<
    UINT32                                             LMLinkSpeed0:2 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                              LMTxPhyCmd1:3 ; ///<
    UINT32                                              LMRxPhyCmd1:2 ; ///<
    UINT32                                             LMLinkSpeed1:2 ; ///<
    UINT32                                           Reserved_15_15:1 ; ///<
    UINT32                                              LMTxPhyCmd2:3 ; ///<
    UINT32                                              LMRxPhyCmd2:2 ; ///<
    UINT32                                             LMLinkSpeed2:2 ; ///<
    UINT32                                           Reserved_23_23:1 ; ///<
    UINT32                                              LMTxPhyCmd3:3 ; ///<
    UINT32                                              LMRxPhyCmd3:2 ; ///<
    UINT32                                             LMLinkSpeed3:2 ; ///<
    UINT32                                           Reserved_31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_WRAP_802B_STRUCT;

// **** D0F0xE4_CORE_0002 Register Definition ****
// Address
#define D0F0xE4_CORE_0002_ADDRESS                               0x2
// Type
#define D0F0xE4_CORE_0002_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0002_HwDebug_0_OFFSET                      0
#define D0F0xE4_CORE_0002_HwDebug_0_WIDTH                       1
#define D0F0xE4_CORE_0002_HwDebug_0_MASK                        0x1
#define D0F0xE4_CORE_0002_Reserved_31_1_OFFSET                  1
#define D0F0xE4_CORE_0002_Reserved_31_1_WIDTH                   31
#define D0F0xE4_CORE_0002_Reserved_31_1_MASK                    0xFFFFFFFE

/// D0F0xE4_CORE_0002
typedef union {
  struct {                                                            ///<
    UINT32                                               HwDebug_0:1; ///<
    UINT32                                           Reserved_31_1:31; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0002_STRUCT;

// **** D0F0xE4_CORE_0010 Register Definition ****
// Address
#define D0F0xE4_CORE_0010_ADDRESS                               0x10
// Type
#define D0F0xE4_CORE_0010_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0010_HwInitWrLock_OFFSET                   0
#define D0F0xE4_CORE_0010_HwInitWrLock_WIDTH                    1
#define D0F0xE4_CORE_0010_HwInitWrLock_MASK                     0x1
#define D0F0xE4_CORE_0010_Reserved_8_1_OFFSET                   1
#define D0F0xE4_CORE_0010_Reserved_8_1_WIDTH                    8
#define D0F0xE4_CORE_0010_Reserved_8_1_MASK                     0x1FE
#define D0F0xE4_CORE_0010_UmiNpMemWrite_OFFSET                  9
#define D0F0xE4_CORE_0010_UmiNpMemWrite_WIDTH                   1
#define D0F0xE4_CORE_0010_UmiNpMemWrite_MASK                    0x200
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_OFFSET            10
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_WIDTH             3
#define D0F0xE4_CORE_0010_RxUmiAdjPayloadSize_MASK              0x1C00
#define D0F0xE4_CORE_0010_Reserved_31_13_OFFSET                 13
#define D0F0xE4_CORE_0010_Reserved_31_13_WIDTH                  19
#define D0F0xE4_CORE_0010_Reserved_31_13_MASK                   0xFFFFE000

/// D0F0xE4_CORE_0010
typedef union {
  struct {                                                            ///<
    UINT32                                            HwInitWrLock:1; ///<
    UINT32                                            Reserved_8_1:8; ///<
    UINT32                                           UmiNpMemWrite:1; ///<
    UINT32                                     RxUmiAdjPayloadSize:3; ///<
    UINT32                                          Reserved_31_13:19; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0010_STRUCT;

// **** D0F0xE4_CORE_0011 Register Definition ****
// Address
#define D0F0xE4_CORE_0011_ADDRESS                               0x11
// Type
#define D0F0xE4_CORE_0011_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0011_DynClkLatency_OFFSET                  0
#define D0F0xE4_CORE_0011_DynClkLatency_WIDTH                   4
#define D0F0xE4_CORE_0011_DynClkLatency_MASK                    0xF
#define D0F0xE4_CORE_0011_Reserved_31_4_OFFSET                  4
#define D0F0xE4_CORE_0011_Reserved_31_4_WIDTH                   28
#define D0F0xE4_CORE_0011_Reserved_31_4_MASK                    0xFFFFFFF0

/// D0F0xE4_CORE_0011
typedef union {
  struct {                                                            ///<
    UINT32                                           DynClkLatency:4; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0011_STRUCT;

// **** D0F0xE4_CORE_001C Register Definition ****
// Address
#define D0F0xE4_CORE_001C_ADDRESS                               0x1C
// Type
#define D0F0xE4_CORE_001C_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_OFFSET              0
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_WIDTH               1
#define D0F0xE4_CORE_001C_TxArbRoundRobinEn_MASK                0x1
#define D0F0xE4_CORE_001C_TxArbSlvLimit_OFFSET                  1
#define D0F0xE4_CORE_001C_TxArbSlvLimit_WIDTH                   5
#define D0F0xE4_CORE_001C_TxArbSlvLimit_MASK                    0x3E
#define D0F0xE4_CORE_001C_TxArbMstLimit_OFFSET                  6
#define D0F0xE4_CORE_001C_TxArbMstLimit_WIDTH                   5
#define D0F0xE4_CORE_001C_TxArbMstLimit_MASK                    0x7C0
#define D0F0xE4_CORE_001C_Reserved_31_11_OFFSET                 11
#define D0F0xE4_CORE_001C_Reserved_31_11_WIDTH                  21
#define D0F0xE4_CORE_001C_Reserved_31_11_MASK                   0xFFFFF800

/// D0F0xE4_CORE_001C
typedef union {
  struct {                                                            ///<
    UINT32                                       TxArbRoundRobinEn:1; ///<
    UINT32                                           TxArbSlvLimit:5; ///<
    UINT32                                           TxArbMstLimit:5; ///<
    UINT32                                          Reserved_31_11:21; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_001C_STRUCT;

// **** D0F0xE4_CORE_0020 Register Definition ****
// Address
#define D0F0xE4_CORE_0020_ADDRESS                               0x20
// Type
#define D0F0xE4_CORE_0020_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_0020_Reserved_8_0_OFFSET                   0
#define D0F0xE4_CORE_0020_Reserved_8_0_WIDTH                    9
#define D0F0xE4_CORE_0020_Reserved_8_0_MASK                     0x1FF
#define D0F0xE4_CORE_0020_CiRcOrderingDis_OFFSET                9
#define D0F0xE4_CORE_0020_CiRcOrderingDis_WIDTH                 1
#define D0F0xE4_CORE_0020_CiRcOrderingDis_MASK                  0x200
#define D0F0xE4_CORE_0020_Reserved_31_10_OFFSET                 10
#define D0F0xE4_CORE_0020_Reserved_31_10_WIDTH                  22
#define D0F0xE4_CORE_0020_Reserved_31_10_MASK                   0xFFFFFC00

/// D0F0xE4_CORE_0020
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_8_0:9; ///<
    UINT32                                         CiRcOrderingDis:1; ///<
    UINT32                                          Reserved_31_10:22; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_0020_STRUCT;

// **** D0F0xE4_CORE_0040 Register Definition ****
// Address
#define D0F0xE4_CORE_0040_ADDRESS                               0x40

// Type
#define D0F0xE4_CORE_0040_TYPE                                  TYPE_D0F0xE4
// Field Data
#define D0F0xE4_CORE_0040_Reserved_0_0_OFFSET                   0
#define D0F0xE4_CORE_0040_Reserved_0_0_WIDTH                    1
#define D0F0xE4_CORE_0040_Reserved_0_0_MASK                     0x1
#define D0F0xE4_CORE_0040_PSymalignMode_OFFSET                  1
#define D0F0xE4_CORE_0040_PSymalignMode_WIDTH                   1
#define D0F0xE4_CORE_0040_PSymalignMode_MASK                    0x2
#define D0F0xE4_CORE_0040_PSymalignHwDebug_OFFSET               2
#define D0F0xE4_CORE_0040_PSymalignHwDebug_WIDTH                1
#define D0F0xE4_CORE_0040_PSymalignHwDebug_MASK                 0x4
#define D0F0xE4_CORE_0040_PElastDeskewHwDebug_OFFSET            3
#define D0F0xE4_CORE_0040_PElastDeskewHwDebug_WIDTH             1
#define D0F0xE4_CORE_0040_PElastDeskewHwDebug_MASK              0x8
#define D0F0xE4_CORE_0040_PIgnoreCrcErr_OFFSET                  4
#define D0F0xE4_CORE_0040_PIgnoreCrcErr_WIDTH                   1
#define D0F0xE4_CORE_0040_PIgnoreCrcErr_MASK                    0x10
#define D0F0xE4_CORE_0040_PIgnoreLenErr_OFFSET                  5
#define D0F0xE4_CORE_0040_PIgnoreLenErr_WIDTH                   1
#define D0F0xE4_CORE_0040_PIgnoreLenErr_MASK                    0x20
#define D0F0xE4_CORE_0040_PIgnoreEdbErr_OFFSET                  6
#define D0F0xE4_CORE_0040_PIgnoreEdbErr_WIDTH                   1
#define D0F0xE4_CORE_0040_PIgnoreEdbErr_MASK                    0x40
#define D0F0xE4_CORE_0040_PIgnoreIdlErr_OFFSET                  7
#define D0F0xE4_CORE_0040_PIgnoreIdlErr_WIDTH                   1
#define D0F0xE4_CORE_0040_PIgnoreIdlErr_MASK                    0x80
#define D0F0xE4_CORE_0040_PIgnoreTokErr_OFFSET                  8
#define D0F0xE4_CORE_0040_PIgnoreTokErr_WIDTH                   1
#define D0F0xE4_CORE_0040_PIgnoreTokErr_MASK                    0x100
#define D0F0xE4_CORE_0040_Reserved_11_9_OFFSET                  9
#define D0F0xE4_CORE_0040_Reserved_11_9_WIDTH                   3
#define D0F0xE4_CORE_0040_Reserved_11_9_MASK                    0xe00
#define D0F0xE4_CORE_0040_PBlkLockMode_OFFSET                   12
#define D0F0xE4_CORE_0040_PBlkLockMode_WIDTH                    1
#define D0F0xE4_CORE_0040_PBlkLockMode_MASK                     0x1000
#define D0F0xE4_CORE_0040_PAlwaysUseFastTxclk_OFFSET            13
#define D0F0xE4_CORE_0040_PAlwaysUseFastTxclk_WIDTH             1
#define D0F0xE4_CORE_0040_PAlwaysUseFastTxclk_MASK              0x2000
#define D0F0xE4_CORE_0040_PElecIdleMode_OFFSET                  14
#define D0F0xE4_CORE_0040_PElecIdleMode_WIDTH                   2
#define D0F0xE4_CORE_0040_PElecIdleMode_MASK                    0xc000
#define D0F0xE4_CORE_0040_PElecIdleMode_VALUE                   0x1
#define D0F0xE4_CORE_0040_DlpIgnoreInL1En_OFFSET                16
#define D0F0xE4_CORE_0040_DlpIgnoreInL1En_WIDTH                 1
#define D0F0xE4_CORE_0040_DlpIgnoreInL1En_MASK                  0x10000
#define D0F0xE4_CORE_0040_Reserved_31_17_OFFSET                 17
#define D0F0xE4_CORE_0040_Reserved_31_17_WIDTH                  15
#define D0F0xE4_CORE_0040_Reserved_31_17_MASK                   0xfffe0000

/// D0F0xE4_CORE_0040
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                            PSymalignMode:1 ; ///<
    UINT32                                         PSymalignHwDebug:1 ; ///<
    UINT32                                      PElastDeskewHwDebug:1 ; ///<
    UINT32                                            PIgnoreCrcErr:1 ; ///<
    UINT32                                            PIgnoreLenErr:1 ; ///<
    UINT32                                            PIgnoreEdbErr:1 ; ///<
    UINT32                                            PIgnoreIdlErr:1 ; ///<
    UINT32                                            PIgnoreTokErr:1 ; ///<
    UINT32                                            Reserved_11_9:3 ; ///<
    UINT32                                             PBlkLockMode:1 ; ///<
    UINT32                                      PAlwaysUseFastTxclk:1 ; ///<
    UINT32                                            PElecIdleMode:2 ; ///<
    UINT32                                          DlpIgnoreInL1En:1 ; ///<
    UINT32                                           Reserved_31_17:15; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_CORE_0040_STRUCT;

// **** D0F0xE4_CORE_00B0 Register Definition ****
// Address
#define D0F0xE4_CORE_00B0_ADDRESS                               0xB0
// Type
#define D0F0xE4_CORE_00B0_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_00B0_Reserved_1_0_OFFSET                   0
#define D0F0xE4_CORE_00B0_Reserved_1_0_WIDTH                    2
#define D0F0xE4_CORE_00B0_Reserved_1_0_MASK                     0x3
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_OFFSET                   2
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_WIDTH                    1
#define D0F0xE4_CORE_00B0_StrapF0MsiEn_MASK                     0x4
#define D0F0xE4_CORE_00B0_Reserved_4_3_OFFSET                   3
#define D0F0xE4_CORE_00B0_Reserved_4_3_WIDTH                    2
#define D0F0xE4_CORE_00B0_Reserved_4_3_MASK                     0x18
#define D0F0xE4_CORE_00B0_StrapF0AerEn_OFFSET                   5
#define D0F0xE4_CORE_00B0_StrapF0AerEn_WIDTH                    1
#define D0F0xE4_CORE_00B0_StrapF0AerEn_MASK                     0x20
#define D0F0xE4_CORE_00B0_Reserved_31_6_OFFSET                  6
#define D0F0xE4_CORE_00B0_Reserved_31_6_WIDTH                   26
#define D0F0xE4_CORE_00B0_Reserved_31_6_MASK                    0xFFFFFFC0

/// D0F0xE4_CORE_00B0
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_1_0:2; ///<
    UINT32                                            StrapF0MsiEn:1; ///<
    UINT32                                            Reserved_4_3:2; ///<
    UINT32                                            StrapF0AerEn:1; ///<
    UINT32                                           Reserved_31_6:26; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_00B0_STRUCT;

// **** D0F0xE4_CORE_00C1 Register Definition ****
// Address
#define D0F0xE4_CORE_00C1_ADDRESS                               0xC1
// Type
#define D0F0xE4_CORE_00C1_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_OFFSET   0
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_WIDTH    1
#define D0F0xE4_CORE_00C1_StrapLinkBwNotificationCapEn_MASK     0x1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_OFFSET            1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_WIDTH             1
#define D0F0xE4_CORE_00C1_StrapGen2Compliance_MASK              0x2
#define D0F0xE4_CORE_00C1_Reserved_31_2_OFFSET                  2
#define D0F0xE4_CORE_00C1_Reserved_31_2_WIDTH                   30
#define D0F0xE4_CORE_00C1_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xE4_CORE_00C1
typedef union {
  struct {                                                            ///<
    UINT32                            StrapLinkBwNotificationCapEn:1; ///<
    UINT32                                     StrapGen2Compliance:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_CORE_00C1_STRUCT;

// **** D0F0xE4_PHY_000B Register Definition ****
// Address
#define D0F0xE4_PHY_000B_ADDRESS                                0xb

// Type
#define D0F0xE4_PHY_000B_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_000B_TxPwrSbiEn_OFFSET                      0
#define D0F0xE4_PHY_000B_TxPwrSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000B_TxPwrSbiEn_MASK                        0x1
#define D0F0xE4_PHY_000B_RxPwrSbiEn_OFFSET                      1
#define D0F0xE4_PHY_000B_RxPwrSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000B_RxPwrSbiEn_MASK                        0x2
#define D0F0xE4_PHY_000B_PcieModeSbiEn_OFFSET                   2
#define D0F0xE4_PHY_000B_PcieModeSbiEn_WIDTH                    1
#define D0F0xE4_PHY_000B_PcieModeSbiEn_MASK                     0x4
#define D0F0xE4_PHY_000B_FreqDivSbiEn_OFFSET                    3
#define D0F0xE4_PHY_000B_FreqDivSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000B_FreqDivSbiEn_MASK                      0x8
#define D0F0xE4_PHY_000B_DllLockSbiEn_OFFSET                    4
#define D0F0xE4_PHY_000B_DllLockSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000B_DllLockSbiEn_MASK                      0x10
#define D0F0xE4_PHY_000B_OffsetCancelSbiEn_OFFSET               5
#define D0F0xE4_PHY_000B_OffsetCancelSbiEn_WIDTH                1
#define D0F0xE4_PHY_000B_OffsetCancelSbiEn_MASK                 0x20
#define D0F0xE4_PHY_000B_SkipBitSbiEn_OFFSET                    6
#define D0F0xE4_PHY_000B_SkipBitSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000B_SkipBitSbiEn_MASK                      0x40
#define D0F0xE4_PHY_000B_IncoherentClkSbiEn_OFFSET              7
#define D0F0xE4_PHY_000B_IncoherentClkSbiEn_WIDTH               1
#define D0F0xE4_PHY_000B_IncoherentClkSbiEn_MASK                0x80
#define D0F0xE4_PHY_000B_EiDetSbiEn_OFFSET                      8
#define D0F0xE4_PHY_000B_EiDetSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000B_EiDetSbiEn_MASK                        0x100
#define D0F0xE4_PHY_000B_Reserved_13_9_OFFSET                   9
#define D0F0xE4_PHY_000B_Reserved_13_9_WIDTH                    5
#define D0F0xE4_PHY_000B_Reserved_13_9_MASK                     0x3e00
#define D0F0xE4_PHY_000B_MargPktSbiEn_OFFSET                    14
#define D0F0xE4_PHY_000B_MargPktSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000B_MargPktSbiEn_MASK                      0x4000
#define D0F0xE4_PHY_000B_PllCmpPktSbiEn_OFFSET                  15
#define D0F0xE4_PHY_000B_PllCmpPktSbiEn_WIDTH                   1
#define D0F0xE4_PHY_000B_PllCmpPktSbiEn_MASK                    0x8000
#define D0F0xE4_PHY_000B_RxEqPktSbiEn_OFFSET                    16
#define D0F0xE4_PHY_000B_RxEqPktSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000B_RxEqPktSbiEn_MASK                      0x10000
#define D0F0xE4_PHY_000B_TxCoeffPktSbiEn_OFFSET                 17
#define D0F0xE4_PHY_000B_TxCoeffPktSbiEn_WIDTH                  1
#define D0F0xE4_PHY_000B_TxCoeffPktSbiEn_MASK                   0x20000
#define D0F0xE4_PHY_000B_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PHY_000B_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PHY_000B_Reserved_31_18_MASK                    0xfffc0000

/// D0F0xE4_PHY_000B
typedef union {
  struct {                                                              ///<
    UINT32                                               TxPwrSbiEn:1 ; ///<
    UINT32                                               RxPwrSbiEn:1 ; ///<
    UINT32                                            PcieModeSbiEn:1 ; ///<
    UINT32                                             FreqDivSbiEn:1 ; ///<
    UINT32                                             DllLockSbiEn:1 ; ///<
    UINT32                                        OffsetCancelSbiEn:1 ; ///<
    UINT32                                             SkipBitSbiEn:1 ; ///<
    UINT32                                       IncoherentClkSbiEn:1 ; ///<
    UINT32                                               EiDetSbiEn:1 ; ///<
    UINT32                                            Reserved_13_9:5 ; ///<
    UINT32                                             MargPktSbiEn:1 ; ///<
    UINT32                                           PllCmpPktSbiEn:1 ; ///<
    UINT32                                             RxEqPktSbiEn:1 ; ///<
    UINT32                                          TxCoeffPktSbiEn:1 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_000B_STRUCT;

// **** D0F0xE4_PHY_000C Register Definition ****
// Address
#define D0F0xE4_PHY_000C_ADDRESS                                0xc

// Type
#define D0F0xE4_PHY_000C_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_000C_TxPwrSbiEn_OFFSET                      0
#define D0F0xE4_PHY_000C_TxPwrSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000C_TxPwrSbiEn_MASK                        0x1
#define D0F0xE4_PHY_000C_RxPwrSbiEn_OFFSET                      1
#define D0F0xE4_PHY_000C_RxPwrSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000C_RxPwrSbiEn_MASK                        0x2
#define D0F0xE4_PHY_000C_PcieModeSbiEn_OFFSET                   2
#define D0F0xE4_PHY_000C_PcieModeSbiEn_WIDTH                    1
#define D0F0xE4_PHY_000C_PcieModeSbiEn_MASK                     0x4
#define D0F0xE4_PHY_000C_FreqDivSbiEn_OFFSET                    3
#define D0F0xE4_PHY_000C_FreqDivSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000C_FreqDivSbiEn_MASK                      0x8
#define D0F0xE4_PHY_000C_DllLockSbiEn_OFFSET                    4
#define D0F0xE4_PHY_000C_DllLockSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000C_DllLockSbiEn_MASK                      0x10
#define D0F0xE4_PHY_000C_OffsetCancelSbiEn_OFFSET               5
#define D0F0xE4_PHY_000C_OffsetCancelSbiEn_WIDTH                1
#define D0F0xE4_PHY_000C_OffsetCancelSbiEn_MASK                 0x20
#define D0F0xE4_PHY_000C_SkipBitSbiEn_OFFSET                    6
#define D0F0xE4_PHY_000C_SkipBitSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000C_SkipBitSbiEn_MASK                      0x40
#define D0F0xE4_PHY_000C_IncoherentClkSbiEn_OFFSET              7
#define D0F0xE4_PHY_000C_IncoherentClkSbiEn_WIDTH               1
#define D0F0xE4_PHY_000C_IncoherentClkSbiEn_MASK                0x80
#define D0F0xE4_PHY_000C_EiDetSbiEn_OFFSET                      8
#define D0F0xE4_PHY_000C_EiDetSbiEn_WIDTH                       1
#define D0F0xE4_PHY_000C_EiDetSbiEn_MASK                        0x100
#define D0F0xE4_PHY_000C_Reserved_13_9_OFFSET                   9
#define D0F0xE4_PHY_000C_Reserved_13_9_WIDTH                    5
#define D0F0xE4_PHY_000C_Reserved_13_9_MASK                     0x3e00
#define D0F0xE4_PHY_000C_MargPktSbiEn_OFFSET                    14
#define D0F0xE4_PHY_000C_MargPktSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000C_MargPktSbiEn_MASK                      0x4000
#define D0F0xE4_PHY_000C_PllCmpPktSbiEn_OFFSET                  15
#define D0F0xE4_PHY_000C_PllCmpPktSbiEn_WIDTH                   1
#define D0F0xE4_PHY_000C_PllCmpPktSbiEn_MASK                    0x8000
#define D0F0xE4_PHY_000C_RxEqPktSbiEn_OFFSET                    16
#define D0F0xE4_PHY_000C_RxEqPktSbiEn_WIDTH                     1
#define D0F0xE4_PHY_000C_RxEqPktSbiEn_MASK                      0x10000
#define D0F0xE4_PHY_000C_TxCoeffPktSbiEn_OFFSET                 17
#define D0F0xE4_PHY_000C_TxCoeffPktSbiEn_WIDTH                  1
#define D0F0xE4_PHY_000C_TxCoeffPktSbiEn_MASK                   0x20000
#define D0F0xE4_PHY_000C_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PHY_000C_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PHY_000C_Reserved_31_18_MASK                    0xfffc0000

/// D0F0xE4_PHY_000C
typedef union {
  struct {                                                              ///<
    UINT32                                               TxPwrSbiEn:1 ; ///<
    UINT32                                               RxPwrSbiEn:1 ; ///<
    UINT32                                            PcieModeSbiEn:1 ; ///<
    UINT32                                             FreqDivSbiEn:1 ; ///<
    UINT32                                             DllLockSbiEn:1 ; ///<
    UINT32                                        OffsetCancelSbiEn:1 ; ///<
    UINT32                                             SkipBitSbiEn:1 ; ///<
    UINT32                                       IncoherentClkSbiEn:1 ; ///<
    UINT32                                               EiDetSbiEn:1 ; ///<
    UINT32                                            Reserved_13_9:5 ; ///<
    UINT32                                             MargPktSbiEn:1 ; ///<
    UINT32                                           PllCmpPktSbiEn:1 ; ///<
    UINT32                                             RxEqPktSbiEn:1 ; ///<
    UINT32                                          TxCoeffPktSbiEn:1 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_000C_STRUCT;

// **** D0F0xE4_PHY_0011 Register Definition ****
// Address
#define D0F0xE4_PHY_0011_ADDRESS                                0x11

// Type
#define D0F0xE4_PHY_0011_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_0011_HonorAllCalReqs_OFFSET                 0
#define D0F0xE4_PHY_0011_HonorAllCalReqs_WIDTH                  1
#define D0F0xE4_PHY_0011_HonorAllCalReqs_MASK                   0x1
#define D0F0xE4_PHY_0011_CodeAdjOnNonRecalEn_OFFSET             1
#define D0F0xE4_PHY_0011_CodeAdjOnNonRecalEn_WIDTH              1
#define D0F0xE4_PHY_0011_CodeAdjOnNonRecalEn_MASK               0x2
#define D0F0xE4_PHY_0011_FilterOutput2En_OFFSET                 2
#define D0F0xE4_PHY_0011_FilterOutput2En_WIDTH                  1
#define D0F0xE4_PHY_0011_FilterOutput2En_MASK                   0x4
#define D0F0xE4_PHY_0011_RonRttLowPwrUpdate_OFFSET              3
#define D0F0xE4_PHY_0011_RonRttLowPwrUpdate_WIDTH               1
#define D0F0xE4_PHY_0011_RonRttLowPwrUpdate_MASK                0x8
#define D0F0xE4_PHY_0011_ContCalEnFuseOverride_OFFSET           4
#define D0F0xE4_PHY_0011_ContCalEnFuseOverride_WIDTH            1
#define D0F0xE4_PHY_0011_ContCalEnFuseOverride_MASK             0x10
#define D0F0xE4_PHY_0011_ContCalEn_OFFSET                       5
#define D0F0xE4_PHY_0011_ContCalEn_WIDTH                        1
#define D0F0xE4_PHY_0011_ContCalEn_MASK                         0x20
#define D0F0xE4_PHY_0011_Reserved_6_6_OFFSET                    6
#define D0F0xE4_PHY_0011_Reserved_6_6_WIDTH                     1
#define D0F0xE4_PHY_0011_Reserved_6_6_MASK                      0x40
#define D0F0xE4_PHY_0011_ContUpdateRttEn_OFFSET                 7
#define D0F0xE4_PHY_0011_ContUpdateRttEn_WIDTH                  1
#define D0F0xE4_PHY_0011_ContUpdateRttEn_MASK                   0x80
#define D0F0xE4_PHY_0011_Reserved_8_8_OFFSET                    8
#define D0F0xE4_PHY_0011_Reserved_8_8_WIDTH                     1
#define D0F0xE4_PHY_0011_Reserved_8_8_MASK                      0x100
#define D0F0xE4_PHY_0011_ContUpdateRonEn_OFFSET                 9
#define D0F0xE4_PHY_0011_ContUpdateRonEn_WIDTH                  1
#define D0F0xE4_PHY_0011_ContUpdateRonEn_MASK                   0x200
#define D0F0xE4_PHY_0011_Reserved_10_10_OFFSET                  10
#define D0F0xE4_PHY_0011_Reserved_10_10_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_10_10_MASK                    0x400
#define D0F0xE4_PHY_0011_ContUpdateDeempEn_OFFSET               11
#define D0F0xE4_PHY_0011_ContUpdateDeempEn_WIDTH                1
#define D0F0xE4_PHY_0011_ContUpdateDeempEn_MASK                 0x800
#define D0F0xE4_PHY_0011_Reserved_12_12_OFFSET                  12
#define D0F0xE4_PHY_0011_Reserved_12_12_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_12_12_MASK                    0x1000
#define D0F0xE4_PHY_0011_CmpSwapSel_OFFSET                      13
#define D0F0xE4_PHY_0011_CmpSwapSel_WIDTH                       2
#define D0F0xE4_PHY_0011_CmpSwapSel_MASK                        0x6000
#define D0F0xE4_PHY_0011_Reserved_15_15_OFFSET                  15
#define D0F0xE4_PHY_0011_Reserved_15_15_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_15_15_MASK                    0x8000
#define D0F0xE4_PHY_0011_CmpSampleSel_OFFSET                    16
#define D0F0xE4_PHY_0011_CmpSampleSel_WIDTH                     2
#define D0F0xE4_PHY_0011_CmpSampleSel_MASK                      0x30000
#define D0F0xE4_PHY_0011_Reserved_18_18_OFFSET                  18
#define D0F0xE4_PHY_0011_Reserved_18_18_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_18_18_MASK                    0x40000
#define D0F0xE4_PHY_0011_CmpaCmpCode_OFFSET                     19
#define D0F0xE4_PHY_0011_CmpaCmpCode_WIDTH                      5
#define D0F0xE4_PHY_0011_CmpaCmpCode_MASK                       0xf80000
#define D0F0xE4_PHY_0011_Reserved_24_24_OFFSET                  24
#define D0F0xE4_PHY_0011_Reserved_24_24_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_24_24_MASK                    0x1000000
#define D0F0xE4_PHY_0011_CalRttEn_OFFSET                        25
#define D0F0xE4_PHY_0011_CalRttEn_WIDTH                         1
#define D0F0xE4_PHY_0011_CalRttEn_MASK                          0x2000000
#define D0F0xE4_PHY_0011_CmpaCmpBase_OFFSET                     26
#define D0F0xE4_PHY_0011_CmpaCmpBase_WIDTH                      1
#define D0F0xE4_PHY_0011_CmpaCmpBase_MASK                       0x4000000
#define D0F0xE4_PHY_0011_Reserved_27_27_OFFSET                  27
#define D0F0xE4_PHY_0011_Reserved_27_27_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_27_27_MASK                    0x8000000
#define D0F0xE4_PHY_0011_CmpaCmpCodeOverride_OFFSET             28
#define D0F0xE4_PHY_0011_CmpaCmpCodeOverride_WIDTH              1
#define D0F0xE4_PHY_0011_CmpaCmpCodeOverride_MASK               0x10000000
#define D0F0xE4_PHY_0011_Reserved_29_29_OFFSET                  29
#define D0F0xE4_PHY_0011_Reserved_29_29_WIDTH                   1
#define D0F0xE4_PHY_0011_Reserved_29_29_MASK                    0x20000000
#define D0F0xE4_PHY_0011_RextLow_OFFSET                         30
#define D0F0xE4_PHY_0011_RextLow_WIDTH                          1
#define D0F0xE4_PHY_0011_RextLow_MASK                           0x40000000
#define D0F0xE4_PHY_0011_CmpTestClkEn_OFFSET                    31
#define D0F0xE4_PHY_0011_CmpTestClkEn_WIDTH                     1
#define D0F0xE4_PHY_0011_CmpTestClkEn_MASK                      0x80000000

/// D0F0xE4_PHY_0011
typedef union {
  struct {                                                              ///<
    UINT32                                          HonorAllCalReqs:1 ; ///<
    UINT32                                      CodeAdjOnNonRecalEn:1 ; ///<
    UINT32                                          FilterOutput2En:1 ; ///<
    UINT32                                       RonRttLowPwrUpdate:1 ; ///<
    UINT32                                    ContCalEnFuseOverride:1 ; ///<
    UINT32                                                ContCalEn:1 ; ///<
    UINT32                                             Reserved_6_6:1 ; ///<
    UINT32                                          ContUpdateRttEn:1 ; ///<
    UINT32                                             Reserved_8_8:1 ; ///<
    UINT32                                          ContUpdateRonEn:1 ; ///<
    UINT32                                           Reserved_10_10:1 ; ///<
    UINT32                                        ContUpdateDeempEn:1 ; ///<
    UINT32                                           Reserved_12_12:1 ; ///<
    UINT32                                               CmpSwapSel:2 ; ///<
    UINT32                                           Reserved_15_15:1 ; ///<
    UINT32                                             CmpSampleSel:2 ; ///<
    UINT32                                           Reserved_18_18:1 ; ///<
    UINT32                                              CmpaCmpCode:5 ; ///<
    UINT32                                           Reserved_24_24:1 ; ///<
    UINT32                                                 CalRttEn:1 ; ///<
    UINT32                                              CmpaCmpBase:1 ; ///<
    UINT32                                           Reserved_27_27:1 ; ///<
    UINT32                                      CmpaCmpCodeOverride:1 ; ///<
    UINT32                                           Reserved_29_29:1 ; ///<
    UINT32                                                  RextLow:1 ; ///<
    UINT32                                             CmpTestClkEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_0011_STRUCT;

// **** D0F0xE4_PHY_2012 Register Definition ****
// Address
#define D0F0xE4_PHY_2012_ADDRESS                                0x2012

// Type
#define D0F0xE4_PHY_2012_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_2012_RefClkSelSub0_OFFSET                   0
#define D0F0xE4_PHY_2012_RefClkSelSub0_WIDTH                    2
#define D0F0xE4_PHY_2012_RefClkSelSub0_MASK                     0x3
#define D0F0xE4_PHY_2012_Reserved_3_2_OFFSET                    2
#define D0F0xE4_PHY_2012_Reserved_3_2_WIDTH                     2
#define D0F0xE4_PHY_2012_Reserved_3_2_MASK                      0xc
#define D0F0xE4_PHY_2012_PcieModeSub0_OFFSET                    4
#define D0F0xE4_PHY_2012_PcieModeSub0_WIDTH                     1
#define D0F0xE4_PHY_2012_PcieModeSub0_MASK                      0x10
#define D0F0xE4_PHY_2012_Reserved_5_5_OFFSET                    5
#define D0F0xE4_PHY_2012_Reserved_5_5_WIDTH                     1
#define D0F0xE4_PHY_2012_Reserved_5_5_MASK                      0x20
#define D0F0xE4_PHY_2012_PllSrcSub0_OFFSET                      6
#define D0F0xE4_PHY_2012_PllSrcSub0_WIDTH                       1
#define D0F0xE4_PHY_2012_PllSrcSub0_MASK                        0x40
#define D0F0xE4_PHY_2012_Reserved_7_7_OFFSET                    7
#define D0F0xE4_PHY_2012_Reserved_7_7_WIDTH                     1
#define D0F0xE4_PHY_2012_Reserved_7_7_MASK                      0x80
#define D0F0xE4_PHY_2012_PllSelOvrdSub0_OFFSET                  8
#define D0F0xE4_PHY_2012_PllSelOvrdSub0_WIDTH                   2
#define D0F0xE4_PHY_2012_PllSelOvrdSub0_MASK                    0x300
#define D0F0xE4_PHY_2012_PllSelOvrdEnSub0_OFFSET                10
#define D0F0xE4_PHY_2012_PllSelOvrdEnSub0_WIDTH                 1
#define D0F0xE4_PHY_2012_PllSelOvrdEnSub0_MASK                  0x400
#define D0F0xE4_PHY_2012_Reserved_31_11_OFFSET                  11
#define D0F0xE4_PHY_2012_Reserved_31_11_WIDTH                   21
#define D0F0xE4_PHY_2012_Reserved_31_11_MASK                    0xfffff800

/// D0F0xE4_PHY_2012
typedef union {
  struct {                                                              ///<
    UINT32                                            RefClkSelSub0:2 ; ///<
    UINT32                                             Reserved_3_2:2 ; ///<
    UINT32                                             PcieModeSub0:1 ; ///<
    UINT32                                             Reserved_5_5:1 ; ///<
    UINT32                                               PllSrcSub0:1 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                           PllSelOvrdSub0:2 ; ///<
    UINT32                                         PllSelOvrdEnSub0:1 ; ///<
    UINT32                                           Reserved_31_11:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_2012_STRUCT;

// **** D0F0xE4_PHY_2013 Register Definition ****
// Address
#define D0F0xE4_PHY_2013_ADDRESS                                0x2013

// Type
#define D0F0xE4_PHY_2013_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_2013_RefClkSelSub1_OFFSET                   0
#define D0F0xE4_PHY_2013_RefClkSelSub1_WIDTH                    2
#define D0F0xE4_PHY_2013_RefClkSelSub1_MASK                     0x3
#define D0F0xE4_PHY_2013_Reserved_3_2_OFFSET                    2
#define D0F0xE4_PHY_2013_Reserved_3_2_WIDTH                     2
#define D0F0xE4_PHY_2013_Reserved_3_2_MASK                      0xc
#define D0F0xE4_PHY_2013_PcieModeSub1_OFFSET                    4
#define D0F0xE4_PHY_2013_PcieModeSub1_WIDTH                     1
#define D0F0xE4_PHY_2013_PcieModeSub1_MASK                      0x10
#define D0F0xE4_PHY_2013_Reserved_5_5_OFFSET                    5
#define D0F0xE4_PHY_2013_Reserved_5_5_WIDTH                     1
#define D0F0xE4_PHY_2013_Reserved_5_5_MASK                      0x20
#define D0F0xE4_PHY_2013_PllSrcSub1_OFFSET                      6
#define D0F0xE4_PHY_2013_PllSrcSub1_WIDTH                       1
#define D0F0xE4_PHY_2013_PllSrcSub1_MASK                        0x40
#define D0F0xE4_PHY_2013_Reserved_7_7_OFFSET                    7
#define D0F0xE4_PHY_2013_Reserved_7_7_WIDTH                     1
#define D0F0xE4_PHY_2013_Reserved_7_7_MASK                      0x80
#define D0F0xE4_PHY_2013_PllSelOvrdSub1_OFFSET                  8
#define D0F0xE4_PHY_2013_PllSelOvrdSub1_WIDTH                   2
#define D0F0xE4_PHY_2013_PllSelOvrdSub1_MASK                    0x300
#define D0F0xE4_PHY_2013_PllSelOvrdEnSub1_OFFSET                10
#define D0F0xE4_PHY_2013_PllSelOvrdEnSub1_WIDTH                 1
#define D0F0xE4_PHY_2013_PllSelOvrdEnSub1_MASK                  0x400
#define D0F0xE4_PHY_2013_Reserved_31_11_OFFSET                  11
#define D0F0xE4_PHY_2013_Reserved_31_11_WIDTH                   21
#define D0F0xE4_PHY_2013_Reserved_31_11_MASK                    0xfffff800

/// D0F0xE4_PHY_2013
typedef union {
  struct {                                                              ///<
    UINT32                                            RefClkSelSub1:2 ; ///<
    UINT32                                             Reserved_3_2:2 ; ///<
    UINT32                                             PcieModeSub1:1 ; ///<
    UINT32                                             Reserved_5_5:1 ; ///<
    UINT32                                               PllSrcSub1:1 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                           PllSelOvrdSub1:2 ; ///<
    UINT32                                         PllSelOvrdEnSub1:1 ; ///<
    UINT32                                           Reserved_31_11:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_2013_STRUCT;

// **** D0F0xE4_PHY_4002 Register Definition ****
// Address
#define D0F0xE4_PHY_4002_ADDRESS                                0x4002

// Type
#define D0F0xE4_PHY_4002_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_4002_PhaseGainGen12Sel_OFFSET               0
#define D0F0xE4_PHY_4002_PhaseGainGen12Sel_WIDTH                4
#define D0F0xE4_PHY_4002_PhaseGainGen12Sel_MASK                 0xf
#define D0F0xE4_PHY_4002_PhaseGainGen3Sel_OFFSET                4
#define D0F0xE4_PHY_4002_PhaseGainGen3Sel_WIDTH                 4
#define D0F0xE4_PHY_4002_PhaseGainGen3Sel_MASK                  0xf0
#define D0F0xE4_PHY_4002_StallDetCtl_OFFSET                     8
#define D0F0xE4_PHY_4002_StallDetCtl_WIDTH                      2
#define D0F0xE4_PHY_4002_StallDetCtl_MASK                       0x300
#define D0F0xE4_PHY_4002_Reserved_10_10_OFFSET                  10
#define D0F0xE4_PHY_4002_Reserved_10_10_WIDTH                   1
#define D0F0xE4_PHY_4002_Reserved_10_10_MASK                    0x400
#define D0F0xE4_PHY_4002_LoopFilterTypeSel_OFFSET               11
#define D0F0xE4_PHY_4002_LoopFilterTypeSel_WIDTH                1
#define D0F0xE4_PHY_4002_LoopFilterTypeSel_MASK                 0x800
#define D0F0xE4_PHY_4002_Reserved_13_12_OFFSET                  12
#define D0F0xE4_PHY_4002_Reserved_13_12_WIDTH                   2
#define D0F0xE4_PHY_4002_Reserved_13_12_MASK                    0x3000
#define D0F0xE4_PHY_4002_LfcMin_OFFSET                          14
#define D0F0xE4_PHY_4002_LfcMin_WIDTH                           8
#define D0F0xE4_PHY_4002_LfcMin_MASK                            0x3fc000
#define D0F0xE4_PHY_4002_LfcMin_VALUE                           0x0
#define D0F0xE4_PHY_4002_LfcMax_OFFSET                          22
#define D0F0xE4_PHY_4002_LfcMax_WIDTH                           8
#define D0F0xE4_PHY_4002_LfcMax_MASK                            0x3fc00000
#define D0F0xE4_PHY_4002_LfcMax_VALUE                           0x8
#define D0F0xE4_PHY_4002_LoopBw2xEnGen3_OFFSET                  30
#define D0F0xE4_PHY_4002_LoopBw2xEnGen3_WIDTH                   1
#define D0F0xE4_PHY_4002_LoopBw2xEnGen3_MASK                    0x40000000
#define D0F0xE4_PHY_4002_LoopBw2xEnGen12_OFFSET                 31
#define D0F0xE4_PHY_4002_LoopBw2xEnGen12_WIDTH                  1
#define D0F0xE4_PHY_4002_LoopBw2xEnGen12_MASK                   0x80000000

/// D0F0xE4_PHY_4002
typedef union {
  struct {                                                              ///<
    UINT32                                        PhaseGainGen12Sel:4 ; ///<
    UINT32                                         PhaseGainGen3Sel:4 ; ///<
    UINT32                                              StallDetCtl:2 ; ///<
    UINT32                                           Reserved_10_10:1 ; ///<
    UINT32                                        LoopFilterTypeSel:1 ; ///<
    UINT32                                           Reserved_13_12:2 ; ///<
    UINT32                                                   LfcMin:8 ; ///<
    UINT32                                                   LfcMax:8 ; ///<
    UINT32                                           LoopBw2xEnGen3:1 ; ///<
    UINT32                                          LoopBw2xEnGen12:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_4002_STRUCT;

// **** D0F0xE4_PHY_4006 Register Definition ****
// Address
#define D0F0xE4_PHY_4006_ADDRESS                                0x4006

// Type
#define D0F0xE4_PHY_4006_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_4006_LeqEnSel_OFFSET                        0
#define D0F0xE4_PHY_4006_LeqEnSel_WIDTH                         1
#define D0F0xE4_PHY_4006_LeqEnSel_MASK                          0x1
#define D0F0xE4_PHY_4006_LeqEnFuseOverride_OFFSET               1
#define D0F0xE4_PHY_4006_LeqEnFuseOverride_WIDTH                1
#define D0F0xE4_PHY_4006_LeqEnFuseOverride_MASK                 0x2
#define D0F0xE4_PHY_4006_Reserved_2_2_OFFSET                    2
#define D0F0xE4_PHY_4006_Reserved_2_2_WIDTH                     1
#define D0F0xE4_PHY_4006_Reserved_2_2_MASK                      0x4
#define D0F0xE4_PHY_4006_DfrDis_OFFSET                          3
#define D0F0xE4_PHY_4006_DfrDis_WIDTH                           1
#define D0F0xE4_PHY_4006_DfrDis_MASK                            0x8
#define D0F0xE4_PHY_4006_DfrForce_OFFSET                        4
#define D0F0xE4_PHY_4006_DfrForce_WIDTH                         1
#define D0F0xE4_PHY_4006_DfrForce_MASK                          0x10
#define D0F0xE4_PHY_4006_DfeVoltage_OFFSET                      5
#define D0F0xE4_PHY_4006_DfeVoltage_WIDTH                       2
#define D0F0xE4_PHY_4006_DfeVoltage_MASK                        0x60
#define D0F0xE4_PHY_4006_Reserved_7_7_OFFSET                    7
#define D0F0xE4_PHY_4006_Reserved_7_7_WIDTH                     1
#define D0F0xE4_PHY_4006_Reserved_7_7_MASK                      0x80
#define D0F0xE4_PHY_4006_DfeEn_OFFSET                           8
#define D0F0xE4_PHY_4006_DfeEn_WIDTH                            1
#define D0F0xE4_PHY_4006_DfeEn_MASK                             0x100
#define D0F0xE4_PHY_4006_VdcDacTri_OFFSET                       9
#define D0F0xE4_PHY_4006_VdcDacTri_WIDTH                        1
#define D0F0xE4_PHY_4006_VdcDacTri_MASK                         0x200
#define D0F0xE4_PHY_4006_TxDiff_OFFSET                          10
#define D0F0xE4_PHY_4006_TxDiff_WIDTH                           8
#define D0F0xE4_PHY_4006_TxDiff_MASK                            0x3fc00
#define D0F0xE4_PHY_4006_Reserved_20_18_OFFSET                  18
#define D0F0xE4_PHY_4006_Reserved_20_18_WIDTH                   3
#define D0F0xE4_PHY_4006_Reserved_20_18_MASK                    0x1c0000
#define D0F0xE4_PHY_4006_DfrTimerCtl_OFFSET                     22
#define D0F0xE4_PHY_4006_DfrTimerCtl_WIDTH                      2
#define D0F0xE4_PHY_4006_DfrTimerCtl_MASK                       0x600000
#define D0F0xE4_PHY_4006_Reserved_31_23_OFFSET                  23
#define D0F0xE4_PHY_4006_Reserved_31_23_WIDTH                   9
#define D0F0xE4_PHY_4006_Reserved_31_23_MASK                    0xff800000

/// D0F0xE4_PHY_4006
typedef union {
  struct {                                                              ///<
    UINT32                                                 LeqEnSel:1 ; ///<
    UINT32                                        LeqEnFuseOverride:1 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                                   DfrDis:1 ; ///<
    UINT32                                                 DfrForce:1 ; ///<
    UINT32                                               DfeVoltage:2 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                                    DfeEn:1 ; ///<
    UINT32                                                VdcDacTri:1 ; ///<
    UINT32                                                   TxDiff:8 ; ///<
    UINT32                                           Reserved_20_18:3 ; ///<
    UINT32                                              DfrTimerCtl:2 ; ///<
    UINT32                                           Reserved_31_23:9 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_4006_STRUCT;

// **** D0F0xE4_PHY_400A Register Definition ****
// Address
#define D0F0xE4_PHY_400A_ADDRESS                                0x400a

// Type
#define D0F0xE4_PHY_400A_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_400A_Reserved_0_0_OFFSET                    0
#define D0F0xE4_PHY_400A_Reserved_0_0_WIDTH                     1
#define D0F0xE4_PHY_400A_Reserved_0_0_MASK                      0x1
#define D0F0xE4_PHY_400A_RxPcieMode_OFFSET                      1
#define D0F0xE4_PHY_400A_RxPcieMode_WIDTH                       2
#define D0F0xE4_PHY_400A_RxPcieMode_MASK                        0x6
#define D0F0xE4_PHY_400A_Reserved_5_3_OFFSET                    3
#define D0F0xE4_PHY_400A_Reserved_5_3_WIDTH                     3
#define D0F0xE4_PHY_400A_Reserved_5_3_MASK                      0x38
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypVal_OFFSET             6
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypVal_WIDTH              5
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypVal_MASK               0x7c0
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypEn_OFFSET              11
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypEn_WIDTH               1
#define D0F0xE4_PHY_400A_Gen12PiOffsetBypEn_MASK                0x800
#define D0F0xE4_PHY_400A_Reserved_12_12_OFFSET                  12
#define D0F0xE4_PHY_400A_Reserved_12_12_WIDTH                   1
#define D0F0xE4_PHY_400A_Reserved_12_12_MASK                    0x1000
#define D0F0xE4_PHY_400A_AnalogWaitTime_OFFSET                  13
#define D0F0xE4_PHY_400A_AnalogWaitTime_WIDTH                   2
#define D0F0xE4_PHY_400A_AnalogWaitTime_MASK                    0x6000
#define D0F0xE4_PHY_400A_Reserved_16_15_OFFSET                  15
#define D0F0xE4_PHY_400A_Reserved_16_15_WIDTH                   2
#define D0F0xE4_PHY_400A_Reserved_16_15_MASK                    0x18000
#define D0F0xE4_PHY_400A_DllLockFastModeEn_OFFSET               17
#define D0F0xE4_PHY_400A_DllLockFastModeEn_WIDTH                1
#define D0F0xE4_PHY_400A_DllLockFastModeEn_MASK                 0x20000
#define D0F0xE4_PHY_400A_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PHY_400A_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PHY_400A_Reserved_31_18_MASK                    0xfffc0000

/// D0F0xE4_PHY_400A
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                               RxPcieMode:2 ; ///<
    UINT32                                             Reserved_5_3:3 ; ///<
    UINT32                                      Gen12PiOffsetBypVal:5 ; ///<
    UINT32                                       Gen12PiOffsetBypEn:1 ; ///<
    UINT32                                           Reserved_12_12:1 ; ///<
    UINT32                                           AnalogWaitTime:2 ; ///<
    UINT32                                           Reserved_16_15:2 ; ///<
    UINT32                                        DllLockFastModeEn:1 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_400A_STRUCT;

// **** D0F0xE4_PHY_4019 Register Definition ****
// Address
#define D0F0xE4_PHY_4019_ADDRESS                                0x4019

// Type
#define D0F0xE4_PHY_4019_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_4019_Reserved_17_0_OFFSET                   0
#define D0F0xE4_PHY_4019_Reserved_17_0_WIDTH                    18
#define D0F0xE4_PHY_4019_Reserved_17_0_MASK                     0x3ffff
#define D0F0xE4_PHY_4019_RxAdaptMode_0__OFFSET                  18
#define D0F0xE4_PHY_4019_RxAdaptMode_0__WIDTH                   1
#define D0F0xE4_PHY_4019_RxAdaptMode_0__MASK                    0x40000
#define D0F0xE4_PHY_4019_RxAdaptMode_3_1__OFFSET                19
#define D0F0xE4_PHY_4019_RxAdaptMode_3_1__WIDTH                 3
#define D0F0xE4_PHY_4019_RxAdaptMode_3_1__MASK                  0x380000
#define D0F0xE4_PHY_4019_RxAdaptMode_5_4__OFFSET                22
#define D0F0xE4_PHY_4019_RxAdaptMode_5_4__WIDTH                 2
#define D0F0xE4_PHY_4019_RxAdaptMode_5_4__MASK                  0xc00000
#define D0F0xE4_PHY_4019_RxAdaptMode_6__OFFSET                  24
#define D0F0xE4_PHY_4019_RxAdaptMode_6__WIDTH                   1
#define D0F0xE4_PHY_4019_RxAdaptMode_6__MASK                    0x1000000
#define D0F0xE4_PHY_4019_RxAdaptMode_7__OFFSET                  25
#define D0F0xE4_PHY_4019_RxAdaptMode_7__WIDTH                   1
#define D0F0xE4_PHY_4019_RxAdaptMode_7__MASK                    0x2000000
#define D0F0xE4_PHY_4019_RxAdaptMode_8__OFFSET                  26
#define D0F0xE4_PHY_4019_RxAdaptMode_8__WIDTH                   1
#define D0F0xE4_PHY_4019_RxAdaptMode_8__MASK                    0x4000000
#define D0F0xE4_PHY_4019_RxAdaptMode_9__OFFSET                  27
#define D0F0xE4_PHY_4019_RxAdaptMode_9__WIDTH                   1
#define D0F0xE4_PHY_4019_RxAdaptMode_9__MASK                    0x8000000
#define D0F0xE4_PHY_4019_Reserved_31_28_OFFSET                  28
#define D0F0xE4_PHY_4019_Reserved_31_28_WIDTH                   4
#define D0F0xE4_PHY_4019_Reserved_31_28_MASK                    0xf0000000

/// D0F0xE4_PHY_4019
typedef union {
  struct {                                                              ///<
    UINT32                                            Reserved_17_0:18; ///<
    UINT32                                           RxAdaptMode_0_:1 ; ///<
    UINT32                                         RxAdaptMode_3_1_:3 ; ///<
    UINT32                                         RxAdaptMode_5_4_:2 ; ///<
    UINT32                                           RxAdaptMode_6_:1 ; ///<
    UINT32                                           RxAdaptMode_7_:1 ; ///<
    UINT32                                           RxAdaptMode_8_:1 ; ///<
    UINT32                                           RxAdaptMode_9_:1 ; ///<
    UINT32                                           Reserved_31_28:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_4019_STRUCT;

// **** D0F0xE4_PHY_401A Register Definition ****
// Address
#define D0F0xE4_PHY_401A_ADDRESS                                0x401a

// Type
#define D0F0xE4_PHY_401A_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_401A_Reserved_9_0_OFFSET                    0
#define D0F0xE4_PHY_401A_Reserved_9_0_WIDTH                     10
#define D0F0xE4_PHY_401A_Reserved_9_0_MASK                      0x3ff
#define D0F0xE4_PHY_401A_ForceAdaptRst_OFFSET                   10
#define D0F0xE4_PHY_401A_ForceAdaptRst_WIDTH                    1
#define D0F0xE4_PHY_401A_ForceAdaptRst_MASK                     0x400
#define D0F0xE4_PHY_401A_ForceDiv8En_OFFSET                     11
#define D0F0xE4_PHY_401A_ForceDiv8En_WIDTH                      1
#define D0F0xE4_PHY_401A_ForceDiv8En_MASK                       0x800
#define D0F0xE4_PHY_401A_ForceAdaptEn_OFFSET                    12
#define D0F0xE4_PHY_401A_ForceAdaptEn_WIDTH                     1
#define D0F0xE4_PHY_401A_ForceAdaptEn_MASK                      0x1000
#define D0F0xE4_PHY_401A_RxFOMValid_OFFSET                      13
#define D0F0xE4_PHY_401A_RxFOMValid_WIDTH                       1
#define D0F0xE4_PHY_401A_RxFOMValid_MASK                        0x2000
#define D0F0xE4_PHY_401A_RxEyeFOM_OFFSET                        14
#define D0F0xE4_PHY_401A_RxEyeFOM_WIDTH                         8
#define D0F0xE4_PHY_401A_RxEyeFOM_MASK                          0x3fc000
#define D0F0xE4_PHY_401A_Reserved_24_22_OFFSET                  22
#define D0F0xE4_PHY_401A_Reserved_24_22_WIDTH                   3
#define D0F0xE4_PHY_401A_Reserved_24_22_MASK                    0x1c00000
#define D0F0xE4_PHY_401A_ResponseMode_OFFSET                    25
#define D0F0xE4_PHY_401A_ResponseMode_WIDTH                     1
#define D0F0xE4_PHY_401A_ResponseMode_MASK                      0x2000000
#define D0F0xE4_PHY_401A_RequestFOM_OFFSET                      26
#define D0F0xE4_PHY_401A_RequestFOM_WIDTH                       1
#define D0F0xE4_PHY_401A_RequestFOM_MASK                        0x4000000
#define D0F0xE4_PHY_401A_EnableFOM_OFFSET                       27
#define D0F0xE4_PHY_401A_EnableFOM_WIDTH                        1
#define D0F0xE4_PHY_401A_EnableFOM_MASK                         0x8000000
#define D0F0xE4_PHY_401A_Reserved_29_28_OFFSET                  28
#define D0F0xE4_PHY_401A_Reserved_29_28_WIDTH                   2
#define D0F0xE4_PHY_401A_Reserved_29_28_MASK                    0x30000000
#define D0F0xE4_PHY_401A_RxAdaptTrack_OFFSET                    30
#define D0F0xE4_PHY_401A_RxAdaptTrack_WIDTH                     2
#define D0F0xE4_PHY_401A_RxAdaptTrack_MASK                      0xc0000000

/// D0F0xE4_PHY_401A
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_9_0:10; ///<
    UINT32                                            ForceAdaptRst:1 ; ///<
    UINT32                                              ForceDiv8En:1 ; ///<
    UINT32                                             ForceAdaptEn:1 ; ///<
    UINT32                                               RxFOMValid:1 ; ///<
    UINT32                                                 RxEyeFOM:8 ; ///<
    UINT32                                           Reserved_24_22:3 ; ///<
    UINT32                                             ResponseMode:1 ; ///<
    UINT32                                               RequestFOM:1 ; ///<
    UINT32                                                EnableFOM:1 ; ///<
    UINT32                                           Reserved_29_28:2 ; ///<
    UINT32                                             RxAdaptTrack:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_401A_STRUCT;

// **** D0F0xE4_PHY_401D Register Definition ****
// Address
#define D0F0xE4_PHY_401D_ADDRESS                                0x401d

// Type
#define D0F0xE4_PHY_401D_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_401D_Reserved_1_0_OFFSET                    0
#define D0F0xE4_PHY_401D_Reserved_1_0_WIDTH                     2
#define D0F0xE4_PHY_401D_Reserved_1_0_MASK                      0x3
#define D0F0xE4_PHY_401D_Gen3OffCorrMethod_OFFSET               2
#define D0F0xE4_PHY_401D_Gen3OffCorrMethod_WIDTH                1
#define D0F0xE4_PHY_401D_Gen3OffCorrMethod_MASK                 0x4
#define D0F0xE4_PHY_401D_Reserved_23_3_OFFSET                   3
#define D0F0xE4_PHY_401D_Reserved_23_3_WIDTH                    21
#define D0F0xE4_PHY_401D_Reserved_23_3_MASK                     0xfffff8
#define D0F0xE4_PHY_401D_OffsetSampleNDfeIndex_OFFSET           24
#define D0F0xE4_PHY_401D_OffsetSampleNDfeIndex_WIDTH            7
#define D0F0xE4_PHY_401D_OffsetSampleNDfeIndex_MASK             0x7f000000
#define D0F0xE4_PHY_401D_OffsetSampleNDfeCtrl_OFFSET            31
#define D0F0xE4_PHY_401D_OffsetSampleNDfeCtrl_WIDTH             1
#define D0F0xE4_PHY_401D_OffsetSampleNDfeCtrl_MASK              0x80000000

/// D0F0xE4_PHY_401D
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_1_0:2 ; ///<
    UINT32                                        Gen3OffCorrMethod:1 ; ///<
    UINT32                                            Reserved_23_3:21; ///<
    UINT32                                    OffsetSampleNDfeIndex:7 ; ///<
    UINT32                                     OffsetSampleNDfeCtrl:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_401D_STRUCT;

// **** D0F0xE4_PHY_4020 Register Definition ****
// Address
#define D0F0xE4_PHY_4020_ADDRESS                                0x4020

// Type
#define D0F0xE4_PHY_4020_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_4020_Reserved_3_0_OFFSET                    0
#define D0F0xE4_PHY_4020_Reserved_3_0_WIDTH                     4
#define D0F0xE4_PHY_4020_Reserved_3_0_MASK                      0xf
#define D0F0xE4_PHY_4020_AdaptFomBer_OFFSET                     4
#define D0F0xE4_PHY_4020_AdaptFomBer_WIDTH                      3
#define D0F0xE4_PHY_4020_AdaptFomBer_MASK                       0x70
#define D0F0xE4_PHY_4020_Reserved_7_7_OFFSET                    7
#define D0F0xE4_PHY_4020_Reserved_7_7_WIDTH                     1
#define D0F0xE4_PHY_4020_Reserved_7_7_MASK                      0x80
#define D0F0xE4_PHY_4020_AdaptOffCalTime_OFFSET                 8
#define D0F0xE4_PHY_4020_AdaptOffCalTime_WIDTH                  4
#define D0F0xE4_PHY_4020_AdaptOffCalTime_MASK                   0xf00
#define D0F0xE4_PHY_4020_Reserved_12_12_OFFSET                  12
#define D0F0xE4_PHY_4020_Reserved_12_12_WIDTH                   1
#define D0F0xE4_PHY_4020_Reserved_12_12_MASK                    0x1000
#define D0F0xE4_PHY_4020_AdaptCDRTime_OFFSET                    13
#define D0F0xE4_PHY_4020_AdaptCDRTime_WIDTH                     4
#define D0F0xE4_PHY_4020_AdaptCDRTime_MASK                      0x1e000
#define D0F0xE4_PHY_4020_Reserved_17_17_OFFSET                  17
#define D0F0xE4_PHY_4020_Reserved_17_17_WIDTH                   1
#define D0F0xE4_PHY_4020_Reserved_17_17_MASK                    0x20000
#define D0F0xE4_PHY_4020_AdaptLEQTime_OFFSET                    18
#define D0F0xE4_PHY_4020_AdaptLEQTime_WIDTH                     4
#define D0F0xE4_PHY_4020_AdaptLEQTime_MASK                      0x3c0000
#define D0F0xE4_PHY_4020_Reserved_22_22_OFFSET                  22
#define D0F0xE4_PHY_4020_Reserved_22_22_WIDTH                   1
#define D0F0xE4_PHY_4020_Reserved_22_22_MASK                    0x400000
#define D0F0xE4_PHY_4020_AdaptDFETime_OFFSET                    23
#define D0F0xE4_PHY_4020_AdaptDFETime_WIDTH                     4
#define D0F0xE4_PHY_4020_AdaptDFETime_MASK                      0x7800000
#define D0F0xE4_PHY_4020_Reserved_27_27_OFFSET                  27
#define D0F0xE4_PHY_4020_Reserved_27_27_WIDTH                   1
#define D0F0xE4_PHY_4020_Reserved_27_27_MASK                    0x8000000
#define D0F0xE4_PHY_4020_AdaptFOMTime_OFFSET                    28
#define D0F0xE4_PHY_4020_AdaptFOMTime_WIDTH                     4
#define D0F0xE4_PHY_4020_AdaptFOMTime_MASK                      0xf0000000

/// D0F0xE4_PHY_4020
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_3_0:4 ; ///<
    UINT32                                              AdaptFomBer:3 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                          AdaptOffCalTime:4 ; ///<
    UINT32                                           Reserved_12_12:1 ; ///<
    UINT32                                             AdaptCDRTime:4 ; ///<
    UINT32                                           Reserved_17_17:1 ; ///<
    UINT32                                             AdaptLEQTime:4 ; ///<
    UINT32                                           Reserved_22_22:1 ; ///<
    UINT32                                             AdaptDFETime:4 ; ///<
    UINT32                                           Reserved_27_27:1 ; ///<
    UINT32                                             AdaptFOMTime:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_4020_STRUCT;

// **** D0F0xE4_PHY_4021 Register Definition ****
// Address
#define D0F0xE4_PHY_4021_ADDRESS                                0x4021

// Type
#define D0F0xE4_PHY_4021_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_4021_Reserved_1_0_OFFSET                    0
#define D0F0xE4_PHY_4021_Reserved_1_0_WIDTH                     2
#define D0F0xE4_PHY_4021_Reserved_1_0_MASK                      0x3
#define D0F0xE4_PHY_4021_AdaptPowerSaveOff_OFFSET               2
#define D0F0xE4_PHY_4021_AdaptPowerSaveOff_WIDTH                1
#define D0F0xE4_PHY_4021_AdaptPowerSaveOff_MASK                 0x4
#define D0F0xE4_PHY_4021_Reserved_3_3_OFFSET                    3
#define D0F0xE4_PHY_4021_Reserved_3_3_WIDTH                     1
#define D0F0xE4_PHY_4021_Reserved_3_3_MASK                      0x8
#define D0F0xE4_PHY_4021_AdaptPowerDownTime_OFFSET              4
#define D0F0xE4_PHY_4021_AdaptPowerDownTime_WIDTH               2
#define D0F0xE4_PHY_4021_AdaptPowerDownTime_MASK                0x30
#define D0F0xE4_PHY_4021_Reserved_6_6_OFFSET                    6
#define D0F0xE4_PHY_4021_Reserved_6_6_WIDTH                     1
#define D0F0xE4_PHY_4021_Reserved_6_6_MASK                      0x40
#define D0F0xE4_PHY_4021_AdaptPiOffsetbypVal_OFFSET             7
#define D0F0xE4_PHY_4021_AdaptPiOffsetbypVal_WIDTH              5
#define D0F0xE4_PHY_4021_AdaptPiOffsetbypVal_MASK               0xf80
#define D0F0xE4_PHY_4021_AdaptPiOffsetBypEn_OFFSET              12
#define D0F0xE4_PHY_4021_AdaptPiOffsetBypEn_WIDTH               1
#define D0F0xE4_PHY_4021_AdaptPiOffsetBypEn_MASK                0x1000
#define D0F0xE4_PHY_4021_Reserved_13_13_OFFSET                  13
#define D0F0xE4_PHY_4021_Reserved_13_13_WIDTH                   1
#define D0F0xE4_PHY_4021_Reserved_13_13_MASK                    0x2000
#define D0F0xE4_PHY_4021_AdaptRxDatGateDis_OFFSET               14
#define D0F0xE4_PHY_4021_AdaptRxDatGateDis_WIDTH                1
#define D0F0xE4_PHY_4021_AdaptRxDatGateDis_MASK                 0x4000
#define D0F0xE4_PHY_4021_Reserved_16_15_OFFSET                  15
#define D0F0xE4_PHY_4021_Reserved_16_15_WIDTH                   2
#define D0F0xE4_PHY_4021_Reserved_16_15_MASK                    0x18000
#define D0F0xE4_PHY_4021_AdaptThLoopGain_OFFSET                 17
#define D0F0xE4_PHY_4021_AdaptThLoopGain_WIDTH                  4
#define D0F0xE4_PHY_4021_AdaptThLoopGain_MASK                   0x1e0000
#define D0F0xE4_PHY_4021_Reserved_22_21_OFFSET                  21
#define D0F0xE4_PHY_4021_Reserved_22_21_WIDTH                   2
#define D0F0xE4_PHY_4021_Reserved_22_21_MASK                    0x600000
#define D0F0xE4_PHY_4021_AdaptLEQLoopGain_OFFSET                23
#define D0F0xE4_PHY_4021_AdaptLEQLoopGain_WIDTH                 2
#define D0F0xE4_PHY_4021_AdaptLEQLoopGain_MASK                  0x1800000
#define D0F0xE4_PHY_4021_Reserved_26_25_OFFSET                  25
#define D0F0xE4_PHY_4021_Reserved_26_25_WIDTH                   2
#define D0F0xE4_PHY_4021_Reserved_26_25_MASK                    0x6000000
#define D0F0xE4_PHY_4021_AdaptSwapBypVal_OFFSET                 27
#define D0F0xE4_PHY_4021_AdaptSwapBypVal_WIDTH                  3
#define D0F0xE4_PHY_4021_AdaptSwapBypVal_MASK                   0x38000000
#define D0F0xE4_PHY_4021_Reserved_30_30_OFFSET                  30
#define D0F0xE4_PHY_4021_Reserved_30_30_WIDTH                   1
#define D0F0xE4_PHY_4021_Reserved_30_30_MASK                    0x40000000
#define D0F0xE4_PHY_4021_AdaptSwapBypEn_OFFSET                  31
#define D0F0xE4_PHY_4021_AdaptSwapBypEn_WIDTH                   1
#define D0F0xE4_PHY_4021_AdaptSwapBypEn_MASK                    0x80000000

/// D0F0xE4_PHY_4021
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_1_0:2 ; ///<
    UINT32                                        AdaptPowerSaveOff:1 ; ///<
    UINT32                                             Reserved_3_3:1 ; ///<
    UINT32                                       AdaptPowerDownTime:2 ; ///<
    UINT32                                             Reserved_6_6:1 ; ///<
    UINT32                                      AdaptPiOffsetbypVal:5 ; ///<
    UINT32                                       AdaptPiOffsetBypEn:1 ; ///<
    UINT32                                           Reserved_13_13:1 ; ///<
    UINT32                                        AdaptRxDatGateDis:1 ; ///<
    UINT32                                           Reserved_16_15:2 ; ///<
    UINT32                                          AdaptThLoopGain:4 ; ///<
    UINT32                                           Reserved_22_21:2 ; ///<
    UINT32                                         AdaptLEQLoopGain:2 ; ///<
    UINT32                                           Reserved_26_25:2 ; ///<
    UINT32                                          AdaptSwapBypVal:3 ; ///<
    UINT32                                           Reserved_30_30:1 ; ///<
    UINT32                                           AdaptSwapBypEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_4021_STRUCT;

// **** D0F0xE4_PHY_E006 Register Definition ****
// Address
#define D0F0xE4_PHY_E006_ADDRESS                                0xe006

// Type
#define D0F0xE4_PHY_E006_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PHY_E006_GangedModeEn_OFFSET                    0
#define D0F0xE4_PHY_E006_GangedModeEn_WIDTH                     1
#define D0F0xE4_PHY_E006_GangedModeEn_MASK                      0x1
#define D0F0xE4_PHY_E006_Reserved_1_1_OFFSET                    1
#define D0F0xE4_PHY_E006_Reserved_1_1_WIDTH                     1
#define D0F0xE4_PHY_E006_Reserved_1_1_MASK                      0x2
#define D0F0xE4_PHY_E006_IsOwnMstr_OFFSET                       2
#define D0F0xE4_PHY_E006_IsOwnMstr_WIDTH                        1
#define D0F0xE4_PHY_E006_IsOwnMstr_MASK                         0x4
#define D0F0xE4_PHY_E006_IncoherentCkDet_OFFSET                 3
#define D0F0xE4_PHY_E006_IncoherentCkDet_WIDTH                  1
#define D0F0xE4_PHY_E006_IncoherentCkDet_MASK                   0x8
#define D0F0xE4_PHY_E006_Reserved_4_4_OFFSET                    4
#define D0F0xE4_PHY_E006_Reserved_4_4_WIDTH                     1
#define D0F0xE4_PHY_E006_Reserved_4_4_MASK                      0x10
#define D0F0xE4_PHY_E006_RdptInitMode_OFFSET                    5
#define D0F0xE4_PHY_E006_RdptInitMode_WIDTH                     3
#define D0F0xE4_PHY_E006_RdptInitMode_MASK                      0xe0
#define D0F0xE4_PHY_E006_Reserved_31_8_OFFSET                   8
#define D0F0xE4_PHY_E006_Reserved_31_8_WIDTH                    24
#define D0F0xE4_PHY_E006_Reserved_31_8_MASK                     0xffffff00

/// D0F0xE4_PHY_E006
typedef union {
  struct {                                                              ///<
    UINT32                                             GangedModeEn:1 ; ///<
    UINT32                                             Reserved_1_1:1 ; ///<
    UINT32                                                IsOwnMstr:1 ; ///<
    UINT32                                          IncoherentCkDet:1 ; ///<
    UINT32                                             Reserved_4_4:1 ; ///<
    UINT32                                             RdptInitMode:3 ; ///<
    UINT32                                            Reserved_31_8:24; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PHY_E006_STRUCT;

// **** D0F0xE4_PIF_0010 Register Definition ****
// Address
#define D0F0xE4_PIF_0010_ADDRESS                                0x10

// Type
#define D0F0xE4_PIF_0010_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PIF_0010_EiDetCycleMode_OFFSET                  4
#define D0F0xE4_PIF_0010_EiDetCycleMode_WIDTH                   1
#define D0F0xE4_PIF_0010_EiDetCycleMode_MASK                    0x10
#define D0F0xE4_PIF_0010_DivInitEnable_OFFSET                   12
#define D0F0xE4_PIF_0010_DivInitEnable_WIDTH                    1
#define D0F0xE4_PIF_0010_DivInitEnable_MASK                     0x1000

// **** D0F0xE4_PIF_0011 Register Definition ****
// Address
#define D0F0xE4_PIF_0011_ADDRESS                                0x11
// Type
#define D0F0xE4_PIF_0011_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PIF_0011_X2Lane10_OFFSET                        0
#define D0F0xE4_PIF_0011_X2Lane10_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane10_MASK                          0x1
#define D0F0xE4_PIF_0011_X2Lane32_OFFSET                        1
#define D0F0xE4_PIF_0011_X2Lane32_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane32_MASK                          0x2
#define D0F0xE4_PIF_0011_X2Lane54_OFFSET                        2
#define D0F0xE4_PIF_0011_X2Lane54_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane54_MASK                          0x4
#define D0F0xE4_PIF_0011_X2Lane76_OFFSET                        3
#define D0F0xE4_PIF_0011_X2Lane76_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane76_MASK                          0x8
#define D0F0xE4_PIF_0011_X2Lane98_OFFSET                        4
#define D0F0xE4_PIF_0011_X2Lane98_WIDTH                         1
#define D0F0xE4_PIF_0011_X2Lane98_MASK                          0x10
#define D0F0xE4_PIF_0011_X2Lane1110_OFFSET                      5
#define D0F0xE4_PIF_0011_X2Lane1110_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1110_MASK                        0x20
#define D0F0xE4_PIF_0011_X2Lane1312_OFFSET                      6
#define D0F0xE4_PIF_0011_X2Lane1312_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1312_MASK                        0x40
#define D0F0xE4_PIF_0011_X2Lane1514_OFFSET                      7
#define D0F0xE4_PIF_0011_X2Lane1514_WIDTH                       1
#define D0F0xE4_PIF_0011_X2Lane1514_MASK                        0x80
#define D0F0xE4_PIF_0011_X4Lane30_OFFSET                        8
#define D0F0xE4_PIF_0011_X4Lane30_WIDTH                         1
#define D0F0xE4_PIF_0011_X4Lane30_MASK                          0x100
#define D0F0xE4_PIF_0011_X4Lane74_OFFSET                        9
#define D0F0xE4_PIF_0011_X4Lane74_WIDTH                         1
#define D0F0xE4_PIF_0011_X4Lane74_MASK                          0x200
#define D0F0xE4_PIF_0011_X4Lane118_OFFSET                       10
#define D0F0xE4_PIF_0011_X4Lane118_WIDTH                        1
#define D0F0xE4_PIF_0011_X4Lane118_MASK                         0x400
#define D0F0xE4_PIF_0011_X4Lane1512_OFFSET                      11
#define D0F0xE4_PIF_0011_X4Lane1512_WIDTH                       1
#define D0F0xE4_PIF_0011_X4Lane1512_MASK                        0x800
#define D0F0xE4_PIF_0011_Reserved_15_12_OFFSET                  12
#define D0F0xE4_PIF_0011_Reserved_15_12_WIDTH                   4
#define D0F0xE4_PIF_0011_Reserved_15_12_MASK                    0xF000
#define D0F0xE4_PIF_0011_X8Lane70_OFFSET                        16
#define D0F0xE4_PIF_0011_X8Lane70_WIDTH                         1
#define D0F0xE4_PIF_0011_X8Lane70_MASK                          0x10000
#define D0F0xE4_PIF_0011_X8Lane158_OFFSET                       17
#define D0F0xE4_PIF_0011_X8Lane158_WIDTH                        1
#define D0F0xE4_PIF_0011_X8Lane158_MASK                         0x20000
#define D0F0xE4_PIF_0011_Reserved_19_18_OFFSET                  18
#define D0F0xE4_PIF_0011_Reserved_19_18_WIDTH                   2
#define D0F0xE4_PIF_0011_Reserved_19_18_MASK                    0xC0000
#define D0F0xE4_PIF_0011_X16Lane150_OFFSET                      20
#define D0F0xE4_PIF_0011_X16Lane150_WIDTH                       1
#define D0F0xE4_PIF_0011_X16Lane150_MASK                        0x100000
#define D0F0xE4_PIF_0011_Reserved_24_21_OFFSET                  21
#define D0F0xE4_PIF_0011_Reserved_24_21_WIDTH                   4
#define D0F0xE4_PIF_0011_Reserved_24_21_MASK                    0x1E00000
#define D0F0xE4_PIF_0011_MultiPif_OFFSET                        25
#define D0F0xE4_PIF_0011_MultiPif_WIDTH                         1
#define D0F0xE4_PIF_0011_MultiPif_MASK                          0x2000000
#define D0F0xE4_PIF_0011_Reserved_31_26_OFFSET                  26
#define D0F0xE4_PIF_0011_Reserved_31_26_WIDTH                   6
#define D0F0xE4_PIF_0011_Reserved_31_26_MASK                    0xFC000000

/// D0F0xE4_PIF_0011
typedef union {
  struct {                                                            ///<
    UINT32                                                X2Lane10:1; ///<
    UINT32                                                X2Lane32:1; ///<
    UINT32                                                X2Lane54:1; ///<
    UINT32                                                X2Lane76:1; ///<
    UINT32                                                X2Lane98:1; ///<
    UINT32                                              X2Lane1110:1; ///<
    UINT32                                              X2Lane1312:1; ///<
    UINT32                                              X2Lane1514:1; ///<
    UINT32                                                X4Lane30:1; ///<
    UINT32                                                X4Lane74:1; ///<
    UINT32                                               X4Lane118:1; ///<
    UINT32                                              X4Lane1512:1; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                                X8Lane70:1; ///<
    UINT32                                               X8Lane158:1; ///<
    UINT32                                          Reserved_19_18:2; ///<
    UINT32                                              X16Lane150:1; ///<
    UINT32                                          Reserved_24_21:4; ///<
    UINT32                                                MultiPif:1; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_PIF_0011_STRUCT;

// **** D0F0xE4_PIF_0012 Register Definition ****
// Address
#define D0F0xE4_PIF_0012_ADDRESS                                0x12
// Type
#define D0F0xE4_PIF_0012_TYPE                                   TYPE_D0F0xE4

// Field Data
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_OFFSET              0
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_WIDTH               3
#define D0F0xE4_PIF_0012_TxPowerStateInTxs2_MASK                0x7
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_OFFSET                  3
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_WIDTH                   1
#define D0F0xE4_PIF_0012_ForceRxEnInL0s_MASK                    0x8
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_OFFSET              4
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_WIDTH               3
#define D0F0xE4_PIF_0012_RxPowerStateInRxs2_MASK                0x70
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_OFFSET             7
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_WIDTH              3
#define D0F0xE4_PIF_0012_PllPowerStateInTxs2_MASK               0x380
#define D0F0xE4_PIF_0012_PllPowerStateInOff_OFFSET              10
#define D0F0xE4_PIF_0012_PllPowerStateInOff_WIDTH               3
#define D0F0xE4_PIF_0012_PllPowerStateInOff_MASK                0x1C00
#define D0F0xE4_PIF_0012_Reserved_15_13_OFFSET                  13
#define D0F0xE4_PIF_0012_Reserved_15_13_WIDTH                   3
#define D0F0xE4_PIF_0012_Reserved_15_13_MASK                    0xE000
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_OFFSET           16
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_WIDTH            1
#define D0F0xE4_PIF_0012_Tx2p5clkClockGatingEn_MASK             0x10000
#define D0F0xE4_PIF_0012_Reserved_23_17_OFFSET                  17
#define D0F0xE4_PIF_0012_Reserved_23_17_WIDTH                   7
#define D0F0xE4_PIF_0012_Reserved_23_17_MASK                    0xFE0000
#define D0F0xE4_PIF_0012_PllRampUpTime_OFFSET                   24
#define D0F0xE4_PIF_0012_PllRampUpTime_WIDTH                    3
#define D0F0xE4_PIF_0012_PllRampUpTime_MASK                     0x7000000
#define D0F0xE4_PIF_0012_Reserved_27_27_OFFSET                  27
#define D0F0xE4_PIF_0012_Reserved_27_27_WIDTH                   1
#define D0F0xE4_PIF_0012_Reserved_27_27_MASK                    0x8000000
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_OFFSET                28
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_WIDTH                 1
#define D0F0xE4_PIF_0012_PllPwrOverrideEn_MASK                  0x10000000
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_OFFSET               29
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_WIDTH                3
#define D0F0xE4_PIF_0012_PllPwrOverrideVal_MASK                 0xE0000000

/// D0F0xE4_PIF_0012
typedef union {
  struct {                                                            ///<
    UINT32                                      TxPowerStateInTxs2:3; ///<
    UINT32                                          ForceRxEnInL0s:1; ///<
    UINT32                                      RxPowerStateInRxs2:3; ///<
    UINT32                                     PllPowerStateInTxs2:3; ///<
    UINT32                                      PllPowerStateInOff:3; ///<
    UINT32                                          Reserved_15_13:3; ///<
    UINT32                                   Tx2p5clkClockGatingEn:1; ///<
    UINT32                                          Reserved_23_17:7; ///<
    UINT32                                           PllRampUpTime:3; ///<
    UINT32                                          Reserved_27_27:1; ///<
    UINT32                                        PllPwrOverrideEn:1; ///<
    UINT32                                       PllPwrOverrideVal:3; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_PIF_0012_STRUCT;

// **** D0F0xE4_PIF_0014 Register Definition ****
// Address
#define D0F0xE4_PIF_0014_ADDRESS                                0x14

// Type
#define D0F0xE4_PIF_0014_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PIF_0014_RxdetectPrgEn_OFFSET                   0
#define D0F0xE4_PIF_0014_RxdetectPrgEn_WIDTH                    1
#define D0F0xE4_PIF_0014_RxdetectPrgEn_MASK                     0x1
#define D0F0xE4_PIF_0014_RxDetectSamplTime_OFFSET               1
#define D0F0xE4_PIF_0014_RxDetectSamplTime_WIDTH                2
#define D0F0xE4_PIF_0014_RxDetectSamplTime_MASK                 0x6
#define D0F0xE4_PIF_0014_PllRampUpTimePrgEn_OFFSET              3
#define D0F0xE4_PIF_0014_PllRampUpTimePrgEn_WIDTH               1
#define D0F0xE4_PIF_0014_PllRampUpTimePrgEn_MASK                0x8
#define D0F0xE4_PIF_0014_Ls2ExitTimePrgEn_OFFSET                4
#define D0F0xE4_PIF_0014_Ls2ExitTimePrgEn_WIDTH                 1
#define D0F0xE4_PIF_0014_Ls2ExitTimePrgEn_MASK                  0x10
#define D0F0xE4_PIF_0014_Service2Step4DelayPrgEn_OFFSET         5
#define D0F0xE4_PIF_0014_Service2Step4DelayPrgEn_WIDTH          1
#define D0F0xE4_PIF_0014_Service2Step4DelayPrgEn_MASK           0x20
#define D0F0xE4_PIF_0014_Service3Step4DelayPrgEn_OFFSET         6
#define D0F0xE4_PIF_0014_Service3Step4DelayPrgEn_WIDTH          1
#define D0F0xE4_PIF_0014_Service3Step4DelayPrgEn_MASK           0x40
#define D0F0xE4_PIF_0014_RxDetectOverrideEn_OFFSET              7
#define D0F0xE4_PIF_0014_RxDetectOverrideEn_WIDTH               1
#define D0F0xE4_PIF_0014_RxDetectOverrideEn_MASK                0x80
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_7_0__OFFSET        8
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_7_0__WIDTH         8
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_7_0__MASK          0xff00
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_15_8__OFFSET       16
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_15_8__WIDTH        8
#define D0F0xE4_PIF_0014_RxDetectOverrideVal_15_8__MASK         0xff0000
#define D0F0xE4_PIF_0014_RxPhyStatusDelay_OFFSET                24
#define D0F0xE4_PIF_0014_RxPhyStatusDelay_WIDTH                 3
#define D0F0xE4_PIF_0014_RxPhyStatusDelay_MASK                  0x7000000
#define D0F0xE4_PIF_0014_RxStaggeringMode_OFFSET                27
#define D0F0xE4_PIF_0014_RxStaggeringMode_WIDTH                 1
#define D0F0xE4_PIF_0014_RxStaggeringMode_MASK                  0x8000000
#define D0F0xE4_PIF_0014_SpeedchangeStep2DelayPrgEn_OFFSET      28
#define D0F0xE4_PIF_0014_SpeedchangeStep2DelayPrgEn_WIDTH       1
#define D0F0xE4_PIF_0014_SpeedchangeStep2DelayPrgEn_MASK        0x10000000
#define D0F0xE4_PIF_0014_RxStaggeringDisable_OFFSET             29
#define D0F0xE4_PIF_0014_RxStaggeringDisable_WIDTH              1
#define D0F0xE4_PIF_0014_RxStaggeringDisable_MASK               0x20000000
#define D0F0xE4_PIF_0014_Pll1AlwaysOnEn_OFFSET                  30
#define D0F0xE4_PIF_0014_Pll1AlwaysOnEn_WIDTH                   1
#define D0F0xE4_PIF_0014_Pll1AlwaysOnEn_MASK                    0x40000000
#define D0F0xE4_PIF_0014_SpeedchangeStep4DelayForceLongEn_OFFSET 31
#define D0F0xE4_PIF_0014_SpeedchangeStep4DelayForceLongEn_WIDTH 1
#define D0F0xE4_PIF_0014_SpeedchangeStep4DelayForceLongEn_MASK  0x80000000

/// D0F0xE4_PIF_0014
typedef union {
  struct {                                                              ///<
    UINT32                                            RxdetectPrgEn:1 ; ///<
    UINT32                                        RxDetectSamplTime:2 ; ///<
    UINT32                                       PllRampUpTimePrgEn:1 ; ///<
    UINT32                                         Ls2ExitTimePrgEn:1 ; ///<
    UINT32                                  Service2Step4DelayPrgEn:1 ; ///<
    UINT32                                  Service3Step4DelayPrgEn:1 ; ///<
    UINT32                                       RxDetectOverrideEn:1 ; ///<
    UINT32                                 RxDetectOverrideVal_7_0_:8 ; ///<
    UINT32                                RxDetectOverrideVal_15_8_:8 ; ///<
    UINT32                                         RxPhyStatusDelay:3 ; ///<
    UINT32                                         RxStaggeringMode:1 ; ///<
    UINT32                               SpeedchangeStep2DelayPrgEn:1 ; ///<
    UINT32                                      RxStaggeringDisable:1 ; ///<
    UINT32                                           Pll1AlwaysOnEn:1 ; ///<
    UINT32                         SpeedchangeStep4DelayForceLongEn:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PIF_0014_STRUCT;

// **** D0F0xE4_PIF_001D Register Definition ****
// Address
#define D0F0xE4_PIF_001D_ADDRESS                                0x1d

// Type
#define D0F0xE4_PIF_001D_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PIF_001D_PrgService3Step4Delay_OFFSET           0
#define D0F0xE4_PIF_001D_PrgService3Step4Delay_WIDTH            18
#define D0F0xE4_PIF_001D_PrgService3Step4Delay_MASK             0x3ffff
#define D0F0xE4_PIF_001D_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PIF_001D_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PIF_001D_Reserved_31_18_MASK                    0xfffc0000

/// D0F0xE4_PIF_001D
typedef union {
  struct {                                                              ///<
    UINT32                                    PrgService3Step4Delay:18; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PIF_001D_STRUCT;

// **** D0F0xE4_PIF_0020 Register Definition ****
// Address
#define D0F0xE4_PIF_0020_ADDRESS                                0x20

// Type
#define D0F0xE4_PIF_0020_TYPE                                   TYPE_D0F0xE4
// Field Data
#define D0F0xE4_PIF_0020_TxPdnbOverrideEn_OFFSET                0
#define D0F0xE4_PIF_0020_TxPdnbOverrideEn_WIDTH                 1
#define D0F0xE4_PIF_0020_TxPdnbOverrideEn_MASK                  0x1
#define D0F0xE4_PIF_0020_TxPdnbOverrideVal_OFFSET               1
#define D0F0xE4_PIF_0020_TxPdnbOverrideVal_WIDTH                3
#define D0F0xE4_PIF_0020_TxPdnbOverrideVal_MASK                 0xe
#define D0F0xE4_PIF_0020_RxPdnbOverrideEn_OFFSET                4
#define D0F0xE4_PIF_0020_RxPdnbOverrideEn_WIDTH                 1
#define D0F0xE4_PIF_0020_RxPdnbOverrideEn_MASK                  0x10
#define D0F0xE4_PIF_0020_RxPdnbOverrideVal_OFFSET               5
#define D0F0xE4_PIF_0020_RxPdnbOverrideVal_WIDTH                3
#define D0F0xE4_PIF_0020_RxPdnbOverrideVal_MASK                 0xe0
#define D0F0xE4_PIF_0020_RxEnOverrideEn_OFFSET                  8
#define D0F0xE4_PIF_0020_RxEnOverrideEn_WIDTH                   1
#define D0F0xE4_PIF_0020_RxEnOverrideEn_MASK                    0x100
#define D0F0xE4_PIF_0020_RxEnOverrideVal_OFFSET                 9
#define D0F0xE4_PIF_0020_RxEnOverrideVal_WIDTH                  1
#define D0F0xE4_PIF_0020_RxEnOverrideVal_MASK                   0x200
#define D0F0xE4_PIF_0020_TxPwrOverrideEn_OFFSET                 10
#define D0F0xE4_PIF_0020_TxPwrOverrideEn_WIDTH                  1
#define D0F0xE4_PIF_0020_TxPwrOverrideEn_MASK                   0x400
#define D0F0xE4_PIF_0020_TxPwrOverrideVal_OFFSET                11
#define D0F0xE4_PIF_0020_TxPwrOverrideVal_WIDTH                 3
#define D0F0xE4_PIF_0020_TxPwrOverrideVal_MASK                  0x3800
#define D0F0xE4_PIF_0020_RxPwrOverrideEn_OFFSET                 14
#define D0F0xE4_PIF_0020_RxPwrOverrideEn_WIDTH                  1
#define D0F0xE4_PIF_0020_RxPwrOverrideEn_MASK                   0x4000
#define D0F0xE4_PIF_0020_RxPwrOverrideVal_OFFSET                15
#define D0F0xE4_PIF_0020_RxPwrOverrideVal_WIDTH                 3
#define D0F0xE4_PIF_0020_RxPwrOverrideVal_MASK                  0x38000
#define D0F0xE4_PIF_0020_Reserved_31_18_OFFSET                  18
#define D0F0xE4_PIF_0020_Reserved_31_18_WIDTH                   14
#define D0F0xE4_PIF_0020_Reserved_31_18_MASK                    0xfffc0000

/// D0F0xE4_PIF_0020
typedef union {
  struct {                                                              ///<
    UINT32                                         TxPdnbOverrideEn:1 ; ///<
    UINT32                                        TxPdnbOverrideVal:3 ; ///<
    UINT32                                         RxPdnbOverrideEn:1 ; ///<
    UINT32                                        RxPdnbOverrideVal:3 ; ///<
    UINT32                                           RxEnOverrideEn:1 ; ///<
    UINT32                                          RxEnOverrideVal:1 ; ///<
    UINT32                                          TxPwrOverrideEn:1 ; ///<
    UINT32                                         TxPwrOverrideVal:3 ; ///<
    UINT32                                          RxPwrOverrideEn:1 ; ///<
    UINT32                                         RxPwrOverrideVal:3 ; ///<
    UINT32                                           Reserved_31_18:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xE4_PIF_0020_STRUCT;

// **** D0F2xF4_x10 Register Definition ****
// Address
#define D0F2xF4_x10_ADDRESS                                     0x10
// Type
#define D0F2xF4_x10_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x10_DTCReplacementSel_OFFSET                    0
#define D0F2xF4_x10_DTCReplacementSel_WIDTH                     2
#define D0F2xF4_x10_DTCReplacementSel_MASK                      0x3
#define D0F2xF4_x10_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x10_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x10_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x10_DTCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x10_DTCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x10_DTCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x10_DTCParityEn_OFFSET                          4
#define D0F2xF4_x10_DTCParityEn_WIDTH                           1
#define D0F2xF4_x10_DTCParityEn_MASK                            0x10
#define D0F2xF4_x10_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x10_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x10_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x10_DTCInvalidationSel_OFFSET                   8
#define D0F2xF4_x10_DTCInvalidationSel_WIDTH                    2
#define D0F2xF4_x10_DTCInvalidationSel_MASK                     0x300
#define D0F2xF4_x10_DTCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x10_DTCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x10_DTCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x10_Reserved_12_11_OFFSET                       11
#define D0F2xF4_x10_Reserved_12_11_WIDTH                        2
#define D0F2xF4_x10_Reserved_12_11_MASK                         0x1800
#define D0F2xF4_x10_DTCBypass_OFFSET                            13
#define D0F2xF4_x10_DTCBypass_WIDTH                             1
#define D0F2xF4_x10_DTCBypass_MASK                              0x2000
#define D0F2xF4_x10_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x10_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x10_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x10_DTCParitySupport_OFFSET                     15
#define D0F2xF4_x10_DTCParitySupport_WIDTH                      1
#define D0F2xF4_x10_DTCParitySupport_MASK                       0x8000
#define D0F2xF4_x10_DTCWays_OFFSET                              16
#define D0F2xF4_x10_DTCWays_WIDTH                               8
#define D0F2xF4_x10_DTCWays_MASK                                0xFF0000
#define D0F2xF4_x10_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x10_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x10_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x10_DTCEntries_OFFSET                           28
#define D0F2xF4_x10_DTCEntries_WIDTH                            4
#define D0F2xF4_x10_DTCEntries_MASK                             0xF0000000

/// D0F2xF4_x10
typedef union {
  struct {                                                            ///<
    UINT32                                       DTCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         DTCLRUUpdatePri:1; ///<
    UINT32                                             DTCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      DTCInvalidationSel:2; ///<
    UINT32                                       DTCSoftInvalidate:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                               DTCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        DTCParitySupport:1; ///<
    UINT32                                                 DTCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              DTCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x10_STRUCT;

// **** D0F2xF4_x11 Register Definition ****
// Address
#define D0F2xF4_x11_ADDRESS                                     0x11
// Type
#define D0F2xF4_x11_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x11_DTCFuncBits_OFFSET                          0
#define D0F2xF4_x11_DTCFuncBits_WIDTH                           2
#define D0F2xF4_x11_DTCFuncBits_MASK                            0x3
#define D0F2xF4_x11_DTCDevBits_OFFSET                           2
#define D0F2xF4_x11_DTCDevBits_WIDTH                            3
#define D0F2xF4_x11_DTCDevBits_MASK                             0x1C
#define D0F2xF4_x11_DTCBusBits_OFFSET                           5
#define D0F2xF4_x11_DTCBusBits_WIDTH                            4
#define D0F2xF4_x11_DTCBusBits_MASK                             0x1E0
#define D0F2xF4_x11_Reserved_9_9_OFFSET                         9
#define D0F2xF4_x11_Reserved_9_9_WIDTH                          1
#define D0F2xF4_x11_Reserved_9_9_MASK                           0x200
#define D0F2xF4_x11_DtcAltHashEn_OFFSET                         10
#define D0F2xF4_x11_DtcAltHashEn_WIDTH                          1
#define D0F2xF4_x11_DtcAltHashEn_MASK                           0x400
#define D0F2xF4_x11_Reserved_15_11_OFFSET                       11
#define D0F2xF4_x11_Reserved_15_11_WIDTH                        5
#define D0F2xF4_x11_Reserved_15_11_MASK                         0xF800
#define D0F2xF4_x11_DtcAddressMask_OFFSET                       16
#define D0F2xF4_x11_DtcAddressMask_WIDTH                        16
#define D0F2xF4_x11_DtcAddressMask_MASK                         0xFFFF0000

/// D0F2xF4_x11
typedef union {
  struct {                                                            ///<
    UINT32                                             DTCFuncBits:2; ///<
    UINT32                                              DTCDevBits:3; ///<
    UINT32                                              DTCBusBits:4; ///<
    UINT32                                            Reserved_9_9:1; ///<
    UINT32                                            DtcAltHashEn:1; ///<
    UINT32                                          Reserved_15_11:5; ///<
    UINT32                                          DtcAddressMask:16; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x11_STRUCT;

// **** D0F2xF4_x14 Register Definition ****
// Address
#define D0F2xF4_x14_ADDRESS                                     0x14
// Type
#define D0F2xF4_x14_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x14_ITCReplacementSel_OFFSET                    0
#define D0F2xF4_x14_ITCReplacementSel_WIDTH                     2
#define D0F2xF4_x14_ITCReplacementSel_MASK                      0x3
#define D0F2xF4_x14_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x14_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x14_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x14_ITCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x14_ITCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x14_ITCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x14_ITCParityEn_OFFSET                          4
#define D0F2xF4_x14_ITCParityEn_WIDTH                           1
#define D0F2xF4_x14_ITCParityEn_MASK                            0x10
#define D0F2xF4_x14_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x14_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x14_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x14_ITCInvalidationSel_OFFSET                   8
#define D0F2xF4_x14_ITCInvalidationSel_WIDTH                    2
#define D0F2xF4_x14_ITCInvalidationSel_MASK                     0x300
#define D0F2xF4_x14_ITCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x14_ITCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x14_ITCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x14_Reserved_12_11_OFFSET                       11
#define D0F2xF4_x14_Reserved_12_11_WIDTH                        2
#define D0F2xF4_x14_Reserved_12_11_MASK                         0x1800
#define D0F2xF4_x14_ITCBypass_OFFSET                            13
#define D0F2xF4_x14_ITCBypass_WIDTH                             1
#define D0F2xF4_x14_ITCBypass_MASK                              0x2000
#define D0F2xF4_x14_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x14_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x14_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x14_ITCParitySupport_OFFSET                     15
#define D0F2xF4_x14_ITCParitySupport_WIDTH                      1
#define D0F2xF4_x14_ITCParitySupport_MASK                       0x8000
#define D0F2xF4_x14_ITCWays_OFFSET                              16
#define D0F2xF4_x14_ITCWays_WIDTH                               8
#define D0F2xF4_x14_ITCWays_MASK                                0xFF0000
#define D0F2xF4_x14_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x14_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x14_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x14_ITCEntries_OFFSET                           28
#define D0F2xF4_x14_ITCEntries_WIDTH                            4
#define D0F2xF4_x14_ITCEntries_MASK                             0xF0000000

/// D0F2xF4_x14
typedef union {
  struct {                                                            ///<
    UINT32                                       ITCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         ITCLRUUpdatePri:1; ///<
    UINT32                                             ITCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      ITCInvalidationSel:2; ///<
    UINT32                                       ITCSoftInvalidate:1; ///<
    UINT32                                          Reserved_12_11:2; ///<
    UINT32                                               ITCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        ITCParitySupport:1; ///<
    UINT32                                                 ITCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              ITCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x14_STRUCT;

// **** D0F2xF4_x15 Register Definition ****
// Address
#define D0F2xF4_x15_ADDRESS                                     0x15

// Type
#define D0F2xF4_x15_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x15_ITCFuncBits_OFFSET                          0
#define D0F2xF4_x15_ITCFuncBits_WIDTH                           2
#define D0F2xF4_x15_ITCFuncBits_MASK                            0x3
#define D0F2xF4_x15_ITCDevBits_OFFSET                           2
#define D0F2xF4_x15_ITCDevBits_WIDTH                            3
#define D0F2xF4_x15_ITCDevBits_MASK                             0x1c
#define D0F2xF4_x15_ITCBusBits_OFFSET                           5
#define D0F2xF4_x15_ITCBusBits_WIDTH                            4
#define D0F2xF4_x15_ITCBusBits_MASK                             0x1e0
#define D0F2xF4_x15_Reserved_9_9_OFFSET                         9
#define D0F2xF4_x15_Reserved_9_9_WIDTH                          1
#define D0F2xF4_x15_Reserved_9_9_MASK                           0x200
#define D0F2xF4_x15_ItcAltHashEn_OFFSET                         10
#define D0F2xF4_x15_ItcAltHashEn_WIDTH                          1
#define D0F2xF4_x15_ItcAltHashEn_MASK                           0x400
#define D0F2xF4_x15_ItcAltHashEn_VALUE                          0x1
#define D0F2xF4_x15_Reserved_15_11_OFFSET                       11
#define D0F2xF4_x15_Reserved_15_11_WIDTH                        5
#define D0F2xF4_x15_Reserved_15_11_MASK                         0xf800
#define D0F2xF4_x15_ITCAddressMask_OFFSET                       16
#define D0F2xF4_x15_ITCAddressMask_WIDTH                        16
#define D0F2xF4_x15_ITCAddressMask_MASK                         0xffff0000
#define D0F2xF4_x15_ITCAddressMask_VALUE                        0x0

/// D0F2xF4_x15
typedef union {
  struct {                                                              ///<
    UINT32                                              ITCFuncBits:2 ; ///<
    UINT32                                               ITCDevBits:3 ; ///<
    UINT32                                               ITCBusBits:4 ; ///<
    UINT32                                             Reserved_9_9:1 ; ///<
    UINT32                                             ItcAltHashEn:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                                           ITCAddressMask:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x15_STRUCT;

// **** D0F2xF4_x18 Register Definition ****
// Address
#define D0F2xF4_x18_ADDRESS                                     0x18
// Type
#define D0F2xF4_x18_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x18_PTCAReplacementSel_OFFSET                   0
#define D0F2xF4_x18_PTCAReplacementSel_WIDTH                    2
#define D0F2xF4_x18_PTCAReplacementSel_MASK                     0x3
#define D0F2xF4_x18_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x18_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x18_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x18_PTCALRUUpdatePri_OFFSET                     3
#define D0F2xF4_x18_PTCALRUUpdatePri_WIDTH                      1
#define D0F2xF4_x18_PTCALRUUpdatePri_MASK                       0x8
#define D0F2xF4_x18_PTCAParityEn_OFFSET                         4
#define D0F2xF4_x18_PTCAParityEn_WIDTH                          1
#define D0F2xF4_x18_PTCAParityEn_MASK                           0x10
#define D0F2xF4_x18_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x18_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x18_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x18_PTCAInvalidationSel_OFFSET                  8
#define D0F2xF4_x18_PTCAInvalidationSel_WIDTH                   2
#define D0F2xF4_x18_PTCAInvalidationSel_MASK                    0x300
#define D0F2xF4_x18_PTCASoftInvalidate_OFFSET                   10
#define D0F2xF4_x18_PTCASoftInvalidate_WIDTH                    1
#define D0F2xF4_x18_PTCASoftInvalidate_MASK                     0x400
#define D0F2xF4_x18_PTCA2MMode_OFFSET                           11
#define D0F2xF4_x18_PTCA2MMode_WIDTH                            1
#define D0F2xF4_x18_PTCA2MMode_MASK                             0x800
#define D0F2xF4_x18_Reserved_12_12_OFFSET                       12
#define D0F2xF4_x18_Reserved_12_12_WIDTH                        1
#define D0F2xF4_x18_Reserved_12_12_MASK                         0x1000
#define D0F2xF4_x18_PTCABypass_OFFSET                           13
#define D0F2xF4_x18_PTCABypass_WIDTH                            1
#define D0F2xF4_x18_PTCABypass_MASK                             0x2000
#define D0F2xF4_x18_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x18_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x18_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x18_PTCAParitySupport_OFFSET                    15
#define D0F2xF4_x18_PTCAParitySupport_WIDTH                     1
#define D0F2xF4_x18_PTCAParitySupport_MASK                      0x8000
#define D0F2xF4_x18_PTCAWays_OFFSET                             16
#define D0F2xF4_x18_PTCAWays_WIDTH                              8
#define D0F2xF4_x18_PTCAWays_MASK                               0xFF0000
#define D0F2xF4_x18_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x18_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x18_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x18_PTCAEntries_OFFSET                          28
#define D0F2xF4_x18_PTCAEntries_WIDTH                           4
#define D0F2xF4_x18_PTCAEntries_MASK                            0xF0000000

/// D0F2xF4_x18
typedef union {
  struct {                                                            ///<
    UINT32                                      PTCAReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                        PTCALRUUpdatePri:1; ///<
    UINT32                                            PTCAParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                     PTCAInvalidationSel:2; ///<
    UINT32                                      PTCASoftInvalidate:1; ///<
    UINT32                                              PTCA2MMode:1; ///<
    UINT32                                          Reserved_12_12:1; ///<
    UINT32                                              PTCABypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                       PTCAParitySupport:1; ///<
    UINT32                                                PTCAWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                             PTCAEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x18_STRUCT;

// **** D0F2xF4_x19 Register Definition ****
// Address
#define D0F2xF4_x19_ADDRESS                                     0x19

// Type
#define D0F2xF4_x19_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x19_PTCAFuncBits_OFFSET                         0
#define D0F2xF4_x19_PTCAFuncBits_WIDTH                          2
#define D0F2xF4_x19_PTCAFuncBits_MASK                           0x3
#define D0F2xF4_x19_PTCADevBits_OFFSET                          2
#define D0F2xF4_x19_PTCADevBits_WIDTH                           3
#define D0F2xF4_x19_PTCADevBits_MASK                            0x1c
#define D0F2xF4_x19_PTCABusBits_OFFSET                          5
#define D0F2xF4_x19_PTCABusBits_WIDTH                           4
#define D0F2xF4_x19_PTCABusBits_MASK                            0x1e0
#define D0F2xF4_x19_Reserved_9_9_OFFSET                         9
#define D0F2xF4_x19_Reserved_9_9_WIDTH                          1
#define D0F2xF4_x19_Reserved_9_9_MASK                           0x200
#define D0F2xF4_x19_PtcAltHashEn_OFFSET                         10
#define D0F2xF4_x19_PtcAltHashEn_WIDTH                          1
#define D0F2xF4_x19_PtcAltHashEn_MASK                           0x400
#define D0F2xF4_x19_PtcAltHashEn_VALUE                          0x1
#define D0F2xF4_x19_Reserved_15_11_OFFSET                       11
#define D0F2xF4_x19_Reserved_15_11_WIDTH                        5
#define D0F2xF4_x19_Reserved_15_11_MASK                         0xf800
#define D0F2xF4_x19_PTCAAddressMask_OFFSET                      16
#define D0F2xF4_x19_PTCAAddressMask_WIDTH                       16
#define D0F2xF4_x19_PTCAAddressMask_MASK                        0xffff0000
#define D0F2xF4_x19_PTCAAddressMask_VALUE                       0x0

/// D0F2xF4_x19
typedef union {
  struct {                                                              ///<
    UINT32                                             PTCAFuncBits:2 ; ///<
    UINT32                                              PTCADevBits:3 ; ///<
    UINT32                                              PTCABusBits:4 ; ///<
    UINT32                                             Reserved_9_9:1 ; ///<
    UINT32                                             PtcAltHashEn:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                                          PTCAAddressMask:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x19_STRUCT;

// **** D0F2xF4_x22 Register Definition ****
// Address
#define D0F2xF4_x22_ADDRESS                                     0x22

// Type
#define D0F2xF4_x22_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x22_L2aUpdateFilterBypass_OFFSET                0
#define D0F2xF4_x22_L2aUpdateFilterBypass_WIDTH                 1
#define D0F2xF4_x22_L2aUpdateFilterBypass_MASK                  0x1
#define D0F2xF4_x22_L2aUpdateFilterRdlatency_OFFSET             1
#define D0F2xF4_x22_L2aUpdateFilterRdlatency_WIDTH              4
#define D0F2xF4_x22_L2aUpdateFilterRdlatency_MASK               0x1e
#define D0F2xF4_x22_Reserved_31_5_OFFSET                        5
#define D0F2xF4_x22_Reserved_31_5_WIDTH                         27
#define D0F2xF4_x22_Reserved_31_5_MASK                          0xffffffe0

/// D0F2xF4_x22
typedef union {
  struct {                                                              ///<
    UINT32                                    L2aUpdateFilterBypass:1 ; ///<
    UINT32                                 L2aUpdateFilterRdlatency:4 ; ///<
    UINT32                                            Reserved_31_5:27; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x22_STRUCT;

// **** D0F2xF4_x30 Register Definition ****
// Address
#define D0F2xF4_x30_ADDRESS                                     0x30
// Type
#define D0F2xF4_x30_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x30_ERRRuleLock1_OFFSET                         0
#define D0F2xF4_x30_ERRRuleLock1_WIDTH                          1
#define D0F2xF4_x30_ERRRuleLock1_MASK                           0x1
#define D0F2xF4_x30_Reserved_3_1_OFFSET                         1
#define D0F2xF4_x30_Reserved_3_1_WIDTH                          3
#define D0F2xF4_x30_Reserved_3_1_MASK                           0xE
#define D0F2xF4_x30_ERRRuleDisable3_OFFSET                      4
#define D0F2xF4_x30_ERRRuleDisable3_WIDTH                       28
#define D0F2xF4_x30_ERRRuleDisable3_MASK                        0xFFFFFFF0

/// D0F2xF4_x30
typedef union {
  struct {                                                            ///<
    UINT32                                            ERRRuleLock1:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                         ERRRuleDisable3:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x30_STRUCT;

// **** D0F2xF4_x33 Register Definition ****
// Address
#define D0F2xF4_x33_ADDRESS                                     0x33
// Type
#define D0F2xF4_x33_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x33_CKGateL2ARegsDisable_OFFSET                 0
#define D0F2xF4_x33_CKGateL2ARegsDisable_WIDTH                  1
#define D0F2xF4_x33_CKGateL2ARegsDisable_MASK                   0x1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_OFFSET              1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_WIDTH               1
#define D0F2xF4_x33_CKGateL2ADynamicDisable_MASK                0x2
#define D0F2xF4_x33_CKGateL2ACacheDisable_OFFSET                2
#define D0F2xF4_x33_CKGateL2ACacheDisable_WIDTH                 1
#define D0F2xF4_x33_CKGateL2ACacheDisable_MASK                  0x4
#define D0F2xF4_x33_CKGateL2ASpare_OFFSET                       3
#define D0F2xF4_x33_CKGateL2ASpare_WIDTH                        1
#define D0F2xF4_x33_CKGateL2ASpare_MASK                         0x8
#define D0F2xF4_x33_CKGateL2ALength_OFFSET                      4
#define D0F2xF4_x33_CKGateL2ALength_WIDTH                       2
#define D0F2xF4_x33_CKGateL2ALength_MASK                        0x30
#define D0F2xF4_x33_CKGateL2AStop_OFFSET                        6
#define D0F2xF4_x33_CKGateL2AStop_WIDTH                         2
#define D0F2xF4_x33_CKGateL2AStop_MASK                          0xC0
#define D0F2xF4_x33_Reserved_31_8_OFFSET                        8
#define D0F2xF4_x33_Reserved_31_8_WIDTH                         24
#define D0F2xF4_x33_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F2xF4_x33
typedef union {
  struct {                                                            ///<
    UINT32                                    CKGateL2ARegsDisable:1; ///<
    UINT32                                 CKGateL2ADynamicDisable:1; ///<
    UINT32                                   CKGateL2ACacheDisable:1; ///<
    UINT32                                          CKGateL2ASpare:1; ///<
    UINT32                                         CKGateL2ALength:2; ///<
    UINT32                                           CKGateL2AStop:2; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x33_STRUCT;

// **** D0F2xF4_x34 Register Definition ****
// Address
#define D0F2xF4_x34_ADDRESS                                     0x34
// Type
#define D0F2xF4_x34_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x34_L2aregGstPgsize_OFFSET                      0
#define D0F2xF4_x34_L2aregGstPgsize_WIDTH                       2
#define D0F2xF4_x34_L2aregGstPgsize_MASK                        0x3
#define D0F2xF4_x34_L2aregHostPgsize_OFFSET                     2
#define D0F2xF4_x34_L2aregHostPgsize_WIDTH                      2
#define D0F2xF4_x34_L2aregHostPgsize_MASK                       0xC
#define D0F2xF4_x34_Reserved_31_4_OFFSET                        4
#define D0F2xF4_x34_Reserved_31_4_WIDTH                         28
#define D0F2xF4_x34_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F2xF4_x34
typedef union {
  struct {                                                            ///<
    UINT32                                         L2aregGstPgsize:2; ///<
    UINT32                                        L2aregHostPgsize:2; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x34_STRUCT;

// **** D0F2xF4_x3B Register Definition ****
// Address
#define D0F2xF4_x3B_ADDRESS                                     0x3b

// Type
#define D0F2xF4_x3B_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x3B_FSM_ADDR_OFFSET                             0
#define D0F2xF4_x3B_FSM_ADDR_WIDTH                              8
#define D0F2xF4_x3B_FSM_ADDR_MASK                               0xff
#define D0F2xF4_x3B_Power_Down_OFFSET                           8
#define D0F2xF4_x3B_Power_Down_WIDTH                            1
#define D0F2xF4_x3B_Power_Down_MASK                             0x100
#define D0F2xF4_x3B_Power_Up_OFFSET                             9
#define D0F2xF4_x3B_Power_Up_WIDTH                              1
#define D0F2xF4_x3B_Power_Up_MASK                               0x200
#define D0F2xF4_x3B_P1_Select_OFFSET                            10
#define D0F2xF4_x3B_P1_Select_WIDTH                             1
#define D0F2xF4_x3B_P1_Select_MASK                              0x400
#define D0F2xF4_x3B_Reserved_OFFSET                             11
#define D0F2xF4_x3B_Reserved_WIDTH                              1
#define D0F2xF4_x3B_Reserved_MASK                               0x800
#define D0F2xF4_x3B_PG_Write_OFFSET                             12
#define D0F2xF4_x3B_PG_Write_WIDTH                              1
#define D0F2xF4_x3B_PG_Write_MASK                               0x1000
#define D0F2xF4_x3B_PG_Read_OFFSET                              13
#define D0F2xF4_x3B_PG_Read_WIDTH                               1
#define D0F2xF4_x3B_PG_Read_MASK                                0x2000
#define D0F2xF4_x3B_Reserved14_26_OFFSET                        14
#define D0F2xF4_x3B_Reserved14_26_WIDTH                         13
#define D0F2xF4_x3B_Reserved14_26_MASK                          0x7ffc000
#define D0F2xF4_x3B_SRBM_override_OFFSET                        27
#define D0F2xF4_x3B_SRBM_override_WIDTH                         1
#define D0F2xF4_x3B_SRBM_override_MASK                          0x8000000
#define D0F2xF4_x3B_REG_ADDR_OFFSET                             28
#define D0F2xF4_x3B_REG_ADDR_WIDTH                              4
#define D0F2xF4_x3B_REG_ADDR_MASK                               0xf0000000

/// D0F2xF4_x3B
typedef union {
  struct {                                                              ///<
    UINT32                                                 FSM_ADDR:8 ; ///<
    UINT32                                               Power_Down:1 ; ///<
    UINT32                                                 Power_Up:1 ; ///<
    UINT32                                                P1_Select:1 ; ///<
    UINT32                                                 Reserved:1 ; ///<
    UINT32                                                 PG_Write:1 ; ///<
    UINT32                                                  PG_Read:1 ; ///<
    UINT32                                            Reserved14_26:13; ///<
    UINT32                                            SRBM_override:1 ; ///<
    UINT32                                                 REG_ADDR:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x3B_STRUCT;

// **** D0F2xF4_x47 Register Definition ****
// Address
#define D0F2xF4_x47_ADDRESS                                     0x47
// Type
#define D0F2xF4_x47_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x47_Reserved_0_0_OFFSET                         0
#define D0F2xF4_x47_Reserved_0_0_WIDTH                          1
#define D0F2xF4_x47_Reserved_0_0_MASK                           0x1
#define D0F2xF4_x47_TwNwEn_OFFSET                               1
#define D0F2xF4_x47_TwNwEn_WIDTH                                1
#define D0F2xF4_x47_TwNwEn_MASK                                 0x2
#define D0F2xF4_x47_TwAtomicFilterEn_OFFSET                     2
#define D0F2xF4_x47_TwAtomicFilterEn_WIDTH                      1
#define D0F2xF4_x47_TwAtomicFilterEn_MASK                       0x4
#define D0F2xF4_x47_Reserved_31_3_OFFSET                        3
#define D0F2xF4_x47_Reserved_31_3_WIDTH                         29
#define D0F2xF4_x47_Reserved_31_3_MASK                          0xFFFFFFF8

/// D0F2xF4_x47
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                                  TwNwEn:1; ///<
    UINT32                                        TwAtomicFilterEn:1; ///<
    UINT32                                           Reserved_31_3:29; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x47_STRUCT;

// **** D0F2xF4_x4C Register Definition ****
// Address
#define D0F2xF4_x4C_ADDRESS                                     0x4C
// Type
#define D0F2xF4_x4C_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x4C_QueueArbFBPri_OFFSET                        0
#define D0F2xF4_x4C_QueueArbFBPri_WIDTH                         1
#define D0F2xF4_x4C_QueueArbFBPri_MASK                          0x1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_OFFSET                1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_WIDTH                 1
#define D0F2xF4_x4C_PTCAddrTransReqUpdate_MASK                  0x2
#define D0F2xF4_x4C_FC1Dis_OFFSET                               2
#define D0F2xF4_x4C_FC1Dis_WIDTH                                1
#define D0F2xF4_x4C_FC1Dis_MASK                                 0x4
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_OFFSET                  3
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_WIDTH                   1
#define D0F2xF4_x4C_DTCUpdateVOneIVZero_MASK                    0x8
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_OFFSET                  4
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_WIDTH                   1
#define D0F2xF4_x4C_DTCUpdateVZeroIVOne_MASK                    0x10
#define D0F2xF4_x4C_FC2Dis_OFFSET                               5
#define D0F2xF4_x4C_FC2Dis_WIDTH                                1
#define D0F2xF4_x4C_FC2Dis_MASK                                 0x20
#define D0F2xF4_x4C_FC3Dis_OFFSET                               6
#define D0F2xF4_x4C_FC3Dis_WIDTH                                1
#define D0F2xF4_x4C_FC3Dis_MASK                                 0x40
#define D0F2xF4_x4C_FC2AltMode_OFFSET                           7
#define D0F2xF4_x4C_FC2AltMode_WIDTH                            1
#define D0F2xF4_x4C_FC2AltMode_MASK                             0x80
#define D0F2xF4_x4C_GstPartialPtcCntrl_OFFSET                   8
#define D0F2xF4_x4C_GstPartialPtcCntrl_WIDTH                    2
#define D0F2xF4_x4C_GstPartialPtcCntrl_MASK                     0x300
#define D0F2xF4_x4C_Reserved_31_10_OFFSET                       10
#define D0F2xF4_x4C_Reserved_31_10_WIDTH                        22
#define D0F2xF4_x4C_Reserved_31_10_MASK                         0xFFFFFC00

/// D0F2xF4_x4C
typedef union {
  struct {                                                            ///<
    UINT32                                           QueueArbFBPri:1; ///<
    UINT32                                   PTCAddrTransReqUpdate:1; ///<
    UINT32                                                  FC1Dis:1; ///<
    UINT32                                     DTCUpdateVOneIVZero:1; ///<
    UINT32                                     DTCUpdateVZeroIVOne:1; ///<
    UINT32                                                  FC2Dis:1; ///<
    UINT32                                                  FC3Dis:1; ///<
    UINT32                                              FC2AltMode:1; ///<
    UINT32                                      GstPartialPtcCntrl:2; ///<
    UINT32                                          Reserved_31_10:22; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x4C_STRUCT;

// **** D0F2xF4_x50 Register Definition ****
// Address
#define D0F2xF4_x50_ADDRESS                                     0x50
// Type
#define D0F2xF4_x50_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x50_PDCReplacementSel_OFFSET                    0
#define D0F2xF4_x50_PDCReplacementSel_WIDTH                     2
#define D0F2xF4_x50_PDCReplacementSel_MASK                      0x3
#define D0F2xF4_x50_Reserved_2_2_OFFSET                         2
#define D0F2xF4_x50_Reserved_2_2_WIDTH                          1
#define D0F2xF4_x50_Reserved_2_2_MASK                           0x4
#define D0F2xF4_x50_PDCLRUUpdatePri_OFFSET                      3
#define D0F2xF4_x50_PDCLRUUpdatePri_WIDTH                       1
#define D0F2xF4_x50_PDCLRUUpdatePri_MASK                        0x8
#define D0F2xF4_x50_PDCParityEn_OFFSET                          4
#define D0F2xF4_x50_PDCParityEn_WIDTH                           1
#define D0F2xF4_x50_PDCParityEn_MASK                            0x10
#define D0F2xF4_x50_Reserved_7_5_OFFSET                         5
#define D0F2xF4_x50_Reserved_7_5_WIDTH                          3
#define D0F2xF4_x50_Reserved_7_5_MASK                           0xE0
#define D0F2xF4_x50_PDCInvalidationSel_OFFSET                   8
#define D0F2xF4_x50_PDCInvalidationSel_WIDTH                    2
#define D0F2xF4_x50_PDCInvalidationSel_MASK                     0x300
#define D0F2xF4_x50_PDCSoftInvalidate_OFFSET                    10
#define D0F2xF4_x50_PDCSoftInvalidate_WIDTH                     1
#define D0F2xF4_x50_PDCSoftInvalidate_MASK                      0x400
#define D0F2xF4_x50_Reserved_11_11_OFFSET                       11
#define D0F2xF4_x50_Reserved_11_11_WIDTH                        1
#define D0F2xF4_x50_Reserved_11_11_MASK                         0x800
#define D0F2xF4_x50_PDCSearchDirection_OFFSET                   12
#define D0F2xF4_x50_PDCSearchDirection_WIDTH                    1
#define D0F2xF4_x50_PDCSearchDirection_MASK                     0x1000
#define D0F2xF4_x50_PDCBypass_OFFSET                            13
#define D0F2xF4_x50_PDCBypass_WIDTH                             1
#define D0F2xF4_x50_PDCBypass_MASK                              0x2000
#define D0F2xF4_x50_Reserved_14_14_OFFSET                       14
#define D0F2xF4_x50_Reserved_14_14_WIDTH                        1
#define D0F2xF4_x50_Reserved_14_14_MASK                         0x4000
#define D0F2xF4_x50_PDCParitySupport_OFFSET                     15
#define D0F2xF4_x50_PDCParitySupport_WIDTH                      1
#define D0F2xF4_x50_PDCParitySupport_MASK                       0x8000
#define D0F2xF4_x50_PDCWays_OFFSET                              16
#define D0F2xF4_x50_PDCWays_WIDTH                               8
#define D0F2xF4_x50_PDCWays_MASK                                0xFF0000
#define D0F2xF4_x50_Reserved_27_24_OFFSET                       24
#define D0F2xF4_x50_Reserved_27_24_WIDTH                        4
#define D0F2xF4_x50_Reserved_27_24_MASK                         0xF000000
#define D0F2xF4_x50_PDCEntries_OFFSET                           28
#define D0F2xF4_x50_PDCEntries_WIDTH                            4
#define D0F2xF4_x50_PDCEntries_MASK                             0xF0000000

/// D0F2xF4_x50
typedef union {
  struct {                                                            ///<
    UINT32                                       PDCReplacementSel:2; ///<
    UINT32                                            Reserved_2_2:1; ///<
    UINT32                                         PDCLRUUpdatePri:1; ///<
    UINT32                                             PDCParityEn:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                      PDCInvalidationSel:2; ///<
    UINT32                                       PDCSoftInvalidate:1; ///<
    UINT32                                          Reserved_11_11:1; ///<
    UINT32                                      PDCSearchDirection:1; ///<
    UINT32                                               PDCBypass:1; ///<
    UINT32                                          Reserved_14_14:1; ///<
    UINT32                                        PDCParitySupport:1; ///<
    UINT32                                                 PDCWays:8; ///<
    UINT32                                          Reserved_27_24:4; ///<
    UINT32                                              PDCEntries:4; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x50_STRUCT;

// **** D0F2xF4_x53 Register Definition ****
// Address
#define D0F2xF4_x53_ADDRESS                                     0x53

// Type
#define D0F2xF4_x53_TYPE                                        TYPE_D0F2xF4
// Field Data
#define D0F2xF4_x53_L2bUpdateFilterBypass_OFFSET                0
#define D0F2xF4_x53_L2bUpdateFilterBypass_WIDTH                 1
#define D0F2xF4_x53_L2bUpdateFilterBypass_MASK                  0x1
#define D0F2xF4_x53_L2bUpdateFilterRdlatency_OFFSET             1
#define D0F2xF4_x53_L2bUpdateFilterRdlatency_WIDTH              4
#define D0F2xF4_x53_L2bUpdateFilterRdlatency_MASK               0x1e
#define D0F2xF4_x53_Reserved_31_5_OFFSET                        5
#define D0F2xF4_x53_Reserved_31_5_WIDTH                         27
#define D0F2xF4_x53_Reserved_31_5_MASK                          0xffffffe0

/// D0F2xF4_x53
typedef union {
  struct {                                                              ///<
    UINT32                                    L2bUpdateFilterBypass:1 ; ///<
    UINT32                                 L2bUpdateFilterRdlatency:4 ; ///<
    UINT32                                            Reserved_31_5:27; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xF4_x53_STRUCT;

// **** D0F2xF4_x56 Register Definition ****
// Address
#define D0F2xF4_x56_ADDRESS                                     0x56
// Type
#define D0F2xF4_x56_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x56_CPPrefetchDis_OFFSET                        0
#define D0F2xF4_x56_CPPrefetchDis_WIDTH                         1
#define D0F2xF4_x56_CPPrefetchDis_MASK                          0x1
#define D0F2xF4_x56_CPFlushOnWait_OFFSET                        1
#define D0F2xF4_x56_CPFlushOnWait_WIDTH                         1
#define D0F2xF4_x56_CPFlushOnWait_MASK                          0x2
#define D0F2xF4_x56_CPFlushOnInv_OFFSET                         2
#define D0F2xF4_x56_CPFlushOnInv_WIDTH                          1
#define D0F2xF4_x56_CPFlushOnInv_MASK                           0x4
#define D0F2xF4_x56_Reserved_15_3_OFFSET                        3
#define D0F2xF4_x56_Reserved_15_3_WIDTH                         13
#define D0F2xF4_x56_Reserved_15_3_MASK                          0xFFF8
#define D0F2xF4_x56_CPRdDelay_OFFSET                            16
#define D0F2xF4_x56_CPRdDelay_WIDTH                             16
#define D0F2xF4_x56_CPRdDelay_MASK                              0xFFFF0000

/// D0F2xF4_x56
typedef union {
  struct {                                                            ///<
    UINT32                                           CPPrefetchDis:1; ///<
    UINT32                                           CPFlushOnWait:1; ///<
    UINT32                                            CPFlushOnInv:1; ///<
    UINT32                                           Reserved_15_3:13; ///<
    UINT32                                               CPRdDelay:16; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x56_STRUCT;

// **** D0F2xF4_x57 Register Definition ****
// Address
#define D0F2xF4_x57_ADDRESS                                     0x57
// Type
#define D0F2xF4_x57_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x57_L1ImuPcieGfxDis_OFFSET                      0
#define D0F2xF4_x57_L1ImuPcieGfxDis_WIDTH                       1
#define D0F2xF4_x57_L1ImuPcieGfxDis_MASK                        0x1
#define D0F2xF4_x57_CPL1Off_1_OFFSET                            1
#define D0F2xF4_x57_CPL1Off_1_WIDTH                             1
#define D0F2xF4_x57_CPL1Off_1_MASK                              0x2
#define D0F2xF4_x57_L1ImuIntGfxDis_OFFSET                       2
#define D0F2xF4_x57_L1ImuIntGfxDis_WIDTH                        1
#define D0F2xF4_x57_L1ImuIntGfxDis_MASK                         0x4
#define D0F2xF4_x57_CPL1Off_15_3_OFFSET                         3
#define D0F2xF4_x57_CPL1Off_15_3_WIDTH                          13
#define D0F2xF4_x57_CPL1Off_15_3_MASK                           0xFFF8
#define D0F2xF4_x57_Reserved_31_16_OFFSET                       16
#define D0F2xF4_x57_Reserved_31_16_WIDTH                        16
#define D0F2xF4_x57_Reserved_31_16_MASK                         0xFFFF0000

/// D0F2xF4_x57
typedef union {
  struct {                                                            ///<
    UINT32                                         L1ImuPcieGfxDis:1; ///<
    UINT32                                               CPL1Off_1:1; ///<
    UINT32                                          L1ImuIntGfxDis:1; ///<
    UINT32                                            CPL1Off_15_3:13; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x57_STRUCT;

// **** D0F2xF4_x80 Register Definition ****
// Address
#define D0F2xF4_x80_ADDRESS                                     0x80
// Type
#define D0F2xF4_x80_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x80_ERRRuleLock0_OFFSET                         0
#define D0F2xF4_x80_ERRRuleLock0_WIDTH                          1
#define D0F2xF4_x80_ERRRuleLock0_MASK                           0x1
#define D0F2xF4_x80_Reserved_3_1_OFFSET                         1
#define D0F2xF4_x80_Reserved_3_1_WIDTH                          3
#define D0F2xF4_x80_Reserved_3_1_MASK                           0xE
#define D0F2xF4_x80_ERRRuleDisable0_OFFSET                      4
#define D0F2xF4_x80_ERRRuleDisable0_WIDTH                       28
#define D0F2xF4_x80_ERRRuleDisable0_MASK                        0xFFFFFFF0

/// D0F2xF4_x80
typedef union {
  struct {                                                            ///<
    UINT32                                            ERRRuleLock0:1; ///<
    UINT32                                            Reserved_3_1:3; ///<
    UINT32                                         ERRRuleDisable0:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x80_STRUCT;

// **** D0F2xF4_x90 Register Definition ****
// Address
#define D0F2xF4_x90_ADDRESS                                     0x90
// Type
#define D0F2xF4_x90_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x90_CKGateL2BRegsDisable_OFFSET                 0
#define D0F2xF4_x90_CKGateL2BRegsDisable_WIDTH                  1
#define D0F2xF4_x90_CKGateL2BRegsDisable_MASK                   0x1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_OFFSET              1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_WIDTH               1
#define D0F2xF4_x90_CKGateL2BDynamicDisable_MASK                0x2
#define D0F2xF4_x90_CKGateL2BMiscDisable_OFFSET                 2
#define D0F2xF4_x90_CKGateL2BMiscDisable_WIDTH                  1
#define D0F2xF4_x90_CKGateL2BMiscDisable_MASK                   0x4
#define D0F2xF4_x90_CKGateL2BCacheDisable_OFFSET                3
#define D0F2xF4_x90_CKGateL2BCacheDisable_WIDTH                 1
#define D0F2xF4_x90_CKGateL2BCacheDisable_MASK                  0x8
#define D0F2xF4_x90_CKGateL2BLength_OFFSET                      4
#define D0F2xF4_x90_CKGateL2BLength_WIDTH                       2
#define D0F2xF4_x90_CKGateL2BLength_MASK                        0x30
#define D0F2xF4_x90_CKGateL2BStop_OFFSET                        6
#define D0F2xF4_x90_CKGateL2BStop_WIDTH                         2
#define D0F2xF4_x90_CKGateL2BStop_MASK                          0xC0
#define D0F2xF4_x90_Reserved_31_8_OFFSET                        8
#define D0F2xF4_x90_Reserved_31_8_WIDTH                         24
#define D0F2xF4_x90_Reserved_31_8_MASK                          0xFFFFFF00

/// D0F2xF4_x90
typedef union {
  struct {                                                            ///<
    UINT32                                    CKGateL2BRegsDisable:1; ///<
    UINT32                                 CKGateL2BDynamicDisable:1; ///<
    UINT32                                    CKGateL2BMiscDisable:1; ///<
    UINT32                                   CKGateL2BCacheDisable:1; ///<
    UINT32                                         CKGateL2BLength:2; ///<
    UINT32                                           CKGateL2BStop:2; ///<
    UINT32                                           Reserved_31_8:24; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x90_STRUCT;

// **** D0F2xF4_x94 Register Definition ****
// Address
#define D0F2xF4_x94_ADDRESS                                     0x94
// Type
#define D0F2xF4_x94_TYPE                                        TYPE_D0F2xF4

// Field Data
#define D0F2xF4_x94_L2bregGstPgsize_OFFSET                      0
#define D0F2xF4_x94_L2bregGstPgsize_WIDTH                       2
#define D0F2xF4_x94_L2bregGstPgsize_MASK                        0x3
#define D0F2xF4_x94_L2bregHostPgsize_OFFSET                     2
#define D0F2xF4_x94_L2bregHostPgsize_WIDTH                      2
#define D0F2xF4_x94_L2bregHostPgsize_MASK                       0xC
#define D0F2xF4_x94_Reserved_31_4_OFFSET                        4
#define D0F2xF4_x94_Reserved_31_4_WIDTH                         28
#define D0F2xF4_x94_Reserved_31_4_MASK                          0xFFFFFFF0

/// D0F2xF4_x94
typedef union {
  struct {                                                            ///<
    UINT32                                         L2bregGstPgsize:2; ///<
    UINT32                                        L2bregHostPgsize:2; ///<
    UINT32                                           Reserved_31_4:28; ///<

  } Field;

  UINT32 Value;
} D0F2xF4_x94_STRUCT;

// **** D0F2xFC_x07_L1 Register Definition ****
// Address
#define D0F2xFC_x07_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x7)
// Type
#define D0F2xFC_x07_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x07_L1_PhantomFuncEn_OFFSET                     0
#define D0F2xFC_x07_L1_PhantomFuncEn_WIDTH                      1
#define D0F2xFC_x07_L1_PhantomFuncEn_MASK                       0x1
#define D0F2xFC_x07_L1_Reserved_10_1_OFFSET                     1
#define D0F2xFC_x07_L1_Reserved_10_1_WIDTH                      10
#define D0F2xFC_x07_L1_Reserved_10_1_MASK                       0x7FE
#define D0F2xFC_x07_L1_SpecReqFilterEn_OFFSET                   11
#define D0F2xFC_x07_L1_SpecReqFilterEn_WIDTH                    1
#define D0F2xFC_x07_L1_SpecReqFilterEn_MASK                     0x800
#define D0F2xFC_x07_L1_AtsSeqNumEn_OFFSET                       12
#define D0F2xFC_x07_L1_AtsSeqNumEn_WIDTH                        1
#define D0F2xFC_x07_L1_AtsSeqNumEn_MASK                         0x1000
#define D0F2xFC_x07_L1_Reserved_13_13_OFFSET                    13
#define D0F2xFC_x07_L1_Reserved_13_13_WIDTH                     1
#define D0F2xFC_x07_L1_Reserved_13_13_MASK                      0x2000
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_OFFSET             14
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_WIDTH              1
#define D0F2xFC_x07_L1_AtsPhysPageOverlapDis_MASK               0x4000
#define D0F2xFC_x07_L1_Reserved_16_15_OFFSET                    15
#define D0F2xFC_x07_L1_Reserved_16_15_WIDTH                     2
#define D0F2xFC_x07_L1_Reserved_16_15_MASK                      0x18000
#define D0F2xFC_x07_L1_L1NwEn_OFFSET                            17
#define D0F2xFC_x07_L1_L1NwEn_WIDTH                             1
#define D0F2xFC_x07_L1_L1NwEn_MASK                              0x20000
#define D0F2xFC_x07_L1_Reserved_31_18_OFFSET                    18
#define D0F2xFC_x07_L1_Reserved_31_18_WIDTH                     14
#define D0F2xFC_x07_L1_Reserved_31_18_MASK                      0xFFFC0000

/// D0F2xFC_x07_L1
typedef union {
  struct {                                                            ///<
    UINT32                                           PhantomFuncEn:1; ///<
    UINT32                                           Reserved_10_1:10; ///<
    UINT32                                         SpecReqFilterEn:1; ///<
    UINT32                                             AtsSeqNumEn:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                   AtsPhysPageOverlapDis:1; ///<
    UINT32                                          Reserved_16_15:2; ///<
    UINT32                                                  L1NwEn:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x07_L1_STRUCT;

// **** D0F2xFC_x0C_L1 Register Definition ****
// Address
#define D0F2xFC_x0C_L1_ADDRESS(Sel)                             ((Sel << 16) | 0xC)
// Type
#define D0F2xFC_x0C_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x0C_L1_UnfilterDis_OFFSET                       0
#define D0F2xFC_x0C_L1_UnfilterDis_WIDTH                        1
#define D0F2xFC_x0C_L1_UnfilterDis_MASK                         0x1
#define D0F2xFC_x0C_L1_FragmentDis_OFFSET                       1
#define D0F2xFC_x0C_L1_FragmentDis_WIDTH                        1
#define D0F2xFC_x0C_L1_FragmentDis_MASK                         0x2
#define D0F2xFC_x0C_L1_CacheirOnly_OFFSET                       2
#define D0F2xFC_x0C_L1_CacheirOnly_WIDTH                        1
#define D0F2xFC_x0C_L1_CacheirOnly_MASK                         0x4
#define D0F2xFC_x0C_L1_CacheiwOnly_OFFSET                       3
#define D0F2xFC_x0C_L1_CacheiwOnly_WIDTH                        1
#define D0F2xFC_x0C_L1_CacheiwOnly_MASK                         0x8
#define D0F2xFC_x0C_L1_Reserved_4_4_OFFSET                      4
#define D0F2xFC_x0C_L1_Reserved_4_4_WIDTH                       1
#define D0F2xFC_x0C_L1_Reserved_4_4_MASK                        0x10
#define D0F2xFC_x0C_L1_ReplacementSel_OFFSET                    5
#define D0F2xFC_x0C_L1_ReplacementSel_WIDTH                     1
#define D0F2xFC_x0C_L1_ReplacementSel_MASK                      0x20
#define D0F2xFC_x0C_L1_Reserved_7_6_OFFSET                      6
#define D0F2xFC_x0C_L1_Reserved_7_6_WIDTH                       2
#define D0F2xFC_x0C_L1_Reserved_7_6_MASK                        0xC0
#define D0F2xFC_x0C_L1_L2Credits_OFFSET                         8
#define D0F2xFC_x0C_L1_L2Credits_WIDTH                          6
#define D0F2xFC_x0C_L1_L2Credits_MASK                           0x3F00
#define D0F2xFC_x0C_L1_Reserved_19_14_OFFSET                    14
#define D0F2xFC_x0C_L1_Reserved_19_14_WIDTH                     6
#define D0F2xFC_x0C_L1_Reserved_19_14_MASK                      0xFC000
#define D0F2xFC_x0C_L1_L1Banks_OFFSET                           20
#define D0F2xFC_x0C_L1_L1Banks_WIDTH                            2
#define D0F2xFC_x0C_L1_L1Banks_MASK                             0x300000
#define D0F2xFC_x0C_L1_Reserved_23_22_OFFSET                    22
#define D0F2xFC_x0C_L1_Reserved_23_22_WIDTH                     2
#define D0F2xFC_x0C_L1_Reserved_23_22_MASK                      0xC00000
#define D0F2xFC_x0C_L1_L1Entries_OFFSET                         24
#define D0F2xFC_x0C_L1_L1Entries_WIDTH                          4
#define D0F2xFC_x0C_L1_L1Entries_MASK                           0xF000000
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_OFFSET                 28
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_WIDTH                  3
#define D0F2xFC_x0C_L1_L1VirtOrderQueues_MASK                   0x70000000
#define D0F2xFC_x0C_L1_Reserved_31_31_OFFSET                    31
#define D0F2xFC_x0C_L1_Reserved_31_31_WIDTH                     1
#define D0F2xFC_x0C_L1_Reserved_31_31_MASK                      0x80000000

/// D0F2xFC_x0C_L1
typedef union {
  struct {                                                            ///<
    UINT32                                             UnfilterDis:1; ///<
    UINT32                                             FragmentDis:1; ///<
    UINT32                                             CacheirOnly:1; ///<
    UINT32                                             CacheiwOnly:1; ///<
    UINT32                                            Reserved_4_4:1; ///<
    UINT32                                          ReplacementSel:1; ///<
    UINT32                                            Reserved_7_6:2; ///<
    UINT32                                               L2Credits:6; ///<
    UINT32                                          Reserved_19_14:6; ///<
    UINT32                                                 L1Banks:2; ///<
    UINT32                                          Reserved_23_22:2; ///<
    UINT32                                               L1Entries:4; ///<
    UINT32                                       L1VirtOrderQueues:3; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x0C_L1_STRUCT;

// **** D0F2xFC_x0D_L1 Register Definition ****
// Address
#define D0F2xFC_x0D_L1_ADDRESS(Sel)                             ((Sel << 16) | 0xD)
// Type
#define D0F2xFC_x0D_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x0D_L1_VOQPortBits_OFFSET                       0
#define D0F2xFC_x0D_L1_VOQPortBits_WIDTH                        3
#define D0F2xFC_x0D_L1_VOQPortBits_MASK                         0x7
#define D0F2xFC_x0D_L1_Reserved_3_3_OFFSET                      3
#define D0F2xFC_x0D_L1_Reserved_3_3_WIDTH                       1
#define D0F2xFC_x0D_L1_Reserved_3_3_MASK                        0x8
#define D0F2xFC_x0D_L1_VOQFuncBits_OFFSET                       4
#define D0F2xFC_x0D_L1_VOQFuncBits_WIDTH                        3
#define D0F2xFC_x0D_L1_VOQFuncBits_MASK                         0x70
#define D0F2xFC_x0D_L1_Reserved_7_7_OFFSET                      7
#define D0F2xFC_x0D_L1_Reserved_7_7_WIDTH                       1
#define D0F2xFC_x0D_L1_Reserved_7_7_MASK                        0x80
#define D0F2xFC_x0D_L1_VOQXorMode_OFFSET                        8
#define D0F2xFC_x0D_L1_VOQXorMode_WIDTH                         1
#define D0F2xFC_x0D_L1_VOQXorMode_MASK                          0x100
#define D0F2xFC_x0D_L1_CacheByPass_OFFSET                       9
#define D0F2xFC_x0D_L1_CacheByPass_WIDTH                        1
#define D0F2xFC_x0D_L1_CacheByPass_MASK                         0x200
#define D0F2xFC_x0D_L1_L1CacheParityEn_OFFSET                   10
#define D0F2xFC_x0D_L1_L1CacheParityEn_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheParityEn_MASK                     0x400
#define D0F2xFC_x0D_L1_L1ParityEn_OFFSET                        11
#define D0F2xFC_x0D_L1_L1ParityEn_WIDTH                         1
#define D0F2xFC_x0D_L1_L1ParityEn_MASK                          0x800
#define D0F2xFC_x0D_L1_L1DTEDis_OFFSET                          12
#define D0F2xFC_x0D_L1_L1DTEDis_WIDTH                           1
#define D0F2xFC_x0D_L1_L1DTEDis_MASK                            0x1000
#define D0F2xFC_x0D_L1_BlockL1Dis_OFFSET                        13
#define D0F2xFC_x0D_L1_BlockL1Dis_WIDTH                         1
#define D0F2xFC_x0D_L1_BlockL1Dis_MASK                          0x2000
#define D0F2xFC_x0D_L1_WqEntrydis_OFFSET                        14
#define D0F2xFC_x0D_L1_WqEntrydis_WIDTH                         5
#define D0F2xFC_x0D_L1_WqEntrydis_MASK                          0x7C000
#define D0F2xFC_x0D_L1_AtsNobufferInsert_OFFSET                 19
#define D0F2xFC_x0D_L1_AtsNobufferInsert_WIDTH                  1
#define D0F2xFC_x0D_L1_AtsNobufferInsert_MASK                   0x80000
#define D0F2xFC_x0D_L1_SndFilterDis_OFFSET                      20
#define D0F2xFC_x0D_L1_SndFilterDis_WIDTH                       1
#define D0F2xFC_x0D_L1_SndFilterDis_MASK                        0x100000
#define D0F2xFC_x0D_L1_L1orderEn_OFFSET                         21
#define D0F2xFC_x0D_L1_L1orderEn_WIDTH                          1
#define D0F2xFC_x0D_L1_L1orderEn_MASK                           0x200000
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_OFFSET                   22
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheInvAllEn_MASK                     0x400000
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_OFFSET                23
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_WIDTH                 3
#define D0F2xFC_x0D_L1_SelectTimeoutPulse_MASK                  0x3800000
#define D0F2xFC_x0D_L1_L1CacheSelReqid_OFFSET                   26
#define D0F2xFC_x0D_L1_L1CacheSelReqid_WIDTH                    1
#define D0F2xFC_x0D_L1_L1CacheSelReqid_MASK                     0x4000000
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_OFFSET              27
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_WIDTH               1
#define D0F2xFC_x0D_L1_L1CacheSelInterleave_MASK                0x8000000
#define D0F2xFC_x0D_L1_PretransNovaFilteren_OFFSET              28
#define D0F2xFC_x0D_L1_PretransNovaFilteren_WIDTH               1
#define D0F2xFC_x0D_L1_PretransNovaFilteren_MASK                0x10000000
#define D0F2xFC_x0D_L1_Untrans2mFilteren_OFFSET                 29
#define D0F2xFC_x0D_L1_Untrans2mFilteren_WIDTH                  1
#define D0F2xFC_x0D_L1_Untrans2mFilteren_MASK                   0x20000000
#define D0F2xFC_x0D_L1_L1DebugCntrMode_OFFSET                   30
#define D0F2xFC_x0D_L1_L1DebugCntrMode_WIDTH                    1
#define D0F2xFC_x0D_L1_L1DebugCntrMode_MASK                     0x40000000
#define D0F2xFC_x0D_L1_Reserved_31_31_OFFSET                    31
#define D0F2xFC_x0D_L1_Reserved_31_31_WIDTH                     1
#define D0F2xFC_x0D_L1_Reserved_31_31_MASK                      0x80000000

/// D0F2xFC_x0D_L1
typedef union {
  struct {                                                            ///<
    UINT32                                             VOQPortBits:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                             VOQFuncBits:3; ///<
    UINT32                                            Reserved_7_7:1; ///<
    UINT32                                              VOQXorMode:1; ///<
    UINT32                                             CacheByPass:1; ///<
    UINT32                                         L1CacheParityEn:1; ///<
    UINT32                                              L1ParityEn:1; ///<
    UINT32                                                L1DTEDis:1; ///<
    UINT32                                              BlockL1Dis:1; ///<
    UINT32                                              WqEntrydis:5; ///<
    UINT32                                       AtsNobufferInsert:1; ///<
    UINT32                                            SndFilterDis:1; ///<
    UINT32                                               L1orderEn:1; ///<
    UINT32                                         L1CacheInvAllEn:1; ///<
    UINT32                                      SelectTimeoutPulse:3; ///<
    UINT32                                         L1CacheSelReqid:1; ///<
    UINT32                                    L1CacheSelInterleave:1; ///<
    UINT32                                    PretransNovaFilteren:1; ///<
    UINT32                                       Untrans2mFilteren:1; ///<
    UINT32                                         L1DebugCntrMode:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x0D_L1_STRUCT;

// **** D0F2xFC_x32_L1 Register Definition ****
// Address
#define D0F2xFC_x32_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x32)

// Type
#define D0F2xFC_x32_L1_TYPE                                     TYPE_D0F2xFC
// Field Data
#define D0F2xFC_x32_L1_AtsMultipleRespEn_OFFSET                 0
#define D0F2xFC_x32_L1_AtsMultipleRespEn_WIDTH                  1
#define D0F2xFC_x32_L1_AtsMultipleRespEn_MASK                   0x1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_OFFSET               1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_WIDTH                1
#define D0F2xFC_x32_L1_AtsMultipleL1toL2En_MASK                 0x2
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_OFFSET                 2
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_WIDTH                  1
#define D0F2xFC_x32_L1_TimeoutPulseExtEn_MASK                   0x4
#define D0F2xFC_x32_L1_TlpprefixerrEn_OFFSET                    3
#define D0F2xFC_x32_L1_TlpprefixerrEn_WIDTH                     1
#define D0F2xFC_x32_L1_TlpprefixerrEn_MASK                      0x8
#define D0F2xFC_x32_L1_DmaBufCredits_OFFSET                     4
#define D0F2xFC_x32_L1_DmaBufCredits_WIDTH                      6
#define D0F2xFC_x32_L1_DmaBufCredits_MASK                       0x3f0
#define D0F2xFC_x32_L1_DmaBufMaxNpCred_OFFSET                   10
#define D0F2xFC_x32_L1_DmaBufMaxNpCred_WIDTH                    6
#define D0F2xFC_x32_L1_DmaBufMaxNpCred_MASK                     0xfc00
#define D0F2xFC_x32_L1_DmaNpHaltDis_OFFSET                      16
#define D0F2xFC_x32_L1_DmaNpHaltDis_WIDTH                       1
#define D0F2xFC_x32_L1_DmaNpHaltDis_MASK                        0x10000
#define D0F2xFC_x32_L1_ForceDmaAttrLow_OFFSET                   17
#define D0F2xFC_x32_L1_ForceDmaAttrLow_WIDTH                    3
#define D0F2xFC_x32_L1_ForceDmaAttrLow_MASK                     0xe0000
#define D0F2xFC_x32_L1_Reserved_31_20_OFFSET                    20
#define D0F2xFC_x32_L1_Reserved_31_20_WIDTH                     12
#define D0F2xFC_x32_L1_Reserved_31_20_MASK                      0xfff00000

/// D0F2xFC_x32_L1
typedef union {
  struct {                                                              ///<
    UINT32                                        AtsMultipleRespEn:1 ; ///<
    UINT32                                      AtsMultipleL1toL2En:1 ; ///<
    UINT32                                        TimeoutPulseExtEn:1 ; ///<
    UINT32                                           TlpprefixerrEn:1 ; ///<
    UINT32                                            DmaBufCredits:6 ; ///<
    UINT32                                          DmaBufMaxNpCred:6 ; ///<
    UINT32                                             DmaNpHaltDis:1 ; ///<
    UINT32                                          ForceDmaAttrLow:3 ; ///<
    UINT32                                           Reserved_31_20:12; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F2xFC_x32_L1_STRUCT;

// **** D0F2xFC_x33_L1 Register Definition ****
// Address
#define D0F2xFC_x33_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x33)
// Type
#define D0F2xFC_x33_L1_TYPE                                     TYPE_D0F2xFC

// Field Data
#define D0F2xFC_x33_L1_L1ClkgateLen_OFFSET                      0
#define D0F2xFC_x33_L1_L1ClkgateLen_WIDTH                       2
#define D0F2xFC_x33_L1_L1ClkgateLen_MASK                        0x3
#define D0F2xFC_x33_L1_Reserved_3_2_OFFSET                      2
#define D0F2xFC_x33_L1_Reserved_3_2_WIDTH                       2
#define D0F2xFC_x33_L1_Reserved_3_2_MASK                        0xC
#define D0F2xFC_x33_L1_L1DmaClkgateEn_OFFSET                    4
#define D0F2xFC_x33_L1_L1DmaClkgateEn_WIDTH                     1
#define D0F2xFC_x33_L1_L1DmaClkgateEn_MASK                      0x10
#define D0F2xFC_x33_L1_L1CacheClkgateEn_OFFSET                  5
#define D0F2xFC_x33_L1_L1CacheClkgateEn_WIDTH                   1
#define D0F2xFC_x33_L1_L1CacheClkgateEn_MASK                    0x20
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_OFFSET                  6
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_WIDTH                   1
#define D0F2xFC_x33_L1_L1CpslvClkgateEn_MASK                    0x40
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_OFFSET               7
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_WIDTH                1
#define D0F2xFC_x33_L1_L1DmaInputClkgateEn_MASK                 0x80
#define D0F2xFC_x33_L1_L1PerfClkgateEn_OFFSET                   8
#define D0F2xFC_x33_L1_L1PerfClkgateEn_WIDTH                    1
#define D0F2xFC_x33_L1_L1PerfClkgateEn_MASK                     0x100
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_OFFSET                 9
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_WIDTH                  1
#define D0F2xFC_x33_L1_L1MemoryClkgateEn_MASK                   0x200
#define D0F2xFC_x33_L1_L1RegClkgateEn_OFFSET                    10
#define D0F2xFC_x33_L1_L1RegClkgateEn_WIDTH                     1
#define D0F2xFC_x33_L1_L1RegClkgateEn_MASK                      0x400
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_OFFSET                11
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_WIDTH                 1
#define D0F2xFC_x33_L1_L1HostreqClkgateEn_MASK                  0x800
#define D0F2xFC_x33_L1_Reserved_30_12_OFFSET                    12
#define D0F2xFC_x33_L1_Reserved_30_12_WIDTH                     19
#define D0F2xFC_x33_L1_Reserved_30_12_MASK                      0x7FFFF000
#define D0F2xFC_x33_L1_L1L2ClkgateEn_OFFSET                     31
#define D0F2xFC_x33_L1_L1L2ClkgateEn_WIDTH                      1
#define D0F2xFC_x33_L1_L1L2ClkgateEn_MASK                       0x80000000

/// D0F2xFC_x33_L1
typedef union {
  struct {                                                            ///<
    UINT32                                            L1ClkgateLen:2; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                          L1DmaClkgateEn:1; ///<
    UINT32                                        L1CacheClkgateEn:1; ///<
    UINT32                                        L1CpslvClkgateEn:1; ///<
    UINT32                                     L1DmaInputClkgateEn:1; ///<
    UINT32                                         L1PerfClkgateEn:1; ///<
    UINT32                                       L1MemoryClkgateEn:1; ///<
    UINT32                                          L1RegClkgateEn:1; ///<
    UINT32                                      L1HostreqClkgateEn:1; ///<
    UINT32                                          Reserved_30_12:19; ///<
    UINT32                                           L1L2ClkgateEn:1; ///<

  } Field;

  UINT32 Value;
} D0F2xFC_x33_L1_STRUCT;

// **** DxFxx68 Register Definition ****
// Address
#define DxFxx68_ADDRESS                                         0x68

// Type
#define DxFxx68_TYPE                                            TYPE_D4F1
// Field Data
#define DxFxx68_PmControl_OFFSET                                0
#define DxFxx68_PmControl_WIDTH                                 2
#define DxFxx68_PmControl_MASK                                  0x3
#define DxFxx68_Reserved_2_2_OFFSET                             2
#define DxFxx68_Reserved_2_2_WIDTH                              1
#define DxFxx68_Reserved_2_2_MASK                               0x4
#define DxFxx68_ReadCplBoundary_OFFSET                          3
#define DxFxx68_ReadCplBoundary_WIDTH                           1
#define DxFxx68_ReadCplBoundary_MASK                            0x8
#define DxFxx68_LinkDis_OFFSET                                  4
#define DxFxx68_LinkDis_WIDTH                                   1
#define DxFxx68_LinkDis_MASK                                    0x10
#define DxFxx68_RetrainLink_OFFSET                              5
#define DxFxx68_RetrainLink_WIDTH                               1
#define DxFxx68_RetrainLink_MASK                                0x20
#define DxFxx68_CommonClockCfg_OFFSET                           6
#define DxFxx68_CommonClockCfg_WIDTH                            1
#define DxFxx68_CommonClockCfg_MASK                             0x40
#define DxFxx68_ExtendedSync_OFFSET                             7
#define DxFxx68_ExtendedSync_WIDTH                              1
#define DxFxx68_ExtendedSync_MASK                               0x80
#define DxFxx68_ClockPowerManagementEn_OFFSET                   8
#define DxFxx68_ClockPowerManagementEn_WIDTH                    1
#define DxFxx68_ClockPowerManagementEn_MASK                     0x100
#define DxFxx68_HWAutonomousWidthDisable_OFFSET                 9
#define DxFxx68_HWAutonomousWidthDisable_WIDTH                  1
#define DxFxx68_HWAutonomousWidthDisable_MASK                   0x200
#define DxFxx68_LinkBWManagementIntEn_OFFSET                    10
#define DxFxx68_LinkBWManagementIntEn_WIDTH                     1
#define DxFxx68_LinkBWManagementIntEn_MASK                      0x400
#define DxFxx68_LinkAutonomousBWIntEn_OFFSET                    11
#define DxFxx68_LinkAutonomousBWIntEn_WIDTH                     1
#define DxFxx68_LinkAutonomousBWIntEn_MASK                      0x800
#define DxFxx68_Reserved_15_12_OFFSET                           12
#define DxFxx68_Reserved_15_12_WIDTH                            4
#define DxFxx68_Reserved_15_12_MASK                             0xf000
#define DxFxx68_LinkSpeed_OFFSET                                16
#define DxFxx68_LinkSpeed_WIDTH                                 4
#define DxFxx68_LinkSpeed_MASK                                  0xf0000
#define DxFxx68_NegotiatedLinkWidth_OFFSET                      20
#define DxFxx68_NegotiatedLinkWidth_WIDTH                       6
#define DxFxx68_NegotiatedLinkWidth_MASK                        0x3f00000
#define DxFxx68_Reserved_26_26_OFFSET                           26
#define DxFxx68_Reserved_26_26_WIDTH                            1
#define DxFxx68_Reserved_26_26_MASK                             0x4000000
#define DxFxx68_LinkTraining_OFFSET                             27
#define DxFxx68_LinkTraining_WIDTH                              1
#define DxFxx68_LinkTraining_MASK                               0x8000000
#define DxFxx68_SlotClockCfg_OFFSET                             28
#define DxFxx68_SlotClockCfg_WIDTH                              1
#define DxFxx68_SlotClockCfg_MASK                               0x10000000
#define DxFxx68_DlActive_OFFSET                                 29
#define DxFxx68_DlActive_WIDTH                                  1
#define DxFxx68_DlActive_MASK                                   0x20000000
#define DxFxx68_LinkBWManagementStatus_OFFSET                   30
#define DxFxx68_LinkBWManagementStatus_WIDTH                    1
#define DxFxx68_LinkBWManagementStatus_MASK                     0x40000000
#define DxFxx68_LinkAutonomousBWStatus_OFFSET                   31
#define DxFxx68_LinkAutonomousBWStatus_WIDTH                    1
#define DxFxx68_LinkAutonomousBWStatus_MASK                     0x80000000

/// DxFxx68
typedef union {
  struct {                                                              ///<
    UINT32                                                PmControl:2 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                          ReadCplBoundary:1 ; ///<
    UINT32                                                  LinkDis:1 ; ///<
    UINT32                                              RetrainLink:1 ; ///<
    UINT32                                           CommonClockCfg:1 ; ///<
    UINT32                                             ExtendedSync:1 ; ///<
    UINT32                                   ClockPowerManagementEn:1 ; ///<
    UINT32                                 HWAutonomousWidthDisable:1 ; ///<
    UINT32                                    LinkBWManagementIntEn:1 ; ///<
    UINT32                                    LinkAutonomousBWIntEn:1 ; ///<
    UINT32                                           Reserved_15_12:4 ; ///<
    UINT32                                                LinkSpeed:4 ; ///<
    UINT32                                      NegotiatedLinkWidth:6 ; ///<
    UINT32                                           Reserved_26_26:1 ; ///<
    UINT32                                             LinkTraining:1 ; ///<
    UINT32                                             SlotClockCfg:1 ; ///<
    UINT32                                                 DlActive:1 ; ///<
    UINT32                                   LinkBWManagementStatus:1 ; ///<
    UINT32                                   LinkAutonomousBWStatus:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxx68_STRUCT;

// **** DxFxxE4_x02 Register Definition ****
// Address
#define DxFxxE4_x02_ADDRESS                                     0x02
// Type
#define DxFxxE4_x02_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_x02_Reserved_14_0_OFFSET                        0
#define DxFxxE4_x02_Reserved_14_0_WIDTH                         15
#define DxFxxE4_x02_Reserved_14_0_MASK                          0x7FFF
#define DxFxxE4_x02_Bitfield_15_15_OFFSET                       15
#define DxFxxE4_x02_Bitfield_15_15_WIDTH                        1
#define DxFxxE4_x02_Bitfield_15_15_MASK                         0x8000
#define DxFxxE4_x02_Reserved_31_16_OFFSET                       16
#define DxFxxE4_x02_Reserved_31_16_WIDTH                        16
#define DxFxxE4_x02_Reserved_31_16_MASK                         0xFFFF0000

/// DxFxxE4_x70
typedef union {
  struct {                                                            ///<
    UINT32                                          Reserved_14_0:15; ///<
    UINT32                                          Bitfield_15_15:1; ///<
    UINT32                                         Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_x02_STRUCT;

// **** DxFxxE4_x70 Register Definition ****
// Address
#define DxFxxE4_x70_ADDRESS                                     0x70
// Type
#define DxFxxE4_x70_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_x70_Reserved_15_0_OFFSET                        0
#define DxFxxE4_x70_Reserved_15_0_WIDTH                         16
#define DxFxxE4_x70_Reserved_15_0_MASK                          0xFFFF
#define DxFxxE4_x70_RxRcbCplTimeout_OFFSET                      16
#define DxFxxE4_x70_RxRcbCplTimeout_WIDTH                       3
#define DxFxxE4_x70_RxRcbCplTimeout_MASK                        0x70000
#define DxFxxE4_x70_RxRcbCplTimeoutMode_OFFSET                  19
#define DxFxxE4_x70_RxRcbCplTimeoutMode_WIDTH                   1
#define DxFxxE4_x70_RxRcbCplTimeoutMode_MASK                    0x80000
#define DxFxxE4_x70_Reserved_31_20_OFFSET                       20
#define DxFxxE4_x70_Reserved_31_20_WIDTH                        12
#define DxFxxE4_x70_Reserved_31_20_MASK                         0xFFF00000

/// DxFxxE4_x70
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_15_0:16; ///<
    UINT32                                         RxRcbCplTimeout:3; ///<
    UINT32                                     RxRcbCplTimeoutMode:1; ///<
    UINT32                                          Reserved_31_20:12; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_x70_STRUCT;

// **** DxFxxE4_xA0 Register Definition ****
// Address
#define DxFxxE4_xA0_ADDRESS                                     0xa0

// Type
#define DxFxxE4_xA0_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xA0_Reserved_0_0_OFFSET                         0
#define DxFxxE4_xA0_Reserved_0_0_WIDTH                          1
#define DxFxxE4_xA0_Reserved_0_0_MASK                           0x1
#define DxFxxE4_xA0_LcDontEnterL23InD0_OFFSET                   1
#define DxFxxE4_xA0_LcDontEnterL23InD0_WIDTH                    1
#define DxFxxE4_xA0_LcDontEnterL23InD0_MASK                     0x2
#define DxFxxE4_xA0_LcResetlIdleCountEn_OFFSET                  2
#define DxFxxE4_xA0_LcResetlIdleCountEn_WIDTH                   1
#define DxFxxE4_xA0_LcResetlIdleCountEn_MASK                    0x4
#define DxFxxE4_xA0_LcResetLink_OFFSET                          3
#define DxFxxE4_xA0_LcResetLink_WIDTH                           1
#define DxFxxE4_xA0_LcResetLink_MASK                            0x8
#define DxFxxE4_xA0_Lc16xClearTxPipe_OFFSET                     4
#define DxFxxE4_xA0_Lc16xClearTxPipe_WIDTH                      4
#define DxFxxE4_xA0_Lc16xClearTxPipe_MASK                       0xf0
#define DxFxxE4_xA0_Lc16xClearTxPipe_VALUE                      0x3
#define DxFxxE4_xA0_LcL0sInactivity_OFFSET                      8
#define DxFxxE4_xA0_LcL0sInactivity_WIDTH                       4
#define DxFxxE4_xA0_LcL0sInactivity_MASK                        0xf00
#define DxFxxE4_xA0_LcL1Inactivity_OFFSET                       12
#define DxFxxE4_xA0_LcL1Inactivity_WIDTH                        4
#define DxFxxE4_xA0_LcL1Inactivity_MASK                         0xf000
#define DxFxxE4_xA0_LcPmiToL1Dis_OFFSET                         16
#define DxFxxE4_xA0_LcPmiToL1Dis_WIDTH                          1
#define DxFxxE4_xA0_LcPmiToL1Dis_MASK                           0x10000
#define DxFxxE4_xA0_LcIncNFtsEn_OFFSET                          17
#define DxFxxE4_xA0_LcIncNFtsEn_WIDTH                           1
#define DxFxxE4_xA0_LcIncNFtsEn_MASK                            0x20000
#define DxFxxE4_xA0_LcLookForIdleInL1L23_OFFSET                 18
#define DxFxxE4_xA0_LcLookForIdleInL1L23_WIDTH                  2
#define DxFxxE4_xA0_LcLookForIdleInL1L23_MASK                   0xc0000
#define DxFxxE4_xA0_LcFactorInExtSync_OFFSET                    20
#define DxFxxE4_xA0_LcFactorInExtSync_WIDTH                     1
#define DxFxxE4_xA0_LcFactorInExtSync_MASK                      0x100000
#define DxFxxE4_xA0_LcWaitForPmAckDis_OFFSET                    21
#define DxFxxE4_xA0_LcWaitForPmAckDis_WIDTH                     1
#define DxFxxE4_xA0_LcWaitForPmAckDis_MASK                      0x200000
#define DxFxxE4_xA0_LcWakeFromL23_OFFSET                        22
#define DxFxxE4_xA0_LcWakeFromL23_WIDTH                         1
#define DxFxxE4_xA0_LcWakeFromL23_MASK                          0x400000
#define DxFxxE4_xA0_LcL1ImmediateAck_OFFSET                     23
#define DxFxxE4_xA0_LcL1ImmediateAck_WIDTH                      1
#define DxFxxE4_xA0_LcL1ImmediateAck_MASK                       0x800000
#define DxFxxE4_xA0_LcL1ImmediateAck_VALUE                      0x1
#define DxFxxE4_xA0_LcAspmToL1Dis_OFFSET                        24
#define DxFxxE4_xA0_LcAspmToL1Dis_WIDTH                         1
#define DxFxxE4_xA0_LcAspmToL1Dis_MASK                          0x1000000
#define DxFxxE4_xA0_LcDelayCount_OFFSET                         25
#define DxFxxE4_xA0_LcDelayCount_WIDTH                          2
#define DxFxxE4_xA0_LcDelayCount_MASK                           0x6000000
#define DxFxxE4_xA0_LcDelayL0sExit_OFFSET                       27
#define DxFxxE4_xA0_LcDelayL0sExit_WIDTH                        1
#define DxFxxE4_xA0_LcDelayL0sExit_MASK                         0x8000000
#define DxFxxE4_xA0_LcDelayL1Exit_OFFSET                        28
#define DxFxxE4_xA0_LcDelayL1Exit_WIDTH                         1
#define DxFxxE4_xA0_LcDelayL1Exit_MASK                          0x10000000
#define DxFxxE4_xA0_LcExtendWaitForElIdle_OFFSET                29
#define DxFxxE4_xA0_LcExtendWaitForElIdle_WIDTH                 1
#define DxFxxE4_xA0_LcExtendWaitForElIdle_MASK                  0x20000000
#define DxFxxE4_xA0_LcEscapeL1L23En_OFFSET                      30
#define DxFxxE4_xA0_LcEscapeL1L23En_WIDTH                       1
#define DxFxxE4_xA0_LcEscapeL1L23En_MASK                        0x40000000
#define DxFxxE4_xA0_LcGateRcvrIdle_OFFSET                       31
#define DxFxxE4_xA0_LcGateRcvrIdle_WIDTH                        1
#define DxFxxE4_xA0_LcGateRcvrIdle_MASK                         0x80000000

/// DxFxxE4_xA0
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved_0_0:1 ; ///<
    UINT32                                       LcDontEnterL23InD0:1 ; ///<
    UINT32                                      LcResetlIdleCountEn:1 ; ///<
    UINT32                                              LcResetLink:1 ; ///<
    UINT32                                         Lc16xClearTxPipe:4 ; ///<
    UINT32                                          LcL0sInactivity:4 ; ///<
    UINT32                                           LcL1Inactivity:4 ; ///<
    UINT32                                             LcPmiToL1Dis:1 ; ///<
    UINT32                                              LcIncNFtsEn:1 ; ///<
    UINT32                                     LcLookForIdleInL1L23:2 ; ///<
    UINT32                                        LcFactorInExtSync:1 ; ///<
    UINT32                                        LcWaitForPmAckDis:1 ; ///<
    UINT32                                            LcWakeFromL23:1 ; ///<
    UINT32                                         LcL1ImmediateAck:1 ; ///<
    UINT32                                            LcAspmToL1Dis:1 ; ///<
    UINT32                                             LcDelayCount:2 ; ///<
    UINT32                                           LcDelayL0sExit:1 ; ///<
    UINT32                                            LcDelayL1Exit:1 ; ///<
    UINT32                                    LcExtendWaitForElIdle:1 ; ///<
    UINT32                                          LcEscapeL1L23En:1 ; ///<
    UINT32                                           LcGateRcvrIdle:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xA0_STRUCT;

// **** DxFxxE4_xA1 Register Definition ****
// Address
#define DxFxxE4_xA1_ADDRESS                                     0xA1
// Type
#define DxFxxE4_xA1_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA1_Reserved_10_0_OFFSET                        0
#define DxFxxE4_xA1_Reserved_10_0_WIDTH                         11
#define DxFxxE4_xA1_Reserved_10_0_MASK                          0x7FF
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_OFFSET               11
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_WIDTH                1
#define DxFxxE4_xA1_LcDontGotoL0sifL1Armed_MASK                 0x800
#define DxFxxE4_xA1_Reserved_31_12_OFFSET                       12
#define DxFxxE4_xA1_Reserved_31_12_WIDTH                        4
#define DxFxxE4_xA1_Reserved_31_12_MASK                         0xF000
#define DxFxxE4_xA1_LcExtendWaitForSkp_OFFSET                   16
#define DxFxxE4_xA1_LcExtendWaitForSkp_WIDTH                    1
#define DxFxxE4_xA1_LcExtendWaitForSkp_MASK                     0x10000
#define DxFxxE4_xA1_Reserved_31_17_OFFSET                       17
#define DxFxxE4_xA1_Reserved_31_17_WIDTH                        15
#define DxFxxE4_xA1_Reserved_31_17_MASK                         0xFFFE0000

/// DxFxxE4_xA1
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_10_0:11; ///<
    UINT32                                  LcDontGotoL0sifL1Armed:1 ; ///<
    UINT32                                          Reserved_15_12:4 ; ///<
    UINT32                                      LcExtendWaitForSkp:1 ; ///<
    UINT32                                          Reserved_31_17:15; ///<
  } Field;

  UINT32 Value;
} DxFxxE4_xA1_STRUCT;

// **** DxFxxE4_xA2 Register Definition ****
// Address
#define DxFxxE4_xA2_ADDRESS                                     0xA2
// Type
#define DxFxxE4_xA2_TYPE                                        TYPE_D2F1xE4

// Field Data
#define DxFxxE4_xA2_LcLinkWidth_OFFSET                          0
#define DxFxxE4_xA2_LcLinkWidth_WIDTH                           3
#define DxFxxE4_xA2_LcLinkWidth_MASK                            0x7
#define DxFxxE4_xA2_Reserved_3_3_OFFSET                         3
#define DxFxxE4_xA2_Reserved_3_3_WIDTH                          1
#define DxFxxE4_xA2_Reserved_3_3_MASK                           0x8
#define DxFxxE4_xA2_LcLinkWidthRd_OFFSET                        4
#define DxFxxE4_xA2_LcLinkWidthRd_WIDTH                         3
#define DxFxxE4_xA2_LcLinkWidthRd_MASK                          0x70
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_OFFSET           7
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_WIDTH            1
#define DxFxxE4_xA2_LcReconfigArcMissingEscape_MASK             0x80
#define DxFxxE4_xA2_LcReconfigNow_OFFSET                        8
#define DxFxxE4_xA2_LcReconfigNow_WIDTH                         1
#define DxFxxE4_xA2_LcReconfigNow_MASK                          0x100
#define DxFxxE4_xA2_LcRenegotiationSupport_OFFSET               9
#define DxFxxE4_xA2_LcRenegotiationSupport_WIDTH                1
#define DxFxxE4_xA2_LcRenegotiationSupport_MASK                 0x200
#define DxFxxE4_xA2_LcRenegotiateEn_OFFSET                      10
#define DxFxxE4_xA2_LcRenegotiateEn_WIDTH                       1
#define DxFxxE4_xA2_LcRenegotiateEn_MASK                        0x400
#define DxFxxE4_xA2_LcShortReconfigEn_OFFSET                    11
#define DxFxxE4_xA2_LcShortReconfigEn_WIDTH                     1
#define DxFxxE4_xA2_LcShortReconfigEn_MASK                      0x800
#define DxFxxE4_xA2_LcUpconfigureSupport_OFFSET                 12
#define DxFxxE4_xA2_LcUpconfigureSupport_WIDTH                  1
#define DxFxxE4_xA2_LcUpconfigureSupport_MASK                   0x1000
#define DxFxxE4_xA2_LcUpconfigureDis_OFFSET                     13
#define DxFxxE4_xA2_LcUpconfigureDis_WIDTH                      1
#define DxFxxE4_xA2_LcUpconfigureDis_MASK                       0x2000
#define DxFxxE4_xA2_Reserved_19_14_OFFSET                       14
#define DxFxxE4_xA2_Reserved_19_14_WIDTH                        6
#define DxFxxE4_xA2_Reserved_19_14_MASK                         0xFC000
#define DxFxxE4_xA2_LcUpconfigCapable_OFFSET                    20
#define DxFxxE4_xA2_LcUpconfigCapable_WIDTH                     1
#define DxFxxE4_xA2_LcUpconfigCapable_MASK                      0x100000
#define DxFxxE4_xA2_LcDynLanesPwrState_OFFSET                   21
#define DxFxxE4_xA2_LcDynLanesPwrState_WIDTH                    2
#define DxFxxE4_xA2_LcDynLanesPwrState_MASK                     0x600000
#define DxFxxE4_xA2_Reserved_31_23_OFFSET                       23
#define DxFxxE4_xA2_Reserved_31_23_WIDTH                        9
#define DxFxxE4_xA2_Reserved_31_23_MASK                         0xFF800000

/// DxFxxE4_xA2
typedef union {
  struct {                                                            ///<
    UINT32                                             LcLinkWidth:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                           LcLinkWidthRd:3; ///<
    UINT32                              LcReconfigArcMissingEscape:1; ///<
    UINT32                                           LcReconfigNow:1; ///<
    UINT32                                  LcRenegotiationSupport:1; ///<
    UINT32                                         LcRenegotiateEn:1; ///<
    UINT32                                       LcShortReconfigEn:1; ///<
    UINT32                                    LcUpconfigureSupport:1; ///<
    UINT32                                        LcUpconfigureDis:1; ///<
    UINT32                                          Reserved_19_14:6; ///<
    UINT32                                       LcUpconfigCapable:1; ///<
    UINT32                                      LcDynLanesPwrState:2; ///<
    UINT32                                          Reserved_31_23:9; ///<

  } Field;

  UINT32 Value;
} DxFxxE4_xA2_STRUCT;

// **** DxFxxE4_xA3 Register Definition ****
// Address
#define DxFxxE4_xA3_ADDRESS                                     0xa3

// Type
#define DxFxxE4_xA3_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xA3_LcXmitNFts_OFFSET                           0
#define DxFxxE4_xA3_LcXmitNFts_WIDTH                            8
#define DxFxxE4_xA3_LcXmitNFts_MASK                             0xff
#define DxFxxE4_xA3_LcXmitNFtsOverrideEn_OFFSET                 8
#define DxFxxE4_xA3_LcXmitNFtsOverrideEn_WIDTH                  1
#define DxFxxE4_xA3_LcXmitNFtsOverrideEn_MASK                   0x100
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_OFFSET              9
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_WIDTH               1
#define DxFxxE4_xA3_LcXmitFtsBeforeRecovery_MASK                0x200
#define DxFxxE4_xA3_Reserved_15_10_OFFSET                       10
#define DxFxxE4_xA3_Reserved_15_10_WIDTH                        6
#define DxFxxE4_xA3_Reserved_15_10_MASK                         0xfc00
#define DxFxxE4_xA3_LcXmitNFtsLimit_OFFSET                      16
#define DxFxxE4_xA3_LcXmitNFtsLimit_WIDTH                       8
#define DxFxxE4_xA3_LcXmitNFtsLimit_MASK                        0xff0000
#define DxFxxE4_xA3_LcNFts_OFFSET                               24
#define DxFxxE4_xA3_LcNFts_WIDTH                                8
#define DxFxxE4_xA3_LcNFts_MASK                                 0xff000000

/// DxFxxE4_xA3
typedef union {
  struct {                                                              ///<
    UINT32                                               LcXmitNFts:8 ; ///<
    UINT32                                     LcXmitNFtsOverrideEn:1 ; ///<
    UINT32                                  LcXmitFtsBeforeRecovery:1 ; ///<
    UINT32                                           Reserved_15_10:6 ; ///<
    UINT32                                          LcXmitNFtsLimit:8 ; ///<
    UINT32                                                   LcNFts:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xA3_STRUCT;

// **** DxFxxE4_xA4 Register Definition ****
// Address
#define DxFxxE4_xA4_ADDRESS                                     0xa4

// Type
#define DxFxxE4_xA4_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xA4_LcGen2EnStrap_OFFSET                        0
#define DxFxxE4_xA4_LcGen2EnStrap_WIDTH                         1
#define DxFxxE4_xA4_LcGen2EnStrap_MASK                          0x1
#define DxFxxE4_xA4_LcGen3EnStrap_OFFSET                        1
#define DxFxxE4_xA4_LcGen3EnStrap_WIDTH                         1
#define DxFxxE4_xA4_LcGen3EnStrap_MASK                          0x2
#define DxFxxE4_xA4_Reserved_5_2_OFFSET                         2
#define DxFxxE4_xA4_Reserved_5_2_WIDTH                          4
#define DxFxxE4_xA4_Reserved_5_2_MASK                           0x3c
#define DxFxxE4_xA4_LcForceDisSwSpeedChange_OFFSET              6
#define DxFxxE4_xA4_LcForceDisSwSpeedChange_WIDTH               1
#define DxFxxE4_xA4_LcForceDisSwSpeedChange_MASK                0x40
#define DxFxxE4_xA4_Reserved_8_7_OFFSET                         7
#define DxFxxE4_xA4_Reserved_8_7_WIDTH                          2
#define DxFxxE4_xA4_Reserved_8_7_MASK                           0x180
#define DxFxxE4_xA4_LcInitiateLinkSpeedChange_OFFSET            9
#define DxFxxE4_xA4_LcInitiateLinkSpeedChange_WIDTH             1
#define DxFxxE4_xA4_LcInitiateLinkSpeedChange_MASK              0x200
#define DxFxxE4_xA4_Reserved_11_10_OFFSET                       10
#define DxFxxE4_xA4_Reserved_11_10_WIDTH                        2
#define DxFxxE4_xA4_Reserved_11_10_MASK                         0xc00
#define DxFxxE4_xA4_LcSpeedChangeAttemptFailed_OFFSET           12
#define DxFxxE4_xA4_LcSpeedChangeAttemptFailed_WIDTH            1
#define DxFxxE4_xA4_LcSpeedChangeAttemptFailed_MASK             0x1000
#define DxFxxE4_xA4_LcCurrentDataRate_OFFSET                    13
#define DxFxxE4_xA4_LcCurrentDataRate_WIDTH                     2
#define DxFxxE4_xA4_LcCurrentDataRate_MASK                      0x6000
#define DxFxxE4_xA4_Reserved_18_15_OFFSET                       15
#define DxFxxE4_xA4_Reserved_18_15_WIDTH                        4
#define DxFxxE4_xA4_Reserved_18_15_MASK                         0x78000
#define DxFxxE4_xA4_LcOtherSideSupportsGen2_OFFSET              19
#define DxFxxE4_xA4_LcOtherSideSupportsGen2_WIDTH               1
#define DxFxxE4_xA4_LcOtherSideSupportsGen2_MASK                0x80000
#define DxFxxE4_xA4_Reserved_26_20_OFFSET                       20
#define DxFxxE4_xA4_Reserved_26_20_WIDTH                        7
#define DxFxxE4_xA4_Reserved_26_20_MASK                         0x7f00000
#define DxFxxE4_xA4_LcMultUpstreamAutoSpdChngEn_OFFSET          27
#define DxFxxE4_xA4_LcMultUpstreamAutoSpdChngEn_WIDTH           1
#define DxFxxE4_xA4_LcMultUpstreamAutoSpdChngEn_MASK            0x8000000
#define DxFxxE4_xA4_Reserved_31_28_OFFSET                       28
#define DxFxxE4_xA4_Reserved_31_28_WIDTH                        4
#define DxFxxE4_xA4_Reserved_31_28_MASK                         0xf0000000

/// DxFxxE4_xA4
typedef union {
  struct {                                                              ///<
    UINT32                                            LcGen2EnStrap:1 ; ///<
    UINT32                                            LcGen3EnStrap:1 ; ///<
    UINT32                                             Reserved_5_2:4 ; ///<
    UINT32                                  LcForceDisSwSpeedChange:1 ; ///<
    UINT32                                             Reserved_8_7:2 ; ///<
    UINT32                                LcInitiateLinkSpeedChange:1 ; ///<
    UINT32                                           Reserved_11_10:2 ; ///<
    UINT32                               LcSpeedChangeAttemptFailed:1 ; ///<
    UINT32                                        LcCurrentDataRate:2 ; ///<
    UINT32                                           Reserved_18_15:4 ; ///<
    UINT32                                  LcOtherSideSupportsGen2:1 ; ///<
    UINT32                                           Reserved_26_20:7 ; ///<
    UINT32                              LcMultUpstreamAutoSpdChngEn:1 ; ///<
    UINT32                                           Reserved_31_28:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xA4_STRUCT;

// **** DxFxxE4_xB1 Register Definition ****
// Address
#define DxFxxE4_xB1_ADDRESS                                     0xb1

// Type
#define DxFxxE4_xB1_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xB1_LcTimedOutState_OFFSET                      0
#define DxFxxE4_xB1_LcTimedOutState_WIDTH                       6
#define DxFxxE4_xB1_LcTimedOutState_MASK                        0x3f
#define DxFxxE4_xB1_LcTimedOut_OFFSET                           6
#define DxFxxE4_xB1_LcTimedOut_WIDTH                            1
#define DxFxxE4_xB1_LcTimedOut_MASK                             0x40
#define DxFxxE4_xB1_LcLookForBwReduction_OFFSET                 7
#define DxFxxE4_xB1_LcLookForBwReduction_WIDTH                  1
#define DxFxxE4_xB1_LcLookForBwReduction_MASK                   0x80
#define DxFxxE4_xB1_LcMoreTs2En_OFFSET                          8
#define DxFxxE4_xB1_LcMoreTs2En_WIDTH                           1
#define DxFxxE4_xB1_LcMoreTs2En_MASK                            0x100
#define DxFxxE4_xB1_LcX12NegotiationDis_OFFSET                  9
#define DxFxxE4_xB1_LcX12NegotiationDis_WIDTH                   1
#define DxFxxE4_xB1_LcX12NegotiationDis_MASK                    0x200
#define DxFxxE4_xB1_LcLinkUpReversalEn_OFFSET                   10
#define DxFxxE4_xB1_LcLinkUpReversalEn_WIDTH                    1
#define DxFxxE4_xB1_LcLinkUpReversalEn_MASK                     0x400
#define DxFxxE4_xB1_LcIllegalState_OFFSET                       11
#define DxFxxE4_xB1_LcIllegalState_WIDTH                        1
#define DxFxxE4_xB1_LcIllegalState_MASK                         0x800
#define DxFxxE4_xB1_LcIllegalStateRestartEn_OFFSET              12
#define DxFxxE4_xB1_LcIllegalStateRestartEn_WIDTH               1
#define DxFxxE4_xB1_LcIllegalStateRestartEn_MASK                0x1000
#define DxFxxE4_xB1_LcWaitForOtherLanesMode_OFFSET              13
#define DxFxxE4_xB1_LcWaitForOtherLanesMode_WIDTH               1
#define DxFxxE4_xB1_LcWaitForOtherLanesMode_MASK                0x2000
#define DxFxxE4_xB1_LcElecIdleMode_OFFSET                       14
#define DxFxxE4_xB1_LcElecIdleMode_WIDTH                        2
#define DxFxxE4_xB1_LcElecIdleMode_MASK                         0xc000
#define DxFxxE4_xB1_LcDisableInferredElecIdleDetect_OFFSET      16
#define DxFxxE4_xB1_LcDisableInferredElecIdleDetect_WIDTH       1
#define DxFxxE4_xB1_LcDisableInferredElecIdleDetect_MASK        0x10000
#define DxFxxE4_xB1_Reserved_18_17_OFFSET                       17
#define DxFxxE4_xB1_Reserved_18_17_WIDTH                        2
#define DxFxxE4_xB1_Reserved_18_17_MASK                         0x60000
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_OFFSET                  19
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_WIDTH                   1
#define DxFxxE4_xB1_LcDeassertRxEnInL0s_MASK                    0x80000
#define DxFxxE4_xB1_LcBlockElIdleinL0_OFFSET                    20
#define DxFxxE4_xB1_LcBlockElIdleinL0_WIDTH                     1
#define DxFxxE4_xB1_LcBlockElIdleinL0_MASK                      0x100000
#define DxFxxE4_xB1_LcBlockElIdleinL0_VALUE                     0x1
#define DxFxxE4_xB1_LcRcvL0toRcvL0sDis_OFFSET                   21
#define DxFxxE4_xB1_LcRcvL0toRcvL0sDis_WIDTH                    1
#define DxFxxE4_xB1_LcRcvL0toRcvL0sDis_MASK                     0x200000
#define DxFxxE4_xB1_LcAssertInactiveDuringHold_OFFSET           22
#define DxFxxE4_xB1_LcAssertInactiveDuringHold_WIDTH            1
#define DxFxxE4_xB1_LcAssertInactiveDuringHold_MASK             0x400000
#define DxFxxE4_xB1_LcWaitForLanesInLwNeg_OFFSET                23
#define DxFxxE4_xB1_LcWaitForLanesInLwNeg_WIDTH                 2
#define DxFxxE4_xB1_LcWaitForLanesInLwNeg_MASK                  0x1800000
#define DxFxxE4_xB1_LcPwrDownNegOffLanes_OFFSET                 25
#define DxFxxE4_xB1_LcPwrDownNegOffLanes_WIDTH                  1
#define DxFxxE4_xB1_LcPwrDownNegOffLanes_MASK                   0x2000000
#define DxFxxE4_xB1_LcDisableLostSymLockArcs_OFFSET             26
#define DxFxxE4_xB1_LcDisableLostSymLockArcs_WIDTH              1
#define DxFxxE4_xB1_LcDisableLostSymLockArcs_MASK               0x4000000
#define DxFxxE4_xB1_LcLinkBwNotificationDis_OFFSET              27
#define DxFxxE4_xB1_LcLinkBwNotificationDis_WIDTH               1
#define DxFxxE4_xB1_LcLinkBwNotificationDis_MASK                0x8000000
#define DxFxxE4_xB1_Reserved_28_28_OFFSET                       28
#define DxFxxE4_xB1_Reserved_28_28_WIDTH                        1
#define DxFxxE4_xB1_Reserved_28_28_MASK                         0x10000000
#define DxFxxE4_xB1_LcTestTimerSel_OFFSET                       29
#define DxFxxE4_xB1_LcTestTimerSel_WIDTH                        2
#define DxFxxE4_xB1_LcTestTimerSel_MASK                         0x60000000
#define DxFxxE4_xB1_ElecIdleForPi_OFFSET                        31
#define DxFxxE4_xB1_ElecIdleForPi_WIDTH                         1
#define DxFxxE4_xB1_ElecIdleForPi_MASK                          0x80000000

/// DxFxxE4_xB1
typedef union {
  struct {                                                              ///<
    UINT32                                          LcTimedOutState:6 ; ///<
    UINT32                                               LcTimedOut:1 ; ///<
    UINT32                                     LcLookForBwReduction:1 ; ///<
    UINT32                                              LcMoreTs2En:1 ; ///<
    UINT32                                      LcX12NegotiationDis:1 ; ///<
    UINT32                                       LcLinkUpReversalEn:1 ; ///<
    UINT32                                           LcIllegalState:1 ; ///<
    UINT32                                  LcIllegalStateRestartEn:1 ; ///<
    UINT32                                  LcWaitForOtherLanesMode:1 ; ///<
    UINT32                                           LcElecIdleMode:2 ; ///<
    UINT32                          LcDisableInferredElecIdleDetect:1 ; ///<
    UINT32                                           Reserved_18_17:2 ; ///<
    UINT32                                      LcDeassertRxEnInL0s:1 ; ///<
    UINT32                                        LcBlockElIdleinL0:1 ; ///<
    UINT32                                       LcRcvL0toRcvL0sDis:1 ; ///<
    UINT32                               LcAssertInactiveDuringHold:1 ; ///<
    UINT32                                    LcWaitForLanesInLwNeg:2 ; ///<
    UINT32                                     LcPwrDownNegOffLanes:1 ; ///<
    UINT32                                 LcDisableLostSymLockArcs:1 ; ///<
    UINT32                                  LcLinkBwNotificationDis:1 ; ///<
    UINT32                                           Reserved_28_28:1 ; ///<
    UINT32                                           LcTestTimerSel:2 ; ///<
    UINT32                                            ElecIdleForPi:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xB1_STRUCT;

// **** DxFxxE4_xB5 Register Definition ****
// Address
#define DxFxxE4_xB5_ADDRESS                                     0xb5

// Type
#define DxFxxE4_xB5_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xB5_LcSelectDeemphasis_OFFSET                   0
#define DxFxxE4_xB5_LcSelectDeemphasis_WIDTH                    1
#define DxFxxE4_xB5_LcSelectDeemphasis_MASK                     0x1
#define DxFxxE4_xB5_LcSelectDeemphasisCntl_OFFSET               1
#define DxFxxE4_xB5_LcSelectDeemphasisCntl_WIDTH                2
#define DxFxxE4_xB5_LcSelectDeemphasisCntl_MASK                 0x6
#define DxFxxE4_xB5_LcRcvdDeemphasis_OFFSET                     3
#define DxFxxE4_xB5_LcRcvdDeemphasis_WIDTH                      1
#define DxFxxE4_xB5_LcRcvdDeemphasis_MASK                       0x8
#define DxFxxE4_xB5_LcCompToDetect_OFFSET                       4
#define DxFxxE4_xB5_LcCompToDetect_WIDTH                        1
#define DxFxxE4_xB5_LcCompToDetect_MASK                         0x10
#define DxFxxE4_xB5_LcResetTsxCntInRlockEn_OFFSET               5
#define DxFxxE4_xB5_LcResetTsxCntInRlockEn_WIDTH                1
#define DxFxxE4_xB5_LcResetTsxCntInRlockEn_MASK                 0x20
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptsAllowed_OFFSET     6
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptsAllowed_WIDTH      2
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptsAllowed_MASK       0xc0
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptFailed_OFFSET       8
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptFailed_WIDTH        1
#define DxFxxE4_xB5_LcAutoSpeedChangeAttemptFailed_MASK         0x100
#define DxFxxE4_xB5_LcClrFailedAutoSpdChangeCnt_OFFSET          9
#define DxFxxE4_xB5_LcClrFailedAutoSpdChangeCnt_WIDTH           1
#define DxFxxE4_xB5_LcClrFailedAutoSpdChangeCnt_MASK            0x200
#define DxFxxE4_xB5_LcEnhancedHotPlugEn_OFFSET                  10
#define DxFxxE4_xB5_LcEnhancedHotPlugEn_WIDTH                   1
#define DxFxxE4_xB5_LcEnhancedHotPlugEn_MASK                    0x400
#define DxFxxE4_xB5_LcRcvrDetEnOverride_OFFSET                  11
#define DxFxxE4_xB5_LcRcvrDetEnOverride_WIDTH                   1
#define DxFxxE4_xB5_LcRcvrDetEnOverride_MASK                    0x800
#define DxFxxE4_xB5_LcEhpRxPhyCmd_OFFSET                        12
#define DxFxxE4_xB5_LcEhpRxPhyCmd_WIDTH                         2
#define DxFxxE4_xB5_LcEhpRxPhyCmd_MASK                          0x3000
#define DxFxxE4_xB5_LcEhpTxPhyCmd_OFFSET                        14
#define DxFxxE4_xB5_LcEhpTxPhyCmd_WIDTH                         2
#define DxFxxE4_xB5_LcEhpTxPhyCmd_MASK                          0xc000
#define DxFxxE4_xB5_LcChipBifUsbIdleEn_OFFSET                   16
#define DxFxxE4_xB5_LcChipBifUsbIdleEn_WIDTH                    1
#define DxFxxE4_xB5_LcChipBifUsbIdleEn_MASK                     0x10000
#define DxFxxE4_xB5_LcChipBifUsbIdleEn_VALUE                    0x0
#define DxFxxE4_xB5_LcL1BlockReconfigEn_OFFSET                  17
#define DxFxxE4_xB5_LcL1BlockReconfigEn_WIDTH                   1
#define DxFxxE4_xB5_LcL1BlockReconfigEn_MASK                    0x20000
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportEn_OFFSET          18
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportEn_WIDTH           1
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportEn_MASK            0x40000
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportMaxFailSel_OFFSET  19
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportMaxFailSel_WIDTH   2
#define DxFxxE4_xB5_LcAutoDisableSpeedSupportMaxFailSel_MASK    0x180000
#define DxFxxE4_xB5_LcFastL1EntryExitEn_OFFSET                  21
#define DxFxxE4_xB5_LcFastL1EntryExitEn_WIDTH                   1
#define DxFxxE4_xB5_LcFastL1EntryExitEn_MASK                    0x200000
#define DxFxxE4_xB5_LcRxPhyCmdInactiveEnMode_OFFSET             22
#define DxFxxE4_xB5_LcRxPhyCmdInactiveEnMode_WIDTH              1
#define DxFxxE4_xB5_LcRxPhyCmdInactiveEnMode_MASK               0x400000
#define DxFxxE4_xB5_LcDscDontEnterL23AfterPmeAck_OFFSET         23
#define DxFxxE4_xB5_LcDscDontEnterL23AfterPmeAck_WIDTH          1
#define DxFxxE4_xB5_LcDscDontEnterL23AfterPmeAck_MASK           0x800000
#define DxFxxE4_xB5_LcHwVoltageIfControl_OFFSET                 24
#define DxFxxE4_xB5_LcHwVoltageIfControl_WIDTH                  2
#define DxFxxE4_xB5_LcHwVoltageIfControl_MASK                   0x3000000
#define DxFxxE4_xB5_LcVoltageTimerSel_OFFSET                    26
#define DxFxxE4_xB5_LcVoltageTimerSel_WIDTH                     4
#define DxFxxE4_xB5_LcVoltageTimerSel_MASK                      0x3c000000
#define DxFxxE4_xB5_LcGoToRecovery_OFFSET                       30
#define DxFxxE4_xB5_LcGoToRecovery_WIDTH                        1
#define DxFxxE4_xB5_LcGoToRecovery_MASK                         0x40000000
#define DxFxxE4_xB5_LcNEieSel_OFFSET                            31
#define DxFxxE4_xB5_LcNEieSel_WIDTH                             1
#define DxFxxE4_xB5_LcNEieSel_MASK                              0x80000000

/// DxFxxE4_xB5
typedef union {
  struct {                                                              ///<
    UINT32                                       LcSelectDeemphasis:1 ; ///<
    UINT32                                   LcSelectDeemphasisCntl:2 ; ///<
    UINT32                                         LcRcvdDeemphasis:1 ; ///<
    UINT32                                           LcCompToDetect:1 ; ///<
    UINT32                                   LcResetTsxCntInRlockEn:1 ; ///<
    UINT32                         LcAutoSpeedChangeAttemptsAllowed:2 ; ///<
    UINT32                           LcAutoSpeedChangeAttemptFailed:1 ; ///<
    UINT32                              LcClrFailedAutoSpdChangeCnt:1 ; ///<
    UINT32                                      LcEnhancedHotPlugEn:1 ; ///<
    UINT32                                      LcRcvrDetEnOverride:1 ; ///<
    UINT32                                            LcEhpRxPhyCmd:2 ; ///<
    UINT32                                            LcEhpTxPhyCmd:2 ; ///<
    UINT32                                       LcChipBifUsbIdleEn:1 ; ///<
    UINT32                                      LcL1BlockReconfigEn:1 ; ///<
    UINT32                              LcAutoDisableSpeedSupportEn:1 ; ///<
    UINT32                      LcAutoDisableSpeedSupportMaxFailSel:2 ; ///<
    UINT32                                      LcFastL1EntryExitEn:1 ; ///<
    UINT32                                 LcRxPhyCmdInactiveEnMode:1 ; ///<
    UINT32                             LcDscDontEnterL23AfterPmeAck:1 ; ///<
    UINT32                                     LcHwVoltageIfControl:2 ; ///<
    UINT32                                        LcVoltageTimerSel:4 ; ///<
    UINT32                                           LcGoToRecovery:1 ; ///<
    UINT32                                                LcNEieSel:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xB5_STRUCT;

// **** DxFxxE4_xB6 Register Definition ****
// Address
#define DxFxxE4_xB6_ADDRESS                                     0xb6

// Type
#define DxFxxE4_xB6_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xB6_LcTxEnableBehaviour_OFFSET                  0
#define DxFxxE4_xB6_LcTxEnableBehaviour_WIDTH                   2
#define DxFxxE4_xB6_LcTxEnableBehaviour_MASK                    0x3
#define DxFxxE4_xB6_Reserved_2_2_OFFSET                         2
#define DxFxxE4_xB6_Reserved_2_2_WIDTH                          1
#define DxFxxE4_xB6_Reserved_2_2_MASK                           0x4
#define DxFxxE4_xB6_LcGatePmiL1WithSlvIdle_OFFSET               3
#define DxFxxE4_xB6_LcGatePmiL1WithSlvIdle_WIDTH                1
#define DxFxxE4_xB6_LcGatePmiL1WithSlvIdle_MASK                 0x8
#define DxFxxE4_xB6_LcBypassEq_OFFSET                           4
#define DxFxxE4_xB6_LcBypassEq_WIDTH                            1
#define DxFxxE4_xB6_LcBypassEq_MASK                             0x10
#define DxFxxE4_xB6_LcRedoEq_OFFSET                             5
#define DxFxxE4_xB6_LcRedoEq_WIDTH                              1
#define DxFxxE4_xB6_LcRedoEq_MASK                               0x20
#define DxFxxE4_xB6_LcExtendEieos_OFFSET                        6
#define DxFxxE4_xB6_LcExtendEieos_WIDTH                         1
#define DxFxxE4_xB6_LcExtendEieos_MASK                          0x40
#define DxFxxE4_xB6_LcIgnoreParity_OFFSET                       7
#define DxFxxE4_xB6_LcIgnoreParity_WIDTH                        1
#define DxFxxE4_xB6_LcIgnoreParity_MASK                         0x80
#define DxFxxE4_xB6_LcEqSearchMode_OFFSET                       8
#define DxFxxE4_xB6_LcEqSearchMode_WIDTH                        2
#define DxFxxE4_xB6_LcEqSearchMode_MASK                         0x300
#define DxFxxE4_xB6_LcDscCheckCoeffsInRlock_OFFSET              10
#define DxFxxE4_xB6_LcDscCheckCoeffsInRlock_WIDTH               1
#define DxFxxE4_xB6_LcDscCheckCoeffsInRlock_MASK                0x400
#define DxFxxE4_xB6_LcUscEqNotReqd_OFFSET                       11
#define DxFxxE4_xB6_LcUscEqNotReqd_WIDTH                        1
#define DxFxxE4_xB6_LcUscEqNotReqd_MASK                         0x800
#define DxFxxE4_xB6_LcUscGoToEq_OFFSET                          12
#define DxFxxE4_xB6_LcUscGoToEq_WIDTH                           1
#define DxFxxE4_xB6_LcUscGoToEq_MASK                            0x1000
#define DxFxxE4_xB6_LcSetQuiesce_OFFSET                         13
#define DxFxxE4_xB6_LcSetQuiesce_WIDTH                          1
#define DxFxxE4_xB6_LcSetQuiesce_MASK                           0x2000
#define DxFxxE4_xB6_LcQuiesceRcvd_OFFSET                        14
#define DxFxxE4_xB6_LcQuiesceRcvd_WIDTH                         1
#define DxFxxE4_xB6_LcQuiesceRcvd_MASK                          0x4000
#define DxFxxE4_xB6_LcUnexpectedCoeffsRcvd_OFFSET               15
#define DxFxxE4_xB6_LcUnexpectedCoeffsRcvd_WIDTH                1
#define DxFxxE4_xB6_LcUnexpectedCoeffsRcvd_MASK                 0x8000
#define DxFxxE4_xB6_LcBypassEqReqPhase_OFFSET                   16
#define DxFxxE4_xB6_LcBypassEqReqPhase_WIDTH                    1
#define DxFxxE4_xB6_LcBypassEqReqPhase_MASK                     0x10000
#define DxFxxE4_xB6_LcForcePresetInEqReqPhase_OFFSET            17
#define DxFxxE4_xB6_LcForcePresetInEqReqPhase_WIDTH             1
#define DxFxxE4_xB6_LcForcePresetInEqReqPhase_MASK              0x20000
#define DxFxxE4_xB6_LcForcePresetValue_OFFSET                   18
#define DxFxxE4_xB6_LcForcePresetValue_WIDTH                    4
#define DxFxxE4_xB6_LcForcePresetValue_MASK                     0x3c0000
#define DxFxxE4_xB6_LcUscDelayDllps_OFFSET                      22
#define DxFxxE4_xB6_LcUscDelayDllps_WIDTH                       1
#define DxFxxE4_xB6_LcUscDelayDllps_MASK                        0x400000
#define DxFxxE4_xB6_LcPcieTxFullSwing_OFFSET                    23
#define DxFxxE4_xB6_LcPcieTxFullSwing_WIDTH                     1
#define DxFxxE4_xB6_LcPcieTxFullSwing_MASK                      0x800000
#define DxFxxE4_xB6_LcEqWaitForEvalDone_OFFSET                  24
#define DxFxxE4_xB6_LcEqWaitForEvalDone_WIDTH                   1
#define DxFxxE4_xB6_LcEqWaitForEvalDone_MASK                    0x1000000
#define DxFxxE4_xB6_Lc8gtSkipOrderEn_OFFSET                     25
#define DxFxxE4_xB6_Lc8gtSkipOrderEn_WIDTH                      1
#define DxFxxE4_xB6_Lc8gtSkipOrderEn_MASK                       0x2000000
#define DxFxxE4_xB6_LcWaitForMoreTsInRlock_OFFSET               26
#define DxFxxE4_xB6_LcWaitForMoreTsInRlock_WIDTH                6
#define DxFxxE4_xB6_LcWaitForMoreTsInRlock_MASK                 0xfc000000

/// DxFxxE4_xB6
typedef union {
  struct {                                                              ///<
    UINT32                                      LcTxEnableBehaviour:2 ; ///<
    UINT32                                             Reserved_2_2:1 ; ///<
    UINT32                                   LcGatePmiL1WithSlvIdle:1 ; ///<
    UINT32                                               LcBypassEq:1 ; ///<
    UINT32                                                 LcRedoEq:1 ; ///<
    UINT32                                            LcExtendEieos:1 ; ///<
    UINT32                                           LcIgnoreParity:1 ; ///<
    UINT32                                           LcEqSearchMode:2 ; ///<
    UINT32                                  LcDscCheckCoeffsInRlock:1 ; ///<
    UINT32                                           LcUscEqNotReqd:1 ; ///<
    UINT32                                              LcUscGoToEq:1 ; ///<
    UINT32                                             LcSetQuiesce:1 ; ///<
    UINT32                                            LcQuiesceRcvd:1 ; ///<
    UINT32                                   LcUnexpectedCoeffsRcvd:1 ; ///<
    UINT32                                       LcBypassEqReqPhase:1 ; ///<
    UINT32                                LcForcePresetInEqReqPhase:1 ; ///<
    UINT32                                       LcForcePresetValue:4 ; ///<
    UINT32                                          LcUscDelayDllps:1 ; ///<
    UINT32                                        LcPcieTxFullSwing:1 ; ///<
    UINT32                                      LcEqWaitForEvalDone:1 ; ///<
    UINT32                                         Lc8gtSkipOrderEn:1 ; ///<
    UINT32                                   LcWaitForMoreTsInRlock:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xB6_STRUCT;

// **** DxFxxE4_xC0 Register Definition ****
// Address
#define DxFxxE4_xC0_ADDRESS                                     0xc0

// Type
#define DxFxxE4_xC0_TYPE                                        TYPE_D4F1xE4
// Field Data
#define DxFxxE4_xC0_StrapFTSyTSxCount_OFFSET                    0
#define DxFxxE4_xC0_StrapFTSyTSxCount_WIDTH                     2
#define DxFxxE4_xC0_StrapFTSyTSxCount_MASK                      0x3
#define DxFxxE4_xC0_StrapLongyTSxCount_OFFSET                   2
#define DxFxxE4_xC0_StrapLongyTSxCount_WIDTH                    2
#define DxFxxE4_xC0_StrapLongyTSxCount_MASK                     0xc
#define DxFxxE4_xC0_StrapMedyTSxCount_OFFSET                    4
#define DxFxxE4_xC0_StrapMedyTSxCount_WIDTH                     2
#define DxFxxE4_xC0_StrapMedyTSxCount_MASK                      0x30
#define DxFxxE4_xC0_StrapShortyTSxCount_OFFSET                  6
#define DxFxxE4_xC0_StrapShortyTSxCount_WIDTH                   2
#define DxFxxE4_xC0_StrapShortyTSxCount_MASK                    0xc0
#define DxFxxE4_xC0_StrapSkipInterval_OFFSET                    8
#define DxFxxE4_xC0_StrapSkipInterval_WIDTH                     3
#define DxFxxE4_xC0_StrapSkipInterval_MASK                      0x700
#define DxFxxE4_xC0_StrapBypassRcvrDet_OFFSET                   11
#define DxFxxE4_xC0_StrapBypassRcvrDet_WIDTH                    1
#define DxFxxE4_xC0_StrapBypassRcvrDet_MASK                     0x800
#define DxFxxE4_xC0_StrapComplianceDis_OFFSET                   12
#define DxFxxE4_xC0_StrapComplianceDis_WIDTH                    1
#define DxFxxE4_xC0_StrapComplianceDis_MASK                     0x1000
#define DxFxxE4_xC0_StrapForceCompliance_OFFSET                 13
#define DxFxxE4_xC0_StrapForceCompliance_WIDTH                  1
#define DxFxxE4_xC0_StrapForceCompliance_MASK                   0x2000
#define DxFxxE4_xC0_StrapReverseLcLanes_OFFSET                  14
#define DxFxxE4_xC0_StrapReverseLcLanes_WIDTH                   1
#define DxFxxE4_xC0_StrapReverseLcLanes_MASK                    0x4000
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_OFFSET       15
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_WIDTH        1
#define DxFxxE4_xC0_StrapAutoRcSpeedNegotiationDis_MASK         0x8000
#define DxFxxE4_xC0_StrapLaneNegotiation_OFFSET                 16
#define DxFxxE4_xC0_StrapLaneNegotiation_WIDTH                  3
#define DxFxxE4_xC0_StrapLaneNegotiation_MASK                   0x70000
#define DxFxxE4_xC0_Reserved_31_19_OFFSET                       19
#define DxFxxE4_xC0_Reserved_31_19_WIDTH                        13
#define DxFxxE4_xC0_Reserved_31_19_MASK                         0xfff80000

/// DxFxxE4_xC0
typedef union {
  struct {                                                              ///<
    UINT32                                        StrapFTSyTSxCount:2 ; ///<
    UINT32                                       StrapLongyTSxCount:2 ; ///<
    UINT32                                        StrapMedyTSxCount:2 ; ///<
    UINT32                                      StrapShortyTSxCount:2 ; ///<
    UINT32                                        StrapSkipInterval:3 ; ///<
    UINT32                                       StrapBypassRcvrDet:1 ; ///<
    UINT32                                       StrapComplianceDis:1 ; ///<
    UINT32                                     StrapForceCompliance:1 ; ///<
    UINT32                                      StrapReverseLcLanes:1 ; ///<
    UINT32                           StrapAutoRcSpeedNegotiationDis:1 ; ///<
    UINT32                                     StrapLaneNegotiation:3 ; ///<
    UINT32                                           Reserved_31_19:13; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} DxFxxE4_xC0_STRUCT;

 // **** D0F0xBC_xC0000000 Register Definition ****
// Address
#define D0F0xBC_xC0000000_ADDRESS                               0xc0000000

// Type
#define D0F0xBC_xC0000000_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0000000_GCK_hard_resetb_OFFSET                0
#define D0F0xBC_xC0000000_GCK_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_GCK_hard_resetb_MASK                  0x1
#define D0F0xBC_xC0000000_Reserved_OFFSET                       1
#define D0F0xBC_xC0000000_Reserved_WIDTH                        3
#define D0F0xBC_xC0000000_Reserved_MASK                         0xe
#define D0F0xBC_xC0000000_GIO_rst_early_resetb_OFFSET           4
#define D0F0xBC_xC0000000_GIO_rst_early_resetb_WIDTH            1
#define D0F0xBC_xC0000000_GIO_rst_early_resetb_MASK             0x10
#define D0F0xBC_xC0000000_TARG_early_resetb_OFFSET              5
#define D0F0xBC_xC0000000_TARG_early_resetb_WIDTH               1
#define D0F0xBC_xC0000000_TARG_early_resetb_MASK                0x20
#define D0F0xBC_xC0000000_Reserved6_7_OFFSET                    6
#define D0F0xBC_xC0000000_Reserved6_7_WIDTH                     2
#define D0F0xBC_xC0000000_Reserved6_7_MASK                      0xc0
#define D0F0xBC_xC0000000_GIO_rst_hard_resetb_OFFSET            8
#define D0F0xBC_xC0000000_GIO_rst_hard_resetb_WIDTH             1
#define D0F0xBC_xC0000000_GIO_rst_hard_resetb_MASK              0x100
#define D0F0xBC_xC0000000_Reserved9_9_OFFSET                    9
#define D0F0xBC_xC0000000_Reserved9_9_WIDTH                     1
#define D0F0xBC_xC0000000_Reserved9_9_MASK                      0x200
#define D0F0xBC_xC0000000_PCIE_powergood_OFFSET                 10
#define D0F0xBC_xC0000000_PCIE_powergood_WIDTH                  1
#define D0F0xBC_xC0000000_PCIE_powergood_MASK                   0x400
#define D0F0xBC_xC0000000_Reserved11_14_OFFSET                  11
#define D0F0xBC_xC0000000_Reserved11_14_WIDTH                   4
#define D0F0xBC_xC0000000_Reserved11_14_MASK                    0x7800
#define D0F0xBC_xC0000000_BIF_cec_hard_resetb_OFFSET            15
#define D0F0xBC_xC0000000_BIF_cec_hard_resetb_WIDTH             1
#define D0F0xBC_xC0000000_BIF_cec_hard_resetb_MASK              0x8000
#define D0F0xBC_xC0000000_VDDC_hard_resetb_OFFSET               16
#define D0F0xBC_xC0000000_VDDC_hard_resetb_WIDTH                1
#define D0F0xBC_xC0000000_VDDC_hard_resetb_MASK                 0x10000
#define D0F0xBC_xC0000000_DC_az_hard_resetb_OFFSET              17
#define D0F0xBC_xC0000000_DC_az_hard_resetb_WIDTH               1
#define D0F0xBC_xC0000000_DC_az_hard_resetb_MASK                0x20000
#define D0F0xBC_xC0000000_Reserved18_18_OFFSET                  18
#define D0F0xBC_xC0000000_Reserved18_18_WIDTH                   1
#define D0F0xBC_xC0000000_Reserved18_18_MASK                    0x40000
#define D0F0xBC_xC0000000_UVD_hard_resetb_OFFSET                19
#define D0F0xBC_xC0000000_UVD_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_UVD_hard_resetb_MASK                  0x80000
#define D0F0xBC_xC0000000_VCE_hard_resetb_OFFSET                20
#define D0F0xBC_xC0000000_VCE_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_VCE_hard_resetb_MASK                  0x100000
#define D0F0xBC_xC0000000_ACP_hard_resetb_OFFSET                21
#define D0F0xBC_xC0000000_ACP_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_ACP_hard_resetb_MASK                  0x200000
#define D0F0xBC_xC0000000_RB0_hard_resetb_OFFSET                22
#define D0F0xBC_xC0000000_RB0_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_RB0_hard_resetb_MASK                  0x400000
#define D0F0xBC_xC0000000_RB1_hard_resetb_OFFSET                23
#define D0F0xBC_xC0000000_RB1_hard_resetb_WIDTH                 1
#define D0F0xBC_xC0000000_RB1_hard_resetb_MASK                  0x800000
#define D0F0xBC_xC0000000_IOMMU_hard_resetb_OFFSET              24
#define D0F0xBC_xC0000000_IOMMU_hard_resetb_WIDTH               1
#define D0F0xBC_xC0000000_IOMMU_hard_resetb_MASK                0x1000000
#define D0F0xBC_xC0000000_Reserved25_31_OFFSET                  25
#define D0F0xBC_xC0000000_Reserved25_31_WIDTH                   7
#define D0F0xBC_xC0000000_Reserved25_31_MASK                    0xfe000000

/// D0F0xBC_xC0000000
typedef union {
  struct {                                                              ///<
    UINT32                                          GCK_hard_resetb:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                     GIO_rst_early_resetb:1 ; ///<
    UINT32                                        TARG_early_resetb:1 ; ///<
    UINT32                                              Reserved6_7:2 ; ///<
    UINT32                                      GIO_rst_hard_resetb:1 ; ///<
    UINT32                                              Reserved9_9:1 ; ///<
    UINT32                                           PCIE_powergood:1 ; ///<
    UINT32                                            Reserved11_14:4 ; ///<
    UINT32                                      BIF_cec_hard_resetb:1 ; ///<
    UINT32                                         VDDC_hard_resetb:1 ; ///<
    UINT32                                        DC_az_hard_resetb:1 ; ///<
    UINT32                                            Reserved18_18:1 ; ///<
    UINT32                                          UVD_hard_resetb:1 ; ///<
    UINT32                                          VCE_hard_resetb:1 ; ///<
    UINT32                                          ACP_hard_resetb:1 ; ///<
    UINT32                                          RB0_hard_resetb:1 ; ///<
    UINT32                                          RB1_hard_resetb:1 ; ///<
    UINT32                                        IOMMU_hard_resetb:1 ; ///<
    UINT32                                            Reserved25_31:7 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0000000_STRUCT;

// **** D0F0xBC_xC0500000 Register Definition ****
// Address
#define D0F0xBC_xC0500000_ADDRESS                               0xC0500000
// Type
#define D0F0xBC_xC0500000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0500000_GckFuseProg_OFFSET                    0
#define D0F0xBC_xC0500000_GckFuseProg_WIDTH                     1
#define D0F0xBC_xC0500000_GckFuseProg_MASK                      0x1
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_OFFSET         1
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_WIDTH          6
#define D0F0xBC_xC0500000_MainPllOpFreqIdStartup_MASK           0x7E
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_OFFSET             7
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_WIDTH              6
#define D0F0xBC_xC0500000_MainPllOpFreqIdMax_MASK               0x1F80
#define D0F0xBC_xC0500000_MainPllBGRefAdj_OFFSET                13
#define D0F0xBC_xC0500000_MainPllBGRefAdj_WIDTH                 5
#define D0F0xBC_xC0500000_MainPllBGRefAdj_MASK                  0x3E000
#define D0F0xBC_xC0500000_MainPllBGCal_OFFSET                   18
#define D0F0xBC_xC0500000_MainPllBGCal_WIDTH                    4
#define D0F0xBC_xC0500000_MainPllBGCal_MASK                     0x3C0000
#define D0F0xBC_xC0500000_MainPllDutyCycleAdj_OFFSET            22
#define D0F0xBC_xC0500000_MainPllDutyCycleAdj_WIDTH             4
#define D0F0xBC_xC0500000_MainPllDutyCycleAdj_MASK              0x3C00000
#define D0F0xBC_xC0500000_MainPllFracDiv_OFFSET                 26
#define D0F0xBC_xC0500000_MainPllFracDiv_WIDTH                  4
#define D0F0xBC_xC0500000_MainPllFracDiv_MASK                   0x3C000000
#define D0F0xBC_xC0500000_MainPllOTAHalfGain_OFFSET             30
#define D0F0xBC_xC0500000_MainPllOTAHalfGain_WIDTH              1
#define D0F0xBC_xC0500000_MainPllOTAHalfGain_MASK               0x40000000
#define D0F0xBC_xC0500000_Reserved_31_31_OFFSET                 31
#define D0F0xBC_xC0500000_Reserved_31_31_WIDTH                  1
#define D0F0xBC_xC0500000_Reserved_31_31_MASK                   0x80000000

/// D0F0xBC_xC0500000
typedef union {
  struct {                                                            ///<
    UINT32                                             GckFuseProg:1; ///<
    UINT32                                  MainPllOpFreqIdStartup:6; ///<
    UINT32                                      MainPllOpFreqIdMax:6; ///<
    UINT32                                         MainPllBGRefAdj:5; ///<
    UINT32                                            MainPllBGCal:4; ///<
    UINT32                                     MainPllDutyCycleAdj:4; ///<
    UINT32                                          MainPllFracDiv:4; ///<
    UINT32                                      MainPllOTAHalfGain:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0500000_STRUCT;

// **** GMMx190 Register Definition ****
// Address
#define GMMx190_ADDRESS                                         0x190

// Type
#define GMMx190_TYPE                                            TYPE_GMM
// Field Data
#define GMMx190_ADDR_OFFSET                                     0
#define GMMx190_ADDR_WIDTH                                      8
#define GMMx190_ADDR_MASK                                       0xff
#define GMMx190_DSM_SEL_OFFSET                                  8
#define GMMx190_DSM_SEL_WIDTH                                   1
#define GMMx190_DSM_SEL_MASK                                    0x100
#define GMMx190_DRB_SEL_OFFSET                                  9
#define GMMx190_DRB_SEL_WIDTH                                   1
#define GMMx190_DRB_SEL_MASK                                    0x200
#define GMMx190_SPARE_BITS31_6_OFFSET                           10
#define GMMx190_SPARE_BITS31_6_WIDTH                            22
#define GMMx190_SPARE_BITS31_6_MASK                             0xfffffc00

/// GMMx190
typedef union {
  struct {                                                              ///<
    UINT32                                                     ADDR:8 ; ///<
    UINT32                                                  DSM_SEL:1 ; ///<
    UINT32                                                  DRB_SEL:1 ; ///<
    UINT32                                           SPARE_BITS31_6:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx190_STRUCT;

// **** GMMx194 Register Definition ****
// Address
#define GMMx194_ADDRESS                                         0x194

// Type
#define GMMx194_TYPE                                            TYPE_GMM
// Field Data
#define GMMx194_DBG_EN_OFFSET                                   0
#define GMMx194_DBG_EN_WIDTH                                    1
#define GMMx194_DBG_EN_MASK                                     0x1
#define GMMx194_DSM_SPR_WREN_D4_OFFSET                          1
#define GMMx194_DSM_SPR_WREN_D4_WIDTH                           1
#define GMMx194_DSM_SPR_WREN_D4_MASK                            0x2
#define GMMx194_DSM_SPR_WREN_D5_OFFSET                          2
#define GMMx194_DSM_SPR_WREN_D5_WIDTH                           1
#define GMMx194_DSM_SPR_WREN_D5_MASK                            0x4
#define GMMx194_DSM_SPR_RDEN_D5_OFFSET                          3
#define GMMx194_DSM_SPR_RDEN_D5_WIDTH                           1
#define GMMx194_DSM_SPR_RDEN_D5_MASK                            0x8
#define GMMx194_DRB_SPR_WREN_OFFSET                             4
#define GMMx194_DRB_SPR_WREN_WIDTH                              1
#define GMMx194_DRB_SPR_WREN_MASK                               0x10
#define GMMx194_DRB_SPR_RDEN_OFFSET                             5
#define GMMx194_DRB_SPR_RDEN_WIDTH                              1
#define GMMx194_DRB_SPR_RDEN_MASK                               0x20
#define GMMx194_GTSC_REFCLK_SEL_OFFSET                          6
#define GMMx194_GTSC_REFCLK_SEL_WIDTH                           1
#define GMMx194_GTSC_REFCLK_SEL_MASK                            0x40
#define GMMx194_GTSC_SCLK_SEL_OFFSET                            7
#define GMMx194_GTSC_SCLK_SEL_WIDTH                             1
#define GMMx194_GTSC_SCLK_SEL_MASK                              0x80
#define GMMx194_GTSC_INIT_EN_OFFSET                             8
#define GMMx194_GTSC_INIT_EN_WIDTH                              1
#define GMMx194_GTSC_INIT_EN_MASK                               0x100
#define GMMx194_DCW_EN_OFFSET                                   9
#define GMMx194_DCW_EN_WIDTH                                    1
#define GMMx194_DCW_EN_MASK                                     0x200
#define GMMx194_DCW_PACK_RATIO_OFFSET                           10
#define GMMx194_DCW_PACK_RATIO_WIDTH                            2
#define GMMx194_DCW_PACK_RATIO_MASK                             0xc00
#define GMMx194_SPARE_BITS31_3_OFFSET                           12
#define GMMx194_SPARE_BITS31_3_WIDTH                            20
#define GMMx194_SPARE_BITS31_3_MASK                             0xfffff000

/// GMMx194
typedef union {
  struct {                                                              ///<
    UINT32                                                   DBG_EN:1 ; ///<
    UINT32                                          DSM_SPR_WREN_D4:1 ; ///<
    UINT32                                          DSM_SPR_WREN_D5:1 ; ///<
    UINT32                                          DSM_SPR_RDEN_D5:1 ; ///<
    UINT32                                             DRB_SPR_WREN:1 ; ///<
    UINT32                                             DRB_SPR_RDEN:1 ; ///<
    UINT32                                          GTSC_REFCLK_SEL:1 ; ///<
    UINT32                                            GTSC_SCLK_SEL:1 ; ///<
    UINT32                                             GTSC_INIT_EN:1 ; ///<
    UINT32                                                   DCW_EN:1 ; ///<
    UINT32                                           DCW_PACK_RATIO:2 ; ///<
    UINT32                                           SPARE_BITS31_3:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx194_STRUCT;

// **** GMMx198 Register Definition ****
// Address
#define GMMx198_ADDRESS                                         0x198

// Type
#define GMMx198_TYPE                                            TYPE_GMM
// Field Data
#define GMMx198_DSM_SPR_DATA0_OFFSET                            0
#define GMMx198_DSM_SPR_DATA0_WIDTH                             32
#define GMMx198_DSM_SPR_DATA0_MASK                              0xffffffff

// **** GMMxE40 Register Definition ****
// Address
#define GMMxE40_ADDRESS                                         0xE40
// Type
#define GMMxE40_TYPE                                            TYPE_GMM

// **** GMMxE60 Register Definition ****
// Address
#define GMMxE60_ADDRESS                                         0xE60
// Type
#define GMMxE60_TYPE                                            TYPE_GMM

// Field Data
#define GMMxE60_Reserved_0_0_OFFSET                             0
#define GMMxE60_Reserved_0_0_WIDTH                              1
#define GMMxE60_Reserved_0_0_MASK                               0x1
#define GMMxE60_SOFT_RESET_BIF_OFFSET                           1
#define GMMxE60_SOFT_RESET_BIF_WIDTH                            1
#define GMMxE60_SOFT_RESET_BIF_MASK                             0x2
#define GMMxE60_Reserved_3_2_OFFSET                             2
#define GMMxE60_Reserved_3_2_WIDTH                              2
#define GMMxE60_Reserved_3_2_MASK                               0xC
#define GMMxE60_SOFT_RESET_ROPLL_OFFSET                         4
#define GMMxE60_SOFT_RESET_ROPLL_WIDTH                          1
#define GMMxE60_SOFT_RESET_ROPLL_MASK                           0x10
#define GMMxE60_SOFT_RESET_DC_OFFSET                            5
#define GMMxE60_SOFT_RESET_DC_WIDTH                             1
#define GMMxE60_SOFT_RESET_DC_MASK                              0x20
#define GMMxE60_SOFT_RESET_SDMA1_OFFSET                         6
#define GMMxE60_SOFT_RESET_SDMA1_WIDTH                          1
#define GMMxE60_SOFT_RESET_SDMA1_MASK                           0x40
#define GMMxE60_SOFT_RESET_DRM_OFFSET                           7
#define GMMxE60_SOFT_RESET_DRM_WIDTH                            1
#define GMMxE60_SOFT_RESET_DRM_MASK                             0x80
#define GMMxE60_SOFT_RESET_GRBM_OFFSET                          8
#define GMMxE60_SOFT_RESET_GRBM_WIDTH                           1
#define GMMxE60_SOFT_RESET_GRBM_MASK                            0x100
#define GMMxE60_SOFT_RESET_HDP_OFFSET                           9
#define GMMxE60_SOFT_RESET_HDP_WIDTH                            1
#define GMMxE60_SOFT_RESET_HDP_MASK                             0x200
#define GMMxE60_SOFT_RESET_IH_OFFSET                            10
#define GMMxE60_SOFT_RESET_IH_WIDTH                             1
#define GMMxE60_SOFT_RESET_IH_MASK                              0x400
#define GMMxE60_SOFT_RESET_MC_OFFSET                            11
#define GMMxE60_SOFT_RESET_MC_WIDTH                             1
#define GMMxE60_SOFT_RESET_MC_MASK                              0x800
#define GMMxE60_SOFT_RESET_CHUB_OFFSET                          12
#define GMMxE60_SOFT_RESET_CHUB_WIDTH                           1
#define GMMxE60_SOFT_RESET_CHUB_MASK                            0x1000
#define GMMxE60_Reserved_13_13_OFFSET                           13
#define GMMxE60_Reserved_13_13_WIDTH                            1
#define GMMxE60_Reserved_13_13_MASK                             0x2000
#define GMMxE60_SOFT_RESET_ROM_OFFSET                           14
#define GMMxE60_SOFT_RESET_ROM_WIDTH                            1
#define GMMxE60_SOFT_RESET_ROM_MASK                             0x4000
#define GMMxE60_SOFT_RESET_SEM_OFFSET                           15
#define GMMxE60_SOFT_RESET_SEM_WIDTH                            1
#define GMMxE60_SOFT_RESET_SEM_MASK                             0x8000
#define GMMxE60_SOFT_RESET_SMU_OFFSET                           16
#define GMMxE60_SOFT_RESET_SMU_WIDTH                            1
#define GMMxE60_SOFT_RESET_SMU_MASK                             0x10000
#define GMMxE60_SOFT_RESET_VMC_OFFSET                           17
#define GMMxE60_SOFT_RESET_VMC_WIDTH                            1
#define GMMxE60_SOFT_RESET_VMC_MASK                             0x20000
#define GMMxE60_SOFT_RESET_UVD_OFFSET                           18
#define GMMxE60_SOFT_RESET_UVD_WIDTH                            1
#define GMMxE60_SOFT_RESET_UVD_MASK                             0x40000
#define GMMxE60_SOFT_RESET_XSP_OFFSET                           19
#define GMMxE60_SOFT_RESET_XSP_WIDTH                            1
#define GMMxE60_SOFT_RESET_XSP_MASK                             0x80000
#define GMMxE60_SOFT_RESET_SDMA_OFFSET                          20
#define GMMxE60_SOFT_RESET_SDMA_WIDTH                           1
#define GMMxE60_SOFT_RESET_SDMA_MASK                            0x100000
#define GMMxE60_SOFT_RESET_TST_OFFSET                           21
#define GMMxE60_SOFT_RESET_TST_WIDTH                            1
#define GMMxE60_SOFT_RESET_TST_MASK                             0x200000
#define GMMxE60_SOFT_RESET_REGBB_OFFSET                         22
#define GMMxE60_SOFT_RESET_REGBB_WIDTH                          1
#define GMMxE60_SOFT_RESET_REGBB_MASK                           0x400000
#define GMMxE60_SOFT_RESET_ORB_OFFSET                           23
#define GMMxE60_SOFT_RESET_ORB_WIDTH                            1
#define GMMxE60_SOFT_RESET_ORB_MASK                             0x800000
#define GMMxE60_SOFT_RESET_VCE_OFFSET                           24
#define GMMxE60_SOFT_RESET_VCE_WIDTH                            1
#define GMMxE60_SOFT_RESET_VCE_MASK                             0x1000000
#define GMMxE60_SOFT_RESET_XDMA_OFFSET                          25
#define GMMxE60_SOFT_RESET_XDMA_WIDTH                           1
#define GMMxE60_SOFT_RESET_XDMA_MASK                            0x2000000
#define GMMxE60_SOFT_RESET_ACP_OFFSET                           26
#define GMMxE60_SOFT_RESET_ACP_WIDTH                            1
#define GMMxE60_SOFT_RESET_ACP_MASK                             0x4000000
#define GMMxE60_SOFT_RESET_SAM_OFFSET                           27
#define GMMxE60_SOFT_RESET_SAM_WIDTH                            1
#define GMMxE60_SOFT_RESET_SAM_MASK                             0x8000000
#define GMMxE60_Reserved_31_28_OFFSET                           28
#define GMMxE60_Reserved_31_28_WIDTH                            4
#define GMMxE60_Reserved_31_28_MASK                             0xF0000000

/// GMMxE60
typedef union {
  struct {                                                            ///<
    UINT32                                            Reserved_0_0:1; ///<
    UINT32                                          SOFT_RESET_BIF:1; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                                        SOFT_RESET_ROPLL:1; ///<
    UINT32                                           SOFT_RESET_DC:1; ///<
    UINT32                                        SOFT_RESET_SDMA1:1; ///<
    UINT32                                          SOFT_RESET_DRM:1; ///<
    UINT32                                         SOFT_RESET_GRBM:1; ///<
    UINT32                                          SOFT_RESET_HDP:1; ///<
    UINT32                                           SOFT_RESET_IH:1; ///<
    UINT32                                           SOFT_RESET_MC:1; ///<
    UINT32                                         SOFT_RESET_CHUB:1; ///<
    UINT32                                          Reserved_13_13:1; ///<
    UINT32                                          SOFT_RESET_ROM:1; ///<
    UINT32                                          SOFT_RESET_SEM:1; ///<
    UINT32                                          SOFT_RESET_SMU:1; ///<
    UINT32                                          SOFT_RESET_VMC:1; ///<
    UINT32                                          SOFT_RESET_UVD:1; ///<
    UINT32                                          SOFT_RESET_XSP:1; ///<
    UINT32                                         SOFT_RESET_SDMA:1; ///<
    UINT32                                          SOFT_RESET_TST:1; ///<
    UINT32                                        SOFT_RESET_REGBB:1; ///<
    UINT32                                          SOFT_RESET_ORB:1; ///<
    UINT32                                          SOFT_RESET_VCE:1; ///<
    UINT32                                         SOFT_RESET_XDMA:1; ///<
    UINT32                                          SOFT_RESET_ACP:1; ///<
    UINT32                                          SOFT_RESET_SAM:1; ///<
    UINT32                                          Reserved_31_28:4; ///<

  } Field;

  UINT32 Value;
} GMMxE60_STRUCT;

// **** GMMx1740 Register Definition ****
// Address
#define GMMx1740_ADDRESS                                        0x1740

// Type
#define GMMx1740_TYPE                                           TYPE_GMM
// Field Data
#define GMMx1740_BIOS_SCRATCH_7_OFFSET                          0
#define GMMx1740_BIOS_SCRATCH_7_WIDTH                           32
#define GMMx1740_BIOS_SCRATCH_7_MASK                            0xffffffff
// VBIOS post check bit
#define ATOM_S7_ASIC_INIT_COMPLETEb1                            0x200

/// GMMx1740
typedef union {
  struct {                                                              ///<
    UINT32                                           BIOS_SCRATCH_7:32; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx1740_STRUCT;

// **** GMMx1854 Register Definition ****
// Address
#define GMMx1854_ADDRESS                                        0x1854

// Type
#define GMMx1854_TYPE                                           TYPE_GMM
// Field Data
#define GMMx1854_LS_EN_OFFSET                                   0
#define GMMx1854_LS_EN_WIDTH                                    1
#define GMMx1854_LS_EN_MASK                                     0x1
#define GMMx1854_LS_HOLD_OFFSET                                 1
#define GMMx1854_LS_HOLD_WIDTH                                  6
#define GMMx1854_LS_HOLD_MASK                                   0x7e
#define GMMx1854_LS_SETUP_OFFSET                                7
#define GMMx1854_LS_SETUP_WIDTH                                 6
#define GMMx1854_LS_SETUP_MASK                                  0x1f80
#define GMMx1854_CG_ENABLE_OFFSET                               13
#define GMMx1854_CG_ENABLE_WIDTH                                1
#define GMMx1854_CG_ENABLE_MASK                                 0x2000
#define GMMx1854_CG_RDY_DELAY_OFFSET                            14
#define GMMx1854_CG_RDY_DELAY_WIDTH                             6
#define GMMx1854_CG_RDY_DELAY_MASK                              0xfc000
#define GMMx1854_CG_OFF_DELAY_OFFSET                            20
#define GMMx1854_CG_OFF_DELAY_WIDTH                             6
#define GMMx1854_CG_OFF_DELAY_MASK                              0x3f00000
#define GMMx1854_CG_ON_DELAY_OFFSET                             26
#define GMMx1854_CG_ON_DELAY_WIDTH                              6
#define GMMx1854_CG_ON_DELAY_MASK                               0xfc000000

/// GMMx1854
typedef union {
  struct {                                                              ///<
    UINT32                                                    LS_EN:1 ; ///<
    UINT32                                                  LS_HOLD:6 ; ///<
    UINT32                                                 LS_SETUP:6 ; ///<
    UINT32                                                CG_ENABLE:1 ; ///<
    UINT32                                             CG_RDY_DELAY:6 ; ///<
    UINT32                                             CG_OFF_DELAY:6 ; ///<
    UINT32                                              CG_ON_DELAY:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx1854_STRUCT;

// **** GMMx1860 Register Definition ****
// Address
#define GMMx1860_ADDRESS                                        0x1860

// Type
#define GMMx1860_TYPE                                           TYPE_GMM
// Field Data
#define GMMx1860_TCS_DISABLE_OFFSET                             0
#define GMMx1860_TCS_DISABLE_WIDTH                              1
#define GMMx1860_TCS_DISABLE_MASK                               0x1
#define GMMx1860_TX_ON_EMPTY_WAIT_DISABLE_OFFSET                1
#define GMMx1860_TX_ON_EMPTY_WAIT_DISABLE_WIDTH                 1
#define GMMx1860_TX_ON_EMPTY_WAIT_DISABLE_MASK                  0x2
#define GMMx1860_ATOM_GRP1_DISABLE_OFFSET                       2
#define GMMx1860_ATOM_GRP1_DISABLE_WIDTH                        1
#define GMMx1860_ATOM_GRP1_DISABLE_MASK                         0x4
#define GMMx1860_LOOPMASK_SKIP_DISABLE_OFFSET                   3
#define GMMx1860_LOOPMASK_SKIP_DISABLE_WIDTH                    1
#define GMMx1860_LOOPMASK_SKIP_DISABLE_MASK                     0x8
#define GMMx1860_ONION_OFF_AFTER_IDLE_OFFSET                    4
#define GMMx1860_ONION_OFF_AFTER_IDLE_WIDTH                     1
#define GMMx1860_ONION_OFF_AFTER_IDLE_MASK                      0x10
#define GMMx1860_ONION_SLEEP_DISABLE_OFFSET                     5
#define GMMx1860_ONION_SLEEP_DISABLE_WIDTH                      1
#define GMMx1860_ONION_SLEEP_DISABLE_MASK                       0x20
#define GMMx1860_Reserved_OFFSET                                6
#define GMMx1860_Reserved_WIDTH                                 6
#define GMMx1860_Reserved_MASK                                  0xfc0
#define GMMx1860_WRITE_NACK_DISABLE_OFFSET                      12
#define GMMx1860_WRITE_NACK_DISABLE_WIDTH                       1
#define GMMx1860_WRITE_NACK_DISABLE_MASK                        0x1000
#define GMMx1860_READ_NACK_DISABLE_OFFSET                       13
#define GMMx1860_READ_NACK_DISABLE_WIDTH                        1
#define GMMx1860_READ_NACK_DISABLE_MASK                         0x2000
#define GMMx1860_ATOMIC_NACK_DISABLE_OFFSET                     14
#define GMMx1860_ATOMIC_NACK_DISABLE_WIDTH                      1
#define GMMx1860_ATOMIC_NACK_DISABLE_MASK                       0x4000
#define GMMx1860_VM_NACK_DISABLE_OFFSET                         15
#define GMMx1860_VM_NACK_DISABLE_WIDTH                          1
#define GMMx1860_VM_NACK_DISABLE_MASK                           0x8000
#define GMMx1860_ADD_DISABLE_OFFSET                             16
#define GMMx1860_ADD_DISABLE_WIDTH                              1
#define GMMx1860_ADD_DISABLE_MASK                               0x10000
#define GMMx1860_SUB_DISABLE_OFFSET                             17
#define GMMx1860_SUB_DISABLE_WIDTH                              1
#define GMMx1860_SUB_DISABLE_MASK                               0x20000
#define GMMx1860_AND_DISABLE_OFFSET                             18
#define GMMx1860_AND_DISABLE_WIDTH                              1
#define GMMx1860_AND_DISABLE_MASK                               0x40000
#define GMMx1860_OR_DISABLE_OFFSET                              19
#define GMMx1860_OR_DISABLE_WIDTH                               1
#define GMMx1860_OR_DISABLE_MASK                                0x80000
#define GMMx1860_XOR_DISABLE_OFFSET                             20
#define GMMx1860_XOR_DISABLE_WIDTH                              1
#define GMMx1860_XOR_DISABLE_MASK                               0x100000
#define GMMx1860_CMPSWAP_DISABLE_OFFSET                         21
#define GMMx1860_CMPSWAP_DISABLE_WIDTH                          1
#define GMMx1860_CMPSWAP_DISABLE_MASK                           0x200000
#define GMMx1860_SWAP_DISABLE_OFFSET                            22
#define GMMx1860_SWAP_DISABLE_WIDTH                             1
#define GMMx1860_SWAP_DISABLE_MASK                              0x400000
#define GMMx1860_SMAX_DISABLE_OFFSET                            23
#define GMMx1860_SMAX_DISABLE_WIDTH                             1
#define GMMx1860_SMAX_DISABLE_MASK                              0x800000
#define GMMx1860_SMIN_DISABLE_OFFSET                            24
#define GMMx1860_SMIN_DISABLE_WIDTH                             1
#define GMMx1860_SMIN_DISABLE_MASK                              0x1000000
#define GMMx1860_UMAX_DISABLE_OFFSET                            25
#define GMMx1860_UMAX_DISABLE_WIDTH                             1
#define GMMx1860_UMAX_DISABLE_MASK                              0x2000000
#define GMMx1860_UMIN_DISABLE_OFFSET                            26
#define GMMx1860_UMIN_DISABLE_WIDTH                             1
#define GMMx1860_UMIN_DISABLE_MASK                              0x4000000
#define GMMx1860_Reserved27_27_OFFSET                           27
#define GMMx1860_Reserved27_27_WIDTH                            1
#define GMMx1860_Reserved27_27_MASK                             0x8000000
#define GMMx1860_IGNORE_TX_ONION_IN_ALL_IDLE_OFFSET             28
#define GMMx1860_IGNORE_TX_ONION_IN_ALL_IDLE_WIDTH              1
#define GMMx1860_IGNORE_TX_ONION_IN_ALL_IDLE_MASK               0x10000000
#define GMMx1860_IGNORE_RX_ONION_IN_ALL_IDLE_OFFSET             29
#define GMMx1860_IGNORE_RX_ONION_IN_ALL_IDLE_WIDTH              1
#define GMMx1860_IGNORE_RX_ONION_IN_ALL_IDLE_MASK               0x20000000
#define GMMx1860_PARTIAL_WRITE_OPT_DISABLE_OFFSET               30
#define GMMx1860_PARTIAL_WRITE_OPT_DISABLE_WIDTH                1
#define GMMx1860_PARTIAL_WRITE_OPT_DISABLE_MASK                 0x40000000
#define GMMx1860_Reserved31_31_OFFSET                           31
#define GMMx1860_Reserved31_31_WIDTH                            1
#define GMMx1860_Reserved31_31_MASK                             0x80000000

/// GMMx1860
typedef union {
  struct {                                                              ///<
    UINT32                                              TCS_DISABLE:1 ; ///<
    UINT32                                 TX_ON_EMPTY_WAIT_DISABLE:1 ; ///<
    UINT32                                        ATOM_GRP1_DISABLE:1 ; ///<
    UINT32                                    LOOPMASK_SKIP_DISABLE:1 ; ///<
    UINT32                                     ONION_OFF_AFTER_IDLE:1 ; ///<
    UINT32                                      ONION_SLEEP_DISABLE:1 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                       WRITE_NACK_DISABLE:1 ; ///<
    UINT32                                        READ_NACK_DISABLE:1 ; ///<
    UINT32                                      ATOMIC_NACK_DISABLE:1 ; ///<
    UINT32                                          VM_NACK_DISABLE:1 ; ///<
    UINT32                                              ADD_DISABLE:1 ; ///<
    UINT32                                              SUB_DISABLE:1 ; ///<
    UINT32                                              AND_DISABLE:1 ; ///<
    UINT32                                               OR_DISABLE:1 ; ///<
    UINT32                                              XOR_DISABLE:1 ; ///<
    UINT32                                          CMPSWAP_DISABLE:1 ; ///<
    UINT32                                             SWAP_DISABLE:1 ; ///<
    UINT32                                             SMAX_DISABLE:1 ; ///<
    UINT32                                             SMIN_DISABLE:1 ; ///<
    UINT32                                             UMAX_DISABLE:1 ; ///<
    UINT32                                             UMIN_DISABLE:1 ; ///<
    UINT32                                            Reserved27_27:1 ; ///<
    UINT32                              IGNORE_TX_ONION_IN_ALL_IDLE:1 ; ///<
    UINT32                              IGNORE_RX_ONION_IN_ALL_IDLE:1 ; ///<
    UINT32                                PARTIAL_WRITE_OPT_DISABLE:1 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx1860_STRUCT;

// **** GMMx1864 Register Definition ****
// Address
#define GMMx1864_ADDRESS                                        0x1864

// Type
#define GMMx1864_TYPE                                           TYPE_GMM
// Field Data
#define GMMx1864_BYPASS_VM_OFFSET                               0
#define GMMx1864_BYPASS_VM_WIDTH                                1
#define GMMx1864_BYPASS_VM_MASK                                 0x1
#define GMMx1864_BYPASS_VM_STATUS_OFFSET                        1
#define GMMx1864_BYPASS_VM_STATUS_WIDTH                         3
#define GMMx1864_BYPASS_VM_STATUS_MASK                          0xe
#define GMMx1864_RQ_RSP_ORDER_MODE_OFFSET                       4
#define GMMx1864_RQ_RSP_ORDER_MODE_WIDTH                        2
#define GMMx1864_RQ_RSP_ORDER_MODE_MASK                         0x30
#define GMMx1864_NB_CACHEABLE_OFFSET                            6
#define GMMx1864_NB_CACHEABLE_WIDTH                             1
#define GMMx1864_NB_CACHEABLE_MASK                              0x40
#define GMMx1864_UNUSED1_OFFSET                                 7
#define GMMx1864_UNUSED1_WIDTH                                  2
#define GMMx1864_UNUSED1_MASK                                   0x180
#define GMMx1864_PERF_CNT_DISABLE_OFFSET                        9
#define GMMx1864_PERF_CNT_DISABLE_WIDTH                         1
#define GMMx1864_PERF_CNT_DISABLE_MASK                          0x200
#define GMMx1864_USE_CLKEN_FOR_IDLE_CNT_OFFSET                  10
#define GMMx1864_USE_CLKEN_FOR_IDLE_CNT_WIDTH                   1
#define GMMx1864_USE_CLKEN_FOR_IDLE_CNT_MASK                    0x400
#define GMMx1864_UNUSED2_OFFSET                                 11
#define GMMx1864_UNUSED2_WIDTH                                  13
#define GMMx1864_UNUSED2_MASK                                   0xfff800
#define GMMx1864_IDLE_LAGCNT_BIT_SEL_OFFSET                     24
#define GMMx1864_IDLE_LAGCNT_BIT_SEL_WIDTH                      5
#define GMMx1864_IDLE_LAGCNT_BIT_SEL_MASK                       0x1f000000
#define GMMx1864_WAKE_ME_UP_OFFSET                              29
#define GMMx1864_WAKE_ME_UP_WIDTH                               1
#define GMMx1864_WAKE_ME_UP_MASK                                0x20000000
#define GMMx1864_Reserved_OFFSET                                30
#define GMMx1864_Reserved_WIDTH                                 2
#define GMMx1864_Reserved_MASK                                  0xc0000000

/// GMMx1864
typedef union {
  struct {                                                              ///<
    UINT32                                                BYPASS_VM:1 ; ///<
    UINT32                                         BYPASS_VM_STATUS:3 ; ///<
    UINT32                                        RQ_RSP_ORDER_MODE:2 ; ///<
    UINT32                                             NB_CACHEABLE:1 ; ///<
    UINT32                                                  UNUSED1:2 ; ///<
    UINT32                                         PERF_CNT_DISABLE:1 ; ///<
    UINT32                                   USE_CLKEN_FOR_IDLE_CNT:1 ; ///<
    UINT32                                                  UNUSED2:13; ///<
    UINT32                                      IDLE_LAGCNT_BIT_SEL:5 ; ///<
    UINT32                                               WAKE_ME_UP:1 ; ///<
    UINT32                                                 Reserved:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx1864_STRUCT;

// **** GMMx186C Register Definition ****
// Address
#define GMMx186C_ADDRESS                                        0x186c

// Type
#define GMMx186C_TYPE                                           TYPE_GMM
// Field Data
#define GMMx186C_MAX_RC_CREDITS_OFFSET                          0
#define GMMx186C_MAX_RC_CREDITS_WIDTH                           8
#define GMMx186C_MAX_RC_CREDITS_MASK                            0xff
#define GMMx186C_MAX_RD_CREDITS_OFFSET                          8
#define GMMx186C_MAX_RD_CREDITS_WIDTH                           8
#define GMMx186C_MAX_RD_CREDITS_MASK                            0xff00
#define GMMx186C_MAX_NC_CREDITS_OFFSET                          16
#define GMMx186C_MAX_NC_CREDITS_WIDTH                           8
#define GMMx186C_MAX_NC_CREDITS_MASK                            0xff0000
#define GMMx186C_MAX_ND_CREDITS_OFFSET                          24
#define GMMx186C_MAX_ND_CREDITS_WIDTH                           8
#define GMMx186C_MAX_ND_CREDITS_MASK                            0xff000000

/// GMMx186C
typedef union {
  struct {                                                              ///<
    UINT32                                           MAX_RC_CREDITS:8 ; ///<
    UINT32                                           MAX_RD_CREDITS:8 ; ///<
    UINT32                                           MAX_NC_CREDITS:8 ; ///<
    UINT32                                           MAX_ND_CREDITS:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx186C_STRUCT;

// **** GMMx18A0 Register Definition ****
// Address
#define GMMx18A0_ADDRESS                                        0x18A0
// Type
#define GMMx18A0_TYPE                                           TYPE_GMM

// **** GMMx18A4 Register Definition ****
// Address
#define GMMx18A4_ADDRESS                                        0x18A4
// Type
#define GMMx18A4_TYPE                                           TYPE_GMM

// **** GMMx18A8 Register Definition ****
// Address
#define GMMx18A8_ADDRESS                                        0x18A8
// Type
#define GMMx18A8_TYPE                                           TYPE_GMM

// **** GMMx2000 Register Definition ****
// Address
#define GMMx2000_ADDRESS                                        0x2000

// Type
#define GMMx2000_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2000_MCDW_WR_ENABLE_OFFSET                          0
#define GMMx2000_MCDW_WR_ENABLE_WIDTH                           1
#define GMMx2000_MCDW_WR_ENABLE_MASK                            0x1
#define GMMx2000_MCDX_WR_ENABLE_OFFSET                          1
#define GMMx2000_MCDX_WR_ENABLE_WIDTH                           1
#define GMMx2000_MCDX_WR_ENABLE_MASK                            0x2
#define GMMx2000_MCDY_WR_ENABLE_OFFSET                          2
#define GMMx2000_MCDY_WR_ENABLE_WIDTH                           1
#define GMMx2000_MCDY_WR_ENABLE_MASK                            0x4
#define GMMx2000_MCDZ_WR_ENABLE_OFFSET                          3
#define GMMx2000_MCDZ_WR_ENABLE_WIDTH                           1
#define GMMx2000_MCDZ_WR_ENABLE_MASK                            0x8
#define GMMx2000_MC_RD_ENABLE_OFFSET                            4
#define GMMx2000_MC_RD_ENABLE_WIDTH                             2
#define GMMx2000_MC_RD_ENABLE_MASK                              0x30
#define GMMx2000_Reserved_OFFSET                                6
#define GMMx2000_Reserved_WIDTH                                 25
#define GMMx2000_Reserved_MASK                                  0x7fffffc0
#define GMMx2000_MCC_INDEX_MODE_ENABLE_OFFSET                   31
#define GMMx2000_MCC_INDEX_MODE_ENABLE_WIDTH                    1
#define GMMx2000_MCC_INDEX_MODE_ENABLE_MASK                     0x80000000

/// GMMx2000
typedef union {
  struct {                                                              ///<
    UINT32                                           MCDW_WR_ENABLE:1 ; ///<
    UINT32                                           MCDX_WR_ENABLE:1 ; ///<
    UINT32                                           MCDY_WR_ENABLE:1 ; ///<
    UINT32                                           MCDZ_WR_ENABLE:1 ; ///<
    UINT32                                             MC_RD_ENABLE:2 ; ///<
    UINT32                                                 Reserved:25; ///<
    UINT32                                    MCC_INDEX_MODE_ENABLE:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2000_STRUCT;

// **** GMMx2004 Register Definition ****
// Address
#define GMMx2004_ADDRESS                                        0x2004
// Type
#define GMMx2004_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2004_Reserved_11_0_OFFSET                           0
#define GMMx2004_Reserved_11_0_WIDTH                            12
#define GMMx2004_Reserved_11_0_MASK                             0xFFF
#define GMMx2004_NOOFCHAN_OFFSET                                12
#define GMMx2004_NOOFCHAN_WIDTH                                 4
#define GMMx2004_NOOFCHAN_MASK                                  0xF000
#define GMMx2004_Reserved_31_16_OFFSET                          16
#define GMMx2004_Reserved_31_16_WIDTH                           16
#define GMMx2004_Reserved_31_16_MASK                            0xFFFF0000

/// GMMx2004
typedef union {
  struct {                                                             ///<
    UINT32                                           Reserved_11_0:12; ///<
    UINT32                                                NOOFCHAN:4 ; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} GMMx2004_STRUCT;

// **** GMMx2008 Register Definition ****
// Address
#define GMMx2008_ADDRESS                                        0x2008
// Type
#define GMMx2008_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2008_CHAN0_OFFSET                                   0
#define GMMx2008_CHAN0_WIDTH                                    3
#define GMMx2008_CHAN0_MASK                                     0x7
#define GMMx2008_CHAN1_OFFSET                                   3
#define GMMx2008_CHAN1_WIDTH                                    3
#define GMMx2008_CHAN1_MASK                                     0x38
#define GMMx2008_CHAN2_OFFSET                                   6
#define GMMx2008_CHAN2_WIDTH                                    3
#define GMMx2008_CHAN2_MASK                                     0x1C0
#define GMMx2008_CHAN3_OFFSET                                   9
#define GMMx2008_CHAN3_WIDTH                                    3
#define GMMx2008_CHAN3_MASK                                     0xE00
#define GMMx2008_Reserved_31_12_OFFSET                          24
#define GMMx2008_Reserved_31_12_WIDTH                           8
#define GMMx2008_Reserved_31_12_MASK                            0xFFFFF000

/// GMMx2008
typedef union {
  struct {                                                             ///<
    UINT32                                                   CHAN0:3 ; ///<
    UINT32                                                   CHAN1:3 ; ///<
    UINT32                                                   CHAN2:3 ; ///<
    UINT32                                                   CHAN3:3 ; ///<
    UINT32                                          Reserved_31_12:20; ///<

  } Field;

  UINT32 Value;
} GMMx2008_STRUCT;

// **** GMMx2024 Register Definition ****
// Address
#define GMMx2024_ADDRESS                                        0x2024
// Type
#define GMMx2024_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2024_FB_BASE_OFFSET                                 0
#define GMMx2024_FB_BASE_WIDTH                                  16
#define GMMx2024_FB_BASE_MASK                                   0xFFFF
#define GMMx2024_FB_TOP_OFFSET                                  16
#define GMMx2024_FB_TOP_WIDTH                                   16
#define GMMx2024_FB_TOP_MASK                                    0xFFFF0000

/// GMMx2024
typedef union {
  struct {                                                            ///<
    UINT32                                                 FB_BASE:16; ///<
    UINT32                                                  FB_TOP:16; ///<

  } Field;

  UINT32 Value;
} GMMx2024_STRUCT;

// **** GMMx2068 Register Definition ****
// Address
#define GMMx2068_ADDRESS                                        0x2068
// Type
#define GMMx2068_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2068_FB_OFFSET_OFFSET                               0
#define GMMx2068_FB_OFFSET_WIDTH                                18
#define GMMx2068_FB_OFFSET_MASK                                 0x3FFFF
#define GMMx2068_Reserved_31_18_OFFSET                          18
#define GMMx2068_Reserved_31_18_WIDTH                           14
#define GMMx2068_Reserved_31_18_MASK                            0xFFFC0000

/// GMMx2068
typedef union {
  struct {                                                            ///<
    UINT32                                               FB_OFFSET:18; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} GMMx2068_STRUCT;

// **** GMMx20A0 Register Definition ****
// Address
#define GMMx20A0_ADDRESS                                        0x20a0

// Type
#define GMMx20A0_TYPE                                           TYPE_GMM
// Field Data
#define GMMx20A0_MCD0_WR_ENABLE_OFFSET                          0
#define GMMx20A0_MCD0_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD0_WR_ENABLE_MASK                            0x1
#define GMMx20A0_MCD1_WR_ENABLE_OFFSET                          1
#define GMMx20A0_MCD1_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD1_WR_ENABLE_MASK                            0x2
#define GMMx20A0_MCD2_WR_ENABLE_OFFSET                          2
#define GMMx20A0_MCD2_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD2_WR_ENABLE_MASK                            0x4
#define GMMx20A0_MCD3_WR_ENABLE_OFFSET                          3
#define GMMx20A0_MCD3_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD3_WR_ENABLE_MASK                            0x8
#define GMMx20A0_MCD4_WR_ENABLE_OFFSET                          4
#define GMMx20A0_MCD4_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD4_WR_ENABLE_MASK                            0x10
#define GMMx20A0_MCD5_WR_ENABLE_OFFSET                          5
#define GMMx20A0_MCD5_WR_ENABLE_WIDTH                           1
#define GMMx20A0_MCD5_WR_ENABLE_MASK                            0x20
#define GMMx20A0_Reserved_OFFSET                                6
#define GMMx20A0_Reserved_WIDTH                                 2
#define GMMx20A0_Reserved_MASK                                  0xc0
#define GMMx20A0_MC_RD_ENABLE_OFFSET                            8
#define GMMx20A0_MC_RD_ENABLE_WIDTH                             3
#define GMMx20A0_MC_RD_ENABLE_MASK                              0x700
#define GMMx20A0_Reserved11_30_OFFSET                           11
#define GMMx20A0_Reserved11_30_WIDTH                            20
#define GMMx20A0_Reserved11_30_MASK                             0x7ffff800
#define GMMx20A0_MCD_INDEX_MODE_ENABLE_OFFSET                   31
#define GMMx20A0_MCD_INDEX_MODE_ENABLE_WIDTH                    1
#define GMMx20A0_MCD_INDEX_MODE_ENABLE_MASK                     0x80000000

/// GMMx20A0
typedef union {
  struct {                                                              ///<
    UINT32                                           MCD0_WR_ENABLE:1 ; ///<
    UINT32                                           MCD1_WR_ENABLE:1 ; ///<
    UINT32                                           MCD2_WR_ENABLE:1 ; ///<
    UINT32                                           MCD3_WR_ENABLE:1 ; ///<
    UINT32                                           MCD4_WR_ENABLE:1 ; ///<
    UINT32                                           MCD5_WR_ENABLE:1 ; ///<
    UINT32                                                 Reserved:2 ; ///<
    UINT32                                             MC_RD_ENABLE:3 ; ///<
    UINT32                                            Reserved11_30:20; ///<
    UINT32                                    MCD_INDEX_MODE_ENABLE:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx20A0_STRUCT;

// **** GMMx2108 Register Definition ****
// Address
#define GMMx2108_ADDRESS                                        0x2108

// Type
#define GMMx2108_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2108_MAXBURST_OFFSET                                0
#define GMMx2108_MAXBURST_WIDTH                                 4
#define GMMx2108_MAXBURST_MASK                                  0xf
#define GMMx2108_LAZY_TIMER_OFFSET                              4
#define GMMx2108_LAZY_TIMER_WIDTH                               4
#define GMMx2108_LAZY_TIMER_MASK                                0xf0
#define GMMx2108_STALL_THRESHOLD_OFFSET                         8
#define GMMx2108_STALL_THRESHOLD_WIDTH                          8
#define GMMx2108_STALL_THRESHOLD_MASK                           0xff00
#define GMMx2108_STALL_MODE_OFFSET                              16
#define GMMx2108_STALL_MODE_WIDTH                               1
#define GMMx2108_STALL_MODE_MASK                                0x10000
#define GMMx2108_Reserved_OFFSET                                17
#define GMMx2108_Reserved_WIDTH                                 15
#define GMMx2108_Reserved_MASK                                  0xfffe0000

/// GMMx2108
typedef union {
  struct {                                                              ///<
    UINT32                                                 MAXBURST:4 ; ///<
    UINT32                                               LAZY_TIMER:4 ; ///<
    UINT32                                          STALL_THRESHOLD:8 ; ///<
    UINT32                                               STALL_MODE:1 ; ///<
    UINT32                                                 Reserved:15; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2108_STRUCT;

// **** GMMx2544 Register Definition ****
// Address
#define GMMx2544_ADDRESS                                        0x2544

// Type
#define GMMx2544_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2544_WR_SWITCH_NUM_OFFSET                           0
#define GMMx2544_WR_SWITCH_NUM_WIDTH                            8
#define GMMx2544_WR_SWITCH_NUM_MASK                             0xff
#define GMMx2544_RD_SWITCH_NUM_OFFSET                           8
#define GMMx2544_RD_SWITCH_NUM_WIDTH                            8
#define GMMx2544_RD_SWITCH_NUM_MASK                             0xff00
#define GMMx2544_ATC_SWITCH_NUM_OFFSET                          16
#define GMMx2544_ATC_SWITCH_NUM_WIDTH                           8
#define GMMx2544_ATC_SWITCH_NUM_MASK                            0xff0000
#define GMMx2544_Reserved_OFFSET                                24
#define GMMx2544_Reserved_WIDTH                                 8
#define GMMx2544_Reserved_MASK                                  0xff000000

/// GMMx2544
typedef union {
  struct {                                                              ///<
    UINT32                                            WR_SWITCH_NUM:8 ; ///<
    UINT32                                            RD_SWITCH_NUM:8 ; ///<
    UINT32                                           ATC_SWITCH_NUM:8 ; ///<
    UINT32                                                 Reserved:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2544_STRUCT;

// **** GMMx25B4 Register Definition ****
// Address
#define GMMx25B4_ADDRESS                                        0x25b4

// Type
#define GMMx25B4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx25B4_CB1_RD_OFFSET                                  0
#define GMMx25B4_CB1_RD_WIDTH                                   1
#define GMMx25B4_CB1_RD_MASK                                    0x1
#define GMMx25B4_CB1_WR_OFFSET                                  1
#define GMMx25B4_CB1_WR_WIDTH                                   1
#define GMMx25B4_CB1_WR_MASK                                    0x2
#define GMMx25B4_DB1_RD_OFFSET                                  2
#define GMMx25B4_DB1_RD_WIDTH                                   1
#define GMMx25B4_DB1_RD_MASK                                    0x4
#define GMMx25B4_DB1_WR_OFFSET                                  3
#define GMMx25B4_DB1_WR_WIDTH                                   1
#define GMMx25B4_DB1_WR_MASK                                    0x8
#define GMMx25B4_TC2_RD_OFFSET                                  4
#define GMMx25B4_TC2_RD_WIDTH                                   1
#define GMMx25B4_TC2_RD_MASK                                    0x10
#define GMMx25B4_Reserved_OFFSET                                5
#define GMMx25B4_Reserved_WIDTH                                 3
#define GMMx25B4_Reserved_MASK                                  0xe0
#define GMMx25B4_ARB_DBG_OFFSET                                 8
#define GMMx25B4_ARB_DBG_WIDTH                                  4
#define GMMx25B4_ARB_DBG_MASK                                   0xf00
#define GMMx25B4_TC2_WR_OFFSET                                  12
#define GMMx25B4_TC2_WR_WIDTH                                   1
#define GMMx25B4_TC2_WR_MASK                                    0x1000
#define GMMx25B4_CB0_CONNECT_CNTL_OFFSET                        13
#define GMMx25B4_CB0_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_CB0_CONNECT_CNTL_MASK                          0x6000
#define GMMx25B4_DB0_CONNECT_CNTL_OFFSET                        15
#define GMMx25B4_DB0_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_DB0_CONNECT_CNTL_MASK                          0x18000
#define GMMx25B4_CB1_CONNECT_CNTL_OFFSET                        17
#define GMMx25B4_CB1_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_CB1_CONNECT_CNTL_MASK                          0x60000
#define GMMx25B4_DB1_CONNECT_CNTL_OFFSET                        19
#define GMMx25B4_DB1_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_DB1_CONNECT_CNTL_MASK                          0x180000
#define GMMx25B4_TC0_CONNECT_CNTL_OFFSET                        21
#define GMMx25B4_TC0_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_TC0_CONNECT_CNTL_MASK                          0x600000
#define GMMx25B4_TC1_CONNECT_CNTL_OFFSET                        23
#define GMMx25B4_TC1_CONNECT_CNTL_WIDTH                         2
#define GMMx25B4_TC1_CONNECT_CNTL_MASK                          0x1800000
#define GMMx25B4_CB0_CID_CNTL_ENABLE_OFFSET                     25
#define GMMx25B4_CB0_CID_CNTL_ENABLE_WIDTH                      1
#define GMMx25B4_CB0_CID_CNTL_ENABLE_MASK                       0x2000000
#define GMMx25B4_DB0_CID_CNTL_ENABLE_OFFSET                     26
#define GMMx25B4_DB0_CID_CNTL_ENABLE_WIDTH                      1
#define GMMx25B4_DB0_CID_CNTL_ENABLE_MASK                       0x4000000
#define GMMx25B4_CB1_CID_CNTL_ENABLE_OFFSET                     27
#define GMMx25B4_CB1_CID_CNTL_ENABLE_WIDTH                      1
#define GMMx25B4_CB1_CID_CNTL_ENABLE_MASK                       0x8000000
#define GMMx25B4_DB1_CID_CNTL_ENABLE_OFFSET                     28
#define GMMx25B4_DB1_CID_CNTL_ENABLE_WIDTH                      1
#define GMMx25B4_DB1_CID_CNTL_ENABLE_MASK                       0x10000000
#define GMMx25B4_TC2_REPAIR_ENABLE_OFFSET                       29
#define GMMx25B4_TC2_REPAIR_ENABLE_WIDTH                        2
#define GMMx25B4_TC2_REPAIR_ENABLE_MASK                         0x60000000
#define GMMx25B4_Reserved31_31_OFFSET                           31
#define GMMx25B4_Reserved31_31_WIDTH                            1
#define GMMx25B4_Reserved31_31_MASK                             0x80000000

/// GMMx25B4
typedef union {
  struct {                                                              ///<
    UINT32                                                   CB1_RD:1 ; ///<
    UINT32                                                   CB1_WR:1 ; ///<
    UINT32                                                   DB1_RD:1 ; ///<
    UINT32                                                   DB1_WR:1 ; ///<
    UINT32                                                   TC2_RD:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                                  ARB_DBG:4 ; ///<
    UINT32                                                   TC2_WR:1 ; ///<
    UINT32                                         CB0_CONNECT_CNTL:2 ; ///<
    UINT32                                         DB0_CONNECT_CNTL:2 ; ///<
    UINT32                                         CB1_CONNECT_CNTL:2 ; ///<
    UINT32                                         DB1_CONNECT_CNTL:2 ; ///<
    UINT32                                         TC0_CONNECT_CNTL:2 ; ///<
    UINT32                                         TC1_CONNECT_CNTL:2 ; ///<
    UINT32                                      CB0_CID_CNTL_ENABLE:1 ; ///<
    UINT32                                      DB0_CID_CNTL_ENABLE:1 ; ///<
    UINT32                                      CB1_CID_CNTL_ENABLE:1 ; ///<
    UINT32                                      DB1_CID_CNTL_ENABLE:1 ; ///<
    UINT32                                        TC2_REPAIR_ENABLE:2 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx25B4_STRUCT;

// **** GMMx2760 Register Definition ****
// Address
#define GMMx2760_ADDRESS                                        0x2760

// Type
#define GMMx2760_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2760_NOOFBANK_OFFSET                                0
#define GMMx2760_NOOFBANK_WIDTH                                 2
#define GMMx2760_NOOFBANK_MASK                                  0x3
#define GMMx2760_NOOFRANKS_OFFSET                               2
#define GMMx2760_NOOFRANKS_WIDTH                                1
#define GMMx2760_NOOFRANKS_MASK                                 0x4
#define GMMx2760_NOOFROWS_OFFSET                                3
#define GMMx2760_NOOFROWS_WIDTH                                 3
#define GMMx2760_NOOFROWS_MASK                                  0x38
#define GMMx2760_NOOFCOLS_OFFSET                                6
#define GMMx2760_NOOFCOLS_WIDTH                                 2
#define GMMx2760_NOOFCOLS_MASK                                  0xc0
#define GMMx2760_CHANSIZE_OFFSET                                8
#define GMMx2760_CHANSIZE_WIDTH                                 1
#define GMMx2760_CHANSIZE_MASK                                  0x100
#define GMMx2760_BURSTLENGTH_OFFSET                             9
#define GMMx2760_BURSTLENGTH_WIDTH                              1
#define GMMx2760_BURSTLENGTH_MASK                               0x200
#define GMMx2760_REQUEST_512B_OFFSET                            10
#define GMMx2760_REQUEST_512B_WIDTH                             1
#define GMMx2760_REQUEST_512B_MASK                              0x400
#define GMMx2760_CHANSIZE_OVERRIDE_OFFSET                       11
#define GMMx2760_CHANSIZE_OVERRIDE_WIDTH                        1
#define GMMx2760_CHANSIZE_OVERRIDE_MASK                         0x800
#define GMMx2760_NOOFGROUPS_OFFSET                              12
#define GMMx2760_NOOFGROUPS_WIDTH                               1
#define GMMx2760_NOOFGROUPS_MASK                                0x1000
#define GMMx2760_BURST_TIME_OFFSET                              13
#define GMMx2760_BURST_TIME_WIDTH                               5
#define GMMx2760_BURST_TIME_MASK                                0x3e000
#define GMMx2760_Reserved_OFFSET                                18
#define GMMx2760_Reserved_WIDTH                                 14
#define GMMx2760_Reserved_MASK                                  0xfffc0000

/// GMMx2760
typedef union {
  struct {                                                              ///<
    UINT32                                                 NOOFBANK:2 ; ///<
    UINT32                                                NOOFRANKS:1 ; ///<
    UINT32                                                 NOOFROWS:3 ; ///<
    UINT32                                                 NOOFCOLS:2 ; ///<
    UINT32                                                 CHANSIZE:1 ; ///<
    UINT32                                              BURSTLENGTH:1 ; ///<
    UINT32                                             REQUEST_512B:1 ; ///<
    UINT32                                        CHANSIZE_OVERRIDE:1 ; ///<
    UINT32                                               NOOFGROUPS:1 ; ///<
    UINT32                                               BURST_TIME:5 ; ///<
    UINT32                                                 Reserved:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2760_STRUCT;

// **** GMMx2768 Register Definition ****
// Address
#define GMMx2768_ADDRESS                                        0x2768

// Type
#define GMMx2768_TYPE                                           TYPE_GMM
// Field Data
#define GMMx2768_READ_CLKS_OFFSET                               0
#define GMMx2768_READ_CLKS_WIDTH                                8
#define GMMx2768_READ_CLKS_MASK                                 0xff
#define GMMx2768_WRITE_CLKS_OFFSET                              8
#define GMMx2768_WRITE_CLKS_WIDTH                               8
#define GMMx2768_WRITE_CLKS_MASK                                0xff00
#define GMMx2768_ARB_RW_SWITCH_OFFSET                           16
#define GMMx2768_ARB_RW_SWITCH_WIDTH                            1
#define GMMx2768_ARB_RW_SWITCH_MASK                             0x10000
#define GMMx2768_RW_SWITCH_HARSH_OFFSET                         17
#define GMMx2768_RW_SWITCH_HARSH_WIDTH                          2
#define GMMx2768_RW_SWITCH_HARSH_MASK                           0x60000
#define GMMx2768_Reserved_OFFSET                                19
#define GMMx2768_Reserved_WIDTH                                 13
#define GMMx2768_Reserved_MASK                                  0xfff80000

/// GMMx2768
typedef union {
  struct {                                                              ///<
    UINT32                                                READ_CLKS:8 ; ///<
    UINT32                                               WRITE_CLKS:8 ; ///<
    UINT32                                            ARB_RW_SWITCH:1 ; ///<
    UINT32                                          RW_SWITCH_HARSH:2 ; ///<
    UINT32                                                 Reserved:13; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx2768_STRUCT;

// **** GMMx2C04 Register Definition ****
// Address
#define GMMx2C04_ADDRESS                                        0x2C04
// Type
#define GMMx2C04_TYPE                                           TYPE_GMM

// Field Data
#define GMMx2C04_NONSURF_BASE_OFFSET                            0
#define GMMx2C04_NONSURF_BASE_WIDTH                             32
#define GMMx2C04_NONSURF_BASE_MASK                              0xFFFFFFFF

/// GMMx2C04
typedef union {
  struct {                                                            ///<
    UINT32                                            NONSURF_BASE:32; ///<

  } Field;

  UINT32 Value;
} GMMx2C04_STRUCT;

// **** GMMx3508 Register Definition ****
// Address
#define GMMx3508_ADDRESS                                        0x3508
// Type
#define GMMx3508_TYPE                                           TYPE_GMM

// Field Data
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_OFFSET                  0
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_WIDTH                   1
#define GMMx3508_RENG_EXECUTE_ON_PWR_UP_MASK                    0x1
#define GMMx3508_RENG_EXECUTE_NOW_OFFSET                        1
#define GMMx3508_RENG_EXECUTE_NOW_WIDTH                         1
#define GMMx3508_RENG_EXECUTE_NOW_MASK                          0x2
#define GMMx3508_RENG_EXECUTE_NOW_START_PTR_OFFSET              2
#define GMMx3508_RENG_EXECUTE_NOW_START_PTR_WIDTH               10
#define GMMx3508_RENG_EXECUTE_NOW_START_PTR_MASK                0xFFC
#define GMMx3508_RENG_EXECUTE_DSP_END_PTR_OFFSET                12
#define GMMx3508_RENG_EXECUTE_DSP_END_PTR_WIDTH                 10
#define GMMx3508_RENG_EXECUTE_DSP_END_PTR_MASK                  0x3FF000
#define GMMx3508_RENG_EXECUTE_END_PTR_OFFSET                    22
#define GMMx3508_RENG_EXECUTE_END_PTR_WIDTH                     10
#define GMMx3508_RENG_EXECUTE_END_PTR_MASK                      0xFFC00000

/// GMMx3508
typedef union {
  struct {                                                            ///<
    UINT32                                  RENG_EXECUTE_ON_PWR_UP:1; ///<
    UINT32                                        RENG_EXECUTE_NOW:1; ///<
    UINT32                              RENG_EXECUTE_NOW_START_PTR:10; ///<
    UINT32                                RENG_EXECUTE_DSP_END_PTR:10; ///<
    UINT32                                    RENG_EXECUTE_END_PTR:10; ///<

  } Field;

  UINT32 Value;
} GMMx3508_STRUCT;

// **** GMMx350C Register Definition ****
// Address
#define GMMx350C_ADDRESS                                        0x350C
// Type
#define GMMx350C_TYPE                                           TYPE_GMM

// Field Data
#define GMMx350C_Reserved_10_0_OFFSET                           0
#define GMMx350C_Reserved_10_0_WIDTH                            11
#define GMMx350C_Reserved_10_0_MASK                             0x7FF
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_OFFSET              11
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_WIDTH               1
#define GMMx350C_RENG_EXECUTE_ON_REG_UPDATE_MASK                0x800
#define GMMx350C_Reserved_15_12_OFFSET                          12
#define GMMx350C_Reserved_15_12_WIDTH                           4
#define GMMx350C_Reserved_15_12_MASK                            0xF000
#define GMMx350C_STCTRL_STUTTER_EN_OFFSET                       16
#define GMMx350C_STCTRL_STUTTER_EN_WIDTH                        1
#define GMMx350C_STCTRL_STUTTER_EN_MASK                         0x10000
#define GMMx350C_Reserved_31_17_OFFSET                          17
#define GMMx350C_Reserved_31_17_WIDTH                           15
#define GMMx350C_Reserved_31_17_MASK                            0xFFFE0000

/// GMMx350C
typedef union {
  struct {                                                            ///<
    UINT32                                           Reserved_10_0:11; ///<
    UINT32                              RENG_EXECUTE_ON_REG_UPDATE:1 ; ///<
    UINT32                                          Reserved_15_12:4 ; ///<
    UINT32                                       STCTRL_STUTTER_EN:1 ; ///<
    UINT32                                          Reserved_31_17:15; ///<
  } Field;

  UINT32 Value;
} GMMx350C_STRUCT;

// **** GMMx3510 Register Definition ****
// Address
#define GMMx3510_ADDRESS                                        0x3510
// Type
#define GMMx3510_TYPE                                           TYPE_GMM

// Field Data
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_OFFSET           0
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_WIDTH            3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE0_MASK             0x7
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_OFFSET           3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_WIDTH            3
#define GMMx3510_RENG_MEM_POWER_CTRL_OVERRIDE1_MASK             0x38
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_OFFSET           6
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_WIDTH            5
#define GMMx3510_STCTRL_NONDISP_IDLE_THRESHOLD_MASK             0x7c0
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_OFFSET                  11
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_WIDTH                   6
#define GMMx3510_RENG_SR_HOLD_THRESHOLD_MASK                    0x1f800
#define GMMx3510_STCTRL_LPT_TARGET_OFFSET                       17
#define GMMx3510_STCTRL_LPT_TARGET_WIDTH                        12
#define GMMx3510_STCTRL_LPT_TARGET_MASK                         0x1ffe0000
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_OFFSET                  29
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_WIDTH                   1
#define GMMx3510_STCTRL_IGNORE_ARB_BUSY_MASK                    0x20000000
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_OFFSET               30
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_WIDTH                1
#define GMMx3510_STCTRL_EXTEND_GMC_OFFLINE_MASK                 0x40000000
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_OFFSET             31
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_WIDTH              1
#define GMMx3510_STCTRL_TIMER_PULSE_OVERRIDE_MASK               0x80000000

/// GMMx3510
typedef union {
  struct {                                                              ///<
    UINT32                            RENG_MEM_POWER_CTRL_OVERRIDE0:3 ; ///<
    UINT32                            RENG_MEM_POWER_CTRL_OVERRIDE1:3 ; ///<
    UINT32                            STCTRL_NONDISP_IDLE_THRESHOLD:5 ; ///<
    UINT32                                   RENG_SR_HOLD_THRESHOLD:6 ; ///<
    UINT32                                        STCTRL_LPT_TARGET:12; ///<
    UINT32                                   STCTRL_IGNORE_ARB_BUSY:1 ; ///<
    UINT32                                STCTRL_EXTEND_GMC_OFFLINE:1 ; ///<
    UINT32                              STCTRL_TIMER_PULSE_OVERRIDE:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx3510_STRUCT;

// **** GMMx3544 Register Definition ****
// Address
#define GMMx3544_ADDRESS                                        0x3544

// Type
#define GMMx3544_TYPE                                           TYPE_GMM
// Field Data
#define GMMx3544_RENG_DISABLE_MCC_OFFSET                        0
#define GMMx3544_RENG_DISABLE_MCC_WIDTH                         6
#define GMMx3544_RENG_DISABLE_MCC_MASK                          0x3f
#define GMMx3544_RENG_DISABLE_MCD_OFFSET                        6
#define GMMx3544_RENG_DISABLE_MCD_WIDTH                         6
#define GMMx3544_RENG_DISABLE_MCD_MASK                          0xfc0
#define GMMx3544_STCTRL_FORCE_PGFSM_CMD_DONE_OFFSET             12
#define GMMx3544_STCTRL_FORCE_PGFSM_CMD_DONE_WIDTH              12
#define GMMx3544_STCTRL_FORCE_PGFSM_CMD_DONE_MASK               0xfff000
#define GMMx3544_STCTRL_IGNORE_ALLOW_STUTTER_OFFSET             24
#define GMMx3544_STCTRL_IGNORE_ALLOW_STUTTER_WIDTH              1
#define GMMx3544_STCTRL_IGNORE_ALLOW_STUTTER_MASK               0x1000000
#define GMMx3544_RENG_MEM_LS_ENABLE_OFFSET                      25
#define GMMx3544_RENG_MEM_LS_ENABLE_WIDTH                       1
#define GMMx3544_RENG_MEM_LS_ENABLE_MASK                        0x2000000
#define GMMx3544_STCTRL_EXCLUDE_NONMEM_CLIENTS_OFFSET           26
#define GMMx3544_STCTRL_EXCLUDE_NONMEM_CLIENTS_WIDTH            1
#define GMMx3544_STCTRL_EXCLUDE_NONMEM_CLIENTS_MASK             0x4000000
#define GMMx3544_Reserved_OFFSET                                27
#define GMMx3544_Reserved_WIDTH                                 5
#define GMMx3544_Reserved_MASK                                  0xf8000000

/// GMMx3544
typedef union {
  struct {                                                              ///<
    UINT32                                         RENG_DISABLE_MCC:6 ; ///<
    UINT32                                         RENG_DISABLE_MCD:6 ; ///<
    UINT32                              STCTRL_FORCE_PGFSM_CMD_DONE:12; ///<
    UINT32                              STCTRL_IGNORE_ALLOW_STUTTER:1 ; ///<
    UINT32                                       RENG_MEM_LS_ENABLE:1 ; ///<
    UINT32                            STCTRL_EXCLUDE_NONMEM_CLIENTS:1 ; ///<
    UINT32                                                 Reserved:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx3544_STRUCT;

// **** GMMx3700 Register Definition ****
// Address
#define GMMx3700_ADDRESS                                        0x3700
// Type
#define GMMx3700_TYPE                                           TYPE_GMM

// **** GMMx3704 Register Definition ****
// Address
#define GMMx3704_ADDRESS                                        0x3704
// Type
#define GMMx3704_TYPE                                           TYPE_GMM

// **** GMMx3748 Register Definition ****
// Address
#define GMMx3748_ADDRESS                                        0x3748
// Type
#define GMMx3748_TYPE                                           TYPE_GMM

// **** GMMx3750 Register Definition ****
// Address
#define GMMx3750_ADDRESS                                        0x3750
// Type
#define GMMx3750_TYPE                                           TYPE_GMM

// **** GMMx3754 Register Definition ****
// Address
#define GMMx3754_ADDRESS                                        0x3754
// Type
#define GMMx3754_TYPE                                           TYPE_GMM

// **** GMMx3758 Register Definition ****
// Address
#define GMMx3758_ADDRESS                                        0x3758
// Type
#define GMMx3758_TYPE                                           TYPE_GMM

// **** GMMx375C Register Definition ****
// Address
#define GMMx375C_ADDRESS                                        0x375C
// Type
#define GMMx375C_TYPE                                           TYPE_GMM

// **** GMMx5428 Register Definition ****
// Address
#define GMMx5428_ADDRESS                                        0x5428
// Type
#define GMMx5428_TYPE                                           TYPE_GMM

// Field Data
#define GMMx5428_CONFIG_MEMSIZE_OFFSET                          0
#define GMMx5428_CONFIG_MEMSIZE_WIDTH                           32
#define GMMx5428_CONFIG_MEMSIZE_MASK                            0xFFFFFFFF

/// GMMx5428
typedef union {
  struct {                                                            ///<
    UINT32                                          CONFIG_MEMSIZE:32; ///<

  } Field;

  UINT32 Value;
} GMMx5428_STRUCT;

// **** GMMx5490 Register Definition ****
// Address
#define GMMx5490_ADDRESS                                        0x5490
// Type
#define GMMx5490_TYPE                                           TYPE_GMM

// Field Data
#define GMMx5490_FB_READ_EN_OFFSET                              0
#define GMMx5490_FB_READ_EN_WIDTH                               1
#define GMMx5490_FB_READ_EN_MASK                                0x1
#define GMMx5490_FB_WRITE_EN_OFFSET                             1
#define GMMx5490_FB_WRITE_EN_WIDTH                              1
#define GMMx5490_FB_WRITE_EN_MASK                               0x2
#define GMMx5490_Reserved_31_2_OFFSET                           2
#define GMMx5490_Reserved_31_2_WIDTH                            30
#define GMMx5490_Reserved_31_2_MASK                             0xFFFFFFFC

/// GMMx5490
typedef union {
  struct {                                                            ///<
    UINT32                                              FB_READ_EN:1; ///<
    UINT32                                             FB_WRITE_EN:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} GMMx5490_STRUCT;

// **** GMMx5C1C Register Definition ****
// Address
#define GMMx5C1C_ADDRESS                                        0x5C1C

// Type
#define GMMx5C1C_TYPE                                           TYPE_GMM
// Field Data
#define GMMx5C1C_PLL_RESET_OFFSET                               0
#define GMMx5C1C_PLL_RESET_WIDTH                                1
#define GMMx5C1C_PLL_RESET_MASK                                 0x1
#define GMMx5C1C_PLL_POWER_DOWN_OFFSET                          1
#define GMMx5C1C_PLL_POWER_DOWN_WIDTH                           1
#define GMMx5C1C_PLL_POWER_DOWN_MASK                            0x2
#define GMMx5C1C_PLL_BYPASS_CAL_OFFSET                          2
#define GMMx5C1C_PLL_BYPASS_CAL_WIDTH                           1
#define GMMx5C1C_PLL_BYPASS_CAL_MASK                            0x4
#define GMMx5C1C_PLL_POST_DIV_SRC_OFFSET                        3
#define GMMx5C1C_PLL_POST_DIV_SRC_WIDTH                         1
#define GMMx5C1C_PLL_POST_DIV_SRC_MASK                          0x8
#define GMMx5C1C_PLL_VCOREF_OFFSET                              4
#define GMMx5C1C_PLL_VCOREF_WIDTH                               2
#define GMMx5C1C_PLL_VCOREF_MASK                                0x30
#define GMMx5C1C_PLL_PCIE_REFCLK_SEL_OFFSET                     6
#define GMMx5C1C_PLL_PCIE_REFCLK_SEL_WIDTH                      1
#define GMMx5C1C_PLL_PCIE_REFCLK_SEL_MASK                       0x40
#define GMMx5C1C_PLL_ANTIGLITCH_RESETB_OFFSET                   7
#define GMMx5C1C_PLL_ANTIGLITCH_RESETB_WIDTH                    1
#define GMMx5C1C_PLL_ANTIGLITCH_RESETB_MASK                     0x80
#define GMMx5C1C_PLL_CALREF_OFFSET                              8
#define GMMx5C1C_PLL_CALREF_WIDTH                               2
#define GMMx5C1C_PLL_CALREF_MASK                                0x300
#define GMMx5C1C_PLL_CAL_BYPASS_REFDIV_OFFSET                   10
#define GMMx5C1C_PLL_CAL_BYPASS_REFDIV_WIDTH                    1
#define GMMx5C1C_PLL_CAL_BYPASS_REFDIV_MASK                     0x400
#define GMMx5C1C_PLL_REFCLK_SEL_OFFSET                          11
#define GMMx5C1C_PLL_REFCLK_SEL_WIDTH                           2
#define GMMx5C1C_PLL_REFCLK_SEL_MASK                            0x1800
#define GMMx5C1C_PLL_ANTI_GLITCH_RESET_OFFSET                   13
#define GMMx5C1C_PLL_ANTI_GLITCH_RESET_WIDTH                    1
#define GMMx5C1C_PLL_ANTI_GLITCH_RESET_MASK                     0x2000
#define GMMx5C1C_PLL_XOCLK_DRV_R_EN_OFFSET                      14
#define GMMx5C1C_PLL_XOCLK_DRV_R_EN_WIDTH                       1
#define GMMx5C1C_PLL_XOCLK_DRV_R_EN_MASK                        0x4000
#define GMMx5C1C_Reserved_OFFSET                                15
#define GMMx5C1C_Reserved_WIDTH                                 1
#define GMMx5C1C_Reserved_MASK                                  0x8000
#define GMMx5C1C_PLL_REF_DIV_SRC_OFFSET                         16
#define GMMx5C1C_PLL_REF_DIV_SRC_WIDTH                          3
#define GMMx5C1C_PLL_REF_DIV_SRC_MASK                           0x70000
#define GMMx5C1C_PLL_LOCK_FREQ_SEL_OFFSET                       19
#define GMMx5C1C_PLL_LOCK_FREQ_SEL_WIDTH                        1
#define GMMx5C1C_PLL_LOCK_FREQ_SEL_MASK                         0x80000
#define GMMx5C1C_PLL_CALIB_DONE_OFFSET                          20
#define GMMx5C1C_PLL_CALIB_DONE_WIDTH                           1
#define GMMx5C1C_PLL_CALIB_DONE_MASK                            0x100000
#define GMMx5C1C_PLL_LOCKED_OFFSET                              21
#define GMMx5C1C_PLL_LOCKED_WIDTH                               1
#define GMMx5C1C_PLL_LOCKED_MASK                                0x200000
#define GMMx5C1C_Reserved22_23_OFFSET                           22
#define GMMx5C1C_Reserved22_23_WIDTH                            2
#define GMMx5C1C_Reserved22_23_MASK                             0xc00000
#define GMMx5C1C_PLL_TIMING_MODE_STATUS_OFFSET                  24
#define GMMx5C1C_PLL_TIMING_MODE_STATUS_WIDTH                   2
#define GMMx5C1C_PLL_TIMING_MODE_STATUS_MASK                    0x3000000
#define GMMx5C1C_PLL_DIG_SPARE_OFFSET                           26
#define GMMx5C1C_PLL_DIG_SPARE_WIDTH                            6
#define GMMx5C1C_PLL_DIG_SPARE_MASK                             0xfc000000

/// GMMx5C1C
typedef union {
  struct {                                                              ///<
    UINT32                                                PLL_RESET:1 ; ///<
    UINT32                                           PLL_POWER_DOWN:1 ; ///<
    UINT32                                           PLL_BYPASS_CAL:1 ; ///<
    UINT32                                         PLL_POST_DIV_SRC:1 ; ///<
    UINT32                                               PLL_VCOREF:2 ; ///<
    UINT32                                      PLL_PCIE_REFCLK_SEL:1 ; ///<
    UINT32                                    PLL_ANTIGLITCH_RESETB:1 ; ///<
    UINT32                                               PLL_CALREF:2 ; ///<
    UINT32                                    PLL_CAL_BYPASS_REFDIV:1 ; ///<
    UINT32                                           PLL_REFCLK_SEL:2 ; ///<
    UINT32                                    PLL_ANTI_GLITCH_RESET:1 ; ///<
    UINT32                                       PLL_XOCLK_DRV_R_EN:1 ; ///<
    UINT32                                                 Reserved:1 ; ///<
    UINT32                                          PLL_REF_DIV_SRC:3 ; ///<
    UINT32                                        PLL_LOCK_FREQ_SEL:1 ; ///<
    UINT32                                           PLL_CALIB_DONE:1 ; ///<
    UINT32                                               PLL_LOCKED:1 ; ///<
    UINT32                                            Reserved22_23:2 ; ///<
    UINT32                                   PLL_TIMING_MODE_STATUS:2 ; ///<
    UINT32                                            PLL_DIG_SPARE:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx5C1C_STRUCT;

// **** GMMx5C90 Register Definition ****
// Address
#define GMMx5C90_ADDRESS                                        0x5c90

// Type
#define GMMx5C90_TYPE                                           TYPE_GMM
// Field Data
#define GMMx5C90_PLL_XOR_LOCK_OFFSET                            0
#define GMMx5C90_PLL_XOR_LOCK_WIDTH                             1
#define GMMx5C90_PLL_XOR_LOCK_MASK                              0x1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_OFFSET                   1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_WIDTH                    1
#define GMMx5C90_PLL_XOR_LOCK_READBACK_MASK                     0x2
#define GMMx5C90_Reserved_OFFSET                                2
#define GMMx5C90_Reserved_WIDTH                                 6
#define GMMx5C90_Reserved_MASK                                  0xfc
#define GMMx5C90_PLL_SPARE_OFFSET                               8
#define GMMx5C90_PLL_SPARE_WIDTH                                6
#define GMMx5C90_PLL_SPARE_MASK                                 0x3f00
#define GMMx5C90_Reserved14_31_OFFSET                           14
#define GMMx5C90_Reserved14_31_WIDTH                            18
#define GMMx5C90_Reserved14_31_MASK                             0xffffc000

/// GMMx5C90
typedef union {
  struct {                                                              ///<
    UINT32                                             PLL_XOR_LOCK:1 ; ///<
    UINT32                                    PLL_XOR_LOCK_READBACK:1 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                                PLL_SPARE:6 ; ///<
    UINT32                                            Reserved14_31:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx5C90_STRUCT;

// **** GMMx5F50 Register Definition ****
// Address
#define GMMx5F50_ADDRESS                                        0x5f50

// Type
#define GMMx5F50_TYPE                                           TYPE_GMM
// Field Data
#define GMMx5F50_PORT_CONNECTIVITY_OFFSET                       0
#define GMMx5F50_PORT_CONNECTIVITY_WIDTH                        3
#define GMMx5F50_PORT_CONNECTIVITY_MASK                         0x7
#define GMMx5F50_Reserved_OFFSET                                3
#define GMMx5F50_Reserved_WIDTH                                 1
#define GMMx5F50_Reserved_MASK                                  0x8
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_OFFSET       4
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_WIDTH        1
#define GMMx5F50_PORT_CONNECTIVITY_OVERRIDE_ENABLE_MASK         0x10
#define GMMx5F50_Reserved5_31_OFFSET                            5
#define GMMx5F50_Reserved5_31_WIDTH                             27
#define GMMx5F50_Reserved5_31_MASK                              0xffffffe0

/// GMMx5F50
typedef union {
  struct {                                                              ///<
    UINT32                                        PORT_CONNECTIVITY:3 ; ///<
    UINT32                                                 Reserved:1 ; ///<
    UINT32                        PORT_CONNECTIVITY_OVERRIDE_ENABLE:1 ; ///<
    UINT32                                             Reserved5_31:27; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx5F50_STRUCT;

// **** GMMx6464 Register Definition ****
// Address
#define GMMx6464_ADDRESS                                        0x6464
// Type
#define GMMx6464_TYPE                                           TYPE_GMM

// Field Data
#define GMMx6464_LVTMA_PWRSEQ_EN_OFFSET                         0
#define GMMx6464_LVTMA_PWRSEQ_EN_WIDTH                          1
#define GMMx6464_LVTMA_PWRSEQ_EN_MASK                           0x1
#define GMMx6464_LVTMA_PWRSEQ_DISABLE_SYNCEN_CONTROL_OF_TX_EN_OFFSET 1
#define GMMx6464_LVTMA_PWRSEQ_DISABLE_SYNCEN_CONTROL_OF_TX_EN_WIDTH 1
#define GMMx6464_LVTMA_PWRSEQ_DISABLE_SYNCEN_CONTROL_OF_TX_EN_MASK 0x2
#define GMMx6464_Reserved_3_2_OFFSET                            2
#define GMMx6464_Reserved_3_2_WIDTH                             2
#define GMMx6464_Reserved_3_2_MASK                              0xC
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_OFFSET               4
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_WIDTH                1
#define GMMx6464_LVTMA_PWRSEQ_TARGET_STATE_MASK                 0x10
#define GMMx6464_Reserved_7_5_OFFSET                            5
#define GMMx6464_Reserved_7_5_WIDTH                             3
#define GMMx6464_Reserved_7_5_MASK                              0xE0
#define GMMx6464_LVTMA_SYNCEN_OFFSET                            8
#define GMMx6464_LVTMA_SYNCEN_WIDTH                             1
#define GMMx6464_LVTMA_SYNCEN_MASK                              0x100
#define GMMx6464_LVTMA_SYNCEN_OVRD_OFFSET                       9
#define GMMx6464_LVTMA_SYNCEN_OVRD_WIDTH                        1
#define GMMx6464_LVTMA_SYNCEN_OVRD_MASK                         0x200
#define GMMx6464_LVTMA_SYNCEN_POL_OFFSET                        10
#define GMMx6464_LVTMA_SYNCEN_POL_WIDTH                         1
#define GMMx6464_LVTMA_SYNCEN_POL_MASK                          0x400
#define GMMx6464_Reserved_15_11_OFFSET                          11
#define GMMx6464_Reserved_15_11_WIDTH                           5
#define GMMx6464_Reserved_15_11_MASK                            0xF800
#define GMMx6464_LVTMA_DIGON_OFFSET                             16
#define GMMx6464_LVTMA_DIGON_WIDTH                              1
#define GMMx6464_LVTMA_DIGON_MASK                               0x10000
#define GMMx6464_LVTMA_DIGON_OVRD_OFFSET                        17
#define GMMx6464_LVTMA_DIGON_OVRD_WIDTH                         1
#define GMMx6464_LVTMA_DIGON_OVRD_MASK                          0x20000
#define GMMx6464_LVTMA_DIGON_POL_OFFSET                         18
#define GMMx6464_LVTMA_DIGON_POL_WIDTH                          1
#define GMMx6464_LVTMA_DIGON_POL_MASK                           0x40000
#define GMMx6464_Reserved_23_19_OFFSET                          19
#define GMMx6464_Reserved_23_19_WIDTH                           5
#define GMMx6464_Reserved_23_19_MASK                            0xF80000
#define GMMx6464_LVTMA_BLON_OFFSET                              24
#define GMMx6464_LVTMA_BLON_WIDTH                               1
#define GMMx6464_LVTMA_BLON_MASK                                0x1000000
#define GMMx6464_LVTMA_BLON_OVRD_OFFSET                         25
#define GMMx6464_LVTMA_BLON_OVRD_WIDTH                          1
#define GMMx6464_LVTMA_BLON_OVRD_MASK                           0x2000000
#define GMMx6464_LVTMA_BLON_POL_OFFSET                          26
#define GMMx6464_LVTMA_BLON_POL_WIDTH                           1
#define GMMx6464_LVTMA_BLON_POL_MASK                            0x4000000
#define GMMx6464_Reserved_31_27_OFFSET                          27
#define GMMx6464_Reserved_31_27_WIDTH                           5
#define GMMx6464_Reserved_31_27_MASK                            0xF8000000

/// GMMx6464
typedef union {
  struct {                                                            ///<
    UINT32                                         LVTMA_PWRSEQ_EN:1; ///<
    UINT32            LVTMA_PWRSEQ_DISABLE_SYNCEN_CONTROL_OF_TX_EN:1; ///<
    UINT32                                            Reserved_3_2:2; ///<
    UINT32                               LVTMA_PWRSEQ_TARGET_STATE:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                            LVTMA_SYNCEN:1; ///<
    UINT32                                       LVTMA_SYNCEN_OVRD:1; ///<
    UINT32                                        LVTMA_SYNCEN_POL:1; ///<
    UINT32                                          Reserved_15_11:5; ///<
    UINT32                                             LVTMA_DIGON:1; ///<
    UINT32                                        LVTMA_DIGON_OVRD:1; ///<
    UINT32                                         LVTMA_DIGON_POL:1; ///<
    UINT32                                          Reserved_23_19:5; ///<
    UINT32                                              LVTMA_BLON:1; ///<
    UINT32                                         LVTMA_BLON_OVRD:1; ///<
    UINT32                                          LVTMA_BLON_POL:1; ///<
    UINT32                                          Reserved_31_27:5; ///<

  } Field;

  UINT32 Value;
} GMMx6464_STRUCT;

// **** GMMx6CC8 Register Definition ****
// Address
#define GMMx6CC8_ADDRESS                                        0x6cc8

// Type
#define GMMx6CC8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx6CC8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx6CC8_Reserved_OFFSET                                2
#define GMMx6CC8_Reserved_WIDTH                                 6
#define GMMx6CC8_Reserved_MASK                                  0xfc
#define GMMx6CC8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx6CC8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx6CC8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx6CC8_Reserved10_15_OFFSET                           10
#define GMMx6CC8_Reserved10_15_WIDTH                            6
#define GMMx6CC8_Reserved10_15_MASK                             0xfc00
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx6CC8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx6CC8_Reserved18_31_OFFSET                           18
#define GMMx6CC8_Reserved18_31_WIDTH                            14
#define GMMx6CC8_Reserved18_31_MASK                             0xfffc0000

/// GMMx6CC8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CC8_STRUCT;

// **** GMMx6CD4 Register Definition ****
// Address
#define GMMx6CD4_ADDRESS                                        0x6cd4

// Type
#define GMMx6CD4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CD4_STUTTER_ENABLE_OFFSET                          0
#define GMMx6CD4_STUTTER_ENABLE_WIDTH                           1
#define GMMx6CD4_STUTTER_ENABLE_MASK                            0x1
#define GMMx6CD4_Reserved_OFFSET                                1
#define GMMx6CD4_Reserved_WIDTH                                 3
#define GMMx6CD4_Reserved_MASK                                  0xe
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx6CD4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx6CD4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx6CD4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx6CD4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx6CD4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx6CD4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx6CD4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx6CD4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx6CD4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx6CD4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx6CD4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx6CD4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx6CD4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx6CD4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx6CD4_Reserved12_15_OFFSET                           12
#define GMMx6CD4_Reserved12_15_WIDTH                            4
#define GMMx6CD4_Reserved12_15_MASK                             0xf000
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx6CD4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx6CD4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CD4_STRUCT;

// **** GMMx6CD8 Register Definition ****
// Address
#define GMMx6CD8_ADDRESS                                        0x6cd8

// Type
#define GMMx6CD8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx6CD8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx6CD8_Reserved_3_1_OFFSET                            1
#define GMMx6CD8_Reserved_3_1_WIDTH                             3
#define GMMx6CD8_Reserved_3_1_MASK                              0xe
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx6CD8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx6CD8_Reserved_7_5_OFFSET                            5
#define GMMx6CD8_Reserved_7_5_WIDTH                             3
#define GMMx6CD8_Reserved_7_5_MASK                              0xe0
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx6CD8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx6CD8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx6CD8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx6CD8_Reserved_15_11_OFFSET                          11
#define GMMx6CD8_Reserved_15_11_WIDTH                           5
#define GMMx6CD8_Reserved_15_11_MASK                            0xf800
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx6CD8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx6CD8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx6CD8_STRUCT;

//===================================================

// **** GMMx78C8 Register Definition ****
// Address
#define GMMx78C8_ADDRESS                                        0x78c8

// Type
#define GMMx78C8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx78C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx78C8_Reserved_OFFSET                                2
#define GMMx78C8_Reserved_WIDTH                                 6
#define GMMx78C8_Reserved_MASK                                  0xfc
#define GMMx78C8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx78C8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx78C8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx78C8_Reserved10_15_OFFSET                           10
#define GMMx78C8_Reserved10_15_WIDTH                            6
#define GMMx78C8_Reserved10_15_MASK                             0xfc00
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx78C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx78C8_Reserved18_31_OFFSET                           18
#define GMMx78C8_Reserved18_31_WIDTH                            14
#define GMMx78C8_Reserved18_31_MASK                             0xfffc0000

/// GMMx78C8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78C8_STRUCT;

// **** GMMx78D4 Register Definition ****
// Address
#define GMMx78D4_ADDRESS                                        0x78d4

// Type
#define GMMx78D4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78D4_STUTTER_ENABLE_OFFSET                          0
#define GMMx78D4_STUTTER_ENABLE_WIDTH                           1
#define GMMx78D4_STUTTER_ENABLE_MASK                            0x1
#define GMMx78D4_Reserved_OFFSET                                1
#define GMMx78D4_Reserved_WIDTH                                 3
#define GMMx78D4_Reserved_MASK                                  0xe
#define GMMx78D4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx78D4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx78D4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx78D4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx78D4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx78D4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx78D4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx78D4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx78D4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx78D4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx78D4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx78D4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx78D4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx78D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx78D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx78D4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx78D4_Reserved12_15_OFFSET                           12
#define GMMx78D4_Reserved12_15_WIDTH                            4
#define GMMx78D4_Reserved12_15_MASK                             0xf000
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx78D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx78D4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78D4_STRUCT;

// **** GMMx78D8 Register Definition ****
// Address
#define GMMx78D8_ADDRESS                                        0x78d8

// Type
#define GMMx78D8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx78D8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx78D8_Reserved_3_1_OFFSET                            1
#define GMMx78D8_Reserved_3_1_WIDTH                             3
#define GMMx78D8_Reserved_3_1_MASK                              0xe
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx78D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx78D8_Reserved_7_5_OFFSET                            5
#define GMMx78D8_Reserved_7_5_WIDTH                             3
#define GMMx78D8_Reserved_7_5_MASK                              0xe0
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx78D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx78D8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx78D8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx78D8_Reserved_15_11_OFFSET                          11
#define GMMx78D8_Reserved_15_11_WIDTH                           5
#define GMMx78D8_Reserved_15_11_MASK                            0xf800
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx78D8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx78D8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx78D8_STRUCT;

// **** GMMx104C8 Register Definition ****
// Address
#define GMMx104C8_ADDRESS                                        0x104c8

// Type
#define GMMx104C8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx104C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx104C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx104C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx104C8_Reserved_OFFSET                                2
#define GMMx104C8_Reserved_WIDTH                                 6
#define GMMx104C8_Reserved_MASK                                  0xfc
#define GMMx104C8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx104C8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx104C8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx104C8_Reserved10_15_OFFSET                           10
#define GMMx104C8_Reserved10_15_WIDTH                            6
#define GMMx104C8_Reserved10_15_MASK                             0xfc00
#define GMMx104C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx104C8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx104C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx104C8_Reserved18_31_OFFSET                           18
#define GMMx104C8_Reserved18_31_WIDTH                            14
#define GMMx104C8_Reserved18_31_MASK                             0xfffc0000

/// GMMx104C8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx104C8_STRUCT;

// **** GMMx104D4 Register Definition ****
// Address
#define GMMx104D4_ADDRESS                                        0x104d4

// Type
#define GMMx104D4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx104D4_STUTTER_ENABLE_OFFSET                          0
#define GMMx104D4_STUTTER_ENABLE_WIDTH                           1
#define GMMx104D4_STUTTER_ENABLE_MASK                            0x1
#define GMMx104D4_Reserved_OFFSET                                1
#define GMMx104D4_Reserved_WIDTH                                 3
#define GMMx104D4_Reserved_MASK                                  0xe
#define GMMx104D4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx104D4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx104D4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx104D4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx104D4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx104D4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx104D4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx104D4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx104D4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx104D4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx104D4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx104D4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx104D4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx104D4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx104D4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx104D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx104D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx104D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx104D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx104D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx104D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx104D4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx104D4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx104D4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx104D4_Reserved12_15_OFFSET                           12
#define GMMx104D4_Reserved12_15_WIDTH                            4
#define GMMx104D4_Reserved12_15_MASK                             0xf000
#define GMMx104D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx104D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx104D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx104D4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx104D4_STRUCT;

// **** GMMx104D8 Register Definition ****
// Address
#define GMMx104D8_ADDRESS                                        0x104d8

// Type
#define GMMx104D8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx104D8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx104D8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx104D8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx104D8_Reserved_3_1_OFFSET                            1
#define GMMx104D8_Reserved_3_1_WIDTH                             3
#define GMMx104D8_Reserved_3_1_MASK                              0xe
#define GMMx104D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx104D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx104D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx104D8_Reserved_7_5_OFFSET                            5
#define GMMx104D8_Reserved_7_5_WIDTH                             3
#define GMMx104D8_Reserved_7_5_MASK                              0xe0
#define GMMx104D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx104D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx104D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx104D8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx104D8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx104D8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx104D8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx104D8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx104D8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx104D8_Reserved_15_11_OFFSET                          11
#define GMMx104D8_Reserved_15_11_WIDTH                           5
#define GMMx104D8_Reserved_15_11_MASK                            0xf800
#define GMMx104D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx104D8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx104D8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx104D8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx104D8_STRUCT;

// **** GMMx110C8 Register Definition ****
// Address
#define GMMx110C8_ADDRESS                                        0x110c8

// Type
#define GMMx110C8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx110C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_OFFSET 0
#define GMMx110C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_WIDTH 2
#define GMMx110C8_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK_MASK  0x3
#define GMMx110C8_Reserved_OFFSET                                2
#define GMMx110C8_Reserved_WIDTH                                 6
#define GMMx110C8_Reserved_MASK                                  0xfc
#define GMMx110C8_URGENCY_WATERMARK_MASK_OFFSET                  8
#define GMMx110C8_URGENCY_WATERMARK_MASK_WIDTH                   2
#define GMMx110C8_URGENCY_WATERMARK_MASK_MASK                    0x300
#define GMMx110C8_Reserved10_15_OFFSET                           10
#define GMMx110C8_Reserved10_15_WIDTH                            6
#define GMMx110C8_Reserved10_15_MASK                             0xfc00
#define GMMx110C8_NB_PSTATE_CHANGE_WATERMARK_MASK_OFFSET         16
#define GMMx110C8_NB_PSTATE_CHANGE_WATERMARK_MASK_WIDTH          2
#define GMMx110C8_NB_PSTATE_CHANGE_WATERMARK_MASK_MASK           0x30000
#define GMMx110C8_Reserved18_31_OFFSET                           18
#define GMMx110C8_Reserved18_31_WIDTH                            14
#define GMMx110C8_Reserved18_31_MASK                             0xfffc0000

/// GMMx110C8
typedef union {
  struct {                                                              ///<
    UINT32                 STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK:2 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                   URGENCY_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved10_15:6 ; ///<
    UINT32                          NB_PSTATE_CHANGE_WATERMARK_MASK:2 ; ///<
    UINT32                                            Reserved18_31:14; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx110C8_STRUCT;

// **** GMMx110D4 Register Definition ****
// Address
#define GMMx110D4_ADDRESS                                        0x110d4

// Type
#define GMMx110D4_TYPE                                           TYPE_GMM
// Field Data
#define GMMx110D4_STUTTER_ENABLE_OFFSET                          0
#define GMMx110D4_STUTTER_ENABLE_WIDTH                           1
#define GMMx110D4_STUTTER_ENABLE_MASK                            0x1
#define GMMx110D4_Reserved_OFFSET                                1
#define GMMx110D4_Reserved_WIDTH                                 3
#define GMMx110D4_Reserved_MASK                                  0xe
#define GMMx110D4_STUTTER_IGNORE_CURSOR_OFFSET                   4
#define GMMx110D4_STUTTER_IGNORE_CURSOR_WIDTH                    1
#define GMMx110D4_STUTTER_IGNORE_CURSOR_MASK                     0x10
#define GMMx110D4_STUTTER_IGNORE_ICON_OFFSET                     5
#define GMMx110D4_STUTTER_IGNORE_ICON_WIDTH                      1
#define GMMx110D4_STUTTER_IGNORE_ICON_MASK                       0x20
#define GMMx110D4_STUTTER_IGNORE_VGA_OFFSET                      6
#define GMMx110D4_STUTTER_IGNORE_VGA_WIDTH                       1
#define GMMx110D4_STUTTER_IGNORE_VGA_MASK                        0x40
#define GMMx110D4_STUTTER_IGNORE_FBC_OFFSET                      7
#define GMMx110D4_STUTTER_IGNORE_FBC_WIDTH                       1
#define GMMx110D4_STUTTER_IGNORE_FBC_MASK                        0x80
#define GMMx110D4_STUTTER_WM_HIGH_FORCE_ON_OFFSET                8
#define GMMx110D4_STUTTER_WM_HIGH_FORCE_ON_WIDTH                 1
#define GMMx110D4_STUTTER_WM_HIGH_FORCE_ON_MASK                  0x100
#define GMMx110D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_OFFSET         9
#define GMMx110D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_WIDTH          1
#define GMMx110D4_STUTTER_WM_HIGH_EXCLUDES_VBLANK_MASK           0x200
#define GMMx110D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_OFFSET      10
#define GMMx110D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_WIDTH       1
#define GMMx110D4_STUTTER_URGENT_IN_NOT_SELF_REFRESH_MASK        0x400
#define GMMx110D4_STUTTER_SELF_REFRESH_FORCE_ON_OFFSET           11
#define GMMx110D4_STUTTER_SELF_REFRESH_FORCE_ON_WIDTH            1
#define GMMx110D4_STUTTER_SELF_REFRESH_FORCE_ON_MASK             0x800
#define GMMx110D4_Reserved12_15_OFFSET                           12
#define GMMx110D4_Reserved12_15_WIDTH                            4
#define GMMx110D4_Reserved12_15_MASK                             0xf000
#define GMMx110D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_OFFSET     16
#define GMMx110D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_WIDTH      16
#define GMMx110D4_STUTTER_EXIT_SELF_REFRESH_WATERMARK_MASK       0xffff0000

/// GMMx110D4
typedef union {
  struct {                                                              ///<
    UINT32                                           STUTTER_ENABLE:1 ; ///<
    UINT32                                                 Reserved:3 ; ///<
    UINT32                                    STUTTER_IGNORE_CURSOR:1 ; ///<
    UINT32                                      STUTTER_IGNORE_ICON:1 ; ///<
    UINT32                                       STUTTER_IGNORE_VGA:1 ; ///<
    UINT32                                       STUTTER_IGNORE_FBC:1 ; ///<
    UINT32                                 STUTTER_WM_HIGH_FORCE_ON:1 ; ///<
    UINT32                          STUTTER_WM_HIGH_EXCLUDES_VBLANK:1 ; ///<
    UINT32                       STUTTER_URGENT_IN_NOT_SELF_REFRESH:1 ; ///<
    UINT32                            STUTTER_SELF_REFRESH_FORCE_ON:1 ; ///<
    UINT32                                            Reserved12_15:4 ; ///<
    UINT32                      STUTTER_EXIT_SELF_REFRESH_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx110D4_STRUCT;

// **** GMMx110D8 Register Definition ****
// Address
#define GMMx110D8_ADDRESS                                        0x110d8

// Type
#define GMMx110D8_TYPE                                           TYPE_GMM
// Field Data
#define GMMx110D8_NB_PSTATE_CHANGE_ENABLE_OFFSET                 0
#define GMMx110D8_NB_PSTATE_CHANGE_ENABLE_WIDTH                  1
#define GMMx110D8_NB_PSTATE_CHANGE_ENABLE_MASK                   0x1
#define GMMx110D8_Reserved_3_1_OFFSET                            1
#define GMMx110D8_Reserved_3_1_WIDTH                             3
#define GMMx110D8_Reserved_3_1_MASK                              0xe
#define GMMx110D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_OFFSET  4
#define GMMx110D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_WIDTH   1
#define GMMx110D8_NB_PSTATE_CHANGE_URGENT_DURING_REQUEST_MASK    0x10
#define GMMx110D8_Reserved_7_5_OFFSET                            5
#define GMMx110D8_Reserved_7_5_WIDTH                             3
#define GMMx110D8_Reserved_7_5_MASK                              0xe0
#define GMMx110D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_OFFSET 8
#define GMMx110D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_WIDTH 1
#define GMMx110D8_NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST_MASK 0x100
#define GMMx110D8_NB_PSTATE_CHANGE_FORCE_ON_OFFSET               9
#define GMMx110D8_NB_PSTATE_CHANGE_FORCE_ON_WIDTH                1
#define GMMx110D8_NB_PSTATE_CHANGE_FORCE_ON_MASK                 0x200
#define GMMx110D8_NB_PSTATE_ALLOW_FOR_URGENT_OFFSET              10
#define GMMx110D8_NB_PSTATE_ALLOW_FOR_URGENT_WIDTH               1
#define GMMx110D8_NB_PSTATE_ALLOW_FOR_URGENT_MASK                0x400
#define GMMx110D8_Reserved_15_11_OFFSET                          11
#define GMMx110D8_Reserved_15_11_WIDTH                           5
#define GMMx110D8_Reserved_15_11_MASK                            0xf800
#define GMMx110D8_NB_PSTATE_CHANGE_WATERMARK_OFFSET              16
#define GMMx110D8_NB_PSTATE_CHANGE_WATERMARK_WIDTH               16
#define GMMx110D8_NB_PSTATE_CHANGE_WATERMARK_MASK                0xffff0000

/// GMMx110D8
typedef union {
  struct {                                                              ///<
    UINT32                                  NB_PSTATE_CHANGE_ENABLE:1 ; ///<
    UINT32                                             Reserved_3_1:3 ; ///<
    UINT32                   NB_PSTATE_CHANGE_URGENT_DURING_REQUEST:1 ; ///<
    UINT32                                             Reserved_7_5:3 ; ///<
    UINT32         NB_PSTATE_CHANGE_NOT_SELF_REFRESH_DURING_REQUEST:1 ; ///<
    UINT32                                NB_PSTATE_CHANGE_FORCE_ON:1 ; ///<
    UINT32                               NB_PSTATE_ALLOW_FOR_URGENT:1 ; ///<
    UINT32                                           Reserved_15_11:5 ; ///<
    UINT32                               NB_PSTATE_CHANGE_WATERMARK:16; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx110D8_STRUCT;

// **** GMMx14A18 Register Definition ****
// Address
#define GMMx14A18_ADDRESS                                       0x14a18

// Type
#define GMMx14A18_TYPE                                          TYPE_GMM
// Field Data
#define GMMx14A18_ClkEn_OFFSET                                  0
#define GMMx14A18_ClkEn_WIDTH                                   1
#define GMMx14A18_ClkEn_MASK                                    0x1
#define GMMx14A18_DMAChGroupingDis_OFFSET                       1
#define GMMx14A18_DMAChGroupingDis_WIDTH                        1
#define GMMx14A18_DMAChGroupingDis_MASK                         0x2
#define GMMx14A18_Reserved_OFFSET                               2
#define GMMx14A18_Reserved_WIDTH                                6
#define GMMx14A18_Reserved_MASK                                 0xfc
#define GMMx14A18_EnbRestrictedWrMem_OFFSET                     8
#define GMMx14A18_EnbRestrictedWrMem_WIDTH                      1
#define GMMx14A18_EnbRestrictedWrMem_MASK                       0x100
#define GMMx14A18_EnbRestrictedWrSRBM_OFFSET                    9
#define GMMx14A18_EnbRestrictedWrSRBM_WIDTH                     1
#define GMMx14A18_EnbRestrictedWrSRBM_MASK                      0x200
#define GMMx14A18_JtagEn_OFFSET                                 10
#define GMMx14A18_JtagEn_WIDTH                                  1
#define GMMx14A18_JtagEn_MASK                                   0x400
#define GMMx14A18_Reserved11_31_OFFSET                          11
#define GMMx14A18_Reserved11_31_WIDTH                           21
#define GMMx14A18_Reserved11_31_MASK                            0xfffff800

/// GMMx14A18
typedef union {
  struct {                                                              ///<
    UINT32                                                    ClkEn:1 ; ///<
    UINT32                                         DMAChGroupingDis:1 ; ///<
    UINT32                                                 Reserved:6 ; ///<
    UINT32                                       EnbRestrictedWrMem:1 ; ///<
    UINT32                                      EnbRestrictedWrSRBM:1 ; ///<
    UINT32                                                   JtagEn:1 ; ///<
    UINT32                                            Reserved11_31:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx14A18_STRUCT;

// **** GMMx14A60 Register Definition ****
// Address
#define GMMx14A60_ADDRESS                                       0x14a60

// Type
#define GMMx14A60_TYPE                                          TYPE_GMM
// Field Data
#define GMMx14A60_FSM_ADDR_OFFSET                               0
#define GMMx14A60_FSM_ADDR_WIDTH                                8
#define GMMx14A60_FSM_ADDR_MASK                                 0xff
#define GMMx14A60_Power_Down_OFFSET                             8
#define GMMx14A60_Power_Down_WIDTH                              1
#define GMMx14A60_Power_Down_MASK                               0x100
#define GMMx14A60_Power_Up_OFFSET                               9
#define GMMx14A60_Power_Up_WIDTH                                1
#define GMMx14A60_Power_Up_MASK                                 0x200
#define GMMx14A60_P1_Select_OFFSET                              10
#define GMMx14A60_P1_Select_WIDTH                               1
#define GMMx14A60_P1_Select_MASK                                0x400
#define GMMx14A60_P2_Select_OFFSET                              11
#define GMMx14A60_P2_Select_WIDTH                               1
#define GMMx14A60_P2_Select_MASK                                0x800
#define GMMx14A60_Wr_OFFSET                                     12
#define GMMx14A60_Wr_WIDTH                                      1
#define GMMx14A60_Wr_MASK                                       0x1000
#define GMMx14A60_Rd_OFFSET                                     13
#define GMMx14A60_Rd_WIDTH                                      1
#define GMMx14A60_Rd_MASK                                       0x2000
#define GMMx14A60_Reserved_OFFSET                               14
#define GMMx14A60_Reserved_WIDTH                                13
#define GMMx14A60_Reserved_MASK                                 0x7ffc000
#define GMMx14A60_SRBM_override_OFFSET                          27
#define GMMx14A60_SRBM_override_WIDTH                           1
#define GMMx14A60_SRBM_override_MASK                            0x8000000
#define GMMx14A60_REG_ADDR_OFFSET                               28
#define GMMx14A60_REG_ADDR_WIDTH                                4
#define GMMx14A60_REG_ADDR_MASK                                 0xf0000000

/// GMMx14A60
typedef union {
  struct {                                                              ///<
    UINT32                                                 FSM_ADDR:8 ; ///<
    UINT32                                               Power_Down:1 ; ///<
    UINT32                                                 Power_Up:1 ; ///<
    UINT32                                                P1_Select:1 ; ///<
    UINT32                                                P2_Select:1 ; ///<
    UINT32                                                       Wr:1 ; ///<
    UINT32                                                       Rd:1 ; ///<
    UINT32                                                 Reserved:13; ///<
    UINT32                                            SRBM_override:1 ; ///<
    UINT32                                                 REG_ADDR:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} GMMx14A60_STRUCT;

// **** D0F0xBC_xC0104000 Register Definition ****
// Address
#define D0F0xBC_xC0104000_ADDRESS                               0xc0104000

// Type
#define D0F0xBC_xC0104000_TYPE                                  TYPE_D0F0xBC
// Field Data
#define D0F0xBC_xC0104000_GPU_DIS_OFFSET                       0
#define D0F0xBC_xC0104000_GPU_DIS_WIDTH                        1
#define D0F0xBC_xC0104000_GPU_DIS_MASK                         0x1
#define D0F0xBC_xC0104000_DEBUG_DISABLE_OFFSET                 1
#define D0F0xBC_xC0104000_DEBUG_DISABLE_WIDTH                  1
#define D0F0xBC_xC0104000_DEBUG_DISABLE_MASK                   0x2
#define D0F0xBC_xC0104000_CC_WRITE_DISABLE_OFFSET              2
#define D0F0xBC_xC0104000_CC_WRITE_DISABLE_WIDTH               1
#define D0F0xBC_xC0104000_CC_WRITE_DISABLE_MASK                0x4
#define D0F0xBC_xC0104000_EFUSE_RD_DISABLE_OFFSET              3
#define D0F0xBC_xC0104000_EFUSE_RD_DISABLE_WIDTH               1
#define D0F0xBC_xC0104000_EFUSE_RD_DISABLE_MASK                0x8
#define D0F0xBC_xC0104000_CG_RST_GLB_REQ_DIS_OFFSET            4
#define D0F0xBC_xC0104000_CG_RST_GLB_REQ_DIS_WIDTH             1
#define D0F0xBC_xC0104000_CG_RST_GLB_REQ_DIS_MASK              0x10
#define D0F0xBC_xC0104000_DRV_RST_MODE_OFFSET                  5
#define D0F0xBC_xC0104000_DRV_RST_MODE_WIDTH                   1
#define D0F0xBC_xC0104000_DRV_RST_MODE_MASK                    0x20
#define D0F0xBC_xC0104000_ROM_DIS_OFFSET                       6
#define D0F0xBC_xC0104000_ROM_DIS_WIDTH                        1
#define D0F0xBC_xC0104000_ROM_DIS_MASK                         0x40
#define D0F0xBC_xC0104000_JPC_REP_DISABLE_OFFSET               7
#define D0F0xBC_xC0104000_JPC_REP_DISABLE_WIDTH                1
#define D0F0xBC_xC0104000_JPC_REP_DISABLE_MASK                 0x80
#define D0F0xBC_xC0104000_RCU_BREAK_POINT1_OFFSET              8
#define D0F0xBC_xC0104000_RCU_BREAK_POINT1_WIDTH               1
#define D0F0xBC_xC0104000_RCU_BREAK_POINT1_MASK                0x100
#define D0F0xBC_xC0104000_RCU_BREAK_POINT2_OFFSET              9
#define D0F0xBC_xC0104000_RCU_BREAK_POINT2_WIDTH               1
#define D0F0xBC_xC0104000_RCU_BREAK_POINT2_MASK                0x200
#define D0F0xBC_xC0104000_GPU_ID_WRITE_DIS_OFFSET              10
#define D0F0xBC_xC0104000_GPU_ID_WRITE_DIS_WIDTH               1
#define D0F0xBC_xC0104000_GPU_ID_WRITE_DIS_MASK                0x400
#define D0F0xBC_xC0104000_DC_WRITE_DIS_OFFSET                  11
#define D0F0xBC_xC0104000_DC_WRITE_DIS_WIDTH                   1
#define D0F0xBC_xC0104000_DC_WRITE_DIS_MASK                    0x800
#define D0F0xBC_xC0104000_HDCP_FUSE_DISABLE_OFFSET             12
#define D0F0xBC_xC0104000_HDCP_FUSE_DISABLE_WIDTH              1
#define D0F0xBC_xC0104000_HDCP_FUSE_DISABLE_MASK               0x1000
#define D0F0xBC_xC0104000_PHY_FUSE_VALID_OFFSET                13
#define D0F0xBC_xC0104000_PHY_FUSE_VALID_WIDTH                 1
#define D0F0xBC_xC0104000_PHY_FUSE_VALID_MASK                  0x2000
#define D0F0xBC_xC0104000_SMU_IOC_MST_DISABLE_OFFSET           14
#define D0F0xBC_xC0104000_SMU_IOC_MST_DISABLE_WIDTH            1
#define D0F0xBC_xC0104000_SMU_IOC_MST_DISABLE_MASK             0x4000
#define D0F0xBC_xC0104000_FCH_LOCKOUT_ENABLE_OFFSET            15
#define D0F0xBC_xC0104000_FCH_LOCKOUT_ENABLE_WIDTH             1
#define D0F0xBC_xC0104000_FCH_LOCKOUT_ENABLE_MASK              0x8000
#define D0F0xBC_xC0104000_FCH_XFIRE_FILTER_ENABLE_OFFSET       16
#define D0F0xBC_xC0104000_FCH_XFIRE_FILTER_ENABLE_WIDTH        1
#define D0F0xBC_xC0104000_FCH_XFIRE_FILTER_ENABLE_MASK         0x10000
#define D0F0xBC_xC0104000_XFIRE_DISABLE_OFFSET                 17
#define D0F0xBC_xC0104000_XFIRE_DISABLE_WIDTH                  1
#define D0F0xBC_xC0104000_XFIRE_DISABLE_MASK                   0x20000
#define D0F0xBC_xC0104000_SAMU_FUSE_DISABLE_OFFSET             18
#define D0F0xBC_xC0104000_SAMU_FUSE_DISABLE_WIDTH              1
#define D0F0xBC_xC0104000_SAMU_FUSE_DISABLE_MASK               0x40000
#define D0F0xBC_xC0104000_BIF_RST_POLLING_DISABLE_OFFSET       19
#define D0F0xBC_xC0104000_BIF_RST_POLLING_DISABLE_WIDTH        1
#define D0F0xBC_xC0104000_BIF_RST_POLLING_DISABLE_MASK         0x80000
#define D0F0xBC_xC0104000_REDUNDANCY_WRITE_DIS_OFFSET          20
#define D0F0xBC_xC0104000_REDUNDANCY_WRITE_DIS_WIDTH           1
#define D0F0xBC_xC0104000_REDUNDANCY_WRITE_DIS_MASK            0x100000
#define D0F0xBC_xC0104000_MEM_HARDREP_EN_OFFSET                21
#define D0F0xBC_xC0104000_MEM_HARDREP_EN_WIDTH                 1
#define D0F0xBC_xC0104000_MEM_HARDREP_EN_MASK                  0x200000
#define D0F0xBC_xC0104000_PCIE_INIT_DISABLE_OFFSET             22
#define D0F0xBC_xC0104000_PCIE_INIT_DISABLE_WIDTH              1
#define D0F0xBC_xC0104000_PCIE_INIT_DISABLE_MASK               0x400000
#define D0F0xBC_xC0104000_DSMU_DISABLE_OFFSET                  23
#define D0F0xBC_xC0104000_DSMU_DISABLE_WIDTH                   1
#define D0F0xBC_xC0104000_DSMU_DISABLE_MASK                    0x800000
#define D0F0xBC_xC0104000_RCU_SPARE_6_0_OFFSET                 24
#define D0F0xBC_xC0104000_RCU_SPARE_6_0_WIDTH                  7
#define D0F0xBC_xC0104000_RCU_SPARE_6_0_MASK                   0x7f000000
#define D0F0xBC_xC0104000_IOMMU_V2_DISABLE_OFFSET              31
#define D0F0xBC_xC0104000_IOMMU_V2_DISABLE_WIDTH               1
#define D0F0xBC_xC0104000_IOMMU_V2_DISABLE_MASK                0x80000000

/// D0F0xBC_xC0104000
typedef union {
  struct {                                                              ///<
    UINT32                                                  GPU_DIS:1 ; ///<
    UINT32                                            DEBUG_DISABLE:1 ; ///<
    UINT32                                         CC_WRITE_DISABLE:1 ; ///<
    UINT32                                         EFUSE_RD_DISABLE:1 ; ///<
    UINT32                                       CG_RST_GLB_REQ_DIS:1 ; ///<
    UINT32                                             DRV_RST_MODE:1 ; ///<
    UINT32                                                  ROM_DIS:1 ; ///<
    UINT32                                          JPC_REP_DISABLE:1 ; ///<
    UINT32                                         RCU_BREAK_POINT1:1 ; ///<
    UINT32                                         RCU_BREAK_POINT2:1 ; ///<
    UINT32                                         GPU_ID_WRITE_DIS:1 ; ///<
    UINT32                                             DC_WRITE_DIS:1 ; ///<
    UINT32                                        HDCP_FUSE_DISABLE:1 ; ///<
    UINT32                                           PHY_FUSE_VALID:1 ; ///<
    UINT32                                      SMU_IOC_MST_DISABLE:1 ; ///<
    UINT32                                       FCH_LOCKOUT_ENABLE:1 ; ///<
    UINT32                                  FCH_XFIRE_FILTER_ENABLE:1 ; ///<
    UINT32                                            XFIRE_DISABLE:1 ; ///<
    UINT32                                        SAMU_FUSE_DISABLE:1 ; ///<
    UINT32                                  BIF_RST_POLLING_DISABLE:1 ; ///<
    UINT32                                     REDUNDANCY_WRITE_DIS:1 ; ///<
    UINT32                                           MEM_HARDREP_EN:1 ; ///<
    UINT32                                        PCIE_INIT_DISABLE:1 ; ///<
    UINT32                                             DSMU_DISABLE:1 ; ///<
    UINT32                                            RCU_SPARE_6_0:7 ; ///<
    UINT32                                         IOMMU_V2_DISABLE:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104000_STRUCT;

// **** D0F0xBC_xC0104004 Field Definition ****
// Address
#define D0F0xBC_xC0104004_ADDRESS                               0xC0104004

// Type
#define D0F0xBC_xC0104004_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104004_Reserved0_18_OFFSET                   0
#define D0F0xBC_xC0104004_Reserved0_18_WIDTH                    19
#define D0F0xBC_xC0104004_Reserved0_18_MASK                     0x7ffff
#define D0F0xBC_xC0104004_VddNbVid0_OFFSET                      19
#define D0F0xBC_xC0104004_VddNbVid0_WIDTH                       8
#define D0F0xBC_xC0104004_VddNbVid0_MASK                        0x7f80000
#define D0F0xBC_xC0104004_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC0104004_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC0104004_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC0104004
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_18:19; ///<
    UINT32                                                VddNbVid0:8 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104004_STRUCT;

// **** D0F0xBC_xC0104007 Field Definition ****
// Address
#define D0F0xBC_xC0104007_ADDRESS                               0xC0104007

// Type
#define D0F0xBC_xC0104007_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104007_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0104007_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0104007_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0104007_VddNbVid1_OFFSET                      3
#define D0F0xBC_xC0104007_VddNbVid1_WIDTH                       8
#define D0F0xBC_xC0104007_VddNbVid1_MASK                        0x7f8
#define D0F0xBC_xC0104007_Reserved11_31_OFFSET                  11
#define D0F0xBC_xC0104007_Reserved11_31_WIDTH                   21
#define D0F0xBC_xC0104007_Reserved11_31_MASK                    0xfffff800

/// D0F0xBC_xC0104007
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                VddNbVid1:8 ; ///<
    UINT32                                            Reserved11_31:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104007_STRUCT;

// **** D0F0xBC_xC0104008 Field Definition ****
// Address
#define D0F0xBC_xC0104008_ADDRESS                               0xC0104008

// Type
#define D0F0xBC_xC0104008_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104008_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0104008_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0104008_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0104008_VddNbVid2_OFFSET                      3
#define D0F0xBC_xC0104008_VddNbVid2_WIDTH                       8
#define D0F0xBC_xC0104008_VddNbVid2_MASK                        0x7f8
#define D0F0xBC_xC0104008_VddNbVid3_OFFSET                      11
#define D0F0xBC_xC0104008_VddNbVid3_WIDTH                       8
#define D0F0xBC_xC0104008_VddNbVid3_MASK                        0x7f800
#define D0F0xBC_xC0104008_Reserved19_31_OFFSET                  19
#define D0F0xBC_xC0104008_Reserved19_31_WIDTH                   13
#define D0F0xBC_xC0104008_Reserved19_31_MASK                    0xfff80000

/// D0F0xBC_xC0104008
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                VddNbVid2:8 ; ///<
    UINT32                                                VddNbVid3:8 ; ///<
    UINT32                                            Reserved19_31:13; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104008_STRUCT;

// **** D0F0xBC_xC01040A0 Register Definition ****
// Address
#define D0F0xBC_xC01040A0_ADDRESS                               0xC01040A0
// Type
#define D0F0xBC_xC01040A0_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC01040A0_SOC_SPARE0_39_38_OFFSET               0
#define D0F0xBC_xC01040A0_SOC_SPARE0_39_38_WIDTH                2
#define D0F0xBC_xC01040A0_SOC_SPARE0_39_38_MASK                 0x3
#define D0F0xBC_xC01040A0_SviLoadLineEn_0_0_OFFSET              2
#define D0F0xBC_xC01040A0_SviLoadLineEn_0_0_WIDTH               1
#define D0F0xBC_xC01040A0_SviLoadLineEn_0_0_MASK                0x4
#define D0F0xBC_xC01040A0_SviLoadLineVdd_6_0_OFFSET             3
#define D0F0xBC_xC01040A0_SviLoadLineVdd_6_0_WIDTH              7
#define D0F0xBC_xC01040A0_SviLoadLineVdd_6_0_MASK               0x3F8
#define D0F0xBC_xC01040A0_SviLoadLineVddNb_6_0_OFFSET           10
#define D0F0xBC_xC01040A0_SviLoadLineVddNb_6_0_WIDTH            7
#define D0F0xBC_xC01040A0_SviLoadLineVddNb_6_0_MASK             0x1FC00
#define D0F0xBC_xC01040A0_SviLoadLineTrimVdd_2_0_OFFSET         17
#define D0F0xBC_xC01040A0_SviLoadLineTrimVdd_2_0_WIDTH          3
#define D0F0xBC_xC01040A0_SviLoadLineTrimVdd_2_0_MASK           0xE0000
#define D0F0xBC_xC01040A0_SviLoadLineTrimVddNb_2_0_OFFSET       20
#define D0F0xBC_xC01040A0_SviLoadLineTrimVddNb_2_0_WIDTH        3
#define D0F0xBC_xC01040A0_SviLoadLineTrimVddNb_2_0_MASK         0x700000
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVdd_1_0_OFFSET       23
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVdd_1_0_WIDTH        2
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVdd_1_0_MASK         0x1800000
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVddNb_1_0_OFFSET     25
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVddNb_1_0_WIDTH      2
#define D0F0xBC_xC01040A0_SviLoadLineOffsetVddNb_1_0_MASK       0x6000000
#define D0F0xBC_xC01040A0_Reserved24_31_OFFSET                  27
#define D0F0xBC_xC01040A0_Reserved24_31_WIDTH                   5
#define D0F0xBC_xC01040A0_Reserved24_31_MASK                    0xF8000000

/// D0F0xBC_xC01040A0
typedef union {
  struct {                                                            ///<
    UINT32                                        SOC_SPARE0_39_38:2; ///<
    UINT32                                       SviLoadLineEn_0_0:1; ///<
    UINT32                                      SviLoadLineVdd_6_0:7; ///<
    UINT32                                    SviLoadLineVddNb_6_0:7; ///<
    UINT32                                  SviLoadLineTrimVdd_2_0:3; ///<
    UINT32                                SviLoadLineTrimVddNb_2_0:3; ///<
    UINT32                                SviLoadLineOffsetVdd_1_0:2; ///<
    UINT32                              SviLoadLineOffsetVddNb_1_0:2; ///<
    UINT32                                           Reserved24_31:5; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC01040A0_STRUCT;

// **** D0F0xBC_xC010400C Field Definition ****
// Address
#define D0F0xBC_xC010400C_ADDRESS                               0xC010400c

// Type
#define D0F0xBC_xC010400C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010400C_Reserved0_14_OFFSET                   0
#define D0F0xBC_xC010400C_Reserved0_14_WIDTH                    15
#define D0F0xBC_xC010400C_Reserved0_14_MASK                     0x7fff
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_OFFSET         15
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_WIDTH          6
#define D0F0xBC_xC010400C_MainPllOpFreqIdStartup_MASK           0x1f8000
#define D0F0xBC_xC010400C_Reserved21_31_OFFSET                  21
#define D0F0xBC_xC010400C_Reserved21_31_WIDTH                   11
#define D0F0xBC_xC010400C_Reserved21_31_MASK                    0xffe00000

/// D0F0xBC_xC010400C
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_14:15; ///<
    UINT32                                   MainPllOpFreqIdStartup:6 ; ///<
    UINT32                                            Reserved21_31:11; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010400C_STRUCT;

// **** D0F0xBC_xC0104098 Field Definition ****
// Address
#define D0F0xBC_xC0104098_ADDRESS                               0xC0104098

// Type
#define D0F0xBC_xC0104098_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0104098_Reserved0_17_OFFSET                   0
#define D0F0xBC_xC0104098_Reserved0_17_WIDTH                    18
#define D0F0xBC_xC0104098_Reserved0_17_MASK                     0x3ffff
#define D0F0xBC_xC0104098_VddNbVid4_OFFSET                      18
#define D0F0xBC_xC0104098_VddNbVid4_WIDTH                       8
#define D0F0xBC_xC0104098_VddNbVid4_MASK                        0x3fc0000
#define D0F0xBC_xC0104098_Reserved26_31_OFFSET                  26
#define D0F0xBC_xC0104098_Reserved26_31_WIDTH                   6
#define D0F0xBC_xC0104098_Reserved26_31_MASK                    0xfc000000

/// D0F0xBC_xC0104098
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_17:18; ///<
    UINT32                                                VddNbVid4:8 ; ///<
    UINT32                                            Reserved26_31:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0104098_STRUCT;

// **** D0F0xBC_xC01040A3 Field Definition ****
// Address
#define D0F0xBC_xC01040A3_ADDRESS                               0xC01040A3

// Type
#define D0F0xBC_xC01040A3_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040A3_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040A3_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040A3_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040A3_VCEFlag0_OFFSET                       3
#define D0F0xBC_xC01040A3_VCEFlag0_WIDTH                        8
#define D0F0xBC_xC01040A3_VCEFlag0_MASK                         0x7f8
#define D0F0xBC_xC01040A3_Reserved31_12_OFFSET                  12
#define D0F0xBC_xC01040A3_Reserved31_12_WIDTH                   21
#define D0F0xBC_xC01040A3_Reserved31_12_MASK                    0xffff8000

/// D0F0xBC_xC01040A3
typedef union {
  struct {                                                              ///<
    UINT32                                                 VCEFlag0:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040A3_STRUCT;

// **** D0F0xBC_xC01040A4 Field Definition ****
// Address
#define D0F0xBC_xC01040A4_ADDRESS                               0xC01040a4

// Type
#define D0F0xBC_xC01040A4_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040A4_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040A4_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040A4_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040A4_VCEFlag1_OFFSET                       3
#define D0F0xBC_xC01040A4_VCEFlag1_WIDTH                        8
#define D0F0xBC_xC01040A4_VCEFlag1_MASK                         0x7f8
#define D0F0xBC_xC01040A4_VCEFlag2_OFFSET                       11
#define D0F0xBC_xC01040A4_VCEFlag2_WIDTH                        8
#define D0F0xBC_xC01040A4_VCEFlag2_MASK                         0x7f800
#define D0F0xBC_xC01040A4_VCEFlag3_OFFSET                       19
#define D0F0xBC_xC01040A4_VCEFlag3_WIDTH                        8
#define D0F0xBC_xC01040A4_VCEFlag3_MASK                         0x7f80000
#define D0F0xBC_xC01040A4_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC01040A4_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC01040A4_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC01040A4
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                 VCEFlag1:8 ; ///<
    UINT32                                                 VCEFlag2:8 ; ///<
    UINT32                                                 VCEFlag3:8 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040A4_STRUCT;

// **** D0F0xBC_xC01040A7 Field Definition ****
// Address
#define D0F0xBC_xC01040A7_ADDRESS                               0xC01040a7

// Type
#define D0F0xBC_xC01040A7_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040A7_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040A7_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040A7_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040A7_VCEFlag4_OFFSET                       3
#define D0F0xBC_xC01040A7_VCEFlag4_WIDTH                        8
#define D0F0xBC_xC01040A7_VCEFlag4_MASK                         0x7f8
#define D0F0xBC_xC01040A7_Reserved11_31_OFFSET                  11
#define D0F0xBC_xC01040A7_Reserved11_31_WIDTH                   21
#define D0F0xBC_xC01040A7_Reserved11_31_MASK                    0xfffff800

/// D0F0xBC_xC01040A7
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                 VCEFlag4:8 ; ///<
    UINT32                                            Reserved11_31:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040A7_STRUCT;

// **** D0F0xBC_xC01040A8 Field Definition ****
// Address
#define D0F0xBC_xC01040A8_ADDRESS                               0xC01040a8

// Type
#define D0F0xBC_xC01040A8_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040A8_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040A8_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040A8_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040A8_VCEMclk_OFFSET                        3
#define D0F0xBC_xC01040A8_VCEMclk_WIDTH                         5
#define D0F0xBC_xC01040A8_VCEMclk_MASK                          0xf8
#define D0F0xBC_xC01040A8_Reserved8_31_OFFSET                   8
#define D0F0xBC_xC01040A8_Reserved8_31_WIDTH                    24
#define D0F0xBC_xC01040A8_Reserved8_31_MASK                     0xffffff00

/// D0F0xBC_xC01040A8
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                  VCEMclk:5 ; ///<
    UINT32                                             Reserved8_31:24; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040A8_STRUCT;

// **** D0F0xBC_xC01040C0 Field Definition ****
// Address
#define D0F0xBC_xC01040C0_ADDRESS                               0xC01040c0

// Type
#define D0F0xBC_xC01040C0_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040C0_Reserved0_22_OFFSET                   0
#define D0F0xBC_xC01040C0_Reserved0_22_WIDTH                    23
#define D0F0xBC_xC01040C0_Reserved0_22_MASK                     0x7fffff
#define D0F0xBC_xC01040C0_ReqSclkSel0_OFFSET                    23
#define D0F0xBC_xC01040C0_ReqSclkSel0_WIDTH                     3
#define D0F0xBC_xC01040C0_ReqSclkSel0_MASK                      0x3800000
#define D0F0xBC_xC01040C0_ReqSclkSel1_OFFSET                    26
#define D0F0xBC_xC01040C0_ReqSclkSel1_WIDTH                     3
#define D0F0xBC_xC01040C0_ReqSclkSel1_MASK                      0x1c000000
#define D0F0xBC_xC01040C0_ReqSclkSel2_OFFSET                    29
#define D0F0xBC_xC01040C0_ReqSclkSel2_WIDTH                     3
#define D0F0xBC_xC01040C0_ReqSclkSel2_MASK                      0xe0000000

/// D0F0xBC_xC01040C0
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_22:23; ///<
    UINT32                                              ReqSclkSel0:3 ; ///<
    UINT32                                              ReqSclkSel1:3 ; ///<
    UINT32                                              ReqSclkSel2:3 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040C0_STRUCT;

// **** D0F0xBC_xC01040C4 Field Definition ****
// Address
#define D0F0xBC_xC01040C4_ADDRESS                               0xC01040c4

// Type
#define D0F0xBC_xC01040C4_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040C4_ReqSclkSel3_OFFSET                    0
#define D0F0xBC_xC01040C4_ReqSclkSel3_WIDTH                     3
#define D0F0xBC_xC01040C4_ReqSclkSel3_MASK                      0x7
#define D0F0xBC_xC01040C4_ReqSclkSel4_OFFSET                    3
#define D0F0xBC_xC01040C4_ReqSclkSel4_WIDTH                     3
#define D0F0xBC_xC01040C4_ReqSclkSel4_MASK                      0x38
#define D0F0xBC_xC01040C4_Reserved6_31_OFFSET                   6
#define D0F0xBC_xC01040C4_Reserved6_31_WIDTH                    26
#define D0F0xBC_xC01040C4_Reserved6_31_MASK                     0xffffffc0

/// D0F0xBC_xC01040C4
typedef union {
  struct {                                                              ///<
    UINT32                                              ReqSclkSel3:3 ; ///<
    UINT32                                              ReqSclkSel4:3 ; ///<
    UINT32                                             Reserved6_31:29; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040C4_STRUCT;

// **** D0F0xBC_xC01040DC Register Definition ****
// Address
#define D0F0xBC_xC01040DC_ADDRESS                               0xC01040DC
// Type
#define D0F0xBC_xC01040DC_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC01040DC_SmuCoolingIndex_7_0_OFFSET            0
#define D0F0xBC_xC01040DC_SmuCoolingIndex_7_0_WIDTH             8
#define D0F0xBC_xC01040DC_SmuCoolingIndex_7_0_MASK              0xFF
#define D0F0xBC_xC01040DC_BapmFuseOverride_0_0_OFFSET           8
#define D0F0xBC_xC01040DC_BapmFuseOverride_0_0_WIDTH            1
#define D0F0xBC_xC01040DC_BapmFuseOverride_0_0_MASK             0x100
#define D0F0xBC_xC01040DC_SOC_SPARE3_10_0_OFFSET                9
#define D0F0xBC_xC01040DC_SOC_SPARE3_10_0_WIDTH                 11
#define D0F0xBC_xC01040DC_SOC_SPARE3_10_0_MASK                  0xFFE00
#define D0F0xBC_xC01040DC_BapmLhtcCap_0_0_OFFSET                20
#define D0F0xBC_xC01040DC_BapmLhtcCap_0_0_WIDTH                 1
#define D0F0xBC_xC01040DC_BapmLhtcCap_0_0_MASK                  0x100000
#define D0F0xBC_xC01040DC_SOC_SPARE4_10_0_OFFSET                21
#define D0F0xBC_xC01040DC_SOC_SPARE4_10_0_WIDTH                 11
#define D0F0xBC_xC01040DC_SOC_SPARE4_10_0_MASK                  0xFFE00000

/// D0F0xBC_xC01040DC
typedef union {
  struct {                                                            ///<
    UINT32                                     SmuCoolingIndex_7_0:8; ///<
    UINT32                                    BapmFuseOverride_0_0:1; ///<
    UINT32                                         SOC_SPARE3_10_0:11; ///<
    UINT32                                         BapmLhtcCap_0_0:1; ///<
    UINT32                                         SOC_SPARE4_10_0:11; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC01040DC_STRUCT;

// **** D0F0xBC_xC01040E0 Field Definition ****
// Address
#define D0F0xBC_xC01040E0_ADDRESS                               0xC01040e0

// Type
#define D0F0xBC_xC01040E0_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040E0_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040E0_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040E0_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040E0_AClkDid0_OFFSET                       3
#define D0F0xBC_xC01040E0_AClkDid0_WIDTH                        7
#define D0F0xBC_xC01040E0_AClkDid0_MASK                         0x3f8
#define D0F0xBC_xC01040E0_AClkDid1_OFFSET                       10
#define D0F0xBC_xC01040E0_AClkDid1_WIDTH                        7
#define D0F0xBC_xC01040E0_AClkDid1_MASK                         0x1fc00
#define D0F0xBC_xC01040E0_AClkDid2_OFFSET                       17
#define D0F0xBC_xC01040E0_AClkDid2_WIDTH                        7
#define D0F0xBC_xC01040E0_AClkDid2_MASK                         0xfe0000
#define D0F0xBC_xC01040E0_AClkDid3_OFFSET                       24
#define D0F0xBC_xC01040E0_AClkDid3_WIDTH                        7
#define D0F0xBC_xC01040E0_AClkDid3_MASK                         0x7f000000
#define D0F0xBC_xC01040E0_Reserved31_31_OFFSET                  31
#define D0F0xBC_xC01040E0_Reserved31_31_WIDTH                   1
#define D0F0xBC_xC01040E0_Reserved31_31_MASK                    0x80000000

/// D0F0xBC_xC01040E0
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                 AClkDid0:7 ; ///<
    UINT32                                                 AClkDid1:7 ; ///<
    UINT32                                                 AClkDid2:7 ; ///<
    UINT32                                                 AClkDid3:7 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040E0_STRUCT;

// **** D0F0xBC_xC01040E3 Field Definition ****
// Address
#define D0F0xBC_xC01040E3_ADDRESS                               0xC01040e3

// Type
#define D0F0xBC_xC01040E3_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040E3_Reserved0_6_OFFSET                    0
#define D0F0xBC_xC01040E3_Reserved0_6_WIDTH                     7
#define D0F0xBC_xC01040E3_Reserved0_6_MASK                      0x7f
#define D0F0xBC_xC01040E3_AClkDid4_OFFSET                       7
#define D0F0xBC_xC01040E3_AClkDid4_WIDTH                        7
#define D0F0xBC_xC01040E3_AClkDid4_MASK                         0x3f80
#define D0F0xBC_xC01040E3_Reserved14_31_OFFSET                  14
#define D0F0xBC_xC01040E3_Reserved14_31_WIDTH                   18
#define D0F0xBC_xC01040E3_Reserved14_31_MASK                    0xffffc000

/// D0F0xBC_xC01040E3
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_6:7 ; ///<
    UINT32                                                 AClkDid4:7 ; ///<
    UINT32                                            Reserved14_31:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040E3_STRUCT;

// **** D0F0xBC_xC01040E4 Field Definition ****
// Address
#define D0F0xBC_xC01040E4_ADDRESS                               0xC01040e4

// Type
#define D0F0xBC_xC01040E4_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040E4_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC01040E4_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC01040E4_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC01040E4_SamClkDid0_OFFSET                     6
#define D0F0xBC_xC01040E4_SamClkDid0_WIDTH                      7
#define D0F0xBC_xC01040E4_SamClkDid0_MASK                       0x1fc0
#define D0F0xBC_xC01040E4_SamClkDid1_OFFSET                     13
#define D0F0xBC_xC01040E4_SamClkDid1_WIDTH                      7
#define D0F0xBC_xC01040E4_SamClkDid1_MASK                       0xfe000
#define D0F0xBC_xC01040E4_SamClkDid2_OFFSET                     20
#define D0F0xBC_xC01040E4_SamClkDid2_WIDTH                      7
#define D0F0xBC_xC01040E4_SamClkDid2_MASK                       0x7f00000
#define D0F0xBC_xC01040E4_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC01040E4_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC01040E4_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC01040E4
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                               SamClkDid0:7 ; ///<
    UINT32                                               SamClkDid1:7 ; ///<
    UINT32                                               SamClkDid2:7 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040E4_STRUCT;

// **** D0F0xBC_xC01040E7 Field Definition ****
// Address
#define D0F0xBC_xC01040E7_ADDRESS                               0xC01040e7

// Type
#define D0F0xBC_xC01040E7_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040E7_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC01040E7_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC01040E7_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC01040E7_SamClkDid3_OFFSET                     3
#define D0F0xBC_xC01040E7_SamClkDid3_WIDTH                      7
#define D0F0xBC_xC01040E7_SamClkDid3_MASK                       0x3f8
#define D0F0xBC_xC01040E7_Reserved10_31_OFFSET                  10
#define D0F0xBC_xC01040E7_Reserved10_31_WIDTH                   22
#define D0F0xBC_xC01040E7_Reserved10_31_MASK                    0xfffffc00

/// D0F0xBC_xC01040E7
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                               SamClkDid3:7 ; ///<
    UINT32                                            Reserved10_31:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040E7_STRUCT;

// **** D0F0xBC_xC01040E8 Field Definition ****
// Address
#define D0F0xBC_xC01040E8_ADDRESS                               0xC01040e8

// Type
#define D0F0xBC_xC01040E8_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC01040E8_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC01040E8_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC01040E8_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC01040E8_SamClkDid4_OFFSET                     2
#define D0F0xBC_xC01040E8_SamClkDid4_WIDTH                      7
#define D0F0xBC_xC01040E8_SamClkDid4_MASK                       0x1fc
#define D0F0xBC_xC01040E8_Reserved9_31_OFFSET                   9
#define D0F0xBC_xC01040E8_Reserved9_31_WIDTH                    23
#define D0F0xBC_xC01040E8_Reserved9_31_MASK                     0xfffffe00

/// D0F0xBC_xC01040E8
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                               SamClkDid4:7 ; ///<
    UINT32                                             Reserved9_31:23; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC01040E8_STRUCT;

// **** D0F0xBC_xC0107064 Field Definition ****
// Address
#define D0F0xBC_xC0107064_ADDRESS                               0xC0107064

// Type
#define D0F0xBC_xC0107064_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107064_Reserved0_18_OFFSET                   0
#define D0F0xBC_xC0107064_Reserved0_18_WIDTH                    19
#define D0F0xBC_xC0107064_Reserved0_18_MASK                     0x7ffff
#define D0F0xBC_xC0107064_PowerplayTableRev_OFFSET              19
#define D0F0xBC_xC0107064_PowerplayTableRev_WIDTH               4
#define D0F0xBC_xC0107064_PowerplayTableRev_MASK                0x780000
#define D0F0xBC_xC0107064_SClkThermDid_OFFSET                   23
#define D0F0xBC_xC0107064_SClkThermDid_WIDTH                    7
#define D0F0xBC_xC0107064_SClkThermDid_MASK                     0x3f800000
#define D0F0xBC_xC0107064_Reserved30_31_OFFSET                  30
#define D0F0xBC_xC0107064_Reserved30_31_WIDTH                   2
#define D0F0xBC_xC0107064_Reserved30_31_MASK                    0xc0000000

/// D0F0xBC_xC0107064
typedef union {
  struct {                                                              ///<
    UINT32                                             Reserved0_18:19; ///<
    UINT32                                        PowerplayTableRev:4 ; ///<
    UINT32                                             SClkThermDid:7 ; ///<
    UINT32                                            Reserved30_31:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107064_STRUCT;

// **** D0F0xBC_xC0107067 Field Definition ****
// Address
#define D0F0xBC_xC0107067_ADDRESS                               0xC0107067

// Type
#define D0F0xBC_xC0107067_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107067_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC0107067_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC0107067_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC0107067_PcieGen2Vid_OFFSET                    6
#define D0F0xBC_xC0107067_PcieGen2Vid_WIDTH                     3
#define D0F0xBC_xC0107067_PcieGen2Vid_MASK                      0x1c0
#define D0F0xBC_xC0107067_PcieGen3Vid_OFFSET                    9
#define D0F0xBC_xC0107067_PcieGen3Vid_WIDTH                     3
#define D0F0xBC_xC0107067_PcieGen3Vid_MASK                      0xe00
#define D0F0xBC_xC0107067_Reserved12_31_OFFSET                  12
#define D0F0xBC_xC0107067_Reserved12_31_WIDTH                   20
#define D0F0xBC_xC0107067_Reserved12_31_MASK                    0xfffff000

/// D0F0xBC_xC0107067
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                              PcieGen2Vid:3 ; ///<
    UINT32                                              PcieGen3Vid:3 ; ///<
    UINT32                                            Reserved12_31:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107067_STRUCT;

// **** D0F0xBC_xC0107068 Field Definition ****
// Address
#define D0F0xBC_xC0107068_ADDRESS                               0xC0107068

// Type
#define D0F0xBC_xC0107068_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107068_Reserved0_3_OFFSET                    0
#define D0F0xBC_xC0107068_Reserved0_3_WIDTH                     4
#define D0F0xBC_xC0107068_Reserved0_3_MASK                      0xf
#define D0F0xBC_xC0107068_SClkDid0_OFFSET                       4
#define D0F0xBC_xC0107068_SClkDid0_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid0_MASK                         0x7f0
#define D0F0xBC_xC0107068_SClkDid1_OFFSET                       11
#define D0F0xBC_xC0107068_SClkDid1_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid1_MASK                         0x3f800
#define D0F0xBC_xC0107068_SClkDid2_OFFSET                       18
#define D0F0xBC_xC0107068_SClkDid2_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid2_MASK                         0x1fc0000
#define D0F0xBC_xC0107068_SClkDid3_OFFSET                       25
#define D0F0xBC_xC0107068_SClkDid3_WIDTH                        7
#define D0F0xBC_xC0107068_SClkDid3_MASK                         0xfe000000

/// D0F0xBC_xC0107068
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_3:4 ; ///<
    UINT32                                                 SClkDid0:7 ; ///<
    UINT32                                                 SClkDid1:7 ; ///<
    UINT32                                                 SClkDid2:7 ; ///<
    UINT32                                                 SClkDid3:7 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107068_STRUCT;

// **** D0F0xBC_xC010706C Field Definition ****
// Address
#define D0F0xBC_xC010706C_ADDRESS                               0xC010706c

// Type
#define D0F0xBC_xC010706C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010706C_SClkDid4_OFFSET                       0
#define D0F0xBC_xC010706C_SClkDid4_WIDTH                        7
#define D0F0xBC_xC010706C_SClkDid4_MASK                         0x7f
#define D0F0xBC_xC010706C_DispClkDid0_OFFSET                    7
#define D0F0xBC_xC010706C_DispClkDid0_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid0_MASK                      0x3f80
#define D0F0xBC_xC010706C_DispClkDid1_OFFSET                    14
#define D0F0xBC_xC010706C_DispClkDid1_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid1_MASK                      0x1fc000
#define D0F0xBC_xC010706C_DispClkDid2_OFFSET                    21
#define D0F0xBC_xC010706C_DispClkDid2_WIDTH                     7
#define D0F0xBC_xC010706C_DispClkDid2_MASK                      0xfe00000
#define D0F0xBC_xC010706C_Reserved28_31_OFFSET                  28
#define D0F0xBC_xC010706C_Reserved28_31_WIDTH                   4
#define D0F0xBC_xC010706C_Reserved28_31_MASK                    0xf0000000

/// D0F0xBC_xC010706C
typedef union {
  struct {                                                              ///<
    UINT32                                                 SClkDid4:7 ; ///<
    UINT32                                              DispClkDid0:7 ; ///<
    UINT32                                              DispClkDid1:7 ; ///<
    UINT32                                              DispClkDid2:7 ; ///<
    UINT32                                            Reserved28_31:4 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010706C_STRUCT;

// **** D0F0xBC_xC010706F Field Definition ****
// Address
#define D0F0xBC_xC010706F_ADDRESS                               0xC010706f

// Type
#define D0F0xBC_xC010706F_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010706F_Reserved0_3_OFFSET                    0
#define D0F0xBC_xC010706F_Reserved0_3_WIDTH                     4
#define D0F0xBC_xC010706F_Reserved0_3_MASK                      0xf
#define D0F0xBC_xC010706F_DispClkDid3_OFFSET                    4
#define D0F0xBC_xC010706F_DispClkDid3_WIDTH                     7
#define D0F0xBC_xC010706F_DispClkDid3_MASK                      0x7f0
#define D0F0xBC_xC010706F_Reserved11_31_OFFSET                  11
#define D0F0xBC_xC010706F_Reserved11_31_WIDTH                   21
#define D0F0xBC_xC010706F_Reserved11_31_MASK                    0xfffff800

/// D0F0xBC_xC010706F
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_3:4 ; ///<
    UINT32                                              DispClkDid3:7 ; ///<
    UINT32                                            Reserved11_31:21; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010706F_STRUCT;

// **** D0F0xBC_xC0107070 Field Definition ****
// Address
#define D0F0xBC_xC0107070_ADDRESS                               0xC0107070

// Type
#define D0F0xBC_xC0107070_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107070_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0107070_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0107070_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0107070_DispClkDid4_OFFSET                    3
#define D0F0xBC_xC0107070_DispClkDid4_WIDTH                     7
#define D0F0xBC_xC0107070_DispClkDid4_MASK                      0x3f8
#define D0F0xBC_xC0107070_LClkDpmDid0_OFFSET                    10
#define D0F0xBC_xC0107070_LClkDpmDid0_WIDTH                     7
#define D0F0xBC_xC0107070_LClkDpmDid0_MASK                      0x1fc00
#define D0F0xBC_xC0107070_LClkDpmDid1_OFFSET                    17
#define D0F0xBC_xC0107070_LClkDpmDid1_WIDTH                     7
#define D0F0xBC_xC0107070_LClkDpmDid1_MASK                      0xfe0000
#define D0F0xBC_xC0107070_LClkDpmDid2_OFFSET                    24
#define D0F0xBC_xC0107070_LClkDpmDid2_WIDTH                     7
#define D0F0xBC_xC0107070_LClkDpmDid2_MASK                      0x7f000000
#define D0F0xBC_xC0107070_Reserved31_31_OFFSET                  31
#define D0F0xBC_xC0107070_Reserved31_31_WIDTH                   1
#define D0F0xBC_xC0107070_Reserved31_31_MASK                    0x80000000

/// D0F0xBC_xC0107070
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                              DispClkDid4:7 ; ///<
    UINT32                                              LClkDpmDid0:7 ; ///<
    UINT32                                              LClkDpmDid1:7 ; ///<
    UINT32                                              LClkDpmDid2:7 ; ///<
    UINT32                                            Reserved31_31:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107070_STRUCT;

// **** D0F0xBC_xC0107073 Field Definition ****
// Address
#define D0F0xBC_xC0107073_ADDRESS                               0xC0107073

// Type
#define D0F0xBC_xC0107073_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107073_Reserved0_6_OFFSET                    0
#define D0F0xBC_xC0107073_Reserved0_6_WIDTH                     7
#define D0F0xBC_xC0107073_Reserved0_6_MASK                      0x7f
#define D0F0xBC_xC0107073_LClkDpmDid3_OFFSET                    7
#define D0F0xBC_xC0107073_LClkDpmDid3_WIDTH                     7
#define D0F0xBC_xC0107073_LClkDpmDid3_MASK                      0x3f80
#define D0F0xBC_xC0107073_Reserved14_31_OFFSET                  14
#define D0F0xBC_xC0107073_Reserved14_31_WIDTH                   18
#define D0F0xBC_xC0107073_Reserved14_31_MASK                    0xffffc000

/// D0F0xBC_xC0107073
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_6:7 ; ///<
    UINT32                                              LClkDpmDid3:7 ; ///<
    UINT32                                            Reserved14_31:18; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107073_STRUCT;

// **** D0F0xBC_xC0107074 Field Definition ****
// Address
#define D0F0xBC_xC0107074_ADDRESS                               0xC0107074

// Type
#define D0F0xBC_xC0107074_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107074_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC0107074_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC0107074_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC0107074_LClkDpmDid4_OFFSET                    6
#define D0F0xBC_xC0107074_LClkDpmDid4_WIDTH                     7
#define D0F0xBC_xC0107074_LClkDpmDid4_MASK                      0x1fc0
#define D0F0xBC_xC0107074_DClkDid0_OFFSET                       13
#define D0F0xBC_xC0107074_DClkDid0_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid0_MASK                         0xfe000
#define D0F0xBC_xC0107074_DClkDid1_OFFSET                       20
#define D0F0xBC_xC0107074_DClkDid1_WIDTH                        7
#define D0F0xBC_xC0107074_DClkDid1_MASK                         0x7f00000
#define D0F0xBC_xC0107074_Reserved27_31_OFFSET                  27
#define D0F0xBC_xC0107074_Reserved27_31_WIDTH                   5
#define D0F0xBC_xC0107074_Reserved27_31_MASK                    0xf8000000

/// D0F0xBC_xC0107074
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                              LClkDpmDid4:7 ; ///<
    UINT32                                                 DClkDid0:7 ; ///<
    UINT32                                                 DClkDid1:7 ; ///<
    UINT32                                            Reserved27_31:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107074_STRUCT;

// **** D0F0xBC_xC0107077 Field Definition ****
// Address
#define D0F0xBC_xC0107077_ADDRESS                               0xC0107077

// Type
#define D0F0xBC_xC0107077_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107077_Reserved0_2_OFFSET                    0
#define D0F0xBC_xC0107077_Reserved0_2_WIDTH                     3
#define D0F0xBC_xC0107077_Reserved0_2_MASK                      0x7
#define D0F0xBC_xC0107077_DClkDid2_OFFSET                       3
#define D0F0xBC_xC0107077_DClkDid2_WIDTH                        7
#define D0F0xBC_xC0107077_DClkDid2_MASK                         0x3f8
#define D0F0xBC_xC0107077_Reserved10_31_OFFSET                  10
#define D0F0xBC_xC0107077_Reserved10_31_WIDTH                   22
#define D0F0xBC_xC0107077_Reserved10_31_MASK                    0xfffffc00

/// D0F0xBC_xC0107077
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_2:3 ; ///<
    UINT32                                                 DClkDid2:7 ; ///<
    UINT32                                            Reserved10_31:22; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107077_STRUCT;

// **** D0F0xBC_xC0107078 Field Definition ****
// Address
#define D0F0xBC_xC0107078_ADDRESS                               0xC0107078

// Type
#define D0F0xBC_xC0107078_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107078_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC0107078_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC0107078_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC0107078_DClkDid3_OFFSET                       2
#define D0F0xBC_xC0107078_DClkDid3_WIDTH                        7
#define D0F0xBC_xC0107078_DClkDid3_MASK                         0x1fc
#define D0F0xBC_xC0107078_DClkDid4_OFFSET                       9
#define D0F0xBC_xC0107078_DClkDid4_WIDTH                        7
#define D0F0xBC_xC0107078_DClkDid4_MASK                         0xfe00
#define D0F0xBC_xC0107078_VClkDid0_OFFSET                       16
#define D0F0xBC_xC0107078_VClkDid0_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid0_MASK                         0x7f0000
#define D0F0xBC_xC0107078_VClkDid1_OFFSET                       23
#define D0F0xBC_xC0107078_VClkDid1_WIDTH                        7
#define D0F0xBC_xC0107078_VClkDid1_MASK                         0x3f800000
#define D0F0xBC_xC0107078_Reserved30_31_OFFSET                  30
#define D0F0xBC_xC0107078_Reserved30_31_WIDTH                   2
#define D0F0xBC_xC0107078_Reserved30_31_MASK                    0xc0000000

/// D0F0xBC_xC0107078
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                                 DClkDid3:7 ; ///<
    UINT32                                                 DClkDid4:7 ; ///<
    UINT32                                                 VClkDid0:7 ; ///<
    UINT32                                                 VClkDid1:7 ; ///<
    UINT32                                            Reserved30_31:2 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107078_STRUCT;

// **** D0F0xBC_xC010707B Field Definition ****
// Address
#define D0F0xBC_xC010707B_ADDRESS                               0xC010707b

// Type
#define D0F0xBC_xC010707B_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010707B_Reserved0_5_OFFSET                    0
#define D0F0xBC_xC010707B_Reserved0_5_WIDTH                     6
#define D0F0xBC_xC010707B_Reserved0_5_MASK                      0x3f
#define D0F0xBC_xC010707B_VClkDid2_OFFSET                       6
#define D0F0xBC_xC010707B_VClkDid2_WIDTH                        7
#define D0F0xBC_xC010707B_VClkDid2_MASK                         0x1fc0
#define D0F0xBC_xC010707B_Reserved13_31_OFFSET                  13
#define D0F0xBC_xC010707B_Reserved13_31_WIDTH                   19
#define D0F0xBC_xC010707B_Reserved13_31_MASK                    0xffffe000

/// D0F0xBC_xC010707B
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_5:6 ; ///<
    UINT32                                                 VClkDid2:7 ; ///<
    UINT32                                            Reserved13_31:19; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010707B_STRUCT;

// **** D0F0xBC_xC010707C Field Definition ****
// Address
#define D0F0xBC_xC010707C_ADDRESS                               0xC010707c

// Type
#define D0F0xBC_xC010707C_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010707C_Reserved0_4_OFFSET                    0
#define D0F0xBC_xC010707C_Reserved0_4_WIDTH                     5
#define D0F0xBC_xC010707C_Reserved0_4_MASK                      0x1f
#define D0F0xBC_xC010707C_VClkDid3_OFFSET                       5
#define D0F0xBC_xC010707C_VClkDid3_WIDTH                        7
#define D0F0xBC_xC010707C_VClkDid3_MASK                         0xfe0
#define D0F0xBC_xC010707C_VClkDid4_OFFSET                       12
#define D0F0xBC_xC010707C_VClkDid4_WIDTH                        7
#define D0F0xBC_xC010707C_VClkDid4_MASK                         0x7f000
#define D0F0xBC_xC010707C_EClkDid0_OFFSET                       19
#define D0F0xBC_xC010707C_EClkDid0_WIDTH                        7
#define D0F0xBC_xC010707C_EClkDid0_MASK                         0x3f80000
#define D0F0xBC_xC010707C_Reserved26_31_OFFSET                  26
#define D0F0xBC_xC010707C_Reserved26_31_WIDTH                   6
#define D0F0xBC_xC010707C_Reserved26_31_MASK                    0xfc000000

/// D0F0xBC_xC010707C
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_4:5 ; ///<
    UINT32                                                 VClkDid3:7 ; ///<
    UINT32                                                 VClkDid4:7 ; ///<
    UINT32                                                 EClkDid0:7 ; ///<
    UINT32                                            Reserved26_31:6 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010707C_STRUCT;

// **** D0F0xBC_xC010707F Field Definition ****
// Address
#define D0F0xBC_xC010707F_ADDRESS                               0xC010707f

// Type
#define D0F0xBC_xC010707F_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC010707F_Reserved0_1_OFFSET                    0
#define D0F0xBC_xC010707F_Reserved0_1_WIDTH                     2
#define D0F0xBC_xC010707F_Reserved0_1_MASK                      0x3
#define D0F0xBC_xC010707F_EClkDid1_OFFSET                       2
#define D0F0xBC_xC010707F_EClkDid1_WIDTH                        7
#define D0F0xBC_xC010707F_EClkDid1_MASK                         0x1fc
#define D0F0xBC_xC010707F_Reserved9_31_OFFSET                   9
#define D0F0xBC_xC010707F_Reserved9_31_WIDTH                    23
#define D0F0xBC_xC010707F_Reserved9_31_MASK                     0xfffffe00

/// D0F0xBC_xC010707F
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_1:2 ; ///<
    UINT32                                                 EClkDid1:7 ; ///<
    UINT32                                             Reserved9_31:23; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC010707F_STRUCT;

// **** D0F0xBC_xC0107080 Field Definition ****
// Address
#define D0F0xBC_xC0107080_ADDRESS                               0xC0107080

// Type
#define D0F0xBC_xC0107080_TYPE                                  TYPE_D0F0xBC
#define D0F0xBC_xC0107080_Reserved0_0_OFFSET                    0
#define D0F0xBC_xC0107080_Reserved0_0_WIDTH                     1
#define D0F0xBC_xC0107080_Reserved0_0_MASK                      0x1
#define D0F0xBC_xC0107080_EClkDid2_OFFSET                       1
#define D0F0xBC_xC0107080_EClkDid2_WIDTH                        7
#define D0F0xBC_xC0107080_EClkDid2_MASK                         0xfe
#define D0F0xBC_xC0107080_EClkDid3_OFFSET                       8
#define D0F0xBC_xC0107080_EClkDid3_WIDTH                        7
#define D0F0xBC_xC0107080_EClkDid3_MASK                         0x7f00
#define D0F0xBC_xC0107080_EClkDid4_OFFSET                       15
#define D0F0xBC_xC0107080_EClkDid4_WIDTH                        7
#define D0F0xBC_xC0107080_EClkDid4_MASK                         0x3f8000
#define D0F0xBC_xC0107080_PowerplayPerfFlag_OFFSET              22
#define D0F0xBC_xC0107080_PowerplayPerfFlag_WIDTH               5
#define D0F0xBC_xC0107080_PowerplayPerfFlag_MASK                0x7c00000
#define D0F0xBC_xC0107080_PowerplayPSFlag_OFFSET                27
#define D0F0xBC_xC0107080_PowerplayPSFlag_WIDTH                 5
#define D0F0xBC_xC0107080_PowerplayPSFlag_MASK                  0xf8000000

/// D0F0xBC_xC0107080
typedef union {
  struct {                                                              ///<
    UINT32                                              Reserved0_0:1 ; ///<
    UINT32                                                 EClkDid2:7 ; ///<
    UINT32                                                 EClkDid3:7 ; ///<
    UINT32                                                 EClkDid4:7 ; ///<
    UINT32                                        PowerplayPerfFlag:5 ; ///<
    UINT32                                          PowerplayPSFlag:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D0F0xBC_xC0107080_STRUCT;

// **** D0F0xBC_xC0107084 Register Definition ****
// Address
#define D0F0xBC_xC0107084_ADDRESS                               0xC0107084
// Type
#define D0F0xBC_xC0107084_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageHiSidd_15_0_OFFSET 0
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageHiSidd_15_0_WIDTH 16
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageHiSidd_15_0_MASK  0xFFFF
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageLoSidd_15_0_OFFSET 16
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageLoSidd_15_0_WIDTH 16
#define D0F0xBC_xC0107084_BapmVddNbBaseLeakageLoSidd_15_0_MASK  0xFFFF0000

/// D0F0xBC_xC0107084
typedef union {
  struct {                                                            ///<
    UINT32                         BapmVddNbBaseLeakageHiSidd_15_0:16; ///<
    UINT32                         BapmVddNbBaseLeakageLoSidd_15_0:16; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0107084_STRUCT;

// **** D0F0xBC_xC01070D0 Register Definition ****
// Address
#define D0F0xBC_xC01070D0_ADDRESS                               0xC01070D0
// Type
#define D0F0xBC_xC01070D0_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC01070D0_Reserved_24_0_OFFSET 0
#define D0F0xBC_xC01070D0_Reserved_24_0_WIDTH 25
#define D0F0xBC_xC01070D0_Reserved_24_0_MASK 0x1FFFFFF
#define D0F0xBC_xC01070D0_StretchEn_OFFSET 25
#define D0F0xBC_xC01070D0_StretchEn_WIDTH 1
#define D0F0xBC_xC01070D0_StretchEn_MASK 0x2000000
#define D0F0xBC_xC01070D0_Reserved_31_26_OFFSET 26
#define D0F0xBC_xC01070D0_Reserved_31_26_WIDTH 6
#define D0F0xBC_xC01070D0_Reserved_31_26_MASK 0xFC000000

/// D0F0xBC_xC01070D0
typedef union {
  struct {                                            ///<
    UINT32                         Reserved_24_0:25;  ///<
    UINT32                              StretchEn:1;  ///<
    UINT32                         Reserved_31_26:6;  ///<
  } Field;

  UINT32 Value;
} D0F0xBC_xC01070D0_STRUCT;

// **** D0F0xBC_xC2100000 Register Definition ****
// Address
#define D0F0xBC_xC2100000_ADDRESS                               0xC2100000
// Type
#define D0F0xBC_xC2100000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC2100000_IntToggle_OFFSET                      0
#define D0F0xBC_xC2100000_IntToggle_WIDTH                       1
#define D0F0xBC_xC2100000_IntToggle_MASK                        0x1
#define D0F0xBC_xC2100000_ServiceIndex_OFFSET                   1
#define D0F0xBC_xC2100000_ServiceIndex_WIDTH                    16
#define D0F0xBC_xC2100000_ServiceIndex_MASK                     0x1FFFE
#define D0F0xBC_xC2100000_Reserved_31_17_OFFSET                 17
#define D0F0xBC_xC2100000_Reserved_31_17_WIDTH                  15
#define D0F0xBC_xC2100000_Reserved_31_17_MASK                   0xFFFE0000

/// D0F0xBC_xC2100000
typedef union {
  struct {                                                            ///<
    UINT32                                               IntToggle:1; ///<
    UINT32                                            ServiceIndex:16; ///<
    UINT32                                          Reserved_31_17:15; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC2100000_STRUCT;

// **** D0F0xBC_xC2100004 Register Definition ****
// Address
#define D0F0xBC_xC2100004_ADDRESS                               0xC2100004
// Type
#define D0F0xBC_xC2100004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC2100004_IntAck_OFFSET                         0
#define D0F0xBC_xC2100004_IntAck_WIDTH                          1
#define D0F0xBC_xC2100004_IntAck_MASK                           0x1
#define D0F0xBC_xC2100004_IntDone_OFFSET                        1
#define D0F0xBC_xC2100004_IntDone_WIDTH                         1
#define D0F0xBC_xC2100004_IntDone_MASK                          0x2
#define D0F0xBC_xC2100004_Reserved_31_2_OFFSET                  2
#define D0F0xBC_xC2100004_Reserved_31_2_WIDTH                   30
#define D0F0xBC_xC2100004_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xBC_xC2100004
typedef union {
  struct {                                                            ///<
    UINT32                                                  IntAck:1; ///<
    UINT32                                                 IntDone:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC2100004_STRUCT;

// **** D0F0xBC_xE0003088 Register Definition ****
// Address
#define D0F0xBC_xE0003088_ADDRESS                               0xE0003088
// Type
#define D0F0xBC_xE0003088_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_OFFSET                  0
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_WIDTH                   1
#define D0F0xBC_xE0003088_SMU_AUTH_DONE_MASK                    0x1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_OFFSET                  1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_WIDTH                   1
#define D0F0xBC_xE0003088_SMU_AUTH_PASS_MASK                    0x2
#define D0F0xBC_xE0003088_Reserved_31_2_OFFSET                  2
#define D0F0xBC_xE0003088_Reserved_31_2_WIDTH                   30
#define D0F0xBC_xE0003088_Reserved_31_2_MASK                    0xFFFFFFFC

/// D0F0xBC_xE0003088
typedef union {
  struct {                                                            ///<
    UINT32                                           SMU_AUTH_DONE:1; ///<
    UINT32                                           SMU_AUTH_PASS:1; ///<
    UINT32                                           Reserved_31_2:30; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xE0003088_STRUCT;

// **** D0F0xBC_xE00030A4 Register Definition ****
// Address
#define D0F0xBC_xE00030A4_ADDRESS                               0xE00030A4
// Type
#define D0F0xBC_xE00030A4_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_OFFSET               0
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_WIDTH                1
#define D0F0xBC_xE00030A4_SMU_AUTH_IN_PROG_MASK                 0x1
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_OFFSET                1
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_WIDTH                 2
#define D0F0xBC_xE00030A4_SMU_KEY_RD_DONE_MASK                  0x6
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_OFFSET           3
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_WIDTH            1
#define D0F0xBC_xE00030A4_SMU_SRAM_RD_BLOCK_EN_MASK             0x8
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_OFFSET           4
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_WIDTH            1
#define D0F0xBC_xE00030A4_SMU_SRAM_WR_BLOCK_EN_MASK             0x10
#define D0F0xBC_xE00030A4_Reserved_7_5_OFFSET                   5
#define D0F0xBC_xE00030A4_Reserved_7_5_WIDTH                    3
#define D0F0xBC_xE00030A4_Reserved_7_5_MASK                     0xE0
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_OFFSET               8
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_WIDTH                4
#define D0F0xBC_xE00030A4_SMU_AUTH_counter_MASK                 0xF00
#define D0F0xBC_xE00030A4_Reserved_15_12_OFFSET                 12
#define D0F0xBC_xE00030A4_Reserved_15_12_WIDTH                  4
#define D0F0xBC_xE00030A4_Reserved_15_12_MASK                   0xF000
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_OFFSET             16
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_WIDTH              1
#define D0F0xBC_xE00030A4_SMU_PROTECTED_MODE_MASK               0x10000
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_OFFSET                    17
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_WIDTH                     1
#define D0F0xBC_xE00030A4_SMU_KEY_SEL_MASK                      0x20000
#define D0F0xBC_xE00030A4_Reserved_31_18_OFFSET                 18
#define D0F0xBC_xE00030A4_Reserved_31_18_WIDTH                  14
#define D0F0xBC_xE00030A4_Reserved_31_18_MASK                   0xFFFC0000

/// D0F0xBC_xE00030A4
typedef union {
  struct {                                                            ///<
    UINT32                                        SMU_AUTH_IN_PROG:1; ///<
    UINT32                                         SMU_KEY_RD_DONE:2; ///<
    UINT32                                    SMU_SRAM_RD_BLOCK_EN:1; ///<
    UINT32                                    SMU_SRAM_WR_BLOCK_EN:1; ///<
    UINT32                                            Reserved_7_5:3; ///<
    UINT32                                        SMU_AUTH_counter:4; ///<
    UINT32                                          Reserved_15_12:4; ///<
    UINT32                                      SMU_PROTECTED_MODE:1; ///<
    UINT32                                             SMU_KEY_SEL:1; ///<
    UINT32                                          Reserved_31_18:14; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xE00030A4_STRUCT;

// **** D0F0xBC_xC0000004 Register Definition ****
// Address
#define D0F0xBC_xC0000004_ADDRESS                               0xC0000004
// Type
#define D0F0xBC_xC0000004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_xC0000004_RCU_TST_jpc_rep_req_OFFSET            0
#define D0F0xBC_xC0000004_RCU_TST_jpc_rep_req_WIDTH             1
#define D0F0xBC_xC0000004_RCU_TST_jpc_rep_req_MASK              0x1
#define D0F0xBC_xC0000004_TST_RCU_jpc_rep_done_OFFSET           1
#define D0F0xBC_xC0000004_TST_RCU_jpc_rep_done_WIDTH            1
#define D0F0xBC_xC0000004_TST_RCU_jpc_rep_done_MASK             0x2
#define D0F0xBC_xC0000004_drv_rst_mode_OFFSET                   2
#define D0F0xBC_xC0000004_drv_rst_mode_WIDTH                    1
#define D0F0xBC_xC0000004_drv_rst_mode_MASK                     0x4
#define D0F0xBC_xC0000004_SMU_DC_efuse_status_invalid_OFFSET    3
#define D0F0xBC_xC0000004_SMU_DC_efuse_status_invalid_WIDTH     1
#define D0F0xBC_xC0000004_SMU_DC_efuse_status_invalid_MASK      0x8
#define D0F0xBC_xC0000004_Reserved_5_4_OFFSET                   4
#define D0F0xBC_xC0000004_Reserved_5_4_WIDTH                    2
#define D0F0xBC_xC0000004_Reserved_5_4_MASK                     0x30
#define D0F0xBC_xC0000004_TP_Tester_OFFSET                      6
#define D0F0xBC_xC0000004_TP_Tester_WIDTH                       1
#define D0F0xBC_xC0000004_TP_Tester_MASK                        0x40
#define D0F0xBC_xC0000004_boot_seq_done_OFFSET                  7
#define D0F0xBC_xC0000004_boot_seq_done_WIDTH                   1
#define D0F0xBC_xC0000004_boot_seq_done_MASK                    0x80
#define D0F0xBC_xC0000004_sclk_deep_sleep_exit_OFFSET           8
#define D0F0xBC_xC0000004_sclk_deep_sleep_exit_WIDTH            1
#define D0F0xBC_xC0000004_sclk_deep_sleep_exit_MASK             0x100
#define D0F0xBC_xC0000004_BREAK_PT1_ACTIVE_OFFSET               9
#define D0F0xBC_xC0000004_BREAK_PT1_ACTIVE_WIDTH                1
#define D0F0xBC_xC0000004_BREAK_PT1_ACTIVE_MASK                 0x200
#define D0F0xBC_xC0000004_BREAK_PT2_ACTIVE_OFFSET               10
#define D0F0xBC_xC0000004_BREAK_PT2_ACTIVE_WIDTH                1
#define D0F0xBC_xC0000004_BREAK_PT2_ACTIVE_MASK                 0x400
#define D0F0xBC_xC0000004_FCH_HALT_OFFSET                       11
#define D0F0xBC_xC0000004_FCH_HALT_WIDTH                        1
#define D0F0xBC_xC0000004_FCH_HALT_MASK                         0x800
#define D0F0xBC_xC0000004_FCH_LOCKDOWN_WRITE_DIS_OFFSET         12
#define D0F0xBC_xC0000004_FCH_LOCKDOWN_WRITE_DIS_WIDTH          1
#define D0F0xBC_xC0000004_FCH_LOCKDOWN_WRITE_DIS_MASK           0x1000
#define D0F0xBC_xC0000004_RCU_GIO_fch_lockdown_OFFSET           13
#define D0F0xBC_xC0000004_RCU_GIO_fch_lockdown_WIDTH            1
#define D0F0xBC_xC0000004_RCU_GIO_fch_lockdown_MASK             0x2000
#define D0F0xBC_xC0000004_Reserved_15_14_OFFSET                 14
#define D0F0xBC_xC0000004_Reserved_15_14_WIDTH                  2
#define D0F0xBC_xC0000004_Reserved_15_14_MASK                   0xC000
#define D0F0xBC_xC0000004_INTERRUPTS_ENABLED_OFFSET             16
#define D0F0xBC_xC0000004_INTERRUPTS_ENABLED_WIDTH              1
#define D0F0xBC_xC0000004_INTERRUPTS_ENABLED_MASK               0x10000
#define D0F0xBC_xC0000004_RCU_DtmCnt0_Done_OFFSET               17
#define D0F0xBC_xC0000004_RCU_DtmCnt0_Done_WIDTH                1
#define D0F0xBC_xC0000004_RCU_DtmCnt0_Done_MASK                 0x20000
#define D0F0xBC_xC0000004_RCU_DtmCnt1_Done_OFFSET               18
#define D0F0xBC_xC0000004_RCU_DtmCnt1_Done_WIDTH                1
#define D0F0xBC_xC0000004_RCU_DtmCnt1_Done_MASK                 0x40000
#define D0F0xBC_xC0000004_RCU_DtmCnt2_Done_OFFSET               19
#define D0F0xBC_xC0000004_RCU_DtmCnt2_Done_WIDTH                1
#define D0F0xBC_xC0000004_RCU_DtmCnt2_Done_MASK                 0x80000
#define D0F0xBC_xC0000004_Reserved_23_20_OFFSET                 20
#define D0F0xBC_xC0000004_Reserved_23_20_WIDTH                  4
#define D0F0xBC_xC0000004_Reserved_23_20_MASK                   0xF00000
#define D0F0xBC_xC0000004_lm32_irq31_sel_OFFSET                 24
#define D0F0xBC_xC0000004_lm32_irq31_sel_WIDTH                  2
#define D0F0xBC_xC0000004_lm32_irq31_sel_MASK                   0x3000000
#define D0F0xBC_xC0000004_Reserved_31_26_OFFSET                 26
#define D0F0xBC_xC0000004_Reserved_31_26_WIDTH                  6
#define D0F0xBC_xC0000004_Reserved_31_26_MASK                   0xFC000000

/// D0F0xBC_xC0000004
typedef union {
  struct {                                                            ///<
    UINT32                                     RCU_TST_jpc_rep_req:1; ///<
    UINT32                                    TST_RCU_jpc_rep_done:1; ///<
    UINT32                                            drv_rst_mode:1; ///<
    UINT32                             SMU_DC_efuse_status_invalid:1; ///<
    UINT32                                            Reserved_5_4:2; ///<
    UINT32                                               TP_Tester:1; ///<
    UINT32                                           boot_seq_done:1; ///<
    UINT32                                    sclk_deep_sleep_exit:1; ///<
    UINT32                                        BREAK_PT1_ACTIVE:1; ///<
    UINT32                                        BREAK_PT2_ACTIVE:1; ///<
    UINT32                                                FCH_HALT:1; ///<
    UINT32                                  FCH_LOCKDOWN_WRITE_DIS:1; ///<
    UINT32                                    RCU_GIO_fch_lockdown:1; ///<
    UINT32                                          Reserved_15_14:2; ///<
    UINT32                                      INTERRUPTS_ENABLED:1; ///<
    UINT32                                        RCU_DtmCnt0_Done:1; ///<
    UINT32                                        RCU_DtmCnt1_Done:1; ///<
    UINT32                                        RCU_DtmCnt2_Done:1; ///<
    UINT32                                          Reserved_23_20:4; ///<
    UINT32                                          lm32_irq31_sel:2; ///<
    UINT32                                          Reserved_31_26:6; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_xC0000004_STRUCT;

// **** D0F0xBC_x80000000 Register Definition ****
// Address
#define D0F0xBC_x80000000_ADDRESS                               0x80000000
// Type
#define D0F0xBC_x80000000_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x80000000_smu_rst_reg_OFFSET                    0
#define D0F0xBC_x80000000_smu_rst_reg_WIDTH                     1
#define D0F0xBC_x80000000_smu_rst_reg_MASK                      0x1
#define D0F0xBC_x80000000_srbm_soft_rst_override_OFFSET         1
#define D0F0xBC_x80000000_srbm_soft_rst_override_WIDTH          1
#define D0F0xBC_x80000000_srbm_soft_rst_override_MASK           0x2
#define D0F0xBC_x80000000_Reserved_29_2_OFFSET                  2
#define D0F0xBC_x80000000_Reserved_29_2_WIDTH                   28
#define D0F0xBC_x80000000_Reserved_29_2_MASK                    0x3FFFFFFC
#define D0F0xBC_x80000000_RegReset_OFFSET                       30
#define D0F0xBC_x80000000_RegReset_WIDTH                        1
#define D0F0xBC_x80000000_RegReset_MASK                         0x40000000
#define D0F0xBC_x80000000_Reserved_31_31_OFFSET                 31
#define D0F0xBC_x80000000_Reserved_31_31_WIDTH                  1
#define D0F0xBC_x80000000_Reserved_31_31_MASK                   0x80000000

/// D0F0xBC_x80000000
typedef union {
  struct {                                                            ///<
    UINT32                                             smu_rst_reg:1; ///<
    UINT32                                  srbm_soft_rst_override:1; ///<
    UINT32                                           Reserved_29_2:28; ///<
    UINT32                                                RegReset:1; ///<
    UINT32                                          Reserved_31_31:1; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x80000000_STRUCT;

// **** D0F0xBC_x80000004 Register Definition ****
// Address
#define D0F0xBC_x80000004_ADDRESS                               0x80000004
// Type
#define D0F0xBC_x80000004_TYPE                                  TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x80000004_smu_ck_disable_OFFSET                 0
#define D0F0xBC_x80000004_smu_ck_disable_WIDTH                  1
#define D0F0xBC_x80000004_smu_ck_disable_MASK                   0x1
#define D0F0xBC_x80000004_smu_auto_cg_en_OFFSET                 1
#define D0F0xBC_x80000004_smu_auto_cg_en_WIDTH                  1
#define D0F0xBC_x80000004_smu_auto_cg_en_MASK                   0x2
#define D0F0xBC_x80000004_Reserved_7_2_OFFSET                   2
#define D0F0xBC_x80000004_Reserved_7_2_WIDTH                    6
#define D0F0xBC_x80000004_Reserved_7_2_MASK                     0xFC
#define D0F0xBC_x80000004_smu_auto_cg_timeout_OFFSET            8
#define D0F0xBC_x80000004_smu_auto_cg_timeout_WIDTH             16
#define D0F0xBC_x80000004_smu_auto_cg_timeout_MASK              0xFFFF00
#define D0F0xBC_x80000004_smu_cken_OFFSET                       24
#define D0F0xBC_x80000004_smu_cken_WIDTH                        1
#define D0F0xBC_x80000004_smu_cken_MASK                         0x1000000
#define D0F0xBC_x80000004_Reserved_31_25_OFFSET                 25
#define D0F0xBC_x80000004_Reserved_31_25_WIDTH                  7
#define D0F0xBC_x80000004_Reserved_31_25_MASK                   0xFE000000

/// D0F0xBC_x80000004
typedef union {
  struct {                                                            ///<
    UINT32                                          smu_ck_disable:1; ///<
    UINT32                                          smu_auto_cg_en:1; ///<
    UINT32                                            Reserved_7_2:6; ///<
    UINT32                                     smu_auto_cg_timeout:16; ///<
    UINT32                                                smu_cken:1; ///<
    UINT32                                          Reserved_31_25:7; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x80000004_STRUCT;

// **** D0F0xBC_x3F800 Register Definition ****
// Address
#define D0F0xBC_x3F800_ADDRESS                                  0x3F800
// Type
#define D0F0xBC_x3F800_TYPE                                     TYPE_D0F0xBC

// Field Data
#define D0F0xBC_x3F800_InterruptsEnabled_OFFSET                 0
#define D0F0xBC_x3F800_InterruptsEnabled_WIDTH                  1
#define D0F0xBC_x3F800_InterruptsEnabled_MASK                   0x1
#define D0F0xBC_x3F800_Reserved_23_1_OFFSET                     1
#define D0F0xBC_x3F800_Reserved_23_1_WIDTH                      23
#define D0F0xBC_x3F800_Reserved_23_1_MASK                       0xFFFFFE
#define D0F0xBC_x3F800_TestCount_OFFSET                         24
#define D0F0xBC_x3F800_TestCount_WIDTH                          8
#define D0F0xBC_x3F800_TestCount_MASK                           0xFF000000

/// D0F0xBC_x3F800
typedef union {
  struct {                                                            ///<
    UINT32                                       InterruptsEnabled:1; ///<
    UINT32                                           Reserved_23_1:23; ///<
    UINT32                                               TestCount:8; ///<

  } Field;

  UINT32 Value;
} D0F0xBC_x3F800_STRUCT;

// **** D0F0xBC_x3FFFC Register Definition ****
// Address
#define D0F0xBC_x3FFFC_ADDRESS                                  0x3FFFC

// Type
#define D0F0xBC_x3FFFC_TYPE                                     TYPE_D0F0xBC

/// D0F0xBC_x3FFFC
typedef struct {
  UINT32 Value;                                                         ///<
} D0F0xBC_x3FFFC_STRUCT;

// **** D0F0xFC_x00 Register Definition ****
// Address
#define D0F0xFC_x00_ADDRESS                                     0x0
// Type
#define D0F0xFC_x00_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x00_IoapicEnable_OFFSET                         0
#define D0F0xFC_x00_IoapicEnable_WIDTH                          1
#define D0F0xFC_x00_IoapicEnable_MASK                           0x1
#define D0F0xFC_x00_Reserved_1_1_OFFSET                         1
#define D0F0xFC_x00_Reserved_1_1_WIDTH                          1
#define D0F0xFC_x00_Reserved_1_1_MASK                           0x2
#define D0F0xFC_x00_IoapicIdExtEn_OFFSET                        2
#define D0F0xFC_x00_IoapicIdExtEn_WIDTH                         1
#define D0F0xFC_x00_IoapicIdExtEn_MASK                          0x4
#define D0F0xFC_x00_Reserved_3_3_OFFSET                         3
#define D0F0xFC_x00_Reserved_3_3_WIDTH                          1
#define D0F0xFC_x00_Reserved_3_3_MASK                           0x8
#define D0F0xFC_x00_IoapicSbFeatureEn_OFFSET                    4
#define D0F0xFC_x00_IoapicSbFeatureEn_WIDTH                     1
#define D0F0xFC_x00_IoapicSbFeatureEn_MASK                      0x10
#define D0F0xFC_x00_Reserved_31_5_OFFSET                        5
#define D0F0xFC_x00_Reserved_31_5_WIDTH                         27
#define D0F0xFC_x00_Reserved_31_5_MASK                          0xFFFFFFE0

/// D0F0xFC_x00
typedef union {
  struct {                                                            ///<
    UINT32                                            IoapicEnable:1; ///<
    UINT32                                            Reserved_1_1:1; ///<
    UINT32                                           IoapicIdExtEn:1; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                       IoapicSbFeatureEn:1; ///<
    UINT32                                           Reserved_31_5:27; ///<

  } Field;

  UINT32 Value;
} D0F0xFC_x00_STRUCT;

// **** D0F0xFC_x0F Register Definition ****
// Address
#define D0F0xFC_x0F_ADDRESS                                     0x0F
// Type
#define D0F0xFC_x0F_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x0F_GBIFExtIntrGrp_OFFSET                         0
#define D0F0xFC_x0F_GBIFExtIntrGrp_WIDTH                          3
#define D0F0xFC_x0F_GBIFExtIntrGrp_MASK                           0x7
#define D0F0xFC_x0F_Reserved_3_3_OFFSET                           3
#define D0F0xFC_x0F_Reserved_3_3_WIDTH                            1
#define D0F0xFC_x0F_Reserved_3_3_MASK                             0x8
#define D0F0xFC_x0F_GBIFExtIntrSwz_OFFSET                         4
#define D0F0xFC_x0F_GBIFExtIntrSwz_WIDTH                          2
#define D0F0xFC_x0F_GBIFExtIntrSwz_MASK                           0x30
#define D0F0xFC_x0F_Reserved_31_6_OFFSET                          6
#define D0F0xFC_x0F_Reserved_31_6_WIDTH                           26
#define D0F0xFC_x0F_Reserved_31_6_MASK                            0xFFFFFFC0

/// D0F0xFC_xOF
typedef union {
  struct {                                                            ///<
    UINT32                                            GBIFExtIntrGrp:3; ///<
    UINT32                                              Reserved_3_3:1; ///<
    UINT32                                            GBIFExtIntrSwz:2; ///<
    UINT32                                             Reserved_31_6:26; ///<
  } Field;

  UINT32 Value;
} D0F0xFC_x0F_STRUCT;

// **** D0F0xFC_x10 Register Definition ****
// Address
#define D0F0xFC_x10_ADDRESS                                     0x10
// Type
#define D0F0xFC_x10_TYPE                                        TYPE_D0F0xFC

// Field Data
#define D0F0xFC_x10_BrExtIntrGrp_OFFSET                         0
#define D0F0xFC_x10_BrExtIntrGrp_WIDTH                          3
#define D0F0xFC_x10_BrExtIntrGrp_MASK                           0x7
#define D0F0xFC_x10_Reserved_3_3_OFFSET                         3
#define D0F0xFC_x10_Reserved_3_3_WIDTH                          1
#define D0F0xFC_x10_Reserved_3_3_MASK                           0x8
#define D0F0xFC_x10_BrExtIntrSwz_OFFSET                         4
#define D0F0xFC_x10_BrExtIntrSwz_WIDTH                          2
#define D0F0xFC_x10_BrExtIntrSwz_MASK                           0x30
#define D0F0xFC_x10_Reserved_15_6_OFFSET                        6
#define D0F0xFC_x10_Reserved_15_6_WIDTH                         10
#define D0F0xFC_x10_Reserved_15_6_MASK                          0xFFC0
#define D0F0xFC_x10_BrIntIntrMap_OFFSET                         16
#define D0F0xFC_x10_BrIntIntrMap_WIDTH                          5
#define D0F0xFC_x10_BrIntIntrMap_MASK                           0x1F0000
#define D0F0xFC_x10_Reserved_31_21_OFFSET                       21
#define D0F0xFC_x10_Reserved_31_21_WIDTH                        11
#define D0F0xFC_x10_Reserved_31_21_MASK                         0xFFE00000

/// D0F0xFC_x10
typedef union {
  struct {                                                            ///<
    UINT32                                            BrExtIntrGrp:3; ///<
    UINT32                                            Reserved_3_3:1; ///<
    UINT32                                            BrExtIntrSwz:2; ///<
    UINT32                                           Reserved_15_6:10; ///<
    UINT32                                            BrIntIntrMap:5; ///<
    UINT32                                          Reserved_31_21:11; ///<

  } Field;

  UINT32 Value;
} D0F0xFC_x10_STRUCT;

// **** D0F0xCC_x01 Register Definition ****
// Address
#define D0F0xCC_x01_ADDRESS                                     0x1
// Type
#define D0F0xCC_x01_TYPE                                        TYPE_D0F0xCC

// Field Data
#define D0F0xCC_x01_BridgeDis_OFFSET                            0
#define D0F0xCC_x01_BridgeDis_WIDTH                             1
#define D0F0xCC_x01_BridgeDis_MASK                              0x1
#define D0F0xCC_x01_BusMasterDis_OFFSET                         1
#define D0F0xCC_x01_BusMasterDis_WIDTH                          1
#define D0F0xCC_x01_BusMasterDis_MASK                           0x2
#define D0F0xCC_x01_CfgDis_OFFSET                               2
#define D0F0xCC_x01_CfgDis_WIDTH                                1
#define D0F0xCC_x01_CfgDis_MASK                                 0x4
#define D0F0xCC_x01_P2pDis_OFFSET                               3
#define D0F0xCC_x01_P2pDis_WIDTH                                1
#define D0F0xCC_x01_P2pDis_MASK                                 0x8
#define D0F0xCC_x01_Reserved_15_4_OFFSET                        4
#define D0F0xCC_x01_Reserved_15_4_WIDTH                         12
#define D0F0xCC_x01_Reserved_15_4_MASK                          0xFFF0
#define D0F0xCC_x01_ExtDevPlug_OFFSET                           16
#define D0F0xCC_x01_ExtDevPlug_WIDTH                            1
#define D0F0xCC_x01_ExtDevPlug_MASK                             0x10000
#define D0F0xCC_x01_ExtDevCrsEn_OFFSET                          17
#define D0F0xCC_x01_ExtDevCrsEn_WIDTH                           1
#define D0F0xCC_x01_ExtDevCrsEn_MASK                            0x20000
#define D0F0xCC_x01_CrsEnable_OFFSET                            18
#define D0F0xCC_x01_CrsEnable_WIDTH                             1
#define D0F0xCC_x01_CrsEnable_MASK                              0x40000
#define D0F0xCC_x01_Reserved_19_19_OFFSET                       19
#define D0F0xCC_x01_Reserved_19_19_WIDTH                        1
#define D0F0xCC_x01_Reserved_19_19_MASK                         0x80000
#define D0F0xCC_x01_SetPowEn_OFFSET                             20
#define D0F0xCC_x01_SetPowEn_WIDTH                              1
#define D0F0xCC_x01_SetPowEn_MASK                               0x100000
#define D0F0xCC_x01_Reserved_22_21_OFFSET                       21
#define D0F0xCC_x01_Reserved_22_21_WIDTH                        2
#define D0F0xCC_x01_Reserved_22_21_MASK                         0x600000
#define D0F0xCC_x01_ApicEnable_OFFSET                           23
#define D0F0xCC_x01_ApicEnable_WIDTH                            1
#define D0F0xCC_x01_ApicEnable_MASK                             0x800000
#define D0F0xCC_x01_ApicRange_OFFSET                            24
#define D0F0xCC_x01_ApicRange_WIDTH                             8
#define D0F0xCC_x01_ApicRange_MASK                              0xFF000000

/// D0F0xCC_x01
typedef union {
  struct {                                                            ///<
    UINT32                                               BridgeDis:1; ///<
    UINT32                                            BusMasterDis:1; ///<
    UINT32                                                  CfgDis:1; ///<
    UINT32                                                  P2pDis:1; ///<
    UINT32                                           Reserved_15_4:12; ///<
    UINT32                                              ExtDevPlug:1; ///<
    UINT32                                             ExtDevCrsEn:1; ///<
    UINT32                                               CrsEnable:1; ///<
    UINT32                                          Reserved_19_19:1; ///<
    UINT32                                                SetPowEn:1; ///<
    UINT32                                          Reserved_22_21:2; ///<
    UINT32                                              ApicEnable:1; ///<
    UINT32                                               ApicRange:8; ///<

  } Field;

  UINT32 Value;
} D0F0xCC_x01_STRUCT;

// **** D0F0xC8 Register Definition ****
// Address
#define D0F0xC8_ADDRESS                                         0xC8
// Type
#define D0F0xC8_TYPE                                            TYPE_D0F0

// Field Data
#define D0F0xC8_NB_DEV_IND_ADDR_OFFSET                          0
#define D0F0xC8_NB_DEV_IND_ADDR_WIDTH                           7
#define D0F0xC8_NB_DEV_IND_ADDR_MASK                            0x7F
#define D0F0xC8_Reserved_15_7_OFFSET                            7
#define D0F0xC8_Reserved_15_7_WIDTH                             9
#define D0F0xC8_Reserved_15_7_MASK                              0xFF80
#define D0F0xC8_NB_DEV_IND_SEL_OFFSET                           16
#define D0F0xC8_NB_DEV_IND_SEL_WIDTH                            8
#define D0F0xC8_NB_DEV_IND_SEL_MASK                             0xFF0000
#define D0F0xC8_Reserved_31_24_OFFSET                           24
#define D0F0xC8_Reserved_31_24_WIDTH                            8
#define D0F0xC8_Reserved_31_24_MASK                             0xFF000000

/// D0F0xC8
typedef union {
  struct {                                                            ///<
    UINT32                                         NB_DEV_IND_ADDR:7; ///<
    UINT32                                           Reserved_15_7:9; ///<
    UINT32                                          NB_DEV_IND_SEL:8; ///<
    UINT32                                          Reserved_31_24:8; ///<

  } Field;

  UINT32 Value;
} D0F0xC8_STRUCT;

// **** D0F0x60 Register Definition ****
// Address
#define D0F0x60_ADDRESS                                         0x60
// Type
#define D0F0x60_TYPE                                            TYPE_D0F0

// **** D0F0x94 Register Definition ****
// Address
#define D0F0x94_ADDRESS                                         0x94
// Type
#define D0F0x94_TYPE                                            TYPE_D0F0

// **** D0F0xB8 Register Definition ****
// Address
#define D0F0xB8_ADDRESS                                         0xB8
// Type
#define D0F0xB8_TYPE                                            TYPE_D0F0

// **** D0F0xD0 Register Definition ****
// Address
#define D0F0xD0_ADDRESS                                         0xD0
// Type
#define D0F0xD0_TYPE                                            TYPE_D0F0

// **** D0F0xE0 Register Definition ****
// Address
#define D0F0xE0_ADDRESS                                         0xE0
// Type
#define D0F0xE0_TYPE                                            TYPE_D0F0

// **** D0F2xF0 Register Definition ****
// Address
#define D0F2xF0_ADDRESS                                         0xF0
// Type
#define D0F2xF0_TYPE                                            TYPE_D0F2

// **** D0F2xF8 Register Definition ****
// Address
#define D0F2xF8_ADDRESS                                         0xF8
// Type
#define D0F2xF8_TYPE                                            TYPE_D0F2

// **** D18F1xF0 Register Definition ****
// Address
#define D18F1xF0_ADDRESS                                        0xF0
// Type
#define D18F1xF0_TYPE                                           TYPE_D18F1

// **** D18F1x120 Register Definition ****
// Address
#define D18F1x120_ADDRESS                                       0x120
// Type
#define D18F1x120_TYPE                                          TYPE_D18F1

// **** D18F1x124 Register Definition ****
// Address
#define D18F1x124_ADDRESS                                       0x124

// Type
#define D18F1x124_TYPE                                          TYPE_D18F1
// Field Data
#define D18F1x124_DramLimitAddr_47_27_OFFSET                    0
#define D18F1x124_DramLimitAddr_47_27_WIDTH                     21
#define D18F1x124_DramLimitAddr_47_27_MASK                      0x1fffff
#define D18F1x124_Reserved_31_21_OFFSET                         21
#define D18F1x124_Reserved_31_21_WIDTH                          11
#define D18F1x124_Reserved_31_21_MASK                           0xffe00000

/// D18F1x124
typedef union {
  struct {                                                              ///<
    UINT32                                      DramLimitAddr_47_27:21; ///<
    UINT32                                           Reserved_31_21:11; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F1x124_STRUCT;

// **** D18F1x200 Register Definition ****
// Address
#define D18F1x200_ADDRESS                                       0x200
// Type
#define D18F1x200_TYPE                                          TYPE_D18F1

// **** D18F1x204 Register Definition ****
// Address
#define D18F1x204_ADDRESS                                       0x204
// Type
#define D18F1x204_TYPE                                          TYPE_D18F1

// **** D18F1x208 Register Definition ****
// Address
#define D18F1x208_ADDRESS                                       0x208
// Type
#define D18F1x208_TYPE                                          TYPE_D18F1

// **** D18F1x20C Register Definition ****
// Address
#define D18F1x20C_ADDRESS                                       0x20C
// Type
#define D18F1x20C_TYPE                                          TYPE_D18F1

// **** D18F1x240 Register Definition ****
// Address
#define D18F1x240_ADDRESS                                       0x240
// Type
#define D18F1x240_TYPE                                          TYPE_D18F1

// **** D18F1x244 Register Definition ****
// Address
#define D18F1x244_ADDRESS                                       0x244
// Type
#define D18F1x244_TYPE                                          TYPE_D18F1

// **** D18F1x248 Register Definition ****
// Address
#define D18F1x248_ADDRESS                                       0x248
// Type
#define D18F1x248_TYPE                                          TYPE_D18F1

// **** D18F1x24C Register Definition ****
// Address
#define D18F1x24C_ADDRESS                                       0x24C
// Type
#define D18F1x24C_TYPE                                          TYPE_D18F1

// **** D18F2x40_dct3 Register Definition ****
// Address
#define D18F2x40_dct3_ADDRESS                                   0x40
// Type
#define D18F2x40_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x40_dct1 Register Definition ****
// Address
#define D18F2x40_dct1_ADDRESS                                   0x40
// Type
#define D18F2x40_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x40_dct0 Register Definition ****
// Address
#define D18F2x40_dct0_ADDRESS                                   0x40
// Type
#define D18F2x40_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x44_dct3 Register Definition ****
// Address
#define D18F2x44_dct3_ADDRESS                                   0x44
// Type
#define D18F2x44_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x44_dct1 Register Definition ****
// Address
#define D18F2x44_dct1_ADDRESS                                   0x44
// Type
#define D18F2x44_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x44_dct0 Register Definition ****
// Address
#define D18F2x44_dct0_ADDRESS                                   0x44
// Type
#define D18F2x44_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x48_dct3 Register Definition ****
// Address
#define D18F2x48_dct3_ADDRESS                                   0x48
// Type
#define D18F2x48_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x48_dct1 Register Definition ****
// Address
#define D18F2x48_dct1_ADDRESS                                   0x48
// Type
#define D18F2x48_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x48_dct0 Register Definition ****
// Address
#define D18F2x48_dct0_ADDRESS                                   0x48
// Type
#define D18F2x48_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x4C_dct0 Register Definition ****
// Address
#define D18F2x4C_dct0_ADDRESS                                   0x4C
// Type
#define D18F2x4C_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x4C_dct3 Register Definition ****
// Address
#define D18F2x4C_dct3_ADDRESS                                   0x4C
// Type
#define D18F2x4C_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x4C_dct1 Register Definition ****
// Address
#define D18F2x4C_dct1_ADDRESS                                   0x4C
// Type
#define D18F2x4C_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x60_dct1 Register Definition ****
// Address
#define D18F2x60_dct1_ADDRESS                                   0x60
// Type
#define D18F2x60_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x60_dct0 Register Definition ****
// Address
#define D18F2x60_dct0_ADDRESS                                   0x60
// Type
#define D18F2x60_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x60_dct3 Register Definition ****
// Address
#define D18F2x60_dct3_ADDRESS                                   0x60
// Type
#define D18F2x60_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x64_dct0 Register Definition ****
// Address
#define D18F2x64_dct0_ADDRESS                                   0x64
// Type
#define D18F2x64_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x64_dct3 Register Definition ****
// Address
#define D18F2x64_dct3_ADDRESS                                   0x64
// Type
#define D18F2x64_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x64_dct1 Register Definition ****
// Address
#define D18F2x64_dct1_ADDRESS                                   0x64
// Type
#define D18F2x64_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x80_dct0 Register Definition ****
// Address
#define D18F2x80_dct0_ADDRESS                                   0x80
// Type
#define D18F2x80_dct0_TYPE                                      TYPE_D18F2_dct0

// **** D18F2x80_dct3 Register Definition ****
// Address
#define D18F2x80_dct3_ADDRESS                                   0x80
// Type
#define D18F2x80_dct3_TYPE                                      TYPE_D18F2_dct3

// **** D18F2x80_dct1 Register Definition ****
// Address
#define D18F2x80_dct1_ADDRESS                                   0x80
// Type
#define D18F2x80_dct1_TYPE                                      TYPE_D18F2_dct1

// **** D18F2x110 Register Definition ****
// Address
#define D18F2x110_ADDRESS                                       0x110
// Type
#define D18F2x110_TYPE                                          TYPE_D18F2

// **** D18F2x114 Register Definition ****
// Address
#define D18F2x114_ADDRESS                                       0x114
// Type
#define D18F2x114_TYPE                                          TYPE_D18F2

// **** D18F4x15C Register Definition ****
// Address
#define D18F4x15C_ADDRESS                                       0x15c

// Type
#define D18F4x15C_TYPE                                          TYPE_D18F4
// Field Data
#define D18F4x15C_BoostSrc_OFFSET                               0
#define D18F4x15C_BoostSrc_WIDTH                                2
#define D18F4x15C_BoostSrc_MASK                                 0x3

// **** D18F4x128 Register Definition ****
// Address
#define D18F4x128_ADDRESS                                       0x128
// Type
#define D18F4x128_TYPE                                          TYPE_D18F4

// **** D18F4x1B8 Register Definition ****
// Address
#define D18F4x1B8_ADDRESS                                       0x1B8
// Type
#define D18F4x1B8_TYPE                                          TYPE_D18F4

// **** DxFxxE4_x01 Register Definition ****
// Address
#define DxFxxE4_x01_ADDRESS                                     0x1
// Type
#define DxFxxE4_x01_TYPE                                        TYPE_D2F1xE4

// **** D0F0x64_x1F Register Definition ****
// Address
#define D0F0x64_x1F_ADDRESS                                     0x1F
// Type
#define D0F0x64_x1F_TYPE                                        TYPE_D0F0x64

// **** D0F2xF4_x49 Register Definition ****
// Address
#define D0F2xF4_x49_ADDRESS                                     0x49
// Type
#define D0F2xF4_x49_TYPE                                        TYPE_D0F2xF4

// **** D0F2xFC_x09_L1 Register Definition ****
// Address
#define D0F2xFC_x09_L1_ADDRESS(Sel)                             ((Sel << 16) | 0x9)
// Type
#define D0F2xFC_x09_L1_TYPE                                     TYPE_D0F2xFC

// **** GMMx15C0 Register Definition ****
// Address
#define GMMx15C0_ADDRESS                                        0x15C0
// Type
#define GMMx15C0_TYPE                                           TYPE_GMM

// **** GMMx2014 Register Definition ****
// Address
#define GMMx2014_ADDRESS                                        0x2014
// Type
#define GMMx2014_TYPE                                           TYPE_GMM

// **** GMMx2018 Register Definition ****
// Address
#define GMMx2018_ADDRESS                                        0x2018
// Type
#define GMMx2018_TYPE                                           TYPE_GMM

// **** GMMx201C Register Definition ****
// Address
#define GMMx201C_ADDRESS                                        0x201C
// Type
#define GMMx201C_TYPE                                           TYPE_GMM

// **** GMMx2020 Register Definition ****
// Address
#define GMMx2020_ADDRESS                                        0x2020
// Type
#define GMMx2020_TYPE                                           TYPE_GMM

// **** GMMx206C Register Definition ****
// Address
#define GMMx206C_ADDRESS                                        0x206C
// Type
#define GMMx206C_TYPE                                           TYPE_GMM

// **** GMMx20AC Register Definition ****
// Address
#define GMMx20AC_ADDRESS                                        0x20AC
// Type
#define GMMx20AC_TYPE                                           TYPE_GMM

// **** GMMx20B8 Register Definition ****
// Address
#define GMMx20B8_ADDRESS                                        0x20B8
// Type
#define GMMx20B8_TYPE                                           TYPE_GMM

// **** GMMx20BC Register Definition ****
// Address
#define GMMx20BC_ADDRESS                                        0x20BC
// Type
#define GMMx20BC_TYPE                                           TYPE_GMM

// **** GMMx20C0 Register Definition ****
// Address
#define GMMx20C0_ADDRESS                                        0x20C0
// Type
#define GMMx20C0_TYPE                                           TYPE_GMM

// **** GMMx20EC Register Definition ****
// Address
#define GMMx20EC_ADDRESS                                        0x20EC
// Type
#define GMMx20EC_TYPE                                           TYPE_GMM

// **** GMMx2114 Register Definition ****
// Address
#define GMMx2114_ADDRESS                                        0x2114
// Type
#define GMMx2114_TYPE                                           TYPE_GMM

// **** GMMx2144 Register Definition ****
// Address
#define GMMx2144_ADDRESS                                        0x2144
// Type
#define GMMx2144_TYPE                                           TYPE_GMM

// **** GMMx21448 Register Definition ****
// Address
#define GMMx2148_ADDRESS                                        0x2148
// Type
#define GMMx2148_TYPE                                           TYPE_GMM

// **** GMMx217C Register Definition ****
// Address
#define GMMx217C_ADDRESS                                        0x217C
// Type
#define GMMx217C_TYPE                                           TYPE_GMM

// **** GMMx2184 Register Definition ****
// Address
#define GMMx2184_ADDRESS                                        0x2184
// Type
#define GMMx2184_TYPE                                           TYPE_GMM

// **** GMMx218C Register Definition ****
// Address
#define GMMx218C_ADDRESS                                        0x218C
// Type
#define GMMx218C_TYPE                                           TYPE_GMM

// **** GMMx2194 Register Definition ****
// Address
#define GMMx2194_ADDRESS                                        0x2194
// Type
#define GMMx2194_TYPE                                           TYPE_GMM

// **** GMMx21C0 Register Definition ****
// Address
#define GMMx21C0_ADDRESS                                        0x21C0
// Type
#define GMMx21C0_TYPE                                           TYPE_GMM

// **** GMMx21E0 Register Definition ****
// Address
#define GMMx21E0_ADDRESS                                        0x21E0
// Type
#define GMMx21E0_TYPE                                           TYPE_GMM

// **** GMMx220C Register Definition ****
// Address
#define GMMx220C_ADDRESS                                        0x220C
// Type
#define GMMx220C_TYPE                                           TYPE_GMM

// **** GMMx2218 Register Definition ****
// Address
#define GMMx2218_ADDRESS                                        0x2218
// Type
#define GMMx2218_TYPE                                           TYPE_GMM

// **** GMMx2478 Register Definition ****
// Address
#define GMMx2478_ADDRESS                                        0x2478
// Type
#define GMMx2478_TYPE                                           TYPE_GMM

// **** GMMx253C Register Definition ****
// Address
#define GMMx253C_ADDRESS                                        0x253C
// Type
#define GMMx253C_TYPE                                           TYPE_GMM

// **** GMMx2550 Register Definition ****
// Address
#define GMMx2550_ADDRESS                                        0x2550
// Type
#define GMMx2550_TYPE                                           TYPE_GMM

// **** GMMx2558 Register Definition ****
// Address
#define GMMx2558_ADDRESS                                        0x2558
// Type
#define GMMx2558_TYPE                                           TYPE_GMM

// **** GMMx25C8 Register Definition ****
// Address
#define GMMx25C8_ADDRESS                                        0x25C8
// Type
#define GMMx25C8_TYPE                                           TYPE_GMM

// **** GMMx25CC Register Definition ****
// Address
#define GMMx25CC_ADDRESS                                        0x25CC
// Type
#define GMMx25CC_TYPE                                           TYPE_GMM

// **** GMMx25E0 Register Definition ****
// Address
#define GMMx25E0_ADDRESS                                        0x25E0
// Type
#define GMMx25E0_TYPE                                           TYPE_GMM

// **** GMMx25E4 Register Definition ****
// Address
#define GMMx25E4_ADDRESS                                        0x25E4
// Type
#define GMMx25E4_TYPE                                           TYPE_GMM

// **** GMMx25E8 Register Definition ****
// Address
#define GMMx25E8_ADDRESS                                        0x25E8
// Type
#define GMMx25E8_TYPE                                           TYPE_GMM

// **** GMMx2628 Register Definition ****
// Address
#define GMMx2628_ADDRESS                                        0x2628
// Type
#define GMMx2628_TYPE                                           TYPE_GMM

// **** GMMx262C Register Definition ****
// Address
#define GMMx262C_ADDRESS                                        0x262C
// Type
#define GMMx262C_TYPE                                           TYPE_GMM

// **** GMMx2648 Register Definition ****
// Address
#define GMMx2648_ADDRESS                                        0x2648
// Type
#define GMMx2648_TYPE                                           TYPE_GMM

// **** GMMx264C Register Definition ****
// Address
#define GMMx264C_ADDRESS                                        0x264C
// Type
#define GMMx264C_TYPE                                           TYPE_GMM

// **** GMMx2650 Register Definition ****
// Address
#define GMMx2650_ADDRESS                                        0x2650
// Type
#define GMMx2650_TYPE                                           TYPE_GMM

// **** GMMx276C Register Definition ****
// Address
#define GMMx276C_ADDRESS                                        0x276C
// Type
#define GMMx276C_TYPE                                           TYPE_GMM

// **** GMMx277C Register Definition ****
// Address
#define GMMx277C_ADDRESS                                        0x277C
// Type
#define GMMx277C_TYPE                                           TYPE_GMM

// **** GMMx2780 Register Definition ****
// Address
#define GMMx2780_ADDRESS                                        0x2780
// Type
#define GMMx2780_TYPE                                           TYPE_GMM

// **** GMMx278C Register Definition ****
// Address
#define GMMx278C_ADDRESS                                        0x278C
// Type
#define GMMx278C_TYPE                                           TYPE_GMM

// **** GMMx2790 Register Definition ****
// Address
#define GMMx2790_ADDRESS                                        0x2790
// Type
#define GMMx2790_TYPE                                           TYPE_GMM

// **** GMMx2794 Register Definition ****
// Address
#define GMMx2794_ADDRESS                                        0x2794
// Type
#define GMMx2794_TYPE                                           TYPE_GMM

// **** GMMx2798 Register Definition ****
// Address
#define GMMx2798_ADDRESS                                        0x2798
// Type
#define GMMx2798_TYPE                                           TYPE_GMM

// **** GMMx27A4 Register Definition ****
// Address
#define GMMx27A4_ADDRESS                                        0x27A4
// Type
#define GMMx27A4_TYPE                                           TYPE_GMM

// **** GMMx27A8 Register Definition ****
// Address
#define GMMx27A8_ADDRESS                                        0x27A8
// Type
#define GMMx27A8_TYPE                                           TYPE_GMM

// **** GMMx27C0 Register Definition ****
// Address
#define GMMx27C0_ADDRESS                                        0x27C0
// Type
#define GMMx27C0_TYPE                                           TYPE_GMM

// **** GMMx27C4 Register Definition ****
// Address
#define GMMx27C4_ADDRESS                                        0x27C4
// Type
#define GMMx27C4_TYPE                                           TYPE_GMM

// **** GMMx27D0 Register Definition ****
// Address
#define GMMx27D0_ADDRESS                                        0x27D0
// Type
#define GMMx27D0_TYPE                                           TYPE_GMM

// **** GMMx27D4 Register Definition ****
// Address
#define GMMx27D4_ADDRESS                                        0x27D4
// Type
#define GMMx27D4_TYPE                                           TYPE_GMM

// **** GMMx2814 Register Definition ****
// Address
#define GMMx2814_ADDRESS                                        0x2814
// Type
#define GMMx2814_TYPE                                           TYPE_GMM

// **** GMMx2818 Register Definition ****
// Address
#define GMMx2818_ADDRESS                                        0x2818
// Type
#define GMMx2818_TYPE                                           TYPE_GMM

// **** GMMx281C Register Definition ****
// Address
#define GMMx281C_ADDRESS                                        0x281C
// Type
#define GMMx281C_TYPE                                           TYPE_GMM

// **** GMMx2820 Register Definition ****
// Address
#define GMMx2820_ADDRESS                                        0x2820
// Type
#define GMMx2820_TYPE                                           TYPE_GMM

// **** GMMx2824 Register Definition ****
// Address
#define GMMx2824_ADDRESS                                        0x2824
// Type
#define GMMx2824_TYPE                                           TYPE_GMM

// **** GMMx2828 Register Definition ****
// Address
#define GMMx2828_ADDRESS                                        0x2828
// Type
#define GMMx2828_TYPE                                           TYPE_GMM

// **** GMMx282C Register Definition ****
// Address
#define GMMx282C_ADDRESS                                        0x282C
// Type
#define GMMx282C_TYPE                                           TYPE_GMM

// **** GMMx2830 Register Definition ****
// Address
#define GMMx2830_ADDRESS                                        0x2830
// Type
#define GMMx2830_TYPE                                           TYPE_GMM

// **** GMMx2834 Register Definition ****
// Address
#define GMMx2834_ADDRESS                                        0x2834
// Type
#define GMMx2834_TYPE                                           TYPE_GMM

// **** GMMx2838 Register Definition ****
// Address
#define GMMx2838_ADDRESS                                        0x2838
// Type
#define GMMx2838_TYPE                                           TYPE_GMM

// **** GMMx283C Register Definition ****
// Address
#define GMMx283C_ADDRESS                                        0x283C
// Type
#define GMMx283C_TYPE                                           TYPE_GMM

// **** GMMx2840 Register Definition ****
// Address
#define GMMx2840_ADDRESS                                        0x2840
// Type
#define GMMx2840_TYPE                                           TYPE_GMM

// **** GMMx2844 Register Definition ****
// Address
#define GMMx2844_ADDRESS                                        0x2844
// Type
#define GMMx2844_TYPE                                           TYPE_GMM

// **** GMMx2848 Register Definition ****
// Address
#define GMMx2848_ADDRESS                                        0x2848
// Type
#define GMMx2848_TYPE                                           TYPE_GMM

// **** GMMx284C Register Definition ****
// Address
#define GMMx284C_ADDRESS                                        0x284C
// Type
#define GMMx284C_TYPE                                           TYPE_GMM

// **** GMMx2850 Register Definition ****
// Address
#define GMMx2850_ADDRESS                                        0x2850
// Type
#define GMMx2850_TYPE                                           TYPE_GMM

// **** GMMx2854 Register Definition ****
// Address
#define GMMx2854_ADDRESS                                        0x2854
// Type
#define GMMx2854_TYPE                                           TYPE_GMM

// **** GMMx2858 Register Definition ****
// Address
#define GMMx2858_ADDRESS                                        0x2858
// Type
#define GMMx2858_TYPE                                           TYPE_GMM

// **** GMMx285C Register Definition ****
// Address
#define GMMx285C_ADDRESS                                        0x285C
// Type
#define GMMx285C_TYPE                                           TYPE_GMM

// **** GMMx2860 Register Definition ****
// Address
#define GMMx2860_ADDRESS                                        0x2860
// Type
#define GMMx2860_TYPE                                           TYPE_GMM

// **** GMMx2864 Register Definition ****
// Address
#define GMMx2864_ADDRESS                                        0x2864
// Type
#define GMMx2864_TYPE                                           TYPE_GMM

// **** GMMx2868 Register Definition ****
// Address
#define GMMx2868_ADDRESS                                        0x2868
// Type
#define GMMx2868_TYPE                                           TYPE_GMM

// **** GMMx286C Register Definition ****
// Address
#define GMMx286C_ADDRESS                                        0x286C
// Type
#define GMMx286C_TYPE                                           TYPE_GMM

// **** GMMx2870 Register Definition ****
// Address
#define GMMx2870_ADDRESS                                        0x2870
// Type
#define GMMx2870_TYPE                                           TYPE_GMM

// **** GMMx2874 Register Definition ****
// Address
#define GMMx2874_ADDRESS                                        0x2874
// Type
#define GMMx2874_TYPE                                           TYPE_GMM

// **** GMMx2878 Register Definition ****
// Address
#define GMMx2878_ADDRESS                                        0x2878
// Type
#define GMMx2878_TYPE                                           TYPE_GMM

// **** GMMx287C Register Definition ****
// Address
#define GMMx287C_ADDRESS                                        0x287C
// Type
#define GMMx287C_TYPE                                           TYPE_GMM

// **** GMMx2880 Register Definition ****
// Address
#define GMMx2880_ADDRESS                                        0x2880
// Type
#define GMMx2880_TYPE                                           TYPE_GMM

// **** GMMx3500 Register Definition ****
// Address
#define GMMx3500_ADDRESS                                        0x3500
// Type
#define GMMx3500_TYPE                                           TYPE_GMM

// **** GMMx3504 Register Definition ****
// Address
#define GMMx3504_ADDRESS                                        0x3504
// Type
#define GMMx3504_TYPE                                           TYPE_GMM

// **** GMMx3538 Register Definition ****
// Address
#define GMMx3538_ADDRESS                                        0x3538
// Type
#define GMMx3538_TYPE                                           TYPE_GMM

// **** GMMx353C Register Definition ****
// Address
#define GMMx353C_ADDRESS                                        0x353C
// Type
#define GMMx353C_TYPE                                           TYPE_GMM

// **** D0F0x64_x19 Register Definition ****
// Address
#define D0F0x64_x19_ADDRESS                                     0x19
// Type
#define D0F0x64_x19_TYPE                                        TYPE_D0F0x64

// **** D0F0x64_x1A Register Definition ****
// Address
#define D0F0x64_x1A_ADDRESS                                     0x1A
// Type
#define D0F0x64_x1A_TYPE                                        TYPE_D0F0x64

// **** D0F0x64_x4E Register Definition ****
// Address
#define D0F0x64_x4E_ADDRESS                                     0x4E
// Type
#define D0F0x64_x4E_TYPE                                        TYPE_D0F0x64

// **** D0F0xBC_x20000 Register Definition ****
// Address
#define D0F0xBC_x20000_ADDRESS                                  0x20000
// Type
#define D0F0xBC_x20000_TYPE                                     TYPE_D0F0xBC

// **** D0F0xBC_x0 Register Definition ****
// Address
#define D0F0xBC_x0_ADDRESS                                      0x0
// Type
#define D0F0xBC_x0_TYPE                                         TYPE_D0F0xBC

// **** D0F0xF8 Register Definition ****
// Address
#define D0F0xF8_ADDRESS                                         0xF8
// Type
#define D0F0xF8_TYPE                                            TYPE_D0F0

// **** D0F0xE4_PIF_0017 Register Definition ****
// Address
#define D0F0xE4_PIF_0017_ADDRESS                                0x17
// Type
#define D0F0xE4_PIF_0017_TYPE                                   TYPE_D0F0xE4

// **** D0F0xE4_WRAP_8021 Register Definition ****
// Address
#define D0F0xE4_WRAP_8021_ADDRESS                               0x8021
// Type
#define D0F0xE4_WRAP_8021_TYPE                                  TYPE_D0F0xE4

// **** D0F0xE4_WRAP_8025 Register Definition ****
// Address
#define D0F0xE4_WRAP_8025_ADDRESS                               0x8025
// Type
#define D0F0xE4_WRAP_8025_TYPE                                  TYPE_D0F0xE4

// **** D0F0xBC_x80008000 Register Definition ****
// Address
#define D0F0xBC_x80008000_ADDRESS                               0x80008000
// Type
#define D0F0xBC_x80008000_TYPE                                  TYPE_D0F0xBC

// **** D0F0xBC_xC210003C Register Definition ****
// Address
#define D0F0xBC_xC210003C_ADDRESS                               0xC210003C
// Type
#define D0F0xBC_xC210003C_TYPE                                  TYPE_D0F0xBC

// **** D0F0xE4_WRAP_8029 Register Definition ****
// Address
#define D0F0xE4_WRAP_8029_ADDRESS                               0x8029
// Type
#define D0F0xE4_WRAP_8029_TYPE                                  TYPE_D0F0xE4

// Field Data
#define D0F0xE4_WRAP_8029_LaneEnable_OFFSET                     0
#define D0F0xE4_WRAP_8029_LaneEnable_WIDTH                      16
#define D0F0xE4_WRAP_8029_LaneEnable_MASK                       0xFFFF
#define D0F0xE4_WRAP_8029_Reserved_31_16_OFFSET                 16
#define D0F0xE4_WRAP_8029_Reserved_31_16_WIDTH                  16
#define D0F0xE4_WRAP_8029_Reserved_31_16_MASK                   0xFFFF0000

/// D0F0xE4_WRAP_8029
typedef union {
  struct {                                                            ///<
    UINT32                                              LaneEnable:16; ///<
    UINT32                                          Reserved_31_16:16; ///<

  } Field;

  UINT32 Value;
} D0F0xE4_WRAP_8029_STRUCT;

#endif
