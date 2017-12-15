@echo .
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
@echo Required software: TortoiseSVN
@echo https://tortoisesvn.net/downloads.html
@echo .
@pause
@REM https://stackoverflow.com/questions/7106012/download-a-single-folder-or-directory-from-a-github-repo
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export "https://github.com/OpenHoldem/openholdembot/trunk/##_OpenHoldem_Release_Directory_##/scraper"
@echo .
@pause