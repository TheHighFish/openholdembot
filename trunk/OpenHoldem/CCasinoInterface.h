#ifndef INC_CCASINOINTERFACE_H
#define INC_CCASINOINTERFACE_H

#include "MagicNumbers.h"


extern class CCasinoInterface
{

public:
	CCasinoInterface();
	~CCasinoInterface();
public:
	bool ClickButton(int autoplayer_code);
	bool ButtonAvailable(int autoplayer_code);
	bool ButtonClickable(int autoplayer_code);
	bool ClickButtonSequence(int first_button, int second_button, int delay_in_milli_seconds);
	bool EnterBetsize(double total_betsize_in_dollars);
	bool UseSliderForAllin();
	bool CloseWindow();
	bool EnterChatMessage(CString &message);
	bool ClickI86ButtonIfAvailable(int button_number);
	int  NumberOfVisibleAutoplayerButtons();

private:
	bool TableLostFocus();
	void ClickRect(RECT rect);
	void SelectSwagText();
	void DeleteSwagText();

	POINT p_null;
	RECT r_null; 
	// ToDo: Sliderbar, Chatbox, etc.

public:
	// regions 
	RECT action_buttons[k_number_of_autoplayer_functions];
	RECT i3_button;
	RECT i3_edit_region;
	RECT i3_slider_region;
	RECT i3_handle_region;
	RECT i86_button;
	RECT i86X_button[k_max_number_of_i86X_buttons];

} *p_casino_interface;

#endif // INC_CCasinoInterface_H