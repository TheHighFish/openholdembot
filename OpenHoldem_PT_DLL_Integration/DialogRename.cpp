// RenameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogRename.h"
#include "OH_MessageBox.h"

// CDlgRename dialog

IMPLEMENT_DYNAMIC(CDlgRename, CDialog)

CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/) : CDialog(CDlgRename::IDD, pParent) 
{
}

CDlgRename::~CDlgRename() 
{
}

void CDlgRename::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLDNAME_EDIT, m_OldName);
	DDX_Control(pDX, IDC_NEWNAME_EDIT, m_NewName);
}

BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRename::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgRename::OnBnClickedCancel)
END_MESSAGE_MAP()

// CDlgRename message handlers
BOOL CDlgRename::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_OldName.SetWindowText(CSoldname);
	m_NewName.SetWindowText(CSoldname);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgRename::OnBnClickedOk() 
{
	char oldstr[512] = {0}, newstr[512] = {0};
	int i = 0;

	m_NewName.GetWindowText(CSnewname);
	strcpy_s(oldstr, 512, CSoldname.GetString());
	strcpy_s(newstr, 512, CSnewname.GetString());

	// Changing a list
	if (memcmp(oldstr, "list", 4)==0 && memcmp(newstr, "list", 4)!=0) 
	{
		OH_MessageBox_Interactive("Lists must begin with the name 'list'.\r\ne.g. 'list45'", "Invalid Name", MB_ICONERROR);
		return;
	}
	else if (memcmp(oldstr, "list", 4)==0 && memcmp(newstr, "list", 4)==0 && strlen(newstr)==4) 
	{
		OH_MessageBox_Interactive("Lists must begin with the name 'list'\r\nand be followed by a number.\r\ne.g. 'list45'", "Invalid Name", MB_ICONERROR);
		return;
	}
	else if (memcmp(oldstr, "list", 4)==0) 
	{
		for (int i=4; i<(int) strlen(newstr); i++) 
		{
			if (newstr[i]<'0' || newstr[i]>'9') 
			{
				OH_MessageBox_Interactive("Lists must begin with the name 'list'\r\nand have all numbers to end.\r\ne.g. 'list45'", "Invalid Name", MB_ICONERROR);
				return;
			}
		}
	}

	// Changing a UDF
	else if (memcmp(oldstr, "f$", 2)==0 && memcmp(newstr, "f$", 2)!=0) 
	{
		OH_MessageBox_Interactive("UDF's must begin with the name 'f$'.\r\ne.g. 'f$myfunc'", "Invalid Name", MB_ICONERROR);
		return;
	}
	else if (memcmp(oldstr, "f$", 2)==0 && strlen(newstr)==2) 
	{
		OH_MessageBox_Interactive("UDF's must begin with the name 'f$'\r\nand be followed be the name of the function.\r\ne.g. 'f$myfunc'", "Invalid Name", MB_ICONERROR);
		return;
	}

	OnOK();
}

void CDlgRename::OnBnClickedCancel() 
{
	OnCancel();
}

