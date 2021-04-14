@echo Building RebuyDemo.exe
@echo.
"c:\Program Files\AutoIT3\Aut2Exe\Aut2Exe.exe" /IN RebuyDemo.au3
move RebuyDemo.exe ..\##_OpenHoldem_Release_Directory_##\tools
@echo.
@pause