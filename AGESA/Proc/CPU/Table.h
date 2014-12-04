/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Register Table Related Functions
 *
 * Contains code to initialize the CPU MSRs and PCI registers with BKDG recommended values
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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

#ifndef _CPU_TABLE_H_
#define _CPU_TABLE_H_

#define MAX_ENTRY_TYPE_ITEMS32 6        // The maximum number of initializer items for UINT32 entry data types.

/**
 * @page regtableimpl Register Table Implementation Guide
 *
 * This register table implementation is modular and extensible, so that support code as
 * well as table data can be family specific or built out if not needed, and new types
 * of table entries can be added with low overhead.  Because many aspects are now generic,
 * there can be common implementations for CPU revision and platform feature matching and for
 * finding and iterating tables.
 *
 * @par Adding a new table entry type.
 *
 * To add a new table entry type follow these steps.
 * <ul>
 * <li>  Add a member to the enum TABLE_ENTRY_TYPE which is a descriptive name of the entry's purpose
 *       or distinct characteristics.
 *
 * <li>  Create an entry data struct with the customized data needed.  For example, custom register designations,
 *       data and mask sizes, or feature comparisons.  Name your struct by adding "_" and upper-casing the enum name
 *       and adding "_TYPE_ENTRY_DATA" at the end.
 *
 * <li>  Add the entry data type as a member of the TABLE_ENTRY_DATA union.  Be aware of the size of your
 *       entry data struct; all table entries in all tables will share any size increase you introduce!
 *
 * <li>  If your data entry contains any member types except for UINT32, you can't use the generic first union member
 *       for the initializers that make up the actual tables (it's just UINT32's). The generic MSR entry is
 *       an example. Follow the steps below:
 *
 *       <ul>
 *       <li>  Make a union which has your entry data type as the first member.  Use TABLE_ENTRY_DATA as the
 *             second member.  Name this with your register followed by "_DATA_INITIALIZER".
 *
 *       <li>  Make a copy of TABLE_ENTRY_FIELDS, and rename it your register "_TYPE_ENTRY_INITIALIZER".  Rename
 *             the TABLE_ENTRY_DATA member of that struct to have the type you created in the previous step.
 *             This type can be used to declare an array of entries and make a register table in some family specific
 *             file.
 *       </ul>
 *
 * <li>  Add the descriptor that will link table entries of your data type to an implementation for it.
 *       <ul>
 *       <li>  Find the options file which instantiates the CPU_SPECIFIC_SERVICES for each logical model that will
 *             support the new entry type.
 *
 *       <li> From there find the instantiation of its TABLE_ENTRY_TYPE_DESCRIPTOR.  Add a descriptor to the
 *            to the list for your new type.  Provide the name of a function which will implement the
 *            entry data.  The function name should reflect that it implements the action for the entry type.
 *            The function must be an instance of F_DO_TABLE_ENTRY.
 *       </ul>
 *
 * <li>  Implement the function for your entry type data.  (If parts of it are family specific add methods to
 *       CPU_SPECIFIC_SERVICES for that and implement them for each family or model required.)  @n
 *       The definition of the function must conform to F_DO_TABLE_ENTRY.
 *       In the function preamble, include a cross reference to the entry enum:
 *       @code
 *       *
 *       * @TableEntryTypeMethod{::MyRegister}
 *       *
 *       @endcode
 *
 * </ul>
 *
 * @par  Adding a new Register Table
 *
 * To add a new register table for a logical CPU model follow the steps below.
 *
 * <ul>
 * <li>  Find the options file which instantiates the CPU_SPECIFIC_SERVICES for the logical model that
 *       should include the table.
 *
 * <li>  From there find the instantiation of its REGISTER_TABLE list. Add the name of the new register table.
 * </ul>
 *
 */

/*------------------------------------------------------------------------------------------*/
/*
 * Define the supported table entries.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * These are the available types of table entries.
 *
 * Each type corresponds to:
 * - a semantics for the type specific data, for example semantics for a Register value,
 * Data value, and Mask value.
 * - optionally, including a method for type specific matching criteria
 * - a method for writing the desired update to the hardware.
 *
 * All types share in common a method to match CPU Family and Model and a method to match
 * platform feature set.
 *
 */
typedef enum {
  MsrRegister,                 ///< Processor MSR registers.
  PciRegister,                 ///< Processor Config Space registers.
  FamSpecificWorkaround,       ///< Processor Family Specific Workarounds which are @b not practical using the other types.
  ProfileFixup,                ///< Processor Performance Profile fixups to PCI Config Registers.
  CoreCountsPciRegister,       ///< Processor PCI Config Registers which depend on core counts.
  ProcCountsPciRegister,       ///< Processor PCI Config Registers which depend on processor counts.
  CompUnitCountsPciRegister,   ///< Processor PCI Config Registers which depend on compute unit counts.
  CompUnitCountsMsr,           ///< Processor MSRs which depend on compute unit counts.
  TableEntryTypeMax            ///< Not a valid entry type, use for limit checking.
} TABLE_ENTRY_TYPE;

/*------------------------------------------------------------------------------------------*/
/*
 * Useful types and defines: Selectors, Platform Features, and type specific features.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * Select tables for the current core.
 *
 * This allows more efficient register table processing, by allowing cores to skip
 * redundantly setting PCI registers, for example.  This feature is not intended to
 * be relied on for function:  it is valid to have a single register table with all settings
 * processed by every core; it's just slower.
 *
 */
typedef enum {
  AllCores,                   ///< Select only tables which apply to all cores.
  ComputeUnitPrimary,         ///< Select tables which apply to the primary core of a compute unit (SharedC, SharedNc).
  PrimaryCores,               ///< Select tables which apply to primary cores.
  BscCore,                    ///< Select tables which apply to the boot core.
  TableCoreSelectorMax        ///< Not a valid selector, use for limit checking.
} TABLE_CORE_SELECTOR;

//----------------------------------------------------------------------------
//                         CPU PERFORM EARLY INIT ON CORE
//
//----------------------------------------------------------------------------
/// Flag definition.

// Condition
#define PERFORM_EARLY_WARM_RESET    0x1          // bit  0 --- the related function needs to be run if it's warm reset
#define PERFORM_EARLY_COLD_BOOT     0x2          // bit  1 --- the related function needs to be run if it's cold boot

#define PERFORM_EARLY_ANY_CONDITION (PERFORM_EARLY_WARM_RESET | PERFORM_EARLY_COLD_BOOT)

// Initializer bit pattern values for platform features.
// Keep in synch with the PLATFORM_FEATURES struct!

// The 5 control flow modes.
#define AMD_PF_NFCM           BIT0
#define AMD_PF_UMA            BIT1    // UMA_DR
#define AMD_PF_UMA_IFCM       BIT2
#define AMD_PF_IFCM           BIT3
#define AMD_PF_IOMMU          BIT4
// Degree of HT connectivity possible.
#define AMD_PF_SINGLE_LINK    BIT5
#define AMD_PF_MULTI_LINK     BIT6
// For some legacy MSRs, define a couple core count bits.  Do not continue adding
// core counts to the platform feats, if you need more than this design a table entry type.
// Here, provide exactly 1, exactly 2, or anything else.
#define AMD_PF_SINGLE_CORE    BIT7
#define AMD_PF_DUAL_CORE      BIT8
#define AMD_PF_MULTI_CORE     BIT9

// Not a platform type, but treat all others as AND
#define AMD_PF_AND            BIT31

#define AMD_PF_ALL    (AMD_PF_NFCM          | \
                       AMD_PF_UMA           | \
                       AMD_PF_UMA_IFCM      | \
                       AMD_PF_IFCM          | \
                       AMD_PF_IOMMU         | \
                       AMD_PF_SINGLE_LINK   | \
                       AMD_PF_MULTI_LINK    | \
                       AMD_PF_SINGLE_CORE   | \
                       AMD_PF_DUAL_CORE     | \
                       AMD_PF_MULTI_CORE)
// Do not include AMD_PF_AND in AMD_PF_ALL !

/**
 * The current platform features.
 *
 * Keep this in sync with defines above that are used in the initializers!
 *
 * The comments with the bit number are useful for the computing the reserved member size, but
 * do not write code that assumes you know what bit number one of these members is.
 *
 * These platform features are standard for all logical families and models.
 */
typedef struct {
  UINT32          PlatformNfcm:1;             ///< BIT_0 Normal Flow Control Mode.
  UINT32          PlatformUma:1;              ///< BIT_1 UMA (Display Refresh) Flow Control.
  UINT32          PlatformUmaIfcm:1;          ///< BIT_2 UMA using Isochronous Flow Control.
  UINT32          PlatformIfcm:1;             ///< BIT_3 Isochronous Flow Control Mode (not UMA).
  UINT32          PlatformIommu:1;            ///< BIT_4 IOMMU (a special case Isochronous mode).
  UINT32          PlatformSingleLink:1;       ///< BIT_5 The processor is in a package which implements only a single HT Link.
  UINT32          PlatformMultiLink:1;        ///< BIT_6 The processor is in a package which implements more than one HT Link.
  UINT32          PlatformSingleCore:1;       ///< BIT_7 Single Core processor, for legacy entries.
  UINT32          PlatformDualCore:1;         ///< BIT_8 Dual Core processor, for legacy entries.
  UINT32          PlatformMultiCore:1;        ///< BIT_9 More than dual Core processor, for legacy entries.
  UINT32          :(30 - 9);                  ///< The possibilities are (not quite) endless.
  UINT32          AndPlatformFeats:1;         ///< BIT_31
} PLATFORM_FEATURES;

/**
 * Platform Features
 */
typedef union {
  UINT32            PlatformValue;            ///< Describe Platform Features in UINT32.
  ///< This one goes first, because then initializers use it automatically for the union.
  PLATFORM_FEATURES PlatformFeatures;         ///< Describe Platform Features in structure
} PLATFORM_FEATS;

// Initializer bit patterns for PERFORMANCE_PROFILE_FEATS.
#define PERFORMANCE_REFRESH_REQUEST_32B       BIT0
#define PERFORMANCE_L3_CACHE                  BIT1
#define PERFORMANCE_NO_L3_CACHE               BIT2
#define PERFORMANCE_MCT_ISOC_VARIABLE         BIT3
#define PERFORMANCE_IS_WARM_RESET             BIT4
#define PERFORMANCE_VRM_HIGH_SPEED_ENABLE     BIT5
#define PERFORMANCE_NB_PSTATES_ENABLE         BIT6
#define PERFORMANCE_AND                       BIT31

#define PERFORMANCE_PROFILE_ALL   (PERFORMANCE_REFRESH_REQUEST_32B | \
                                   PERFORMANCE_L3_CACHE            | \
                                   PERFORMANCE_NO_L3_CACHE         | \
                                   PERFORMANCE_MCT_ISOC_VARIABLE   | \
                                   PERFORMANCE_IS_WARM_RESET       | \
                                   PERFORMANCE_VRM_HIGH_SPEED_ENABLE | \
                                   PERFORMANCE_NB_PSTATES_ENABLE)

/**
 * Performance Profile specific Type Features.
 *
 * Register settings for the different control flow modes can have additional dependencies
 */
typedef struct {
  UINT32       RefreshRequest32Byte:1;         ///< BIT_0.   Display Refresh Requests use 32 bytes (32BE).
  UINT32       L3Cache:1;                      ///< BIT_1    L3 Cache is present.
  UINT32       NoL3Cache:1;                    ///< BIT_2    L3 Cache is NOT present.
  UINT32       MctIsocVariable:1;              ///< BIT_3    Mct Isoc Read Priority set to variable.
  UINT32       IsWarmReset:1;                  ///< BIT_4    This boot is on a warm reset, cold reset pass is already completed.
  UINT32       VrmHighSpeed:1;                 ///< BIT_5    Select high speed VRM.
  UINT32       NbPstates:1;                    ///< BIT_6    Northbridge PStates are enabled
  UINT32       :(30 - 6);                      ///< available for future expansion.
  UINT32       AndPerformanceFeats:1;          ///< BIT_31.  AND other selected features.
} PERFORMANCE_PROFILE_FEATURES;

/**
 * Performance Profile features.
 */
typedef union {
  UINT32                        PerformanceProfileValue;     ///< Initializer value.
  PERFORMANCE_PROFILE_FEATURES  PerformanceProfileFeatures;  ///< The performance profile features.
} PERFORMANCE_PROFILE_FEATS;

/**
 * Package Type Features
 *
 */
typedef struct {
  UINT32        PkgType0:1;                 ///< Package Type 0
  UINT32        PkgType1:1;                 ///< Package Type 1
  UINT32        PkgType2:1;                 ///< Package Type 2
  UINT32        PkgType3:1;                 ///< Package Type 3
  UINT32        PkgType4:1;                 ///< Package Type 4
  UINT32        PkgType5:1;                 ///< Package Type 5
  UINT32        PkgType6:1;                 ///< Package Type 6
  UINT32        PkgType7:1;                 ///< Package Type 7
  UINT32        PkgType8:1;                 ///< Package Type 8
  UINT32        PkgType9:1;                 ///< Package Type 9
  UINT32        PkgType10:1;                ///< Package Type 10
  UINT32        PkgType11:1;                ///< Package Type 11
  UINT32        PkgType12:1;                ///< Package Type 12
  UINT32        PkgType13:1;                ///< Package Type 13
  UINT32        PkgType14:1;                ///< Package Type 14
  UINT32        PkgType15:1;                ///< Package Type 15
  UINT32        Reserved:15;                ///< Package Type Reserved
  UINT32        ReservedAndFeats:1;         ///< BIT_31.  AND other selected features. Always zero here.
} PACKAGE_TYPE_FEATURES;

// Initializer Values for Package Type
#define PACKAGE_TYPE_ALL              0XFFFF       ///< Package Type apply all packages

// Core Range Initializer values.
#define COUNT_RANGE_LOW            0ul
#define COUNT_RANGE_HIGH           0xFFul

// A count range matching none is often useful as the second range, matching will then be
// based on the first range.  A count range all is provided as a first range for default settings.
#define COUNT_RANGE_NONE           ((((COUNT_RANGE_HIGH) << 8) | (COUNT_RANGE_HIGH)) << 16)
#define COUNT_RANGE_ALL            (((COUNT_RANGE_HIGH) << 8) | (COUNT_RANGE_LOW))
#define IGNORE_FREQ_0              (((COUNT_RANGE_HIGH) << 8) | (COUNT_RANGE_HIGH))
#define IGNORE_PROCESSOR_0         (((COUNT_RANGE_HIGH) << 8) | (COUNT_RANGE_HIGH))

#define CORE_RANGE_0(min, max)            ((((UINT32)(max)) << 8) | (UINT32)(min))
#define CORE_RANGE_1(min, max)            (((((UINT32)(max)) << 8) | (UINT32)(min)) << 16)
#define PROCESSOR_RANGE_0(min, max)       ((((UINT32)(max)) << 8) | (UINT32)(min))
#define PROCESSOR_RANGE_1(min, max)       (((((UINT32)(max)) << 8) | (UINT32)(min)) << 16)
#define DEGREE_RANGE_0(min, max)          ((((UINT32)(max)) << 8) | (UINT32)(min))
#define DEGREE_RANGE_1(min, max)          (((((UINT32)(max)) << 8) | (UINT32)(min)) << 16)
#define FREQ_RANGE_0(min, max)            ((((UINT32)(max)) << 8) | (UINT32)(min))
#define FREQ_RANGE_1(min, max)            (((((UINT32)(max)) << 8) | (UINT32)(min)) << 16)
#define COMPUTE_UNIT_RANGE_0(min, max)    ((((UINT32)(max)) << 8) | (UINT32)(min))
#define COMPUTE_UNIT_RANGE_1(min, max)    (((((UINT32)(max)) << 8) | (UINT32)(min)) << 16)

/**
 * Count Range Feature, two count ranges for core counts, processor counts, or node counts.
 */
typedef struct {
  UINT32       Range0Min:8;           ///< The minimum of the first count range.
  UINT32       Range0Max:8;           ///< The maximum of the first count range.
  UINT32       Range1Min:8;           ///< The minimum of the second count range.
  UINT32       Range1Max:8;           ///< The maximum of the second count range.
} COUNT_RANGE_FEATURE;

/**
 * Core Count Ranges for table data.
 *
 * Provide a pair of core count ranges. If the actual core count is included in either range (OR),
 * the feature should be considered a match.
 */
typedef union {
  UINT32       CoreRangeValue;          ///< Initializer value.
  COUNT_RANGE_FEATURE CoreRanges;       ///< The Core Counts.
} CORE_COUNT_RANGES;

/**
 * Processor count ranges for table data.
 *
 * Provide a pair of processor count ranges.  If the actual counts are included in either range (OR),
 * the feature should be considered a match.
 */
typedef union {
  UINT32       ProcessorCountRangeValue;             ///< Initializer value.
  COUNT_RANGE_FEATURE ProcessorCountRanges;          ///< The Processor and Node Counts.
} PROCESSOR_COUNTS;

/**
 * Compute unit count ranges for table data.
 *
 * Provide a pair of compute unit count ranges. If the actual counts are included in either ranges (OR),
 * the feature should be considered a match.
 */
typedef union {
  UINT32    ComputeUnitRangeValue;                ///< Initializer value.
  COUNT_RANGE_FEATURE ComputeUnitRanges;          ///< The Processor and Node Counts.
} COMPUTE_UNIT_COUNTS;

/*------------------------------------------------------------------------------------------*/
/*
 * The specific data for each table entry.
 */
/*------------------------------------------------------------------------------------------*/

/**
 *  Make an extra type so we can use compilers that don't support designated initializers.
 *
 *  All the entry type unions are no more than 5 UINT32's in size.  For entry types which are a struct of UINT32's,
 *  this type can be used so that initializers can be declared TABLE_ENTRY_FIELDS, instead of a special non-union type.
 *  A non-union type then has to be cast back to TABLE_ENTRY_FIELDS in order to process the table, and you can't mix
 *  entry types with non-union initializers in the same table with any other type.
 *
 *  If the entry type contains anything but UINT32's, then it must have a non-union initializer type for creating the
 *  actual tables.  For example, MSR entry has UINT64 and workaround entry has a function pointer.
 */
typedef UINT32 GENERIC_TYPE_ENTRY_INITIALIZER[MAX_ENTRY_TYPE_ITEMS32];

/**
 * Table Entry Data for MSR Registers.
 *
 * Apply data to register after mask, for MSRs.
 */
typedef struct {
  UINT32  Address;                    ///< MSR address
  UINT64  Data;                       ///< Data to set in the MSR
  UINT64  Mask;                       ///< Mask to be applied to the MSR. Set every bit of all updated fields.
} MSR_TYPE_ENTRY_DATA;

/**
 * Table Entry Data for PCI Registers.
 *
 * Apply data to register after mask, for PCI Config registers.
 */
typedef struct {
  PCI_ADDR  Address;                  ///< Address should contain Function, Offset only.  It will apply to all CPUs
  UINT32    Data;                     ///< Data to be written into PCI device
  UINT32    Mask;                     ///< Mask to be used before data write. Set every bit of all updated fields.
} PCI_TYPE_ENTRY_DATA;

/**
 * Table Entry Data for Profile Fixup Registers.
 *
 * If TypeFeats matches current config, apply data to register after mask for PCI Config registers.
 */
typedef struct {
  PERFORMANCE_PROFILE_FEATS TypeFeats; ///< Profile Fixup Features.
  PCI_TYPE_ENTRY_DATA PciEntry;        ///< The PCI Register entry data.
} PROFILE_FIXUP_TYPE_ENTRY_DATA;

/**
 * Core Count dependent PCI registers.
 *
 */
typedef struct {
  PERFORMANCE_PROFILE_FEATS TypeFeats; ///< Profile Fixup Features.
  CORE_COUNT_RANGES CoreCounts;        ///< Specify up to two core count ranges to match.
  PCI_TYPE_ENTRY_DATA PciEntry;        ///< The PCI Register entry data.
} CORE_COUNTS_PCI_TYPE_ENTRY_DATA;

/**
 * Processor Count dependent PCI registers.
 *
 */
typedef struct {
  PERFORMANCE_PROFILE_FEATS TypeFeats; ///< Profile Fixup Features.
  PROCESSOR_COUNTS  ProcessorCounts;   ///< Specify a processor count range.
  PCI_TYPE_ENTRY_DATA PciEntry;        ///< The PCI Register entry data.
} PROCESSOR_COUNTS_PCI_TYPE_ENTRY_DATA;

/**
 * Compute Unit Count dependent PCI registers.
 *
 */
typedef struct {
  PERFORMANCE_PROFILE_FEATS TypeFeats;         ///< Profile Fixup Features.
  COMPUTE_UNIT_COUNTS       ComputeUnitCounts; ///< Specify a compute unit count range.
  PCI_TYPE_ENTRY_DATA       PciEntry;          ///< The PCI Register entry data.
} COMPUTE_UNIT_COUNTS_PCI_TYPE_ENTRY_DATA;

/**
 * Compute Unit Count dependent MSR registers.
 *
 */
typedef struct {
  COMPUTE_UNIT_COUNTS       ComputeUnitCounts; ///< Specify a compute unit count range.
  MSR_TYPE_ENTRY_DATA       MsrEntry;          ///< The MSR Register entry data.
} COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA;

/**
 * A Family Specific Workaround method.
 *
 * \@TableTypeFamSpecificInstances.
 *
 * When called, the entry's CPU Logical ID and Platform Features matched the current config.
 * The method must implement any specific criteria checking for the workaround.
 *
 * See if you can use the other entries or make an entry specifically for the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model specific.
 *
 * @param[in]     Data       The table data value, for example to indicate which CPU and Platform types matched.
 * @param[in]     StdHeader  Config params for library, services.
 */
typedef VOID F_FAM_SPECIFIC_WORKAROUND (
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );
/// Reference to a method.
typedef F_FAM_SPECIFIC_WORKAROUND *PF_FAM_SPECIFIC_WORKAROUND;

/**
 * Table Entry Data for Family Specific Workarounds.
 *
 * See if you can use the other entries or make an entry specifically for the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model specific.
 *
 * Call DoAction passing Data.
 */
typedef struct {
  PF_FAM_SPECIFIC_WORKAROUND  DoAction;     ///< A function implementing the workaround.
  UINT32                Data;               ///< This data is passed to DoAction().
} FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA;

/**
 * Package Type Features
 *
 * FamilyPackageType are various among CPU families.
 *
 */
typedef union {
  UINT32                      PackageTypeValue;    ///< Package Type
  PACKAGE_TYPE_FEATURES       FamilyPackageType;   ///< Package Type of CPU family
} PACKAGE_TYPE_FEATS;

/*------------------------------------------------------------------------------------------*/
/*
 * A complete register table and table entries.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * All the available entry data types.
 */
typedef union {
  GENERIC_TYPE_ENTRY_INITIALIZER          InitialValues;         ///< Not a valid entry type; as the first union item,
                                                                 ///< it can be used with initializers.
  MSR_TYPE_ENTRY_DATA                     MsrEntry;              ///< Msr entry.
  PCI_TYPE_ENTRY_DATA                     PciEntry;              ///< PCI entry.
  FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA FamSpecificEntry;      ///< Family Specific Workaround entry.
  PROFILE_FIXUP_TYPE_ENTRY_DATA           FixupEntry;            ///< Profile Fixup entry.
  CORE_COUNTS_PCI_TYPE_ENTRY_DATA         CoreCountEntry;        ///< Core count dependent settings.
  PROCESSOR_COUNTS_PCI_TYPE_ENTRY_DATA    ProcCountEntry;        ///< Processor count entry.
  COMPUTE_UNIT_COUNTS_PCI_TYPE_ENTRY_DATA CompUnitCountEntry;    ///< Compute unit count dependent entry.
  COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA CompUnitCountMsrEntry; ///< Compute unit count dependent MSR entry.
} TABLE_ENTRY_DATA;

/**
 * Register Table Entry common fields.
 *
 * All the various types of register table entries are subclasses of this object.
 */
typedef struct {
  TABLE_ENTRY_TYPE       EntryType;                  ///< The type of table entry this is.
  CPU_LOGICAL_ID         CpuRevision;                ///< Common CPU Logical ID match criteria.
  PLATFORM_FEATS         Features;                   ///< Common Platform Features match criteria.
  TABLE_ENTRY_DATA       Entry;                      ///< The type dependent entry data (ex. register, data, mask).
} TABLE_ENTRY_FIELDS;

/**
 * An entire register table.
 */
typedef struct {
  TABLE_CORE_SELECTOR  Selector;                     ///< For efficiency, these cores should process this table
  UINTN                NumberOfEntries;              ///< The number of entries in the table.
  CONST TABLE_ENTRY_FIELDS  *Table;                  ///< The table entries.
} REGISTER_TABLE;

/*------------------------------------------------------------------------------------------*/
/*
 * Describe implementers for table entries.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * Implement the semantics of a Table Entry Type.
 *
 * @TableEntryTypeInstances.
 *
 * @param[in]       CurrentEntry        The type specific entry data to be implemented (that is written).
 * @param[in]       PlatformConfig      Config handle for platform specific information
 * @param[in]       StdHeader           Config params for library, services.
 */
typedef VOID F_DO_TABLE_ENTRY (
  IN       TABLE_ENTRY_DATA       *CurrentEntry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );
/// Reference to a method
typedef F_DO_TABLE_ENTRY *PF_DO_TABLE_ENTRY;

/*------------------------------------------------------------------------------------------*/
/*
 * Non-union initializers for entry data which is not just UINT32.
 */
/*------------------------------------------------------------------------------------------*/

/**
 * A union of data types, that can be initialized with MSR data.
 *
 * This ensures the entry data is the same size as TABLE_ENTRY_DATA.
 */
typedef union {
  MSR_TYPE_ENTRY_DATA    MsrInitializer;            ///< The data in the table initializer is assigned to this member.
  TABLE_ENTRY_DATA       Reserved;                  ///< Make sure the size is the same as the real union.
} MSR_DATA_INITIALIZER;

/**
 * A type suitable for an initializer for MSR Table entries.
 */
typedef struct {
  TABLE_ENTRY_TYPE       Type;                       ///< The type of table entry this is.
  CPU_LOGICAL_ID         CpuRevision;                ///< Common CPU Logical ID match criteria.
  PLATFORM_FEATS         Features;                   ///< Common Platform Features match criteria.
  MSR_DATA_INITIALIZER    EntryData;                 ///< The special union which accepts msr data initializer.
} MSR_TYPE_ENTRY_INITIALIZER;

/**
 * A union of data types, that can be initialized with MSR CU data.
 *
 * This ensures the entry data is the same size as TABLE_ENTRY_DATA.
 */
typedef union {
  COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA  MsrInitializer;  ///< The data in the table initializer is assigned to this member.
  TABLE_ENTRY_DATA       Reserved;                          ///< Make sure the size is the same as the real union.
} MSR_CU_DATA_INITIALIZER;

/**
 * A type suitable for an initializer for MSR CU count Table entries.
 */
typedef struct {
  TABLE_ENTRY_TYPE           Type;                   ///< The type of table entry this is.
  CPU_LOGICAL_ID             CpuRevision;            ///< Common CPU Logical ID match criteria.
  PLATFORM_FEATS             Features;               ///< Common Platform Features match criteria.
  MSR_CU_DATA_INITIALIZER    EntryData;              ///< The special union which accepts msr data initializer.
} MSR_CU_TYPE_ENTRY_INITIALIZER;

/**
 * A union of data types, that can be initialized with Family Specific Workaround data.
 *
 * This ensures the entry data is the same size as TABLE_ENTRY_DATA.
 */
typedef union {
  FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA    FamSpecificInitializer; ///< The data in the table initializer is assigned to this member.
  TABLE_ENTRY_DATA       Reserved;                                   ///< Make sure the size is the same as the real union.
} FAM_SPECIFIC_WORKAROUND_DATA_INITIALIZER;

/**
 * A type suitable for an initializer for Family Specific Workaround Table entries.
 */
typedef struct {
  TABLE_ENTRY_TYPE       Type;                             ///< The type of table entry this is.
  CPU_LOGICAL_ID         CpuRevision;                      ///< Common CPU Logical ID match criteria.
  PLATFORM_FEATS         Features;                         ///< Common Platform Features match criteria.
  FAM_SPECIFIC_WORKAROUND_DATA_INITIALIZER    EntryData;   ///< Special union accepts family specific workaround data initializer.
} FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_INITIALIZER;

/*------------------------------------------------------------------------------------------*/
/*
 * Table related function prototypes (many are instance of F_DO_TABLE_ENTRY method).
 */
/*------------------------------------------------------------------------------------------*/

/**
 * Get the next register table
 */
REGISTER_TABLE **GetNextRegisterTable (
  IN       TABLE_CORE_SELECTOR     Selector,
  IN       REGISTER_TABLE         **RegisterTableList,
  IN OUT   REGISTER_TABLE       ***RegisterTableHandle,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/**
 * If current core is CoreSelector core
 */
BOOLEAN
IsCoreSelector (
  IN       TABLE_CORE_SELECTOR       Selector,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  );

/**
 * Set the registers for this core based on entries in a list of Register Table.
 */
VOID
SetRegistersFromTable (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       REGISTER_TABLE         **RegisterTable,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/**
 * Find the features of the running platform.
 */
VOID
GetPlatformFeatures (
     OUT   PLATFORM_FEATS    *Features,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/**
 * Checks register table entry type specific criteria to the platform.
 */
BOOLEAN
DoesEntryTypeSpecificInfoMatch (
  IN       UINT32   PlatformTypeSpecificFeatures,
  IN       UINT32   EntryTypeFeatures
  );

/**
 * Perform the MSR Register Entry.
 */
VOID
SetRegisterForMsrEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Perform the PCI Register Entry.
 */
VOID
SetRegisterForPciEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Perform the Performance Profile PCI Register Entry.
 */
VOID
SetRegisterForPerformanceProfileEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Perform the Core Counts Performance PCI Register Entry.
 */
VOID
SetRegisterForCoreCountsPerformanceEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Perform the Processor Counts PCI Register Entry.
 */
VOID
SetRegisterForProcessorCountsEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Perform the Compute Unit Counts PCI Register Entry.
 */
VOID
SetRegisterForComputeUnitCountsEntry (
  IN       TABLE_ENTRY_DATA       *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/**
 * Perform the Compute Unit Counts MSR Register Entry.
 */
VOID
SetMsrForComputeUnitCountsEntry (
  IN       TABLE_ENTRY_DATA       *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

/**
 * Perform the Family Specific Workaround Register Entry.
 */
VOID
SetRegisterForFamSpecificWorkaroundEntry (
  IN       TABLE_ENTRY_DATA      *Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

/**
 * Compare counts to a pair of ranges.
 */
BOOLEAN
IsEitherCountInRange (
  IN       UINTN                FirstCount,
  IN       UINTN                SecondCount,
  IN       COUNT_RANGE_FEATURE  Ranges
  );

/**
 * Returns the performance profile features list of the currently running processor core.
 */
VOID
GetPerformanceFeatures (
     OUT   PERFORMANCE_PROFILE_FEATS    *Features,
  IN       PLATFORM_CONFIGURATION       *PlatformConfig,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  );

#endif  // _CPU_TABLE_H_

