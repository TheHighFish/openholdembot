//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

// MainToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "MyCToolBar.h"

// CMyToolBar

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

CMyToolBar::CMyToolBar() 
{
}

CMyToolBar::~CMyToolBar() 
{
}

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
END_MESSAGE_MAP()


void CMyToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler) 
{
}
