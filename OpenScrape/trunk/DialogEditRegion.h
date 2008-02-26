#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgEditRegion dialog

class CDlgEditRegion : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditRegion)

public:
	CDlgEditRegion(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditRegion();

	CString						titletext;
	CArray <CString, CString>	strings;
	
	CString						name;

// Dialog Data
	enum { IDD = IDD_EDIT_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Name;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
