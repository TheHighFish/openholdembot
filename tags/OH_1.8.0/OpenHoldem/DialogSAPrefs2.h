#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "global.h"

// CDlgSAPrefs2 dialog

class CDlgSAPrefs2 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs2)

public:
	CDlgSAPrefs2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs2();

// Dialog Data
	enum { IDD = IDD_SAPREFS2 };

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDeltaposSwagdelay1Spin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSwagdelay2Spin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSwagdelay3Spin(NMHDR *pNMHDR, LRESULT *pResult);

	CEdit			m_FrameDelay, m_SwagDelay1, m_SwagDelay2, m_SwagDelay3;
	CSpinButtonCtrl m_FrameDelay_Spin, m_SwagDelay1_Spin, m_SwagDelay2_Spin, m_SwagDelay3_Spin;
	CComboBox		m_TextSelection, m_TextDeletion, m_BetConfirmation, m_ButtonClicking;
	CButton			m_AutoAP;

	DECLARE_MESSAGE_MAP()
};
