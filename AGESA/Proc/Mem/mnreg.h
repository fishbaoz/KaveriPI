/* $NoKeywords:$ */
/**
 * @file
 *
 * mnreg.h
 *
 * Definitions for whole register tokens
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem)
 * @e \$Revision: 282148 $ @e \$Date: 2014-01-07 10:57:44 -0600 (Tue, 07 Jan 2014) $
 *
 **/
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
  * ***************************************************************************
  *
 */

#ifndef _MNREG_H_
#define _MNREG_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */

/// Registers used in memory initialization
typedef enum {
  // NB_REG identifiers
  NbRegRangeStart = BFEndOfList, ///< -------------------------- Start of NB_REG range
  RegDramBase0,                  ///< Register DramBase0
  RegDramLimit0,                 ///< Register DramLimit0
  RegDramBaseHi0,                ///< Register DramBaseHi0
  RegDramLimitHi0,               ///< Register DramLimitHi0
  RegDramBase1,                  ///< Register DramBase1
  RegDramLimit1,                 ///< Register DramLimit1
  RegDramBaseHi1,                ///< Register DramBaseHi1
  RegDramLimitHi1,               ///< Register DramLimitHi1
  RegDramBase2,                  ///< Register DramBase2
  RegDramLimit2,                 ///< Register DramLimit2
  RegDramBaseHi2,                ///< Register DramBaseHi2
  RegDramLimitHi2,               ///< Register DramLimitHi2
  RegDramBase3,                  ///< Register DramBase3
  RegDramLimit3,                 ///< Register DramLimit3
  RegDramBaseHi3,                ///< Register DramBaseHi3
  RegDramLimitHi3,               ///< Register DramLimitHi3
  RegDramBase4,                  ///< Register DramBase4
  RegDramLimit4,                 ///< Register DramLimit4
  RegDramBaseHi4,                ///< Register DramBaseHi4
  RegDramLimitHi4,               ///< Register DramLimitHi4
  RegDramBase5,                  ///< Register DramBase5
  RegDramLimit5,                 ///< Register DramLimit5
  RegDramBaseHi5,                ///< Register DramBaseHi5
  RegDramLimitHi5,               ///< Register DramLimitHi5
  RegDramBase6,                  ///< Register DramBase6
  RegDramLimit6,                 ///< Register DramLimit6
  RegDramBaseHi6,                ///< Register DramBaseHi6
  RegDramLimitHi6,               ///< Register DramLimitHi6
  RegDramBase7,                  ///< Register DramBase7
  RegDramLimit7,                 ///< Register DramLimit7
  RegDramBaseHi7,                ///< Register DramBaseHi7
  RegDramLimitHi7,               ///< Register DramLimitHi7

  RegDramHoleAddr,               ///< Register DramHoleAddr
  RegDctCfgSel,                  ///< Register DctCfgSel
  RegDramBaseSysAddr,            ///< Register DramBaseSysAddr
  RegDramLimitSysAddr,           ///< Register DramLimitSysAddr

  RegDramCtlrBase0,              ///< Register DramCtlrBase0
  RegDramCtlrBase1,              ///< Register DramCtlrBase1
  RegDramCtlrBase2,              ///< Register DramCtlrBase2
  RegDramCtlrBase3,              ///< Register DramCtlrBase3

  RegDramCtlrLimit0,             ///< Register DramCtlrLimit0
  RegDramCtlrLimit1,             ///< Register DramCtlrLimit1
  RegDramCtlrLimit2,             ///< Register DramCtlrLimit2
  RegDramCtlrLimit3,             ///< Register DramCtlrLimit3

  RegDctHiAddrOffset0,           ///< Register DctHiAddrOffset0
  RegDctHiAddrOffset1,           ///< Register DctHiAddrOffset1
  RegDctHiAddrOffset2,           ///< Register DctHiAddrOffset2
  RegDctHiAddrOffset3,           ///< Register DctHiAddrOffset3

  RegDramCtl,                    ///< Register DramCtl
  RegDramInit,                   ///< Register DramInit
  RegDramBankAddr,               ///< Register DramBankAddr
  RegDramMRS,                    ///< Register DramMRS
  RegDramTmgLo,                  ///< Register DramTmgLo
  RegDramTmgHi,                  ///< Register DramTmgHi
  RegDramConfigLo,               ///< Register DramConfigLo
  RegDramConfigHi,               ///< Register DramConfigHi

  RegDctAddlOffset,              ///< Register DctAddlOffset
  RegDctAddlData,                ///< Register DctAddlData
  RegDctAddlOffsetBrdcst,        ///< Register DctAddlOffset for broadcast
  RegDctAddlDataBrdcst,          ///< Register DctAddlData for broadcast

  RegDctTempThrottle,            ///< Register DctTempThrottle
  RegDramCtlrMisc2,              ///< Register DramCtlrMisc2
  RegDramCtlrMisc3,              ///< Register DramCtlrMisc3
  RegTraceBufferCtlr,            ///< Register TraceBufferCtlr
  RegSwapIntLvRgn,               ///< Register SwapIntLvRgn
  RegMctCfgLo,                   ///< Register MctCfgLo
  RegMctCfgHi,                   ///< Register MctCfgHi
  RegExtMctCfgLo,                ///< Register ExtMctCfgLo
  RegExtMctCfgHi,                ///< Register ExtMctCfgHi
  RegDramNbPstate,               ///< Register DramNbPstate

  RegGmcToDctCtl0,               ///< Register GmcToDctCtl0
  RegGmcToDctCtl1,               ///< Register GmcToDctCtl1
  RegGmcToDctCtl2,               ///< Register GmcToDctCtl2
  RegGmcToDctFifoCfg1,           ///< Register GMC to DCT FIFO Config 1

  RegCSBaseAddr0,                ///< Register CSBaseAddr0
  RegCSBaseAddr1,                ///< Register CSBaseAddr1
  RegCSBaseAddr2,                ///< Register CSBaseAddr2
  RegCSBaseAddr3,                ///< Register CSBaseAddr3
  RegCSMask0,                    ///< Register CSMask0
  RegCSMask1,                    ///< Register CSMask1
  RegDramCtlrSelLo,              ///< Register DramCtlrSelLo
  RegDramCtlrSelHi,              ///< Register DramCtlrSelHi

  RegDdr3DramTmg0,               ///< Register Ddr3DramTmg0
  RegDdr3DramTmg1,               ///< Register Ddr3DramTmg1
  RegDdr3DramTmg2,               ///< Register Ddr3DramTmg2
  RegDdr3DramTmg3,               ///< Register Ddr3DramTmg3
  RegDdr3DramTmg4,               ///< Register Ddr3DramTmg4
  RegDdr3DramTmg5,               ///< Register Ddr3DramTmg5
  RegDdr3DramTmg6,               ///< Register Ddr3DramTmg6
  RegDdr3DramTmg7,               ///< Register Ddr3DramTmg7
  RegDdr3DramTmg8,               ///< Register Ddr3DramTmg8
  RegDdr3DramTmg9,               ///< Register Ddr3DramTmg9
  RegDdr3DramTmg10,              ///< Register Ddr3DramTmg10
  RegPhyRODTCSLow ,              ///< Register RegPhyRODTCSLow
  RegPhyRODTCSHigh,              ///< Register RegPhyRODTCSHigh
  RegPhyWODTCSLow ,              ///< Register RegPhyWODTCSLow
  RegPhyWODTCSHigh,              ///< Register RegPhyWODTCSHigh
  RegDdr3DramPwrMng0,            ///< Register Ddr3DramPwrMng0
  RegDdr3DramPwrMng1,            ///< Register Ddr3DramPwrMng1
  RegDdr3DramOdtCtl,             ///< Register Ddr3DramOdtClt
  RegMemPsCtlSts,                ///< Register MemPsCtlSts
  RegMrsBuf0,                    ///< Register MRS Buffer 0
  RegMrsBuf1,                    ///< Register MRS Buffer 1

  RegDramLpbkTrnCtl,             ///< Register DramLpbkTrnCtl
  RegTargetABase,                ///< Register TargetABase
  RegDramCmd0,                   ///< Register DramCmd0
  RegDramCmd1,                   ///< Register DramCmd1
  RegDramCmd2,                   ///< Register DramCmd2
  RegDramPRBS,                   ///< Register DramPRBS
  RegDramStatus1,                ///< Register DramStatus1
  RegDramDqMaskLo,               ///< Register DramDqMaskLo
  RegDramDqMaskHi,               ///< Register DramDqMaskHi
  RegDramEccMask,                ///< Register DramEccMask
  RegDQErrLo,                    ///< Register DQErrLo
  RegDQErrHi,                    ///< Register DQErrHi

  RegGddr5DramTmg0,              ///< Register Gddr5DramTmg0
  RegGddr5DramTmg1,              ///< Register Gddr5DramTmg1
  RegGddr5DramTmg2,              ///< Register Gddr5DramTmg2
  RegGddr5DramTmg3,              ///< Register Gddr5DramTmg3
  RegGddr5DramTmg4,              ///< Register Gddr5DramTmg4
  RegGddr5DramTmg5,              ///< Register Gddr5DramTmg5
  RegGddr5DramTmg6,              ///< Register Gddr5DramTmg6
  RegGddr5DramTmg7,              ///< Register Gddr5DramTmg7
  RegGddr5DramTmg8,              ///< Register Gddr5DramTmg8
  RegGddr5DramPowerMng0,         ///< Register Gddr5DramPowerMng0
  RegGddr5DramPowerMng1,         ///< Register Gddr5DramPowerMng1
  RegGddr5DramCtrl0,             ///< Register Gddr5DramCtrl0
  RegGddr5DramNbPstate,          ///< Register Gddr5DramNbPstate

  RegNbCap2,                     ///< Register NbCap2
  RegNbPstateCtl,                ///< Register NbPstateCtl
  RegNbPstateStatus,             ///< Register NbPstateStatus
  RegNbPstate0,                  ///< Register NB Pstate 0
  RegNbPstate1,                  ///< Register NB Pstate 1
  RegNbPstate2,                  ///< Register NB Pstate 2
  RegNbPstate3,                  ///< Register NB Pstate 3
  RegNbFusionCfg,                ///< Register NB Fusion Configuration
  RegCUPwrStsLo,                 ///< Register Compute Unit Power Status Low
  RegCUPwrStsHi,                 ///< Register Compute Unit Power Status High

  RegCkeToCsMap,                 ///< Register Dram CKE to CS Map

  RegMcaNbCtl,                   ///< Register MCA NB Control
  RegMcaNbCfg,                   ///< Register MCA NB Configuration
  RegScrubRateCtl,               ///< Register Scrub Rate Control
  RegDramScrubAddrLo,            ///< Register DRAM Scrub Address Low
  RegDramScrubAddrHi,            ///< Register DRAM Scrub Address High
  RegNbCfg1Lo,                   ///< Register NB Configuration 1 Low
  RegCpuid,                      ///< Register CPUID Family/Model/Stepping
  RegExtMcaNbCfg,                ///< Register Extended NB MCA Configuration
  RegDramCtlMisc3,               ///< Register DRAM Controller Misc 3
  RegProductInformationRegister1,///< VDDP/VDDR Low Voltage Support Bit from ScratchFuse
  RegNbEccExclusionBaseLow,      ///< ECC Exclusion Base Address Low
  RegNbEccExclusionBaseHigh,     ///< ECC Exclusion Base Address High
  RegNbEccExclusionLimitLow,     ///< ECC Exclusion Limit Address Low
  RegNbEccExclusionLimitHigh,    ///< ECC Exclusion Limit Address High
  RegDctAndFusePowerGateCtrl,    ///< DCT and Fuse Power Gate Control
  RegNbCap,                      ///< Northbridge Capabilities

  RegReserved01,                 ///< Reserved
  RegReserved02,                 ///< Reserved
  RegReserved03,                 ///< Reserved
  RegReserved04,                 ///< Reserved
  RegReserved05,                 ///< Reserved

  NbRegRangeEnd,                 ///< -------------------------- End of NB_REG range

                                 // DCT_PHY_REG identifiers
  DctPhyRegRangeStart,           ///< -------------------------- Start of DCT_PHY_REG range
  RegRxCtl1,                     ///< Register RxCtl1
  RegRdPtrInitVal,               ///< Register RdPtrInitVal
  RegDataRdPtrInitVal,           ///< Register DataRdPtrInitVal
  RegEdcRdPtrInitVal,            ///< Register EdcRdPtrInitVal
  RegDataRdPtrOffset,            ///< Register DataRdPtrOffset
  RegDqDqsRcvCntrl1,             ///< Register DqDqsRcvCtrl1
  RegPmuRst,                     ///< Register PmuRst
  RegPwrStateCmd,                ///< Register Power State Command
  RegDsMbMsg,                    ///< Register DS Mailbox Message
  RegDsMbProtocol,               ///< Register DS Mailbox Protocol
  RegMbProtocolShadow,           ///< Register Mailbox Protocol Shadow
  RegUsMbMsg,                    ///< Register US Mailbox Message
  RegUsMbProtocol,               ///< Register US Mailbox Protocol
  RegUsMb2Msg,                   ///< Register US Mailbox 2 Message
  RegUsMb2Protocol,              ///< Register US Mailbox 2 Protocol
  RegSramMsgBlk,                 ///< Register SRAM Message Block
  RegGlobalCtl,                  ///< Register Global Control
  RegGlobalCtlSlaveAbyte,        ///< Register Global Control Slave ABYTE
  RegGlobalCtlSlaveDbyte,        ///< Register Global Control Slave DBYTE
  RegGlobalCtlSlaveAbit,         ///< Register Global Control Slave ABIT
  RegCalMisc2,                   ///< Register Cal Misc 2
  RegPllMemPs0,                  ///< Register PLL Mem Pstate 0
  RegPllMemPs1,                  ///< Register PLL Mem Pstate 1
  RegPmuClkDiv,                  ///< Register PMU Clock divider
  RegPllRegWaitTime,             ///< Register PllRegWaitTime
  RegPllLockTime,                ///< Register PllRegLockTime
  RegPhyCADCtl,                  ///< Register Phy CAD control
  RegMemResetCtl,                ///< Register Mem reset control
  RegDisCal,                     ///< Register Disable calibration
  RegCadTxSlewRate,              ///< Register CAD Tx Slew Rate
  RegDataTxSlewRate,             ///< Register DATA Tx Slew Rate
  RegAcsmCtrl13,                 ///< Register AcsmCtrl13
  RegX8DevIDCtl,                 ///< Register X8 DeviceID Control
  RegTxImpedance,                ///< Register for all TX impedance
  RegCadTxImpedance,             ///< Register CAD Tx Impedance
  RegDataTxImpedance,            ///< Register Data Tx Impedance
  RegDataRxImpedance,            ///< Register Data Rx Impedance
  RegAcsmOdtCtrl0,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl1,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl2,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl3,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl4,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl5,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl6,               ///< Register ACSM ODT Ctrl
  RegAcsmOdtCtrl7,               ///< Register ACSM ODT Ctrl
  RegVrefByteAbyte,              ///< Register Vref Byte
  RegVrefByteDbyte,              ///< Register Vref Byte
  RegVrefByteMaster,             ///< Register Vref Byte
  RegProcOdtTmg,                 ///< Register Proc ODT Timing
  RegPhyClkCtl,                  ///< Register Phy clock control
  RegTxDly,                      ///< Register TX delay
  RegDataRxDly,                  ///< Register Data Rx Delay
  RegDataTxDly,                  ///< Register Data Tx Delay
  RegDataTxEqHiImp,              ///< Register Data Tx EQ Hi Impedence
  RegDataTxEqLoImp,              ///< Register Data Tx EQ Lo Impedence
  RegDataTxEqBoostImp,           ///< Register Data Tx EQ Boost Impedence
  RegCadDllLockMaintenance,      ///< Register CAD DLL Lock Maintenance
  RegDataDllLockMaintenance,     ///< Register Data DLL Lock Maintenance
  RegDctPhyA78DllControl,        ///< Register DctPhyA78 DLL Control
  RegDctPhyD78DllControl,        ///< Register DctPhyD78 DLL Control
  RegDqDqsRxCtl,                 ///< Register DqDqs Rx Control
  RegVrefInGlobal,               ///< Register VrefInGlobal
  RegVrefOutGlobal0,             ///< Register VrefOutGlobal0
  RegVrefOutGlobal1,             ///< Register VrefOutGlobal1
  RegVrefOutGlobal2,             ///< Register VrefOutGlobal2
  RegVrefOutGlobal3,             ///< Register VrefOutGlobal3
  RegRdPtrOffsetCad0Group0,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad0Group1,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad0Group2,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad0Group3,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad1Group0,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad1Group1,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad1Group2,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad1Group3,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad2Group0,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad2Group1,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad2Group2,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad2Group3,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad3Group0,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad3Group1,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad3Group2,      ///< Register CAD RdPtrOffset
  RegRdPtrOffsetCad3Group3,      ///< Register CAD RdPtrOffset
  RegDataRdPtrOffset0,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset1,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset2,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset3,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset4,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset5,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset6,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset7,           ///< Register Data RdPtrOffset
  RegDataRdPtrOffset8,           ///< Register Data RdPtrOffset
  RegTxDlyCad0Group0,            ///< Register CAD TxDly
  RegTxDlyCad0Group1,            ///< Register CAD TxDly
  RegTxDlyCad0Group2,            ///< Register CAD TxDly
  RegTxDlyCad0Group3,            ///< Register CAD TxDly
  RegTxDlyCad1Group0,            ///< Register CAD TxDly
  RegTxDlyCad1Group1,            ///< Register CAD TxDly
  RegTxDlyCad1Group2,            ///< Register CAD TxDly
  RegTxDlyCad1Group3,            ///< Register CAD TxDly
  RegTxDlyCad2Group0,            ///< Register CAD TxDly
  RegTxDlyCad2Group1,            ///< Register CAD TxDly
  RegTxDlyCad2Group2,            ///< Register CAD TxDly
  RegTxDlyCad2Group3,            ///< Register CAD TxDly
  RegTxDlyCad3Group0,            ///< Register CAD TxDly
  RegTxDlyCad3Group1,            ///< Register CAD TxDly
  RegTxDlyCad3Group2,            ///< Register CAD TxDly
  RegTxDlyCad3Group3,            ///< Register CAD TxDly
  RegDllTuningCad0Group0,        ///< Register CAD Dll Tuning
  RegDllTuningCad0Group1,        ///< Register CAD Dll Tuning
  RegDllTuningCad0Group2,        ///< Register CAD Dll Tuning
  RegDllTuningCad1Group0,        ///< Register CAD Dll Tuning
  RegDllTuningCad1Group1,        ///< Register CAD Dll Tuning
  RegDllTuningCad1Group2,        ///< Register CAD Dll Tuning
  RegDllTuningCad2Group0,        ///< Register CAD Dll Tuning
  RegDllTuningCad2Group1,        ///< Register CAD Dll Tuning
  RegDllTuningCad2Group2,        ///< Register CAD Dll Tuning
  RegDllTuningCad3Group0,        ///< Register CAD Dll Tuning
  RegDllTuningCad3Group1,        ///< Register CAD Dll Tuning
  RegDllTuningCad3Group2,        ///< Register CAD Dll Tuning
  RegDataDllTuningCad0Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad0Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad0Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad1Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad1Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad1Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad2Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad2Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad2Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad3Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad3Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad3Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad4Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad4Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad4Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad5Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad5Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad5Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad6Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad6Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad6Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad7Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad7Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad7Group2,    ///< Register Data Dll Tuning
  RegDataDllTuningCad8Group0,    ///< Register Data Dll Tuning
  RegDataDllTuningCad8Group1,    ///< Register Data Dll Tuning
  RegDataDllTuningCad8Group2,    ///< Register Data Dll Tuning
  RegDataDqDqsRxCtl00,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl01,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl02,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl03,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl04,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl05,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl06,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl07,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl08,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl09,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl0a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl0b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl10,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl11,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl12,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl13,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl14,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl15,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl16,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl17,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl18,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl19,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl1a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl1b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl20,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl21,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl22,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl23,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl24,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl25,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl26,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl27,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl28,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl29,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl2a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl2b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl30,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl31,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl32,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl33,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl34,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl35,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl36,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl37,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl38,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl39,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl3a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl3b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl40,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl41,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl42,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl43,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl44,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl45,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl46,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl47,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl48,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl49,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl4a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl4b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl50,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl51,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl52,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl53,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl54,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl55,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl56,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl57,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl58,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl59,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl5a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl5b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl60,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl61,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl62,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl63,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl64,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl65,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl66,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl67,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl68,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl69,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl6a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl6b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl70,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl71,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl72,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl73,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl74,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl75,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl76,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl77,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl78,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl79,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl7a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl7b,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl80,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl81,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl82,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl83,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl84,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl85,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl86,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl87,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl88,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl89,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl8a,           ///< Register Data DqDqsRxCtl
  RegDataDqDqsRxCtl8b,           ///< Register Data DqDqsRxCtl
  RegDataRxDly000,               ///< Register Data RxDly
  RegDataRxDly001,               ///< Register Data RxDly
  RegDataRxDly002,               ///< Register Data RxDly
  RegDataRxDly003,               ///< Register Data RxDly
  RegDataRxDly010,               ///< Register Data RxDly
  RegDataRxDly011,               ///< Register Data RxDly
  RegDataRxDly012,               ///< Register Data RxDly
  RegDataRxDly013,               ///< Register Data RxDly
  RegDataRxDly020,               ///< Register Data RxDly
  RegDataRxDly021,               ///< Register Data RxDly
  RegDataRxDly022,               ///< Register Data RxDly
  RegDataRxDly023,               ///< Register Data RxDly
  RegDataRxDly030,               ///< Register Data RxDly
  RegDataRxDly031,               ///< Register Data RxDly
  RegDataRxDly032,               ///< Register Data RxDly
  RegDataRxDly033,               ///< Register Data RxDly
  RegDataRxDly100,               ///< Register Data RxDly
  RegDataRxDly101,               ///< Register Data RxDly
  RegDataRxDly102,               ///< Register Data RxDly
  RegDataRxDly103,               ///< Register Data RxDly
  RegDataRxDly110,               ///< Register Data RxDly
  RegDataRxDly111,               ///< Register Data RxDly
  RegDataRxDly112,               ///< Register Data RxDly
  RegDataRxDly113,               ///< Register Data RxDly
  RegDataRxDly120,               ///< Register Data RxDly
  RegDataRxDly121,               ///< Register Data RxDly
  RegDataRxDly122,               ///< Register Data RxDly
  RegDataRxDly123,               ///< Register Data RxDly
  RegDataRxDly130,               ///< Register Data RxDly
  RegDataRxDly131,               ///< Register Data RxDly
  RegDataRxDly132,               ///< Register Data RxDly
  RegDataRxDly133,               ///< Register Data RxDly
  RegDataRxDly200,               ///< Register Data RxDly
  RegDataRxDly201,               ///< Register Data RxDly
  RegDataRxDly202,               ///< Register Data RxDly
  RegDataRxDly203,               ///< Register Data RxDly
  RegDataRxDly210,               ///< Register Data RxDly
  RegDataRxDly211,               ///< Register Data RxDly
  RegDataRxDly212,               ///< Register Data RxDly
  RegDataRxDly213,               ///< Register Data RxDly
  RegDataRxDly220,               ///< Register Data RxDly
  RegDataRxDly221,               ///< Register Data RxDly
  RegDataRxDly222,               ///< Register Data RxDly
  RegDataRxDly223,               ///< Register Data RxDly
  RegDataRxDly230,               ///< Register Data RxDly
  RegDataRxDly231,               ///< Register Data RxDly
  RegDataRxDly232,               ///< Register Data RxDly
  RegDataRxDly233,               ///< Register Data RxDly
  RegDataRxDly300,               ///< Register Data RxDly
  RegDataRxDly301,               ///< Register Data RxDly
  RegDataRxDly302,               ///< Register Data RxDly
  RegDataRxDly303,               ///< Register Data RxDly
  RegDataRxDly310,               ///< Register Data RxDly
  RegDataRxDly311,               ///< Register Data RxDly
  RegDataRxDly312,               ///< Register Data RxDly
  RegDataRxDly313,               ///< Register Data RxDly
  RegDataRxDly320,               ///< Register Data RxDly
  RegDataRxDly321,               ///< Register Data RxDly
  RegDataRxDly322,               ///< Register Data RxDly
  RegDataRxDly323,               ///< Register Data RxDly
  RegDataRxDly330,               ///< Register Data RxDly
  RegDataRxDly331,               ///< Register Data RxDly
  RegDataRxDly332,               ///< Register Data RxDly
  RegDataRxDly333,               ///< Register Data RxDly
  RegDataRxDly400,               ///< Register Data RxDly
  RegDataRxDly401,               ///< Register Data RxDly
  RegDataRxDly402,               ///< Register Data RxDly
  RegDataRxDly403,               ///< Register Data RxDly
  RegDataRxDly410,               ///< Register Data RxDly
  RegDataRxDly411,               ///< Register Data RxDly
  RegDataRxDly412,               ///< Register Data RxDly
  RegDataRxDly413,               ///< Register Data RxDly
  RegDataRxDly420,               ///< Register Data RxDly
  RegDataRxDly421,               ///< Register Data RxDly
  RegDataRxDly422,               ///< Register Data RxDly
  RegDataRxDly423,               ///< Register Data RxDly
  RegDataRxDly430,               ///< Register Data RxDly
  RegDataRxDly431,               ///< Register Data RxDly
  RegDataRxDly432,               ///< Register Data RxDly
  RegDataRxDly433,               ///< Register Data RxDly
  RegDataRxDly500,               ///< Register Data RxDly
  RegDataRxDly501,               ///< Register Data RxDly
  RegDataRxDly502,               ///< Register Data RxDly
  RegDataRxDly503,               ///< Register Data RxDly
  RegDataRxDly510,               ///< Register Data RxDly
  RegDataRxDly511,               ///< Register Data RxDly
  RegDataRxDly512,               ///< Register Data RxDly
  RegDataRxDly513,               ///< Register Data RxDly
  RegDataRxDly520,               ///< Register Data RxDly
  RegDataRxDly521,               ///< Register Data RxDly
  RegDataRxDly522,               ///< Register Data RxDly
  RegDataRxDly523,               ///< Register Data RxDly
  RegDataRxDly530,               ///< Register Data RxDly
  RegDataRxDly531,               ///< Register Data RxDly
  RegDataRxDly532,               ///< Register Data RxDly
  RegDataRxDly533,               ///< Register Data RxDly
  RegDataRxDly600,               ///< Register Data RxDly
  RegDataRxDly601,               ///< Register Data RxDly
  RegDataRxDly602,               ///< Register Data RxDly
  RegDataRxDly603,               ///< Register Data RxDly
  RegDataRxDly610,               ///< Register Data RxDly
  RegDataRxDly611,               ///< Register Data RxDly
  RegDataRxDly612,               ///< Register Data RxDly
  RegDataRxDly613,               ///< Register Data RxDly
  RegDataRxDly620,               ///< Register Data RxDly
  RegDataRxDly621,               ///< Register Data RxDly
  RegDataRxDly622,               ///< Register Data RxDly
  RegDataRxDly623,               ///< Register Data RxDly
  RegDataRxDly630,               ///< Register Data RxDly
  RegDataRxDly631,               ///< Register Data RxDly
  RegDataRxDly632,               ///< Register Data RxDly
  RegDataRxDly633,               ///< Register Data RxDly
  RegDataRxDly700,               ///< Register Data RxDly
  RegDataRxDly701,               ///< Register Data RxDly
  RegDataRxDly702,               ///< Register Data RxDly
  RegDataRxDly703,               ///< Register Data RxDly
  RegDataRxDly710,               ///< Register Data RxDly
  RegDataRxDly711,               ///< Register Data RxDly
  RegDataRxDly712,               ///< Register Data RxDly
  RegDataRxDly713,               ///< Register Data RxDly
  RegDataRxDly720,               ///< Register Data RxDly
  RegDataRxDly721,               ///< Register Data RxDly
  RegDataRxDly722,               ///< Register Data RxDly
  RegDataRxDly723,               ///< Register Data RxDly
  RegDataRxDly730,               ///< Register Data RxDly
  RegDataRxDly731,               ///< Register Data RxDly
  RegDataRxDly732,               ///< Register Data RxDly
  RegDataRxDly733,               ///< Register Data RxDly
  RegDataRxDly800,               ///< Register Data RxDly
  RegDataRxDly801,               ///< Register Data RxDly
  RegDataRxDly802,               ///< Register Data RxDly
  RegDataRxDly803,               ///< Register Data RxDly
  RegDataRxDly810,               ///< Register Data RxDly
  RegDataRxDly811,               ///< Register Data RxDly
  RegDataRxDly812,               ///< Register Data RxDly
  RegDataRxDly813,               ///< Register Data RxDly
  RegDataRxDly820,               ///< Register Data RxDly
  RegDataRxDly821,               ///< Register Data RxDly
  RegDataRxDly822,               ///< Register Data RxDly
  RegDataRxDly823,               ///< Register Data RxDly
  RegDataRxDly830,               ///< Register Data RxDly
  RegDataRxDly831,               ///< Register Data RxDly
  RegDataRxDly832,               ///< Register Data RxDly
  RegDataRxDly833,               ///< Register Data RxDly
  RegDataTxDly000,               ///< Register Data TxDly
  RegDataTxDly001,               ///< Register Data TxDly
  RegDataTxDly002,               ///< Register Data TxDly
  RegDataTxDly003,               ///< Register Data TxDly
  RegDataTxDly010,               ///< Register Data TxDly
  RegDataTxDly011,               ///< Register Data TxDly
  RegDataTxDly012,               ///< Register Data TxDly
  RegDataTxDly013,               ///< Register Data TxDly
  RegDataTxDly020,               ///< Register Data TxDly
  RegDataTxDly021,               ///< Register Data TxDly
  RegDataTxDly022,               ///< Register Data TxDly
  RegDataTxDly023,               ///< Register Data TxDly
  RegDataTxDly030,               ///< Register Data TxDly
  RegDataTxDly031,               ///< Register Data TxDly
  RegDataTxDly032,               ///< Register Data TxDly
  RegDataTxDly033,               ///< Register Data TxDly
  RegDataTxDly100,               ///< Register Data TxDly
  RegDataTxDly101,               ///< Register Data TxDly
  RegDataTxDly102,               ///< Register Data TxDly
  RegDataTxDly103,               ///< Register Data TxDly
  RegDataTxDly110,               ///< Register Data TxDly
  RegDataTxDly111,               ///< Register Data TxDly
  RegDataTxDly112,               ///< Register Data TxDly
  RegDataTxDly113,               ///< Register Data TxDly
  RegDataTxDly120,               ///< Register Data TxDly
  RegDataTxDly121,               ///< Register Data TxDly
  RegDataTxDly122,               ///< Register Data TxDly
  RegDataTxDly123,               ///< Register Data TxDly
  RegDataTxDly130,               ///< Register Data TxDly
  RegDataTxDly131,               ///< Register Data TxDly
  RegDataTxDly132,               ///< Register Data TxDly
  RegDataTxDly133,               ///< Register Data TxDly
  RegDataTxDly200,               ///< Register Data TxDly
  RegDataTxDly201,               ///< Register Data TxDly
  RegDataTxDly202,               ///< Register Data TxDly
  RegDataTxDly203,               ///< Register Data TxDly
  RegDataTxDly210,               ///< Register Data TxDly
  RegDataTxDly211,               ///< Register Data TxDly
  RegDataTxDly212,               ///< Register Data TxDly
  RegDataTxDly213,               ///< Register Data TxDly
  RegDataTxDly220,               ///< Register Data TxDly
  RegDataTxDly221,               ///< Register Data TxDly
  RegDataTxDly222,               ///< Register Data TxDly
  RegDataTxDly223,               ///< Register Data TxDly
  RegDataTxDly230,               ///< Register Data TxDly
  RegDataTxDly231,               ///< Register Data TxDly
  RegDataTxDly232,               ///< Register Data TxDly
  RegDataTxDly233,               ///< Register Data TxDly
  RegDataTxDly300,               ///< Register Data TxDly
  RegDataTxDly301,               ///< Register Data TxDly
  RegDataTxDly302,               ///< Register Data TxDly
  RegDataTxDly303,               ///< Register Data TxDly
  RegDataTxDly310,               ///< Register Data TxDly
  RegDataTxDly311,               ///< Register Data TxDly
  RegDataTxDly312,               ///< Register Data TxDly
  RegDataTxDly313,               ///< Register Data TxDly
  RegDataTxDly320,               ///< Register Data TxDly
  RegDataTxDly321,               ///< Register Data TxDly
  RegDataTxDly322,               ///< Register Data TxDly
  RegDataTxDly323,               ///< Register Data TxDly
  RegDataTxDly330,               ///< Register Data TxDly
  RegDataTxDly331,               ///< Register Data TxDly
  RegDataTxDly332,               ///< Register Data TxDly
  RegDataTxDly333,               ///< Register Data TxDly
  RegDataTxDly400,               ///< Register Data TxDly
  RegDataTxDly401,               ///< Register Data TxDly
  RegDataTxDly402,               ///< Register Data TxDly
  RegDataTxDly403,               ///< Register Data TxDly
  RegDataTxDly410,               ///< Register Data TxDly
  RegDataTxDly411,               ///< Register Data TxDly
  RegDataTxDly412,               ///< Register Data TxDly
  RegDataTxDly413,               ///< Register Data TxDly
  RegDataTxDly420,               ///< Register Data TxDly
  RegDataTxDly421,               ///< Register Data TxDly
  RegDataTxDly422,               ///< Register Data TxDly
  RegDataTxDly423,               ///< Register Data TxDly
  RegDataTxDly430,               ///< Register Data TxDly
  RegDataTxDly431,               ///< Register Data TxDly
  RegDataTxDly432,               ///< Register Data TxDly
  RegDataTxDly433,               ///< Register Data TxDly
  RegDataTxDly500,               ///< Register Data TxDly
  RegDataTxDly501,               ///< Register Data TxDly
  RegDataTxDly502,               ///< Register Data TxDly
  RegDataTxDly503,               ///< Register Data TxDly
  RegDataTxDly510,               ///< Register Data TxDly
  RegDataTxDly511,               ///< Register Data TxDly
  RegDataTxDly512,               ///< Register Data TxDly
  RegDataTxDly513,               ///< Register Data TxDly
  RegDataTxDly520,               ///< Register Data TxDly
  RegDataTxDly521,               ///< Register Data TxDly
  RegDataTxDly522,               ///< Register Data TxDly
  RegDataTxDly523,               ///< Register Data TxDly
  RegDataTxDly530,               ///< Register Data TxDly
  RegDataTxDly531,               ///< Register Data TxDly
  RegDataTxDly532,               ///< Register Data TxDly
  RegDataTxDly533,               ///< Register Data TxDly
  RegDataTxDly600,               ///< Register Data TxDly
  RegDataTxDly601,               ///< Register Data TxDly
  RegDataTxDly602,               ///< Register Data TxDly
  RegDataTxDly603,               ///< Register Data TxDly
  RegDataTxDly610,               ///< Register Data TxDly
  RegDataTxDly611,               ///< Register Data TxDly
  RegDataTxDly612,               ///< Register Data TxDly
  RegDataTxDly613,               ///< Register Data TxDly
  RegDataTxDly620,               ///< Register Data TxDly
  RegDataTxDly621,               ///< Register Data TxDly
  RegDataTxDly622,               ///< Register Data TxDly
  RegDataTxDly623,               ///< Register Data TxDly
  RegDataTxDly630,               ///< Register Data TxDly
  RegDataTxDly631,               ///< Register Data TxDly
  RegDataTxDly632,               ///< Register Data TxDly
  RegDataTxDly633,               ///< Register Data TxDly
  RegDataTxDly700,               ///< Register Data TxDly
  RegDataTxDly701,               ///< Register Data TxDly
  RegDataTxDly702,               ///< Register Data TxDly
  RegDataTxDly703,               ///< Register Data TxDly
  RegDataTxDly710,               ///< Register Data TxDly
  RegDataTxDly711,               ///< Register Data TxDly
  RegDataTxDly712,               ///< Register Data TxDly
  RegDataTxDly713,               ///< Register Data TxDly
  RegDataTxDly720,               ///< Register Data TxDly
  RegDataTxDly721,               ///< Register Data TxDly
  RegDataTxDly722,               ///< Register Data TxDly
  RegDataTxDly723,               ///< Register Data TxDly
  RegDataTxDly730,               ///< Register Data TxDly
  RegDataTxDly731,               ///< Register Data TxDly
  RegDataTxDly732,               ///< Register Data TxDly
  RegDataTxDly733,               ///< Register Data TxDly
  RegDataTxDly800,               ///< Register Data TxDly
  RegDataTxDly801,               ///< Register Data TxDly
  RegDataTxDly802,               ///< Register Data TxDly
  RegDataTxDly803,               ///< Register Data TxDly
  RegDataTxDly810,               ///< Register Data TxDly
  RegDataTxDly811,               ///< Register Data TxDly
  RegDataTxDly812,               ///< Register Data TxDly
  RegDataTxDly813,               ///< Register Data TxDly
  RegDataTxDly820,               ///< Register Data TxDly
  RegDataTxDly821,               ///< Register Data TxDly
  RegDataTxDly822,               ///< Register Data TxDly
  RegDataTxDly823,               ///< Register Data TxDly
  RegDataTxDly830,               ///< Register Data TxDly
  RegDataTxDly831,               ///< Register Data TxDly
  RegDataTxDly832,               ///< Register Data TxDly
  RegDataTxDly833,               ///< Register Data TxDly
  RegClkTreeTermCntrMstr,        ///< Register ClkTreeTermCntrMstr
  RegDctPhy0x01F1F045,           ///< Register RegDctPhy 0x01F1F045
  RegCalRate,                    ///< Register CalRate
  RegCalBusy,                    ///< Register CalBusy
  RegFAF04A,                     ///< Register 0x00FAF04A
  RegLaneToCrcMap0,              ///< Register Lane To CRC Map0
  RegLaneToCrcMap1,              ///< Register Lane To CRC Map1
  RegDctPhy0x01F1F043,           ///< Register RegDctPhy 0x01F1F043
  RegDctPhy0x00F0F04E,           ///< Abyte Tx Control DQ
  RegTxControlDq,                ///< Register TxControlDq
  RegCadByteDbgCtrl,             ///< Register Abyte ByteDbgCtrl
  RegDataByteDbgCtrl,            ///< Register Dbyte ByteDbgCtrl
  RegMasterByteDbgCtrl,          ///< Register Master ByteDbgCtrl
  RegMasterD3EvntMerr,           ///< Register Master D3_EvntMerr
  RegDataDqDqsRcvCntrl3,         ///< Register Dbyte DqDqsRcvCntrl3
  RegDataRcvMajorMode,           ///< Register Dbyte DataRcvMajorMode
  RegDctPhyD0091F04A,            ///< Unused Register Dbyte x0091_F04A
  RegDctPhyD00910F77,            ///< Unused Register Dbyte x0091_0F77
  RegDctPhyD00910000,            ///< Unused Register Dbyte x0091_0000
  RegDctPhyD0391F04D,            ///< Unused Register Dbyte x0391_F04D
  RegDctPhyD0391F041,            ///< Unused Register Dbyte x0391_F041
  RegDctPhyD00F1804A,            ///< Unused Register Dbyte x00F1_804A
  RegDctPhyD00F1904A,            ///< Unused Register Dbyte x00F1_904A
  RegDctPhyD00F1B04A,            ///< Unused Register Dbyte x00F1_B04A
  RegCadTxImpedance00,           ///< Register Cad Tx Impedance
  RegCadTxImpedance01,           ///< Register Cad Tx Impedance
  RegCadTxImpedance02,           ///< Register Cad Tx Impedance
  RegCadTxImpedance03,           ///< Register Cad Tx Impedance
  RegCadTxImpedance04,           ///< Register Cad Tx Impedance
  RegCadTxImpedance05,           ///< Register Cad Tx Impedance
  RegCadTxImpedance06,           ///< Register Cad Tx Impedance
  RegCadTxImpedance07,           ///< Register Cad Tx Impedance
  RegCadTxImpedance08,           ///< Register Cad Tx Impedance
  RegCadTxImpedance09,           ///< Register Cad Tx Impedance
  RegCadTxImpedance0a,           ///< Register Cad Tx Impedance
  RegCadTxImpedance0b,           ///< Register Cad Tx Impedance
  RegCadTxImpedance10,           ///< Register Cad Tx Impedance
  RegCadTxImpedance11,           ///< Register Cad Tx Impedance
  RegCadTxImpedance12,           ///< Register Cad Tx Impedance
  RegCadTxImpedance13,           ///< Register Cad Tx Impedance
  RegCadTxImpedance14,           ///< Register Cad Tx Impedance
  RegCadTxImpedance15,           ///< Register Cad Tx Impedance
  RegCadTxImpedance16,           ///< Register Cad Tx Impedance
  RegCadTxImpedance17,           ///< Register Cad Tx Impedance
  RegCadTxImpedance18,           ///< Register Cad Tx Impedance
  RegCadTxImpedance19,           ///< Register Cad Tx Impedance
  RegCadTxImpedance1a,           ///< Register Cad Tx Impedance
  RegCadTxImpedance1b,           ///< Register Cad Tx Impedance
  RegCadTxImpedance20,           ///< Register Cad Tx Impedance
  RegCadTxImpedance21,           ///< Register Cad Tx Impedance
  RegCadTxImpedance22,           ///< Register Cad Tx Impedance
  RegCadTxImpedance23,           ///< Register Cad Tx Impedance
  RegCadTxImpedance24,           ///< Register Cad Tx Impedance
  RegCadTxImpedance25,           ///< Register Cad Tx Impedance
  RegCadTxImpedance26,           ///< Register Cad Tx Impedance
  RegCadTxImpedance27,           ///< Register Cad Tx Impedance
  RegCadTxImpedance28,           ///< Register Cad Tx Impedance
  RegCadTxImpedance29,           ///< Register Cad Tx Impedance
  RegCadTxImpedance2a,           ///< Register Cad Tx Impedance
  RegCadTxImpedance2b,           ///< Register Cad Tx Impedance
  RegCadTxImpedance30,           ///< Register Cad Tx Impedance
  RegCadTxImpedance31,           ///< Register Cad Tx Impedance
  RegCadTxImpedance32,           ///< Register Cad Tx Impedance
  RegCadTxImpedance33,           ///< Register Cad Tx Impedance
  RegCadTxImpedance34,           ///< Register Cad Tx Impedance
  RegCadTxImpedance35,           ///< Register Cad Tx Impedance
  RegCadTxImpedance36,           ///< Register Cad Tx Impedance
  RegCadTxImpedance37,           ///< Register Cad Tx Impedance
  RegCadTxImpedance38,           ///< Register Cad Tx Impedance
  RegCadTxImpedance39,           ///< Register Cad Tx Impedance
  RegCadTxImpedance3a,           ///< Register Cad Tx Impedance
  RegCadTxImpedance3b,           ///< Register Cad Tx Impedance
  RegDctPhy0x00F1F04E,           ///< Dbyte Tx Control DQ
  DctPhyRegRangeEnd,             ///< -------------------------- End of DCT_PHY_REG range

  RegIdLimit                     ///< Total number of register identifiers

};  ///< REG_BF_NAME

/// Bit field location
typedef struct {
  UINT32  LoBit: 6;         ///< Low bit of the bit field
  UINT32  HiBit: 6;         ///< High bit of the bit field
  UINT32  RegIndex: 12;     ///< Register that the bit field is on
  UINT32  Reserved: 7;      ///< Reserved
  UINT32  Linked: 1;        ///< 1: The bit field has high bits defined in the very next Bf identifier
  ///< 0: The bit field has no further extension
} BF_LOCATION;

/**
  REG_DEF(TableName, RegIndex, Addr)

  @param[in]    TableName
  @param[in]    RegIndex
  @param[in]    Addr

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define REG_DEF(TableName, RegIndex, Addr) \
  TableName[RegIndex] = Addr

/**
  _BF_DEF(TableName, RegIndex, BfIndex, Hi, Lo)

  @param[in]    TableName
  @param[in]    RegIndex
  @param[in]    BfIndex
  @param[in]    Hi
  @param[in]    Lo

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define _BF_DEF(TableName, RegIndex, BfIndex, Hi, Lo) \
  TableName[BfIndex] = ( \
    ((UINT32) RegIndex << 12) | ( \
      (Hi > Lo) ? (\
        (((UINT32) Hi << 6) | (UINT32) Lo) \
      ) : ( \
        (((UINT32) Lo << 6) | (UINT32) Hi) \
      ) \
    ) \
  )

/**
  LINK_BF(TableName, LowerBfIndex, HigherBfIndex):
  This is one way link: any write to LowerBfIndex would write to HigherBfIndex,
                        but NOT the other way around.
  Requirement: LowerBfIndex must be declared *right* before HigherBfIndex.

  @param[in]    TableName
  @param[in]    LowerBfIndex
  @param[in]    HigherBfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define LINK_BF(TableName, LowerBfIndex, HigherBfIndex) { \
  ASSERT ((LowerBfIndex < BFEndOfList) || (LowerBfIndex > RegIdLimit)); \
  ASSERT ((HigherBfIndex < BFEndOfList) || (HigherBfIndex > RegIdLimit)); \
  ASSERT (LowerBfIndex == (HigherBfIndex - 1)) ; \
  TableName[LowerBfIndex] |= ((UINT32) 1) << 31; \
}

/*----------------------------------------------------------------------------
 *                           CHIP TO PAD TRANSLATION
 *
 *----------------------------------------------------------------------------
 */

/// Pad group names
typedef enum {
  PAD_ALL = 0,  ///< broadcast to all pads
  PAD_CKE,      ///< pad CKE
  PAD_ADDR_CMD, ///< pad ADDR_CMD
  PAD_CLK,      ///< pad CLK
  PAD_CS,       ///< pad CS
  PAD_ODT,      ///< pad ODT
  PAD_DQ_L,     ///< pad DQ_L
  PAD_DQ_H,     ///< pad DQ_H
  PAD_DQS_DM,   ///< pad DQS_DM
  PAD_DQS_H,    ///< pad DQS_H
  PAD_PROC_ODT, ///< pad PROC_ODT
  PAD_DQ,       ///< pad DQ
  PAD_DQS,      ///< pad DQS
} PAD_NAME;

#define  MEMCKE0    PAD_CKE
#define  MEMADD0    PAD_ADDR_CMD
#define  MEMBANK0   PAD_ADDR_CMD
#define  MEMPAR0    PAD_ADDR_CMD
#define  MEMCLK0_L  PAD_CLK
#define  MEMCLK0_H  PAD_CLK
#define  MEMCS0_L   PAD_CS
#define  MEMCAS0_L  PAD_ADDR_CMD
#define  MEMWE0_L   PAD_ADDR_CMD
#define  MEMRAS0_L  PAD_ADDR_CMD
#define  MEMODT0    PAD_ODT
#define  MEMDQ_L    PAD_DQ_L
#define  MEMDQ_H    PAD_DQ_H
#define  MEMDQSDM   PAD_DQS_DM
#define  MEMDQS_H   PAD_DQS_H
#define  MEMDQ      PAD_DQ
#define  MEMDQS     PAD_DQS

/// DDR Chip to pad entry
typedef struct {
  UINT16 PadName: 4;   ///< Pad name
  UINT16 Instance: 4;  ///< Pad instance
  UINT16 Chiplet: 4;   ///< Chiplet number
  UINT8  Group: 4;     ///< Group number
  UINT8  PadNum: 4;    ///< Pad number
} DDR_CHIP_TO_PAD_ENTRY;

/*----------------------------------------------------------------------------
 *                           BIT FIELD TOKEN ENCODING
 *
 *----------------------------------------------------------------------------
 */

/// Sub-structure used to parse a BF token
typedef struct {
  UINT32  Index: 12;       ///< Index into NBRegTable table
  UINT32  Reserved: 3;     ///< Reserved
  UINT32  Direct: 1;       ///< Access the address directly. No BitField decoding as needed.
  UINT32  PadName: 5;      ///< ie CKE, ODT, ADDR,...
  UINT32  PadIdxPlus1: 5;  ///< Pad instance of a certain type
  UINT32  DLNPlus1: 4;     ///< DIMM/DLL/NBPs number
  UINT32  MemPsPlus1: 2;   ///< MemPs number
} BF_TOKEN_PARSE;

/// Bit field name encoding
typedef union {
  BIT_FIELD_NAME  FieldName; ///< BF token
  BF_TOKEN_PARSE  Parse;     ///< BF token parse
} BF_NAME_ENC;

/**
  PER_INSTANCE(BfIndex)
  Specifies that set function to only per instance.

  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_INSTANCE(BfIndex) ((BfIndex) | (1 << 15))

/**
  MEM_PS_PER_INSTANCE(p, RegAddress)
  Specifies that set function to Mem Pstate p of register address

  @param[in]    p
  @param[in]    RegAddress

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define MEM_PS_PER_INSTANCE(p, RegAddress) ((RegAddress) | (((p) & 0x3) << 24))

/**
  SW_BC_DIS(BfIndex)
  Specifies that set function to not attempt SW broadcast

  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define SW_BC_DIS(BfIndex) ((BfIndex) | (0xFFFF0000ul))

/**
  PER_MEM_PS(p, BfIndex)
  Specifies that set function to only broadcast to Mem Pstate p of bit field BfIndex

  @param[in]    p
  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_MEM_PS(p, BfIndex) ((BfIndex) | ((UINT32) (((p) + 1) & 0x3) << 30))
#define ALL_MEM_PS  2

/**
  PER_NB_PS(p, BfIndex)
  Specifies that set function to only broadcast to NB Pstate p of bit field BfIndex

  @param[in]    p
  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_NB_PS(p, BfIndex) ((BfIndex) | ((UINT32) (((p) + 1) & 0xF) << 26))
#define NB_PS_PMU   8
#define ALL_NB_PS   0xFF

/**
  PER_DIMM(p, BfIndex)
  Specifies that set function to only broadcast to NB Pstate p of bit field BfIndex

  @param[in]    d
  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_DIMM(d, BfIndex) ((BfIndex) | ((UINT32) (((d) + 1) & 0xF) << 26))
#ifndef ALL_DIMMS
  #define ALL_DIMMS  0xFF
#endif


/**
  PER_CAD_PAD(t, i, BfIndex)
  Specifies that set function to only broadcast a certain pad (ie MEMCKE[3])

  @param[in]    t - pad type (ie MEMCKE)
  @param[in]    i - pad index (ie 3)
  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_CAD_PAD(t, i, BfIndex) ((BfIndex) | ((UINT32) (t) << 16) | ((UINT32) (((i) + 1) & 0x1F) << 21))
#define ALL_PADS   0xFF

/**
  PER_DATA_BYTE(t, b, BfIndex)
  Specifies that set function to only broadcast a certain data pad (ie DQ byte 3)

  @param[in]    t - data pin type (ie DQ)
  @param[in]    b - data byte (ie byte 3)
  @param[in]    BfIndex

  @return   REG_BF_ENC  Access params encrypted in REG_BF_ENC format.
--*/
#define PER_DATA_BYTE(t, b, BfIndex) ((BfIndex) | ((UINT32) (t) << 16) | ((UINT32) (((b) + 1) & 0x1F) << 21))
#define ECC_BYTE  8
#define UNUSED_BYTE  9
#define ALL_BYTES 0xFF

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */

#endif  /* _MNREG_H_ */

