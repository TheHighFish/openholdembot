#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.12.0
 Author:         TheHighFish

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

Local $pre_created_release_dir = "##_OpenHoldem_Release_Directory_##"
Local $new_openholdem_dir      = "OpenHoldem_7.x.y"
Local $binary_dir              = "Release"
Local $binary_optimized_dir    = "Release_Optimized"
Local $openppl_dir             = "OpenPPL"
Local $openppl_library_dir     = $openppl_dir & "\OpenPPL_Library"
Local $openppl_manual_dir      = $openppl_dir & "\OpenPPL_Manual"
Local $pokertracker_docu_dir   = "Documentation\Chapters\symbols"
Local $release_notes           = $pre_created_release_dir & "\documents\OpenHoldem Release Notes.txt"

; Script Start - Add your code below here
MsgBox(0, "Next Step", "Have a look at http://code.google.com/p/openholdembot/source/list and ask the developers, if all work is completed and remind them to update the release-notes")
; Open release-notes for editing
ShellExecute($release_notes)
Sleep(1000)
MsgBox(0, "Next Step", "Update the version and release-date in the release-notes ")
MsgBox(0, "Next Step", "Change the version of OpenHoldem. (Search the OpenHoldem-project for the old one, e.g. ""4.2.5"" and ""4.25"", but don't auto-replace) OpenHoldem\stdaxafx.h OpenHoldem\OpenHoldem.rc")
MsgBox(0, "Next Step", "Choose the correct build option. Usually optimized for OH, but debug if there are some known problems left. Release for Perl (necessary!), and release for the rest (as end-users usually miss debug-DLLs)." )
MsgBox(0, "Next Step", "Rebuild everything")
MsgBox(0, "Next Step", "Test ""everything"", at least briefly that OH ""works"".")
MsgBox(0, "Next Step", "Commit everything to GoogleCode")
MsgBox(0, "Next Step", "Tag the release on googlecode: TurtoiseSVN -> Branch/Tag and then create a tag for svn/tags/OpenHoldem_7.x.y. Comment: ""Tagging OpenHoldem 7.x.y for release"".")

DirRemove($new_openholdem_dir, 1)
DirCopy($pre_created_release_dir, $new_openholdem_dir)
; Copy all executables to the new directory plus their LIBs to support linking
CopyNeededFile($binary_optimized_dir, $new_openholdem_dir, "OpenHoldem.exe")
CopyNeededFile($binary_dir, $new_openholdem_dir, "ManualMode.exe")
CopyNeededFile($binary_dir, $new_openholdem_dir, "OHReplay.exe")
CopyNeededFile($binary_dir, $new_openholdem_dir, "OpenHoldem.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "OpenScrape.exe")
; Copy DLLs to the new directory  plus their LIBs to support linking
CopyNeededFile($binary_dir, $new_openholdem_dir, "DumperDll.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "SymbolDumperUserDll.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "keyboard.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "Reference Keyboard DLL.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "mouse.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "Reference Mouse DLL.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "perl_callback.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "Perl_Callback.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "pokertracker_query_definitions.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "PokerTracker_Query_Definitions.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "scraper.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "Reference Scraper DLL.lib")
CopyNeededFile($binary_dir, $new_openholdem_dir, "user.dll")
CopyNeededFile($binary_dir, $new_openholdem_dir, "Reference User DLL.lib")
; Remove the *.exp and unnecessary *.pdb files.
FileDelete($new_openholdem_dir & "\*.exp")
FileDelete($new_openholdem_dir & "\*.pdb")
; Add the OpenPPL-library
CopyNeededFile($openppl_library_dir, $new_openholdem_dir, "OpenPPL_Library.ohf")
; Add OpenPPL_Manual.chm and symbol_pokertracker.pdf
CopyNeededFile($openppl_manual_dir, $new_openholdem_dir, "OpenPPL_Manual.chm")
CopyNeededFile($pokertracker_docu_dir, $new_openholdem_dir, "symbols_pokertracker.pdf")
; Remove replay-direcoty (if existent), logs and other private data
DirRemove($new_openholdem_dir & "\Replay")
FileDelete($new_openholdem_dir & "\logs\*.*")
; Move everything to the userts desktop
DirMove($new_openholdem_dir, @DesktopDir & "\" & $new_openholdem_dir)
; Release it
MsgBox(0, "Next Step", "The new direcory is at your desktop. Please create a rar-archive")
MsgBox(0, "Next Step", "Upload the rar-file to https://sites.google.com/site/openholdempokerbot/downloads")
; Open release-notes for announcement
ShellExecute($release_notes)
Sleep(1000)
MsgBox(0, "Next Step", "Announce the new download in ""OpenHoldem Stickies"" and post the release-notes.")

Func CopyNeededFile($source_dir, $destination_dir, $name)
   Local $source = $source_dir & "\" & $name
   Local $destination = $destination_dir & "\" & $name
   If Not FileCopy($source, $destination) Then
	  MsgBox(0, "Error", "Missing file: " & $source)
	  Exit(-1)
   EndIf
EndFunc


