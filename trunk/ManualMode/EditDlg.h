#pragma once
#include "afxwin.h"
#include "resource.h"

// CEditDlg dialog

class CEditDlg : public CDialog {
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditDlg();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	enum { IDD = IDD_EDITDLG };
	CString		m_titletext;
	CString		m_result;
	CEdit		m_EditEntry;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
