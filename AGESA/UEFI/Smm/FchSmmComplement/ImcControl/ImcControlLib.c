/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH IMC lib
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Hudson-2 UEFI Driver
 * @e sub-project   Imc Control Driver
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

#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_IMCCONTROL_IMCCONTROLLIB_FILECODE
#include "FchSmmLib.h"
#include "FchPlatform.h"

extern EFI_SMM_SYSTEM_TABLE                 *mSmst;
extern EFI_SMM_CPU_IO_INTERFACE             *mSmmIo;
extern UINT32                               mImcThermalEnable;   ///< Bit0=81FunZone0 support(1=On;0=Off); bit1-3=81FunZone1-Zone3;

BOOLEAN
FchIsImcEnabled (
  VOID
  )
{
  UINT8   dbSysConfig;
  LibFchSmmMemRead (mSmmIo, SMM_IO_UINT8, (UINT64) (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80), &dbSysConfig);
  if (dbSysConfig & BIT2) {
    return TRUE;
  } else {
    return FALSE;
  }
}

VOID
FchWriteECmsg (
  IN       UINT8     Address,
  IN       UINT8     OpFlag,
  IN       VOID      *Value
  )
{
  LibFchSmmIoWrite (mSmmIo, SMM_IO_UINT8, MailBoxPort, &Address);
  LibFchSmmIoWrite (mSmmIo, SMM_IO_UINT8, MailBoxPort + 1, (UINT8 *)Value);
}

VOID
FchReadECmsg (
  IN       UINT8     Address,
  IN       UINT8     OpFlag,
     OUT   VOID      *Value
  )
{
  LibFchSmmIoWrite (mSmmIo, SMM_IO_UINT8, MailBoxPort, &Address);
  LibFchSmmIoRead (mSmmIo, SMM_IO_UINT8, MailBoxPort + 1, (UINT8 *)Value);
}

VOID
CallFchStall (
  IN       UINT32              uSec
  )
{
  UINT16 timerAddr;
  UINT32 startTime;
  UINT32 elapsedTime;

  LibFchSmmMemRead (mSmmIo, SMM_IO_UINT16, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64), &timerAddr);
  if ( timerAddr == 0 ) {
    uSec = uSec / 2;
    while ( uSec != 0 ) {
      LibFchSmmIoRead (mSmmIo, SMM_IO_UINT8, FCHOEM_IO_DELAY_PORT, (UINT8 *) (&startTime));
      uSec--;
    }
  } else {
    LibFchSmmIoRead (mSmmIo, SMM_IO_UINT32, timerAddr, &startTime);
    for ( ;; ) {
      LibFchSmmIoRead (mSmmIo, SMM_IO_UINT32, timerAddr, &elapsedTime);
      if ( elapsedTime < startTime ) {
        elapsedTime = elapsedTime + FCH_MAX_TIMER - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }
      if ( (elapsedTime * FCHOEM_ELAPSED_TIME_UNIT / FCHOEM_ELAPSED_TIME_DIVIDER) > uSec ) {
        break;
      }
    }
  }
}

VOID
FchWaitForEcLDN9MailboxCmdAck (
  VOID
  )
{
  UINT8    Msgdata;
  UINT16   Delaytime;

  Msgdata = 0;

  for (Delaytime = 0; Delaytime <= 500; Delaytime++) {
    FchReadECmsg (MSG_REG0, AccessWidth8, &Msgdata);
    if ( Msgdata == 0xfa) {
      break;
    }

    CallFchStall (1000);                            /// Wait for 1ms
  }
}

/**
 * ImcSleep - IMC Sleep.
 *
 *
 *
 */
VOID
FchImcSleep (
  VOID
  )
{
  UINT8        Msgdata;

  if (!(FchIsImcEnabled ()) ) {
    return;                                                ///IMC is not enabled
  }

  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
  Msgdata = 0xB4;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
  Msgdata = 0x96;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
  FchWaitForEcLDN9MailboxCmdAck ();
}


/**
 * ImcEnableSurebootTimer - IMC Enable Sureboot Timer.
 *
 *
 *
 */
/*
VOID
FchImcEnableSurebootTimer (
  VOID
  )
{
  UINT8                  Msgdata;
  AMD_CONFIG_PARAMS      StdHeader;

  ImcDisableSurebootTimer ();

  Msgdata = 0x00;

  if (!(IsImcEnabled (&StdHeader)) ) {
    return;                                      ///IMC is not enabled
  }

  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata, &StdHeader);
  Msgdata = 0x01;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata, &StdHeader);
  Msgdata = ( (LocalCfgPtr->Imc.ImcSureBootTimer) << 6) -1;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata, &StdHeader);
  Msgdata = 0x94;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata, &StdHeader);
  FchWaitForEcLDN9MailboxCmdAck (&StdHeader);
}
*/
/**
 * ImcDisableSurebootTimer - IMC Disable Sureboot Timer.
 *
 *
 *
 *
 */
VOID
FchImcDisableSurebootTimer (
  VOID
  )
{
  UINT8   Msgdata;

  if (!(FchIsImcEnabled ()) ) {
    return;                                      ///IMC is not enabled
  }

  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
  Msgdata = 0x01;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
  Msgdata = 0x94;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
  FchWaitForEcLDN9MailboxCmdAck ();
}

/**
 * ImcWakeup - IMC Wakeup.
 *
 *
 *
 *
 */
VOID
FchImcWakeup (
  VOID
  )
{
  UINT8   Msgdata;

  if (!(FchIsImcEnabled ()) ) {
    return;                                      ///IMC is not enabled
  }

  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
  Msgdata = 0xB5;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
  Msgdata = 0x96;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
  FchWaitForEcLDN9MailboxCmdAck ();
}

/**
 * ImcIdle - IMC Idle.
 *
 *
 *
 *
 */
VOID
FchImcIdle (
  VOID
  )
{
  UINT8   Msgdata;

  if (!(FchIsImcEnabled ()) ) {
    return;                                      ///IMC is not enabled
  }

  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
  Msgdata = 0x01;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
  Msgdata = 0x98;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
  FchWaitForEcLDN9MailboxCmdAck ();
}

/**
 * ImcResume - IMC Resume.
 *
 *
 *
 *
 */
VOID
FchImcResume (
  VOID
  )
{
  UINT8   Msgdata;

  if (!(FchIsImcEnabled ()) ) {
    return;                                      ///IMC is not enabled
  }

  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
  Msgdata = 0x02;
  FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
  Msgdata = 0x00;
  FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
  Msgdata = 0x98;
  FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
  FchWaitForEcLDN9MailboxCmdAck ();
}

/**
 * FchImcThermalZoneEnable - IMC Thermal Zone Enable.
 *
 *
 *
 *
 */
VOID
FchImcThermalZoneEnable (
  VOID
  )
{
  UINT8   Msgdata;
  UINT8   ZoneNum;

  if (!(FchIsImcEnabled ()) ) {
    return;                                      ///IMC is not enabled
  }

  for ( ZoneNum = 0; ZoneNum < 4; ZoneNum++ ) {
    if (mImcThermalEnable & (1 << ZoneNum)) {
      Msgdata = 0x00;
      FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
      Msgdata = ZoneNum;
      FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
      Msgdata = 0x00;
      FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
      Msgdata = 0x80;
      FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
      FchWaitForEcLDN9MailboxCmdAck ();

      FchReadECmsg (MSG_REG2, AccessWidth8, &Msgdata);
      Msgdata |= BIT0;
      FchWriteECmsg (MSG_REG2, AccessWidth8, &Msgdata);
      Msgdata = 0x00;
      FchWriteECmsg (MSG_REG0, AccessWidth8, &Msgdata);
      Msgdata = ZoneNum;
      FchWriteECmsg (MSG_REG1, AccessWidth8, &Msgdata);
      Msgdata = 0x81;
      FchWriteECmsg (MSG_SYS_TO_IMC, AccessWidth8, &Msgdata);
      FchWaitForEcLDN9MailboxCmdAck ();
    }
  }
}
