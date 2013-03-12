#ifndef INC_DIALOGHANDLIST_H
#define INC_DIALOGHANDLIST_H

#include "resource.h"

// CDlgHandList dialog

class CDlgHandList : public CDialog
{
public:
	CDlgHandList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHandList();
	virtual BOOL OnInitDialog();
	CString GetHandListAsString();

	enum { IDD = IDD_HANDLIST };
	int		hand_list_num;
	bool	checked[13][13];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnCheckClick(UINT controlID);
	afx_msg void OnBnClickedPP();
	afx_msg void OnBnClickedBroadways();
	afx_msg void OnBnClickedSuited();
	afx_msg void OnBnClickedClear();

	CStatic m_HandList_Name, m_CommentST;
	CButton m_Check[13][13];

	int nhands;

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGHANDLIST_H