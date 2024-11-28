@echo off

cd build

:A
cls
cmake --build .
pause
goto A
