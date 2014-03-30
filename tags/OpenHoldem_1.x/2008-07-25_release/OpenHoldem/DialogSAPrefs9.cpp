// DialogSAPrefs9.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs9.h"
#include "Registry.h"
#include "global.h"
#include "Perl.hpp"


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

    m_LoadPerlInterpreter.SetCheck(global.preferences.Perl_load_Interpreter ? BST_CHECKED : BST_UNCHECKED);
    m_LoadDefaultPerlFormula.SetCheck(global.preferences.Perl_load_default_Formula ? BST_CHECKED : BST_UNCHECKED);
    m_DefaultPerlFormula.SetWindowText(global.preferences.Perl_default_Formula);
    m_PerlEditor.SetWindowText(global.preferences.Perl_Editor);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs9::OnOK()
{
    Registry		reg;

    global.preferences.Perl_load_Interpreter = m_LoadPerlInterpreter.GetCheck()==BST_CHECKED ? true : false;
    global.preferences.Perl_load_default_Formula = m_LoadDefaultPerlFormula.GetCheck()==BST_CHECKED ? true : false;
    m_DefaultPerlFormula.GetWindowText(global.preferences.Perl_default_Formula);
    m_PerlEditor.GetWindowText(global.preferences.Perl_Editor);

    reg.read_reg();
    reg.Perl_load_Interpreter = global.preferences.Perl_load_Interpreter;
    reg.Perl_load_default_Formula = global.preferences.Perl_load_default_Formula;
    reg.Perl_default_Formula = global.preferences.Perl_default_Formula;
    reg.Perl_Editor = global.preferences.Perl_Editor;
    reg.write_reg();

	// Load Perl interpreter without a restart
	if (global.preferences.Perl_load_Interpreter)
	{
		the_Perl_Interpreter = Perl();	
	}

    CSAPrefsSubDlg::OnOK();
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs9, CDialog)
END_MESSAGE_MAP()


// DialogSAPrefs9 message handlers
