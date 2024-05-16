@echo off
REM ##############################################################
REM ##   CONFIGURAZIONE SCRIPT LANCIO CALCOLO WISE (CREA NPLE)
REM ##   CONFIG SCRIPT TO RUN WISE ELABORATIONE (CREATE NPLE)
REM ##
REM ## APP_PATH = Working Directory 
REM ## INPUT_FILE_NAME = Name of the file with variable and values' 
REM ##                   that *must be* present in APP_PATH
REM ##                   APP_PATH *must* end with  \
REM ##############################################################

REM Nome del progetto in corso
REM Project's name
set "PROJECT=###### PROVE 4 VARIABILI 2 VALORI ######"

REM Working directory (must end with \)
REM Directory di lavoro (deve terminare con \)
REM set "APP_PATH=C:\Users\dberta\Documents\Lavoro\Ferrero\Test\Fatturazione\Test\ReverseData\"
REM set "APP_PATH=C:\Users\dberta\Documents\Lavoro\Ferrero\Test\Consegnabile\Test Runs\NEW_WISE_ORDINI\ReversePairWise\"
set "APP_PATH=C:\Users\dberta\temp\quattro\"
rem set "APP_PATH=C:\Users\dberta\elisabetta\"

REM Input file (must be present in the working directory)
REM File di input (deve essere presente nella directory di lavoro)
REM set "INPUT_FILE_NAME=fatture.txt"
REM set "INPUT_FILE_NAME=ordini.txt"
set "INPUT_FILE_NAME=test_4.txt"
REM set "INPUT_FILE_NAME=fatture.txt"

REM Separator **MUST BE EQUAL** to the separator used in the input file to separate fields
REM Il separatore **DEVE** essere uguale al separatore utilizzato sul file di input INPUT_FILE_NAME per separare i campi
set "SEP=;"