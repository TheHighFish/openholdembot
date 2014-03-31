#ifndef INC_DIALOGSAPREFS20_H
#define INC_DIALOGSAPREFS20_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs20 dialog

class CDlgSAPrefs20 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs20)

public:
	CDlgSAPrefs20(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs20();

// Dialog Data
	enum { IDD = IDD_SAPREFS20 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:  
	// Debugging
	CButton  	m_RadioButton_autoconnector;
	CButton  	m_RadioButton_autoplayer;
	CButton  	m_RadioButton_heartbeat;
	CButton  	m_RadioButton_prwin;
	CButton  	m_RadioButton_icm;
	CButton  	m_RadioButton_occlusionchecker;
	CButton  	m_RadioButton_pokertracker;
	CButton  	m_RadioButton_rebuy;
	CButton  	m_RadioButton_replayframes;
	CButton  	m_RadioButton_scraper;
	CButton  	m_RadioButton_sessioncounter;
	CButton  	m_RadioButton_stableframescounter;
	CButton  	m_RadioButton_symbolengine;
	CButton  	m_RadioButton_blindlocking;
	CButton		m_RadioButton_memorysymbols;
	CButton		m_RadioButton_dll_extension;
	CButton  	m_RadioButton_alltherest;
};

#endif