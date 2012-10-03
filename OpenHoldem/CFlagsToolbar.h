#include "MyCtoolbar.h"

extern class CFlagsToolbar: public CWnd
{
public:
	CFlagsToolbar();
	~CFlagsToolbar();
public:
	bool GetFlag(const int i); 
	int  GetFlagMax();
	long int  GetFlagBits();
public:
	void SetFlag(int flag_number, bool new_value);
public:
	CMyToolBar _tool_bar;
protected:
	afx_msg void OnClickedFlags();
	DECLARE_MESSAGE_MAP()
private:
	void InitCFlagsToolbar();
private:
	bool _flags[k_number_of_flags];
	CCritSec	m_critsec;
} *p_flags_toolbar;