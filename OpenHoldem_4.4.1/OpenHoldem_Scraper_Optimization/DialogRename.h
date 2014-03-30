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

#ifndef INC_DIALOGRENAME_H
#define INC_DIALOGRENAME_H

// CDlgRename dialog

class CDlgRename : public CDialog
{
	DECLARE_DYNAMIC(CDlgRename)

public:
	CDlgRename(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRename();

// Dialog Data
	enum { IDD = IDD_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_OldName;
	CEdit m_NewName;
	CString CSoldname;
	CString CSnewname;
	virtual BOOL OnInitDialog();
};


#endif //INC_DIALOGRENAME_H