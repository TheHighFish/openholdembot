#ifndef INC_DIALOGSCRAPEROUTPUT_H
#define INC_DIALOGSCRAPEROUTPUT_H

#include "WinMgr.h"
#include "SizerBar.h"
#include "Resource.h"

// CDlgScraperOutput dialog
class CDlgScraperOutput : public CDialog
{
public:
	CDlgScraperOutput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgScraperOutput();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	void AddListboxItems(void);
	void UpdateDisplay();

	enum { IDD = IDD_SCRAPER_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	afx_msg void OnLbnSelchangeRegionlist();
	afx_msg void OnCbnSelchangeZoom();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnWinMgr(WPARAM wp, LPARAM lp);
	void DoBitblt(HBITMAP bitmap, int r$index);

	CWinMgr			m_winMgr;				// window manager
	CSizerBar		m_winMgrSizerBar;		// sizer bar
	CListBox		m_RegionList;
	CStatic			m_ScraperBitmap;
	CComboBox		m_Zoom;
	CEdit			m_ScraperResult;
	bool			in_startup;

	DECLARE_MESSAGE_MAP()
};


extern CDlgScraperOutput	*m_ScraperOutputDlg;

#endif //INC_DIALOGSCRAPEROUTPUT_H