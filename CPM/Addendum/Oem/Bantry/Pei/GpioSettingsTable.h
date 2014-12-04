/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for GPIO.
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
#ifndef _GPIO_SETTINGS_TABLE_H_
#define _GPIO_SETTINGS_TABLE_H_
#include "AmdCpmOemInitPeim.h"

//
// GPIO Init Table
//
AMD_CPM_GPIO_INIT_TABLE             gCpmGpioPreInitTable = {
  {CPM_SIGNATURE_GPIO_PRE_INIT, sizeof (gCpmGpioPreInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    GPIO_DEFINITION(32,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),    // INTE#/GPIO32 -> DET0
    GPIO_DEFINITION(33,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),    // INTF#/GPIO33 -> DET1
    GPIO_DEFINITION(34,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),    // INTG#/GPIO34 -> DET2
    0xFF,
  }
};

//
// GPIO Init Table
//
AMD_CPM_GPIO_INIT_TABLE             gCpmGpioInitTable = {
  {CPM_SIGNATURE_GPIO_INIT, sizeof (gCpmGpioInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    GPIO_DEFINITION(0,      GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD0/GPIO0 -> W_DISABLED#
    GPIO_DEFINITION(1,      GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD1/GPIO1 -> MPCIE_PWRON
    GPIO_DEFINITION(2,      GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,         GPIO_STICKY_EN),     // AD2/GPIO2 -> PWR_LED_EC
    GPIO_DEFINITION(3,      GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),    // AD3/GPIO3 -> IR_TX0_DET
    GPIO_DEFINITION(4,      GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),    // AD4/GPIO4 -> GFX_PRSNT#2, NA
    GPIO_DEFINITION(5,      GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD5/GPIO5 -> GFX_MODE_X16
    GPIO_DEFINITION(14,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD14/GPIO14 -> BOARD_ID0
    GPIO_DEFINITION(15,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD15/GPIO15 -> BOARD_ID1
    GPIO_DEFINITION(16,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD16/GPIO16 -> BOARD_ID2
    GPIO_DEFINITION(17,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD17/GPIO17 -> BOARD_ID3
    GPIO_DEFINITION(18,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD18/GPIO18 -> BOARD_ID4
    GPIO_DEFINITION(19,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // AD19/GPIO19 -> BOARD_ID5
    GPIO_DEFINITION(41,     GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // REQ2#/CLK_REQ8#/GPIO41 -> CLK_REQ8#
//    GPIO_DEFINITION(42,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // REQ3#/CLK_REQ5#/GPIO42 -> FCH_SDA to PCIE EEPROM
    GPIO_DEFINITION(45,     GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // GNT2#/GPO45 -> SD_LED
//    GPIO_DEFINITION(46,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // GNT3#/CLK_REQ7# /GPIO46 -> FCH_SCL to PCIE EEPROM
    GPIO_DEFINITION(49,     GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_EN),     // LDRQ1#/ CLK_REQ6#/GPIO49 -> CLK_REQ6#
//    GPIO_DEFINITION(50,     GPIO_FUNCTION_2,    GPIO_OUTPUT_LOW,    GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // SMARTVOLT1/SATA_IS2#/GPIO50 -> COMM_EN
    GPIO_DEFINITION(51,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // SMARTVOLT2/SHUTDOWN#/GPIO51 -> IR_TX1_DET
    GPIO_DEFINITION(52,     GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // GPIO52
    GPIO_DEFINITION(59,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // SATA_IS5#/FANIN3/GPIO59 -> FPAUD_PRESENCE#
    GPIO_DEFINITION(61,     GPIO_FUNCTION_2,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // CLK_REQ1#/FANOUT4/GPIO61 -> FCH_PROCHOT#
//    GPIO_DEFINITION(62,     GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // CLK_REQ2#/FANIN4/GPIO62 -> FCH_MPCIE_CLKREQ#
    GPIO_DEFINITION(63,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // CLK_REQ3#/SATA_IS1#/GPIO63 -> GFX_PRSNT#2
    GPIO_DEFINITION(64,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // CLK_REQ4#/SATA_IS0#/GPIO64 -> IR_TX0_DET
    GPIO_DEFINITION(65,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // CLK_REQG#/GPIO65/OSCIN -> FCH_IDLEEXIT#_R

    GPIO_DEFINITION(102,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // USB_OC6#/IR_TX1/GEVENT6#  -> IR_TX1
//    GPIO_DEFINITION(103,    GPIO_FUNCTION_0,    GPIO_OUTPUT_LOW,    GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // DDR3_RST#/GEVENT7#/VGA_PD -> VGA_PD, special pin difination for SB900 VGA OUTPUT, high active, VGA power for Hudson-M2 will be down when it was asserted.
    GPIO_DEFINITION(106,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // GBE_LED2/GEVENT10# -> ESATA_DET#
    GPIO_DEFINITION(108,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // USB_OC0#/TRST#/GEVENT12# -> FCH_JTAG_TRST#
    GPIO_DEFINITION(112,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // USB_OC4/IR_RX0/GEVENT16#  -> IR_RX0
    GPIO_DEFINITION(113,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // USB_OC5/IR_TX0/GEVENT17# -> IR_TX0
    GPIO_DEFINITION(117,    GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // SPI_CS3#/GBE_STAT1/GEVENT21# -> LAN_SMALERT#
    GPIO_DEFINITION(118,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // RI#/GEVENT22# -> LAN_WAKEUP#
    GPIO_DEFINITION(119,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),    // LPC_SMI#/GEVENT23# -> for TALERT# (TWARN)

    GPIO_DEFINITION(174,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),   // TEMPIN3/TALERT#/GPIO174 -> TALERT#_R
    GPIO_DEFINITION(175,    GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_EN),    // VIN0/GPIO175 -> APU_S0_RST#
    GPIO_DEFINITION(176,    GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_EN),    // VIN1/GPIO176 -> APU_S1_RST#
    GPIO_DEFINITION(177,    GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_PD_DIS,     GPIO_STICKY_EN),    // VIN2/GPIO177 -> APU_GPP_RST#
    GPIO_DEFINITION(183,    GPIO_FUNCTION_1,    GPIO_OUTPUT_LOW,    GPIO_PD_EN,         GPIO_STICKY_DIS),   // GBE_LED0/GPIO183 -> GEB_EN#
    GPIO_DEFINITION(184,    GPIO_FUNCTION_2,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,         GPIO_STICKY_DIS),   // IR_LED#/LLB#/GPIO184 -> IR_RX_IND
    GPIO_DEFINITION(189,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,         GPIO_STICKY_EN),    // PS2KB_DAT/GPIO189  -> MEM_1V25#
    GPIO_DEFINITION(190,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,         GPIO_STICKY_EN),    // PS2KB_CLK/GPIO190 -> MEM_1V5#
    GPIO_DEFINITION(191,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),   // PS2M_DAT/GPIO191 -> DP2_CAB_DP_HDMI#
    GPIO_DEFINITION(192,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),   // PS2M_CLK/GPIO192 -> DP3_CAB_DP_HDMI#
    GPIO_DEFINITION(197,    GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),   // EC_PWM0/EC_TIMER0/GPIO197 -> DP4_CAB_DP_HDMI#
    GPIO_DEFINITION(198,    GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PU_EN,         GPIO_STICKY_DIS),   // EC_PWM1/EC_TIMER1/GPIO198 -> DP5_CAB_DP_HDMI#
    GPIO_DEFINITION(199,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),   // EC_PWM2/EC_TIMER2/GPIO199 -> NOT USED
//    GPIO_DEFINITION(200,    GPIO_FUNCTION_2,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),   // EC_PWM3/EC_TIMER3/GPIO200 -> FCH_GPIO200
//    GPIO_DEFINITION(201,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS,     GPIO_STICKY_DIS),   // KSI_0/GPIO201 -> EC_GPIO201
    0xFF,
  }
};
#endif