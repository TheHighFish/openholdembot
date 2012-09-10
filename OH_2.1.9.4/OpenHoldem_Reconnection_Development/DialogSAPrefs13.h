#ifndef INC_DIALOGSAPREFS13_H
#define INC_DIALOGSAPREFS13_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs12 dialog

class CDlgSAPrefs13 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs13)

public:
	CDlgSAPrefs13(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs13();

// Dialog Data
	enum { IDD = IDD_SAPREFS13 };
	CEdit	_class_name_edit;
	CEdit	_class_mutex_edit;
	CButton _simple_window_title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
};

#endif