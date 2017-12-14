@echo This software is part of the Openholdem project
@echo Download page:         https://github.com/OpenHoldem/openholdembot/releases
@echo Forums:                http://www.maxinmontreal.com/forums/index.php
@echo Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
@echo .
@echo This tool will download the "offical" tablemaps 
@echo from the OpenHoldem-repository at GitHub.
@echo It will also download one replay-frame per map
@echo for verification / as setup-instruction.
@echo .
@echo Required software: wget
@echo http://gnuwin32.sourceforge.net/packages/wget.htm
@echo .
@pause
@REM Getting a remote directory with wget
@REM https://stackoverflow.com/questions/273743/using-wget-to-recursively-fetch-a
@REM -directory-with-arbitrary-files-in-it
@REM
@REM https://www.linux.com/blog/wget-and-downloading-entire-remote-directory%20
rem "C:\Program Files\GnuWin32\bin\wget" -r --level=0 -x -erobots=off -np -R "index.html*" --no-check-certificate "https://github.com/OpenHoldem/openholdembot/tree/master/##_OpenHoldem_Release_Directory_##/scraper"

rem "C:\Program Files\GnuWin32\bin\wget" -r --level=0 -x -erobots=off -np -R "index.html*" --no-check-certificate "https://github.com/OpenHoldem/openholdembot/tree/master/Deprecated

@rem !!! "C:\Program Files\GnuWin32\bin\wget" -r --level=0 -x -erobots=off -np -R "index.html*" --no-check-certificate "www.maxinmontreal.com"

@REM "C:\Program Files\GnuWin32\bin\wget" --no-check-certificate https://raw.githubusercontent.com/OpenHoldem/openholdembot/master/prepare_release_semi_automatically.au3

@REM https://stackoverflow.com/questions/7106012/download-a-single-folder-or-directory-from-a-github-repo
"C:\Program Files\TortoiseSVN\bin\svn.exe" export "https://github.com/OpenHoldem/openholdembot/trunk/##_OpenHoldem_Release_Directory_##/scraper"
@echo .
@pause