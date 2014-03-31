; Author:       Relhby
; Released at:  http://www.maxinmontreal.com/forums/viewtopic.php?f=259&t=15292
; License:      GPL v3


#include <Array.au3>

; Getting command-line-arguments
If ($CmdLine[0] <> 1) Then
	MsgBox(0, "Error", "Expecting exactly 1 filename as command-line-paramater.")
	Exit
EndIf

; Preparing filenames
Local $filename_in  = $CmdLine[1]
local $filename_out = StringReplace($filename_in, ".ohf", "_optimized.ohf", 1)
If ($filename_in = $filename_out) Then
	MsgBox(0, "Error", "Filename [" & $filename_in & "] does not end on [.ohf]")
	Exit
EndIf

; Opening files
Local $filein   = FileOpen($filename_in, 0)
Local $fileout  = FileOpen($filename_out, 2)

; Global vars
Local $counter  = 0
Local $prevfunc = ""
; array to store the generated code for open-ended when-conditions
Local $open_ended_when_conditions[1] = [""]


; Check if file opened for reading OK
If $filein = -1 Then
    MsgBox(0, "Error", "Unable to open file [" & $filename_in & "]")
    Exit
EndIf

; Read in lines of text until the EOF is reached
While 1
	Local $line = FileReadLine($filein)
    If @error = -1 Then ExitLoop

	Local $result = StringInStr($line, "]  &&")

	; Write "normal" lines without modification
	If $result = 0 then FileWriteLine($fileout, $line)
	If $result <> 0 then
		; Process open-ended when-conditions
		Local $func = StringLeft($line,$result)
		If $func <> $prevfunc then
			; New open-ended when-condition
			; Generate a function for it
			$prevfunc = $func
			$counter  = $counter+1
			_ArrayAdd($open_ended_when_conditions, "##f$OEWC_" & $counter & "##")
			_ArrayAdd($open_ended_when_conditions, $func)
			_ArrayAdd($open_ended_when_conditions, "")
			_ArrayDisplay($open_ended_when_conditions)
		EndIf

		; Code-line consisting of an OEWC, a normal condition and an action
		; replace the OEWC by its corresponding function and write to file
		FileWriteLine($fileout, "f$OEWC" & $counter & StringMid($line, $result+1, stringlen($line)-$result ))
    EndIf

	;  MsgBox(0, "Line read:", $line)
WEnd
_ArrayDisplay($open_ended_when_conditions)

; Add functions for open-ended when-conditions to the code
FileWriteLine($fileout, "")
FileWriteLine($fileout, "//***********************************************************")
FileWriteLine($fileout, "//")
FileWriteLine($fileout, "//  Auto-generated functions")
FileWriteLine($fileout, "//  for optimized open-ended when-conditions")
FileWriteLine($fileout, "//")
FileWriteLine($fileout, "//***********************************************************")
For $i = 0 To (UBound($open_ended_when_conditions) - 1)
	FileWriteLine($fileout, $open_ended_when_conditions[$i])
Next

; Close all files
FileClose($filein)
FileClose($fileout)

; Final feedback
MsgBox(0, "Info", "Conversion done: [" & $filename_out & "]")
