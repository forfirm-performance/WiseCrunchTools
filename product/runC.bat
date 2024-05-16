@echo off
REM ###################################################
REM ##      SCRIPT LANCIO GESTIONE CONSTRAINS
REM ##       RUN SCRIPT TO MANAGE CONSTRAINS
REM ###################################################

REM ****************** PLEASE NOTE ***********************
REM Run as runCR.bat <nomefile> 1>ok.txt 2>ko.txt        *
REM In ok.txt we have valid nple (STDOUT) while in       *
REM ko.txt we have NOT valid nple (STDERR) 		         *
REM ******************************************************

REM ****************** NOTA BENE *************************
REM Lanciare come runCR.bat <nomefile> 1>ok.txt 2>ko.txt *
REM In ok.txt ci sono le nple valide (STDOUT) mentre in  *
REM ko.txt ci sono le nple NON valide (STDERR)           *
REM ******************************************************

REM Chiamo il config.bat e leggo la variabile SEP e PROGETTO
REM Call config.bat and read SEP and PROJECT
cls
call config.bat
echo[ 
echo[
echo %PROJECT%
echo[ 
echo[
REM Imposto i parametri in input
REM Set input parameters
set err=0
IF [%1]==[] set err=1
IF [%1]==[/?] set err=1
IF [%err%]==[1] (goto HELP) ELSE (goto CONTINUA)
:HELP
	echo[ 
	echo[
	echo Usage
	echo    runCR {FILE}
	echo[
	echo FILE      Valid input file name (nple or Test Set file name)
	echo[ 
	echo [/?]      This help
	echo[ 
	GOTO:EOF

:CONTINUA
set FILE=%1

REM Set datapath on BATCH CURRENT DIR =%~dp0 and not on execution dir %CD%
REM Imposto il datapath sulla DIRECTORY DELLO SCRIPT CORRENTE =%~dp0 e NON su quella di esecuzione del batch %CD% 
REM Look at/Vedasi http://weblogs.asp.net/whaggard/archive/2005/01/28/get-directory-path-of-an-executing-batch-file.aspx

set datapath_withslash=%~dp0
REM If directory with trailing / ... -I perl option seems not work. Remove trailing /
REM Se la directory ha lo / finale l'opzione -I del perl sembra non funzionare. Si rimuove lo / finale
REM Look t/Vedasi http://stackoverflow.com/questions/2952401/remove-trailing-slash-from-batch-file-input
IF %datapath_withslash:~-1%==\ SET datapath_noslash=%datapath_withslash:~0,-1%

REM Run constrains script
REM Lancio lo script perl di esecuzione delle constrains
perl -I"%datapath_noslash%" "%datapath_withslash%bin\runConstrains.pl" %FILE% %SEP%


