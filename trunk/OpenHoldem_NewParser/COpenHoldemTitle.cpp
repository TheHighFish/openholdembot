//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#include "stdafx.h"
#include "COpenHoldemTitle.h"

#include <assert.h>
#include "CAutoConnector.h"
#include "CFilenames.h"
#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "../CTablemap/CTablemap.h"
#include "MainFrm.h"

COpenHoldemTitle *p_openholdem_title = NULL;

COpenHoldemTitle::COpenHoldemTitle()
{
	user_defined_title = "";
	assert(p_filenames != NULL);
	simple_title = p_filenames->PureExecutableFilename();
}

COpenHoldemTitle::~COpenHoldemTitle()
{}

CString COpenHoldemTitle::GetTitle()
{
	// user-defined overwrites everything
	if (user_defined_title != "")
	{
		return user_defined_title;
	}
	if (preferences.simple_window_title())
	{
		return simple_title;
	}
	else
	{
		return FullTitle();
	}
}

CString COpenHoldemTitle::FullTitle()
{
	assert(p_autoconnector != NULL);
	assert(p_function_collection != NULL);
	assert(p_tablemap != NULL);

	CString full_title;
	char table_title[MAX_WINDOW_TITLE];

	GetWindowText(p_autoconnector->attached_hwnd(), table_title, 
		MAX_WINDOW_TITLE);
	if (p_autoconnector->IsConnected())
	{
		full_title.Format("%s - %s (%s)", p_function_collection->Title(), 
			p_tablemap->sitename(), table_title);
	}
	else
	{
		full_title.Format("%s", p_function_collection->Title());
	}
	return full_title;
}

void COpenHoldemTitle::SetUserDefinedOpenHoldemTitle(CString new_title)
{
	user_defined_title = new_title;
	UpdateTitle();
}

void COpenHoldemTitle::UpdateTitle()
{
	if (PMainframe() == NULL)
	{
		// Missing main window can happen very early during execution
		// if OpenHoldem creates a default document with default title
		// but the window does not yet exist.
		return;
	}

	// PostMessage(WMA_SETWINDOWTEXT, 0, (LPARAM)(GetTitle().GetString()));
	// can't be used, because that would call COpenHoldemHopperCommunication::OnSetWindowText
	// which would then call SetUserDefinedOpenHoldemTitle()
	// which would call UpdateTitle()
	// -> endless recursion
	static CString current_title;
	current_title = GetTitle();
	HWND main_window = PMainframe()->GetSafeHwnd();
	SetWindowText(main_window, current_title);

}
