#include "StdAfx.h"
#include "StickyButton.h"

const UINT WM_STICKYBUTTONDOWN = RegisterWindowMessage("WM_STICKYBUTTONDOWN");
const UINT WM_STICKYBUTTONUP = RegisterWindowMessage("WM_STICKYBUTTONUP");

CStickyButton::CStickyButton(void) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		m_bState = false;
		m_bIgnore = false;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CStickyButton::Constructor : \n"); 
		throw;
	}
#endif
}

CStickyButton::~CStickyButton(void) {
}
BEGIN_MESSAGE_MAP(CStickyButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CStickyButton::OnBnClicked)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CStickyButton::OnBnClicked() {
#ifdef SEH_ENABLE
	try {
#endif
		if (m_bIgnore) {
			m_bIgnore = false;
			SetState(true);
		}
		else {
			m_bState = !m_bState;
			SetState(m_bState); 
			ASSERT(GetParent());
			GetParent()->SendMessage(m_bState ? WM_STICKYBUTTONDOWN : WM_STICKYBUTTONUP, (WPARAM) this->GetSafeHwnd());
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CStickyButton::OnBnClicked : \n"); 
		throw;
	}
#endif
}

void CStickyButton::OnKillFocus(CWnd* pNewWnd) {
#ifdef SEH_ENABLE
	try {
#endif
		if (m_bState) {
			m_bIgnore = true;
		} 

		// The following function pops up the button if it's down:
		CButton::OnKillFocus(pNewWnd);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CStickyButton::OnKillFocus : \n"); 
		throw;
	}
#endif
}
