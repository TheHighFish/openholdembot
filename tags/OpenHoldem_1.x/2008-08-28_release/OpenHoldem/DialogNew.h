#ifndef INC_DIALOGNEW_H
#define INC_DIALOGNEW_H

// CDlgNew dialog

class CDlgNew : public CDialog
{
	DECLARE_DYNAMIC(CDlgNew)

public:
	CDlgNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNew();

// Dialog Data
	enum { IDD = IDD_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_NewName;
	CString CSnewname;
	int type;  // 0 = new list, 1 = new udf
	virtual BOOL OnInitDialog();
	CStatic m_Desc;
};

#endif //INC_DIALOGNEW_H