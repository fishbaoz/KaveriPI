/* $NoKeywords:$ */
/**
 * @file
 *
 * Create outline and references for GNB Component mainpage documentation.
 *
 * Design guides, maintenance guides, and general documentation, are
 * collected using this file onto the documentation mainpage.
 * This file contains doxygen comment blocks, only.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Documentation
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


/**
 * @page gnbmain GNB Component Documentation
 *
 * Additional documentation for the GNB component consists of
 *
 * - Maintenance Guides:
 *   - @subpage PcieLaneDescription    "PCIe/DDI Lane description table"
 *   - @subpage DualLinkDviDescription "Example Dual Link DVI connector description"
 *   - add here >>>
 * - Design Guides:
 *   - @subpage BuildConfigDescription    "Build Configurations"
 *   - add here >>>
 *
 */


/**
 *  @page PcieLaneDescription
 *
 * PCIe/DDI Lane descriptions can be found in the BKDG for
 * a given family.  Please refer to the BKDG to identify valid
 * configurations for PCIe and DDI lane allocation.
 */

/**
 *  @page DualLinkDviDescription Example Dual Link DVI connector description
 *  Examples of various Dual Link DVI descriptors.
 *  @code
 *  // Dual Link DVI on dedicated display lanes. DP1_TXP/N[0]..DP1_TXP/N[3] - master, DP0_TXP/N[0]..DP0_TXP/N[3] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 24, 32),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * // Dual Link DVI on dedicated display lanes. DP0_TXP/N[0]..DP0_TXP/N[3] - master, DP1_TXP/N[0]..DP1_TXP/N[3] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 32, 24),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * // Dual Link DVI on PCIe lanes. P_GFX_TXP/N[0]..P_GFX_TXP/N[3] - master, P_GFX_TXP/N[4]..P_GFX_TXP/N[7] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 8, 15),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * // Dual Link DVI on PCIe lanes. P_GFX_TXP/N[7]..P_GFX_TXP/N[4] - master, P_GFX_TXP/N[0]..P_GFX_TXP/N[3] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 15, 8),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * // Dual Link DVI on PCIe lanes. P_GFX_TXP/N[8]..P_GFX_TXP/N[11] - master, P_GFX_TXP/N[12]..P_GFX_TXP/N[15] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 16, 23),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * // Dual Link DVI on PCIe lanes. P_GFX_TXP/N[12]..P_GFX_TXP/N[15] - master, P_GFX_TXP/N[8]..P_GFX_TXP/N[11] - slave.
 *  PCIe_PORT_DESCRIPTOR DdiList [] = {
 *    {
 *      DESCRIPTOR_TERMINATE_LIST,   //Descriptor flags
 *      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 23, 16),
 *      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDvi, Aux1, Hdp1, 0)
 *    }
 *  }
 * @endcode
 */

 /**
 * @page BuildConfigDescription GNB Build Configurations.
 *
 * Build configurations are configuration constants. The purpose
 * of build configurations is to specify configuration info
 * regarding the GNB component.
 *
 * The documented build configurations are imported from a user
 * controlled file for processing. The build configurations for
 * all platform solutions are listed below:
 *
 * @anchor BLDCFG_USE_SYNCFLOOD_AS_NMI
 * @li @e BLDCFG_USE_SYNCFLOOD_AS_NMI @n
 *        This build configuration defines the function of the
 *        SYNCFLOOD/NMI# pin.  If TRUE, then the pin is defined
 *        as NMI#.  If FALSE or undefined, the pin is defined as
 *        SYNCFLOOD_L.
 *
 */

