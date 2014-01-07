@echo off
set MYDIR=C:\Users\Anthon\Desktop\vs projects\Game Assignment\
for /F %%x in ('dir /B/D %MYDIR%') do (
  set FILENAME=%MYDIR%\%%x\log\IL_ERROR.log
  echo ===========================  Search in !FILENAME! ===========================
  c:\utils\grep motiv !FILENAME!
)

PAUSE