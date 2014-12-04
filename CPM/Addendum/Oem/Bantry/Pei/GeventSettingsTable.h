/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for GEVENT.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef GEVENT_SETTINGS_TABLE_H_
#define GEVENT_SETTINGS_TABLE_H_
#include "AmdCpmOemInitPeim.h"

//
// GEVENT Init Table
//
AMD_CPM_GEVENT_INIT_TABLE           gCpmGeventInitTable = {
  {CPM_SIGNATURE_GEVENT_INIT, sizeof (gCpmGeventInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  { //                          EventEnable     SciTrigE        SciLevl         SmiSciEn        SciS0En         SciMap      SmiTrig     SmiControl
    GEVENT_DEFINITION ( 0x00,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_00,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x01,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_01,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x02,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_02,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x03,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_03,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x04,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_04,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x05,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_05,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x06,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_06,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x07,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_07,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x08,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_08,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x09,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_09,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0A,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_10,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0B,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_11,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0C,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_12,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0D,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_13,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0E,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_14,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0F,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_15,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x10,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_16,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x11,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_17,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x12,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_18,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x13,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_19,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x14,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_20,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x15,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_21,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x16,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_22,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x17,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_23,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    0xFF,
  }
};
#endif