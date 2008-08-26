// DlgPpro.cpp : implementation file
//

#include "stdafx.h"
#include <process.h>

#include "OpenHoldem.h"
#include "MainFrm.h"

#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CGlobal.h"

#include "DialogPpro.h"
#include "PokerPro.h"
#include "Registry.h"
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
}

CDlgPpro::CDlgPpro(CWnd* pParent /*=NULL*/)	: CDialog(CDlgPpro::IDD, pParent) 
{

    __SEH_SET_EXCEPTION_HANDLER


    __SEH_HEADER

    __SEH_LOGFATAL("CDlgPpro::Constructor : \n");
}

CDlgPpro::~CDlgPpro() 
{
    __SEH_HEADER

    __SEH_LOGFATAL("CDlgPpro::Destructor : \n");
}


BOOL CDlgPpro::OnInitDialog() 
{
    __SEH_HEADER
    int			i;
    Registry	reg;
    int			max_x, max_y;

    CDialog::OnInitDialog();

    CString text;
    m_HostName.SetWindowText(ppro.hostname);
    m_Port.SetWindowText(ppro.port);
    m_UserName.SetWindowText(ppro.username);
    m_Password.SetWindowText(ppro.password);
    if (ppro.handhistory) 
	{
        m_HandHistory.SetCheck(BST_CHECKED);
    }
    else 
	{
        m_HandHistory.SetCheck(BST_UNCHECKED);
    }
    text.Format("%d", ppro.chips);
    m_Chips.SetWindowText(text);
    if (ppro.autoseat) 
	{
        m_AutoSeat.SetCheck(BST_CHECKED);
    }
    else 
	{
        m_AutoSeat.SetCheck(BST_UNCHECKED);
    }
    for (i=0; i<MAX_TABLES; i++) 
	{
        if (m_TableList.FindString(0, ppro.data.m_ginf[i].m_name) == LB_ERR &&
                strlen(ppro.data.m_ginf[i].m_name)>0) 
		{
            m_TableList.AddString(ppro.data.m_ginf[i].m_name);
        }
    }
    need_to_do_autoseat = false;
    need_to_do_autochips = false;
    messages_changed = false;

    // Restore window location
    reg.read_reg();
    if (reg.ppro_x != -1 && reg.ppro_y!=-1) 
	{
        max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
        max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
        ::SetWindowPos(m_hWnd, HWND_TOP, min(reg.ppro_x, max_x), min(reg.ppro_y, max_y),
                       reg.ppro_dx, reg.ppro_dy, SWP_NOCOPYBITS);
    }

    // Timer to keep things updated as socket messages are processed
    SetTimer(STATE_TIMER, 100, 0);

    // Check to see if we need to do autoseat
    SetTimer(AUTOSEAT_TIMER, 100, 0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE

    __SEH_LOGFATAL("CDlgPpro::OnInitDialog : \n");
}

BOOL CDlgPpro::DestroyWindow() 
{
    __SEH_HEADER

    KillTimer(STATE_TIMER);
    KillTimer(AUTOSEAT_TIMER);
    return CDialog::DestroyWindow();

    __SEH_LOGFATAL("CDlgPpro::DestroyWindow : \n");
}


// CDlgPpro message handlers

void CDlgPpro::OnBnClickedOk() 
{
    __SEH_HEADER

    save_settings_to_reg();
    OnOK();

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedOk : \n");
}

void CDlgPpro::OnBnClickedCancel() 
{
    __SEH_HEADER

    save_settings_to_reg();
    OnCancel();

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedCancel : \n");


}

void CDlgPpro::OnBnClickedConnectButton() 
{
    __SEH_HEADER
    CDlgPpro		dlgppro;
    CString			s;
    PokerMessage	pm;
    Registry		reg;
    CString			text;
    CMainFrame		*cmf;

    cmf = (CMainFrame *)theApp.m_pMainWnd;

    // Disconnect if we are already connected
    if (ppro.m_socket != INVALID_SOCKET) 
	{
		// stop threads
		if (p_heartbeat_thread)
		{
			delete p_heartbeat_thread;
			p_heartbeat_thread = NULL;
		}
		if (p_pokertracker_thread)
		{
			delete p_pokertracker_thread;
			p_pokertracker_thread = NULL;
		}

		// Make sure autoplayer is off
        p_global->autoplay = false;

        // Do the disconnect
        ppro.disconnect();
        memset(&ppro.data, 0, sizeof(ppdata));
        ppro.data.m_userchair = -1;

        // Enable buttons, menu items
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);

        // Tell everything that we not connected
        p_global->ppro_is_connected = false;
    }
    else 
	{
        save_settings_to_reg();

        // connect
        if (ppro.connect(ppro.hostname.GetString(), ppro.port.GetString()) < 0) {
            MessageBox("connection failed", "PPro connect error", MB_OK);
            return;
        }

        ppro.reset_hand();

        // send version info
        ppro.send_version();

        // log in
        int ret = ppro.send_login(ppro.username.GetString(), ppro.password.GetString());
        if ( ret < 0) 
		{
            MessageBox("login failed", "PPro connect error", MB_OK);
            return;
        }

        // Tell everything that we are connected
        p_global->ppro_is_connected = true;

        // Reset symbols
        p_symbols->ResetSymbolsFirstTime();

        // Disable buttons, menu items
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, false);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, false);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, false);
        cmf->m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);

        // Make sure autoplayer is off
        p_global->autoplay = false;

		// start heartbeat thread
		if (p_heartbeat_thread)
		{
			delete p_heartbeat_thread;
			write_log("Stopped heartbeat thread: %08x\n", p_heartbeat_thread);
			p_heartbeat_thread = NULL;
		}

		p_heartbeat_thread = new CHeartbeatThread;
		write_log("Started heartbeat thread: %08x\n", p_heartbeat_thread);
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedConnectButton :\n");
}

void CDlgPpro::OnBnClickedJointableButton() 
{
    __SEH_HEADER
    // I'm not yet joined to a table, so try to join
    if (ppro.data.m_tinf.m_tid == 0) 
	{
        do_table_select();
    }

    // I'm joined, so unjoin
    else {
        // log OH title bar text and table reset
        write_log("%s - %s(NOT ATTACHED)\n", p_global->formula_name.GetString(), ppro.data.m_site_name);
        write_log("TABLE RESET\n*************************************************************\n");

        // Stop logging
        stop_log();

        ppro.send_goto(0);
        ppro.reset_hand();
        memset(&ppro.data.m_tinf, 0, sizeof(TINF));
        ppro.data.m_userchair = -1;
        ppro.data.m_tinf.m_tid = 0;
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedJointableButton :\n");
}

void CDlgPpro::OnBnClickedSitdownButton() 
{
    __SEH_HEADER
    CDlgSitDown  dlg_sit;

    if (ppro.data.m_userchair==-1 && dlg_sit.DoModal() == IDOK) 
	{
        ppro.send_sit(dlg_sit.m_selected_chair);
    }
    else 
	{
        ppro.send_stand(ppro.data.m_userchair);
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedSitdownButton :\n");
}

void CDlgPpro::OnBnClickedBuychipsButton() 
{
    __SEH_HEADER

    char		itemtext[100];

    m_ChipsToBuy.GetWindowText(itemtext, 100);
    if (atof(itemtext)>0) 
	{
        ppro.send_chips(atof(itemtext));
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedBuychipsButton :\n");
}

void CDlgPpro::OnBnClickedDepositButton() 
{
    __SEH_HEADER

    char		itemtext[100];

    m_DepositAmount.GetWindowText(itemtext, 100);
    if (atof(itemtext)>0) {
        ppro.send_deposit(atof(itemtext));
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedDepositButton :\n");
}

void CDlgPpro::OnLbnDblclkTableList() 
{
    __SEH_HEADER

    do_table_select();

    __SEH_LOGFATAL("CDlgPpro::OnLbnDblclkTableList :\n");
}

void CDlgPpro::OnBnClickedSitinButton() 
{
    __SEH_HEADER

    if (ppro.data.m_userchair!=-1) 
	{
        if (ppro.data.m_pinf[ppro.data.m_userchair].m_isActive&0x1) 
		{
            ppro.send_sitout(ppro.data.m_userchair);
        }
        else 
		{
            ppro.send_sitin(ppro.data.m_userchair);
        }
    }

    __SEH_LOGFATAL("CDlgPpro::OnBnClickedSitinButton :\n");
}

void CDlgPpro::OnTimer(UINT nIDEvent) 
{
    __SEH_HEADER
    CString			s;
    int				n, i, j;
    bool			found;
    char			itemtext[100];
    HWND			hwnd_focus, hwnd_foreground, hwnd_active;

    if (nIDEvent == STATE_TIMER) 
	{

        // connect button
        if (ppro.m_socket == INVALID_SOCKET) 
		{
            m_ConnectButton.SetWindowText("Connect");
        }
        else 
		{
            m_ConnectButton.SetWindowText("Disconnect");
        }

        // join table button
        if ((ppro.m_socket==INVALID_SOCKET || m_TableList.GetCurSel()==LB_ERR) &&
                ppro.data.m_tinf.m_tid == 0) 
		{
            m_JoinTable_Button.EnableWindow(false);
            m_JoinTable_Button.SetWindowTextA("Join Table");
        }
        else 
		{
            m_JoinTable_Button.EnableWindow(true);
            if (ppro.data.m_tinf.m_tid == 0) 
			{
                m_JoinTable_Button.SetWindowTextA("Join Table");
            }
            else 
			{
                m_JoinTable_Button.SetWindowTextA("Leave Table");
            }
        }

        // Sit down button
        if (ppro.m_socket==INVALID_SOCKET) 
		{
            m_SitDownButton.EnableWindow(false);
            m_SitDownButton.SetWindowText("Sit Down");
        }
        else if (ppro.data.m_tinf.m_tid==0) 
		{
            m_SitDownButton.EnableWindow(false);
            m_SitDownButton.SetWindowText("Sit Down");
        }
        else 
		{
            if (ppro.data.m_userchair==-1) 
			{
                m_SitDownButton.SetWindowText("Sit Down");
                if (m_AutoSeat.GetCheck()==BST_UNCHECKED || ppro.data.m_tinf.m_tid==0) 
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
        if (ppro.data.m_isauthenticated == 0 || ppro.data.m_tinf.m_tid==0) 
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
        if (ppro.data.m_isauthenticated == 0) 
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
        if (ppro.data.m_isauthenticated == 0 || ppro.data.m_userchair==-1) 
		{
            m_SitInButton.EnableWindow(false);
        }
        else 
		{
            m_SitInButton.EnableWindow(true);
            if (ppro.data.m_userchair!=-1) 
			{
                if (ppro.data.m_pinf[ppro.data.m_userchair].m_isActive&0x1) 
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
        if (ppro.m_socket == INVALID_SOCKET) 
		{
            m_ConnectStatus.SetWindowText("Connected to: Not Connected");
        }
        else 
		{
            s.Format("Connected to: %s", ppro.data.m_site_name);
            m_ConnectStatus.SetWindowText(s);
        }

        // logged in status
        if (ppro.data.m_isauthenticated == 0) 
		{
            m_LoginStatus.SetWindowText("Logged in as: Not Logged In");
        }
        else 
		{
            s.Format("Logged in as: %s", ppro.username.GetString());
            m_LoginStatus.SetWindowText(s);
        }

        // table status
        if (ppro.data.m_tinf.m_tid == 0) 
		{
            m_StatusTable.SetWindowTextA("Joined at table: Not Joined");
        }
        else 
		{
            s.Format("Joined at table: %s", ppro.data.m_ginf[ppro.data.m_tinf.m_tid].m_name);
            m_StatusTable.SetWindowText(s);
        }

        // seat status
        if (ppro.data.m_userchair==-1) 
		{
            m_StatusChair.SetWindowTextA("Seated in chair: Not Seated");
        }
        else 
		{
            s.Format("Seated in chair: %d", ppro.data.m_userchair);
            m_StatusChair.SetWindowText(s);
        }

        // table list, chips, autoseat
        if (ppro.data.m_isauthenticated==0 || ppro.data.m_tinf.m_tid!=0) 
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
                if (m_TableList.FindString(0, ppro.data.m_ginf[i].m_name) == LB_ERR &&
                        strlen(ppro.data.m_ginf[i].m_name)>0) 
				{
                    m_TableList.AddString(ppro.data.m_ginf[i].m_name);
                }
            }
            n = m_TableList.GetCount();
            for (i=0; i<n; i++) 
			{
                found = false;
                m_TableList.GetText(i, itemtext);
                for (j=0; j<MAX_TABLES; j++) 
				{
                    if (strcmp(itemtext, ppro.data.m_ginf[j].m_name)==0) 
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
        if (ppro.m_socket == INVALID_SOCKET) 
		{
            m_HostName.EnableWindow(true);
            m_Port.EnableWindow(true);
            m_UserName.EnableWindow(true);
            m_Password.EnableWindow(true);
            m_HandHistory.EnableWindow(true);
        }
        else 
		{
            m_HostName.EnableWindow(false);
            m_Port.EnableWindow(false);
            m_UserName.EnableWindow(false);
            m_Password.EnableWindow(false);
            m_HandHistory.EnableWindow(false);
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
                ppro.data.m_tinf.m_tid!=0 &&
                ppro.data.m_userchair==-1) 
		{

            int r = ppro.get_random_vacant_chair();
            ppro.send_sit(r);
            need_to_do_autoseat = false;
        }

        // Auto chips, if necessary
        if (need_to_do_autochips==true &&
                ppro.data.m_tinf.m_tid!=0 &&
                ppro.data.m_userchair!=-1) 
		{

            m_Chips.GetWindowText(itemtext, 100);
            if (atof(itemtext)>0) 
			{
                ppro.send_chips(atof(itemtext));
            }
            need_to_do_autochips=false;
        }
    }

    __SEH_LOGFATAL("CDlgPpro::OnTimer :\n");

}

void CDlgPpro::do_table_select(void) 
{
    __SEH_HEADER
    int		i, table;
    char	itemtext[100];

    if (m_TableList.GetCurSel()!=LB_ERR) 
	{
        m_TableList.GetText(m_TableList.GetCurSel(), itemtext);

        table = -1;
        for (i=0; i<MAX_TABLES; i++) 
		{
            if (strcmp(itemtext, ppro.data.m_ginf[i].m_name)==0) 
			{
                table = i;
                i = MAX_TABLES+1;
            }
        }

        if (table != -1) 
		{
            ppro.send_goto(table);
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

        // Start logging
        start_log();

        write_log("%s - %s(%s)\n", p_global->formula_name.GetString(), ppro.data.m_site_name, ppro.data.m_tinf.m_name);
        write_log("TABLE RESET\n*************************************************************\n");
    }

    __SEH_LOGFATAL("CDlgPpro::do_table_select :\n");
}

void CDlgPpro::save_settings_to_reg(void) 
{
    __SEH_HEADER
    Registry		reg;
    CString			text;
    WINDOWPLACEMENT	wp;

    GetWindowPlacement(&wp);

    m_HostName.GetWindowText(ppro.hostname);
    m_Port.GetWindowText(ppro.port);
    m_UserName.GetWindowText(ppro.username);
    m_Password.GetWindowText(ppro.password);
    ppro.handhistory = m_HandHistory.GetCheck() == BST_CHECKED;
    m_Chips.GetWindowText(text);
    ppro.chips = strtoul(text.GetString(), NULL, 10);
    m_AutoSeat.GetWindowText(text);
    ppro.autoseat = m_AutoSeat.GetCheck() == BST_CHECKED;

    reg.read_reg();
    reg.hostname = ppro.hostname;
    reg.port = ppro.port;
    reg.username = ppro.username;
    reg.password = ppro.password;
    reg.handhistory = ppro.handhistory;
    reg.chips = ppro.chips;
    reg.autoseat = ppro.autoseat;
    reg.ppro_x = wp.rcNormalPosition.left;
    reg.ppro_y = wp.rcNormalPosition.top;
    reg.ppro_dx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
    reg.ppro_dy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
    reg.write_reg();

    __SEH_LOGFATAL("CDlgPpro::save_settings_to_reg :\n");
}
