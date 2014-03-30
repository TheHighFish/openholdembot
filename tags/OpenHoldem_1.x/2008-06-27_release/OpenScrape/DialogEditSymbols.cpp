// DialogEditSymbols.cpp : implementation file
//

#include "stdafx.h"
#include "DialogEditSymbols.h"
#include "OpenScrapeDoc.h"

// CDlgEditSymbols dialog

IMPLEMENT_DYNAMIC(CDlgEditSymbols, CDialog)

CDlgEditSymbols::CDlgEditSymbols(CWnd* pParent /*=NULL*/) : CDialog(CDlgEditSymbols::IDD, pParent)
{
}

CDlgEditSymbols::~CDlgEditSymbols()
{
}

void CDlgEditSymbols::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_TITLETEXT, m_Titletext);
	DDX_Control(pDX, IDC_PARSERESULTS, m_ParseResults);
	DDX_Control(pDX, IDC_PARSEBUTTON, m_ParseButton);
}


BEGIN_MESSAGE_MAP(CDlgEditSymbols, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditSymbols::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PARSEBUTTON, &CDlgEditSymbols::OnBnClickedParsebutton)
	ON_CBN_SELCHANGE(IDC_NAME, &CDlgEditSymbols::OnCbnSelchangeName)
END_MESSAGE_MAP()


// CDlgEditSymbols message handlers

BOOL CDlgEditSymbols::OnInitDialog()
{
	CDialog::OnInitDialog();

	int			i;
	CString		text;

	CDialog::OnInitDialog();

	SetWindowText(titletext.GetString());

	for (i=0; i<strings.GetSize(); i++)  m_Name.AddString(strings[i]);
	m_Name.SelectString(-1, name);
	m_Name.SetWindowPos(NULL, 0, 0, 145, 300, SWP_NOMOVE | SWP_NOZORDER);

	m_Value.SetWindowText(value.GetString());

	m_Titletext.SetWindowText(titlebartext.GetString());

	OnCbnSelchangeName();

	m_Name.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditSymbols::OnBnClickedOk()
{
	m_Name.GetWindowText(name);
	m_Value.GetWindowText(value);

	OnOK();
}


void CDlgEditSymbols::OnBnClickedParsebutton()
{
	CString				results, text, format;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();

	m_Titletext.GetWindowText(text);
	m_Value.GetWindowText(format);
	
	pDoc->trans.parse_string_bsl(text, format, &results);

	m_ParseResults.SetWindowText(results.GetString());
}

void CDlgEditSymbols::OnCbnSelchangeName()
{
	m_Name.GetWindowText(name);

	if (name.Find("ttlimits") != -1)
	{
		m_Titletext.EnableWindow(true);
		m_ParseResults.EnableWindow(true);
		m_ParseButton.EnableWindow(true);
	}

	else
	{
		m_Titletext.EnableWindow(false);
		m_ParseResults.EnableWindow(false);
		m_ParseButton.EnableWindow(false);
	}
}
