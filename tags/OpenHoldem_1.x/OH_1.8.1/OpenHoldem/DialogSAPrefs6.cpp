// DialogSAPrefs6.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "global.h"
#include "Registry.h"
#include "debug.h"
#include "pokertracker.h"
#include "OpenHoldem.h"
#include "MainFrm.h"

#include "DialogSAPrefs6.h"

// CDlgSAPrefs6 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs6, CSAPrefsSubDlg)

CDlgSAPrefs6::CDlgSAPrefs6(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs6::IDD, pParent)
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
}

CDlgSAPrefs6::~CDlgSAPrefs6()
{
}

void CDlgSAPrefs6::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
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
	CString text;

	CSAPrefsSubDlg::OnInitDialog();

	m_pt_disable.SetCheck(PT.disable == false ? BST_UNCHECKED : BST_CHECKED);

	m_pt_ip.SetWindowText(PT.ip_addr.GetString());
	m_pt_port.SetWindowText(PT.port.GetString());
	m_pt_user.SetWindowText(PT.user.GetString());
	m_pt_pass.SetWindowText(PT.pass.GetString());
	m_pt_dbname.SetWindowText(PT.dbname.GetString());

	text.Format("%d", PT.update_delay);
	m_UpdateDelay.SetWindowText(text);
	m_UpdateDelay_Spin.SetRange(1, 120);
	m_UpdateDelay_Spin.SetPos(PT.update_delay);
	m_UpdateDelay_Spin.SetBuddy(&m_UpdateDelay);

	text.Format("%d", PT.cache_refresh);
	m_CacheRefresh.SetWindowText(text);
	m_CacheRefresh_Spin.SetRange(15, 240);
	m_CacheRefresh_Spin.SetPos(PT.cache_refresh);
	m_CacheRefresh_Spin.SetBuddy(&m_CacheRefresh);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs6::OnOK()
{
	Registry		reg;
	CString			text;

	PT.disable = m_pt_disable.GetCheck() == BST_CHECKED ? true : false;
	m_pt_ip.GetWindowText(PT.ip_addr);
	m_pt_port.GetWindowText(PT.port);
	m_pt_user.GetWindowText(PT.user);
	m_pt_pass.GetWindowText(PT.pass);
	m_pt_dbname.GetWindowText(PT.dbname);

	m_UpdateDelay.GetWindowText(text);
	PT.update_delay = atoi(text.GetString());

	m_CacheRefresh.GetWindowText(text);
	PT.cache_refresh = atoi(text.GetString());

	PT.conn_str = "host=" + PT.ip_addr;
	PT.conn_str += " port=" + PT.port;
	PT.conn_str += " user=" + PT.user;
	PT.conn_str += " password=" + PT.pass;
	PT.conn_str += " dbname=" + PT.dbname;

	reg.read_reg();
	reg.pt_disable = PT.disable;
	reg.pt_ip_addr = PT.ip_addr;
	reg.pt_port = PT.port;
	reg.pt_user = PT.user;
	reg.pt_pass = PT.pass;
	reg.pt_dbname = PT.dbname;
	reg.pt_updatedelay = PT.update_delay;
	reg.pt_cacherefresh = PT.cache_refresh;
	reg.write_reg();

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs6::OnBnClickedPtTest()
{
#ifdef SEH_ENABLE
	try {
#endif
		CString			conn_str, ip_addr, port, user, pass, dbname, e;
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
		conn_str += " dbname=" + dbname;

		// Set busy cursor
		global.m_WaitCursor = true;
		pMyMainWnd->BeginWaitCursor();

		// Test the connection parameters
		PGconn	*pgconn = PQconnectdb(conn_str.GetString());

		// Unset busy cursor	
		pMyMainWnd->EndWaitCursor();
		global.m_WaitCursor = false;

		if (PQstatus(pgconn) == CONNECTION_OK) {
			write_log("PostgreSQL DB opened successfully <%s/%s/%s>\n", ip_addr, port, dbname);
			if (PQisthreadsafe()) { 
				write_log("PostgreSQL library is thread safe.\n\n"); 
				MessageBox("PostgreSQL DB opened successfully", "Success", MB_OK);
			}
			else { 
				write_log("PostgreSQL library is *NOT* thread safe!  This is a problem!\n\n"); 
				MessageBox("PostgreSQL DB opened successfully, but\nPostgreSQL library is *NOT* thread safe!\nThis is a problem!", 
					"Success (partial)", MB_OK);						
			}
			PQfinish(pgconn);
		} 
		else {
			write_log("ERROR opening PostgreSQL DB: %s\n\n", PQerrorMessage(pgconn));
			e = "ERROR opening PostgreSQL DB:\n";
			e += PQerrorMessage(pgconn);
			e += "\nConn string:";
			e += conn_str;
			MessageBox(e.GetString(), "ERROR", MB_OK);						
			PQfinish(pgconn);
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgSAPrefs6::OnBnClickedPtTest :\n"); 
		throw;
	}
#endif
}
