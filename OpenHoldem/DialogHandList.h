//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_DIALOGHANDLIST_H
#define INC_DIALOGHANDLIST_H

#include "resource.h"
#include "COHScriptList.h"

// CDlgHandList dialog

class CDlgHandList : public CDialog {
  friend class DialogFormulaScintilla;
public:
	CDlgHandList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHandList();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_HANDLIST };
  COHScriptList *p_list;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnCheckClick(UINT controlID);
	afx_msg void OnBnClickedPP();
	afx_msg void OnBnClickedBroadways();
	afx_msg void OnBnClickedSuited();
	afx_msg void OnBnClickedClear();

	CStatic m_HandList_Name, m_CommentST;
  // Indices 2..k_rank_ace get used.
  // 0 and 1 exist only for technical reasons; unused.
	CButton m_Check[k_rank_ace+1][k_rank_ace+1];

private:
	int nhands;
	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGHANDLIST_H