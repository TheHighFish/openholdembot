// MainToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "MyCToolBar.h"

// CMyToolBar

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

CMyToolBar::CMyToolBar() 
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

    __SEH_LOGFATAL("CMyToolBar::Constructor :\n");
}

CMyToolBar::~CMyToolBar() 
{
    __SEH_HEADER

    __SEH_LOGFATAL("CMyToolBar::Destructor :\n");
}

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
END_MESSAGE_MAP()


void CMyToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("CMyToolBar::OnUpdateCmdUI :\n");
}
