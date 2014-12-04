/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch Pcib controller
 *
 * Init Pcib Controller features (PEI phase).
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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
****************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE PROC_FCH_PCIB_PCIBRESET_FILECODE
/**
 * FchInitResetPcibPciTable - Pcib device registers initial
 * during the power on stage.
 *
 *
 *
 *
 */
REG8_MASK FchInitResetPcibPciTable[] =
{
  //
  // P2P Bridge (Bus 0, Dev 20, Func 4)
  //
  {0x00, PCIB_BUS_DEV_FUN, 0},
  {FCH_PCIB_REG4B, 0xFF, BIT6 + BIT7 + BIT4},
  {FCH_PCIB_REG40, 0xDF, 0x20},
  {FCH_PCIB_REG50, 0x02, 0x01},
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitResetPcib - Config Pcib controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetPcib (
  IN  VOID     *FchDataPtr
  )
{
  AMD_CONFIG_PARAMS         *StdHeader;

  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;

  ProgramPciByteTable (
    (REG8_MASK*) (&FchInitResetPcibPciTable[0]),
    sizeof (FchInitResetPcibPciTable) / sizeof (REG8_MASK),
    StdHeader
    );
  if ( UserOptions.FchBldCfg->CfgFchPort80BehindPcib ) {
    FchInitResetPcibPort80Enable (FchDataPtr);
  }
}

/**
 * FchInitResetPcibPort80Enable - Pcib device registers initial
 * during the power on stage.
 *
 *
 *
 *
 */
REG8_MASK FchInitResetPcibPort80EnableTable[] =
{
  //
  // P2P Bridge (Bus 0, Dev 20, Func 4)
  //
  {0x00, PCIB_BUS_DEV_FUN, 0},
  {FCH_PCIB_REG1C, 0x00, 0xF0},
  {FCH_PCIB_REG1D, 0x00, 0x00},
  {FCH_PCIB_REG04, 0x00, 0x21},
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitResetPcibPort80Enable - Enable Port80 Behind PCIB
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetPcibPort80Enable (
  IN  VOID     *FchDataPtr
  )
{
  AMD_CONFIG_PARAMS         *StdHeader;

  StdHeader = &((AMD_RESET_PARAMS *)FchDataPtr)->StdHeader;

  ProgramPciByteTable (
    (REG8_MASK*) (&FchInitResetPcibPort80EnableTable[0]),
    sizeof (FchInitResetPcibPort80EnableTable) / sizeof (REG8_MASK),
    StdHeader
    );
}

