// DialogEditSymbols.cpp : implementation file
//

#include "stdafx.h"
#include "DialogEditSymbols.h"
#include "OpenScrapeDoc.h"

// CDlgEditSymbols dialog

IMPLEMENT_DYNAMIC(CDlgEditSymbols, CDialog)

CDlgEditSymbols::CDlgEditSymbols(CWnd* pParent /*=NULL*/) : CDialog(CDlgEditSymbols::IDD, pParent)
{
    __SEH_SET_EXCEPTION_HANDLER
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
	ON_EN_KILLFOCUS(IDC_VALUE, &CDlgEditSymbols::OnEnKillfocusValue)
END_MESSAGE_MAP()


// CDlgEditSymbols message handlers

BOOL CDlgEditSymbols::OnInitDialog()
{
	CDialog::OnInitDialog();

	int			i;
	CString		text;

	CDialog::OnInitDialog();

	SetWindowText(titletext.GetString());
	m_Titletext.SetWindowText(titlebartext.GetString());

	// Set drop down choices for "Record name" field and select current
	for (int i=0; i<strings.GetSize(); i++)  m_Name.AddString(strings[i]);
	m_Name.SelectString(-1, name);
	m_Name.SetWindowPos(NULL, 0, 0, 145, 300, SWP_NOMOVE | SWP_NOZORDER);

	m_Value.SetWindowText(value);
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
	CTransform			trans;

	m_Titletext.GetWindowText(text);
	m_Value.GetWindowText(format);
	
	trans.ParseStringBSL(text, format, &results);

	m_ParseResults.SetWindowText(results.GetString());
}

void CDlgEditSymbols::OnCbnSelchangeName()
{
	LRESULT index = m_Name.SendMessage(CB_GETCURSEL, (WPARAM) 0, 0);
	int len = m_Name.SendMessage(CB_GETLBTEXTLEN, (WPARAM) index, (LPARAM) 0);

	char *buffer = new char[len+1];
 
	m_Name.SendMessage(CB_GETLBTEXT, (WPARAM) index, (LPARAM) buffer);
	name.Format("%s", buffer);

	delete [] buffer;           
	buffer = NULL;

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

	m_Value.GetWindowText(value);
	m_Value.Clear();
	m_Value.ResetContent();
	SetDefaultValues();
	if (m_Value.SelectString(-1, value) == CB_ERR)
		m_Value.SetWindowText(value);
}


void CDlgEditSymbols::OnEnKillfocusValue()
{
	CString s, v;
	m_Name.GetWindowText(s);
	m_Value.GetWindowText(v);

	if (s == "swagselectionmethod")
	{
		if (v != "Sgl Click" 
			&& v!= "Dbl Click" 
			&& v!= "Triple Click"
			&& v!= "Click Drag"
			&& v!= "Nothing")
			MessageBox(
				"Valid values for swagselectionmethod are:\n"
				"'Sgl Click', 'Dbl Click', 'Triple Click', 'Click Drag' and 'Nothing'.",
				"Invalid value", MB_OK);
	}
	else if (s == "swagdeletionmethod")
	{
		if (v!="Delete" && v!="Backspace")
			MessageBox("Valid values for swagdeletionmethod are:\n"
			           "'Delete' and 'Backspace'",
					   "Invalid value", MB_OK);	
	}

	else if (s=="swagconfirmationmethod")
	{
		if (v!="Enter" && v!="Click Bet")
			MessageBox("Valid values for swagconfirmationmethod are:\n"
			           "'Enter' and 'Click Bet'",
					   "Invalid value", MB_OK);	
	}

	else if (s=="buttonclickmethod")
	{
		if (v!="Single" && v!="Double")
			MessageBox("Valid values for buttonclickmethod are:\n"
			           "'Single' and 'Double'",
					   "Invalid value", MB_OK);	
	}
}

void CDlgEditSymbols::SetDefaultValues()
{
	m_Value.Clear();
	if (name.MakeLower() == "swagselectionmethod")
	{
		m_Value.AddString("Sgl Click");
		m_Value.AddString("Dbl Click");
		m_Value.AddString("Click Drag");
		m_Value.AddString("Nothing");
	}
	
	else if (name.MakeLower() == "swagdeletionmethod")
	{
		m_Value.AddString("Delete");
		m_Value.AddString("Backspace");
		m_Value.AddString("Nothing");
	}

	else if (name.MakeLower() == "swagconfirmationmethod")
	{
		m_Value.AddString("Enter");
		m_Value.AddString("Click Bet");
		m_Value.AddString("Nothing");
	}
	
	else if (name.MakeLower() == "buttonclickmethod")
	{
		m_Value.AddString("Single");
		m_Value.AddString("Double");
	}
	
	else if (name.MakeLower() == "balancenumbersonly")
	{
		m_Value.AddString("False");
		m_Value.AddString("True");
	}
	
	else if (name.MakeLower() == "chipscrapemethod")
	{
		m_Value.AddString("Original");
		m_Value.AddString("All");
		m_Value.AddString("XxY");
	}
}