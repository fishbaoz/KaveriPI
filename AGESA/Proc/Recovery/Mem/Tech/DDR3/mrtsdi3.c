/* $NoKeywords:$ */
/**
 * @file
 *
 * mrtsdi3.c
 *
 * Technology Software DRAM Init for DDR3 Recovery
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
#include "mrt3.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_TECH_DDR3_MRTSDI3_FILECODE
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


/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */



/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initiates software DRAM init
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTDramInitSw3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT8 ChipSel;
  MEM_DATA_STRUCT *MemPtr;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;

  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart Dram Init\n");
  IDS_HDT_CONSOLE (MEM_FLOW, "\tEnDramInit = 1 for DCT%d\n", NBPtr->Dct);
  // 3.Program F2x[1,0]7C[EnDramInit]=1
  NBPtr->SetBitField (NBPtr, BFEnDramInit, 1);

  // 4.wait 200us
  MemRecUWait10ns (20000, MemPtr);

  NBPtr->SetBitField (NBPtr, BFDeassertMemRstX, 1);

  // 6.wait 500us
  MemRecUWait10ns (50000, MemPtr);

  // 7.NOP or deselect & take CKE high
  NBPtr->SetBitField (NBPtr, BFAssertCke, 1);

  // 8.wait 360ns
  MemRecUWait10ns (36, MemPtr);

  // The following steps are performed with registered DIMMs only and
  // must be done for each chip select pair:
  //
  if (NBPtr->ChannelPtr->RegDimmPresent != 0) {
    MemRecTDramControlRegInit3 (TechPtr);
  }

  for (ChipSel = 0; ChipSel < MAX_CS_PER_CHANNEL; ChipSel++) {
    if ((NBPtr->DCTPtr->Timings.CsPresent & (UINT16) 1 << ChipSel) != 0) {

      // Set Dram ODT per ChipSel
      NBPtr->SetDramOdtRec (NBPtr, MISSION_MODE, ChipSel, (NBPtr->DimmToBeUsed << 1));

      NBPtr->SetBitField (NBPtr, BFMrsChipSel, ChipSel);
      // 13.Send EMRS(2)
      MemRecTEMRS23 (TechPtr);
      NBPtr->SendMrsCmd (NBPtr);

      // 14.Send EMRS(3). Ordinarily at this time, MrsAddress[2:0]=000b
      MemRecTEMRS33 (TechPtr);
      NBPtr->SendMrsCmd (NBPtr);

      // 15.Send EMRS(1).
      MemRecTEMRS13 (TechPtr);
      NBPtr->SendMrsCmd (NBPtr);

      // 16.Send MRS with MrsAddress[8]=1(reset the DLL)
      MemRecTMRS3 (TechPtr);
      NBPtr->SendMrsCmd (NBPtr);

      //wait 500us
      MemRecUWait10ns (50000, MemPtr);

      if (NBPtr->ChannelPtr->RegDimmPresent == 0) {
        break;
      }
    }
  }

  // 17.Send two ZQCL commands (to even then odd chip select)
  NBPtr->sendZQCmd (NBPtr);
  NBPtr->sendZQCmd (NBPtr);

  // 18.Program F2x[1,0]7C[EnDramInit]=0
  NBPtr->SetBitField (NBPtr, BFEnDramInit, 0);
  IDS_HDT_CONSOLE (MEM_FLOW, "End Dram Init\n\n");
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function calculates the EMRS1 value
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTEMRS13 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT16 MrsAddress;
  UINT8 DramTerm;

  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  // BA2=0,BA1=0,BA0=1
  NBPtr->SetBitField (NBPtr, BFMrsBank, 1);

  MrsAddress = 0;

  // program MrsAddress[5,1]=output driver impedance control (DIC):
  // based on F2x[1,0]84[DrvImpCtrl], which is 2'b01
  MrsAddress |= ((UINT16) 1 << 1);

  // program MrsAddress[9,6,2]=nominal termination resistance of ODT (RTT):
  // based on F2x[1,0]84[DramTerm], which is 3'b001 (60 Ohms)
  if (!(NBPtr->IsSupported[CheckDramTerm])) {
    DramTerm = (UINT8) NBPtr->GetBitField (NBPtr, BFDramTerm);
  } else {
    DramTerm = NBPtr->PsPtr->DramTerm;
  }
  if ((DramTerm & 1) != 0) {
    MrsAddress |= ((UINT16) 1 << 2);
  }
  if ((DramTerm & 2) != 0) {
    MrsAddress |= ((UINT16) 1 << 6);
  }
  if ((DramTerm & 4) != 0) {
    MrsAddress |= ((UINT16) 1 << 9);
  }

  // program MrsAddress[12]=output disable (QOFF):
  // based on F2x[1,0]84[Qoff], which is 1'b0

  // program MrsAddress[11]=TDQS:
  // based on F2x[1,0]94[RDqsEn], which is 1'b0

  NBPtr->SetBitField (NBPtr, BFMrsAddress, MrsAddress);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function calculates the EMRS2 value
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTEMRS23 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT16 MrsAddress;
  UINT8 DramTermDyn;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  // BA2=0,BA1=1,BA0=0
  NBPtr->SetBitField (NBPtr, BFMrsBank, 2);

  // program MrsAddress[5:3]=CAS write latency (CWL):
  // based on F2x[1,0]84[Tcwl], which is 3'b000
  //
  MrsAddress = 0;

  // program MrsAddress[6]=auto self refresh method (ASR):
  // based on F2x[1,0]84[ASR], which is 1'b1
  // program MrsAddress[7]=self refresh temperature range (SRT):
  // based on F2x[1,0]84[SRT], which is also 1'b0
  //
  MrsAddress |= (UINT16) 1 << 6;

  // program MrsAddress[10:9]=dynamic termination during writes (RTT_WR):
  // based on F2x[1,0]84[DramTermDyn]
  //
  if (!(NBPtr->IsSupported[CheckDramTermDyn])) {
    DramTermDyn = (UINT8) NBPtr->GetBitField (NBPtr, BFDramTermDyn);
  } else {
    DramTermDyn = NBPtr->PsPtr->DynamicDramTerm;
  }
  MrsAddress |= (UINT16) DramTermDyn << 9;
  NBPtr->SetBitField (NBPtr, BFMrsAddress, MrsAddress);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function calculates the EMRS3 value
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTEMRS33 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  // BA2=0,BA1=1,BA0=1
  NBPtr->SetBitField (NBPtr, BFMrsBank, 3);

  // program MrsAddress[1:0]=multi purpose register address location
  // (MPR Location):based on F2x[1,0]84[MprLoc], which is 0
  // program MrsAddress[2]=multi purpose register
  // (MPR):based on F2x[1,0]84[MprEn], which is also 0
  //
  NBPtr->SetBitField (NBPtr, BFMrsAddress, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This sets MSS value
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 */

VOID
MemRecTMRS3 (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT16 MrsAddress;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;

  // BA2=0,BA1=0,BA0=0
  NBPtr->SetBitField (NBPtr, BFMrsBank, 0);

  // program MrsAddress[1:0]=burst length and control method
  // (BL):based on F2x[1,0]84[BurstCtrl], which is 1'b0
  //
  MrsAddress = 0;

  // program MrsAddress[3]=1 (BT):interleaved
  MrsAddress |= (UINT16) 1 << 3;

  // program MrsAddress[6:4,2]=read CAS latency
  // (CL):based on F2x[1,0]88[Tcl], which is 4'b0010
  MrsAddress |= (UINT16) 2 << 4;

  // program MrsAddress[11:9]=write recovery for auto-precharge
  // (WR):based on F2x[1,0]84[Twr], which is 3'b010
  //
  MrsAddress |= (UINT16) 2 << 9;

  // program MrsAddress[12]=0 (PPD):slow exit

  // program MrsAddress[8]=1 (DLL):DLL reset
  MrsAddress |= (UINT16) 1 << 8;   // just issue DLL reset at first time

  NBPtr->SetBitField (NBPtr, BFMrsAddress, MrsAddress);
}
