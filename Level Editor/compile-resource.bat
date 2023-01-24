@echo off
set /p file="Please Select a file: "
C:\raylib\w64devkit\bin\windres.exe -i %file% -O coff -o resource.coff
pause