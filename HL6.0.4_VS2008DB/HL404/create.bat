@echo off
set msdevdir=D:\Windows\Microsoft Visual Studio\Common\MSDev98
set VcOsDir=WIN95
if "%OS%" == "Windows_NT" set VcOsDir=WINNT
if "%OS%" == "Windows_NT" set PATH=%MSDevDir%\BIN;%MSVCDir%\BIN;%VSCommonDir%\TOOLS\%VcOsDir%;%VSCommonDir%\TOOLS;%PATH%
if "%OS%" == "" set PATH="%MSDevDir%\BIN";"%MSVCDir%\BIN";"%VSCommonDir%\TOOLS\%VcOsDir%";"%VSCommonDir%\TOOLS";"%windir%\SYSTEM";"%PATH%"

MSDEV .\RCFactory.dsw /make "rcfactory - win32 release" /REBUILD /OUT .\RC.TXT
