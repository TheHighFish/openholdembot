#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgEditHashPoint dialog

class CDlgEditHashPoint : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgEditHashPoint)

public:
	CDlgEditHashPoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditHashPoint();

	CString						titletext;
	CString						type;
	int							x, y;

// Dialog Data
	enum { IDD = IDD_EDIT_HASHPOINTS };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CComboBox m_Type;
	CEdit m_X;
	CEdit m_Y;
};
