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
	afx_msg void OnCheckClick(UINT controlID);

	CStatic m_HandList_Name, m_CommentST;
	CButton m_Check[13][13];

	int nhands;

	DECLARE_MESSAGE_MAP()
};
