/* $NoKeywords:$ */
/**
 * @file
 *
 * mrtrci3.c
 *
 * Technology Control word initialization for DDR3 Recovery
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem)
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

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "OptionMemory.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mru.h"
#include "mt.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_TECH_DDR3_MRTRCI3_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

UINT8
STATIC
MemRecTGetCtlWord3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 CtrlWordNum
  );

VOID
STATIC
MemRecTSendCtlWord3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 CmdNum,
  IN       UINT8 Value
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function sends control words
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 *     @return  pDCT->Timings.TrwtTO updated
 */

VOID
MemRecTDramControlRegInit3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT8 RCNum;
  UINT8 Data;

  MEM_DATA_STRUCT *MemPtr;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;

  // wait 8us TACT must be changed to optimize to 8 MEM CLKs
  // and wait 6us for PLL LOCK
  MemRecUWait10ns (80 + 60, MemPtr);

  // 2. Program F2x[1, 0]A8[CtrlWordCS]=bit mask for target chip selects.
  NBPtr->SetBitField (NBPtr, BFCtrlWordCS, 3 << (NBPtr->DimmToBeUsed << 1));

  for (RCNum = 0; RCNum <= 15; RCNum++) {
    // wait 8us for TMRD, must be changed to optimize to 8 MEM CLKs
    MemRecUWait10ns (80, MemPtr);

    if ((RCNum != 6) && (RCNum != 7)) {
      Data = MemRecTGetCtlWord3 (TechPtr, RCNum);
      MemRecTSendCtlWord3 (TechPtr, RCNum, Data);
    }
  }

  MemRecUWait10ns (60, MemPtr);   // wait 6us for TSTAB
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function calculates the ControlRC value
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]     CtrlWordNum  -  control Word number.
 *
 *     @return  Control Word value
 */

UINT8
STATIC
MemRecTGetCtlWord3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 CtrlWordNum
  )
{
  UINT8  Data;

  CH_DEF_STRUCT *ChannelPtr;

  ChannelPtr = TechPtr->NBPtr->ChannelPtr;

  Data = 0;  //Default value for all control words is 0
  switch (CtrlWordNum) {
  case 0:
    Data = 0x02;  // DA4=1
    break;
  case 1:
    Data = 0x0C;  // if single rank, set DBA1 and DBA0
    break;
  case 3:
    Data = ChannelPtr->CtrlWrd03[TechPtr->NBPtr->DimmToBeUsed];
    break;
  case 4:
    Data = ChannelPtr->CtrlWrd04[TechPtr->NBPtr->DimmToBeUsed];
    break;
  case 5:
    Data = ChannelPtr->CtrlWrd05[TechPtr->NBPtr->DimmToBeUsed];
    break;
  case 9:
    Data = 0x0D;
    break;
  default:;
  }

  return (Data&0x0F);
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function sends control word command
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]     CmdNum  -  control number.
 *     @param[in]     Value  -  value to send
 *
 */

VOID
STATIC
MemRecTSendCtlWord3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 CmdNum,
  IN       UINT8 Value
  )
{
  MEM_NB_BLOCK  *NBPtr;

  ASSERT (CmdNum < 16);
  ASSERT (Value < 16);

  NBPtr = TechPtr->NBPtr;

  // 1. Program MrsBank and MrsAddress.
  //    n = [BA2, A2, A1, A0].
  //    data = [BA1, BA0, A4, A3].
  //    Set all other bits in MrsAddress to zero.
  //
  NBPtr->SetBitField (NBPtr, BFMrsBank, ((CmdNum&8) >> 1) | (Value >> 2));
  NBPtr->SetBitField (NBPtr, BFMrsAddress, ((Value&3) << 3) | (CmdNum&7));
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tCS%d RC%02d %04x\n",
              (NBPtr->GetBitField (NBPtr, BFDramInitRegReg) >> 20) & 0xF,
              ((NBPtr->GetBitField (NBPtr, BFDramInitRegReg) >> 15) & 8) |
              (NBPtr->GetBitField (NBPtr, BFDramInitRegReg) & 7),
              ((NBPtr->GetBitField (NBPtr, BFDramInitRegReg) >> 14) & 0xC) |
              ((NBPtr->GetBitField (NBPtr, BFDramInitRegReg) >> 3) & 3));

  // 2.Set SendCtrlWord=1
  NBPtr->SetBitField (NBPtr, BFSendCtrlWord, 1);
  // 3.Wait for BFSendCtrlWord=0
  while (NBPtr->GetBitField (NBPtr, BFSendCtrlWord) != 0) {}
}
