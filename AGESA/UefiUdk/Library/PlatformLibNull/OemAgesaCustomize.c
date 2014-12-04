/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD OEM Agesa Customization Before calling Agesa Entry Points
 *
 * This file is placed in the user's platform directory and contains the
 * build option selections desired for that platform.
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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
 * ***************************************************************************
 */
#include "PiPei.h"
#include "AGESA.h"
#include "amdlib.h"
#include "OemAgesaCustomize.h"
#include "OemGnbPcieComplex.h"

BOOLEAN
GetSocketModuleOfNode (
  IN       UINT32    Node,
     OUT   UINT32    *Socket,
     OUT   UINT32    *Module,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
GetGivenModuleCoreRange (
  IN       UINT32            Socket,
  IN       UINT32            Module,
     OUT   UINT32            *LowCore,
     OUT   UINT32            *HighCore,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );
/*--------------------------------------------------------
 * !!! The CMOS_LOCATION_OFFSET can be set to the desired
 * offset  by Platform Ownder. The desired offset shou;ld
 * be set.
 *--------------------------------------------------------*/
#define     CMOS_LOCATION_OFFSET        0x80


/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitReset
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitReset
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitReset (
  IN  EFI_PEI_SERVICES    **PeiServices,
  IN  AMD_RESET_PARAMS    *InitReset
  )
{
  //
  // WARNING WARNING WARNING
  // This section should have the implementation to customize the structure
  // which doesn't require any PeiServices to retrieve any settings.
  // In this section, any customization could impact BSP and AP both
  //

  if (PeiServices) {
    //
    // This section should have implementation to customize the structure which may require
    // to use PeiServices to retrieve some info which will help customize the structure.
    // This will be done when this API gets called by BSP.
    //
  } else {
    //
    // This section should for AP that doesn't need PeiServices
    //
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitEarly
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitEarly
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitEarly (
  IN  EFI_PEI_SERVICES    **PeiServices,
  IN  AMD_EARLY_PARAMS    *InitEarly
  )
{
  if (PeiServices) {
    InitEarly->GnbConfig.PcieComplexList = &PcieComplexDescriptor;

  } else {
    //
    // This section should for AP that doesn't need PeiServices
    //
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitPost
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitPost
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitPost (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  AMD_POST_PARAMS    *InitPost
  )
{
  //
  // Temporary limit bottom of IO to 2G
  //
  InitPost->MemConfig.BottomIo = 0x80000000 >> 24;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitResume
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitResume
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitResume (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  AMD_RESUME_PARAMS  *InitResume
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeS3LateRestore
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *S3LateParams
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeS3LateRestore (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  AMD_S3LATE_PARAMS  *S3LateParams
  )
{
}

VOID
STATIC
MakeApicIdHobEntry (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8     NodeCount;
  UINT8     NodeNum;
  UINT32    Socket;
  UINT32    Module;
  UINT32    LowCore;
  UINT32    HighCore;
  UINT32    RegVal;
  UINT32    i;
  PCI_ADDR  PciAddress;
  AMD_APIC_PARAMS ApicParams;

  // Get Node count
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 24, FUNC_0, 0x60);
  LibAmdPciRead (AccessWidth32 , PciAddress, &RegVal, StdHeader);
  NodeCount = (UINT8) (((RegVal >> 4) & 0x7) + 1);

  NodeNum = 0;
  ApicParams.StdHeader = *StdHeader;
  for (NodeNum = 0; NodeNum < NodeCount; NodeNum++) {
    GetSocketModuleOfNode ((UINT32) NodeNum, &Socket, &Module, StdHeader);
    GetGivenModuleCoreRange (Socket, Module, &LowCore, &HighCore, StdHeader);
    for (i = LowCore; i <= HighCore; i++) {
      ApicParams.Socket = (UINT8) Socket;
      ApicParams.Core = (UINT8)i;
      AmdGetApicId (&ApicParams);
#if 0
      if (ApicParams.IsPresent) {
        EntryPtr->ProximityDomain = NodeNum;
        EntryPtr->ProcessorIdLow = ApicParams.ApicAddress;
        EntryPtr->CpuCoreCount = (UINT16) (HighCore - LowCore + 1);
        EntryPtr->Flags.Enabled = 1;
        EntryPtr->Flags.CpuPresent = 1;
      }
#endif
    }
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitReset
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitReset
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitReset
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitReset (
  IN  EFI_PEI_SERVICES    **PeiServices,
  IN  AMD_RESET_PARAMS    *InitReset
  )
{
  //
  // WARNING WARNING WARNING
  // This section should have the implementation to customize the structure
  // which doesn't require any PeiServices to retrieve any settings.
  // In this section, any customization could impact BSP and AP both
  //

  if (PeiServices) {

    //
    // This section should have implementation to customize the structure which may require
    // to use PeiServices to retrieve some info which will help customize the structure.
    // This will be done when this API gets called by BSP.
    //
  } else {
    //
    // This section should for AP that doesn't need PeiServices
    //
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitEarly
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitEarly
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitEarly
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitEarly (
  IN  EFI_PEI_SERVICES    **PeiServices,
  IN  AMD_EARLY_PARAMS    *InitEarly
  )
{
  //
  // WARNING WARNING WARNING
  // This section should have the implementation to customize the structure
  // which doesn't require any PeiServices to retrieve any settings.
  // In this section, any customization could impact BSP and AP both
  //

  if (PeiServices) {
    MakeApicIdHobEntry (&InitEarly->StdHeader);
    //
    // This section should have implementation to customize the structure which may require
    // to use PeiServices to retrieve some info which will help customize the structure.
    // This will be done when this API gets called by BSP.
    //
  } else {
    //
    // This section should for AP that doesn't need PeiServices
    //
  }
}
/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitPost
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitPost
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitPost
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitPost (
  IN  EFI_PEI_SERVICES    **PeiServices,
  IN  AMD_POST_PARAMS     *InitPost
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitResume
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *InitResume
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitResume (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  AMD_RESUME_PARAMS  *InitResume
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterS3LateRestore
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      **PeiServices
 *    @param[in]      *S3LateParams
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterS3LateRestore (
  IN  EFI_PEI_SERVICES   **PeiServices,
  IN  AMD_S3LATE_PARAMS  *S3LateParams
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  SaveApInitVector
 *
 *  Description:
 *    This function will store the offset of the function which can be invoked
 *    by AP after setting up the Cache as RAM and bypass PeiCore, and PEI Services
 *    to perform AGESA AmdInitReset and AmdInitEarly. Since APs, as of now, do not
 *    use any of the PEI Core and PEI Services.
 *  Parameters:
 *    @param[in]        ApInitAddress
 *    @param[in, out]   *ContentToRestore
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
SaveApInitVector (
  IN       UINT32              ApInitAddress,
  IN OUT   UINT32              *ContentToRestore
  )
{
  AMD_INTERFACE_PARAMS    AmdInterfaceParams;
  UINT32                  Value32;
  UINTN                   Index;

  //
  // !!! This is one of the suggested solutions where BSP can launch AP without
  // going through intitializing of Pei Core Services. This function saves the
  // ApInit pointer. In this process, the current content of CMOS gets saved
  // and returned to caller which will be restored once APInit is done by
  // calling RestoreContentVector
  //
  for (Index = CMOS_LOCATION_OFFSET; Index <= (CMOS_LOCATION_OFFSET + 3); Index++) {
    LibAmdIoWrite (AccessWidth8, 0x72, &Index, &AmdInterfaceParams.StdHeader);
    LibAmdIoRead (AccessWidth8, 0x73, &Value32, &AmdInterfaceParams.StdHeader);
    Value32 = (Value32 & 0x000000FF);
    *ContentToRestore |= (Value32 << ((Index - CMOS_LOCATION_OFFSET)*8));
    Value32 = (UINT32) (ApInitAddress & 0xff);
    LibAmdIoWrite (AccessWidth8, 0x73, &Value32, &AmdInterfaceParams.StdHeader);
    ApInitAddress >>= 8;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  RestoreContentVector
 *
 *  Description:
 *    This function will restore the previous content of the location
 *    where ApInit vector is being saved. It is up to platform owner if
 *    this function need to be implemented. In case there is no implementation,
 *    this function need to be present for compatibility reasons.
 *  Parameters:
 *    @param[in]      ContentToRestore
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
RestoreContentVector (
  IN  UINT32     ContentToRestore
  )
{
  AMD_INTERFACE_PARAMS   AmdInterfaceParams;
  UINT32                 Value32;
  UINTN                  Index;

  Value32 = 0;
  for (Index = CMOS_LOCATION_OFFSET; Index <= (CMOS_LOCATION_OFFSET + 3); Index++) {
    LibAmdIoWrite (AccessWidth8, 0x72, &Index, &AmdInterfaceParams.StdHeader);
    Value32 = (UINT8) ((ContentToRestore >> (Index - CMOS_LOCATION_OFFSET)*8));
    LibAmdIoWrite (AccessWidth8, 0x73, &Value32, &AmdInterfaceParams.StdHeader);
  }
}
