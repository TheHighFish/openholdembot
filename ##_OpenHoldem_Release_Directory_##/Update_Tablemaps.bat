@echo.
@echo This software is part of the Openholdem project
@echo Download page:         https://github.com/OpenHoldem/openholdembot/releases
@echo Forums:                http://www.maxinmontreal.com/forums/index.php
@echo Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
@echo.
@echo This tool will download the "offical" tablemaps 
@echo from the OpenHoldem-repository at GitHub.
@echo It will also download one replay-frame per map
@echo for verification / as setup-instruction.
@echo.
@REM testing if SVN.exe exists
@REM https://stackoverflow.com/questions/4002819/finding-the-path-of-the-program-that-will-execute-from-the-command-line-in-windo
@where /Q SVN.exe
@REM Checking the error-level of the last command
@REM http://steve-jansen.github.io/guides/windows-batch-scripting/part-3-return-codes.html
@IF %ERRORLEVEL% NEQ 0 (
   @echo Required software: TortoiseSVN
   @echo https://tortoisesvn.net/downloads.html
   @echo.
   @pause
   @exit
)
@REM Using the %ProgramFiles% variable
@REM https://stackoverflow.com/questions/19652678/how-to-mention-c-program-files-in-batchfile
@REM Getting a remote directory from GitHub with SVN
@REM https://stackoverflow.com/questions/7106012/download-a-single-folder-or-directory-from-a-github-repo
"%ProgramFiles%\TortoiseSVN\bin\svn.exe" --force export "https://github.com/OpenHoldem/openholdembot/trunk/##_OpenHoldem_Release_Directory_##/scraper"
@echo.
@pause