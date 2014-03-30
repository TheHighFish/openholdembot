#ifndef INC_DIALOGSELECTTABLE_H
#define INC_DIALOGSELECTTABLE_H

#include "HScrollListBox.h"

// CDlgSelectTable dialog


class CDlgSelectTable : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectTable)

public:
	CDlgSelectTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectTable();

	CArray <STableList, STableList>		tlist;

// Dialog Data
	enum { IDD = IDD_SELECTTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//CListBox m_List_Select_Table;
	CHScrollListBox m_HSLB;
	int	selected_item;

protected:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLbnDblclkListSelectTable();
	CButton m_OK;
	afx_msg void OnLbnSelchangeListSelectTable();
};


#endif //INC_DIALOGSELECTTABLE_H