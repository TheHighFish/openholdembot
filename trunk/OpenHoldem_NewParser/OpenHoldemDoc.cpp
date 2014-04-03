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

// OpenHoldemDoc.cpp : implementation of the COpenHoldemDoc class
//

#include "stdafx.h"
#include "CAutoplayer.h"
#include "CDllExtension.h"
#include "CFormulaParser.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemTitle.h"
#include "CPreferences.h"
#include "DialogFormulaScintilla.h"
#include "MainFrm.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"


// COpenHoldemDoc
IMPLEMENT_DYNCREATE(COpenHoldemDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenHoldemDoc, CDocument)
END_MESSAGE_MAP()

// COpenHoldemDoc construction/destruction
COpenHoldemDoc::COpenHoldemDoc() 
{
	//!!!p_formula->ClearFormula();
}

COpenHoldemDoc::~COpenHoldemDoc() 
{
}

BOOL COpenHoldemDoc::SaveModified()
{
	if (m_formulaScintillaDlg)
	{
		if (m_formulaScintillaDlg->m_dirty)
		{
			if (OH_MessageBox_Interactive(
				"The Formula Editor has un-applied changes.\n"
				"Really exit?", 
				"Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO)
			{
				return false;
			}
		}

		// Kill the formula dialog
		if(m_formulaScintillaDlg) 
			m_formulaScintillaDlg->DestroyWindow();

	}
	return CDocument::SaveModified();
}

BOOL COpenHoldemDoc::OnNewDocument() 
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Default bot
	//!!!p_formula->SetEmptyDefaultBot();

	// Try to unload dll
	p_dll_extension->UnloadDll();

	// Create hand list matrices
	//!!!p_formula->CreateHandListMatrices();

	// Create parse trees for default formula
	//!!!p_formula->ParseAllFormula(PMainframe()->GetSafeHwnd());
	SetModifiedFlag(true);
	p_openholdem_title->UpdateTitle();

	p_dll_extension->LoadDll("");
	return true;
}


// COpenHoldemDoc serialization
void COpenHoldemDoc::Serialize(CArchive& ar) 
{
	// Writing a file
	if (ar.IsStoring()) 
	{
		// Store archive in the new OpenHoldem format
		//!!!p_formula->WriteFormula(ar);
		// Do not close this archive here.
		// It's expected to stay open at this point!
	}
	// Reading a file
	else 
	{
		// Extra caution, in case we want to load a formula,
		// while the autoplayer is engaged.
		// This currently can only happen via the MRU-list.
		// The alternative way would be to disable that list...
		//
		// MainFrame.cpp:
		// ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CMainFrame::OnUpdateLRUList)
		//
		// void CMainFrame::OnUpdateLRUList(CCmdUI *pCmdUI)
		// {
		//	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
		// }
		//
		// Unfortunatelly this does not work, 
		// but removes the list and replaces it once with the default: "Recent file".
		//
		// And there's very little information about both
		// ON_UPDATE_COMMAND_UI_RANGE and temporary disabling of MRU-lists.
		//
		// So we decided to go that route.
		//
		if (p_autoplayer->autoplayer_engaged())
		{
			OH_MessageBox_Interactive("Can't load formula while autoplayer engaged.", "ERROR", 0);
			return;
		}
        // The formula editor gets now handled automatically (Rev. 1425)

		// Read ohf / whf file
		CFormulaParser formula_parser;
		formula_parser.ParseFile(ar);
		SetModifiedFlag(false); // !!!R?

		// Try to unload dll
		p_dll_extension->UnloadDll();

		// Create hand list matrices
		//!!!p_formula->CreateHandListMatrices();

		p_dll_extension->LoadDll("");
		p_openholdem_title->UpdateTitle();
	}
}

COpenHoldemDoc * COpenHoldemDoc::GetDocument() 
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (COpenHoldemDoc *) pFrame->GetActiveDocument();
}

