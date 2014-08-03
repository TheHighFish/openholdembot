; Turns random flags buttons on and off,
; just to demonstrate communication between hopper and OH

Local $message_ID = 0
Local $set_button = False
Local $list_of_openholdem_HWNDs = WinList("OpenHoldem")

If $list_of_openholdem_HWNDs[0][0] = 0 Then
	MsgBox(0x10, "Error", "OpenHoldem.exe not found.")
	Exit
EndIf

Local $openholdem_HWND = $list_of_openholdem_HWNDs[1][1]

While True
	Local $button_number = Floor(Random(1, 20))
	$set_button = Not $set_button
	If $set_button Then
		$message_ID = 0x8005
	Else
		; unset the button
		$message_ID = 0x8006
	EndIf
	; Similar to USBFreaks message tutorial
	DllCall("user32.dll", "lresult", "SendMessage", "hwnd", _
		$openholdem_HWND, "uint", $message_ID, "wparam", 0, _
		"lparam", $button_number)
	Sleep(125)
WEnd
