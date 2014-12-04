/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug Routines for performance analysis
 *
 * Contains AMD AGESA debug macros and functions for performance analysis
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
/*****************************************************************************
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
#ifndef _IDS_PERFORMANCE_DATA_POINT

  #define _IDS_PERFORMANCE_DATA_POINT
  #define IDS_PERF_VERSION 0x00010000ul  //version number 0.1.0.0
/// Time points performance function used
  typedef enum {
    TP_BEGINPROCAMDINITEARLY             = 0x100,  ///< BeginProcAmdInitEarly
    TP_ENDPROCAMDINITEARLY               = 0x101,  ///< EndProcAmdInitEarly
    TP_BEGINAMDTOPOINITIALIZE            = 0x102,  ///< BeginAmdTopoInitialize
    TP_ENDAMDTOPOINITIALIZE              = 0x103,  ///< EndAmdTopoInitialize
    TP_BEGINGNBINITATEARLIER             = 0x104,  ///< BeginGnbInitAtEarlier
    TP_ENDGNBINITATEARLIER               = 0x105,  ///< EndGnbInitAtEarlier
    TP_BEGINAMDCPUEARLY                  = 0x106,  ///< BeginAmdCpuEarly
    TP_ENDAMDCPUEARLY                    = 0x107,  ///< EndAmdCpuEarly
    TP_BEGINGNBINITATEARLY               = 0x108,  ///< BeginGnbInitAtEarly
    TP_ENDGNBINITATEARLY                 = 0x109,  ///< EndGnbInitAtEarly
    TP_BEGINPROCAMDINITENV               = 0x10A,  ///< BeginProcAmdInitEnv
    TP_ENDPROCAMDINITENV                 = 0x10B,  ///< EndProcAmdInitEnv
    TP_BEGININITENV                      = 0x10C,  ///< BeginInitEnv
    TP_ENDINITENV                        = 0x10D,  ///< EndInitEnv
    TP_BEGINGNBINITATENV                 = 0x10E,  ///< BeginGnbInitAtEnv
    TP_ENDGNBINITATENV                   = 0x10F,  ///< EndGnbInitAtEnv
    TP_BEGINPROCAMDINITLATE              = 0x110,  ///< BeginProcAmdInitLate
    TP_ENDPROCAMDINITLATE                = 0x111,  ///< EndProcAmdInitLate
    TP_BEGINCREATSYSTEMTABLE             = 0x112,  ///< BeginCreatSystemTable
    TP_ENDCREATSYSTEMTABLE               = 0x113,  ///< EndCreatSystemTable
    TP_BEGINDISPATCHCPUFEATURESLATE      = 0x114,  ///< BeginDispatchCpuFeaturesLate
    TP_ENDDISPATCHCPUFEATURESLATE        = 0x115,  ///< EndDispatchCpuFeaturesLate
    TP_BEGINAMDCPULATE                   = 0x116,  ///< BeginAmdCpuLate
    TP_ENDAMDCPULATE                     = 0x117,  ///< EndAmdCpuLate
    TP_BEGINGNBINITATLATE                = 0x118,  ///< BeginGnbInitAtLate
    TP_ENDGNBINITATLATE                  = 0x119,  ///< EndGnbInitAtLate
    TP_BEGINPROCAMDINITMID               = 0x11A,  ///< BeginProcAmdInitMid
    TP_ENDPROCAMDINITMID                 = 0x11B,  ///< EndProcAmdInitMid
    TP_BEGININITMID                      = 0x11E,  ///< BeginInitMid
    TP_ENDINITMID                        = 0x11F,  ///< EndInitMid
    TP_BEGINGNBINITATMID                 = 0x120,  ///< BeginGnbInitAtMid
    TP_ENDGNBINITATMID                   = 0x121,  ///< EndGnbInitAtMid
    TP_BEGINPROCAMDINITPOST              = 0x122,  ///< BeginProcAmdInitPost
    TP_ENDPROCAMDINITPOST                = 0x123,  ///< EndProcAmdInitPost
    TP_BEGINGNBINITATPOST                = 0x124,  ///< BeginGnbInitAtPost
    TP_ENDGNBINITATPOST                  = 0x125,  ///< EndGnbInitAtPost
    TP_BEGINAMDMEMAUTO                   = 0x126,  ///< BeginAmdMemAuto
    TP_ENDAMDMEMAUTO                     = 0x127,  ///< EndAmdMemAuto
    TP_BEGINAMDCPUPOST                   = 0x128,  ///< BeginAmdCpuPost
    TP_ENDAMDCPUPOST                     = 0x129,  ///< EndAmdCpuPost
    TP_BEGINGNBINITATPOSTAFTERDRAM       = 0x12A,  ///< BeginGnbInitAtPostAfterDram
    TP_ENDGNBINITATPOSTAFTERDRAM         = 0x12B,  ///< EndGnbInitAtPostAfterDram
    TP_BEGINPROCAMDINITRESET             = 0x12C,  ///< BeginProcAmdInitReset
    TP_ENDPROCAMDINITRESET               = 0x12D,  ///< EndProcAmdInitReset
    TP_BEGININITRESET                    = 0x12E,  ///< BeginInitReset
    TP_ENDINITRESET                      = 0x12F,  ///< EndInitReset
    TP_BEGINHTINITRESET                  = 0x130,  ///< BeginHtInitReset
    TP_ENDHTINITRESET                    = 0x131,  ///< EndHtInitReset
    TP_BEGINPROCAMDINITRESUME            = 0x132,  ///< BeginProcAmdInitResume
    TP_ENDPROCAMDINITRESUME              = 0x133,  ///< EndProcAmdInitResume
    TP_BEGINAMDMEMS3RESUME               = 0x134,  ///< BeginAmdMemS3Resume
    TP_ENDAMDMEMS3RESUME                 = 0x135,  ///< EndAmdMemS3Resume
    TP_BEGINDISPATCHCPUFEATURESS3RESUME  = 0x136,  ///< BeginDispatchCpuFeaturesS3Resume
    TP_ENDDISPATCHCPUFEATURESS3RESUME    = 0x137,  ///< EndDispatchCpuFeaturesS3Resume
    TP_BEGINSETCORESTSCFREQSEL           = 0x138,  ///< BeginSetCoresTscFreqSel
    TP_ENDSETCORESTSCFREQSEL             = 0x139,  ///< EndSetCoresTscFreqSel
    TP_BEGINMEMFMCTMEMCLR_INIT           = 0x13A,  ///< BeginMemFMctMemClr_Init
    TP_ENDNMEMFMCTMEMCLR_INIT            = 0x13B,  ///< EndnMemFMctMemClr_Init
    TP_BEGINMEMBEFOREMEMDATAINIT         = 0x13C,  ///< BeginMemBeforeMemDataInit
    TP_ENDMEMBEFOREMEMDATAINIT           = 0x13D,  ///< EndMemBeforeMemDataInit
    TP_BEGINPROCAMDMEMAUTO               = 0x13E,  ///< BeginProcAmdMemAuto
    TP_ENDPROCAMDMEMAUTO                 = 0x13F,  ///< EndProcAmdMemAuto
    TP_BEGINMEMMFLOWC32                  = 0x140,  ///< BeginMemMFlowC32
    TP_ENDMEMMFLOWC32                    = 0x141,  ///< EndMemMFlowC32
    TP_BEGINMEMINITIALIZEMCT             = 0x142,  ///< BeginMemInitializeMCT
    TP_ENDMEMINITIALIZEMCT               = 0x143,  ///< EndMemInitializeMCT
    TP_BEGINMEMSYSTEMMEMORYMAPPING       = 0x144,  ///< BeginMemSystemMemoryMapping
    TP_ENDMEMSYSTEMMEMORYMAPPING         = 0x145,  ///< EndMemSystemMemoryMapping
    TP_BEGINMEMDRAMTRAINING              = 0x146,  ///< BeginMemDramTraining
    TP_ENDMEMDRAMTRAINING                = 0x147,  ///< EndMemDramTraining
    TP_BEGINMEMOTHERTIMING               = 0x148,  ///< BeginMemOtherTiming
    TP_ENDMEMOTHERTIMING                 = 0x149,  ///< EndMemOtherTiming
    TP_BEGINMEMUMAMEMTYPING              = 0x14A,  ///< BeginMemUMAMemTyping
    TP_ENDMEMUMAMEMTYPING                = 0x14B,  ///< EndMemUMAMemTyping
    TP_BEGINMEMMEMCLR                    = 0x14C,  ///< BeginMemMemClr
    TP_ENDMEMMEMCLR                      = 0x14D,  ///< EndMemMemClr
    TP_BEGINMEMMFLOWTN                   = 0x14E,  ///< BeginMemMFlowTN
    TP_ENDMEMMFLOWTN                     = 0x14F,  ///< EndMemMFlowTN
    TP_BEGINAGESAHOOKBEFOREDRAMINIT      = 0x150,  ///< BeginAgesaHookBeforeDramInit
    TP_ENDAGESAHOOKBEFOREDRAMINIT        = 0x151,  ///< EndAgesaHookBeforeDramInit
    TP_BEGINPROCMEMDRAMTRAINING          = 0x152,  ///< BeginProcMemDramTraining
    TP_ENDPROCMEMDRAMTRAINING            = 0x153,  ///< EndProcMemDramTraining
    TP_BEGINGNBINITATS3SAVE              = 0x154,  ///< BeginGnbInitAtS3Save
    TP_ENDGNBINITATS3SAVE                = 0x155,  ///< EndGnbInitAtS3Save
    TP_BEGINGNBLOADSCSDATA               = 0x156,  ///< BeginGnbLoadScsData
    TP_ENDGNBLOADSCSDATA                 = 0x157,  ///< EndGnbLoadScsData
    TP_BEGINGNBPCIETRAINING              = 0x158,  ///< BeginGnbPcieTraining
    TP_ENDGNBPCIETRAINING                = 0x159,  ///< EndGnbPcieTraining
    IDS_TP_END                                     ///< End of IDS TP list
  } IDS_PERF_DATA;
#endif
