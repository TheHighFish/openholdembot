#pragma once
#include "afxwin.h"
#include "resource.h"


// CDlgEditSymbols dialog

class CDlgEditSymbols : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgEditSymbols)

public:
	CDlgEditSymbols(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditSymbols();

	CString						titletext;
	CArray <CString, CString>	strings;
	CString						titlebartext, name, value;

// Dialog Data
	enum { IDD = IDD_EDIT_SYMBOLS };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedParsebutton();
	afx_msg void OnCbnSelchangeName();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CComboBox m_Name;
	CEdit m_Value;
	CEdit m_Titletext;
	CEdit m_ParseResults;
	CButton m_ParseButton;
};
