#ifndef INC_DIALOGSAPREFS12_H
#define INC_DIALOGSAPREFS12_H

#include "resource.h"
#include "afxwin.h"


// CDlgSAPrefs12 dialog

class CDlgSAPrefs12 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs12)

public:
	CDlgSAPrefs12(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs12();

// Dialog Data
	enum { IDD = IDD_SAPREFS12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
	void OnOK();

protected:
	CButton  	m_RadioButton_Disable;
	/*
	CButton  	m_RadioButton_Enable_My_Turn;
	CButton  	m_RadioButton_Enable_Always;*/
	CButton		m_Button_Stop_On_Error;   
};

#endif