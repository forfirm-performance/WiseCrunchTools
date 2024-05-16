@echo off
REM ###################################################
REM ##      SCRIPT LANCIO GENERA TEST SET
REM ##     RUN SCRIPT TO GENERATE TEST SET
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

REM Definizione delle variabili di input al file: test set file, numero wise, 5 optioni
REM Input to the batch: test set file name, wise number and 5 options
set err=0
IF [%1]==[]   set err=1
IF [%1]==[/?] set err=1
IF [%2]==[]   set err=1
IF [%3]==[]   set err=1
IF [%err%]==[1] (goto HELP) ELSE (goto CONTINUA)
:HELP
	echo[ 
	echo[
	echo Usage
	echo    runTSF {FILE TESTSET BASE} {FILE TESTSET PARZIALE} {WISE NUMBER}
	ECHO[
	echo FILE TESTSET BASE     Test Set valid input file (the 'clean' file) 
	echo                       used to pick up test case.
	echo[
	echo FILE TESTSET PARZIALE Test Set with valid test case, to be excluded
	echo                       from the generation; the delta only will be 
	echo                       created.
	echo[
	echo WISE NUMBER           Is an interger number between 1 and the number
    echo                       of variables (row=MAX_WISE) of input file.
	echo[ 
	echo [/?]                  This help
	echo[ 
	GOTO:EOF

:CONTINUA
set FILE_TESTSET_BASE=%1
set FILE_TESTSET_PARZIALE=%2
set WISE=%3

REM Salvo la directory corrente
REM Save current directory
set OLDCD=%CD%

REM File di input (variabili e relativi valori delle variabili) letti da config.bat
REM Input file (path+name) from config.bat
set FILE_INPUT=%APP_PATH%%INPUT_FILE_NAME% 

REM Prefisso del file di output di ProdCart.exe
REM Output file prefix for ProdCart.exe
set PREFIX_FILE_NAME_OUTPUT_PRODCART=out_p


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

REM Se il file delle nple del wise non esiste, mando il messaggio di errore
REM If nple wise file does not exist, send error 
IF NOT EXIST "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" (
    echo[ 
	echo[
	echo Nple file  "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" does not exist! Run runW %WISE% to create !
	echo[ 
	echo[ 
	GOTO:EOF
)
	
REM Se il file TestSet Base, da cui vengono estratti i test, non esiste, mando il messaggio di errore
REM If Test Set Base file does not exist, send error 
IF NOT EXIST "%FILE_TESTSET_BASE%" (
    echo[ 
	echo[
	echo File "%FILE_TESTSET_BASE%" does not exist!
	echo[ 
	echo[ 
	GOTO:EOF
)

REM Se il file TestSet Parziale non esiste, contenente i test con nple già coperte, mando il messaggio di errore
REM If Test Set Parziale file does not exist, send error 
IF NOT EXIST "%FILE_TESTSET_PARZIALE%" (
    echo[ 
	echo[
	echo File "%FILE_TESTSET_PARZIALE%" does not exist!
	echo[ 
	echo[ 
	GOTO:EOF
)

	REM Creo il file delle nple vietate con calcolaCopertura.exe sul FILE_TESTSET_PARZIALE
	REM Generate file with already considered nple with calcolaCopertura.exe on FILE_TESTSET_PARZIALE
	erase "%APP_PATH%vietate_%WISE%.txt" 1>NUL 2>NUL
	calcolaCopertura.exe -w: "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt"  -t: "%FILE_TESTSET_PARZIALE%" -o: %WISE% -s: %SEP% -n > "%APP_PATH%vietate_%WISE%.txt"
	
	REM Butto giu' il task se esiste
	REM Kill the task if exixt
	taskkill /F /FI "IMAGENAME eq generaTestSet.exe" 1>NUL 2>NUL
	erase delta_test_set_generato_%WISE%.txt clean_delta_test_set_generato_%WISE%.txt 1>NUL 2>NUL
	start /min /b generaTestSet.exe -w: "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" -t: "%FILE_TESTSET_BASE%" -o: %WISE% -s: %SEP% -f: "%APP_PATH%vietate_%WISE%.txt"   1>delta_test_set_generato_%WISE%.txt 2>delta_clean_test_set_generato_%WISE%.txt
	echo[ 
	echo[
	echo Will be created file 'test_set_generato_%WISE%.txt' and file 'clean_test_set_generato_%WISE%.txt' in the current directory when process 'generaTestSet.exe', visible from TaskManager, will finish.	
	echo[ 
	echo[
	goto LOOP

:LOOP	
for /F "delims=*" %%p in ('tasklist /FI "IMAGENAME eq generaTestSet.exe" ^| findstr "generaTestSet.exe" ') do (
	echo[
	echo[
	tasklist /FI "IMAGENAME eq generaTestSet.exe
	echo[
	echo[
	echo *PLEASE NOTE*: Using CTRL+C, the batch terminate but the process 'generaTestSet.exe' will not be killed and keep running
	echo *NOTA BENE* : Uscendo con CTRL+C, il batch termina ma il processo 'generaTestSet.exe', se attivo, continua a essere eseguito. 
	echo[
	echo To kill the process use the command:
	echo Per eliminare il processo usare il comando: 
	echo     taskkill /F /FI "IMAGENAME eq generaTestSet.exe"
	echo[
	echo or close the process 'generaTestSet.exe' from Task Manager
	echo oppure chiudere il processo 'generaTestSet.exe' da Task Manager 
	echo[
	echo[
	timeout /t 30 /nobreak
	goto LOOP
)

echo[
echo[
erase "%APP_PATH%vietate_%WISE%.txt"  1>NUL 2>NUL
cd "%OLDCD%";
echo #################################################
echo ###       TEST SET GENERATION FINISHED !      ###
echo ### LA GENERAZIONE DEL TESTSET E' TERMINATA ! ###
echo #################################################
echo[
echo[
dir "delta_clean_test_set_generato_%WISE%.txt" 
dir "delta_test_set_generato_%WISE%.txt"





















