@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CONSOLE.HPJ. >"hlp\Console.hm"
echo. >>"hlp\Console.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Console.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Console.hm"
echo. >>"hlp\Console.hm"
echo // Prompts (IDP_*) >>"hlp\Console.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Console.hm"
echo. >>"hlp\Console.hm"
echo // Resources (IDR_*) >>"hlp\Console.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Console.hm"
echo. >>"hlp\Console.hm"
echo // Dialogs (IDD_*) >>"hlp\Console.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Console.hm"
echo. >>"hlp\Console.hm"
echo // Frame Controls (IDW_*) >>"hlp\Console.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Console.hm"
REM -- Make help for Project CONSOLE


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Console.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Console.hlp" goto :Error
if not exist "hlp\Console.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Console.hlp" Debug
if exist Debug\nul copy "hlp\Console.cnt" Debug
if exist Release\nul copy "hlp\Console.hlp" Release
if exist Release\nul copy "hlp\Console.cnt" Release
echo.
goto :done

:Error
echo hlp\Console.hpj(1) : error: Problem encountered creating help file

:done
echo.
