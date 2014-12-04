/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Cpu SMM Save state driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/******************************************************************************
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
****************************************************************************/
#include "SmmCpuProtocol.h"

EFI_SMM_BASE2_PROTOCOL  *mSmmBasePtr;

EFI_SMM_CPU_PROTOCOL    mSmmCpuProtocol = {
   ReadCpuSaveState,
   WriteCpuSaveSate
};

//
// Build the map of various register entries in the Smm save area
//
EFI_SMM_SAVE_AREA_MAP  mSmmSaveAreaMap[] = {
//  { SMM_OFFSET,    EFI_REGISTER,                       size in save area

  { 0xFE00, EFI_SMM_SAVE_STATE_REGISTER_ES,            sizeof (UINT16) }, // ES_Selector
//  { 0xFE02, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // ES_Attributes
//  { 0xFE04, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // ES_Limit
//  { 0xFE08, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64)},  // ES_Base
    { 0xFE10, EFI_SMM_SAVE_STATE_REGISTER_CS,            sizeof (UINT16) }, // CS_Selector
//  { 0xFE12, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // CS_Attributes
//  { 0xFE14, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // CS_Limit
//  { 0xFE18, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // CS_Base
    { 0xFE20, EFI_SMM_SAVE_STATE_REGISTER_SS,            sizeof (UINT16) }, // SS_Selector
//  { 0xFE22, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // SS_Attributes
//  { 0xFE24, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // SS_Limit
//  { 0xFE28, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SS_Base
    { 0xFE30, EFI_SMM_SAVE_STATE_REGISTER_DS,            sizeof (UINT16) }, // DS_Selector
//  { 0xFE32, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // DS_Attributes
//  { 0xFE34, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // DS_Limit
//  { 0xFE38, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // DS_Base
    { 0xFE40, EFI_SMM_SAVE_STATE_REGISTER_FS,            sizeof (UINT16) }, // FS_Selector
//  { 0xFE42, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // FS_Attributes
//  { 0xFE44, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // FS_Limit
//  { 0xFE48, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // FS_Base
    { 0xFE50, EFI_SMM_SAVE_STATE_REGISTER_GS,            sizeof (UINT16) }, // GS_Selector
//  { 0xFE52, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // GS_Attributes
//  { 0xFE54, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // GS_Limit
//  { 0xFE58, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // GS_Base

  { 0xFE64, EFI_SMM_SAVE_STATE_REGISTER_GDTLIMIT,      sizeof (UINT16) }, // GDTR_Limit
  { 0xFE68, EFI_SMM_SAVE_STATE_REGISTER_GDTBASE,       sizeof (UINT64) }, // GDTR_Base
  { 0xFE70, EFI_SMM_SAVE_STATE_REGISTER_LDTR_SEL,      sizeof (UINT16) }, // LDTR_Selector
  { 0xFE72, EFI_SMM_SAVE_STATE_REGISTER_LDTINFO,       sizeof (UINT16) }, // LDTR_Attributes
  { 0xFE74, EFI_SMM_SAVE_STATE_REGISTER_LDTLIMIT,      sizeof (UINT32) }, // LDTR_Limit
  { 0xFE78, EFI_SMM_SAVE_STATE_REGISTER_LDTBASE,       sizeof (UINT64) }, // LDTR_Base
  { 0xFE84, EFI_SMM_SAVE_STATE_REGISTER_IDTLIMIT,      sizeof (UINT16) }, // IDTR_Limit
  { 0xFE88, EFI_SMM_SAVE_STATE_REGISTER_IDTBASE,       sizeof (UINT64) }, // IDTR_Base
  { 0xFE90, EFI_SMM_SAVE_STATE_REGISTER_TR_SEL,        sizeof (UINT16) }, // TR_Selector
//  { 0xFE92, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT16) }, // TR_Attributes
//  { 0xFE94, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // TR_Limit
//  { 0xFE98, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // TR_Base
                                                                            //
//  { 0xFEA0, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // IO_RIP
//  { 0xFEA8, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) },  // IO_RCX
//  { 0xFEB0, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // IO_RSI
//  { 0xFEB8, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // IO_RDI
  { 0xFEC0, EFI_SMM_SAVE_STATE_REGISTER_IO,            sizeof (UINT32) }, // IO_Restart_Dword
//  { 0xFEC8, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT8)  }, // IO_Restart
//  { 0xFEC9, EFI_SMM_SAVE_STATE_REGISTER_NA             sizeof (UINT8)  }, // Auto_Halt_Restart

//  { 0xFED0, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // EFER
//  { 0xFED8, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Guest
//  { 0xFEE0, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Guest_VMCB_Address
//  { 0xFEE8, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Guest_Virtual_Interrupt
//  { 0xFEFC, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // SMM_Revision_Identifier1
//  { 0xFF00, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT32) }, // SMBASE
//  { 0xFF20, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Guest_PAT
//  { 0xFF28, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Host_EFER
//  { 0xFF30, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Host_CR4
//  { 0xFF38, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Host_CR3
//  { 0xFF40, EFI_SMM_SAVE_STATE_REGISTER_NA,            sizeof (UINT64) }, // SVM_Host_CR0

  { 0xFF48, EFI_SMM_SAVE_STATE_REGISTER_CR4,           sizeof (UINT64) }, // CR4
  { 0xFF50, EFI_SMM_SAVE_STATE_REGISTER_CR3,           sizeof (UINT64) }, // CR3
  { 0xFF58, EFI_SMM_SAVE_STATE_REGISTER_CR0,           sizeof (UINT64) }, // CR0
  { 0xFF60, EFI_SMM_SAVE_STATE_REGISTER_DR7,           sizeof (UINT64) }, // DR7
  { 0xFF68, EFI_SMM_SAVE_STATE_REGISTER_DR6,           sizeof (UINT64) }, // DR6
  { 0xFF70, EFI_SMM_SAVE_STATE_REGISTER_RFLAGS,        sizeof (UINT64) }, // RFLAGS
  { 0xFF78, EFI_SMM_SAVE_STATE_REGISTER_RIP,           sizeof (UINT64) }, // RIP

  { 0xFF80, EFI_SMM_SAVE_STATE_REGISTER_R15,          sizeof (UINT64) },  // R15
  { 0xFF88, EFI_SMM_SAVE_STATE_REGISTER_R14,          sizeof (UINT64) },  // R14
  { 0xFF90, EFI_SMM_SAVE_STATE_REGISTER_R13,          sizeof (UINT64) },  // R13
  { 0xFF98, EFI_SMM_SAVE_STATE_REGISTER_R12,          sizeof (UINT64) },  // R12
  { 0xFFA0, EFI_SMM_SAVE_STATE_REGISTER_R11,          sizeof (UINT64) },  // R11
  { 0xFFA8, EFI_SMM_SAVE_STATE_REGISTER_R10,          sizeof (UINT64) },  // R10
  { 0xFFB0, EFI_SMM_SAVE_STATE_REGISTER_R9,           sizeof (UINT64) },  // R9
  { 0xFFB8, EFI_SMM_SAVE_STATE_REGISTER_R8,           sizeof (UINT64) },  // R8

  { 0xFFC0, EFI_SMM_SAVE_STATE_REGISTER_RDI,          sizeof (UINT64) },  // RDI
  { 0xFFC8, EFI_SMM_SAVE_STATE_REGISTER_RSI,          sizeof (UINT64) },  // RSI
  { 0xFFD0, EFI_SMM_SAVE_STATE_REGISTER_RBP,          sizeof (UINT64) },  // RBP
  { 0xFFD8, EFI_SMM_SAVE_STATE_REGISTER_RSP,          sizeof (UINT64) },  // RSP
  { 0xFFE0, EFI_SMM_SAVE_STATE_REGISTER_RBX,          sizeof (UINT64) },  // RBX
  { 0xFFE8, EFI_SMM_SAVE_STATE_REGISTER_RDX,          sizeof (UINT64) },  // RDX
  { 0xFFF0, EFI_SMM_SAVE_STATE_REGISTER_RCX,          sizeof (UINT64) },  // RCX
  { 0xFFF8, EFI_SMM_SAVE_STATE_REGISTER_RAX,          sizeof (UINT64) },  // RAX
};

EFI_PHYSICAL_ADDRESS                *mCpuSaveBaseArray;

GLOBAL_REMOVE_IF_UNREFERENCED VOID *mCpuSaveArea;

/*---------------------------------------------------------------------------------------*/
/**
 *  Get SMM base.
 *
 *  This function is used to get the SMM base of CPU. It will be exectuted on various BSP/AP thread by
 *  this driver to get Smmbase of various cores. This function will read the respective CPU MSR to get
 *  the SMM base
 *
 *  @param[in,out] Buffer             Upon return, this holds the SmmBase value read from the MSR
 *
 *  @retval NA
**/
/*---------------------------------------------------------------------------------------*/
VOID
EFIAPI
GetSmmBase (
  IN OUT   VOID  *Buffer
  )
{
  //Read MSR to get SMM base. FMO - Should this be moved to library function
  *(EFI_PHYSICAL_ADDRESS *)Buffer = AsmReadMsr64 (MSR_CPU_SMM_BASE);
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Return Physical memory address of SmmSave area that will hold the value of register in SmmSaveArea
 *
 *  @param[in]  Width              The number of bytes to read from the CPU save state.
 *  @param[in]  Register           Register
 *  @param[in]  CpuIndex           Specifies the zero-based index of the CPU save state.
 *  @param[out] SmmRegAddress      Address pointer
 *
 *  @retval EFI_SUCCESS            The register was read from Save State.
 *  @retval EFI_NOT_FOUND          The register is not defined for the Save State of Processor.
 *  @retval EFI_INVALID_PARAMETER  Input parameters are not valid, for example, Processor No or register width
 *                                 is not correct.This or Buffer is NULL.
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
GetSmmSaveAreaOffset (
  IN       UINTN                        Width,
  IN       EFI_SMM_SAVE_STATE_REGISTER  Register,
  IN       UINTN                        CpuIndex,
     OUT   EFI_PHYSICAL_ADDRESS        *SmmRegAddress
  )
{
  UINTN i;
  EFI_PHYSICAL_ADDRESS    TempAddress;

  //Bail if CpuIndex invalid
  if (CpuIndex > gSmst->NumberOfCpus) {
    return (EFI_INVALID_PARAMETER);
  }

  // Loop through the structure and see if the register is defined in the SaveAreamap
  for (i = 0; i < sizeof (mSmmSaveAreaMap) / sizeof (EFI_SMM_SAVE_AREA_MAP); i++ ) {

    if (mSmmSaveAreaMap[i].Register == Register) {
      // Check if width is adequate
      if (mSmmSaveAreaMap[i].RegisterWidth < Width) {
        return (EFI_INVALID_PARAMETER);
      }

      // Checks OK. SmmRegAddress = SmmBase of CPU + SmmRegOffset
      TempAddress = (EFI_PHYSICAL_ADDRESS)mCpuSaveBaseArray[CpuIndex] + (EFI_PHYSICAL_ADDRESS)mSmmSaveAreaMap[i].SmmSaveRegOffset;
      switch (Width) {
      case 1:
        *SmmRegAddress = *(UINT8 *) TempAddress;
        break;

      case 2:
        *SmmRegAddress = *(UINT16 *) TempAddress;
        break;

      case 4:
        *SmmRegAddress = *(UINT32 *) TempAddress;
        break;

      case 8:
        *SmmRegAddress = *(UINT64 *) TempAddress;
        break;

      default:
        ASSERT (FALSE);
      }
      return  (EFI_SUCCESS);
    }
  }

  return (EFI_NOT_FOUND);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Read Smm area for IO cycle information outside SMM
 *
 *  This function is used to read the proxy EFI_SMM_SAVE_STATE_REGISTER_IO register
 *  This function read the IoRestartDword offset in the Smm save area and process
 *  this dword to build the SMM_SAVE_STATE_IO_INFO in the Buffer
 *
 *  @param[in]  Width              The number of bytes to read from the CPU save state.
 *  @param[in]  CpuIndex           Specifies the zero-based index of the CPU save state.
 *  @param[out] Buffer             Upon return, this holds the SMM_SAVE_STATE_IO_INFO buffer if there is pending IO outside SMM
 *
 *  @retval EFI_SUCCESS            The register was read from Save State.
 *  @retval EFI_NOT_FOUND          The register is not defined for the Save State of Processor.
 *  @retval EFI_INVALID_PARAMETER  Input parameters are not valid, for example, Processor No or register width
 *                                 is not correct.This or Buffer is NULL.
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
ReadIoRestartRegister (
  IN       UINTN    Width,
  IN       UINTN    CpuIndex,
     OUT   VOID     *Buffer
  )
{
  EFI_STATUS                   Status;
  IO_RESTART_DWORD             IoRestart;
  EFI_SMM_SAVE_STATE_IO_INFO  *CpuIoInfo = Buffer;

  if (Width != sizeof (EFI_SMM_SAVE_STATE_IO_INFO)) {
    return (EFI_INVALID_PARAMETER);
  }

  // initialize the buffer that need to be updated
  ZeroMem (Buffer, Width);

  // Find the physical location of Smm save area where IoRestartDword is saved
  Status = GetSmmSaveAreaOffset (
                                 sizeof (UINT32),
                                 EFI_SMM_SAVE_STATE_REGISTER_IO,
                                 CpuIndex,
                                 (VOID *)&IoRestart
                                 );

  // If valid bit not set then no IO instuction at time of SMM. return
  if (!IoRestart.Val) {
    return (EFI_SUCCESS);
  }

  //
  // Continue with the update. First update IoType
  //
  if (IoRestart.Type) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_TYPE_INPUT;
  } else {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_TYPE_OUTPUT;
  }

  if (IoRestart.REP) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_TYPE_REP_PREFIX;
  }

  if (IoRestart.STR) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_TYPE_STRING;
  }

  //
  // Update IoWidth
  //
  if (IoRestart.SZ8) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8;
  } else if (IoRestart.SZ16) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_WIDTH_UINT16;
  } else if (IoRestart.SZ32) {
    CpuIoInfo->IoWidth = EFI_SMM_SAVE_STATE_IO_WIDTH_UINT32;
  }

  //
  // Update IoPort
  //
  CpuIoInfo->IoPort = (UINT16) IoRestart.Port;

  //
  // Update IoData. Read from Ioport to update the IoData
  //
  gSmst->SmmIo.Io.Read (
    &gSmst->SmmIo,
    CpuIoInfo->IoWidth,
    CpuIoInfo->IoPort,
    1,
    &CpuIoInfo->IoData
    );

  // CpuIoInfo/buffer is ready return success
  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Read data from the CPU save state.
 *
 *  This function is used to read the specified number of bytes of the specified register from the CPU
 *  save state of the specified CPU and place the value into the buffer. If the CPU does not support the
 *  specified register Register, then EFI_NOT_FOUND  should be returned. If the CPU does not
 *  support the specified register width Width, then EFI_INVALID_PARAMETER is returned.
 *
 *  @param[in]  This               The EFI_SMM_CPU_PROTOCOL instance.
 *  @param[in]  Width              The number of bytes to read from the CPU save state.
 *  @param[in]  Register           Specifies the CPU register to read form the save state.
 *  @param[in]  CpuIndex           Specifies the zero-based index of the CPU save state.
 *  @param[out] Buffer             Upon return, this holds the CPU register value read from the save state.
 *
 *  @retval EFI_SUCCESS            The register was read from Save State.
 *  @retval EFI_NOT_FOUND          The register is not defined for the Save State of Processor.
 *  @retval EFI_INVALID_PARAMETER  Input parameters are not valid, for example, Processor No or register width
 *                                 is not correct.This or Buffer is NULL.
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
ReadCpuSaveState (
  IN       CONST EFI_SMM_CPU_PROTOCOL   *This,
  IN       UINTN                        Width,
  IN       EFI_SMM_SAVE_STATE_REGISTER  Register,
  IN       UINTN                        CpuIndex,
     OUT   VOID                        *Buffer
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Source;

  //Check for Pseudo register
  if ((Register == EFI_SMM_SAVE_STATE_REGISTER_IO)) {
          return ( ReadIoRestartRegister (
                      Width,
                      CpuIndex,
                      Buffer
                      )
                  );
  }

  Status = GetSmmSaveAreaOffset (
                                 Width,
                                 Register,
                                 CpuIndex,
                                 &Source
                                 );

  if (EFI_ERROR (Status)) {
    return (Status);
  }

  CopyMem (Buffer,
           (VOID *)&Source,
           Width
           );

  //Check for PseudoRegister

  return (EFI_SUCCESS);
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Write data to the CPU save state.
 *
 *  This function is used to write the specified number of bytes of the specified register to the CPU save
 *  state of the specified CPU and place the value into the buffer. If the CPU does not support the
 *  specified register Register, then EFI_UNSUPPORTED should be returned. If the CPU does not
 *  support the specified register width Width, then EFI_INVALID_PARAMETER is returned.
 *
 *  @param[in]  This               The EFI_SMM_CPU_PROTOCOL instance.
 *  @param[in]  Width              The number of bytes to write to the CPU save state.
 *  @param[in]  Register           Specifies the CPU register to write to the save state.
 *  @param[in]  CpuIndex           Specifies the zero-based index of the CPU save state.
 *  @param[in]  Buffer             Upon entry, this holds the new CPU register value.
 *
 *  @retval EFI_SUCCESS            The register was written to Save State.
 *  @retval EFI_NOT_FOUND          The register is not defined for the Save State of Processor.
 *  @retval EFI_INVALID_PARAMETER  Input parameters are not valid. For example:
 *                                 ProcessorIndex or Width is not correct.
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
WriteCpuSaveSate (
  IN CONST EFI_SMM_CPU_PROTOCOL   *This,
  IN UINTN                        Width,
  IN EFI_SMM_SAVE_STATE_REGISTER  Register,
  IN UINTN                        CpuIndex,
  IN CONST VOID                   *Buffer
  )
{

  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Destination;

  //Check for Pseudo register
  if ((Register == EFI_SMM_SAVE_STATE_REGISTER_IO) ||
      (Register == EFI_SMM_SAVE_STATE_REGISTER_LMA )) {
        return (EFI_INVALID_PARAMETER);
  }

  Status = GetSmmSaveAreaOffset (
                                 Width,
                                 Register,
                                 CpuIndex,
                                 &Destination);

  if (EFI_ERROR (Status)) {
    return (Status);
  }

  CopyMem ((VOID *)Destination, Buffer, Width);
  return (EFI_SUCCESS);

}

/*---------------------------------------------------------------------------------------*/
/**
 *  Just a test case
 *
 *  This function is used to test this driver.
 *  Get the RIP of all cores outside the SMM mode
 *
 *  @retval EFI_SUCCESS            Test run successafully
 *  @retval EFI_NOT_FOUND          Incorrect CPU or invalid EIP offset
 *  @retval EFI_INVALID_PARAMETER  Incorrect parameter
 *
**/
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
TestInterface ()
{
  EFI_STATUS            Status;
  UINTN                 i;
  UINT64                Data;

  EFI_SMM_CPU_PROTOCOL  *CpuSaveProtocol;

  //Locate SmmCpuProtocol
  Status = gSmst->SmmLocateProtocol (
               &gEfiSmmCpuProtocolGuid,
               NULL,
               &CpuSaveProtocol
               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Read RAX from all cores
  for (i = 0; i < gSmst->NumberOfCpus; i++) {
    // Use SmmCpuProtocol to read the RIP of each core outside SMM. Read this value from Smm space
    Status = CpuSaveProtocol->ReadSaveState (
                                    CpuSaveProtocol,
                                    sizeof (UINT64),
                                    EFI_SMM_SAVE_STATE_REGISTER_RIP,
                                    i,
                                    &Data);
    DEBUG ((EFI_D_ERROR, "TestInterface : Cpu %x rip %08x \n", i, Data));

    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return (Status);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD SMM CPU Protocol driver
 * Example of dispatcher driver that handled IO TRAP requests only
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
SmmCpuProtocolEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    SmmCpuProtocolHandle = NULL;
  UINTN                         i;
  BOOLEAN                       InSmm;

  // Locate Smmbase2 protocol
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  &mSmmBasePtr
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Check if in Smm mode. Bail out if not
  mSmmBasePtr->InSmm (mSmmBasePtr, &InSmm);
  if (!InSmm) {
    return EFI_LOAD_ERROR;
  }

  //
  // We are in Smm mode
  //

  // Allocate buffer to save SmmBase of all the cores
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (EFI_PHYSICAL_ADDRESS) * gSmst->NumberOfCpus,
                    &mCpuSaveBaseArray
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get SmmBase of all cores and save in arrary allocated above
  for (i = 0; i < gSmst->NumberOfCpus; i++) {
    Status = gSmst->SmmStartupThisAp (
                     GetSmmBase,
                     i,
                     &mCpuSaveBaseArray[i]
                  );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    DEBUG ((EFI_D_ERROR, "SmmCpuProtocol:: Cpu %x SmmBase %08x \n", i, mCpuSaveBaseArray[i]));
  }

  // All done install the SmmCpuProtocol
  Status = gSmst->SmmInstallProtocolInterface (
               &SmmCpuProtocolHandle,
               &gEfiSmmCpuProtocolGuid,
               EFI_NATIVE_INTERFACE,
               &mSmmCpuProtocol
               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TestInterface ();

  return Status;
}

/*   FMO Check if we need to update Smst Cpu save area. If so allocate the memory here
     and update on each SMM entry/exit. But saving/restoring this area on every SMM entry
     will extend the SMM time (as we need to do it every Smm entry/exit) So for now only
     expose the SmmCpuProtocol. Also in that case we need to move this code into
     SmmPlatformLib so that SMM save area can be saved/restore on every SmmEntry/exit

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    CPU_SAVE_AREA_SIZE * gSmst->NumberOfCpus,
                    &mCpuSaveArea
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gSmst->CpuSaveState = mCpuSaveArar

  //Also hook SMMHandler to update this data structure
  ..
*/
