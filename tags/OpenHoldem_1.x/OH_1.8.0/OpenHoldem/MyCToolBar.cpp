// MainToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "MyCToolBar.h"
#include "debug.h"
#include "global.h"

// CMyToolBar

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

CMyToolBar::CMyToolBar() {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMyToolBar::Constructor :\n"); 
		throw;
	}
#endif
}

CMyToolBar::~CMyToolBar() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMyToolBar::Destructor :\n"); 
		throw;
	}
#endif
}

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
END_MESSAGE_MAP()



void CMyToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler) {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMyToolBar::OnUpdateCmdUI :\n"); 
		throw;
	}
#endif
}
