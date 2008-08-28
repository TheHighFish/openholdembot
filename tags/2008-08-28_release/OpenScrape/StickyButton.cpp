#include "StdAfx.h"
#include "StickyButton.h"
#include "debug.h"

const UINT WM_STICKYBUTTONDOWN = RegisterWindowMessage("WM_STICKYBUTTONDOWN");
const UINT WM_STICKYBUTTONUP = RegisterWindowMessage("WM_STICKYBUTTONUP");

CStickyButton::CStickyButton(void) 
{
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);

	m_bState = false;
	m_bIgnore = false;
}

CStickyButton::~CStickyButton(void) 
{
}
BEGIN_MESSAGE_MAP(CStickyButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CStickyButton::OnBnClicked)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CStickyButton::OnBnClicked() 
{
	__SEH_HEADER

	if (m_bIgnore) {

		m_bIgnore = false;
		SetState(true);
	}
	else 
	{
		m_bState = !m_bState;
		SetState(m_bState); 
		ASSERT(GetParent());
		GetParent()->SendMessage(m_bState ? WM_STICKYBUTTONDOWN : WM_STICKYBUTTONUP, (WPARAM) this->GetSafeHwnd());
	}

	__SEH_LOGFATAL("CStickyButton::OnBnClicked : \n");
}

void CStickyButton::OnKillFocus(CWnd* pNewWnd) 
{
	__SEH_HEADER

	if (m_bState) 
	{
		m_bIgnore = true;
	} 

	// The following function pops up the button if it's down:
	CButton::OnKillFocus(pNewWnd);

	__SEH_LOGFATAL("CStickyButton::OnKillFocus : \n");
}
