/* $NoKeywords:$ */
/**
 * @file
 *
 * Amd Smm 64 bit entry/exit code
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   SMM Access DXE Driver
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
#include <PiSmm.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmCommunication.h>
#include <Porting.h>
#include "SmmFoundation.h"

#define HandleLocalCoreSmm(x)      //TBD this need to move to Library class SmmFoundation will call
#define IsSmmFromIoHub() TRUE  //TBD this need to move to Library class SmmFoundation will call

#define SMM_WAIT_TIMEOUT  (1200)    //Wait for Smm sync = 2 min??
#define MSR_CPU_SMM_BASE  0xc0010111ul


#define DEBUG_SPIN(SpinLock, Expression)      \
      while (!AcquireSpinLockOrFail (SpinLock));      \
      DEBUG (Expression);                     \
      ReleaseSpinLock (SpinLock);

typedef
VOID
  (EFIAPI *SMMCORE_ENTRY) (
  IN CONST EFI_SMM_ENTRY_CONTEXT  *);

//Define Semaphore
SPIN_LOCK                     *gSmmSpin;

SPIN_LOCK                     gCheckSpringBoard;
volatile UINT32               gNotInSmm;
volatile UINT32               gWaitInSmm;
volatile UINT32               FailSmmCoreCount;
volatile BOOLEAN              WaitInSmmCheck;
volatile BOOLEAN              gExitSmm;


volatile UINTN                gApServiceIndex    = 0;
volatile EFI_AP_PROCEDURE     gApServiceFn        = NULL;
VOID                          *gApServiceParam    = NULL;

volatile BOOLEAN        gSmmInitDone      = FALSE;
STATIC UINTN gSmmEntryCount[4] = { 0, 0, 0, 0 };
STATIC UINTN gSmmExitCount[4]  = { 0, 0, 0, 0 };



#define LOCAL_APIC_ADDR         0xFEE00000ul
#define INT_CMD_REG_LO          0x300
#define INT_CMD_REG_HI          0x310
#define APIC_ID_REG             0x20
#define INIT          (5 << 8)
#define SIPI          (6 << 8)
#define LEVEL_ASSERT      (1 << 14)

typedef struct {
  UINT8   Reserved1[0xfe00]; // 0xfe00
  UINT8   Reserved2[0xc9];   //
  UINT8   SMMFEC9;           // 0xfec9
  UINT8   Reserved3[0x35];   //
} SMM_CPU_STATE64;

/*----------------------------------------------------------------------------------------*/
/**
 *  Send SMI to corrosponding cpu core using Smi IPL message via local APIC
 *
 *  This function trigger SMI to respective ApicId (0xff means to all cores)
 *
 *  @param[in]  ApicId  Cpu core number (zero base)
 *
 *  @return  None
 *
**/
/*----------------------------------------------------------------------------------------*/
VOID
SendSmiIpl (
  IN       UINT8 ApicId
  )
{
  UINT32           val;
  volatile UINT32 *MemIo;

  // FMO move to seperate library
  val = (UINT8) ApicId;

  MemIo = (volatile UINT32 *) (UINTN) (LOCAL_APIC_ADDR + INT_CMD_REG_HI);
  *MemIo &= 0x00ffffff;
  *MemIo |= (val << 24);

  MemIo = (volatile UINT32 *) (UINTN) (LOCAL_APIC_ADDR + INT_CMD_REG_LO);
  if (ApicId == 0xff) {
    *MemIo = (BIT9 | BIT19);    // DSH[bit18:19]= 10 - All including self, MT[bit8:10] = 010 - Message SMI
  } else {
    *MemIo = BIT9;              // MT[bit8:10] = 010 - Message SMI
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *  AP Service routine to handle any BSP calls
 *
 *  This function handle calls coming from BSP
 *
 *  @param[in]  SmmHandle   Smm foundation handle
 *
 *  @return  None
 *
**/
/*----------------------------------------------------------------------------------------*/
VOID
ApServiceBspRequest (
  IN       SMM_FOUNDATION_TABLE  *SmmHandle
  )
{
  // Check if BSP has set any pending request for respective AP
  if (gApServiceIndex == (UINTN) SmmHandle->CpuNumber) {

    //DEBUG_SPIN (SmmHandle->SmmSpinLock, (EFI_D_ERROR, "SMM: Run Task %x on core %x \n", gApServiceFn, SmmHandle->CpuNumber ));
    gApServiceFn (gApServiceParam);
    gApServiceIndex = 0;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 *  AP Service routine to handle BSP call to start process on a specific CPU thread.
 *  This routine is passed to SmmCore as part of context buffer
 *  This function is called by Smm core
 *
 *  @param[in]      Procedure              Function address that Smmcore wishes to execute on specific core
 *  @param[in]      CpuNumber              The core id where this procedure need to be executed
 *  @param[in,out]  OPTIONAL ProcArguments The parameter to the procedure
 *
 *  @retval         Status.
**/
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
SmmStartupThisAp (
  IN       EFI_AP_PROCEDURE  Procedure,
  IN       UINTN             CpuNumber,
  IN OUT   VOID              *ProcArguments OPTIONAL
  )
{
  if (!CpuNumber) {
    //If this is for Bsp Handle it here
    Procedure (ProcArguments);
    return EFI_SUCCESS;
  }

  // Find the respective AP service index and set the value to request the AP to handle the call
  gApServiceFn    = Procedure;
  gApServiceParam = ProcArguments;
  gApServiceIndex = CpuNumber;

  while (gApServiceIndex);

  gApServiceFn = NULL;

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Main entry point of all the AP's
 *
 *  @param[in]  SmmHandle Smm foundation handle for respective core
 *
 *  @return  none
 *
**/
/*----------------------------------------------------------------------------------------*/
VOID
ApHandler (
  IN       SMM_FOUNDATION_TABLE    *SmmHandle
  )
{

  // Now wait till BSP is done. Service any call from BSP
  //
  while (gExitSmm == FALSE) {
    ApServiceBspRequest (SmmHandle);
  }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Main entry point of BP SMM handle
 *
 *  @param[in]  SmmHandle Smm foundation handle for respective core
 *
 *  @return  none
 *
**/
/*----------------------------------------------------------------------------------------*/
VOID
BspHandler (
  IN       SMM_FOUNDATION_TABLE    *SmmHandle
  )
{
  EFI_SMM_ENTRY_CONTEXT   Context;

  //
  // All core in Smm now. Go to main SMM Handler
  //
  Context.SmmStartupThisAp = SmmStartupThisAp;
  Context.CurrentlyExecutingCpu = SmmHandle->CpuNumber;
  Context.NumberOfCpus = SmmHandle->TotalCores;
  Context.CpuSaveStateSize = NULL;
  Context.CpuSaveState = NULL;

  if (SmmHandle->UefiSmmCoreBase) {
    ((SMMCORE_ENTRY )SmmHandle->UefiSmmCoreBase) ((CONST EFI_SMM_ENTRY_CONTEXT *)&Context);
  }

}


/*----------------------------------------------------------------------------------------*/
/**
 *  Smm global init routine
 *  Initialize the SMM global variable
 *
 *  @param[in]  SmmHandle Smm foundation handle for respective core
 *
 *  @return  none
 *
**/
/*----------------------------------------------------------------------------------------*/
VOID
SmmEarlyInit (
  IN       SMM_FOUNDATION_TABLE    *SmmHandle
  )
{

  // Aquire Global Semaphore and peform the Global init for the very first SMM
  while (!AcquireSpinLockOrFail (SmmHandle->SmmSpinLock));

  // If this is the very first time we are in SMM then Initialize global variable
  if (!gSmmInitDone) {
    gSmmSpin = SmmHandle->SmmSpinLock;
    gWaitInSmm = gNotInSmm  = SmmHandle->TotalCores;
    InitializeSpinLock (&gCheckSpringBoard);
    gSmmInitDone = TRUE;
    WaitInSmmCheck = TRUE;
  }

  ReleaseSpinLock (SmmHandle->SmmSpinLock);
}


/*----------------------------------------------------------------------------------------*/
/**
 *  Check if there is timeout
 *
 *  This function converts the elapsed ticks of running performance counter to
 *  time value in unit of nanoseconds.
 *
 *  @param[in]      Timeout   Timeout value in second
 *  @param[in,out]  StartTick Start time
 *
 *  @return True if timer
 *
**/
/*----------------------------------------------------------------------------------------*/
BOOLEAN
EFIAPI
IsTimeOut (
  IN       UINT64   Timeout,
  IN OUT   UINT64   StartTick
  )
{
  UINT64 CurrentTick;
  UINT64 DeltaTick;
  UINT64 ElapsedSec;
  UINT64 Frequency;
  UINT64 StartValue;
  UINT64 EndValue;

  CurrentTick = GetPerformanceCounter ();
  Frequency = GetPerformanceCounterProperties (&StartValue, &EndValue);

  DeltaTick = ((CurrentTick > StartTick)?
                  (CurrentTick - StartTick) :
                  (EndValue - StartValue) - StartTick + CurrentTick);

  //
  //                 DeltaTicks
  // Elapsed Time = ------------
  //                 Frequency
  //
  ElapsedSec  = DivU64x32Remainder (DeltaTick, (UINT32)Frequency, NULL);
  if (ElapsedSec < Timeout) {
    return FALSE;
  }
  //CpuDeadLoop();
  return TRUE;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  SMM64 SMI Handler entry for all the cores
 *
 *  @param[in]  ImageHandle  Smm16 builds the SMM_Foundation table & call this entry point
 *  @param[in]  SystemTable  NA. The paramete is needed to satisfy the build time linker
 *
 *  @return NA.The paramete is needed to satisfy the build time linker
 *
**/
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
_ModuleEntryPoint (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT64                  StartTick;
  UINT64                  Data64;
  SMM_FOUNDATION_TABLE    *SmmHandle = ImageHandle;
  SMM_CPU_STATE64         *SaveStatePointer;

  //
  //      S M M   6 4 E N T R Y
  //
  // We are in SMM64 Entry. This entry point is run on every SMM after SMM16 code switch to 64 bit mode.
  // Each core will run this code
  //

  // For very first SMM init global variable
  SmmEarlyInit (SmmHandle);
  gExitSmm = FALSE;

  //DEBUG_SPIN (SmmHandle->SmmSpinLock, (EFI_D_ERROR, "Core [%x] inSmm\n", SmmHandle->CpuNumber));

  // For debug only. Keep track of smm entry count on each core..
  gSmmEntryCount[SmmHandle->CpuNumber] += 1;

  //
  // Below step 1-6 are per BKDG to syncronize CPU entry/exit
  //

  // BKDG Step 1 Aquire gCheckSprinBoard Semaphore
  if (AcquireSpinLockOrFail (&gCheckSpringBoard)) {
    // Call IOHub SMM libraty to check SMM from IOHub
    if (!IsSmmFromIoHub ()) {
      SendSmiIpl (0xff);  // Broadcast SMM to all
      return (EFI_SUCCESS);
    }
  }

  // BKDG Step 2
  InterlockedDecrement ((UINT32 *)&gNotInSmm);

  // Now wait for all cores to decrement gNotInSmm to zero. Wait for timeout
  StartTick = GetPerformanceCounter ();
  while (gNotInSmm) {
    CpuPause ();
    if (IsTimeOut (SMM_WAIT_TIMEOUT, StartTick)) {
      DEBUG_SPIN (SmmHandle->SmmSpinLock, (EFI_D_ERROR, "SMM timeout %x \n", gNotInSmm));
      FailSmmCoreCount = gNotInSmm;
      gNotInSmm = 0;
    }
  }

  // Init counter to indicate how many cores in SMM wait

  // BKDG Step 3 Execute Core local SMM Event
  HandleLocalCoreSmm (SmmHandle);

  // BKDG Step 4 Handle SMM
  if (SmmHandle->CpuNumber == 0) {
    gWaitInSmm = SmmHandle->TotalCores - FailSmmCoreCount;
    BspHandler (SmmHandle);
    gExitSmm = TRUE;
  } else {
    ApHandler (SmmHandle);
  }

  // BKDG Step 5
  InterlockedDecrement ((UINT32 *)&gWaitInSmm);

  // I need to add this WaitInSmmCheck conditional to ensure the sequential decrement, followed by
  // immidiate incerement (step 6) below does not cause any other core to miss see that this variable
  // was turned zero at some point. Without this condition one of the thread remain stuck
  while (gWaitInSmm && WaitInSmmCheck) {
    CpuPause ();
  }
  WaitInSmmCheck = FALSE;

  // BKDG Step 6
  // wait for all cores to sync.back
  InterlockedIncrement ((UINT32 *)&gWaitInSmm);
  while (gWaitInSmm != (SmmHandle->TotalCores - FailSmmCoreCount)) {
    CpuPause ();
  }

  // BKDG Step 7 If BSP reset semaphore
  if (SmmHandle->CpuNumber == 0) {
    ReleaseSpinLock (&gCheckSpringBoard);
    gNotInSmm = SmmHandle->TotalCores;
    WaitInSmmCheck = TRUE;
  } else {
    Data64 = AsmReadMsr64 (MSR_CPU_SMM_BASE);
    SaveStatePointer = (SMM_CPU_STATE64*) Data64;
    SaveStatePointer->SMMFEC9 = 0;
  }

  // For debug only. Keep track of smm exit count on each core..
  gSmmExitCount[SmmHandle->CpuNumber] += 1;

  return (EFI_SUCCESS);
}
