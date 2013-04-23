// DialogSAPrefs9.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs9.h"

#include "CPerl.hpp"
#include "CPreferences.h"


// DialogSAPrefs9 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs9, CDialog)

CDlgSAPrefs9::CDlgSAPrefs9(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs9::IDD, pParent)
{
}

CDlgSAPrefs9::~CDlgSAPrefs9()
{
}

void CDlgSAPrefs9::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOAD_PERL_INTERPRETER, m_LoadPerlInterpreter);
	DDX_Control(pDX, IDC_LOAD_DEFAULT_PERL_FORMULA, m_LoadDefaultPerlFormula);
	DDX_Control(pDX, IDC_DEFAULT_PERL_FORMULA, m_DefaultPerlFormula);
	DDX_Control(pDX, IDC_PERL_EDITOR, m_PerlEditor);
}

BOOL CDlgSAPrefs9::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();

	m_LoadPerlInterpreter.SetCheck(prefs.perl_load_interpreter() ? BST_CHECKED : BST_UNCHECKED);
	m_LoadDefaultPerlFormula.SetCheck(prefs.perl_load_default_formula() ? BST_CHECKED : BST_UNCHECKED);
	m_DefaultPerlFormula.SetWindowText(prefs.perl_default_formula());
	m_PerlEditor.SetWindowText(prefs.perl_editor());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs9::OnOK()
{
	CString text = "";

	prefs.set_perl_load_interpreter(m_LoadPerlInterpreter.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_perl_load_default_formula(m_LoadDefaultPerlFormula.GetCheck()==BST_CHECKED ? true : false);


	m_DefaultPerlFormula.GetWindowText(text);
	prefs.set_perl_default_formula(text);

	m_PerlEditor.GetWindowText(text);
	prefs.set_perl_editor(text);

   	// Load Perl interpreter without a restart
	if (prefs.perl_load_interpreter())
	{
		if (p_perl)
			delete p_perl;

		p_perl = new CPerl;
	}

	CSAPrefsSubDlg::OnOK();
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs9, CDialog)
END_MESSAGE_MAP()

// DialogSAPrefs9 message handlers
