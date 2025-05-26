@echo off
setlocal

pushd %~dp0
del /q .\build\*
mkdir .\build
pushd .\build

set OUTPUTEXE=out.exe
set OUTPUTPDB=out.pdb
set INSTALLDIR=..\bin

@REM set SRC="../src/creature.cc" "../src/main.cc" "../src/player.cc" "../src/skill.cc"
set SRC="../src/map.cc" "../src/script.cc" "../src/util.cc"
cl -W3 -WX -Zi -we4244 -we4456 -we4457 -wd4996 -std:c++14 -EHsc -utf-8 -MP8 -DBUILD_DEBUG=1 -DARCH_X64=1 -D_CRT_SECURE_NO_WARNINGS=1 -Fe:"%OUTPUTEXE%" -Fd:"%OUTPUTPDB%" %* %SRC% /link -subsystem:console -incremental:no -opt:ref -dynamicbase "ws2_32.lib"

if errorlevel 1 exit /b errorlevel

mkdir "%INSTALLDIR%"
copy /Y "%OUTPUTEXE%" "%INSTALLDIR%"
copy /Y "%OUTPUTPDB%" "%INSTALLDIR%"

popd
popd

endlocal
