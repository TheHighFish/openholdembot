#ifndef INC_DIALOGSAPREFS5_H
#define INC_DIALOGSAPREFS5_H

#include "resource.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs5 dialog

class CDlgSAPrefs5 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs5)

public:
	CDlgSAPrefs5(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs5();
	enum { IDD = IDD_SAPREFS5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK();

	CComboBox m_HandrankValue;
	CEdit m_AvTime;
	virtual BOOL OnInitDialog();
	CButton m_DisableCaching;

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS5_H