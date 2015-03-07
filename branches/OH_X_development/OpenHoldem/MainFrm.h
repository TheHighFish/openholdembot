#ifndef INC_MAINFRM_H
#define INC_MAINFRM_H

#include "..\CTablemap\CTablemap.h"
#include "..\CCritSec\CCritSec.h"

#define		HWND_CHECK_TIMER				1
#define		ENABLE_BUTTONS_TIMER		2
#define		UPDATE_STATUS_BAR_TIMER	3

// Forward declarations to avoid unnecessary include files
class COpenHoldemStatusbar;
class CDlgFormulaScintilla;
class CDlgScraperOutput;
class COpenHoldemTitle;
class CFlagsToolbar;
class CWhiteInfoBox;

class CMainFrame : public CFrameWnd {
 public:
  // Read-only access to GUI elements
  // GUI objects
  COpenHoldemStatusbar* p_openholdem_statusbar();
  CDlgFormulaScintilla* m_formulaScintillaDlg();
  CDlgScraperOutput*    m_ScraperOutputDlg();
  COpenHoldemTitle*     p_openholdem_title();
  CFlagsToolbar*        p_flags_toolbar();
  CWhiteInfoBox*        p_white_info_box();
 protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MESSAGE_MAP()

	CMainFrame();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnEditFormula();
	afx_msg void OnEditForceuserchair();
	afx_msg void OnEditViewLog();
	afx_msg void OnEditTagLog();
	afx_msg void OnScraperOutput();
	afx_msg void OnViewShootreplayframe();
  afx_msg void OnManualMode();
	afx_msg void OnEditPreferences();
	afx_msg void OnFileLoadOpenPPL();
	afx_msg void OnFileLoadTableMap();
	afx_msg void OnDllLoad();
	afx_msg void OnBnClickedRedCircle();
	afx_msg void OnBnClickedGreenCircle();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateStatus(CCmdUI *pCmdUI);
	afx_msg void OnAutoplayer();
	afx_msg void OnValidator();
	afx_msg void OnClickedFlags();

	afx_msg void OnUpdateMenuFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuFileOpen(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMenuFileEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuFileLoadProfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuDllLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewMainToolbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewStatusbar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShootreplayframe(CCmdUI *pCmdUI);
  afx_msg void OnUpdateViewScraperOutput(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditForceuserchair(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlCheckSyntax(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlEditMainFormula(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDllLoadspecificfile();
	afx_msg void OnPerlLoadFormula();
	afx_msg void OnPerlLoadSpecificFormula();
	afx_msg void OnPerlEditMainFormula();
	afx_msg void OnPerlCheckSyntax();
	afx_msg void OnPerlReloadFormula();
	afx_msg void OnHelpProblemSolver();
 public:
	afx_msg void OnHelp();
	afx_msg void OnHelpOpenPPL();
	afx_msg void OnHelpForums();

 public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();
	virtual BOOL DestroyWindow();
	void	OpenHelpFile(CString windows_help_file_chm);
	CString	_exec_filename;

 public:
	// public accessors
	const bool wait_cursor() { return _wait_cursor; }

 public:
	void DisableButtonsOnConnect();
	void EnableButtonsOnDisconnect();
	void UnattachOHFromPokerWindow();
 public:
	void StartTimer();
	void ResetDisplay();
	void KillTimer();
 public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_wait_cursor(const bool b) { ENT _wait_cursor = b; }
#undef ENT
 private:
  // GUI objects
  COpenHoldemStatusbar *_p_openholdem_statusbar;
  CDlgFormulaScintilla *_p_formulaScintillaDlg;
  CDlgScraperOutput    *_p_ScraperOutputDlg;
  COpenHoldemTitle     *_p_openholdem_title;
  CFlagsToolbar        *_p_flags_toolbar;
  CWhiteInfoBox        *_gp_white_info_box;
 private:
	// private variables - use public accessors and public mutators to address these
	bool			_wait_cursor;	 // Used if we need to display a wait cursor anywhere
private:
	// private functions and variables - not available via accessors or mutators
	int CreateStatusBar(void);

	RECT			_prev_att_rect, _prev_wrect;

	CCritSec		m_critsec;
};

// used by EnumProcTopLevelWindowList function
//!!!!!
extern CArray <STableList, STableList>		g_tlist; 

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam);

// Accessor function because p_mainframe never gets initialized.
CMainFrame* PMainframe();

// !!!!!
#define GUI PMainframe()

#endif /* INC_MAINFRM_H */
