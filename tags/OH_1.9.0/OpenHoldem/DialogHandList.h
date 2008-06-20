#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgHandList dialog

class CDlgHandList : public CDialog
{
public:
	CDlgHandList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHandList();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_HANDLIST };
	int		hand_list_num;
	bool	checked[13][13];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();

	CStatic m_HandList_Name;
	CButton m_Check[13][13];

	DECLARE_MESSAGE_MAP()
};
