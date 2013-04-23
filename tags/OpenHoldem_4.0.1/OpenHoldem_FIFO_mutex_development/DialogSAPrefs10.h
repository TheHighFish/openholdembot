#ifndef INC_DIALOGSAPREFS10_H
#define INC_DIALOGSAPREFS10_H

#include "resource.h"

#include "SAPrefsDialog.h"

// DialogSAPrefs10 dialog

class CDlgSAPrefs10 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs10)

public:
	CDlgSAPrefs10(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs10();

// Dialog Data
	enum { IDD = IDD_SAPREFS10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CEdit m_MinimumDelay;
	CSpinButtonCtrl m_MinimumDelay_Spin;
	CEdit m_RandomDelay;
	CSpinButtonCtrl m_RandomDelay_Spin;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
};


#endif //INC_DIALOGSAPREFS10_H