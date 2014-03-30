#pragma once


// CMyToolBar

class CMyToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CMyToolBar)

public:
	CMyToolBar();
	virtual ~CMyToolBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
};


