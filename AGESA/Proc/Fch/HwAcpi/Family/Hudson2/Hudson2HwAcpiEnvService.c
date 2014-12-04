/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Fch HwAcpi controller
 *
 * Init HwAcpi Controller features.
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
#include "amdlib.h"
#include "cpuServices.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_HWACPI_FAMILY_HUDSON2_HUDSON2HWACPIENVSERVICE_FILECODE

#define AMD_CPUID_APICID_LPC_BID    0x00000001ul  // Local APIC ID, Logical Processor Count, Brand ID

/**
 * FchInitEnvHwAcpiMmioTable - Fch ACPI MMIO initial
 * during POST.
 *
 */
ACPI_REG_WRITE FchHudson2InitEnvHwAcpiMmioTable[] =
{
  {00, 00, 0xB0, 0xAC},                                         /// Signature

  //
  // HPET workaround
  //
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54 + 3, 0xFC, BIT0 + BIT1},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54 + 2, 0x7F, BIT7},
  {PMIO_BASE >> 8,  FCH_PMIOA_REG54 + 2, 0x7F, 0x00},
  //
  // Enable Hudson-2 A12 ACPI bits at PMIO 0xC0 [30, 10:3]
  // ClrAllStsInThermalEvent 3 Set to 1 to allow ASF remote power down/power cycle, Thermal event, Fan slow event to clear all the Gevent status and enabled bits. The bit should be set to 1 all the time.
  // UsbGoodClkDlyEn         4 Set to 1 to delay de-assertion of Usb clk by 6 Osc clk. The bit should be set to 1 all the time.
  // ForceNBCPUPwr           5 Set to 1 to force CPU pwrGood to be toggled along with NB pwrGood.
  // MergeUsbPerReq          6 Set to 1 to merge usb perdical traffic into usb request as one of break event.
  // IMCWatchDogRstEn        7 Set to 1 to allow IMC watchdog timer to reset entire acpi block. The bit should be set to 1 when IMC is enabled.
  // GeventStsFixEn          8 1: Gevent status is not reset by its enable bit. 0: Gevent status is reset by its enable bit.
  // PmeTimerFixEn           9 Set to 1 to reset Pme Timer when going to sleep state.
  // UserRst2EcEn           10 Set to 1 to route user reset event to Ec. The bit should be set to 1 when IMC is enabled.
  // Smbus0ClkSEn           30 Set to 1 to enable SMBus0 controller clock stretch support.
  //
  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xFB, BIT2},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC0, 0, 0xF9},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC0 + 1, 0x04, 0x07},
  //
  // RtcSts              19-17 RTC_STS set only in Sleep State.
  // GppPme                 20 Set to 1 to enable PME request from SB GPP.
  // Pcireset               22 Set to 1 to allow SW to reset PCIe.
  //
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2, 0x20, 0x58},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2 + 1, 0, 0x40},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC2, 0xEF, BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REGC4, 0xFB, 0},

  {PMIO_BASE >> 8, FCH_PMIOA_REGCC, 0xF8, 0x01},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74, 0x00, BIT0 + BIT1 + BIT2 + BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REG74 + 3, 0xDF, 0},
  {PMIO_BASE >> 8, FCH_PMIOA_REGDE + 1, 0xFC, BIT0 + BIT1},
  {PMIO_BASE >> 8, FCH_PMIOA_REGDE, 0xEF, BIT4},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBA, 0xF7, BIT3},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBA + 1, 0xBF, BIT6},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBC, 0xFD, BIT1},
  {PMIO_BASE >> 8, FCH_PMIOA_REGDC, 0x7C, BIT0},                 /// Hiding Flash Controller PM_IO 0xDC[7] = 0x0 & PM_IO 0xDC [1:0]=0x01
  {PMIO_BASE >> 8, FCH_PMIOA_REGBF, 0xFE, 0},
  {PMIO_BASE >> 8, FCH_PMIOA_REGBE, 0xFE, BIT0},

  {SMI_BASE >> 8, FCH_SMI_Gevent1, 0, 1},
  {SMI_BASE >> 8, FCH_SMI_Gevent3, 0, 3},
  {SMI_BASE >> 8, FCH_SMI_Gevent4, 0, 4},
  {SMI_BASE >> 8, FCH_SMI_Gevent5, 0, 5},
  {SMI_BASE >> 8, FCH_SMI_Gevent6, 0, 6},
  {SMI_BASE >> 8, FCH_SMI_Gevent23, 0, 23},
  {SMI_BASE >> 8, FCH_SMI_xHC0Pme, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_xHC1Pme, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup0, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup1, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup2, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_Usbwakup3, 0, 11},
  {SMI_BASE >> 8, FCH_SMI_IMCGevent0, 0, 12},
  {SMI_BASE >> 8, FCH_SMI_FanThGevent, 0, 13},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme0, 0, 15},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme1, 0, 16},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme2, 0, 17},
  {SMI_BASE >> 8, FCH_SMI_SBGppPme3, 0, 18},
  {SMI_BASE >> 8, FCH_SMI_GecPme, 0, 19},
  {SMI_BASE >> 8, FCH_SMI_CIRPme, 0, 28},
  {SMI_BASE >> 8, FCH_SMI_Gevent8, 0, 24},
  {SMI_BASE >> 8, FCH_SMI_AzaliaPme, 0, 27},
  {SMI_BASE >> 8, FCH_SMI_SataGevent0, 0, 30},
  {SMI_BASE >> 8, FCH_SMI_SataGevent1, 0, 31},
  {SMI_BASE >> 8, FCH_SMI_REG08,  0xE7, 0},
  {SMI_BASE >> 8, FCH_SMI_REG0C + 2, 0xF7, BIT3},
  {SMI_BASE >> 8, FCH_SMI_TWARN, 0, 9},
  {SMI_BASE >> 8, FCH_SMI_REG3C,  0, BIT6},
  {SMI_BASE >> 8, FCH_SMI_REG84 + 2,  0, BIT7},

  //
  // CG PLL CMOX Clock Driver Setting for power saving
  //
  {MISC_BASE >> 8, FCH_MISC_REG18 + 0x06, 0x18, 0xE0},
  {MISC_BASE >> 8, FCH_MISC_REG18 + 0x07, 0x00, 0x1F},

  {MISC_BASE >> 8, FCH_MISC_REG50 + 3, 0xDF, BIT5},
  {MISC_BASE >> 8, FCH_MISC_REG50 + 2, 0xF7, BIT3},
  {MISC_BASE >> 8, FCH_MISC_REG50 + 1, 0xFE, BIT0},
  //{SERIAL_DEBUG_BASE >> 8, FCH_SDB_REG74, 0, 0},
  {0xFF, 0xFF, 0xFF, 0xFF},
};

/**
 * FchHudson2InitEnvHwAcpiPciTable - PCI device registers initial
 * during early POST.
 *
 */
REG8_MASK FchHudson2InitEnvHwAcpiPciTable[] =
{
  //
  // SMBUS Device (Bus 0, Dev 20, Func 0)
  //
  {0x00, SMBUS_BUS_DEV_FUN, 0},
  {FCH_CFG_REG10, 0X00, (FCH_VERSION & 0xFF)},                ///Program the version information
  {FCH_CFG_REG11, 0X00, (FCH_VERSION >> 8)},
  {0xFF, 0xFF, 0xFF},
};


/**
 * ProgramPFchAcpiMmio - Config HwAcpi MMIO registers
 *   Acpi S3 resume won't execute this procedure (POST only)
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramEnvPFchAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE*) (&FchHudson2InitEnvHwAcpiMmioTable[0]), StdHeader);
}

/**
 * ProgramFchEnvHwAcpiPciReg - Config HwAcpi PCI controller
 * before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvHwAcpiPciReg (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // FCH CFG programming
  //
  //Early post initialization of pci config space
  //
  ProgramPciByteTable ((REG8_MASK*) (&FchHudson2InitEnvHwAcpiPciTable[0]), sizeof (FchHudson2InitEnvHwAcpiPciTable) / sizeof (REG8_MASK), StdHeader);

  if ( LocalCfgPtr->Smbus.SmbusSsid != NULL ) {
    RwPci ((SMBUS_BUS_DEV_FUN << 16) + FCH_CFG_REG2C, AccessWidth32, 0x00, LocalCfgPtr->Smbus.SmbusSsid, StdHeader);
  }
  if ( LocalCfgPtr->Misc.NoneSioKbcSupport ) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) ( BIT2 + BIT1), BIT1);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGED, AccessWidth8, ~(UINT32) ( BIT2 + BIT1), 0);
  }
}

/**
 * FchVgaInit - Config VGA CODEC
 *
 * @param[in] VOID empty
 *
 */
VOID
FchVgaInit (
  OUT VOID
  )
{
  //
  // Cobia_Nutmeg_DP-VGA Electrical SI validation_Lower RGB Luminance level BGADJ=0x1F & DACADJ=0x1B
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC4, AccessWidth8, 0xff, BIT5 );
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8, AccessWidth8, 0x00, 0x17 );
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD9, AccessWidth8, 0x00, ((BGADJ << 2) + (((DACADJ & 0xf0) >> 4) & 0x3)));
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8, AccessWidth8, 0x00, 0x16 );
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD9, AccessWidth8, 0x0f, ((DACADJ & 0x0f) << 4));

  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x00))) = (0x08 << 4) + (UINT8) ((EFUS_DAC_ADJUSTMENT_CONTROL >> 16) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x01))) = (UINT8) ((EFUS_DAC_ADJUSTMENT_CONTROL >> 8) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x02))) = (UINT8) ((EFUS_DAC_ADJUSTMENT_CONTROL >> 0) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x03))) = (UINT8) (0x03);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x04))) = (UINT8) (((EFUS_DAC_ADJUSTMENT_CONTROL_DATA) >> 0) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x05))) = (UINT8) (((EFUS_DAC_ADJUSTMENT_CONTROL_DATA) >> 8) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x06))) = (UINT8) (((EFUS_DAC_ADJUSTMENT_CONTROL_DATA) >> 16) & 0xff);
  *((UINT8*) ((UINTN)(PKT_DATA_REG + 0x07))) = (UINT8) (((EFUS_DAC_ADJUSTMENT_CONTROL_DATA) >> 24) & 0xff);
  *((UINT8*) ((UINTN)(PKT_LEN_REG))) = 0x08;
  *((UINT8*) ((UINTN)(PKT_CTRL_REG))) = 0x01;
}

/**
 * ProgramSpecificFchInitEnvAcpiMmio - Config HwAcpi MMIO before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramSpecificFchInitEnvAcpiMmio (
  IN  VOID     *FchDataPtr
  )
{
  CPUID_DATA      CpuId;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

#ifdef ACPI_SLEEP_TRAP
  //
  // Set SLP_TYPE as SMI event
  //
  RwMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0, AccessWidth8, ~(BIT2 + BIT3), BIT2);

  //
  // Disabled SLP function for S1/S3/S4/S5
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE, AccessWidth8, ~(UINT32) BIT5, 0x00);

  //
  // Set S state transition disabled (BIT0) force ACPI to send SMI message when writing to SLP_TYP Acpi register. (BIT1)
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG08 + 3, AccessWidth8, ~(BIT0 + BIT1), BIT1);

  //
  // Enabled Global Smi ( BIT7 clear as 0 to enable )
  //
  RwMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98 + 3 , AccessWidth8, ~(UINT32) BIT7, 0x00);
#endif

  //
  // Set Stutter timer settings
  //
  LibAmdCpuidRead (AMD_CPUID_APICID_LPC_BID, &CpuId, StdHeader);

  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG80 + 1, AccessWidth8, ~(UINT32) (BIT3 + BIT4), BIT3 + BIT4);

  //
  // Set LDTSTP# duration to 10us for Specific CPU, or when HT link is 200MHz
  //
  if ((LocalCfgPtr->HwAcpi.AnyHt200MhzLink) || ((CpuId.EAX_Reg & 0x00ff00f0) == 0x100080) || ((CpuId.EAX_Reg & 0x00ff00f0) == 0x100090) || ((CpuId.EAX_Reg & 0x00ff00f0) == 0x1000A0)) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG94, AccessWidth8, 0, 0x0A);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG80 + 3, AccessWidth8, 0xFE, 0x28);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG94, AccessWidth8, 0, 0x01);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG80 + 3, AccessWidth8, 0xFE, 0x20);
  }

  //
  // SSC will provide better jitter margin
  //
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x13, AccessWidth8, 0xFC, 0x01);
  //
  // Ac Loss Control
  //
  AcLossControl ((UINT8) LocalCfgPtr->HwAcpi.PwrFailShadow);
  //
  //FCH VGA Init
  //
  FchVgaInit ();

  //
  // 2.16 Enable DMAACTIVE
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG7F, AccessWidth8, 0xFE, 0x01);

  //
  // Set ACPIMMIO by OEM Input table
  //
  ProgramFchAcpiMmioTbl ((ACPI_REG_WRITE *) (LocalCfgPtr->HwAcpi.OemProgrammingTablePtr), StdHeader);
}

/**
 * ValidateFchVariant - Validate FCH Variant
 *
 *
 *
 * @param[in] FchDataPtr
 *
 */
VOID
ValidateFchVariant (
  IN  VOID     *FchDataPtr
  )
{
  UINT8                  XhciEfuse;
  UINT8                  PcieEfuse;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  switch ( LocalCfgPtr->Misc.FchVariant ) {
  case FCH_M3T:
    //Disable Devices for M3T
    LocalCfgPtr->Gec.GecEnable = 1;
    LocalCfgPtr->Hwm.HwMonitorEnable = 0;
    LocalCfgPtr->Sd.SdConfig = 0;
    LocalCfgPtr->Ir.IrConfig = 0;
    break;

  default:
    break;
  }

  // add Efuse checking for Xhci enable/disable
  XhciEfuse = XHCI_EFUSE_LOCATION;
  GetEfuseStatus (&XhciEfuse, StdHeader);
  if ((XhciEfuse & (BIT0 + BIT1)) == (BIT0 + BIT1)) {
    LocalCfgPtr->Usb.Xhci0Enable = 0;
    LocalCfgPtr->Usb.Xhci1Enable = 0;
  }

  // add Efuse checking for PCIE Gen2 enable
  PcieEfuse = PCIE_FORCE_GEN1_EFUSE_LOCATION;
  GetEfuseStatus (&PcieEfuse, StdHeader);
  if ( PcieEfuse & BIT0 ) {
    LocalCfgPtr->Gpp.GppGen2 = 0;
  }
}

/**
 * IsExternalClockMode - Is External Clock Mode?
 *
 *
 * @retval  TRUE or FALSE
 *
 */
BOOLEAN
IsExternalClockMode (
  IN  VOID     *FchDataPtr
  )
{
  UINT8    MISC80;
  ReadMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80, AccessWidth8, &MISC80);
  return ( (BOOLEAN) ((MISC80 & BIT4) == 0) );
}


/**
 * ProgramFchEnvSpreadSpectrum - Config SpreadSpectrum before
 * PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
ProgramFchEnvSpreadSpectrum (
  IN  VOID     *FchDataPtr
  )
{
  UINT8        PortStatus;
  UINT8        FchSpreadSpectrum;

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchSpreadSpectrum = LocalCfgPtr->HwAcpi.SpreadSpectrum;

  if ((FchSpreadSpectrum > 0) && !(IsExternalClockMode (FchDataPtr))) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x40, AccessWidth32, (UINT32) (~(0x1 << 25)), (UINT32) (0x1 << 25));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, (UINT32) (~(0x1 << 0)), (UINT32) (0x0 << 0));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x18, AccessWidth32, (UINT32) (~(0x7FF << 5)), (UINT32) (0x318 << 5));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x18, AccessWidth32, (UINT32) (~(0xF << 16)), (UINT32) (0x0 << 16));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x10, AccessWidth32, (UINT32) (~(0xFFFF << 8)), (UINT32) (0x7296 << 8));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x10, AccessWidth32, (UINT32) (~(0xFF << 0)), (UINT32) (0x94 << 0));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x1C, AccessWidth32, (UINT32) (~(0x3F << 0)), (UINT32) (0x0 << 0));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, (UINT32) (~(0xF << 28)), (UINT32) (0x9 << 28));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, (UINT32) (~(0x1 << 7)), (UINT32) (0x0 << 7));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth32, (UINT32) (~(0x1 << 8)), (UINT32) (0x1 << 8));
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x10, AccessWidth32, (UINT32) (~(0x3 << 24)), (UINT32) (0x1 << 24));

    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG08, AccessWidth8, 0xFE, 0x01);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG08, AccessWidth8, 0xFE, 0x00);
  }

  //
  // PLL 100Mhz Reference Clock Buffer setting for internal clock generator mode (BIT5)
  // OSC Clock setting for  internal clock generator mode (BIT6)
  //
  GetChipSysMode (&PortStatus, StdHeader);
  if ( ((PortStatus & ChipSysIntClkGen) == ChipSysIntClkGen) ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG04 + 1, AccessWidth8, ~(UINT32) (BIT5 + BIT6), BIT5 + BIT6);
  }
  //
  // Power down DISP2_CLK and PCIE_RCLK_Output for power savings
  //
  if ( LocalCfgPtr->HwAcpi.PwrDownDisp2ClkPcieR ) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG04 + 1, AccessWidth8, ~(UINT32) (BIT0 + BIT2 + BIT3), BIT0 + BIT2 + BIT3);
  } else {
    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG04 + 1, AccessWidth8, ~(UINT32) (BIT0 + BIT2 + BIT3), 0);
  }
}

/**
 * TurnOffCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
TurnOffCG2 (
  OUT VOID
  )
{
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x40, AccessWidth8, ~(UINT32) BIT6, 0);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGDA, AccessWidth8, 0x0F, 0xA0);
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + 0x41, AccessWidth8, ~(UINT32) (BIT1 + BIT0), (BIT1 + BIT0));
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x41, AccessWidth8, ~(UINT32) ( BIT4), (BIT4));
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x41, AccessWidth8, ~(UINT32) (BIT6), (BIT6));
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x08, AccessWidth8, ~(UINT32) BIT6, BIT6);
  RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x1C, AccessWidth8, ~(UINT32) BIT6, BIT6);
}

/**
 * BackUpCG2
 *
 *
 * @retval  VOID
 *
 */
VOID
BackUpCG2 (
  OUT VOID
  )
{
  UINT8 Byte;
  ReadMem (ACPI_MMIO_BASE + MISC_BASE + 0x1C, AccessWidth8, &Byte);
  if (Byte & BIT6) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0x41, AccessWidth8, ~(UINT32) (BIT6), (0));
  }
}

/**
 * HpetInit - Program Fch HPET function
 *
 *
 *
 * @param[in] FchDataPtr         Fch configuration structure pointer.
 *
 */
VOID
HpetInit (
  IN  VOID     *FchDataPtr
  )
{
  DESCRIPTION_HEADER   *HpetTable;
  UINT8                FchHpetTimer;
  UINT8                FchHpetMsiDis;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  FchHpetTimer = (UINT8) LocalCfgPtr->Hpet.HpetEnable;
  FchHpetMsiDis = (UINT8) LocalCfgPtr->Hpet.HpetMsiDis;

  HpetTable = NULL;
  if ( FchHpetTimer == TRUE ) {
    //
    //Program the HPET BAR address
    //
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG50, AccessWidth32, FCH_HPET_REG_MASK, LocalCfgPtr->Hpet.HpetBase);

    //
    //Enabling decoding of HPET MMIO
    //Enable HPET MSI support
    //Enable High Precision Event Timer (also called Multimedia Timer) interrupt
    //
    if ( FchHpetMsiDis == FALSE ) {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG50, AccessWidth32, FCH_HPET_REG_MASK, BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
#ifdef FCH_TIMER_TICK_INTERVAL_WA
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG50, AccessWidth32, FCH_HPET_REG_MASK, BIT0 + BIT1);
#endif
    } else {
      RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG50, AccessWidth32, FCH_HPET_REG_MASK, BIT0 + BIT1);
    }

  } else {
    if ( ! (LocalCfgPtr->Misc.S3Resume) ) {
      HpetTable = (DESCRIPTION_HEADER*) AcpiLocateTable ('TEPH');
    }
    if ( HpetTable != NULL ) {
      HpetTable->Signature = 'HPET';
    }
  }
}
