@echo off
REM ###################################################
REM ##  SCRIPT LANCIO CALCOLO WISE (CREATORE NPLE)
REM ##      RUN WISE SCRIPT (NPLE CREATOR)
REM ###################################################

REM Chiamo il config.bat e legge le variabili APP_PATH e INPUT_FILE_NAME
REM Call config.bat and read APP_PATH and INPUT_FILE_NAME
cls
call config.bat
echo[ 
echo[
echo %PROJECT%
echo[ 
echo[

REM Controllo che il path APP_PATH ed il file INPUT_FILE_NAME di input esistano; altrimenti esco
REM Verify APP_PATH and file INPUT_FILE_NAME exist
IF NOT EXIST "%APP_PATH%" (
    echo[ 
	echo[
	echo Directory "%APP_PATH%" does not exist. Insert a valid path !
	echo[ 
	echo[
	exit /B
)
IF NOT EXIST "%APP_PATH%%INPUT_FILE_NAME%" (
	echo[ 
	echo[
	echo File "%APP_PATH%%INPUT_FILE_NAME%" does not exist. Insert a valid input file!
	echo[ 
	echo[
	exit /B
)

REM Definizione delle variabili di input al file
REM Imput to the batch: wise number
set err=0
IF [%1]==[] set err=1
IF [%1]==[/?] set err=1
IF [%err%]==[1] (goto HELP) ELSE (goto CONTINUA)
:HELP
		echo[ 
		echo[
		echo Usage:
		echo    runW {WISE}
		echo[		
		echo WISE     Interger number between 1 and the number of variables (row) 
		echo          of input file.
		echo[ 
		echo [/?]     This help
		echo[ 
		GOTO:EOF


:CONTINUA
set WISE=%1

REM File di input (variabili e relativi valori delle variabili) letti da config.bat
REM Input file (path+name) from config.bat
set FILE_INPUT=%APP_PATH%%INPUT_FILE_NAME% 

REM Leggo in numero di righe del file di input ed imposto il WISE_MAX
REM Read num row from input file and set WISE_MAX
	erase "%APP_PATH%temp_c_l.txt" 1>NUL 2>NUL
	count_line.exe "%FILE_INPUT%" > "%APP_PATH%temp_c_l.txt"
	REM Eseguo la lettura del comando type (che stampa il contenuto del file) e lo imposto nella variabile 'stringa'
	REM Read with type command the file APP_PATH/temp_c_l.txt content and set the variable 'stringa'
	FOR /F "tokens=*" %%i IN ('type "%APP_PATH%temp_c_l.txt"') DO (set WISE_MAX=%%i)
	
REM Controllo che il numero del WISE non sia minore di zero o maggiore di WISE_MAX (usando l'eseguibile contrLimWise per evitare quirk del DOS)
REM Verify that WISE>0 and WISE <= WISE_MAX (using a C++ exe contrLimWise, to avoid DOS trick)
	erase "%APP_PATH%temp_c_l.txt" 1>NUL 2>NUL
	contrLimWise.exe %WISE% %WISE_MAX% > "%APP_PATH%temp_c_l.txt"
	REM Eseguo la lettura del comando type (che stampa il contenuto del file) e lo imposto nella variabile 'ret'
	REM Read with type command the file APP_PATH/temp_c_l.txt content and set the variable 'ret'
	set ret=ko
	FOR /F "tokens=*" %%i IN ('type "%APP_PATH%temp_c_l.txt"') DO (set ret=%%i)
	IF %ret%==ko (
		echo[ 
		echo[
		echo WISE = %WISE% value is not valid. It must be an integer number between 1 and %WISE_MAX%!
		echo[ 
		echo[ 
		GOTO:EOF
	)
	erase "%APP_PATH%temp_c_l.txt" 1>NUL 2>NUL

REM Definisco il file di errore
REM Set error file
set ERROR_FILE=%APP_PATH%%err.txt 
erase "%ERROR_FILE%" 1>NUL 2>NUL

REM Salvo la directory corrente
REM Save current DIR
set OLDCD=%CD%

REM Prefisso del file di output delle combinazioni e relativo path
REM Combination output file prefix and path 
set JUST_PREFIX_FILE_OUTPUT_COMBINAZIONI=out_c
set FILE_OUTPUT_COMBINAZIONI=%APP_PATH%%JUST_PREFIX_FILE_OUTPUT_COMBINAZIONI%.txt

REM Directory di creazione e nome dei file di input per ProdCart.exe (output di creaFileProdcart.exe)
REM Print out dir and input file name for ProdCart.exe (output for creaFileProdcart.exe)
set PATH_OUTPUT_CREAFILEPRODCART=%APP_PATH%IN_PRODCART\
set JUST_PREFIX_CREAFILEPRODCART=in_p

REM Prefisso del file di output di ProdCart.exe
REM Output file prefix for ProdCart.exe
set PREFIX_FILE_NAME_OUTPUT_PRODCART=out_p

REM Creazione della directory PATH_OUTPUT_CREAFILEPRODCART se non esiste
REM Makedir for PATH_OUTPUT_CREAFILEPRODCART, if does not exist
IF NOT EXIST "%PATH_OUTPUT_CREAFILEPRODCART%" mkdir "%PATH_OUTPUT_CREAFILEPRODCART%" 1>NUL 2>>"%ERROR_FILE%"

REM ###################################################
REM Lancio il file delle combinazioni:
REM Input: Stringa e valore di k
REM Output: File di output con tutte le combinazioni
REM         dei valori delle variabili k a k
REM
REM Run combination calculus:
REM Input: String and k value
REM Output: Output file with all combinations
REM         of values' variable k at k
REM #####################################################
	IF  EXIST "%FILE_OUTPUT_COMBINAZIONI%"  erase "%FILE_OUTPUT_COMBINAZIONI%" 1>NUL 2>>"%ERROR_FILE%"
	creaStringa.exe -i: "%FILE_INPUT%" > "%APP_PATH%temp.txt"
	REM Eseguo la lettura del comando type (che stampa il contenuto del file) e lo imposto nella variabile 'stringa'
	REM Read with type command the file APP_PATH/temp_c_l.txt content and set the variable 'stringa'
	FOR /F "tokens=*" %%i IN ('type "%APP_PATH%temp.txt"') DO (set stringa=%%i)
    Combinazioni_n_k.exe -s: %stringa% -k: %WISE% >  "%FILE_OUTPUT_COMBINAZIONI%"
	IF  EXIST "%APP_PATH%temp.txt" erase "%APP_PATH%temp.txt" 1>NUL 2>>NUL

REM ########################################################
REM Lancio il file di creazione dei files per ProdCart.exe
REM Input: path+nome del file delle variabili, path+nome 
REM        del file di input delle combinazioni, 
REM        path+prefisso dei file di output da generare
REM Output: File di output (piu' di uno)
REM
REM Run file creation for ProdCart.exe
REM Input: variables path+file name, combinations path+file
REM        path+prefix file output to generate
REM Output: File with nple (more than one)
REM #########################################################

    IF EXIST "%PATH_OUTPUT_CREAFILEPRODCART%%JUST_PREFIX_CREAFILEPRODCART%*.txt" erase "%PATH_OUTPUT_CREAFILEPRODCART%%JUST_PREFIX_CREAFILEPRODCART%*.txt" 1>NUL 2>>"%ERROR_FILE%"
	creaFileProdcart.exe -i: "%FILE_INPUT%" -c: "%FILE_OUTPUT_COMBINAZIONI%" -p: "%PATH_OUTPUT_CREAFILEPRODCART%%JUST_PREFIX_CREAFILEPRODCART%_%WISE%_"

REM ########################################################
REM Lancio ProdCart su tutti i file generati
REM Input: Nome del file con valori delle variabili
REM Output: File di output

REM Run cartesian product on all file previously generated
REM Input: File name with variables values
REM Output: Output file with all the nple
REM ########################################################

   cd %PATH_OUTPUT_CREAFILEPRODCART%
   IF EXIST "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" erase "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" 1>NUL 2>>"%ERROR_FILE%"
   for %%i in (%JUST_PREFIX_CREAFILEPRODCART%_%WISE%_*.txt) DO (
		ProdCart.exe -i: %%i -s: %SEP%  >>   %PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt
   )
   IF EXIST "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt"  erase "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" 1>NUL 2>>"%ERROR_FILE%"
   IF EXIST "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" copy  "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" "%APP_PATH%" /Y 1>NUL 2>>"%ERROR_FILE%"
   IF EXIST "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" erase "%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" 1>NUL 2>NUL
   
REM Modifico il nome del file delle combinazioni con il numero WISE
REM Change combination file name adding _WISE number   
   cd       "%APP_PATH%"
   IF EXIST "%JUST_PREFIX_FILE_OUTPUT_COMBINAZIONI%_%WISE%.txt" erase  "%JUST_PREFIX_FILE_OUTPUT_COMBINAZIONI%_%WISE%.txt" 1>NUL 2>>"%ERROR_FILE%"
   IF EXIST "%FILE_OUTPUT_COMBINAZIONI%" rename "%FILE_OUTPUT_COMBINAZIONI%" "%JUST_PREFIX_FILE_OUTPUT_COMBINAZIONI%_%WISE%.txt"
   IF EXIST  "%FILE_OUTPUT_COMBINAZIONI%" erase  "%FILE_OUTPUT_COMBINAZIONI%" 1>NUL 2>>"%ERROR_FILE%"
   
REM Ritorno alla directory di partenza
REM Come back home !
	cd "%OLDCD%" 
	

   
   