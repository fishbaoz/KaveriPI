/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 specific utility functions.
 *
 * Provides numerous utility functions specific to family 15h.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "cpuPstateTables.h"
#include "cpuF15PowerMgmt.h"
#include "cpuApicUtilities.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "cpuF15Utilities.h"
#include "cpuEarlyInit.h"
#include "cpuPostInit.h"
#include "cpuFeatures.h"
#include "OptionMultiSocket.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)
#define FILECODE PROC_CPU_FAMILY_0X15_CPUF15UTILITIES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern CPU_FAMILY_SUPPORT_TABLE            PstateFamilyServiceTable;
extern OPTION_MULTISOCKET_CONFIGURATION    OptionMultiSocketConfiguration;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Disables the desired P-state.
 *
 *  @CpuServiceMethod{::F_CPU_DISABLE_PSTATE}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[in]   StateNumber              The P-State to disable.
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15DisablePstate (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       UINT8 StateNumber,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 LocalMsrRegister;

  ASSERT (StateNumber < NM_PS_REG);
  LibAmdMsrRead (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  ((F15_PSTATE_MSR *) &LocalMsrRegister)->PsEnable = 0;
  LibAmdMsrWrite (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Transitions the executing core to the desired P-state.
 *
 *  @CpuServiceMethod{::F_CPU_TRANSITION_PSTATE}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[in]   StateNumber              The new P-State to make effective.
 *  @param[in]   WaitForTransition        True if the caller wants the transition completed upon return.
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS       Always Succeeds
 */
AGESA_STATUS
F15TransitionPstate (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       UINT8              StateNumber,
  IN       BOOLEAN            WaitForTransition,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64       LocalMsrRegister;

  LibAmdMsrRead (MSR_PSTATE_CURRENT_LIMIT, &LocalMsrRegister, StdHeader);
  ASSERT (((PSTATE_CURLIM_MSR *) &LocalMsrRegister)->PstateMaxVal >= StateNumber);
  LibAmdMsrRead (MSR_PSTATE_CTL, &LocalMsrRegister, StdHeader);
  ((PSTATE_CTRL_MSR *) &LocalMsrRegister)->PstateCmd = (UINT64) StateNumber;
  LibAmdMsrWrite (MSR_PSTATE_CTL, &LocalMsrRegister, StdHeader);
  if (WaitForTransition) {
    do {
      LibAmdMsrRead (MSR_PSTATE_STS, &LocalMsrRegister, StdHeader);
    } while (((PSTATE_STS_MSR *) &LocalMsrRegister)->CurPstate != (UINT64) StateNumber);
  }
  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Determines the rate at which the executing core's time stamp counter is
 *  incrementing.
 *
 *  @CpuServiceMethod{::F_CPU_GET_TSC_RATE}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[out]  FrequencyInMHz           TSC actual frequency.
 *  @param[in]   StdHeader                Header for library and services.
 *
 *  @return      The most severe status of all called services
 */
AGESA_STATUS
F15GetTscRate (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8        NumBoostStates;
  UINT32       LocalPciRegister;
  UINT64       LocalMsrRegister;
  PCI_ADDR     PciAddress;
  PSTATE_CPU_FAMILY_SERVICES  *FamilyServices;

  LibAmdMsrRead (0xC0010015, &LocalMsrRegister, StdHeader);
  if ((LocalMsrRegister & 0x01000000) != 0) {
    FamilyServices = NULL;
    GetFeatureServicesOfCurrentCore (&PstateFamilyServiceTable, (CONST VOID **)&FamilyServices, StdHeader);
    ASSERT (FamilyServices != NULL);
    OptionMultiSocketConfiguration.GetCurrPciAddr (&PciAddress, StdHeader);
    PciAddress.Address.Function = FUNC_4;
    PciAddress.Address.Register = CPB_CTRL_REG;
    LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
    NumBoostStates = (UINT8) ((F15_CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates;
    return (FamilyServices->GetPstateFrequency (FamilyServices, NumBoostStates, FrequencyInMHz, StdHeader));
  } else {
    return (FamilySpecificServices->GetCurrentNbFrequency (FamilySpecificServices, FrequencyInMHz, StdHeader));
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Initially launches the desired core to run from the reset vector.
 *
 * @CpuServiceMethod{::F_CPU_AP_INITIAL_LAUNCH}.
 *
 * @param[in]   FamilySpecificServices   The current Family Specific Services.
 * @param[in]   SocketNum                The Processor on which the core is to be launched
 * @param[in]   ModuleNum                The Module in that processor containing that core
 * @param[in]   CoreNum                  The Core to launch
 * @param[in]   PrimaryCoreNum           The id of the module's primary core.
 * @param[in]   StdHeader                Header for library and services
 *
 * @retval      TRUE          The core was launched
 * @retval      FALSE         The core was previously launched
 */
BOOLEAN
F15LaunchApCore (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       UINT32 SocketNum,
  IN       UINT32 ModuleNum,
  IN       UINT32 CoreNum,
  IN       UINT32 PrimaryCoreNum,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32    NodeRelativeCoreNum;
  UINT32    LocalPciRegister;
  PCI_ADDR  PciAddress;
  BOOLEAN   LaunchFlag;
  AGESA_STATUS Ignored;

  // Code Start
  LaunchFlag = FALSE;
  NodeRelativeCoreNum = CoreNum - PrimaryCoreNum;
  GetPciAddress (StdHeader, SocketNum, ModuleNum, &PciAddress, &Ignored);
  PciAddress.Address.Function = FUNC_0;

  if (NodeRelativeCoreNum == 0) {
    PciAddress.Address.Register = LINK_INIT_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
    if ((LocalPciRegister & LINK_INIT_CTRL_REQ_DIS) != 0) {
      LocalPciRegister &= ~LINK_INIT_CTRL_REQ_DIS;
      LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      LaunchFlag = TRUE;
    }
  } else {
    PciAddress.Address.Register = CORE_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
    if ((LocalPciRegister & (1 << NodeRelativeCoreNum)) == 0) {
      LocalPciRegister |= (1 << NodeRelativeCoreNum);
      LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      LaunchFlag = TRUE;
    }
  }
  return (LaunchFlag);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Returns whether or not BIOS is responsible for configuring the NB COFVID.
 *
 *  @CpuServiceMethod{::F_CPU_IS_NBCOF_INIT_NEEDED}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[in]   PciAddress               The northbridge to query by pci base address.
 *  @param[out]  NbVidUpdateAll           Do all NbVids need to be updated
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      TRUE                    Perform northbridge frequency and voltage config.
 *  @retval      FALSE                   Do not configure them.
 */
BOOLEAN
F15CommonGetNbCofVidUpdate (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       PCI_ADDR *PciAddress,
     OUT   BOOLEAN *NbVidUpdateAll,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  *NbVidUpdateAll = FALSE;
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Is the Northbridge PState feature enabled?
 *
 * @CpuServiceMethod{::F_IS_NB_PSTATE_ENABLED}.
 *
 * @param[in]      FamilySpecificServices         The current Family Specific Services.
 * @param[in]      PlatformConfig                 Platform profile/build option config structure.
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @retval         TRUE                           The NB PState feature is enabled.
 * @retval         FALSE                          The NB PState feature is not enabled.
 */
BOOLEAN
F15IsNbPstateEnabled (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32   LocalPciRegister;
  PCI_ADDR PciAddress;
  BOOLEAN  PowerMode;
  BOOLEAN  SkipHwCfg;

  SkipHwCfg = FALSE;

  IDS_OPTION_HOOK (IDS_NBPSDIS_OVERRIDE, &SkipHwCfg, StdHeader);

  // Defaults to Power Optimized Mode
  PowerMode = TRUE;

  // If system is optimized for performance, disable NB P-States
  if (PlatformConfig->PlatformProfile.PlatformPowerPolicy == Performance) {
    PowerMode = FALSE;
  }

  PciAddress.AddressValue = F15_NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
  if (((((F15_NB_PSTATE_CTRL_REGISTER *) &LocalPciRegister)->NbPstateMaxVal != 0) || SkipHwCfg)
      && (PowerMode)) {
    return TRUE;
  }
  return FALSE;
}
