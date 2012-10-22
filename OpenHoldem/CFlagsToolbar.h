#include "MyCtoolbar.h"

extern class CFlagsToolbar: public CWnd
{
public:
	CFlagsToolbar(CFrameWnd *parent_window);
	~CFlagsToolbar();
public:
	bool GetFlag(const int i); 
	int  GetFlagMax();
	long int  GetFlagBits();
public:
	void SetFlag(int flag_number, bool new_value);
	void DisableButtonsOnConnect();
	void EnableButtonsOnDisconnect();
	void UnattachOHFromPokerWindow();
public:
	void EnableButton(int button_ID, bool new_status);
	void CheckButton(int button_ID, bool new_status);
	bool IsButtonChecked(int button_ID);
	bool IsButtonEnabled(int button_ID);
public:
	CMyToolBar _tool_bar;
protected:
	afx_msg void OnClickedFlags();
	DECLARE_MESSAGE_MAP()
private:
	void InitCFlagsToolbar(CWnd *parent_window);
	void CreateMainToolbar(void);
	void CreateFlagsToolbar(void);
	void AlignToolbars(void);
private:
	CMyToolBar	m_MainToolBar;
	bool		_flags[k_number_of_flags];
	CFrameWnd	*_parent_window;
	CCritSec	m_critsec;
} *p_flags_toolbar;