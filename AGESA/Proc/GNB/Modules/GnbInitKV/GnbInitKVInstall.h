/* $NoKeywords:$ */
/**
 * @file
 *
 * KV service installation file
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
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
* ***************************************************************************
*
*/

#ifndef _GNB_INIT_KV_INSTALL_H_
#define _GNB_INIT_KV_INSTALL_H_

//-----------------------------------------------------------------------
// Specify definition used by module services
//-----------------------------------------------------------------------

#include  "GnbPcie.h"
#include  "GnbUraServices.h"
#include  "GnbPcieFamServices.h"
#include  "GnbFamServices.h"
#include  "GnbGfxFamServices.h"

//-----------------------------------------------------------------------
// Export services
//-----------------------------------------------------------------------

#if  (AGESA_ENTRY_INIT_EARLY == TRUE)

  extern F_PCIEFMGETSBCONFIGINFO                  PcieGetSbConfigInfoKV;
  extern F_PCIEFMGETCOMPLEXDATALENGTH             PcieGetComplexDataLengthKV;
  extern F_PCIEFMBUILDCOMPLEXCONFIGURATION        PcieBuildComplexConfigurationKV;
  extern F_PCIEFMCONFIGUREENGINESLANEALLOCATION   PcieConfigureEnginesLaneAllocationKV;
  extern F_PCIEFMCHECKPORTPCIDEVICEMAPPING        PcieCheckPortPciDeviceMappingKV;
  extern F_PCIEFMMAPPORTPCIADDRESS                PcieMapPortPciAddressKV;
  extern F_PCIEFMCHECKPORTPCIELANECANBEMUXED      PcieCheckPortPcieLaneCanBeMuxedKV;


  PCIe_FAM_CONFIG_SERVICES GnbPcieConfigProtocolKV = {
    PcieGetComplexDataLengthKV,
    PcieBuildComplexConfigurationKV,
    PcieConfigureEnginesLaneAllocationKV,
    PcieCheckPortPciDeviceMappingKV,
    PcieMapPortPciAddressKV,
    PcieCheckPortPcieLaneCanBeMuxedKV,
    PcieGetSbConfigInfoKV
  };

  GNB_SERVICE GnbPcieConfigServicesKV = {
    GnbPcieFamConfigService,
    AMD_FAMILY_KV,
    &GnbPcieConfigProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieConfigServicesKV
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  extern F_PCIEFMGETCORECONFIGURATIONVALUE        PcieGetCoreConfigurationValueKV;
  extern F_PCIEFMGETLINKSPEEDCAP                  PcieGetLinkSpeedCapKV;
  extern F_PCIEFMGETNATIVEPHYLANEBITMAP           PcieGetNativePhyLaneBitmapKV;
  extern F_PCIEFMSETLINKSPEEDCAP                  PcieSetLinkSpeedCapV4;

  PCIe_FAM_INIT_SERVICES GnbPcieInitProtocolKV = {
    PcieGetCoreConfigurationValueKV,
    PcieGetLinkSpeedCapKV,
    PcieGetNativePhyLaneBitmapKV,
    PcieSetLinkSpeedCapV4
  };

  GNB_SERVICE GnbPcieInitServicesKV = {
    GnbPcieFamInitService,
    AMD_FAMILY_KV,
    &GnbPcieInitProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbPcieInitServicesKV
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE)
  #if IDSOPT_IDS_ENABLED == TRUE
    #if IDSOPT_TRACING_ENABLED == TRUE
      extern F_PCIEFMDEBUGGETHOSTREGADDRESSSPACESTRING  PcieDebugGetHostRegAddressSpaceStringKV;
      extern F_PCIEFMDEBUGGETWRAPPERNAMESTRING          PcieDebugGetWrapperNameStringKV;
      extern F_PCIEFMDEBUGGETCORECONFIGURATIONSTRING    PcieDebugGetCoreConfigurationStringKV;

      PCIe_FAM_DEBUG_SERVICES GnbPcieDebugProtocolKV = {
        PcieDebugGetHostRegAddressSpaceStringKV,
        PcieDebugGetWrapperNameStringKV,
        PcieDebugGetCoreConfigurationStringKV
      };

      GNB_SERVICE GnbPcieDebugServicesKV = {
        GnbPcieFamDebugService,
        AMD_FAMILY_KV,
        &GnbPcieDebugProtocolKV,
        SERVICES_POINTER
      };
      #undef  SERVICES_POINTER
      #define SERVICES_POINTER  &GnbPcieDebugServicesKV
    #endif
  #endif
#endif

#if  (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  extern F_GNB_REGISTER_ACCESS  GnbRegisterReadKV;
  extern F_GNB_REGISTER_ACCESS  GnbRegisterWriteKV;

  GNB_REGISTER_SERVICE GnbRegisterAccessProtocolKV = {
    GnbRegisterReadKV,
    GnbRegisterWriteKV
  };

  GNB_SERVICE GnbRegisterAccessServicesKV = {
    GnbRegisterAccessService,
    AMD_FAMILY_KV,
    &GnbRegisterAccessProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbRegisterAccessServicesKV

  extern F_GNBFMCREATEIVRSENTRY    GnbCreateIvrsEntryKV;
  extern F_GNBFMCHECKIOMMUPRESENT  GnbCheckIommuPresentKV;

  GNB_FAM_IOMMU_SERVICES GnbIommuConfigProtocolKV = {
    GnbCheckIommuPresentKV,
    GnbCreateIvrsEntryKV
  };

  GNB_SERVICE GnbIommuConfigServicesKV = {
    GnbIommuService,
    AMD_FAMILY_KV,
    &GnbIommuConfigProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbIommuConfigServicesKV

  extern F_GNBURASERVICELOCATEREGTBL GnbUraLocateRegTblKV;
  extern F_GNBURASERVICEGET GnbUraGetKV;
  extern F_GNBURASERVICESET GnbUraSetKV;
  extern F_GNBURASERVICESTREAMSET GnbUraStreamSetKV;

  GNB_URA_SERVICE GnbUraProtocolKV = {
    GnbUraLocateRegTblKV,
    GnbUraGetKV,
    GnbUraSetKV,
    GnbUraStreamSetKV
  };

  GNB_SERVICE GnbUraServicesKV = {
    GnbUraService,
    AMD_FAMILY_KV,
    &GnbUraProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbUraServicesKV
  extern F_GFXMAPENGINETODISPLAYPATH GfxMapEngineToDisplayPathKV;
  extern F_GFXDISABLECONTROLLER      GfxDisableControllerKV;
  extern F_GFXCALCULATECLOCK         GfxCalculateClockKV;
  extern F_GFXISVBIOSPOSTED          GfxIsVbiosPostedKV;

  GFX_FAM_SERVICES GfxFamilyServiceProtocolKV = {
    GfxMapEngineToDisplayPathKV,
    GfxDisableControllerKV,
    GfxCalculateClockKV,
    GfxIsVbiosPostedKV
  };

  GNB_SERVICE GfxFamilyServicesKV = {
    GfxFamService,
    AMD_FAMILY_KV,
    &GfxFamilyServiceProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GfxFamilyServicesKV

  extern F_GNBTIMESTAMP GnbTimeStampKV;

  GNB_FAM_TS_SERVICES GnbFamTsProtocolKV = {
    GnbTimeStampKV,
  };

  GNB_SERVICE GnbFamTsServicesKV = {
    GnbFamTsService,
    AMD_FAMILY_KV,
    &GnbFamTsProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbFamTsServicesKV

  #if (OPTION_FM2r2_SOCKET_SUPPORT == TRUE)
    extern F_GNBFMALIBGETBASETABLE PcieAlibGetBaseTableKVD;
    #define AlibGetBaseTableKV  PcieAlibGetBaseTableKVD
  #else
    extern F_GNBFMALIBGETBASETABLE PcieAlibGetBaseTableKVM;
    #define AlibGetBaseTableKV  PcieAlibGetBaseTableKVM
  #endif

  GNB_FAM_ALIB_SERVICES AlibProtocolKV = {
    AlibGetBaseTableKV
  };

  GNB_SERVICE AlibServicesKV = {
    GnbFamAlibService,
    AMD_FAMILY_KV,
    &AlibProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &AlibServicesKV


  extern GNB_BUILD_OPTIONS_KV  GnbBuildOptionsKV;

  GNB_FAM_BUILDOPTIONS_POINTERS GnbOptionsProtocolKV = {
    &GnbBuildOptionsKV
  };

  GNB_SERVICE GnbOptionsServicesKV = {
    GnbFamBuildOptionPointer,
    AMD_FAMILY_KV,
    &GnbOptionsProtocolKV,
    SERVICES_POINTER
  };
  #undef  SERVICES_POINTER
  #define SERVICES_POINTER  &GnbOptionsServicesKV

#endif
#endif  // _GNB_INIT_KV_INSTALL_H_
