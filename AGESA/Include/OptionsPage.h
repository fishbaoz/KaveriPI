/* $NoKeywords:$ */
/**
 * @file
 *
 * Create outline and references for Build Configuration and Options Component mainpage documentation.
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
 * @page optionmain Build Configuration and Options Documentation
 *
 * Additional documentation for the Build Configuration and Options component consists of
 *
 * - Introduction and Overview to Build Options
 *   - @subpage platforminstall "Platform Build Options"
 *   - @subpage bldcfg "Build Configuration Item Cross Reference"
 *   - @subpage examplecustomizations "Customization Examples"
 * - Maintenance Guides:
 *   -  For debug of the Options system, use compiler options
 *      @n <tt>      /P /EP /C /FAs      </tt> @n
 *      PreProcessor output is produced in an .i file in the directory where the project
 *      file is located.
 * - Design Guides:
 *   - add here >>>
 *
 */

/**
 * @page platforminstall Platform Build Options.
 *
 * Build options are boolean constants. The purpose of build options is to remove code
 * from the build to reduce the overall code size present in the ROM image. Unless
 * otherwise specified, the default action is to include all options. If a build option is
 * not specifically listed as disabled, then it is included into the build.
 *
 * The documented build options are imported from a user controlled file for
 * processing. The build options for all platform solutions are listed below:
 *
 * @anchor BLDOPT_REMOVE_UDIMMS_SUPPORT
 * @li @e BLDOPT_REMOVE_UDIMMS_SUPPORT @n
 *        If unbuffered DIMMs are NOT expected to be required in the system, the code that
 *        handles unbuffered DIMMs can be removed from the build.
 *
 * @anchor BLDOPT_REMOVE_RDIMMS_SUPPORT
 * @li @e  BLDOPT_REMOVE_RDIMMS_SUPPORT @n
 *         If registered DIMMs are NOT expected to be required in the system, the code
 *         that handles registered DIMMs can be removed from the build.
 *
  * @anchor BLDOPT_REMOVE_LRDIMMS_SUPPORT
 * @li @e  BLDOPT_REMOVE_LRDIMMS_SUPPORT @n
 *         If Load Reduced DIMMs are NOT expected to be required in the system, the code
 *         that handles Load Reduced DIMMs can be removed from the build.
 *
 *     @note The above three options operate independently from each other; however, at
 *     least one of the unbuffered , registered or load reduced DIMM options must be present in the build.
 *
 * @anchor BLDOPT_REMOVE_ECC_SUPPORT
 * @li @e  BLDOPT_REMOVE_ECC_SUPPORT @n
 *         Use this option to remove the code for Error Checking & Correction.
 *
 * @anchor BLDOPT_REMOVE_BANK_INTERLEAVE
 * @li @e  BLDOPT_REMOVE_BANK_INTERLEAVE @n
 *         Interleaving is a mechanism to do performance fine tuning. This option
 *         interleaves memory between banks on a DIMM.
 *
 * @anchor BLDOPT_REMOVE_DCT_INTERLEAVE
 * @li @e  BLDOPT_REMOVE_DCT_INTERLEAVE @n
 *         Interleaving is a mechanism to do performance fine tuning. This option
 *         interleaves memory from two DRAM controllers.
 *
 * @anchor BLDOPT_REMOVE_NODE_INTERLEAVE
 * @li @e  BLDOPT_REMOVE_NODE_INTERLEAVE @n
 *         Interleaving is a mechanism to do performance fine tuning. This option
 *         interleaves memory from two HyperTransport nodes.
 *
 * @anchor BLDOPT_REMOVE_PARALLEL_TRAINING
 * @li @e  BLDOPT_REMOVE_PARALLEL_TRAINING @n
 *         For multi-socket systems, training memory in parallel can reduce the time
 *         needed to boot.
 *
 * @anchor BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT
 * @li @e  BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT @n
 *         Online Spare support is removed by this option.
 *
 * @anchor BLDOPT_REMOVE_MULTISOCKET_SUPPORT
 * @li @e BLDOPT_REMOVE_MULTISOCKET_SUPPORT @n
 *        Many systems use only a single socket and may benefit in code space to remove
 *        this code. However, certain processors have multiple HyperTransport nodes
 *        within a single socket. For these processors, the multi-node support is
 *        required and this option has no effect.
 *
 * @anchor BLDOPT_REMOVE_ACPI_PSTATES
 * @li @e  BLDOPT_REMOVE_ACPI_PSTATES @n
 *         This option removes the code that generates the ACPI tables used in power
 *         management.
 *
 * @anchor BLDOPT_REMOVE_CRAT
 * @li @e  BLDOPT_REMOVE_CRAT @n
 *         This option removes the code that generates the CRAT tables used in performance
 *         tuning.
 *
 * @anchor BLDOPT_REMOVE_CDIT
 * @li @e  BLDOPT_REMOVE_CDIT @n
 *         This option removes the code that generates the CDIT tables used in performance
 *         tuning.
 *
 * @anchor BLDOPT_REMOVE_SRAT
 * @li @e  BLDOPT_REMOVE_SRAT @n
 *         This option removes the code that generates the SRAT tables used in performance
 *         tuning.
 *
 * @anchor BLDOPT_REMOVE_SLIT
 * @li @e  BLDOPT_REMOVE_SLIT @n
 *         This option removes the code that generates the SLIT tables used in performance
 *         tuning.
 *
 * @anchor BLDOPT_REMOVE_WHEA
 * @li @e  BLDOPT_REMOVE_WHEA @n
 *         This option removes the code that generates the WHEA tables used in error
 *         handling and reporting.
 *
 * @anchor BLDOPT_REMOVE_DMI
 * @li @e  BLDOPT_REMOVE_DMI @n
 *         This option removes the code that generates the DMI tables used in system
 *         management.
 *
 * @anchor BLDOPT_REMOVE_DQS_TRAINING
 * @li @e  BLDOPT_REMOVE_DQS_TRAINING @n
 *         This option removes the code used in memory performance tuning.
 *
 * @anchor BLDOPT_REMOVE_EARLY_SAMPLES
 * @li @e  BLDOPT_REMOVE_EARLY_SAMPLES @n
 *         Special support for Early Samples is included. Default setting is FALSE.
 *
 * @anchor BLDOPT_REMOVE_C6_STATE
 * @li @e  BLDOPT_REMOVE_C6_STATE @n
 *         This option removes the code which implements the C6 C-state feature.
 *
 * @anchor BLDOPT_REMOVE_MEM_RESTORE_SUPPORT
 * @li @e  BLDOPT_REMOVE_MEM_RESTORE_SUPPORT @n
 *         This option removes the memory context restore feature.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_10_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_10_SUPPORT @n
 *         If the package contains support for family 10h processors, remove that support.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_12_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_12_SUPPORT @n
 *         If the package contains support for family 10h processors, remove that support.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_14_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_14_SUPPORT @n
 *         If the package contains support for family 14h processors, remove that support.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_15_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_15_SUPPORT @n
 *         If the package contains support for family 15h processors, remove that support.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT @n
 *         If the package contains support for family 15h model 1x processors, remove that support.
 *
 * @anchor BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT
 * @li @e  BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT @n
 *         If the package contains support for family 15h model 3x processors, remove that support.

 * @anchor BLDOPT_REMOVE_AM3_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_AM3_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for AM3 sockets.
 *
 * @anchor BLDOPT_REMOVE_ASB2_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_ASB2_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for ASB2 sockets.
 *
 * @anchor BLDOPT_REMOVE_C32_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_C32_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for C32 sockets.
 *
 * @anchor BLDOPT_REMOVE_FM1_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FM1_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FM1 sockets.
 *
 * @anchor BLDOPT_REMOVE_FP1_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FP1_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FP1 sockets.
 *
 * @anchor BLDOPT_REMOVE_FP2_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FP2_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FP2 sockets.
 *
 * @anchor BLDOPT_REMOVE_FS1_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FS1_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FS1 sockets.
 *
 * @anchor BLDOPT_REMOVE_FT1_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FT1_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FT1 sockets.
 *
 * @anchor BLDOPT_REMOVE_G34_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_G34_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for G34 sockets.
 *
 * @anchor BLDOPT_REMOVE_S1G3_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_S1G3_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for S1G3 sockets.
 *
 * @anchor BLDOPT_REMOVE_S1G4_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_S1G4_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for S1G4 sockets.
 *
 * @anchor BLDOPT_REMOVE_FP3_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FP3_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FP3 sockets.
 *
 * @anchor BLDOPT_REMOVE_FM2r2_SOCKET_SUPPORT
 * @li @e  BLDOPT_REMOVE_FM2r2_SOCKET_SUPPORT @n
 *         This option removes the code which implements support for processors packaged for FM2r2 sockets.
 *
 */

/**
 * @page examplecustomizations Customization Examples
 *
 * The Addendum \<plat\>Options.c file for each platform contains the minimum required
 * customizations for that platform.  That is, it contains settings which would be needed
 * to boot a SimNow! bsd for that platform.
 * However, each individual product based on that platform will have customizations necessary for
 * that hardware.  Since the actual customizations needed vary so much, they are not included in
 * the \<plat\>Options.c.  This section provides examples of useful customizations that you can use or
 * modify to suit your needs.
 *
 * @par
 *
 * Source for the examples shown can be found at Addendum\\Examples. @n
 *
 * None.
 *
 */
