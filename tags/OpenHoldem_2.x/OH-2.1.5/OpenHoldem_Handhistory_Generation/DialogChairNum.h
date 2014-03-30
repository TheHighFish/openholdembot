#pragma once
#include "afxwin.h"
#include "Resource.h"

// CDlgChairNum dialog

class CDlgChairNum : public CDialog
{
	DECLARE_DYNAMIC(CDlgChairNum)

public:
	CDlgChairNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgChairNum();

// Dialog Data
	enum { IDD = IDD_CHARINUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CEdit m_ChairNum;
	int	chair;
};
