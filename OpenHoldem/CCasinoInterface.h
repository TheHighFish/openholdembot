#ifndef INC_CCASINOINTERFACE_H
#define INC_CCASINOINTERFACE_H

#include "MagicNumbers.h"


extern class CCasinoInterface
{
public:
	CCasinoInterface();
	~CCasinoInterface();
public:
	bool ClickButton(int button_code);
	bool ClickButtonSequence(int first_button_code, int second_button_code, 
		int delay_in_milli_seconds);
	bool EnterBetsize(double total_betsize_in_dollars);
	bool UseSliderForAllin();
	bool CloseWindow();
	bool EnterChatMessage(char *message);
	bool ClickI86ButtonIfAvailable(int button_number);
	int  NumberOfVisibleButtons();
private:
	bool TableLostFocues();
	void ClickRect(RECT rect);
	void SelectSwagText();
	void DeleteSwagText();
	void GetNeccessaryTablemapObjects();
private:
	RECT action_buttons[k_number_of_autoplayer_functions];
	POINT p_null;
	// ToDo: Sliderbar, Chatbox, etc.
// tablemap regions
private:
	RECT allin_button;
	RECT raise_button;
	RECT call_button;
	RECT check_button;
	RECT fold_button;
	RECT prefold_button;
	RECT sitin_button;
	RECT sitout_button;
	RECT leave_button;
	RECT autopost_button;
	RECT i86_button;
	RECT i86X_button[k_max_number_of_i86X_buttons];
	RECT i3_button;     // Old WH-Standard; i3 is for swag
	RECT i3_edit_region;
	RECT i3_slider_region;
	RECT i3_handle_region;
// regions defined
private:
	bool allin_button_defined;
	bool raise_button_defined;
	bool call_button_defined;
	bool check_button_defined;
	bool fold_button_defined;
	bool prefold_button_defined;
	bool sitin_button_defined;
	bool sitout_button_defined;
	bool leave_button_defined;
	bool autopost_button_defined;
	bool i86_button_defined;
	bool i86X_button_defined[k_max_number_of_i86X_buttons];
	bool i3_button_defined;
	bool i3_edit_defined;
	bool i3_slider_defined;
	bool i3_handle_defined;
// regions available
private:
	bool allin_option_available;
	bool allin_button_available;
	bool raise_button_available;
	bool call_button_available;
	bool check_button_available;
	bool fold_button_available;
	bool prefold_button_available;
	bool sitin_button_available;
	bool sitout_button_available;
	bool leave_button_available;
	bool autopost_button_available;
	bool i86_button_available;
	bool i86X_button_available[k_max_number_of_i86X_buttons];
	bool i3_button_available;
private:
	bool _i86X_state[k_max_number_of_i86X_buttons];
} *p_casino_interface;

#endif // INC_CCasinoInterface_H