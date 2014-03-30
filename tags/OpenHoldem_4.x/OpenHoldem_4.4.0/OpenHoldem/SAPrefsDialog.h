//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef AFX_PREFSDIALOG_H__1B15B002_9152_11D3_A10C_00500402F30B__INCLUDED_
#define AFX_PREFSDIALOG_H__1B15B002_9152_11D3_A10C_00500402F30B__INCLUDED_

/*********************************************************************

   Copyright (C) 2002 Smaller Animals Software, Inc.

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.

   3. This notice may not be removed or altered from any source distribution.

   http://www.smalleranimals.com
   smallest@smalleranimals.com

**********************************************************************/
// PrefsDialog.h : header file
//

#include "SAPrefsStatic.h"
#include "SAPrefsSubDlg.h"

#include <afxtempl.h>

class pageStruct
{
   public:
   CSAPrefsSubDlg *pDlg;
   UINT id;
   CSAPrefsSubDlg *pDlgParent;
   CString csCaption;
};

#define WM_CHANGE_PAGE (WM_APP + 100)
#define WM_SET_FOCUS_WND (WM_APP + 101)

/////////////////////////////////////////////////////////////////////////////
// CSAPrefsDialog dialog

class CSAPrefsDialog : public CDialog
{
// Construction
public:
	CSAPrefsDialog(CWnd* pParent = NULL);   // standard constructor
   ~CSAPrefsDialog();

// Dialog Data
	//{{AFX_DATA(CSAPrefsDialog)
	enum { IDD = IDD_SAPREFS };
	CStatic	m_boundingFrame;
	//}}AFX_DATA

	// dialog title
	void SetTitle(CString t)   {m_csTitle = t;}
	
	// used in the pretty shaded static control
	void SetConstantText(CString t)   {m_csConstantText = t;}

	// if you set this, UpdateData will be called on the current page before
	// switching to the new page. if UpdateData fails, the page is not changed.
	void SetValidateBeforeChangingPages(bool b)	{m_bValidateBeforeChangingPage = b;}

	// add a page (page, page title, optional parent)
	bool AddPage(CSAPrefsSubDlg &page, const char *pCaption, CSAPrefsSubDlg *pDlgParent = NULL);
	
	// show a page
	bool ShowPage(int iPage);
	bool ShowPage(CSAPrefsSubDlg * pPage);
	
	// end the dialog with a special return code
	void EndSpecial(UINT res, bool bOk = true);
	
	// set the first page
	void SetStartPage(CSAPrefsSubDlg *pPage = NULL) {m_pStartPage = pPage;}
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSAPrefsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
   
   bool EndOK();

   // if you don't like this, you can replace it with a static
   CSAPrefsStatic	   m_captionBar;
	CTreeCtrl		  m_pageTree;

   // check to see if this dlg has already been added to the tree
   HTREEITEM FindHTREEItemForDlg(CSAPrefsSubDlg *pParent);

	// Generated message map functions
	//{{AFX_MSG(CSAPrefsDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangedPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPhelp();
	afx_msg void OnSelchangingPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnChangePage(WPARAM w, LPARAM l);
	afx_msg long OnSetFocusWnd(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()

   CPtrArray   m_pages;
   int		 m_iCurPage;
   CRect	   m_frameRect;
   CString	 m_csTitle, m_csConstantText;

   bool m_bValidateBeforeChangingPage;

	CSAPrefsSubDlg	*m_pStartPage;

   // store info about *pDlgs that have been added to 
   // the tree - used for quick lookup of parent nodes
   // DWORDs are used because HTREEITEMs can't be... blame Microsoft
   CMap< CSAPrefsSubDlg *, CSAPrefsSubDlg *, UINT, UINT&  > m_dlgMap;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSDIALOG_H__1B15B002_9152_11D3_A10C_00500402F30B__INCLUDED_)
