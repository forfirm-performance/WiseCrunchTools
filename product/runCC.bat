@echo off
REM ###################################################
REM ##      SCRIPT LANCIO CALCOLO COPERTURA
REM ## RUN SCRIPT TO CALCULATE TEST SET COVERAGE
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
REM Imput to the batch: test set file name, wise number and 5 options
set err=0
IF [%1]==[]   set err=1
IF [%1]==[/?] set err=1
IF [%2]==[]  set err=1
IF [%err%]==[1] (goto HELP) ELSE (goto CONTINUA)
:HELP
	echo[ 
	echo[
	echo Usage
	echo    runCC {FILE TESTSET} {WISE NUMBER} {OPTIONS}
	echo[
	echo FILE TESTSET     Test Set valid input file (the 'clean' file)	
	echo[
	echo WISE NUMBER      Is an interger number between 1 and the number of variables 
	echo                  (row=MAX_WISE) of input file.
	echo[
	echo OPTION           One or more of the following:
	echo                        -n     Print out matched nple
	echo                        -d     Print out details (#TestSet, #Wise)
	echo                        -f     Print out Frequency Map
	echo                        -r     Print out Statistic
	echo                        -z     Print out TestSet record with number
    echo                               of matched nple
	echo[ 
	echo [/?]             This help
	echo[ 
	GOTO:EOF

:CONTINUA
set FILE_TESTSET=%1
set WISE=%2
set OPT_01=%3
set OPT_02=%4
set OPT_03=%5
set OPT_04=%6
set OPT_05=%7


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
IF EXIST "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" (
	calcolaCopertura.exe  -w: "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt"  -t: "%FILE_TESTSET%" -o: %WISE% -s: %SEP% %OPT_01% %OPT_02% %OPT_03% %OPT_04% %OPT_05%
) else (
    echo[ 
	echo[
	echo Nple file "%APP_PATH%%PREFIX_FILE_NAME_OUTPUT_PRODCART%_%WISE%.txt" does not exist. Run runW %WISE% to create!
	echo[ 
	echo[ 
)

REM Ritorno alla directory di partenza
REM Come back home !
cd %OLDCD%

















