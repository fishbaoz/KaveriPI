/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEI LIBRARY
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH PEI LIBRARY
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
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
 ******************************************************************************
 */
#include "Tiano.h"
#include "Pei.h"

#include "FchPeiLib.h"
#define FILECODE UEFI_PEI_FCHPEILIB_FCHPEILIB_FILECODE


/*********************************************************************************
 * Name: LibFchMemRead8
 *
 * Description
 *   Read 8-bit data from memory
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *   Address     : Memory address
 *
 * Output
 *   the read data
 *
 *********************************************************************************/
UINT8
LibFchMemRead8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address
  )
{
  PEI_CPU_IO_PPI       *CpuIo;
  UINT8                Data;

  CpuIo = (*PeiServices)->CpuIo;
  Data = CpuIo->MemRead8 (
                  PeiServices,
                  CpuIo,
                  Address
                  );
  return (Data);
}


/*********************************************************************************
 * Name: LibFchMemWrite8
 *
 * Description
 *   Read 8-bit data from memory
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *   Address     : Memory address
 *
 * Output
 *   the read data
 *
 *********************************************************************************/
VOID
LibFchMemWrite8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             *Value
  )
{
  PEI_CPU_IO_PPI       *CpuIo;

  CpuIo = (*PeiServices)->CpuIo;
  CpuIo->MemWrite8 (
           PeiServices,
           CpuIo,
           Address,
           *Value
           );
}


VOID
LibFchMemRMW8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             AndMask,
  IN       UINT8             OrMask
  )
{
  PEI_CPU_IO_PPI       *CpuIo;
  UINT8                Data;

  CpuIo = (*PeiServices)->CpuIo;
  Data = CpuIo->MemRead8 (
                  PeiServices,
                  CpuIo,
                  Address
                  );
  Data &= AndMask;
  Data |= OrMask;
  CpuIo->MemWrite8 (
           PeiServices,
           CpuIo,
           Address,
           Data
           );
}


/*********************************************************************************
 * Name: LibFchPmIoRead8
 *
 * Description
 *   Read 8 bits from the PMIO space using CD6/CD7
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *   RegIndex    : the register to read from
 *
 * Output
 *   the read data
 *
 *********************************************************************************/
UINT8
LibFchPmIoRead8 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  PEI_CPU_IO_PPI       *CpuIo;
  UINT8                Data;

  // get the CpuIo pointer
  CpuIo = (*PeiServices)->CpuIo;

  // perform the read
  CpuIo->IoWrite8 (
           PeiServices,
           CpuIo,
           FCH_IOMAP_REGCD6,
           RegIndex
           );

  Data = CpuIo->IoRead8 (
                  PeiServices,
                  CpuIo,
                  FCH_IOMAP_REGCD7
                  );

  return (Data);
}


/*********************************************************************************
 * Name: LibFchPmIoRead16
 *
 * Description
 *   Read 16 bits from the PMIO space using CD6/CD7
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *   RegIndex    : the register to read from
 *
 * Output
 *   the read data
 *
 *********************************************************************************/
UINT16
LibFchPmIoRead16 (
  IN       EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  UINT16               Data;

  // get the data using two 1 byte read
  Data  = LibFchPmIoRead8 (PeiServices, RegIndex);
  Data |= (LibFchPmIoRead8 (PeiServices, RegIndex + 1) << 8);

  return (Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * LibFchGetAcpiMmio - Get AGESA FCH ACPIMMIO Address
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 *
 */
UINT32
LibFchGetAcpiMmio (
  IN       EFI_PEI_SERVICES  **PeiServices
  )
{
  UINTN       MmioTempAddr;
  UINT32      AcpiMmioAddr;

  MmioTempAddr = LibFchPmIoRead16 (PeiServices, FCH_PMIOA_REG24 + 2);
  AcpiMmioAddr = (UINT32) (MmioTempAddr << 16);
  MmioTempAddr = LibFchPmIoRead16 (PeiServices, FCH_PMIOA_REG24);
  AcpiMmioAddr |= (UINT32) MmioTempAddr;
  AcpiMmioAddr &= 0xFFFFC000;
  return (AcpiMmioAddr);
}

/*---------------------------------------------------------------------------------------*/
/**
 * LibFchPciCfgRead - FCH Read PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg Read lenght
 * @param[in]      Address       PCI Cfg Read Address
 * @param[in]      Value         Return Read value buffer
 *
 */
VOID
LibFchPciCfgRead (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN OUT   VOID                     *Value
  )
{
  PEI_PCI_CFG_PPI       *PciCfg;

  PciCfg = ( PEI_PCI_CFG_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Read (
            PeiServices,
            PciCfg,
            Width,
            Address,
            Value
            );
}

/*---------------------------------------------------------------------------------------*/
/**
 * LibFchPciCfgWrite - FCH Write PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg Write lenght
 * @param[in]      Address       PCI Cfg Write Address
 * @param[in]      Value         Return Write value buffer
 *
 */
VOID
LibFchPciCfgWrite (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN OUT   VOID                     *Value
  )
{
  PEI_PCI_CFG_PPI       *PciCfg;

  PciCfg = ( PEI_PCI_CFG_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Write (
            PeiServices,
            PciCfg,
            Width,
            Address,
            Value
            );
}

/*---------------------------------------------------------------------------------------*/
/**
 * LibFchPciCfgModify - FCH Modify PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg modify lenght
 * @param[in]      Address       PCI Cfg modify Address
 * @param[in]      SetBits       Set PCI Cfg Register bitmap
 * @param[in]      ClearBits     Clear PCI Cfg Register bitmap
 *
 */
VOID
LibFchPciCfgModify (
  IN       EFI_PEI_SERVICES         **PeiServices,
  IN       PEI_PCI_CFG_PPI_WIDTH    Width,
  IN       UINT64                   Address,
  IN       UINTN                    SetBits,
  IN       UINTN                    ClearBits
  )
{
  PEI_PCI_CFG_PPI       *PciCfg;

  PciCfg = ( PEI_PCI_CFG_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Modify (
            PeiServices,
            PciCfg,
            Width,
            Address,
            SetBits,
            ClearBits
            );
}

/*---------------------------------------------------------------------------------------*/
/**
 * LibFchPeiAuxInitialization - Initialization of FCH PEI AUX Environment
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 *
 */
VOID
LibFchPeiAuxInitialization (
  IN       EFI_PEI_SERVICES         **PeiServices
  )
{
  PEI_CPU_IO_PPI       *CpuIo;
  UINT8                Data;

  // get the CpuIo pointer
  CpuIo = (*PeiServices)->CpuIo;

  // perform the read
  CpuIo->IoWrite8 (
           PeiServices,
           CpuIo,
           FCH_IOMAP_REGCD6,
           FCH_PMIOA_REG24
           );

  Data = CpuIo->IoRead8 (
                  PeiServices,
                  CpuIo,
                  FCH_IOMAP_REGCD7
                  );
  Data |= 0x01;

  CpuIo->IoWrite8 (
           PeiServices,
           CpuIo,
           FCH_IOMAP_REGCD7,
           Data
           );
}
