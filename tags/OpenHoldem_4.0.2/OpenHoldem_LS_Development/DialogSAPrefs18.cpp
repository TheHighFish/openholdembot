// DialogSAPrefs1.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSAPrefs18.h"

#include "SAPrefsSubDlg.h"
#include "CPreferences.h"

// CDlgSAPrefs18 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs18, CSAPrefsSubDlg)

CDlgSAPrefs18::CDlgSAPrefs18(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs18::IDD, pParent)
{
}

CDlgSAPrefs18::~CDlgSAPrefs18()
{
}

void CDlgSAPrefs18::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAZY_SCRAPER_WHEN_MY_TURN, _scrape_when_my_turn_button);
	DDX_Control(pDX, IDC_LAZY_SCRAPER_WHEN_SITIN_SITOUT_LEAVE, _scrape_when_sitin_sitout_leave_button);
	DDX_Control(pDX, IDC_LAZY_SCRAPER_WHEN_WE_HOLD_CARDS, _scrape_when_cards_button);
	DDX_Control(pDX, IDC_LAZY_SCRAPER_ALWAYS_FULL_SCRAPE, _scrape_always_button);
	
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs18, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs1 message handlers
BOOL CDlgSAPrefs18::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	// Always true (and disabled)
	// Just to show complete information.
	_scrape_when_my_turn_button.SetCheck(true);
	if (prefs.lazy_scraping_when_sitin_sitout_leave())
	{
		_scrape_when_sitin_sitout_leave_button.SetCheck(true);
	}
	if (prefs.lazy_scraping_when_we_hold_cards())
	{
		_scrape_when_cards_button.SetCheck(true);
	}
	if (prefs.lazy_scraping_always_complete())
	{
		_scrape_always_button.SetCheck(true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs18::OnOK()
{

	// Don't care about the value of _scrape_when_my_turn_button
	// It is always set and can't be changed.
	prefs.set_lazy_scraping_when_sitin_sitout_leave(_scrape_when_sitin_sitout_leave_button.GetCheck() == true);
	prefs.set_lazy_scraping_when_we_hold_cards(_scrape_when_cards_button.GetCheck() == true);
	prefs.set_lazy_scraping_always_complete(_scrape_always_button.GetCheck() == true);

	CSAPrefsSubDlg::OnOK();
}
