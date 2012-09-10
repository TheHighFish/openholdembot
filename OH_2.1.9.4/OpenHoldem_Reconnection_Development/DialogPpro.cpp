// Dlgp_pokerpro->cpp : implementation file
//

#include "stdafx.h"
#include <process.h>

#include "OpenHoldem.h"
#include "MainFrm.h"

#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CPokerPro.h"
#include "CAutoplayer.h"
#include "CFormula.h"

#include "DialogPPro.h"
#include "DialogSitDown.h"

// CDlgPpro dialog
CDlgPpro			*m_pproDlg;

IMPLEMENT_DYNAMIC(CDlgPpro, CDialog)

BEGIN_MESSAGE_MAP(CDlgPpro, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPpro::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPpro::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CDlgPpro::OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_JOINTABLE_BUTTON, &CDlgPpro::OnBnClickedJointableButton)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_SITDOWN_BUTTON, &CDlgPpro::OnBnClickedSitdownButton)
	ON_BN_CLICKED(IDC_BUYCHIPS_BUTTON, &CDlgPpro::OnBnClickedBuychipsButton)
	ON_BN_CLICKED(IDC_DEPOSIT_BUTTON, &CDlgPpro::OnBnClickedDepositButton)
	ON_LBN_DBLCLK(IDC_TABLE_LIST, &CDlgPpro::OnLbnDblclkTableList)
	ON_BN_CLICKED(IDC_SITIN_BUTTON, &CDlgPpro::OnBnClickedSitinButton)
END_MESSAGE_MAP()


void CDlgPpro::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PPRO_HOSTNAME, m_HostName);
	DDX_Control(pDX, IDC_PPRO_PORT, m_Port);
	DDX_Control(pDX, IDC_PPRO_USERNAME, m_UserName);
	DDX_Control(pDX, IDC_PPRO_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_PPRO_CHIPS, m_Chips);
	DDX_Control(pDX, IDC_PPRO_AUTOSEAT, m_AutoSeat);
	DDX_Control(pDX, IDC_CONNECT_STATUS, m_ConnectStatus);
	DDX_Control(pDX, IDC_LOGIN_STATUS, m_LoginStatus);
	DDX_Control(pDX, IDC_STATUS_TABLE, m_StatusTable);
	DDX_Control(pDX, IDC_STATUS_CHAIR, m_StatusChair);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_ConnectButton);
	DDX_Control(pDX, IDC_TABLE_LIST, m_TableList);
	DDX_Control(pDX, IDC_JOINTABLE_BUTTON, m_JoinTable_Button);
	DDX_Control(pDX, IDC_SITDOWN_BUTTON, m_SitDownButton);
	DDX_Control(pDX, IDC_BUYCHIPS_BUTTON, m_BuyChipsButton);
	DDX_Control(pDX, IDC_DEPOSIT_BUTTON, m_DepositButton);
	DDX_Control(pDX, IDC_CHIPSTOBUY, m_ChipsToBuy);
	DDX_Control(pDX, IDC_DEPOSITAMOUNT, m_DepositAmount);
	DDX_Control(pDX, IDC_SITIN_BUTTON, m_SitInButton);
	DDX_Control(pDX, IDC_PPRO_MESSAGES, m_PproMessages);
	DDX_Control(pDX, IDC_PPRO_HANDHISTORY, m_HandHistory);
	DDX_Control(pDX, IDC_PPRO_FULLTILTLOGFORMAT, m_FTHHLogFormat);
}

CDlgPpro::CDlgPpro(CWnd* pParent /*=NULL*/)	: CDialog(CDlgPpro::IDD, pParent) 
{
}

CDlgPpro::~CDlgPpro() 
{
}

BOOL CDlgPpro::OnInitDialog() 
{
	int			i = 0;
	int			max_x = 0, max_y = 0;

	CDialog::OnInitDialog();

	CString text = "";
	m_HostName.SetWindowText(prefs.ppro_hostname());
	m_Port.SetWindowText(prefs.ppro_port());
	m_UserName.SetWindowText(prefs.ppro_username());
	m_Password.SetWindowText(prefs.ppro_password());
	if (prefs.ppro_handhistory()) 
	{
		m_HandHistory.SetCheck(BST_CHECKED);
	}
	else 
	{
		m_HandHistory.SetCheck(BST_UNCHECKED);
	}
	if (prefs.ppro_ft_hhlog_format()) 
	{
		m_FTHHLogFormat.SetCheck(BST_CHECKED);
	}
	else 
	{
		m_FTHHLogFormat.SetCheck(BST_UNCHECKED);
	}
	text.Format("%d", prefs.ppro_chips());
	m_Chips.SetWindowText(text);
	if (prefs.ppro_autoseat()) 
	{
		m_AutoSeat.SetCheck(BST_CHECKED);
	}
	else 
	{
		m_AutoSeat.SetCheck(BST_UNCHECKED);
	}
	for (i=0; i<MAX_TABLES; i++) 
	{
		if (m_TableList.FindString(0, p_pokerpro->ppdata()->m_ginf[i].m_name) == LB_ERR &&
				strlen(p_pokerpro->ppdata()->m_ginf[i].m_name)>0) 
		{
			m_TableList.AddString(p_pokerpro->ppdata()->m_ginf[i].m_name);
		}
	}
	need_to_do_autoseat = false;
	need_to_do_autochips = false;
	messages_changed = false;

	// Restore window location
	if (prefs.ppro_x() != -1 && prefs.ppro_y() !=-1) 
	{
		max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		::SetWindowPos(m_hWnd, HWND_TOP, min(prefs.ppro_x(), max_x), min(prefs.ppro_y(), max_y),
					   prefs.ppro_dx(), prefs.ppro_dy(), SWP_NOCOPYBITS);
	}

	// Timer to keep things updated as socket messages are processed
	SetTimer(STATE_TIMER, 100, 0);

	// Check to see if we need to do autoseat
	SetTimer(AUTOSEAT_TIMER, 100, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgPpro::DestroyWindow() 
{
	KillTimer(STATE_TIMER);
	KillTimer(AUTOSEAT_TIMER);
	return CDialog::DestroyWindow();
}

// CDlgPpro message handlers
void CDlgPpro::OnBnClickedOk() 
{
	SaveSettingsToRegistry();
	OnOK();
}

void CDlgPpro::OnBnClickedCancel() 
{
	SaveSettingsToRegistry();
	OnCancel();
}

void CDlgPpro::OnBnClickedConnectButton() 
{
	CDlgPpro		dlgppro;
	CString			s = "";
	PokerMessage	pm;
	CString			text = "";
	CMainFrame		*cmf = (CMainFrame *)theApp.m_pMainWnd;

	// Disconnect if we are already connected
	if (p_pokerpro->IsConnected()) 
	{
		// stop threads
		if (p_heartbeat_thread)
		{
			delete p_heartbeat_thread;
			p_heartbeat_thread = NULL;
		}

		if (p_pokertracker_thread)
			p_pokertracker_thread->StopThread();

		// Make sure autoplayer is off
		p_autoplayer->set_autoplayer_engaged(false);

		// Do the disconnect
		p_pokerpro->Disconnect();
		p_pokerpro->set_ppdata_clear();
		p_pokerpro->set_ppdata_reset_userchair();

		// Enable buttons, menu items
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);
	}
	else 
	{
		SaveSettingsToRegistry();

		// connect
		if (p_pokerpro->Connect(prefs.ppro_hostname().GetString(), prefs.ppro_port().GetString()) < 0) {
			MessageBox("connection failed", "PPro connect error", MB_OK);
			return;
		}

		p_pokerpro->ResetHand();

		// send version info
		p_pokerpro->SendVersion();

		// log in
		int ret = p_pokerpro->SendLogin(prefs.ppro_username().GetString(), prefs.ppro_password().GetString());
		if ( ret < 0) 
		{
			MessageBox("login failed", "PPro connect error", MB_OK);
			return;
		}

		// Reset symbols
		p_symbols->ResetSymbolsFirstTime();

		// Disable buttons, menu items
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, false);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, false);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, false);
		cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);

		// Make sure autoplayer is off
		p_autoplayer->set_autoplayer_engaged(false);

		// start heartbeat thread
		if (p_heartbeat_thread)
		{
			delete p_heartbeat_thread;
			write_log(3, "Stopped heartbeat thread: %08x\n", p_heartbeat_thread);
			p_heartbeat_thread = NULL;
		}

		p_heartbeat_thread = new CHeartbeatThread;
		write_log(3, "Started heartbeat thread: %08x\n", p_heartbeat_thread);
	}
}

void CDlgPpro::OnBnClickedJointableButton() 
{
	// I'm not yet joined to a table, so try to join
	if (p_pokerpro->ppdata()->m_tinf.m_tid == 0) 
	{
		DoTableSelect();
	}

	// I'm joined, so unjoin
	else 
	{
		// log OH title bar text and table reset
		write_log(1, "%s - %s(NOT ATTACHED)\n", p_formula->formula_name().GetString(), p_pokerpro->ppdata()->m_site_name);
		write_log(1, "TABLE RESET\n*************************************************************\n");

		// Stop logging
		stop_log();

		p_pokerpro->SendGoto(0);
		p_pokerpro->ResetHand();
		p_pokerpro->set_ppdata_tinf_reset();
		p_pokerpro->set_ppdata_reset_userchair();
		p_pokerpro->set_ppdata_tinf_tid_reset();
	}
}

void CDlgPpro::OnBnClickedSitdownButton() 
{
	CDlgSitDown  dlg_sit;

	if (p_pokerpro->ppdata()->m_userchair==-1 && dlg_sit.DoModal() == IDOK) 
	{
		p_pokerpro->SendSit(dlg_sit.m_selected_chair);
	}
	else 
	{
		p_pokerpro->SendStand(p_pokerpro->ppdata()->m_userchair);
	}
}

void CDlgPpro::OnBnClickedBuychipsButton() 
{
	char		itemtext[100] = {0};

	m_ChipsToBuy.GetWindowText(itemtext, 100);
	if (atof(itemtext)>0) 
	{
		p_pokerpro->SendChips(atof(itemtext));
	}
}

void CDlgPpro::OnBnClickedDepositButton() 
{
	char		itemtext[100] = {0};

	m_DepositAmount.GetWindowText(itemtext, 100);

	if (atof(itemtext)>0) 
		p_pokerpro->SendDeposit(atof(itemtext));
}

void CDlgPpro::OnLbnDblclkTableList() 
{
	DoTableSelect();
}

void CDlgPpro::OnBnClickedSitinButton() 
{
	if (p_pokerpro->ppdata()->m_userchair!=-1) 
	{
		if (p_pokerpro->ppdata()->m_pinf[p_pokerpro->ppdata()->m_userchair].m_isActive&0x1) 
		{
			p_pokerpro->SendSitout(p_pokerpro->ppdata()->m_userchair);
		}
		else 
		{
			p_pokerpro->SendSitin(p_pokerpro->ppdata()->m_userchair);
		}
	}
}

void CDlgPpro::OnTimer(UINT nIDEvent) 
{
	CString			s = "";
	int				n = 0, i = 0, j = 0;
	bool			found = false;
	char			itemtext[100] = {0};
	HWND			hwnd_focus = NULL, hwnd_foreground = NULL, hwnd_active = NULL;

	if (nIDEvent == STATE_TIMER) 
	{

		// connect button
		if (!p_pokerpro->IsConnected()) 
		{
			m_ConnectButton.SetWindowText("Connect");
		}
		else 
		{
			m_ConnectButton.SetWindowText("Disconnect");
		}

		// join table button
		if ((!p_pokerpro->IsConnected() || m_TableList.GetCurSel()==LB_ERR) && p_pokerpro->ppdata()->m_tinf.m_tid == 0) 
		{
			m_JoinTable_Button.EnableWindow(false);
			m_JoinTable_Button.SetWindowTextA("Join Table");
		}
		else 
		{
			m_JoinTable_Button.EnableWindow(true);
			if (p_pokerpro->ppdata()->m_tinf.m_tid == 0) 
			{
				m_JoinTable_Button.SetWindowTextA("Join Table");
			}
			else 
			{
				m_JoinTable_Button.SetWindowTextA("Leave Table");
			}
		}

		// Sit down button
		if (!p_pokerpro->IsConnected()) 
		{
			m_SitDownButton.EnableWindow(false);
			m_SitDownButton.SetWindowText("Sit Down");
		}
		else if (p_pokerpro->ppdata()->m_tinf.m_tid==0) 
		{
			m_SitDownButton.EnableWindow(false);
			m_SitDownButton.SetWindowText("Sit Down");
		}
		else 
		{
			if (p_pokerpro->ppdata()->m_userchair==-1) 
			{
				m_SitDownButton.SetWindowText("Sit Down");
				if (m_AutoSeat.GetCheck()==BST_UNCHECKED || p_pokerpro->ppdata()->m_tinf.m_tid==0) 
				{
					m_SitDownButton.EnableWindow(true);
				}
				else 
				{
					m_SitDownButton.EnableWindow(false);
				}
			}
			else 
			{
				m_SitDownButton.SetWindowText("Stand Up");
				m_SitDownButton.EnableWindow(true);
			}
		}

		// Buy chips button and edit field
		if (p_pokerpro->ppdata()->m_isauthenticated == 0 || p_pokerpro->ppdata()->m_tinf.m_tid==0) 
		{
			m_BuyChipsButton.EnableWindow(false);
			m_ChipsToBuy.EnableWindow(false);
		}
		else 
		{
			m_BuyChipsButton.EnableWindow(true);
			m_ChipsToBuy.EnableWindow(true);
		}

		// Deposit button
		if (p_pokerpro->ppdata()->m_isauthenticated == 0) 
		{
			m_DepositButton.EnableWindow(false);
			m_DepositAmount.EnableWindow(false);
		}
		else 
		{
			m_DepositButton.EnableWindow(true);
			m_DepositAmount.EnableWindow(true);
		}

		// Sit in button
		if (p_pokerpro->ppdata()->m_isauthenticated == 0 || p_pokerpro->ppdata()->m_userchair==-1) 
		{
			m_SitInButton.EnableWindow(false);
		}
		else 
		{
			m_SitInButton.EnableWindow(true);
			if (p_pokerpro->ppdata()->m_userchair!=-1) 
			{
				if (p_pokerpro->ppdata()->m_pinf[p_pokerpro->ppdata()->m_userchair].m_isActive&0x1) 
				{
					m_SitInButton.SetWindowTextA("Sit Out");
				}
				else 
				{
					m_SitInButton.SetWindowTextA("Sit In");
				}
			}
		}

		// connection status
		if (!p_pokerpro->IsConnected()) 
		{
			m_ConnectStatus.SetWindowText("Connected to: Not Connected");
		}
		else 
		{
			s.Format("Connected to: %s", p_pokerpro->ppdata()->m_site_name);
			m_ConnectStatus.SetWindowText(s);
		}

		// logged in status
		if (p_pokerpro->ppdata()->m_isauthenticated == 0) 
		{
			m_LoginStatus.SetWindowText("Logged in as: Not Logged In");
		}
		else 
		{
			s.Format("Logged in as: %s", prefs.ppro_username().GetString());
			m_LoginStatus.SetWindowText(s);
		}

		// table status
		if (p_pokerpro->ppdata()->m_tinf.m_tid == 0) 
		{
			m_StatusTable.SetWindowTextA("Joined at table: Not Joined");
		}
		else 
		{
			s.Format("Joined at table: %s", p_pokerpro->ppdata()->m_ginf[p_pokerpro->ppdata()->m_tinf.m_tid].m_name);
			m_StatusTable.SetWindowText(s);
		}

		// seat status
		if (p_pokerpro->ppdata()->m_userchair==-1) 
		{
			m_StatusChair.SetWindowTextA("Seated in chair: Not Seated");
		}
		else 
		{
			s.Format("Seated in chair: %d", p_pokerpro->ppdata()->m_userchair);
			m_StatusChair.SetWindowText(s);
		}

		// table list, chips, autoseat
		if (p_pokerpro->ppdata()->m_isauthenticated==0 || p_pokerpro->ppdata()->m_tinf.m_tid!=0) 
		{
			m_TableList.EnableWindow(false);
			m_Chips.EnableWindow(false);
			m_AutoSeat.EnableWindow(false);
		}
		else 
		{
			m_TableList.EnableWindow(true);
			if (m_AutoSeat.GetCheck()==BST_CHECKED) 
			{
				m_Chips.EnableWindow(true);
			}
			else 
			{
				m_Chips.EnableWindow(false);
			}
			m_AutoSeat.EnableWindow(true);
			for (i=0; i<MAX_TABLES; i++) 
			{
				if (m_TableList.FindString(0, p_pokerpro->ppdata()->m_ginf[i].m_name) == LB_ERR &&
						strlen(p_pokerpro->ppdata()->m_ginf[i].m_name)>0) 
				{
					m_TableList.AddString(p_pokerpro->ppdata()->m_ginf[i].m_name);
				}
			}
			n = m_TableList.GetCount();
			for (i=0; i<n; i++) 
			{
				found = false;
				m_TableList.GetText(i, itemtext);
				for (j=0; j<MAX_TABLES; j++) 
				{
					if (strcmp(itemtext, p_pokerpro->ppdata()->m_ginf[j].m_name)==0) 
					{
						found = true;
						j = MAX_TABLES+1;
					}
				}
				if (!found) 
				{
					m_TableList.DeleteString(i);
					n = m_TableList.GetCount();
				}
			}
		}

		// hostname, port, username, password, handhistory
		if (!p_pokerpro->IsConnected()) 
		{
			m_HostName.EnableWindow(true);
			m_Port.EnableWindow(true);
			m_UserName.EnableWindow(true);
			m_Password.EnableWindow(true);
			m_HandHistory.EnableWindow(true);
			m_FTHHLogFormat.EnableWindow(true);
		}
		else 
		{
			m_HostName.EnableWindow(false);
			m_Port.EnableWindow(false);
			m_UserName.EnableWindow(false);
			m_Password.EnableWindow(false);
			m_HandHistory.EnableWindow(false);
			m_FTHHLogFormat.EnableWindow(false);
		}

		// Messages
		if (messages_changed) 
		{
			// Save the active window
			hwnd_focus = ::GetFocus();
			hwnd_foreground = ::GetForegroundWindow();
			hwnd_active = ::GetActiveWindow();

			// Update the dialog
			m_PproMessages.SetFocus();
			m_PproMessages.SetSel(0, -1);
			m_PproMessages.ReplaceSel(messages);

			// Restore the active window
			::SetActiveWindow(hwnd_active);
			::SetForegroundWindow(hwnd_foreground);
			::SetFocus(hwnd_focus);

			messages_changed = false;
		}

	}
	else if (nIDEvent == AUTOSEAT_TIMER) 
	{
		// Auto seat, if necessary
		if (need_to_do_autoseat==true &&
				p_pokerpro->ppdata()->m_tinf.m_tid!=0 &&
				p_pokerpro->ppdata()->m_userchair==-1) 
		{

			int r = p_pokerpro->GetRandomVacantChair();
			p_pokerpro->SendSit(r);
			need_to_do_autoseat = false;
		}

		// Auto chips, if necessary
		if (need_to_do_autochips==true &&
				p_pokerpro->ppdata()->m_tinf.m_tid!=0 &&
				p_pokerpro->ppdata()->m_userchair!=-1) 
		{

			m_Chips.GetWindowText(itemtext, 100);
			if (atof(itemtext)>0) 
			{
				p_pokerpro->SendChips(atof(itemtext));
			}
			need_to_do_autochips=false;
		}
	}
}

void CDlgPpro::DoTableSelect(void) 
{
	int		i = 0, table = 0;
	char	itemtext[100] = {0};

	if (m_TableList.GetCurSel()!=LB_ERR) 
	{
		m_TableList.GetText(m_TableList.GetCurSel(), itemtext);

		table = -1;
		for (i=0; i<MAX_TABLES; i++) 
		{
			if (strcmp(itemtext, p_pokerpro->ppdata()->m_ginf[i].m_name)==0) 
			{
				table = i;
				i = MAX_TABLES+1;
			}
		}

		if (table != -1) 
		{
			p_pokerpro->SendGoto(table);
		}
		if (m_AutoSeat.GetCheck()==BST_CHECKED) 
		{
			need_to_do_autoseat = true;
			m_Chips.GetWindowText(itemtext, 100);
			if (atoi(itemtext) > 0) 
			{
				need_to_do_autochips = true;
			}
		}

		// Start logging, in case the log-level got changed.
		start_log();

		write_log(1, "%s - %s(%s)\n", p_formula->formula_name().GetString(), p_pokerpro->ppdata()->m_site_name, p_pokerpro->ppdata()->m_tinf.m_name);
		write_log(1, "TABLE RESET\n*************************************************************\n");
	}
}

void CDlgPpro::SaveSettingsToRegistry(void) 
{
	CString			text = "";
	int				chips = 0;
	WINDOWPLACEMENT	wp;

	GetWindowPlacement(&wp);


	m_HostName.GetWindowText(text);
	prefs.set_ppro_hostname(text);

	m_Port.GetWindowText(text);
	prefs.set_ppro_port(text);

	m_UserName.GetWindowText(text);
	prefs.set_ppro_username(text);

	m_Password.GetWindowText(text);
	prefs.set_ppro_password(text);

	prefs.set_ppro_handhistory(m_HandHistory.GetCheck() == BST_CHECKED);
	prefs.set_ppro_ft_hhlog_format(m_FTHHLogFormat.GetCheck() == BST_CHECKED);
	
	m_Chips.GetWindowText(text);
	chips = strtoul(text.GetString(), NULL, 10);
	prefs.set_ppro_chips(chips);
	
	prefs.set_ppro_autoseat(m_AutoSeat.GetCheck() == BST_CHECKED);
	
	prefs.set_ppro_x(wp.rcNormalPosition.left);
	
	prefs.set_ppro_y(wp.rcNormalPosition.top);
	
	prefs.set_ppro_dx(wp.rcNormalPosition.right - wp.rcNormalPosition.left);
	
	prefs.set_ppro_dy(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
}
