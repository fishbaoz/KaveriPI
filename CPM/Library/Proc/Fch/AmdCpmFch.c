/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Library for Common FCH Function
 *
 * Contains interface to the AMD CPM library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Library
 * @e \$Revision: 289766 $   @e \$Date: 2014-04-15 09:44:01 -0500 (Tue, 15 Apr 2014) $
 *
 */
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

#include <AmdCpmBase.h>
#include <AmdCpmBaseIo.h>
#include CPM_PPI_DEFINITION (AmdCpmTablePpi)

#define GPIO_BASE                       0x100
#define SMI_BASE                        0x200
#define PMIO_BASE                       0x300
#define PMIO2_BASE                      0x400
#define RAM_BASE                        0x500
#define CMOSRAM_BASE                    0x600
#define CMOS_BASE                       0x700
#define ACPI_BASE                       0x800
#define IOMUX_BASE                      0xD00
#define MISC_BASE                       0xE00
#define GPIO_BANK_BASE                  0x1500

#define AMD_SMIREG_EVENT_ENABLE         0x04
#define AMD_SMIREG_SCITRIG              0x08
#define AMD_SMIREG_SCILEVEL             0x0C
#define AMD_SMIREG_SMISCIEN             0x14
#define AMD_SMIREG_SWSCIEN              0x18
#define AMD_SMIREG_SWSCIDATA            0x1C
#define AMD_SMIREG_SCIS0EN              0x20
#define AMD_SMIREG_SCIMAP0              0x40
#define AMD_SMIREG_SMITRIG              0x98
#define AMD_SMIREG_SMICONTROL0          0xA0
#define AMD_SMICONTROL_MASK             0x03
#define AMD_GEVENT_SCIMASK              0xE0    // And Mask ~ 0x1F

#define IO_DELAY_PORT                   0xDE
#define MAX_TIMER                       0xFFFFFFFFul
#define ELAPSED_TIME_UNIT               28
#define ELAPSED_TIME_DIVIDER            100

#define SB_SATA_BUS                     0x00
#define SB_SATA_DEVICE                  0x11
#define SB_SATA_FUNCTION                0x00

AMD_CPM_SATA_MODE_MASK  SataModeMask[] = {
  {0x7800, 0x01},           // IDE
  {0x7801, 0x02},           // AHCI
  {0x7802, 0x04},           // RAID
  {0x7803, 0x04},           // RAID5
  {0x7804, 0x08},           // AMD AHCI
  {0x7805, 0x04},           // Dot Hill RAID
  {0x780A, 0x04},           // 3rd Party RAID
  {0x4390, 0x01},           // IDE
  {0x4391, 0x02},           // AHCI
  {0x4392, 0x04},           // RAID
  {0x4393, 0x04},           // RAID5
  {0x4394, 0x08},           // AMD AHCI
  {0xFFFF, 0xFF},           //EOT
};

PCIE_BRIDGE_NAME FchPcieBridgeName[] = {
  {0, 21, 0, 0x30, AMD_CPM_FCH_PCIE_GPP0_NAME},
  {0, 21, 1, 0x31, AMD_CPM_FCH_PCIE_GPP1_NAME},
  {0, 21, 2, 0x32, AMD_CPM_FCH_PCIE_GPP2_NAME},
  {0, 21, 3, 0x33, AMD_CPM_FCH_PCIE_GPP3_NAME},
  0
};

UINT8 FchGeventGpioMappingTable[] = {
  0x7E,         // GEVENT0 => GPIO126
  0x81,         // GEVENT1 => GPIO129
  0x03,         // GEVENT2 => GPIO3
  0x16,         // GEVENT3 => GPIO22
  0x04,         // GEVENT4 => GPIO4
  0x15,         // GEVENT5 => GPIO21
  0x0E,         // GEVENT6 => GPIO14
  0x05,         // GEVENT7 => GPIO5
  0x02,         // GEVENT8 => GPIO2
  0x20,         // GEVENT9 => GPIO32
  0x06,         // GEVENT10 => GPIO6
  0x07,         // GEVENT11 => GPIO7
  0x10,         // GEVENT12 => GPIO16
  0x11,         // GEVENT13 => GPIO17
  0x12,         // GEVENT14 => GPIO18
  0x18,         // GEVENT15 => GPIO24
  0x17,         // GEVENT16 => GPIO23
  0x19,         // GEVENT17 => GPIO25
  0x0B,         // GEVENT18 => GPIO11
  0x01,         // GEVENT19 => GPIO1
  0x0F,         // GEVENT20 => GPIO15
  0x0D,         // GEVENT21 => GPIO13
  0x09          // GEVENT22 => GPIO9
};

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmFchGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Value;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Value = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    Value = ((CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin) & 0x80) == 0x80);
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
    Value = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2) & 0x01;
    break;
  }
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of KBC GPIO Pin on reference board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmKbcGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Data;
  UINT8                   PinNum;
  EFI_STATUS              Status;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  PinNum              = (UINT8) Pin;
  Status = CommonFunctionPtr->KbcRead (0x60, 0xA1, &PinNum, &Data);
  if (EFI_ERROR (Status)) {
    CPM_DEADLOOP ();
  }
  return ((Data & 0x80) == 0x80);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 *                      [0x0000: 0x00FF]: FCH GPIO Pin
 *                      [0x0100: 0x01FF]: KBC GPIO Pin on Reference Board
 *                      [0x0200: 0x0FFF]: Reserved
 *
 * @retval    The State of GPIO Pin
 *
 */
UINT8
CpmGetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin
  )
{
  UINT16  Select;
  UINT16  PinNum;
  UINT8   Value;

  Select = Pin >> 8;
  PinNum = Pin & 0xFF;
  Value = 0;
  switch (Select) {
  case 0:
    Value = CpmFchGetGpio (This, PinNum);
    break;
  case 1:
    Value = CpmKbcGetGpio (This, PinNum);
    break;
  }
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set FCH GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. See Definition of AMD_CPM_GPIO_SETTING
 *
 */
VOID
CpmFchSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GPIO_SETTING    GpioPin;
  UINT8                   temp;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  GpioPin.Raw = Value;
  if (GpioPin.Gpio.IoMuxEn) {
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + IOMUX_BASE + Pin, (UINT8) GpioPin.Gpio.IoMux);
  }

  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    temp = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin);
    if (GpioPin.Gpio.PresetEn) {
      temp = (temp & 0xE3) | 0x08;
      if (GpioPin.Gpio.Sticky) {
        temp |= 0x04;
      }
      if (GpioPin.Gpio.PullUp) {
        temp &= 0xF7;
      }
      if (GpioPin.Gpio.PullDown) {
        temp |= 0x10;
      }
    }
    if (GpioPin.Gpio.SetEnB == 0) {
      if (GpioPin.Gpio.OutEnB == 0) {
        temp &= 0x9F;
        if (GpioPin.Gpio.Out) {
          temp |= 0x40;
        }
      } else {
        temp |= 0x20;
      }
    }
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BASE + Pin, temp);
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
    temp = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2);
    if (GpioPin.Gpio.PresetEn) {
      temp = temp & 0xC7;
      if (GpioPin.Gpio.PullUpSel) {
        temp |= 0x08;
      }
      if (GpioPin.Gpio.PullUp) {
        temp |= 0x10;
      }
      if (GpioPin.Gpio.PullDown) {
        temp |= 0x20;
      }
    }
    if (GpioPin.Gpio.SetEnB == 0) {
      if (GpioPin.Gpio.OutEnB == 0) {
        temp |= 0x80;
        if (GpioPin.Gpio.Out) {
          temp |= 0x40;
        } else {
          temp &= 0xBF;
        }
      } else {
        temp &= 0x7F;
      }
    }
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + (Pin << 2) + 2, temp);
    break;
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Set KBC GPIO Pin on Reference Board
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 * @param[in] Value   The Setting of GPIO Pin. 0: Low. 1: High
 *
 */
VOID
CpmKbcSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   PinNum;
  EFI_STATUS              Status;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  PinNum              = (UINT8) (Pin & 0x7F) | (Value == 1 ? 0x80: 0);
  Status = CommonFunctionPtr->KbcWrite (0x60, 0xAF, &PinNum);
  if (EFI_ERROR (Status)) {
    CPM_DEADLOOP ();
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set GPIO Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GPIO Pin Number.
 *                      [0x0000: 0x00FF]: FCH GPIO Pin
 *                      [0x0100: 0x01FF]: KBC GPIO Pin on Reference Board
 *                      [0x0200: 0x0FFF]: Reserved
 * @param[in] Value   The Setting of GPIO Pin.
 *
 */
VOID
CpmSetGpio (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  UINT16  Select;
  UINT16  PinNum;

  Select = Pin >> 8;
  PinNum = Pin & 0xFF;

  switch (Select) {
  case 0:
    CpmFchSetGpio (This, PinNum, Value);
    break;
  case 1:
    CpmKbcSetGpio (This, PinNum, Value);
    break;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the State of GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 *
 * @retval    The State of GEVENT Pin
 *
 */
UINT8
CpmGetGevent (
  IN       VOID                        *This,
  IN       UINT8                       Pin
  )
{
  UINT8   Value;
  Value = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    Value = CpmGetGpio (This, Pin + 0x60);
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
    Value = 0;
    if (Pin <= 22) {
      Value = CpmGetGpio (This, FchGeventGpioMappingTable[Pin]);
    }
    break;
  }
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the State of GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The Setting of GEVENT Pin. See Definition of AMD_CPM_GEVENT_SETTING
 *
 */
VOID
CpmSetGevent (
  IN       VOID                        *This,
  IN       UINT16                      Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GEVENT_SETTING  GeventPin;
  UINT32                  andMask32;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16                  SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  GeventPin.Raw = Value;

  if (Pin < 0x40) {
    SciMap = GeventPin.Gevent.SciMap;
    andMask32 = ~(1 << SciMap);

    //EventEnable: 0-Disable, 1-Enable.
    if (GeventPin.Gevent.EventEnable) {
      if (GeventPin.Gevent.SciTrigAuto) {
        //SciTrig: 0-Falling Edge, 1-Rising Edge.
        CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG,
                                                andMask32,
                                                (CommonFunctionPtr->GetGevent (This, (UINT8) Pin) ? 0 : 1) << SciMap);
      } else {
        //SciTrig: 0-Falling Edge, 1-Rising Edge.
        CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG,
                                                andMask32,
                                                GeventPin.Gevent.SciTrig << SciMap);
      }

      //SciLevl: 0-Edge trigger, 1-Level Trigger.
      CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCILEVEL,
                                              andMask32,
                                              GeventPin.Gevent.SciLevl << SciMap);

      //SmiSciEn: 0-Not send SMI, 1-Send SMI
      CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMISCIEN,
                                              andMask32,
                                              GeventPin.Gevent.SmiSciEn << SciMap);

      //SciS0En: 0-Disable, 1-Enable
      CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCIS0EN,
                                              andMask32,
                                              GeventPin.Gevent.SciS0En << SciMap);

      //SciMap: 00000b ~ 11111b.
      CommonFunctionPtr->MmioAndThenOr8 (     MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCIMAP0 + Pin,
                                              (UINT8)AMD_GEVENT_SCIMASK,
                                              (UINT8)GeventPin.Gevent.SciMap);

      if (Pin < 24) {
        //SmiTrig: 0-Active Low, 1-Active High.
        CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMITRIG,
                                                ~(1 << Pin),
                                                GeventPin.Gevent.SmiTrig << Pin);
      }

      //SmiControl: 0-Disable, 1-SMI, 2-NMI, 3-IRQ13
      CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMICONTROL0 + Pin / 4,
                                              ~(AMD_SMICONTROL_MASK << ((Pin & 0x3) * 2)),
                                              GeventPin.Gevent.SmiControl << ((Pin & 0x3) * 2));
    }
  } else if ((Pin & 0xFF00) == 0x0100) {
    CommonFunctionPtr->MmioWrite16 (MainTablePtr->AcpiMemIoBaseAddr + GPIO_BANK_BASE + ((Pin & 0xFF) << 2), Value);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set SMI Control for GEVENT Pin
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The Setting of GEVENT Pin. See Definition of AMD_CPM_GEVENT_SETTING
 *
 */
VOID
CpmSetSmiControl (
  IN       VOID                        *This,
  IN       UINT8                       Pin,
  IN       UINT16                      Value
  )
{
  AMD_CPM_GEVENT_SETTING  GeventPin;
  UINT32                  andMask32;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16                  SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  GeventPin.Raw = Value;

  if (Pin < 0x40) {
    SciMap = GeventPin.Gevent.SciMap;
    andMask32 = ~(1 << Pin);

    //SmiControl: 0-Disable, 1-SMI, 2-NMI, 3-IRQ13
    CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SMICONTROL0 + Pin / 4,
                                            ~(AMD_SMICONTROL_MASK << ((Pin & 0x3) * 2)),
                                            GeventPin.Gevent.SmiControl << ((Pin & 0x3) * 2));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the way to set corresponding bit in Event Status
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 * @param[in] Value   The bit controls the way to set Event Status.
 *                      0: Active Low. 1: Active High
 *
 */
VOID
CpmSetGeventSciTrig (
  IN       VOID                        *This,
  IN       UINT8                       Pin,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT32                  andMask32;
  UINT8                   SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SciMap = CommonFunctionPtr->GetSciMap (This, Pin);
  andMask32 = ~(1 << SciMap);
  CommonFunctionPtr->MmioAndThenOr32 (    MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SCITRIG,
                                          andMask32,
                                          Value << SciMap);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Trigger a GEVENT SCI interrupt
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Pin     The GEVENT Pin Number.
 *
 */
VOID
CpmSetGeventSci (
  IN       VOID                        *This,
  IN       UINT8                       Pin
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   SciMap;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SciMap = CommonFunctionPtr->GetSciMap (This, Pin);
  CommonFunctionPtr->MmioOr32 (MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIEN, 1 << SciMap);
  CommonFunctionPtr->MmioWrite32 (MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIDATA, 1 << SciMap);
  CommonFunctionPtr->MmioWrite32 (MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIDATA, 0);
  CommonFunctionPtr->MmioWrite32 (MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + AMD_SMIREG_SWSCIEN, 0);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH Revision Id
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The FCH Revision Id
 *
 */
UINT8
CpmGetFchRevisionId (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   FchRevision;
  UINT8                   CpuRevision;
  CPUID_DATA              Data;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  CpuRevision         = CommonFunctionPtr->GetCpuRevisionId (This);
  FchRevision         = CPM_FCH_REVISION_ID_DEFAULT;
  switch (CpuRevision) {
  case CPM_CPU_REVISION_ID_KB:
    FchRevision = CPM_FCH_REVISION_ID_KB;
    break;
  case CPM_CPU_REVISION_ID_ML:
    FchRevision = CPM_FCH_REVISION_ID_ML;
    break;
  case CPM_CPU_REVISION_ID_CZ:
    CommonFunctionPtr->CpuidRead (0x80000001, &Data);
    if ((Data.EBX_Reg & 0xF0000000) != 0x30000000) {
      FchRevision = CPM_FCH_REVISION_ID_CZ;
    }
    break;
  }
  return FchRevision;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH Strap Setting
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The State of FCH Strap Pin
 *              BIT0: ImcEnableStrap: 0: Disable. 1: Enable
 *              BIT1: ClkGenStrap: 0: External. 1: Internal
 *              BIT2: S5+ Support: 0: Disable. 1: Enable
 *
 */
UINT32
CpmGetStrap (
  IN       VOID                        *This
  )
{
  UINT8                   StrapStatus;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT32                  StrapValue;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  StrapValue = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
    StrapStatus = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x80);
    StrapValue |= ((StrapStatus & BIT2) >> 2) | ((StrapStatus & BIT4) >> 3);
    StrapStatus = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x82);
    StrapValue |= StrapStatus & BIT2;
    break;
  case CPM_FCH_REVISION_ID_KB:
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
    StrapStatus = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x80);
    StrapValue |= (StrapStatus & BIT2) >> 2;
    StrapStatus = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x82);
    StrapValue |= StrapStatus & BIT1;
    break;
  }
  return StrapValue;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set FCH ClkReq
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] ClkId   PCIE Clock Pin Number
 * @param[in] ClkReq  ClkReq Mapping
 *
 */
VOID
CpmSetClkReq (
  IN       VOID                        *This,
  IN       UINT8                       ClkId,
  IN       UINT8                       ClkReq
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   Flag;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Flag = 0;
  switch (((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb) {
  case CPM_FCH_REVISION_ID_DEFAULT:
    if (ClkId <= 9) {
      Flag = 1;
    } else {
      switch (ClkId) {
      case CPM_CLKID_APU_CLK:
      case CPM_CLKID_DISP2_CLK:
      case CPM_CLKID_PCIE_RCLK_OUTPUT:
      case CPM_CLKID_DISP_CLK:
        CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05,
                                        0xFE ^ (1 << ((ClkId & 0x7F) + 1)),
                                        0x01 | ((ClkReq ? 0 : 1) << ((ClkId & 0x7F) + 1))
                                        );
        break;
      case CPM_CLKID_PCIE_RCLK:
        CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05,
                                        0xDF,
                                        ClkReq ? 0 : 0x20
                                        );
        break;
      case CPM_CLKID_CLOCK_BUFFER_BIAS:
        CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x06,
                                        0xFE,
                                        ClkReq ? 0 : 1
                                        );
        break;
      case CPM_CLKID_OSCOUT2_OUTOFF:
        CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x40,
                                        0x7F,
                                        ClkReq ? 0 : 0x80
                                        );
        break;
      }
    }
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_KB:
  case CPM_FCH_REVISION_ID_CZ:
    if (ClkId <= 3 || ClkId == 9) {
      Flag = 1;
    } else if (ClkId == CPM_CLKID_PCIE_RCLK) {
      CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + 0x05,
                                        0xDF,
                                        ClkReq ? 0 : 0x20
                                        );
    }
    break;
  }

  if (Flag) {
    CommonFunctionPtr->MmioAndThenOr8 ( MainTablePtr->AcpiMemIoBaseAddr + MISC_BASE + (ClkId >> 1),
                                        0x0F << (ClkId % 2 ? 0: 4),
                                        ClkReq << (ClkId % 2 ? 4: 0)
                                        );
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Add some delay in 1us/unit
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Delay   Delay in 1us/unit
 *
 */
VOID
CpmStall (
  IN       VOID                        *This,
  IN       UINT32                      Delay
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT16 timerAddr;
  UINT32 startTime;
  UINT32 elapsedTime;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  timerAddr = CommonFunctionPtr->MmioRead16 (MainTablePtr->AcpiMemIoBaseAddr + PMIO_BASE + 0x64);
  if ( timerAddr == 0 ) {
    Delay = Delay / 2;
    while ( Delay != 0 ) {
      CommonFunctionPtr->IoRead8 (IO_DELAY_PORT);
      Delay --;
    }
  } else {
    startTime = CommonFunctionPtr->IoRead32 (timerAddr);
    for ( ;; ) {
      elapsedTime = CommonFunctionPtr->IoRead32 (timerAddr);
      if ( elapsedTime < startTime ) {
        elapsedTime = elapsedTime + MAX_TIMER - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }
      if ((elapsedTime * ELAPSED_TIME_UNIT / ELAPSED_TIME_DIVIDER) > Delay) {
        break;
      }
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set Fan On
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Fan     Fan Number
 *
 */
VOID
CpmSetFanOn (
  IN       VOID                        *This,
  IN       UINT8                       Fan
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10, 0x07);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set ProcHot
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Enable  Prochot Enable: 0: Disable. 1: Enable
 * @param[in] Fan     FanOut Pin Number
 * @param[in] Freq    Frequency for FanOut Pin
 *
 */
VOID
CpmSetProchot (
  IN       VOID                        *This,
  IN       UINT8                       Enable,
  IN       UINT8                       Fan,
  IN       UINT8                       Freq
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  if (Enable) {
    CommonFunctionPtr->MmioOr8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x01, 0x04);            // SET FAN Active with high
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x02, Freq);         // SET FAN Frequence
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x03, 0x01);         // SET FAN Duty number
    CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10 + 0x04, 0x00);
    CommonFunctionPtr->MmioAndThenOr8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10, 0xF8, 0x06);      // SET FAN enabled as manually control
  } else {
    CommonFunctionPtr->MmioOr8 (MainTablePtr->AcpiMemIoBaseAddr + PMIO2_BASE + Fan * 0x10, 0x07);                   // Disable FAN for PROCHOT
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Current SATA Mode
 *
 *
 * @param[in] This    The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    The SATA Mode
 *              BIT0: IDE Mode
 *              BIT1: AHCI Mode
 *              BIT2: RAID Mode
 *              BIT3: AMD AHCI Mode
 *
 */
UINT8
CpmGetSataMode (
  IN      VOID                        *This
  )
{
  UINT8                   Mask;
  UINT16                  DeviceId;
  AMD_CPM_SATA_MODE_MASK  *TablePtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  Mask = 0;
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  DeviceId = CommonFunctionPtr->PciRead16 (This, SB_SATA_BUS, SB_SATA_DEVICE, SB_SATA_FUNCTION, 0x02);
  TablePtr = &SataModeMask[0];
  while ( TablePtr->DeviceId != 0xFFFF ) {
    if ( TablePtr->DeviceId == DeviceId ) {
      Mask = TablePtr->Mask;
      break;
    }
    TablePtr ++;
  }
  return ( Mask );
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether the device is in FCH
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Device    The PCI Device Number
 * @param[in] Function  The PCI Function Number
 *
 * @retval    TRUE      The Device is in FCH
 *            FALSE     The Device is not in FCH
 *
 */
BOOLEAN
EFIAPI
CpmIsFchDevice (
  IN       VOID                        *This,
  IN       UINT8                       Device,
  IN       UINT8                       Function
  )
{
  if (Device <= 0x10) {
    return 0;
  } else {
    return 1;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get FCH PCIe Bridge Name in ASL Code
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Device    The PCI Device Number
 * @param[in] Function  The PCI Function Number
 *
 * @retval    PCIe Bridge Name in ASL code
 *
 */
UINT32
CpmGetFchPcieAslName (
  IN       VOID                       *This,
  IN       UINT8                      Device,
  IN       UINT8                      Function
  )
{
  PCIE_BRIDGE_NAME  *BridgeNamePtr;
  UINT32  Name;

  Name = 0;
  BridgeNamePtr = &FchPcieBridgeName[0];
  while (BridgeNamePtr->Device != 0) {
    if (BridgeNamePtr->Device == Device && BridgeNamePtr->Function == Function) {
#ifdef AMD_CPM_EDKII
      switch (BridgeNamePtr->NameId) {
      case 0x30:
        Name = PcdGet32 (FchPcieGpp0AslName);
        break;
      case 0x31:
        Name = PcdGet32 (FchPcieGpp1AslName);
        break;
      case 0x32:
        Name = PcdGet32 (FchPcieGpp2AslName);
        break;
      case 0x33:
        Name = PcdGet32 (FchPcieGpp3AslName);
        break;
      }
      if (!Name) {
        Name = BridgeNamePtr->Name;
      }
#else
      Name = BridgeNamePtr->Name;
#endif
      break;
    }
    BridgeNamePtr ++;
  }
  return Name;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the Mapping Bit Number of GEVENT pin in Event Status
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] GeventPin GEVENT Pin Number
 *
 * @retval    SCI Mapping Bit Number
 *
 */
UINT8
CpmGetSciMap (
  IN       VOID                        *This,
  IN       UINT8                       GeventPin
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;

  return CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + 0x40 + GeventPin);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Current Boot Mode
 *
 *
 * @retval    Boot Mode
 *
 */
UINT8
CpmGetBootMode (
  )
{
  UINT16                  Address;
  UINT16                  Value;

  CpmIoWrite8 (0xCD6, 0x63);
  Address = (UINT16)CpmIoRead8 (0xCD7);
  CpmIoWrite8 (0xCD6, 0x62);
  Address = Address * 0x100 + (UINT16)CpmIoRead8 (0xCD7);
  Value = CpmIoRead16 (Address) & 0x1C00;

  return (UINT8) (Value >> 10);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check whether it is a RTC wake up
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 * @retval    TRUE      RTC wake up
 *            FALSE     Not RTC wake up
 *
 */
BOOLEAN
EFIAPI
CpmIsRtcWakeup (
  IN       VOID                        *This
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  UINT8                   Status;
  UINT8                   Enable;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  Status = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + 0x01);
  Enable = CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + 0x03);
  if ((Status & BIT2) && (Enable & BIT2)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read RTC register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of RTC register
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetRtc (
  IN       VOID                        *This,
  IN       UINT8                       Index
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  return CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + CMOS_BASE + Index);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write RTC register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of RTC register
 * @param[in] Value     The value of RTC register
 *
 */
VOID
EFIAPI
CpmSetRtc (
  IN       VOID                        *This,
  IN       UINT8                       Index,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + CMOS_BASE + Index, Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read ACPI register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of ACPI register
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetAcpi (
  IN       VOID                        *This,
  IN       UINT8                       Index
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  return CommonFunctionPtr->MmioRead8 (MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + Index);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write ACPI register
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Index     The index of ACPI register
 * @param[in] Value     The value of ACPI register
 *
 */
VOID
EFIAPI
CpmSetAcpi (
  IN       VOID                        *This,
  IN       UINT8                       Index,
  IN       UINT8                       Value
  )
{
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr->MmioWrite8 (MainTablePtr->AcpiMemIoBaseAddr + ACPI_BASE + Index, Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read the data from Save Context Area
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Offset    The offset of data to read
 *
 * @retval    RTC register value
 *
 */
UINT8
EFIAPI
CpmGetSaveContext (
  IN       VOID                        *This,
  IN       UINT8                       Offset
  )
{
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  UINT8                       Data;
  UINT32                      Address;
  UINT8                       Flag;

  Data = 0;
  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (This, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    Flag = Offset & 0x80;
    Offset = Offset & 0x7F;
    if (Flag && (Offset < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset;
      Data = CommonFunctionPtr->MmioRead8 (Address + (UINT32)Offset);
    } else if (Offset < SaveContextTablePtr->BufferSize - sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT)) {
      Address = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset + sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT);
      Data = CommonFunctionPtr->MmioRead8 (Address + (UINT32)Offset);
    } else {
      CPM_DEADLOOP ();
    }
  }
  return Data;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write the data to Save Context Area
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 * @param[in] Offset    The offset of data to read
 * @param[in] Data      The data to write
 *
 */
VOID
EFIAPI
CpmSetSaveContext (
  IN       VOID                        *This,
  IN       UINT8                       Offset,
  IN       UINT8                       Data
  )
{
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  UINT32                      Address;
  UINT8                       Flag;

  CommonFunctionPtr   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  MainTablePtr        = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (This, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    Flag = Offset & 0x80;
    Offset = Offset & 0x7F;
    if (Flag && (Offset < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset;
      CommonFunctionPtr->MmioWrite8 (Address + (UINT32)Offset, Data);
    } else if (!Flag && (Offset < SaveContextTablePtr->BufferSize - sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT))) {
      Address = MainTablePtr->AcpiMemIoBaseAddr + 0x100 * SaveContextTablePtr->BufferType + SaveContextTablePtr->BufferOffset + sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT);
      CommonFunctionPtr->MmioWrite8 (Address + (UINT32)Offset, Data);
    } else {
      CPM_DEADLOOP ();
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Register CPM Common FCH Function
 *
 *
 * @param[in] This      The pointer of AMD CPM Table Ppi or Protocol
 *
 */
VOID
EFIAPI
CpmRegisterFch (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;
  UINT8                   Data[4];
  UINT8                   FchRevision;

  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  FchRevision = CpmGetFchRevisionId (This);
  ((AMD_CPM_TABLE_PPI*)This)->ChipId.Sb = FchRevision;

  switch (FchRevision) {
  case CPM_FCH_REVISION_ID_DEFAULT:
  case CPM_FCH_REVISION_ID_KB:
    CommonFunctionPtr->IoWrite8 (0xCD6, 0x24);
    Data[0] = CommonFunctionPtr->IoRead8 (0xCD7);
    if (!(Data[0] & BIT0)) {
      CommonFunctionPtr->IoWrite8 (0xCD7, Data[0] | BIT0);
    }
    if (!MainTablePtr->AcpiMemIoBaseAddr) {
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x25);
      Data[1] = CommonFunctionPtr->IoRead8 (0xCD7);
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x26);
      Data[2] = CommonFunctionPtr->IoRead8 (0xCD7);
      CommonFunctionPtr->IoWrite8 (0xCD6, 0x27);
      Data[3] = CommonFunctionPtr->IoRead8 (0xCD7);
      MainTablePtr->AcpiMemIoBaseAddr = (Data[3] << 24) + (Data[2] << 16) + (Data[1] << 8);
    }
    break;
  case CPM_FCH_REVISION_ID_ML:
  case CPM_FCH_REVISION_ID_CZ:
    MainTablePtr->AcpiMemIoBaseAddr = 0xFED80000;
    break;
  }

  CommonFunctionPtr->GetGpio          = CpmGetGpio;
  CommonFunctionPtr->SetGpio          = CpmSetGpio;
  CommonFunctionPtr->GetGevent        = CpmGetGevent;
  CommonFunctionPtr->SetGevent        = CpmSetGevent;
  CommonFunctionPtr->SetSmiControl    = CpmSetSmiControl;
  CommonFunctionPtr->SetGeventSciTrig = CpmSetGeventSciTrig;
  CommonFunctionPtr->SetGeventSci     = CpmSetGeventSci;
  CommonFunctionPtr->GetStrap         = CpmGetStrap;
  CommonFunctionPtr->SetClkReq        = CpmSetClkReq;
  CommonFunctionPtr->Stall            = CpmStall;
  CommonFunctionPtr->SetFanOn         = CpmSetFanOn;
  CommonFunctionPtr->SetProchot       = CpmSetProchot;
  CommonFunctionPtr->GetSataMode      = CpmGetSataMode;
  CommonFunctionPtr->IsFchDevice      = CpmIsFchDevice;
  CommonFunctionPtr->GetSciMap        = CpmGetSciMap;
  CommonFunctionPtr->GetFchPcieAslName  = CpmGetFchPcieAslName;
  CommonFunctionPtr->GetBootMode      = CpmGetBootMode;
  CommonFunctionPtr->IsRtcWakeup      = CpmIsRtcWakeup;
  CommonFunctionPtr->GetRtc           = CpmGetRtc;
  CommonFunctionPtr->SetRtc           = CpmSetRtc;
  CommonFunctionPtr->GetAcpi          = CpmGetAcpi;
  CommonFunctionPtr->SetAcpi          = CpmSetAcpi;
  CommonFunctionPtr->GetSaveContext   = CpmGetSaveContext;
  CommonFunctionPtr->SetSaveContext   = CpmSetSaveContext;

  return;
}
