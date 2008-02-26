// SelectTableDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogSelectTable.h"
#include "debug.h"
#include "global.h"

// CDlgSelectTable dialog

IMPLEMENT_DYNAMIC(CDlgSelectTable, CDialog)

CDlgSelectTable::CDlgSelectTable(CWnd* pParent /*=NULL*/) : CDialog(CDlgSelectTable::IDD, pParent) {
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
		logfatal("CDlgSelectTable::Constructor :\n"); 
		throw;
	}
#endif
}

CDlgSelectTable::~CDlgSelectTable() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::Destructor :\n"); 
		throw;
	}
#endif
}

void CDlgSelectTable::DoDataExchange(CDataExchange* pDX) {
#ifdef SEH_ENABLE
	try {
#endif
		CDialog::DoDataExchange(pDX);
		//DDX_Control(pDX, IDC_LIST_SELECT_TABLE, m_List_Select_Table);
		DDX_Control(pDX, IDC_LIST_SELECT_TABLE, m_HSLB);
		DDX_Control(pDX, IDOK, m_OK);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::DoDataExchange :\n"); 
		throw;
	}
#endif
}


BEGIN_MESSAGE_MAP(CDlgSelectTable, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSelectTable::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST_SELECT_TABLE, &CDlgSelectTable::OnLbnDblclkListSelectTable)
	ON_LBN_SELCHANGE(IDC_LIST_SELECT_TABLE, &CDlgSelectTable::OnLbnSelchangeListSelectTable)
END_MESSAGE_MAP()


// CDlgSelectTable message handlers

void CDlgSelectTable::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		selected_item = m_HSLB.GetCurSel();
		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::OnBnClickedOk :\n"); 
		throw;
	}
#endif
}

BOOL CDlgSelectTable::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		int		i, N;

		CDialog::OnInitDialog();

		selected_item = -1;

		// Populate listbox
		N = (int) tlist.GetSize();
		for (i=0; i<N; i++) {
			m_HSLB.AddString(tlist[i].title);
		}

		m_OK.EnableWindow(false);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::OnInitDialog :\n"); 
		throw;
	}
#endif
}

void CDlgSelectTable::OnLbnDblclkListSelectTable() {
#ifdef SEH_ENABLE
	try {
#endif
		OnBnClickedOk();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::OnLbnDblclkListSelectTable :\n"); 
		throw;
	}
#endif
}

void CDlgSelectTable::OnLbnSelchangeListSelectTable() {
#ifdef SEH_ENABLE
	try {
#endif
		m_OK.EnableWindow(true);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSelectTable::OnLbnSelchangeListSelectTable :\n"); 
		throw;
	}
#endif
}
