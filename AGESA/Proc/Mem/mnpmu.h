/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmu.h
 *
 * Definitions for PMU support
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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

#ifndef _MNPMU_H_
#define _MNPMU_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

// Maximum PMU SRAM size for DMEM/IMEM
#define PMU_SRAM_DMEM_SIZE_MAX      0x1800  ///< PMU DMEM size is 6K Bytes
#define PMU_SRAM_IMEM_SIZE_MAX      0x3800  ///< PMU IMEM size is 14K Bytes

#define PMU_IMAGE_NOT_USEABLE    0x0000  ///< The Image is not valid or useable

// Upstream mailbox 1 messages
#define USMSG_DEVINIT      0x00  ///< PMU has completed DevInit
#define USMSG_TRAINWRLVL   0x01  ///< PMU has completed TSTAGE_WrLvl
#define USMSG_TRAINRXEN    0x02  ///< PMU has completed TSTAGE_RxEn
#define USMSG_TRAINRDDQS1D 0x03  ///< PMU has completed TSTAGE_TrainRdDqs1D
#define USMSG_TRAINWRDQ1D  0x04  ///< PMU has completed TSTAGE_TrainWrDq1D
#define USMSG_TRAINRD2D    0x05  ///< PMU has completed TSTAGE_TrainRd2D
#define USMSG_TRAINWR2D    0x06  ///< PMU has completed TSTAGE_TrainWr2D
#define USMSG_PMUQEMPTY    0x07  ///< PMU has completed all of its SequenceCtl tasks and is in a powergated idle state
#define USMSG_US2MSGRDY    0x08  ///< PMU is ready to stream a message through US mailbox 2
#define USMSG_FAIL         0xFF  ///< PMU has encountered an error which requires requester to abort waiting for remaining pending upstream messages

#define DCT_OFFSET_AUTO_INC_EN  ((UINT32) 1 << 29)
#define PMU_FIRMWARE_SRAM_START 0x00050000ul
#define PMU_SRAM_DMEM_OFFSET    0x0000  ///< PMU SRAM DMEM offset
#define PMU_SRAM_IMEM_OFFSET    0x4000  ///< PMU SRAM IMEM offset

// AMD PMU Signature.
#define PMU_SIGNATURE           {'!', '!', '!', 'P', 'M', 'U'}   ///< PMU Signature with 6 characters limited.

/// AMD PMU Header
typedef struct {
  CHAR8   Signature[6];                       ///< PMU signature
  CHAR8   Instance;                           ///< PMU instance
  CHAR8   PackageName[16];                    ///< Package string
  CHAR8   TerminatorNull;                     ///< null terminated string
  CHAR8   VerReserved[7];                     ///< reserved space
} AMD_PMU_HEADER;

// PMU firmware Code/Data can not be fragmented.  The offset for Data must
// start at 0x0000 and Code must start at 0x4000.
/// The Option of PMU firmware
typedef struct {
  AMD_PMU_HEADER VersionString;                                 ///< PMU embedded signature
  UINT32  SramSize;                                             ///< Total size of PMU SRAM, included DMEM/IMEM.
  UINT16  SramDataImage[PMU_SRAM_DMEM_SIZE_MAX / sizeof (UINT16)];  ///< PMU SRAM Data Image
  UINT16  SramCodeImage[PMU_SRAM_IMEM_SIZE_MAX / sizeof (UINT16)];  ///< PMU SRAM Code Image
} PMU_FIRMWARE_D3;

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */
/// Fields on PMU SRAM
typedef enum {
  PmuSRAMRangeStart = RegIdLimit, ///< -------------------------- Start of PMU SRAM range

  // DDR3 SRAM message block
  PmuRevision,                    ///< Revision         Table revision
  PmuFirmwareRev,                 ///< PMU firmware revision
  PmuCpuId,                       ///< CpuId            32-bit CPUID
  PmuCpuIdHi,                     ///< CpuId            32-bit CPUID
  PmuDramType,                    ///< DramType
  PmuModuleType,                  ///< ModuleType
  PmuRawCard0,                    ///< RawCard0
  PmuRawCard1,                    ///< RawCard1
  PmuRawCard2,                    ///< RawCard2
  PmuRawCard3,                    ///< RawCard3
  PmuChipSelect,                  ///< ChipSelect       Specifies which chipselects are present and should be trained. Bit 0 is chip select 0, Bit 1 is
                                  ///<                  chip,select 1, etc. IF Gddr5Mode then ChipSelect[7:1] are ignored.
  PmuAddrMirror,                  ///< AddrMirror       For each chipselect, specifies which chipselects have address mirroring (BA0 swapped with BA1,
                                  ///<                  A3,swapped with A4, A5 swapped with A6, A7 swapped with A8). This field is used in Ddr3Mode.
  PmuDimm0Cols,                   ///< Dimm0Cols        Number of device column address bits for Dimm0.
  PmuDimm1Cols,                   ///< Dimm1Cols        Number of device column address bits for Dimm1.
  PmuDimm2Cols,                   ///< Dimm2Cols        Number of device column address bits for Dimm2.
  PmuDimm3Cols,                   ///< Dimm3Cols        Number of device column address bits for Dimm3.
  PmuDimm0Banks,                  ///< Dimm0Banks       Number of device bank address bits for Dimm0.
  PmuDimm1Banks,                  ///< Dimm1Banks       Number of device bank address bits for Dimm1.
  PmuDimm2Banks,                  ///< Dimm2Banks       Number of device bank address bits for Dimm2.
  PmuDimm3Banks,                  ///< Dimm3Banks       Number of device bank address bits for Dimm3.
  PmuDimm0Rows,                   ///< Dimm0Rows        Number of device row address bits for Dimm0.
  PmuDimm1Rows,                   ///< Dimm1Rows        Number of device row address bits for Dimm1.
  PmuDimm2Rows,                   ///< Dimm2Rows        Number of device row address bits for Dimm2.
  PmuDimm3Rows,                   ///< Dimm3Rows        Number of device row address bits for Dimm3.
  PmuTestFail,                    ///< TestFail         Specifies which chipselects (if any) have failed training.
                                  ///<                  Bit 0 is chipselect 0, Bit 1 is chipselect 1, and etc...
  PmuPerRankTiming,               ///< PerRankTiming    1=The channel is configured to use four timing sources for four independent chipselects. 0=The
                                  ///<                  channel,is configured to use four timing sources for four pairs of chipselects (logical DIMMs).
  PmuCurrentTemp,                 ///< CurrentTemp      11-bit temperature in degrees C, with -49C offset, in 0.125C increments, unsigned. See
                                  ///<                  D18F5xA8_x383[GblMaxTemp] in,TN BKDG
  PmuSequenceCtl,                 ///< SequenceCtl      PMU Sequence Control Word
                                  ///<                  [0] 1=DevInit
                                  ///<                  [1] 1=RxEnDly,Training
                                  ///<                  [2] 1=1D,Rd-Dqs Training
                                  ///<                  [3] 1=1D,Wr-Dq Training
                                  ///<                  [4] 1=2D,Read Training
                                  ///<                  [5] 1=2D,Write Training
                                  ///<                  [15:6] Reserved
  PmuHdtCtl,                      ///< HdtCtl
  PmuCkeSetup,                    ///< CkeSetup
  PmuCsOdtSetup,                  ///< CsOdtSetup
  PmuAddrCmdSetup,                ///< AddrCmdSetup
  PmuSlowAccessMode,              ///< SlowAccessMode   SlowAccessMode
  PmutRP,                         ///< tRP              In units of memclocks. See D18F2x200_dct[3:0]_mp[1:0][Trp] for definition.
  PmutMRD,                        ///< tMRD             In units of memclocks. See D18F2x220_dct[3:0][Tmrd] for definition.
  PmutRFC,                        ///< tRFC             In units of memclocks. The maximum value for all DIMMs. See D18F2x208_dct[3:0][Trfc].
  PmuMR0,                         ///< MR0
  PmuMR1,                         ///< MR1
  PmuMR2,                         ///< MR2
  PmuCD_R_W,                      ///< CD_R_W           Command delay, read to write, any chip select to any other chipselect. Units are memclocks, unsigned integer.
  PmuCD_R_R,                      ///< CD_R_R           Command delay, read to read, any chip select to any other chipselect. Units are memclocks, unsigned integer.
  PmuCD_W_W,                      ///< CD_W_W           Command delay, write to write, any chip select to any other chipselect. Units are memclocks, unsigned integer.
  PmuCD_W_R,                      ///< CD_W_R           Command delay, write to read, any chip select to any other chipselect. Units are memclocks, unsigned integer.
  PmuCD_R_R_SD,                   ///< CD_R_R_SD        Command delay, read to read, any chip select to another chipselect of same DIMM. Units are memclocks, unsigned integer.
  PmuCD_W_W_SD,                   ///< CD_W_W_SD        Command delay, write to write, any chip select to another chipselect of same DIMM. Units are memclocks, unsigned integer.
  PmuTrdrdban_Phy,                ///< Trdrdban_Phy     In memclocks.

  //  GDDR5 SRAM message block
  PmuCh0Dis,                      ///< Ch0Dis           1=Disable the lower 32-bits. Used for GDDR5 mode only.
  PmuCh1Dis,                      ///< Ch1Dis           1=Disable the upper 32-bits. Used for GDDR5 mode only.
  PmuInitACTerm,                  ///< InitACTerm       1=Addr/Cmd term is ZQ. Addr/Cmd term is ZQ/2. Used during DevInit1, for GDDR5 mode only.
  PmuMR3,                         ///< MR3
  PmuMR4,                         ///< MR4
  PmuMR5,                         ///< MR5
  PmuMR6,                         ///< MR6
  PmuMR7,                         ///< MR7
  PmuMR8,                         ///< MR8
  PmuMR15,                        ///< MR15
  PmuMRnum,                       ///< MRnum            SendMrs Specifies the MR number for a single MRS command. Write data is one of MR0 to MR15 in this table.
                                  ///<                  is, chip select 1, etc..IF Gddr5Mode then ChipSelect[7:1] are ignored.

  PmuSRAMRangeEnd                 ///< -------------------------- End of PMU SRAM range
};  ///< REG_BF_NAME

/// PMU SRAM field location
typedef struct {
  UINT32  LoBit: 6;         ///< Low bit of the bit field
  UINT32  HiBit: 6;         ///< High bit of the bit field
  UINT32  RegOffset: 10;    ///< Register that the bit field is on
  UINT32  Reserved: 9;      ///< Reserved
  UINT32  Linked: 1;        ///< 1: The bit field has high bits defined in the very next Bf identifier
  ///< 0: The bit field has no further extension
} PMU_SRAM_ADDR;

/**
  PMU_SRAM (TableName, BfIndex, RegOffset, Hi, Lo)

  @param[in]    TableName
  @param[in]    BfIndex
  @param[in]    RegOffset
  @param[in]    Hi
  @param[in]    Lo

  @return   PMU_SRAM  Encrypted PMU SRAM field location
--*/
#define PMU_SRAM(TableName, BfIndex, RegOffset, Hi, Lo) \
  TableName[BfIndex] = ( \
    ((UINT32) RegOffset << 12) | ( \
      (Hi > Lo) ? (\
        (((UINT32) Hi << 6) | (UINT32) Lo) \
      ) : ( \
        (((UINT32) Lo << 6) | (UINT32) Hi) \
      ) \
    ) \
  )

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
VOID
MemNPmuResetNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNStartPmuNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPendOnPmuCompletionNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcMR0 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

UINT32
MemNCalcMR1 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

UINT32
MemNCalcMR2 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  );

#endif  /* _MNPMU_H_ */

