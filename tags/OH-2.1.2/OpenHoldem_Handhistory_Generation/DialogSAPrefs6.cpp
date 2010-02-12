// DialogSAPrefs6.cpp : implementation file
//

#include "stdafx.h"

#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "DialogSAPrefs6.h"
#include "OpenHoldem.h"
#include "MainFrm.h"
#include "SAPrefsSubDlg.h"


// CDlgSAPrefs6 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs6, CSAPrefsSubDlg)

CDlgSAPrefs6::CDlgSAPrefs6(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs6::IDD, pParent)
{
}

CDlgSAPrefs6::~CDlgSAPrefs6()
{
}

void CDlgSAPrefs6::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PT_VERSION, m_pt_version);
	DDX_Control(pDX, IDC_PT_IP, m_pt_ip);
	DDX_Control(pDX, IDC_PT_PORT, m_pt_port);
	DDX_Control(pDX, IDC_PT_USER, m_pt_user);
	DDX_Control(pDX, IDC_PT_PASS, m_pt_pass);
	DDX_Control(pDX, IDC_PT_DBNAME, m_pt_dbname);
	DDX_Control(pDX, IDC_UPDATEDELAY, m_UpdateDelay);
	DDX_Control(pDX, IDC_UPDATEDELAY_SPIN, m_UpdateDelay_Spin);
	DDX_Control(pDX, IDC_CACHEREFRESH, m_CacheRefresh);
	DDX_Control(pDX, IDC_CACHEREFRESH_SPIN, m_CacheRefresh_Spin);
	DDX_Control(pDX, IDC_PT_DISABLE, m_pt_disable);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs6, CSAPrefsSubDlg)
	ON_BN_CLICKED(IDC_PT_TEST, &CDlgSAPrefs6::OnBnClickedPtTest)
END_MESSAGE_MAP()

// CDlgSAPrefs6 message handlers
BOOL CDlgSAPrefs6::OnInitDialog()
{
	CString		text = "";

	CSAPrefsSubDlg::OnInitDialog();

	m_pt_disable.SetCheck(!prefs.pt_disable() ? BST_UNCHECKED : BST_CHECKED);

	m_pt_version.AddString("2");
	m_pt_version.AddString("3");
	if (prefs.pt_version() == "2" || prefs.pt_version() == "3")
		m_pt_version.SelectString(0, prefs.pt_version().GetString());
	else
		m_pt_version.SelectString(0, "2");

	m_pt_ip.SetWindowText(prefs.pt_ip_addr().GetString());
	m_pt_port.SetWindowText(prefs.pt_port().GetString());
	m_pt_user.SetWindowText(prefs.pt_user().GetString());
	m_pt_pass.SetWindowText(prefs.pt_pass().GetString());
	m_pt_dbname.SetWindowText(prefs.pt_dbname().GetString());

	text.Format("%d", prefs.pt_update_delay());
	m_UpdateDelay.SetWindowText(text);
	m_UpdateDelay_Spin.SetRange(1, 120);
	m_UpdateDelay_Spin.SetPos(prefs.pt_update_delay());
	m_UpdateDelay_Spin.SetBuddy(&m_UpdateDelay);

	text.Format("%d", prefs.pt_cache_refresh());
	m_CacheRefresh.SetWindowText(text);
	m_CacheRefresh_Spin.SetRange(15, 240);
	m_CacheRefresh_Spin.SetPos(prefs.pt_cache_refresh());
	m_CacheRefresh_Spin.SetBuddy(&m_CacheRefresh);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs6::OnOK()
{
	CString			text = "";

	prefs.set_pt_disable(m_pt_disable.GetCheck() == BST_CHECKED ? true : false);

	m_pt_version.GetWindowText(text);
	prefs.set_pt_version(text);

	m_pt_ip.GetWindowText(text);
	prefs.set_pt_ip_addr(text);

	m_pt_port.GetWindowText(text);
	prefs.set_pt_port(text);

	m_pt_user.GetWindowText(text);
	prefs.set_pt_user(text);

	m_pt_pass.GetWindowText(text);
	prefs.set_pt_pass(text);

	m_pt_dbname.GetWindowText(text);
	prefs.set_pt_dbname(text);

	m_UpdateDelay.GetWindowText(text);
	prefs.set_pt_update_delay(atoi(text.GetString()));

	m_CacheRefresh.GetWindowText(text);
	prefs.set_pt_cache_refresh(atoi(text.GetString()));

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs6::OnBnClickedPtTest()
{
	CString			conn_str = "", ip_addr = "", port = "", user = "", pass = "", dbname = "", e = "";
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	m_pt_ip.GetWindowText(ip_addr);
	m_pt_port.GetWindowText(port);
	m_pt_user.GetWindowText(user);
	m_pt_pass.GetWindowText(pass);
	m_pt_dbname.GetWindowText(dbname);

	conn_str = "host=" + ip_addr;
	conn_str += " port=" + port;
	conn_str += " user=" + user;
	conn_str += " password=" + pass;
	conn_str += " dbname='" + dbname;
	conn_str += "'";

	// Set busy cursor
	pMyMainWnd->set_wait_cursor(true);
	pMyMainWnd->BeginWaitCursor();

	// Test the connection parameters
	PGconn	*pgconn = PQconnectdb(conn_str.GetString());

	// Unset busy cursor
	pMyMainWnd->EndWaitCursor();
	pMyMainWnd->set_wait_cursor(false);

	if (PQstatus(pgconn) == CONNECTION_OK) 
	{
		write_log_pokertracker(1, "Test: PostgreSQL DB opened successfully <%s/%s/%s>\n", ip_addr, port, dbname);
		if (PQisthreadsafe()) 
		{
			write_log_pokertracker(1, "Test: PostgreSQL library is thread safe.\n\n");
			MessageBox("PostgreSQL DB opened successfully", "Success", MB_OK);
		}
		else 
		{
			write_log_pokertracker(1, "Test: PostgreSQL library is *NOT* thread safe!  This is a problem!\n\n");
			MessageBox("PostgreSQL DB opened successfully, but\nPostgreSQL library is *NOT* thread safe!\nThis is a problem!",
					   "Success (partial)", MB_OK);
		}
		PQfinish(pgconn);
	}
	else 
	{
		write_log_pokertracker(1, "Test: ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(pgconn));
		e = "ERROR opening PostgreSQL DB:\n";
		e += PQerrorMessage(pgconn);
		e += "\nConn string:";
		e += conn_str;

		MessageBox(e.GetString(), "ERROR", MB_OK);

		PQfinish(pgconn);
	}
}
