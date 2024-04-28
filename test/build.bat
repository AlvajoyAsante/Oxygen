:: Alvajoy "Alvajoy123" Asante
:: Last updated: 9/5/2022
:: This compiles the API into a folder and uses it to detect bugs of any sort.

@echo off

title Testing: Oxygen
:start

:: Deletes the old version so it does not conflit with the new version
echo Deleting the old version of Oxygen from testing folder...
rmdir /S/Q ".\src\oxygen"

echo Cleaning folder directories for compiling ...
rmdir /S/Q ".\bin"
rmdir /S/Q ".\obj"

:: Clears the screen of any infomations displayed whiles deleting the folder.
cls

:: Makes a folder named notify and copies all the source code into the hyra folder
echo Copying the lastest version of Oxygen into testing folder...
mkdir .\src\oxygen
xcopy /y/s/i "..\oxygen" .\src\oxygen

:: Clears the screen of any infomations displayed whiles create folder.
cls

:: Compiles all the program in order to test for errors and bugs
make
PAUSE 

:: Once the bug has been fixed then cleans via dev then the screen is cleared. 
cls
goto start