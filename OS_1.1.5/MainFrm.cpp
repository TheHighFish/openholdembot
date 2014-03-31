// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OpenScrape.h"
#include "OpenScrapeDoc.h"
#include "OpenScrapeView.h"
#include "MainFrm.h"
#include "registry.h"
#include "DialogSelectTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Global variable to hold the startup path
char		startup_path[MAX_PATH];

// Global variable for holding table list by callback function
CArray <STableList, STableList>		g_tlist;


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CONNECTTOWINDOW, &CMainFrame::OnViewConnecttowindow)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_GREENCIRCLE, &CMainFrame::OnViewConnecttowindow)
	ON_COMMAND(ID_VIEW_SHOWREGIONBOXES, &CMainFrame::OnViewShowregionboxes)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_REDRECTANGLE, &CMainFrame::OnViewShowregionboxes)
	ON_COMMAND(ID_VIEW_REFRESH, &CMainFrame::OnViewRefresh)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_REFRESH, &CMainFrame::OnViewRefresh)

	ON_COMMAND(ID_EDIT_UPDATEHASHES, &CMainFrame::OnEditUpdatehashes)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWREGIONBOXES, &CMainFrame::OnUpdateViewShowregionboxes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURRENTWINDOWSIZE, &CMainFrame::OnUpdateViewCurrentwindowsize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// Save startup directory
	::GetCurrentDirectory(sizeof(startup_path) - 1, startup_path);

	show_regions = true;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	TBBUTTONINFO	tbi;
	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create toolbar
//	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
//		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
//		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	if (!m_wndToolBar.CreateEx(this, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Make formula button sticky
	m_wndToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_REDRECTANGLE, &tbi);
	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_REDRECTANGLE, show_regions);

	// Create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Set toolbar button status
	//m_wndToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Start timer that blinks selected region
	SetTimer(BLINKER_TIMER, 500, 0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	Registry	reg;
	int			max_x, max_y;

	WNDCLASS wnd;
	HINSTANCE hInst = AfxGetInstanceHandle();

	// Set class name
	if (!(::GetClassInfo(hInst, "OpenScrape", &wnd)))
	{
		wnd.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc      = ::DefWindowProc;
		wnd.cbClsExtra       = wnd.cbWndExtra = 0;
		wnd.hInstance        = hInst;
		wnd.hIcon            = AfxGetApp()->LoadIcon(IDI_ICON1);
		wnd.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wnd.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wnd.lpszMenuName     = NULL;
		wnd.lpszClassName    = "OpenScrape";

		AfxRegisterClass( &wnd );
	}  
	cs.lpszClass = "OpenScrape";

	// Restore window location and size
	reg.read_reg();
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	cs.x = min(reg.main_x, max_x);
	cs.y = min(reg.main_y, max_y);
	cs.cx = reg.main_dx;
	cs.cy = reg.main_dy;

	return true;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers
BOOL CMainFrame::DestroyWindow()
{
	Registry		reg;
	WINDOWPLACEMENT wp;

	// Save window position
	reg.read_reg();
	GetWindowPlacement(&wp);

	reg.main_x = wp.rcNormalPosition.left;
	reg.main_y = wp.rcNormalPosition.top;
	reg.main_dx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	reg.main_dy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
	reg.write_reg();

	// Close the TableMap dialog here so it can save its position properly
	if (theApp.m_TableMapDlg)  theApp.m_TableMapDlg->DestroyWindow();

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnViewConnecttowindow()
{
	LPARAM				lparam;
	int					i, N;
	CDlgSelectTable		cstd;
	STableList			tablelisthold;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	RECT				newrect;

	// Clear global list for holding table candidates
	g_tlist.RemoveAll();

	// Populate global candidate table list
	lparam = NULL;
	EnumWindows(EnumProcTopLevelWindowList, lparam);

	// Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
	if (N==0) 
	{
		MessageBox("No valid windows found", "Cannot find window", MB_OK);
	}
	else 
	{
		for (i=0; i<N; i++) 
		{
			tablelisthold.hwnd = g_tlist[i].hwnd;
			tablelisthold.title = g_tlist[i].title;
			tablelisthold.path = g_tlist[i].path;
			tablelisthold.crect.left = g_tlist[i].crect.left;
			tablelisthold.crect.top = g_tlist[i].crect.top;
			tablelisthold.crect.right = g_tlist[i].crect.right;
			tablelisthold.crect.bottom = g_tlist[i].crect.bottom;
			cstd.tlist.Add(tablelisthold);
		}

		// Display table select dialog
		if (cstd.DoModal() == IDOK) 
		{
			// Save hwnd and rect of window we are attached to
			pDoc->attached_hwnd = g_tlist[cstd.selected_item].hwnd;
			pDoc->attached_rect.left = g_tlist[cstd.selected_item].crect.left;
			pDoc->attached_rect.top = g_tlist[cstd.selected_item].crect.top;
			pDoc->attached_rect.right = g_tlist[cstd.selected_item].crect.right;
			pDoc->attached_rect.bottom = g_tlist[cstd.selected_item].crect.bottom;

			SaveBmpPbits();

			// Resize window
			::GetClientRect(pDoc->attached_hwnd, &newrect);
			AdjustWindowRect(&newrect, GetWindowLong(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), GWL_STYLE), true);
			SetWindowPos(NULL, 0, 0, newrect.right-newrect.left+4, newrect.bottom-newrect.top+47, SWP_NOMOVE);
		}
	}

	// Force re-draw
	Invalidate(true);
	theApp.m_TableMapDlg->Invalidate(true);
}


void CMainFrame::OnViewShowregionboxes()
{
	show_regions = !show_regions;

	m_wndToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_REDRECTANGLE, show_regions);

	// Force re-draw
	Invalidate(true);
}

void CMainFrame::OnEditUpdatehashes()
{
	int		ret;

	COpenScrapeDoc	*pDoc = COpenScrapeDoc::GetDocument();
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	ret = pDoc->trans.update_hashes(pMyMainWnd->GetSafeHwnd(), startup_path);

	// Redraw the tree
	theApp.m_TableMapDlg->update_tree("");

	if (ret == SUCCESS)  
		MessageBox("Hashes updated successfully.", "Success", MB_OK);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	COpenScrapeDoc			*pDoc = COpenScrapeDoc::GetDocument();
	COpenScrapeView			*pView = COpenScrapeView::GetView();

	if (nIDEvent == BLINKER_TIMER) 
	{
		pDoc->blink_on = !pDoc->blink_on;
		pView->blink_rect();

	}

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnViewRefresh()
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	RECT				crect, newrect;

	if (pDoc->attached_hwnd)
	{
		SaveBmpPbits();

		// Update saved rect
		::GetClientRect(pDoc->attached_hwnd, &crect);
		pDoc->attached_rect.left = crect.left;
		pDoc->attached_rect.top = crect.top;
		pDoc->attached_rect.right = crect.right;
		pDoc->attached_rect.bottom = crect.bottom;

		// Resize window
		::GetClientRect(pDoc->attached_hwnd, &newrect);
		AdjustWindowRect(&newrect, GetWindowLong(AfxGetApp()->m_pMainWnd->GetSafeHwnd(), GWL_STYLE), true);
		SetWindowPos(NULL, 0, 0, newrect.right-newrect.left+4, newrect.bottom-newrect.top+47, SWP_NOMOVE);

		// Force re-draw
		Invalidate(true);
		theApp.m_TableMapDlg->Invalidate(true);
	}

	else 
	{
		OnViewConnecttowindow();
	}
}

void CMainFrame::OnUpdateViewShowregionboxes(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(show_regions);
}

void CMainFrame::SaveBmpPbits(void)
{
	HDC					hdc;
	HDC					hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC					hdcCompatible = CreateCompatibleDC(hdcScreen); 
	HBITMAP				old_bitmap;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	int					width, height;

	// Clean up from a previous connect, if needed
	if (pDoc->attached_bitmap != NULL)
	{
		DeleteObject(pDoc->attached_bitmap);
		pDoc->attached_bitmap = NULL;
	}

	if (pDoc->attached_pBits) 
	{
		delete []pDoc->attached_pBits;
		pDoc->attached_pBits = NULL;
	}

	// Get DC for connected window
	hdc = ::GetDC(pDoc->attached_hwnd);

	// Save bitmap of connected window
	width = pDoc->attached_rect.right - pDoc->attached_rect.left;
	height = pDoc->attached_rect.bottom - pDoc->attached_rect.top;
	pDoc->attached_bitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, pDoc->attached_bitmap);
	BitBlt(hdcCompatible, 0, 0, width, height, hdc, 
		   pDoc->attached_rect.left, pDoc->attached_rect.top, SRCCOPY);

	// Get pBits of connected window
	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Populate BITMAPINFOHEADER
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, pDoc->attached_bitmap, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pDoc->attached_pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, pDoc->attached_bitmap, 0, height, pDoc->attached_pBits, bmi, DIB_RGB_COLORS);

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	SelectObject(hdcCompatible, old_bitmap);
	::ReleaseDC(pDoc->attached_hwnd, hdc);
	DeleteDC(hdcCompatible);
	DeleteDC(hdcScreen);

}


BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) {
	CString				title, winclass;
	char				text[512];
	RECT				crect;
	STableList			tablelisthold;

	// If this is not a top level window, then return
	if (GetParent(hwnd) != NULL) {
		return true;
	}

	// If this window is not visible, then return
	if (!IsWindowVisible(hwnd)) {
		return true;
	}

	// If there is no caption on this window, then return
	GetWindowText(hwnd, text, sizeof(text));
	if (strlen(text)==0) {
		return true;
	}
	title = text;

	// Found a window, get client area rect
	GetClientRect(hwnd, &crect);

	// Save it in the list
	tablelisthold.hwnd = hwnd;
	tablelisthold.title = title;
	tablelisthold.crect.left = crect.left;
	tablelisthold.crect.top = crect.top;
	tablelisthold.crect.right = crect.right;
	tablelisthold.crect.bottom = crect.bottom;
	g_tlist.Add(tablelisthold);

	return true;  // keep processing through entire list of windows
}


void CMainFrame::OnUpdateViewCurrentwindowsize(CCmdUI *pCmdUI)
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				text;

	if (pDoc->attached_hwnd)
	{
		text.Format("Current size: %dx%d", pDoc->attached_rect.right - pDoc->attached_rect.left, 
										   pDoc->attached_rect.bottom - pDoc->attached_rect.top);
		pCmdUI->SetText(text.GetString());
		pCmdUI->Enable(true);
	}

	else
	{
		pCmdUI->SetText("Current size: 0x0");
		pCmdUI->Enable(false);
	}
}
