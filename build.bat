@echo off

cd build

:A
cmake --build .
pause
cls
goto A
