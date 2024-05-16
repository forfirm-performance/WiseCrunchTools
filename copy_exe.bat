echo off
REM Cancellazione degli eseguibili da bin
cd product/bin
echo Cancellazione file .exe in bin
pause
del *.exe
cd ../..

REM  Copia dei file eseguibili
echo Copia file exe da Release
pause
copy source\calcolaCopertura\bin\Release\*.exe product\bin\*.*
copy source\calcolaCoperturaSlow\bin\Release\*.exe product\bin\*.*
copy source\Combinazioni_n_k\bin\Release\*.exe product\bin\*.*
copy source\contrLimWise\bin\Release\*.exe product\bin\*.*
copy source\count_line\bin\Release\*.exe product\bin\*.*
copy source\creaFileProdCart\bin\Release\*.exe product\bin\*.*
copy source\creaStringa\bin\Release\*.exe product\bin\*.*
copy source\generaTestSet\bin\Release\*.exe product\bin\*.*
copy source\generaTestSetSlow\bin\Release\*.exe product\bin\*.*
copy source\ProdCart\bin\Release\*.exe product\bin\*.*
copy source\reduceNple\bin\Release\*.exe product\bin\*.*
copy source\uniqueRowFile\bin\Release\*.exe product\bin\*.*