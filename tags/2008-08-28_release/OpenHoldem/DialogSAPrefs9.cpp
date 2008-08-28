// DialogSAPrefs9.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs9.h"

#include "CGlobal.h"

#include "Registry.h"
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

    m_LoadPerlInterpreter.SetCheck(p_global->preferences.Perl_load_Interpreter ? BST_CHECKED : BST_UNCHECKED);
    m_LoadDefaultPerlFormula.SetCheck(p_global->preferences.Perl_load_default_Formula ? BST_CHECKED : BST_UNCHECKED);
    m_DefaultPerlFormula.SetWindowText(p_global->preferences.Perl_default_Formula);
    m_PerlEditor.SetWindowText(p_global->preferences.Perl_Editor);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs9::OnOK()
{
    Registry		reg;

    p_global->preferences.Perl_load_Interpreter = m_LoadPerlInterpreter.GetCheck()==BST_CHECKED ? true : false;
    p_global->preferences.Perl_load_default_Formula = m_LoadDefaultPerlFormula.GetCheck()==BST_CHECKED ? true : false;
    m_DefaultPerlFormula.GetWindowText(p_global->preferences.Perl_default_Formula);
    m_PerlEditor.GetWindowText(p_global->preferences.Perl_Editor);

    reg.read_reg();
    reg.Perl_load_Interpreter = p_global->preferences.Perl_load_Interpreter;
    reg.Perl_load_default_Formula = p_global->preferences.Perl_load_default_Formula;
    reg.Perl_default_Formula = p_global->preferences.Perl_default_Formula;
    reg.Perl_Editor = p_global->preferences.Perl_Editor;
    reg.write_reg();

	// Load Perl interpreter without a restart
	if (p_global->preferences.Perl_load_Interpreter)
	{
		if (the_Perl_Interpreter)
			delete the_Perl_Interpreter;

		the_Perl_Interpreter = new Perl;
	}

    CSAPrefsSubDlg::OnOK();
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs9, CDialog)
END_MESSAGE_MAP()


// DialogSAPrefs9 message handlers
