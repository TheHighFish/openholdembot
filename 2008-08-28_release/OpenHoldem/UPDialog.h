#ifndef __UNIVERSALPROGRESSDIALOG_H_A0B4F977__97BA__43c0__83A2__6B64410E92E2
#define __UNIVERSALPROGRESSDIALOG_H_A0B4F977__97BA__43c0__83A2__6B64410E92E2

#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

#include "Resource.h"										//Resource File containing the Dialog Control ID Values

#define CUPDIALOG_DIALOG_ID			(IDD_PROGRESS_DIALOG)	//Dialog Box Id
#define CUPDIALOG_TEXTBOX_ID		(IDC_PROGRESS_TEXT)		//Static Control Id
#define CUPDIALOG_PROGRESSBAR_ID	(IDC_PROGRESS_BAR)		//ProgressBar Control Id
#define CUPDIALOG_CANCELBUTTON_ID	(IDCANCEL)				//Cancel Button Control Id (This allows us to use Any other button as Cancel Button)
#define CUPDIALOG_WM_MESSAGE		(WM_USER+1234)			//User Defined Message
#define CUPDIALOG_TERMINATE_DELAY	(500)					//Amount of time to wait after signaling the termination, in MilliSeconds.
#define CUPDIALOG_CONTROL_CLASSES	(ICC_PROGRESS_CLASS)	//We are using Progress bar Control

class CUPDialog;
typedef struct _CUPDialogUserProcData CUPDUPDATA;
typedef bool (*LP_CUPDIALOG_USERPROC)(const CUPDUPDATA*);

typedef struct _ProgressThreadData
{
protected:
	_ProgressThreadData()
	{
		ZeroMemory(this,sizeof(_ProgressThreadData));
	}
public:
	HWND	hThreadWnd;				//The Dialog that Created the Thread !!
	LPVOID	pUserProcParam;			//Parameter that shoud be sent to the UserProc
	bool	bAlive;					//Indicates the Thread State Alive/Dead
	bool	bTerminate;				//Thread Monitors this to Know if it has to Terminate itself

	LP_CUPDIALOG_USERPROC	m_lpUserProc;	//User Progress Procedure - Called by ProgressDialogBox From the ThreadProc

	enum							//These would be used by Thread - Should be inSync with DlgProc Values !!
	{
		WM_DISABLECONTROLS = CUPDIALOG_WM_MESSAGE,
		WM_PROGRESSTHREADCOMPLETED,
		WM_PROGRESSBARUPDATE,
		WM_PROGRESSTEXTUPDATE,
		WM_CANCELPROGRESSTHREAD
	};

}PROGRESSTHREADDATA;

typedef PROGRESSTHREADDATA* LPPROGRESSTHREADDATA;

struct _CUPDialogUserProcData: private _ProgressThreadData
{
	friend class CUPDialog;
private:
	_CUPDialogUserProcData()	{	ZeroMemory(this,sizeof(CUPDUPDATA));	}

public:
	inline LPVOID GetAppData() const	{	return	this->pUserProcParam;	}

	inline bool ShouldTerminate() const	{	return	this->bTerminate;		}

	inline void SetProgress(LPCTSTR lpszProgressText) const
	{
		if(::IsWindow(this->hThreadWnd) && this->bTerminate == false)
			::SendMessage(this->hThreadWnd, _ProgressThreadData::WM_PROGRESSTEXTUPDATE, 0, (LPARAM)lpszProgressText);
	}
	inline void SetProgress(UINT_PTR dwProgressbarPos) const
	{
		if(::IsWindow(this->hThreadWnd) && this->bTerminate == false)
			::SendMessage(this->hThreadWnd, _ProgressThreadData::WM_PROGRESSBARUPDATE, dwProgressbarPos, 0);
	}
	inline void SetProgress(LPCTSTR lpszProgressText, UINT_PTR dwProgressbarPos) const
	{
		SetProgress(lpszProgressText);
		SetProgress(dwProgressbarPos);
	}
	inline void SetProgress(LPCTSTR lpszProgressText, UINT_PTR dwProgressbarPos, bool show_dialog) const
	{
		SetProgress(lpszProgressText);
		SetProgress(dwProgressbarPos);
		::ShowWindow(this->hThreadWnd, show_dialog ? SW_SHOW : SW_HIDE);
	}
};


class CUPDialog
{
	static struct _tagInitCommonControls
	{		
		_tagInitCommonControls();	//We use this Constructor to call the InitCommonControlsEx() function
	}
			m_InitCommonControls;	//This Static Variable Facilitates us to call the InitCommonControlsEx() function Only Once Per App

	HWND	m_hParentWnd;			//The Window that Requested the Progress Operation; Needed to Create the DialogBox

	bool	m_bAllowCancel;			//Should the Dialog allow PreEmtption by user before Completion?

	HANDLE	m_hThread;				//Holds the Handle to the Created Thread

	TCHAR	m_szDialogCaption[256];	//Fill up with the Title of the DialogBox

	CUPDUPDATA 	m_ThreadData;

	friend INT_PTR CALLBACK ProgressDlgProc(HWND,UINT,WPARAM,LPARAM);	//The Windows Message Procedure for the DialogBox

	void Cleanup();

public:
	enum { IDD = CUPDIALOG_DIALOG_ID };

	CUPDialog(HWND hParentWnd,LP_CUPDIALOG_USERPROC lpUserProc,LPVOID lpUserProcParam,LPCTSTR lpszDlgTitle=_T("Please Wait.."),bool bAllowCancel=true);

	virtual ~CUPDialog();

	INT_PTR DoModal();	//Returns: IDOK => Sucessful;	(IDCANCEL && HIWORD(wParam)==0) => Some Error in UserProc;	(IDCANCEL && HIWORD(wParam)==1) =>User Cancelled the Dialog;
};

#endif