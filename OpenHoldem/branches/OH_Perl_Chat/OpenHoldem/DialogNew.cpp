// NewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogNew.h"
#include "debug.h"
#include "global.h"

// CDlgNew dialog

IMPLEMENT_DYNAMIC(CDlgNew, CDialog)

CDlgNew::CDlgNew(CWnd* pParent /*=NULL*/) : CDialog(CDlgNew::IDD, pParent) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::Constructor :\n"); 
		throw;
	}
#endif
}

CDlgNew::~CDlgNew() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::Destructor :\n"); 
		throw;
	}
#endif
}

void CDlgNew::DoDataExchange(CDataExchange* pDX) {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_NEWNAME_EDIT, m_NewName);
		DDX_Control(pDX, IDC_DESC, m_Desc);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::DoDataExchange :\n"); 
		throw;
	}
#endif
}


BEGIN_MESSAGE_MAP(CDlgNew, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNew::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgNew::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgNew message handlers
BOOL CDlgNew::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::OnInitDialog();

		if (type==0) {
			m_NewName.SetWindowText("list");
			m_Desc.SetWindowText("New list name:");
			::SetWindowText(m_hWnd, "New List");
		}
		else {
			m_NewName.SetWindowText("f$");
			m_Desc.SetWindowText("New UDF name:");
			::SetWindowText(m_hWnd, "New UDF");
		}

		return TRUE;  // return TRUE unless you set the focus to a control
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::OnInitDialog :\n"); 
		throw;
	}
#endif
}

void CDlgNew::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		char str[512];
		int i;

		m_NewName.GetWindowText(CSnewname);
		strcpy(str, CSnewname.GetString());
		
		// Changing a list
		if (type==0 && memcmp(str, "list", 4)!=0) {
			MessageBox("Lists must begin with the name 'list'.\r\ne.g. 'list45'", "Invalid Name", MB_ICONERROR);
			return;
		}
		else if (type==0 && memcmp(str, "list", 4)==0 && strlen(str)==4) {
			MessageBox("Lists must begin with the name 'list'\r\nand be followed by a number.\r\ne.g. 'list45'", "Invalid Name", MB_ICONERROR);
			return;
		}
		else if (type==0) {
			for (i=4; i<(int) strlen(str); i++) {
				if (str[i]<'0' || str[i]>'9') {
					MessageBox("Lists must begin with the name 'list'\r\nand have all numbers to end.  e.g. 'list45'", "Invalid Name", MB_ICONERROR);
					return;
				}
			}
		}

		// Changing a UDF
		else if (type==1 && memcmp(str, "f$", 2)!=0) {
			MessageBox("UDF's must begin with the name 'f$'\r\ne.g. 'f$myfunc'", "Invalid Name", MB_ICONERROR);
			return;
		}
		else if (type==1 && strlen(str)==2) {
			MessageBox("UDF's must begin with the name 'f$'\r\nand be followed be the name of the function.\r\ne.g. 'f$myfunc'", "Invalid Name", MB_ICONERROR);
			return;
		}

		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::OnBnClickedOk :\n"); 
		throw;
	}
#endif
}

void CDlgNew::OnBnClickedCancel() {
#ifdef SEH_ENABLE
	try {
#endif
		OnCancel();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgNew::OnBnClickedCancel : \n"); 
		throw;
	}
#endif
}

