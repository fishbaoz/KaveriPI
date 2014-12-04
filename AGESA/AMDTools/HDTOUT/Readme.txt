------------------------------------------------------------------------
HDTOUT v2.2.5
Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
------------------------------------------------------------------------

Changes in 2.3.0
  - Reserve three filter flags for platform in case it needs to use HDTOUT to log platform data.

Changes in 2.2.5
  - Add TEST_POINT filter

Changes in 2.2.4
  - Improve bit field parsing to show field names during MEM_SETREG for KV.

Changes in 2.2.3
  - Add MEM_UNDEF_BF filter to show undefined memory bit field accesses.

Changes in 2.2.2
  - Fix crash on OR-B0

Changes in 2.2.0
  - Add LIB_PCI_RD and LIB_PCI_WR filter

Changes in 2.1.0
  - Fix script to work with HDT 8.0.336 due to new OR workaround with Exit PDM
  - Add check to avoid enabling-at-reset when BIOS has enabled on its own.
  - Remove HDT-on-at-reset when script die or interrupted.

Changes in 2.0.6
  - Fix enable-at-reset feature works with Wombat
  - Add $exit_dbg_after_reset control flag since some chipset requires exit_dbg after writing to CF9

Changes in 2.0.5
  - Add 'HDTOUT Spec.pdf'
  - Readme.txt will only contains release log
  - Due to the hardware dependency of the HDTOUT scripts, AGESA will only
    maintain hdtout2008.pl and will deprecate the other scripts. They will
    be moved into "Samples" directory for customers to reference while
    writing their own scripts. Below is the list of scripts that are deprecated:
      +  EnableHdtoutAtReset.pl
      +  FuncList.pm
      +  MemRdWr.pl
      +  setbp2008.pl
      +  StringBP.pm
  - No changes in scripts

Changes in 2.0.4
  - Fix enable-at-reset issue on SB880
  - Add Reset support: users can choose where to cold/warm reset by using HdtoutSelectionTable.
  - Fix STDIN bugs
  - Fix SPD dump bug

Changes in 2.0.2
  - Add "N" Enable without reset
  - Add Debug S3 resume tip to help

Changes in 2.0.1
  - Add support for parse ASSERT Message
    - Add wp comand to specify the working path, allow parse assert function
    get more information
  - Fix "Ctrl + C" doesn't work Properly in Simnow

Changes in 2.0.0
  - Update command line interface, use hdtout2008.pl -h for detail information
     - Configure output logfile
     - Configure possum id
     - Set print buffer size
     - Configure YAAP property
     - Configure SPD output
     - [new] Add filter control, user can set the filter of output at runtime

Changes in 1.4.0
  - Add option to show PCI register read/write (requires new BIOS)
  - Add option to read SPD data (requires new BIOS)
  - Move default output directory from C: to C:\HDTOUT to support Windows 7
  - Add YAAP support
  - Move control flags to top of hdtout2008.pl for easy modification
  - Add bsp_only flag so script does not need to scan through all the cores
  - Add usage
  - Fix bug when using hdtout string to break in


