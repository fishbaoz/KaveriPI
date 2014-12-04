/* $NoKeywords:$ */
/**
 * @file
 *
 * mnPmuSramMsgBlockKV.h
 *
 * PMU SRAM Message Block related function for KV.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
/*****************************************************************************
*
* Copyright 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef _MNPMUSRAMMSGBLOCKKV_H_
#define _MNPMUSRAMMSGBLOCKKV_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

// The members size and the order of the structure is very important. The structure mapped
// directly from Table PMU SRAM Message Block of KV BKDG.
/// Kaveri PMU SRAM Message Block.
typedef struct _PMU_SRAM_MSG_BLOCK_KV {
  UINT8   Revision;           ///< Location 0, Table revision
  UINT8   Reserved0;          ///< Location 1, Reserved
  UINT16  FirmwareRevsionId;  ///< Location 3:2, PMU firmware revision ID
  UINT32  CpuId;              ///< Location 7:4, 32-bit CPUID
  UINT8   DramType;           ///< Location 8, Dram type from SPD byte 2
  UINT8   ModuleType;         ///< Location 9, Dram type from SPD byte 3
  UINT8   RawCard0;           ///< Location Ah, DIMM0 reference raw card from module type specific SPD location
  UINT8   RawCard1;           ///< Location Bh, DIMM1 reference raw card from module type specific SPD location
  UINT8   Reserved1;          ///< Location Ch, Reserved
  UINT8   Reserved2;          ///< Location Dh, Reserved
  UINT8   ChipSelect;         ///< Location Eh, Specify which chipselects are present and should be trained
  UINT8   AddrMirror;         ///< Location Fh, Specify which chipselects have address mirroring
  UINT8   Dimm0Cols;          ///< Location 10h, Number of device column address bits for Dimm0
  UINT8   Dimm0Banks;         ///< Location 11h, Number of device bank address bits for Dimm0
  UINT8   Dimm0Rows;          ///< Location 12h, Number of device row address bits for Dimm0
  UINT8   Dimm1Cols;          ///< Location 13h, Number of device column address bits for Dimm0
  UINT8   Dimm1Banks;         ///< Location 14h, Number of device bank address bits for Dimm0
  UINT8   Dimm1Rows;          ///< Location 15h, Number of device row address bits for Dimm0
  UINT8   Reserved3[7];       ///< Location 1Ch:16h, Reserved
  UINT8   PerRankTiming;      ///< Location 1Dh, 1=The channel is configured to use four timing
                              ///< sources for four independent chipselects. 0=The
                              ///< channel is configured to use four timing sources
                              ///< for four pairs of chipselects
  UINT16  CurrentTemp;        ///< Location 1Fh:1Eh, 11-bit temperature in degrees C, with -49C offset, in 0.125C increments, unsigned
  UINT16  SequenceCtl;        ///< Location 21h:20h, PMU Sequence Control Word
                              ///< [0] 1=DevInit
                              ///< [1] 1=WrLvl Training
                              ///< [2] 1=RxEnDly Training
                              ///< [3] 1=1D Rd-Dqs Training
                              ///< [4] 1=1D Wr-Dq Training
                              ///< [5] 1=2D Read Training
                              ///< [6] 1=2D Write Training
                              ///< [15:7] Reserved
  UINT8   Reserved4;          ///< Location 22h, Reserved
  UINT8   CkeSetup_M0;        ///< Location 23h, Setup time on CAD bus signals
  UINT8   CsOdtSetup_M0;      ///< Location 24h, Setup time on CAD bus signals
  UINT8   AddrCmdSetup_M0;    ///< Location 25h, Setup time on CAD bus signals
  UINT8   SlowAccessMode_M0;  ///< Location 26h, 1=2T Timing is enabled in the controller
  UINT8   TRP_M0;             ///< Location 27h, In unit of memclocks
  UINT8   TMRD_M0;            ///< Location 28h, In unit of memclocks
  UINT8   TRFC_M0;            ///< Location 29h, In unit of memclocks
  UINT16  MR0_M0;             ///< Location 2Bh:2Ah
  UINT16  MR1_M0;             ///< Location 2Dh:2Ch
  UINT16  MR2_M0;             ///< Location 2Fh:2Eh
  UINT8   CD_R_W_M0;          ///< Location 30h, Command delay, read to write, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_R_R_M0;          ///< Location 31h, Command delay, read to read, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_W_W_M0;          ///< Location 32h, Command delay, write to write, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_W_R_M0;          ///< Location 33h, Command delay, write to read, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_R_R_SD_M0;       ///< Location 34h, Command delay, read to read, any chip select to any other chipselect same DIMM. Units are memclocks
  UINT8   CD_W_W_SD_M0;       ///< Location 35h, Command delay, write to write, any chip select to any other chipselect same DIMM. Units are memclocks
  UINT8   Trdrdban_Phy_M0;    ///< Location 36h, In memclocks
  UINT8   CkeSetup_M1;        ///< Location 37h, Setup time on CAD bus signals
  UINT8   CsOdtSetup_M1;      ///< Location 38h, Setup time on CAD bus signals
  UINT8   AddrCmdSetup_M1;    ///< Location 39h, Setup time on CAD bus signals
  UINT8   SlowAccessMode_M1;  ///< Location 3Ah, 1=2T Timing is enabled in the controller
  UINT8   TRP_M1;             ///< Location 3Bh, In unit of memclocks
  UINT8   TMRD_M1;            ///< Location 3Ch, In unit of memclocks
  UINT8   TRFC_M1;            ///< Location 3Dh, In unit of memclocks
  UINT16  MR0_M1;             ///< Location 3Fh:3Eh
  UINT16  MR1_M1;             ///< Location 41h:40h
  UINT16  MR2_M1;             ///< Location 43h:42h
  UINT8   CD_R_W_M1;          ///< Location 44h, Command delay, read to write, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_R_R_M1;          ///< Location 45h, Command delay, read to read, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_W_W_M1;          ///< Location 46h, Command delay, write to write, any chip select to any other chipselect. Units are memclocks
  UINT8   CD_W_R_M1;          ///< Location 47h, Command delay, write to read, any chip select to any other chipselect same DIMM. Units are memclocks
  UINT8   CD_R_R_SD_M1;       ///< Location 48h, Command delay, read to read, any chip select to any other chipselect same DIMM. Units are memclocks
  UINT8   CD_W_W_SD_M1;       ///< Location 49h, Command delay, write to write, any chip select to any other chipselect. Units are memclocks
  UINT8   Trdrdban_Phy_M1;    ///< Location 4Ah, In memclocks
  UINT8   M1StateEn;          ///< Location 4Bh, M1StateEn
  UINT8   Reserved5[4];       ///< Location 4Fh:4Ch, Reserved
} PMU_SRAM_MSG_BLOCK_KV;

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

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */

BOOLEAN
MemNWritePmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNInitPmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

BOOLEAN
MemNPostPmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );
#endif //_MNPMUSRAMMSGBLOCKKV_H_
