@CLS
@Echo This batch-file will try to run the OpenPPL-testsuite,
@Echo using "release\OpenPPL.Exe" and the "ok_*.txt"-files
@Echo in "OpenPPL/TestSuite".
@Echo It won't try to translate tehe "error_*.txt"-files,
@Echo so you shouldn't get any error-messages.
@ECHO.
@Pause

@IF NOT EXIST Release\OpenPPL.Exe @ECHO ERROR: Release\OpenPPL.Exe does not exist
@IF NOT EXIST Release\OpenPPL.Exe Pause 

@COPY OpenPPL_Library\OpenPPL_Library.ohf .
@COPY ..\mh_str_Handstrength_Library\mh_str_Handstrength_Library.ohf .

FOR %%f IN (OpenPPL\TestSuite\ok_*.txt) DO Release\OpenPPL.Exe %%f > Temp.OHF

@DEL OpenPPL_Library.ohf
@DEL mh_str_Handstrength_Library.ohf
REM DEL Temp.OHF
