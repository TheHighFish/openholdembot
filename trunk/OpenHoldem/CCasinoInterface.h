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
	bool ButtonAvailable(int button_code);
	bool ClickButtonSequence(int first_button_code, int second_button_code, int delay_in_milli_seconds);
	bool EnterBetsize(double total_betsize_in_dollars);
	bool UseSliderForAllin();
	bool CloseWindow();
	bool EnterChatMessage(CString &message);
	bool ClickI86ButtonIfAvailable(int button_number);

private:
	bool TableLostFocus();
	void ClickRect(RECT rect);
	void SelectSwagText();
	void DeleteSwagText();

	POINT p_null;
	// ToDo: Sliderbar, Chatbox, etc.

} *p_casino_interface;

#endif // INC_CCasinoInterface_H