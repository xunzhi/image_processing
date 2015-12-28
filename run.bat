@echo off
::Running in WINDOWS
::Xunzhi Wang
::wxzpursue@gmail.com

set MAKER="D:\Program Files\CodeBlocks\MinGW\bin\mingw32-make" -f
set INPUT_PATH=.\test_images
set OUTPUT_PATH=.

echo Making by %MAKER%
echo Input path: %INPUT_PATH%
echo Output path: %OUTPUT_PATH%
echo **********

::%MAKER% Makefile

if %1 EQU resize (
echo Resizing
%MAKER% Makefile resize
::the_starry_night.raw origin.jpg
resize %INPUT_PATH%\origin.jpg %OUTPUT_PATH%\resize.jpg 512 512 400 400 
::r test_images\origin.jpg resize.jpg 1 1 400 400
goto :EOF
)
if %1 EQU clean (
%MAKER% Makefile clean
)
if %1 EQU cleanImage (
del *.raw *.jpg
)
echo **********