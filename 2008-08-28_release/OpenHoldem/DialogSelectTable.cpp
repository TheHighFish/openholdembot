// SelectTableDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "DialogSelectTable.h"

// CDlgSelectTable dialog

IMPLEMENT_DYNAMIC(CDlgSelectTable, CDialog)

CDlgSelectTable::CDlgSelectTable(CWnd* pParent /*=NULL*/) : CDialog(CDlgSelectTable::IDD, pParent) 
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSelectTable::Constructor :\n");
}

CDlgSelectTable::~CDlgSelectTable() 
{
    __SEH_HEADER

    __SEH_LOGFATAL("CDlgSelectTable::Destructor :\n");
}

void CDlgSelectTable::DoDataExchange(CDataExchange* pDX) 
{
    __SEH_HEADER

    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_LIST_SELECT_TABLE, m_List_Select_Table);
    DDX_Control(pDX, IDC_LIST_SELECT_TABLE, m_HSLB);
    DDX_Control(pDX, IDOK, m_OK);

    __SEH_LOGFATAL("CDlgSelectTable::DoDataExchange :\n");
}


BEGIN_MESSAGE_MAP(CDlgSelectTable, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgSelectTable::OnBnClickedOk)
    ON_LBN_DBLCLK(IDC_LIST_SELECT_TABLE, &CDlgSelectTable::OnLbnDblclkListSelectTable)
    ON_LBN_SELCHANGE(IDC_LIST_SELECT_TABLE, &CDlgSelectTable::OnLbnSelchangeListSelectTable)
END_MESSAGE_MAP()


// CDlgSelectTable message handlers

void CDlgSelectTable::OnBnClickedOk() 
{
    __SEH_HEADER

    selected_item = m_HSLB.GetCurSel();
    OnOK();

    __SEH_LOGFATAL("CDlgSelectTable::OnBnClickedOk :\n");
}

BOOL CDlgSelectTable::OnInitDialog() 
{
    __SEH_HEADER

    int			i, N;
	CString		t;

	CDialog::OnInitDialog();

    selected_item = -1;

    // Populate listbox
    N = (int) tlist.GetSize();
    for (i=0; i<N; i++)
	{
		t = tlist[i].title + " {" + tlist[i].path + "}";
        m_HSLB.AddString(t);
	}

    m_OK.EnableWindow(false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE

    __SEH_LOGFATAL("CDlgSelectTable::OnInitDialog :\n");

}

void CDlgSelectTable::OnLbnDblclkListSelectTable() 
{
    __SEH_HEADER

	OnBnClickedOk();

    __SEH_LOGFATAL("CDlgSelectTable::OnLbnDblclkListSelectTable :\n");

}

void CDlgSelectTable::OnLbnSelchangeListSelectTable() 
{
    __SEH_HEADER

    m_OK.EnableWindow(true);

    __SEH_LOGFATAL("CDlgSelectTable::OnLbnSelchangeListSelectTable :\n");
}
