Local $version_number = "4.0.5"
Local $packaging_directory = "..\..\OpenHoldem_" & $version_number & "\"
local $release_optimized_directory = "release - optimized\"
Local $debug_directory = "debug\"

Func Message($message)
	Local $complete_message = $message & @CRLF & @CRLF & "Then continue."
	MsgBox(0, "Next Step...", $complete_message) 
EndFunc

Func	PrepareReleaseDirectory()
	; Assuming this file lives in a directory like OH_SVN/trunk/

	DirCopy("##_OpenHoldem_Release_Directory_##", $packaging_directory)
#cs
	This directory includes:
	* dbdhelp.dll
	* ReleaseNotes (make sure, they are complete and release date is set)
	* the default scraper directory
	* "sources of code.txt"
	* the license-file
#ce

	; Delete export-files (info for the linker)
	FileDelete($packaging_directory & "*.exp")
	; Delete program-databases (incremental linking and debugging)
	FileDelete($packaging_directory & "*.pdb")
	; Delete log-files
	FileDelete($packaging_directory & "*.log")
	; Remove any replay-directory if existent
	DirRemove($packaging_directory & "replay", 1)
	DirRemove($packaging_directory & "License_MicroSoft_dbghelp_dll", 1)
	DirRemove($packaging_directory & ".svn", 1)
#cs*
* Add the Documentation
* Add all other files
#ce
	SafeCopy($debug_directory & "ManualMode.exe", $packaging_directory & "ManualMode.exe")
	SafeCopy($debug_directory & "OpenScrape.exe", $packaging_directory & "Openscrape.exe")
	SafeCopy($debug_directory & "OHReplay.exe", $packaging_directory & "OHReplay.exe")
	SafeCopy($release_optimized_directory & "OpenHoldem.exe", $packaging_directory & "OpenHoldem.exe")
	SafeCopy($release_optimized_directory & "OpenHoldem.lib", $packaging_directory & "OpenHoldem.lib")
EndFunc

Func SafeCopy($source, $destination)
	Local $success = FileCopy ($source, $destination)
	If (Not $success) Then
		MsgBox(0x30, "ERROR", "Can't copy file: " & $source & @CRLF _
			& @CRLF & "Going to terminate...")
		Exit(-1)
	EndIf
EndFunc

Func CreateRarArchive()

EndFunc

#cs=============================================================================

  Main program

#ce=============================================================================

Message("Please change the OpenHoldem version")
Message("Please choose the correct build option (optimized for OH, debug for the rest")
Message("Please rebuild everything")
Message("Please test ""everything"", at least that OH ""works""")
Message("Please commit everything")
Message("Please tag the release on googlecode" & @CRLF _
  & "TurtoiseSVN -> Branch/Tag" & @CRLF _
  & "and then create a tag for svn/tags/OpenHoldem_4.x.y" & @CRLF _
  & "Comment: ""Tagging OpenHoldem 4.x.y for release""")

PrepareReleaseDirectory()
CreateRarArchive()

Message("Rar-archive has been created on your desktop" & @CRLF _
  &	"Please upload the rar-file to google-code." & @CRLF _
  & "Please dont forget summary and description." & @CRLF _
  & "Labels: Featured, Type-Executable, OpSys-Windows")
Message("Please remove the label ""Featured"" from the last release of OpenHoldem")
Message("Please announce the new download in ""OpenHoldem Stickies"" and post the release-notes.")



