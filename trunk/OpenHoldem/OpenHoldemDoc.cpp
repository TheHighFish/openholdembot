// OpenHoldemDoc.cpp : implementation of the COpenHoldemDoc class
//

#include "stdafx.h"
#include "OpenHoldemDoc.h"
#include "OpenHoldem.h"
#include "MainFrm.h"

#include "CGlobal.h"
#include "CPreferences.h"

#include "DialogFormulaScintilla.h"

// COpenHoldemDoc

IMPLEMENT_DYNCREATE(COpenHoldemDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenHoldemDoc, CDocument)
END_MESSAGE_MAP()


// COpenHoldemDoc construction/destruction

COpenHoldemDoc::COpenHoldemDoc() 
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

	p_formula->ClearFormula();

	__SEH_LOGFATAL("COpenHoldemDoc::Constructor : \n"); 
}


COpenHoldemDoc::~COpenHoldemDoc() 
{
}

BOOL COpenHoldemDoc::OnNewDocument() 
{
	__SEH_HEADER

	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	if (!CDocument::OnNewDocument())
	return FALSE;

	// Kill the formula dialog, if it is open
	if(m_formulaScintillaDlg) 
	{
		delete m_formulaScintillaDlg;
		m_formulaScintillaDlg	=	NULL;
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
	}

	// Default bot
	p_formula->SetDefaultBot();

	// Try to unload dll
	p_dll_extension->UnloadDll();

	// Create hand list matrices
	p_formula->CreateHandListMatrices();

	// Create parse trees for default formula
	p_formula->ParseAllFormula(pMyMainWnd->GetSafeHwnd());

	SetTitle("Default");
	//SetModifiedFlag(true);

	// Load dll, if set in preferences
	if (prefs.dll_load_on_startup())
	p_dll_extension->LoadDll("");

	return true;
 
	__SEH_LOGFATAL("COpenHoldemDoc::OnNewDocument : \n"); 
}


// COpenHoldemDoc serialization
void COpenHoldemDoc::Serialize(CArchive& ar) 
{
	__SEH_HEADER

	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	// Writing a file
	if (ar.IsStoring()) 
	{

		if (IsWinHoldemFormat(ar.GetFile()->GetFileName())) 
		{
			MessageBox(0, "Converting file formats\n{whf, whx} -> {ohf}",
				"File Conversion", MB_OK | MB_ICONINFORMATION);		
			CString the_old_FileName = GetPathName();
			CString the_new_FileName = GetPathName();
			the_new_FileName.Replace("whf", "ohf");
			SetPathName(the_new_FileName);			
			ar.GetFile()->Close();			
			//ar.GetFile()->Rename(the_old_FileName, the_new_FileName);	
			ar.GetFile()->Open(the_new_FileName, CFile::modeCreate | CFile::modeWrite);
		}

		p_formula->WriteFormula(ar);
	}

	// Reading a file
	else 
	{
		// Kill the formula dialog, if it is open
		if(m_formulaScintillaDlg) 
		{
			delete m_formulaScintillaDlg;
			m_formulaScintillaDlg	=	NULL;
			pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
		}

		// Read ohf / whf file
		ReadFormula(ar);
		SetModifiedFlag(false);

		p_formula->set_formula_name(ar.GetFile()->GetFileName());

		// Try to unload dll
		p_dll_extension->UnloadDll();

		// Create hand list matrices
		p_formula->CreateHandListMatrices();

		// Create parse trees for newly loaded formula
		p_formula->ParseAllFormula(pMyMainWnd->GetSafeHwnd());

		// Load dll, if set in preferences
		if (prefs.dll_load_on_startup())
			p_dll_extension->LoadDll("");
	}
 
	__SEH_LOGFATAL("COpenHoldemDoc::Serialize :\n"); 
}


void COpenHoldemDoc::ReadFormula(CArchive& ar) 
{
	__SEH_HEADER

	// Clear everything
	p_formula->ClearFormula();

	// There are two types of formulas
	//   * ohf 
	//   * whf and optional whx
	// In the latter case we have to read both files. 
	p_formula->ReadFormulaFile(ar, true);

	CFile *cf_whf = ar.GetFile();  
	CString CSpath = cf_whf->GetFilePath(); 

	if (IsWinHoldemFormat(CSpath))
		{
			CFile *cf_whf = ar.GetFile();
			CFile cf_whx; 
			CString CSpath = cf_whf->GetFilePath();
			CSpath.Replace(".whf", ".whx");

			if (cf_whx.Open(CSpath, CFile::modeNoTruncate | CFile::modeRead| CFile::shareDenyWrite)) 
			{ 
				CArchive ar_whx(&cf_whx, CArchive::load);   
				// Read whx file, too. //???	
				p_formula->ReadFormulaFile(ar_whx, false);	
			}
	}

	// Check and add missing...
	p_formula->CheckForDefaultFormulaEntries();
			
	__SEH_LOGFATAL("COpenHoldemDoc::ReadFormula :\n"); 
}

BOOL COpenHoldemDoc::IsWinHoldemFormat(CString the_FileName)
{	
	__SEH_HEADER

	unsigned int Length = the_FileName.GetLength();

	// Path maybe undefined at startup...
	if (Length < 3)
		return false;

	// Checking the first character of the file extension
	// Assuming an extension of size 3: "ohf" or "whf".
	char critical_Character = the_FileName.GetString()[Length - 3];

	return (critical_Character == 'w');

	__SEH_LOGFATAL("COpenHoldemDoc::IsWinHoldemFormat :\n");
}

COpenHoldemDoc * COpenHoldemDoc::GetDocument() 
{
	__SEH_HEADER

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (COpenHoldemDoc *) pFrame->GetActiveDocument();
 
	__SEH_LOGFATAL("COpenHoldemDoc::GetDocument : \n"); 
}

