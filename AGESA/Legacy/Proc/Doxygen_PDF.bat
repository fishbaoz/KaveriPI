@echo off
goto Begin
:Documentation
echo off
echo **************************************************************************
echo * Doxygen_PDF.bat
echo *     This batchfile uses Doxygen / LaTeX to generate a PDF document
echo *     based on the AGESA(TM) source code.  It requires Doxygen and a
echo *     LaTeX installation to be installed and in your PATH variable.
echo *     It has been tested with Doxygen 1.7.1 and MiKTeX 2.9.
echo *
echo * Syntax:
echo *           Doxygen_PDF.bat
echo *
echo * Notes:
echo *
echo *   If the $(AGESA_ROOT) environment variable is set, documentation
echo *   will be generated based on that code within that directory.
echo *
echo *   If $(AGESA_ROOT) is not set, documentation will be generated
echo *   from code in directories relative to the current path. In this
echo *   case it is assumed that this batch file is run from the Legacy\Proc
echo *   directory.
echo *
echo *   The resultant PDF file will be located in the \Proc subdirectory of
echo *   AGESA(TM).
echo *
echo **************************************************************************
goto Exit

:Begin

echo Executing script from %CD%

IF DEFINED AGESA_ROOT (PUSHD %AGESA_ROOT%\Proc) ELSE (PUSHD ..\..\Proc)

echo ----
echo Running Doxygen on Doxyfile-proc in %CD%

doxygen Doxyfile-proc 1>doxygen.out 2>&1

echo ----
echo Doxygen is complete.  Checking if output was successfully generated.

IF NOT EXIST "latex" GOTO doxyfail
GOTO doxypass

:doxyfail
echo ----
echo LaTeX output not generated.  Cannot generate PDF.
echo Please see %CD%\doxygen.out for details.
POPD
GOTO Exit

:doxypass
PUSHD "latex"
echo ----
echo Processing files in %CD%

del /s /f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl refman.pdf 1>nul 2>&1
echo r > r.txt

echo ----
echo Running pdflatex
pdflatex refman -interaction=nonstopmode -extra-mem-top=40000000 -pool-size=6200000 -enable-installer <r.txt 1>>latex.out 2>&1
echo ----
echo Running makeindex
makeindex refman.idx 1>>latex.out 2>&1
echo ----
echo Re-running pdflatex to update references
pdflatex refman -interaction=nonstopmode -extra-mem-top=40000000 -pool-size=6200000 -enable-installer <r.txt 1>>latex.out 2>&1


setlocal enabledelayedexpansion
set count=5
:repeat
set content=X
for /F "tokens=*" %%T in ( 'findstr /C:"Rerun LaTeX" refman.log' ) do set content="%%~T"
if !content! == X for /F "tokens=*" %%T in ( 'findstr /C:"Rerun to get cross-references right" refman.log' ) do set content="%%~T"
if !content! == X goto :skip
set /a count-=1
if !count! EQU 0 goto :skip

echo ----
echo Re-running pdflatex to update references
pdflatex refman -interaction=nonstopmode -extra-mem-top=40000000 -pool-size=6200000 -enable-installer <r.txt 1>>latex.out 2>&1
goto :repeat
:skip
endlocal

IF NOT EXIST "refman.pdf" GOTO latexfail
GOTO latexpass

:latexfail
POPD
echo ----
echo Script failed! Arch2008.pdf not generated.
GOTO cleanup

:latexpass
copy refman.pdf ..\Arch2008.pdf 1>nul 2>&1
POPD
echo ----
echo Script complete! Arch2008.pdf is available in %CD%
del /q /f latex 1>nul 2>&1
rmdir latex 1>nul 2>&1
del /q /f doxygen.out 1>nul 2>&1

:cleanup

POPD
echo ----

:Exit