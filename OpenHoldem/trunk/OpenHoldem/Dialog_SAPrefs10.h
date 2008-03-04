#pragma once


// Dialog_SAPrefs10 dialog

class Dialog_SAPrefs10 : public CDialog
{
	DECLARE_DYNAMIC(Dialog_SAPrefs10)

public:
	Dialog_SAPrefs10(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_SAPrefs10();

// Dialog Data
	enum { IDD = IDD_SAPREFS10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
