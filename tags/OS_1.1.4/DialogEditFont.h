#pragma once
#include "afxwin.h"
#include "resource.h"
#include "structs_defines.h"
#include "../CTransform/Transform.h"

// CDlgEditFont dialog

class CDlgEditFont : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditFont)

public:
	CDlgEditFont(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditFont();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_EDIT_FONT };
	CString			titletext;
	CString			character;
	CString			type;
	CArray <Stablemap_font, Stablemap_font>		*new_t$_recs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeCharlist();
	afx_msg void OnEnKillfocusCharacter();
	afx_msg void OnEnKillfocusType();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSort();

	CEdit		m_Character;
	CComboBox	m_Type;
	CListBox	m_CharList;
	CEdit		m_PixelSep;
	LOGFONT		lf_fixed;
	CFont		separation_font;

	DECLARE_MESSAGE_MAP()
};
