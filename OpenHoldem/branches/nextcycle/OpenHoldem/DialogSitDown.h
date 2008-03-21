#pragma once
#include "afxwin.h"


// CDlgSitDown dialog

class CDlgSitDown : public CDialog
{
	DECLARE_DYNAMIC(CDlgSitDown)

public:
	CDlgSitDown(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSitDown();


// Dialog Data
	enum { IDD = IDD_SITDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_AvailableChairs;
	afx_msg void OnLbnDblclkAvailablechairs();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	int vacant[10];
	int nvacant;
	int m_selected_chair;
};
