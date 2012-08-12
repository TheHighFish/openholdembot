; Author:       Relhby
; Released at:  http://www.maxinmontreal.com/forums/viewtopic.php?f=259&t=15292
; License:      GPL v3


#include <Array.au3>


If ($CmdLine[0] <> 1) Then
	MsgBox(0, "Error", "Expecting exactly 1 filename as command-line-paramater.")
	Exit
EndIf


Local $filename_in  = $CmdLine[0]
local $filename_out = $filename_in
StringReplace($filename_out, ".ohf", "_optimized.ohf", 1)

Local $filein   = FileOpen($filename_in, 0)
Local $fileout  = FileOpen($filename_out, 2)
Local $counter  = 0
Local $prevfunc = ""
; array to store the generated code for open-ended when-conditions
Local $open_ended_when_conditions


; Check if file opened for reading OK
If $filein = -1 Then
    MsgBox(0, "Error", "Unable to open file.")
    Exit
EndIf


; Read in lines of text until the EOF is reached
While 1
	Local $line = FileReadLine($filein)
    If @error = -1 Then ExitLoop

	Local $result = StringInStr($line, "]  &&")

	If $result=0 then FileWriteLine($fileout, $line)
	If $result<>0 then
    Local $func=StringLeft($line,$result)
	If $func<>$prevfunc then
        $prevfunc=$func
        $counter=$counter+1
		_ArrayAdd($open_ended_when_conditions, "")
		_ArrayAdd($open_ended_when_conditions, "##f$myfunc" & $counter & "##")
		_ArrayAdd($open_ended_when_conditions, $func)
     EndIf

	FileWriteLine($fileout, "f$myfunc" & $counter & StringMid($line, $result+1, stringlen($line)-$result ))
    EndIf

	;  MsgBox(0, "Line read:", $line)
WEnd


; Add functions for open-ended when-conditions to the code
For $i = 0 To (UBound($open_ended_when_conditions) - 1)
	FileWriteLine($fileout, $open_ended_when_conditions[$i])
Next


FileClose($filein)
FileClose($fileout)

