#pragma once
#include "afxwin.h"

extern const UINT WM_STICKYBUTTONDOWN;
extern const UINT WM_STICKYBUTTONUP;

class CStickyButton : public CButton {
public:
	CStickyButton(void);
	~CStickyButton(void);

	bool	m_bState;
	bool	m_bIgnore;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClicked();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
