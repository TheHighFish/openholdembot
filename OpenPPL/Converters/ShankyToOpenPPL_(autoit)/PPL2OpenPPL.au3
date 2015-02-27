#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         poker_dog
 Requires:       symbols.txt

 Script Function:
	This script will take the contents of a PPL text profile and convert it to OpenPPL for use with OpenHoldem v7.5 and greater.
	The file sysmbols.txt is required which contains all the Hold'em PPL symbols and their OpenPPL counterpart.  It needs to be
	kept in the same folder as this script when it is run.
	
	When you run the script, you will be asked for the PPL profile that you wish to convert.  After selecting the file the conversion
	begins.  A new file is created with the .oppl extension as well as a log file showing all changes that were made by presenting
	the original code on one line and the modified code directly beneath it for easy comparison.  Note - the source PPL file is never
	altered.  However, the converted file will overwrite any existing file with the same name as will the change log.
	
	You will notice in many of the routines that very similar values are searched for and compared.  This is my attempt to account for
	various	developer inconsistencies such as extra spacings and bad formatting that could be found in any random PPL profile.
	
	I am sure some things will get missed and you will ultimately have an error when you open the converted file in	OpenHoldem but
	I am confident the number of errors will be fewer than if you did this conversion process manually.
	
	I hope that you find this utility useful.

#ce ----------------------------------------------------------------------------

HotKeySet("{ESC}", "Terminate")

Dim $version = "1.01"

Dim $PPL = ""
Dim $OpenPPL = ""

Dim $symbols[200]
Dim $source[20000]
Dim $original[20000]
Dim $NumberOfLines = 0
Dim $NumberOfSymbols = 0

Dim $RaiseMethod = "RaiseBy " ; "RaiseTo "
Dim $UseApproxEqualOperator = True ; False

$PPL = FileOpenDialog("Select the source PPL file", "C:\", "(*.txt)", 1)

LoadSymbols()
LoadSourceFile()

Main()

Exit

Func Main()
	
	$start = TimerInit()
	
	ProgressOn("Performing PPL to OpenPPL Conversion", "", "", 68, 75, "")
	
	CreateConsistencies()
	RemoveOptionSettings()
	FixFunctionHeaders()
	RemoveDelays()
	RemoveSitout()
	RemoveBeep()
	FixBets()
	FixRaises()
	If $UseApproxEqualOperator = True Then FixIntegers()
	FixSymbols()
	FixHandExpressions()
	FixBoardExpressions()
	StandardizeKeywords()
	FixUserVariables()
	AddForce()
	CleanUp()
	WriteConvertedFile()
	
	$elapsed = TimerDiff($start)
	
	WriteChangeLog()
	
	ProgressOff()
		
	MsgBox(0, "Finished", "Conversion complete." & @CRLF & @CRLF & "Lines: " & $NumberOfLines & "     Time: " & Round($elapsed/100000*100,5) & " seconds" & @CRLF & @CRLF & "Saved as: " & $OpenPPL & "     ")
	
EndFunc ; ==> Main()

Func CreateConsistencies()
	
	; We create some consistencies centered around spacing which will make subsequent operations more streamlined
	; Once the conversion is complete we will revert some of these changes
	
	; remove all extra spaces (any sequence of two or more spaces)
	
	UpdateProgress(5,"Create consistencies...")
	
	While 1
		$MadeChanges = False
		For $i = 0 to $NumberOfLines Step + 1
			If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "  ") Then
				$source[$i] = StringReplace($source[$i], "  ", " ", 0)
				$MadeChanges = True
			EndIf
		Next
		If $MadeChanges = False Then ExitLoop
	WEnd
		
	; make all ' = ' consistant by removing leading spaces
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " =") Then
			$source[$i] = StringReplace($source[$i], " =", "=", 0)
		EndIf
	Next
	
	; make all ' = ' consistant by removing trailing spaces
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "= ") Then
			$source[$i] = StringReplace($source[$i], "= ", "=", 0)
		EndIf
	Next
	
	
	; add a space after all '('
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "(") Then
			$source[$i] = StringReplace($source[$i], "(", "( ", 0)
		EndIf
	Next
	
	
	; add a space before all ')'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], ")") Then
			$source[$i] = StringReplace($source[$i], ")", " )", 0)
		EndIf
	Next
	
	; look for missing spaces in common situations
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "suitedor") Then
			$source[$i] = StringReplace($source[$i], "suitedor", "suited or", 0)
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "nothave") Then
			$source[$i] = StringReplace($source[$i], "nothave", "not have", 0)
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " not(") Then
			$source[$i] = StringReplace($source[$i], " not(", " not (", 0)
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " and(") Then
			$source[$i] = StringReplace($source[$i], " and(", " and (", 0)
		EndIf
	Next
	
	; remove all extra spaces (any sequence of two or more spaces)
	While 1
		$MadeChanges = False
		For $i = 0 to $NumberOfLines Step + 1
			If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "  ") Then
				$source[$i] = StringReplace($source[$i], "  ", " ", 0)
				$MadeChanges = True
			EndIf
		Next
		If $MadeChanges = False Then ExitLoop
	WEnd
	
EndFunc ; ==> CreateConsistencies()

Func RemoveOptionSettings()
	
	UpdateProgress(10, "Commenting option settings...")
	
	; PPL option settings are commented out and when the converted file is created they are
	; placed under the ##notes## function where they are safe to delete at the users choosing
	For $i = 0 to $NumberOfLines Step + 1
		If $source[$i] = "preflop" Then ExitLoop
		$source[$i] = "// " & $source[$i]
	Next
	
EndFunc ; ==> RemoveOptionSettings()

Func FixFunctionHeaders()
	
	UpdateProgress(15, "Fixing function headers...")
	
	; PPL betting rounds, preflop, flop, turn, river are converted to OpenHoldem function headings; ##f$<round>##
	For $i = 0 to $NumberOfLines
		If $source[$i] = "preflop" Or $source[$i] = "flop" Or $source[$i] = "turn" Or $source[$i] = "river" Then
			$source[$i] = "##f$" & $source[$i] & "##"
		EndIf
	Next

EndFunc ; ==> ChangeFunctionHeaders()

Func RemoveDelays()
	
	UpdateProgress(20, "Removing delay commands...")
	
	; PPL Delay command is not supported by OpenPPL and gets removed but the rest of the line remains intact
	Local $newstring = ""
	
	For $i = 0 to $NumberOfLines
		If StringInStr($source[$i], "delay") Then
			$temp = StringSplit($source[$i], " ")
			For $j = 1 to $temp[0] - 2
				$newstring = $newstring & " " & $temp[$j]
			Next
			$source[$i] = $newstring
			$newstring = ""
		EndIf
	Next

EndFunc ; ==> RemoveDelays()

Func RemoveSitout()
	
	UpdateProgress(25, "Removing sitout commands...")
	
	; PPL Sitout command is not supported by OpenPPL and thus lines containing this command get
	; commented out but left in for the user to modify or delete if desired
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "sitout") Then
			$source[$i] = "// " & $source[$i]
		EndIf
	Next
	
EndFunc ; ==> ChangeSitout()

Func RemoveBeep()
	
	UpdateProgress(30, "Removing beep commands...")
	
	; PPL Beep command is not supported by OpenPPL and thus lines containing this command get
	; commented out but left in for the user to modify or delete if desired
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "beep") Then
			$source[$i] = "// " & $source[$i]
		EndIf
	Next
	
EndFunc ; ==> ChangeBeep()

Func FixBets()
	
	UpdateProgress(35, "Fixing bet commands...")
	
	; PPL command Bet ## gets replaced by OpenPPL 'RaiseBy' or 'RaiseTo' based on user preference at
	; the top of this file
	For $i = 0 to $NumberOfLines Step + 1
		For $j = 1 To 9 Step + 1
			If StringInStr($source[$i], " bet " & $j) Then
				$source[$i] = StringReplace($source[$i], " Bet " & $j, " " & $RaiseMethod & $j)
			EndIf
		Next
	Next

EndFunc ; ==> FixBets()

Func FixRaises()
	
	UpdateProgress(40, "Fixing raise commands...")
	
	; PPL command Raise gets replaced by OpenPPL 'RaiseBy' or 'RaiseTo' based on user preference at
	; the top of this file
	For $i = 0 to $NumberOfLines Step + 1
		For $j = 1 To 9 Step + 1
			If StringInStr($source[$i], " raise " & $j) Then
				$source[$i] = StringReplace($source[$i], " Raise " & $j, " " & $RaiseMethod & $j)
			EndIf
		Next
	Next

EndFunc ; ==> FixRaises()

Func FixIntegers()
	
	UpdateProgress(45, "Adding approximate-equal commands...")
	
	; PPL uses integers and OpenHoldem uses floating point numbers so it is recommended to use the
	; approximatly-equal symbol for 'BetSize' and 'StackSize' which is selectable at the top of this file
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "betsize=") Then
			$source[$i] = StringReplace($source[$i], "betsize=", "BetSize ~~ ")
		EndIf
	Next
	
	; change 'StackSize =' to 'StackSize ~~'
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "stacksize=") Then
			$source[$i] = StringReplace($source[$i], "stacksize=", "StackSize ~~ ")
		EndIf
	Next
	
EndFunc ; ==> FixIntegers()

Func FixSymbols()
	
	UpdateProgress(60, "Fixing symbols...")
	
	; PPL symbol naming and OpenPPL symbol naming differ (use of numbers, capitalization, etc) so
	; all PPL symbols that have a known OpenPPL counterpart are compared and changes are made where necessary
	
	For $i = 1 to $NumberOfLines ; on every line of code...
		For $j = 1 to $NumberOfSymbols ; go through all the symbols in symbols.txt
			$temp = StringSplit($symbols[$j], ",")
			If StringInStr($source[$i], $temp[1], 0) Then
				$source[$i] = StringReplace($source[$i], $temp[1], $temp[2], 0)
			EndIf
		Next
	Next
	
	; work around for HaveSecondNutFlushDraw always being written HaveSecondNutFlushdraw for no apparent reason
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "flushdraw") Then
			$source[$i] = StringReplace($source[$i], "flushdraw", "FlushDraw", 0)
		EndIf
	Next
	
	; work around for HaveSecondBestKickerOrBetter always being written HaveSecondBestKickerorbetter for no apparent reason
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "orbetter") Then
			$source[$i] = StringReplace($source[$i], "orbetter", "OrBetter", 0)
		EndIf
	Next
	
EndFunc ; ==> FixSymbols()

Func FixHandExpressions()
	
	UpdateProgress(65, "Fixing hand expressions...")
	
	; PPL hand expressions are converted to OpenPPL format
	For $i = 0 to $NumberOfLines Step + 1
		While StringInStr($source[$i], " Hand=")
			$source[$i] = StringReplace($source[$i], " Hand=", " hand$")
		WEnd
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		While StringInStr($source[$i], " suited")
			$source[$i] = StringReplace($source[$i], " suited", "suited")
		WEnd
	Next
	
EndFunc ; ==> ChangeHandExpressions()

Func FixBoardExpressions()
	
	UpdateProgress(70, "Fixing board expressions...")
	
	; PPL board expressions are converted to OpenPPL format
	For $i = 0 to $NumberOfLines Step + 1
		While StringInStr($source[$i], " Board=")
			$source[$i] = StringReplace($source[$i], " Board=", " board$")
		WEnd
	Next
	
EndFunc ; ==> ChangeBoardExpressions()

Func StandardizeKeywords()
	
	UpdateProgress(75, "Standardizing keywords...")
	
	; change all 'when' to 'WHEN'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "when ") Then
			$source[$i] = StringReplace($source[$i], "when ", "WHEN ", 0)
		EndIf
	Next
	
	; change all 'force' to 'FORCE'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " force") Then
			$source[$i] = StringReplace($source[$i], " force", " FORCE", 0)
		EndIf
	Next

	; change all 'and' to 'AND'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " and ") Then
			$source[$i] = StringReplace($source[$i], " and ", " AND ", 0)
		EndIf
	Next
	
	; change all 'or' to 'OR'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " or ") Then
			$source[$i] = StringReplace($source[$i], " or ", " OR ", 0)
		EndIf
	Next
	
	; change all 'not' to 'NOT'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " not ") Then
			$source[$i] = StringReplace($source[$i], " not ", " NOT ", 0)
		EndIf
	Next
	
	; change the action 'fold' to 'Fold'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "fold force") Then
			$source[$i] = StringReplace($source[$i], "fold force", "Fold FORCE", 0)
		EndIf
	Next
	
	; change the action 'call' to 'Call'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "call force") Then
			$source[$i] = StringReplace($source[$i], "call force", "Call FORCE", 0)
		EndIf
	Next
	
	; change the action 'call' to 'Call'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "call ") Then
			$source[$i] = StringReplace($source[$i], "call ", "Call ", 0)
		EndIf
	Next
	
	; change the action 'bet' to 'Bet'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " bet") Then
			$source[$i] = StringReplace($source[$i], " bet", " Bet", 0)
		EndIf
	Next
	
	; change the action 'raisemin' to 'RaiseMin'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "raisemin force") Then
			$source[$i] = StringReplace($source[$i], "raisemin force", "RaiseMin FORCE", 0)
		EndIf
	Next
	
	; change the action 'raisemax' to 'RaiseMax'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "raisemax force") Then
			$source[$i] = StringReplace($source[$i], "raisemax force", "RaiseMax FORCE", 0)
		EndIf
	Next
	
	; change the action 'raisepot' to 'RaisePot'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "raisepot force") Then
			$source[$i] = StringReplace($source[$i], "raisepot force", "RaisePot FORCE", 0)
		EndIf
	Next
	
	; change the action 'check' to 'Check'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "=check") Then
			$source[$i] = StringReplace($source[$i], "=check", "=Check", 0)
		EndIf
	Next
	
	; change the action 'first' to 'First'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "=first") Then
			$source[$i] = StringReplace($source[$i], "=first", "=First", 0)
		EndIf
	Next
	
	; change all 'last' to 'Last'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "=last") Then
			$source[$i] = StringReplace($source[$i], "=last", "=Last", 0)
		EndIf
	Next
	
	; change all 'none' to 'None'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "=none") Then
			$source[$i] = StringReplace($source[$i], "=none", "=None", 0)
		EndIf
	Next

EndFunc ; ==> StandardizeKeywords()

Func FixUserVariables()
	
	UpdateProgress(80, "Fixing user defined variables...")
	
	; PPL User statements require a Set command in front of them in OpenPPL but only when you actually intend
	; to set the value and not reference the value so this script only attempts to add set if it is the last User
	; statement in the line
		
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " user") Then
			$source[$i] = StringReplace($source[$i], " user", " SET user", -1)
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "when set user") Then
			$source[$i] = StringReplace($source[$i], "when set user", "WHEN user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "when ( set user") Then
			$source[$i] = StringReplace($source[$i], "when ( set user", "WHEN ( user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "and set user") Then
			$source[$i] = StringReplace($source[$i], "and set user", "AND user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "and ( set user") Then
			$source[$i] = StringReplace($source[$i], "and ( set user", "AND ( user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "or set user") Then
			$source[$i] = StringReplace($source[$i], "or set user", "OR user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "or ( set user") Then
			$source[$i] = StringReplace($source[$i], "or ( set user", "OR ( user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "not set user") Then
			$source[$i] = StringReplace($source[$i], "not set user", "NOT user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "not ( set user") Then
			$source[$i] = StringReplace($source[$i], "not ( set user", "NOT ( user")
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " User", 1) Then
			$source[$i] = StringReplace($source[$i], " User", " user")
		EndIf
	Next
	
EndFunc ; ==> FixUserVariables()

Func AddForce()
	
	UpdateProgress(85, "Adding missing force commands...")
	
	; check to make sure lines that end in actions have 'FORCE' at the end
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 4) = " Bet" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 5) = " Call" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 5) = " Fold" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 7) = " BetPot" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 9) = " RaiseMin" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 9) = " RaiseMax" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 9) = " RaisePot" Then
			$source[$i] = $source[$i] & " FORCE"
		EndIf
	Next
	
EndFunc ; ==> AddForce()

Func CleanUp()
	
	UpdateProgress(90, "Performing clean up...")
	
	; make all '=' consistant by adding leading and trailing spaces
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "=") Then
			$source[$i] = StringReplace($source[$i], "=", " = ", 0)
		EndIf
	Next
	
	; fix any '< =' that were created
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "< =") Then
			$source[$i] = StringReplace($source[$i], "< =", "<=", 0)
		EndIf
	Next
	
	; fix any '> =' that were created
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "> =") Then
			$source[$i] = StringReplace($source[$i], "> =", ">=", 0)
		EndIf
	Next
	
	; remove any spaces after all '('
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], "( ") Then
			$source[$i] = StringReplace($source[$i], "( ", "(", 0)
		EndIf
	Next
	
	
	; remove any spaces before all ')'
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringInStr($source[$i], " )") Then
			$source[$i] = StringReplace($source[$i], " )", ")", 0)
		EndIf
	Next
	
	; make sure there is a space before all FORCE commands
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringRight($source[$i], 5) = "FORCE" And StringRight($source[$i], 6) <> " FORCE" Then
			$source[$i] = StringReplace($source[$i], "FORCE", " FORCE")
		EndIf
	Next
	
	; make sure there is a space after all WHEN commands
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringLeft($source[$i], 4) = "WHEN" And StringLeft($source[$i], 5) <> "WHEN " Then
			$source[$i] = StringReplace($source[$i], "WHEN", "WHEN ")
		EndIf
	Next
	
	; make sure there is a space after all WHEN commands when preceeded by a space (common)
	For $i = 0 to $NumberOfLines Step + 1
		If StringLeft($source[$i], 2) <> "//" And StringLeft($source[$i], 5) = " WHEN" And StringLeft($source[$i], 6) <> " WHEN " Then
			$source[$i] = StringReplace($source[$i], " WHEN", " WHEN ")
		EndIf
	Next
	
EndFunc ; ==> CleanUp()

Func LoadSymbols()
	
	$i = 0
	$line = ""
	
	$symbolsfile =  FileOpen(@ScriptDir & "\symbols.txt", 0)
	
	; Check if the input file opened for reading OK
	If $symbolsfile = -1 Then
		MsgBox(0, "Error", "The file 'symbols.txt' (required) was not found.")
		Exit
	EndIf
	
	While 1
		
		$i = $i + 1
		$line = FileReadLine($symbolsfile)
		If @error = -1 Or $line = "" Then ExitLoop
		$symbols[$i] = $line
		$NumberOfSymbols = $i
		
	WEnd
	
	FileClose($symbolsfile)
	
EndFunc ; ==> LoadSymbols()

Func LoadSourceFile()
	
	$i = 0	
	$line = ""
	
	$sourcefile = FileOpen($PPL, 0)
	
	; Check if the input file opened for reading OK
	If $sourcefile = -1 Then
		MsgBox(0, "Error", "Unable to open file.")
		Exit
	EndIf
	
	While 1
		
		$i = $i + 1
		$line = FileReadLine($sourcefile)
		If @error = -1 Then ExitLoop
		$source[$i] = $line
		$original[$i] = $line
		$NumberOfLines = $i
		
	WEnd
	
	FileClose($sourcefile)
	
EndFunc ; ==> LoadSourceFile()

Func WriteConvertedFile()
	
	UpdateProgress(95, "Writing converted file...")
	
	$OpenPPL = StringLeft($PPL, StringLen($PPL)-3) & "oppl"
	$convertedfile = FileOpen($OpenPPL, 2)
	
	; Check if the output file opened for writing OK
	If $convertedfile = -1 Then
		MsgBox(0, "Error", "Unable to open file.")
		Exit
	EndIf
	
	FileWriteLine($convertedfile, "##notes##")
	FileWriteLine($convertedfile, @CRLF)
	FileWriteLine($convertedfile, "///////////////////////////////////////////////////////////////////////")
	FileWriteLine($convertedfile, "//                                                                   //")
	FileWriteLine($convertedfile, "// Converted using Poker_dog's PPL to OpenPPL Converter version " & $version & " //")
	FileWriteLine($convertedfile, "//                                                                   //")
	FileWriteLine($convertedfile, "///////////////////////////////////////////////////////////////////////")
	FileWriteLine($convertedfile, @CRLF)
		
	For $i = 1 To $NumberOfLines Step + 1
		FileWriteLine($convertedfile, $source[$i])
	Next
	
	FileClose($convertedfile)
	
EndFunc ; ==> WriteConvertedFile()

Func WriteChangeLog()
	
	UpdateProgress(100, "Writing change log...")
	
	$Log = StringLeft($PPL, StringLen($PPL)-4) & " - change.log"
	$changelog = FileOpen($Log, 2)
	
	; Check if the output file opened for writing OK
	If $changelog = -1 Then
		MsgBox(0, "Error", "Unable to open file.")
		Exit
	EndIf
	
	FileWriteLine($changelog, "Change Log" & @CRLF & @CRLF)
	FileWriteLine($changelog, @TAB & "Source: " & $PPL)
	FileWriteLine($changelog, @TAB & "Output: " & $OpenPPL & @CRLF & @CRLF)
	
	For $i = 1 To $NumberOfLines Step + 1
		If StringCompare($original[$i], $source[$i], 1) <> 0 Then
			FileWriteLine($changelog, "Line: " & $i)
			FileWriteLine($changelog, " PPL: " & $original[$i])
			FileWriteLine($changelog, "OPPL: " & $source[$i])
			FileWriteLine($changelog, @CRLF & @CRLF)
		EndIf
	Next
	
EndFunc ; ==> WriteChangeLog()

Func UpdateProgress($percent,$routine)
	
	ProgressSet($percent, @LF & $routine, "")
	
EndFunc ; ==> UpdateProgress()

Func Terminate()
	Exit
EndFunc ; ==> Terminate()
