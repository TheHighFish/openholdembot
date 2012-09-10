#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgEdit dialog
class CDlgEdit : public CDialog 
{
	DECLARE_DYNAMIC(CDlgEdit)

public:
	CDlgEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEdit();
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
