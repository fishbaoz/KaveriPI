/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU F15 KV BrandId related functions and structures.
 *
 * Contains code that provides CPU BrandId information
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 287808 $   @e \$Date: 2014-03-21 14:14:44 -0500 (Fri, 21 Mar 2014) $
 *
 */
/*
 ******************************************************************************
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "cpuEarlyInit.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "cpuF15KvPowerMgmt.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVBRANDID_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define NAME_STRING_ADDRESS_PORT 0x194
#define NAME_STRING_DATA_PORT 0x198
#define NAME_STRING_SIZE_IN_DWORDS 12

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
/// A structure containing brand string
typedef struct {
  UINT16      ConfigId;      ///< The unique configuration ID associated with the string
  CONST CHAR8 *Stringstart;  ///< The literal string
} CPU_F15KV_EXCEPTION_BRAND;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
STATIC
F15KvIsException (
     OUT   UINT32                 *ExceptionId,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

VOID
F15KvSetBrandIdRegistersAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
// Override brand strings defined in the Processor Revision Guide
// Brand string is always 48 bytes

CONST CHAR8 ROMDATA str_F15KvException4C[48] = "AMD FX-7600P Radeon R7, 12 Compute Cores 4C+8G";  //4Ch
CONST CHAR8 ROMDATA str_F15KvException52[48] = "AMD A10-7400P Radeon R6, 10 Compute Cores 4C+6G"; //52h
CONST CHAR8 ROMDATA str_F15KvException53[48] = "AMD A8-7200P Radeon R5, 8 Compute Cores 4C+4G";   //53h
CONST CHAR8 ROMDATA str_F15KvException49[48] = "AMD FX-7500 Radeon R7, 10 Compute Cores 4C+6G";   //49h
CONST CHAR8 ROMDATA str_F15KvException4A[48] = "AMD A10-7300 Radeon R6, 10 Compute Cores 4C+6G";  //4Ah
CONST CHAR8 ROMDATA str_F15KvException4B[48] = "AMD A8-7100 Radeon R5, 8 Compute Cores 4C+4G";    //4Bh
CONST CHAR8 ROMDATA str_F15KvException4D[48] = "AMD A10 PRO-7350B R6, 10 Compute Cores 4C+6G";    //4Dh
CONST CHAR8 ROMDATA str_F15KvException37[48] = "AMD A10-7850K Radeon R7, 12 Compute Cores 4C+8G"; //37h
CONST CHAR8 ROMDATA str_F15KvException45[48] = "AMD A10-7800 Radeon R7, 12 Compute Cores 4C+8G";  //45h
CONST CHAR8 ROMDATA str_F15KvException38[48] = "AMD A10-7700K Radeon R7, 10 Compute Cores 4C+6G"; //38h
CONST CHAR8 ROMDATA str_F15KvException46[48] = "AMD A8-7600 Radeon R7, 10 Compute Cores 4C+6G";   //46h
CONST CHAR8 ROMDATA str_F15KvException47[48] = "AMD A6-7400K Radeon R5, 6 Compute Cores 2C+4G";   //47h
CONST CHAR8 ROMDATA str_F15KvException48[48] = "AMD A4-7300 Radeon R5, 5 Compute Cores 2C+3G";    //48h

// Override brand string for an unprogrammed part, also defined in the Processor Revision Guide
CONST CHAR8 ROMDATA str_F15KvUnprogrammed_Sample[48] = "AMD Unprogrammed Engineering Sample";
/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
CONST CPU_F15KV_EXCEPTION_BRAND ROMDATA CpuF15KvExceptionBrandIdString[] =
{
  {0x004C, str_F15KvException4C},
  {0x0052, str_F15KvException52},
  {0x0053, str_F15KvException53},
  {0x0049, str_F15KvException49},
  {0x004A, str_F15KvException4A},
  {0x004B, str_F15KvException4B},
  {0x004D, str_F15KvException4D},
  {0x0037, str_F15KvException37},
  {0x0045, str_F15KvException45},
  {0x0038, str_F15KvException38},
  {0x0046, str_F15KvException46},
  {0x0047, str_F15KvException47},
  {0x0048, str_F15KvException48},
  {0xFFFF, NULL},
};

/*---------------------------------------------------------------------------------------*/
/**
 * Set the Processor Name String register based on F5x194/198
 *
 * This function copies F5x198_x[B:0] to MSR_C001_00[35:30]
 *
 *  @param[in]   FamilyServices      The current Family Specific Services.
 *  @param[in]   EarlyParams         Service parameters.
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 */
VOID
F15KvSetBrandIdRegistersAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32    PciData;
  UINT32    MsrIndex;
  UINT32    ExceptionId;
  UINT64    *MsrNameStringPtrPtr;
  UINT32    MsrNameString[NAME_STRING_SIZE_IN_DWORDS];
  PCI_ADDR  PciAddress;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_5, NAME_STRING_ADDRESS_PORT);
    // check if D18F5x198_x0 is 00000000h.
    PciData = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
    PciAddress.Address.Register = NAME_STRING_DATA_PORT;
    LibAmdPciRead (AccessWidth32, PciAddress, &MsrNameString[0], StdHeader);
    if (MsrNameString[0] != 0) {
      if (F15KvIsException (&ExceptionId, StdHeader)) {
        ASSERT (ExceptionId < (sizeof (CpuF15KvExceptionBrandIdString) / sizeof (CpuF15KvExceptionBrandIdString[0])));

        MsrNameStringPtrPtr = (UINT64 *) CpuF15KvExceptionBrandIdString[ExceptionId].Stringstart;
      } else {
        for (PciData = 1; PciData < NAME_STRING_SIZE_IN_DWORDS; PciData++) {
          PciAddress.Address.Register = NAME_STRING_ADDRESS_PORT;
          LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
          PciAddress.Address.Register = NAME_STRING_DATA_PORT;
          LibAmdPciRead (AccessWidth32, PciAddress, &MsrNameString[PciData], StdHeader);
        }
        MsrNameStringPtrPtr = (UINT64 *) MsrNameString;
      }
    } else {
      // It is unprogrammed (unfused) parts and use a name string of "AMD Unprogrammed Engineering Sample"
      MsrNameStringPtrPtr = (UINT64 *) str_F15KvUnprogrammed_Sample;
    }

    // Put values into name MSRs,  Always write the full 48 bytes
    for (MsrIndex = MSR_CPUID_NAME_STRING0; MsrIndex <= MSR_CPUID_NAME_STRING5; MsrIndex++) {
      LibAmdMsrWrite (MsrIndex, MsrNameStringPtrPtr, StdHeader);
      MsrNameStringPtrPtr++;
    }
  }
}

/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Check if it's an exception
 *
 * For family 15h KV, brand string is obtained from F5x198_x[B:0], but there may be exceptions.
 * This function checks if it's an exception.
 *
 *  @param[out]  ExceptionId         Id of exception
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 *  @retval      TRUE                It's an exception
 *  @retval      FALSE               It's NOT an exception
 */
BOOLEAN
STATIC
F15KvIsException (
     OUT   UINT32                 *ExceptionId,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT16                     CfgIdOnPart;
  UINT32                     i;
  BOOLEAN                    IsException;
  PCI_ADDR                   PciAddress;
  POWER_CTRL_MISC_REGISTER   PwrCtlMisc;
  NATIVE_DIE_CPUID_REGISTER  NativeDieCpuid;

  *ExceptionId = 0xFFFF;
  IsException = FALSE;

  PciAddress.AddressValue = NATIVE_DIE_CPUID_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NativeDieCpuid, StdHeader);
  if ((NativeDieCpuid.BaseFamily == 0xF) &&
      (NativeDieCpuid.ExtFamily == 0x06) &&
      (NativeDieCpuid.ExtModel == 0x3)) {
    PciAddress.Address.Function = FUNC_3;
    PciAddress.Address.Register = PW_CTL_MISC_REG;
    LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &PwrCtlMisc, StdHeader);
    CfgIdOnPart = (UINT16) PwrCtlMisc.ConfigId;
    for (i = 0; CpuF15KvExceptionBrandIdString[i].ConfigId != 0xFFFF; i++) {
      if (CpuF15KvExceptionBrandIdString[i].ConfigId == CfgIdOnPart) {
        *ExceptionId = i;
        IsException = TRUE;
        break;
      }
    }
  }

  return IsException;
}

