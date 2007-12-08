// SchdVtrRecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SchdVtrRec.h"
#include "SchdVtrRecDlg.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSchdVtrRecApp theApp;

/* --------------------------------------------------------------------------

	tray icon code based on 
	http://www.go4expert.com/forums/showthread.php?t=343

-------------------------------------------------------------------------- */

#define MYWM_NOTIFYICON (WM_USER+1)

LRESULT CSchdVtrRecDlg::onTrayNotify(WPARAM wParam,LPARAM lParam)
{
    UINT uMsg = (UINT) lParam; 
    switch (uMsg ) 
    { 
		case WM_LBUTTONDBLCLK:
			this->ShowWindow(SW_SHOW);
			break;

		case WM_RBUTTONUP:
			{
				CPoint pt;    
				GetCursorPos(&pt);
				m_TrayMenu.GetSubMenu(0)->TrackPopupMenu
				(
					TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
					pt.x, pt.y,
					this
				);
			};
			break;
    } 
    return TRUE;
};


void CSchdVtrRecDlg::OnSize(UINT nType, int cx, int cy)
{
	if(nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSize(nType, cx, cy);
	};
} 

void CSchdVtrRecDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE,&tnd);
};

void CSchdVtrRecDlg::InitTray()
{
	/* init tray meny */
	m_TrayMenu.LoadMenu(IDR_MENU);

	/* setup NOTIFYICONDATA struct */
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = this->GetSafeHwnd();;
	tnd.uID = IDR_MAINFRAME;
	tnd.uFlags = NIF_MESSAGE|NIF_ICON;
	tnd.uCallbackMessage = MYWM_NOTIFYICON;
	tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_MAINFRAME));
	lstrcpyn(tnd.szTip, (LPCTSTR)title, sizeof(tnd.szTip));

	/* stup trey icon */
	DWORD dwMessage = NIM_ADD;
	Shell_NotifyIcon(dwMessage, &tnd);    
}

void CSchdVtrRecDlg::OnClose()
{
	ShowWindow(SW_HIDE);
};

/* --------------------------------------------------------------------------

  ignore default dialog behave

-------------------------------------------------------------------------- */


void CSchdVtrRecDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
};

void CSchdVtrRecDlg::OnOK()
{
	ShowWindow(SW_HIDE);
};

/* --------------------------------------------------------------------------

  standart mfc deals

-------------------------------------------------------------------------- */


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecDlg dialog

CSchdVtrRecDlg::CSchdVtrRecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSchdVtrRecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSchdVtrRecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSchdVtrRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSchdVtrRecDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSchdVtrRecDlg, CDialog)
	//{{AFX_MSG_MAP(CSchdVtrRecDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MYWM_NOTIFYICON, onTrayNotify)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND_EX(ID_PROGRAM_SHOW, OnMenuCommand)
	ON_COMMAND_EX(ID_PROGRAM_EXIT, OnMenuCommand)
	ON_COMMAND_EX(ID_PROGRAM_HIDE, OnMenuCommand)
	ON_COMMAND_EX(ID_PROGRAM_ABOUT, OnMenuCommand)
//	ON_COMMAND_EX(ID_CANCEL_ENCODING, OnMenuCommand)
//	ON_COMMAND_EX(ID_CLEAR_USED_FLAG, OnMenuCommand)
//	ON_COMMAND_EX(ID_WATCH2CONVERT_ABOUT, OnMenuCommand)

	ON_WM_TIMER()
//	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

//	ON_UPDATE_COMMAND_UI(IDC_WATCH_LIST_COUNT,		ui_update)
//	ON_UPDATE_COMMAND_UI(IDC_WATCH_LIST_PATH,		ui_update)
//	ON_UPDATE_COMMAND_UI(ID_CANCEL_ENCODING,		ui_update)
//	ON_UPDATE_COMMAND_UI(ID_CLEAR_USED_FLAG,		ui_update)

	ON_NOTIFY(LVN_GETDISPINFO, IDC_SCHEDULE_LIST, OnScheduleListGetDispInfo)
//	ON_NOTIFY(LVN_GETDISPINFO, IDC_ENCODINGS_LIST_STATE, OnEncodingsListGetDispInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecDlg message handlers

BOOL CSchdVtrRecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/* compose title */
	sprintf
	(
		title, 
		"%s [%s]", 
		theApp.m_opts.m_schedule_file_name,
		theApp.m_opts.m_serial_port_name
	);

	/* set window title */
	SetWindowText(title);

	/* configure files list */
	schedule_list = (CListCtrl*)GetDlgItem(IDC_SCHEDULE_LIST);
	schedule_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	/* Create files list columns */
	schedule_list->InsertColumn(0, _T("REMAIN TO START"), LVCFMT_LEFT, 120, -1);
	schedule_list->InsertColumn(1,_T("DATE"), LVCFMT_LEFT , 200, -1);
	schedule_list->InsertColumn(2,_T("DURATION"), LVCFMT_LEFT, 120, -1);
	schedule_list->InsertColumn(3,_T("REMAIN TO FINISH"), LVCFMT_LEFT, 120,-1);
	schedule_list->SetItemCountEx(10, LVSICF_NOSCROLL);


	/* init tray way */
	InitTray();
 
	/* show window hidden */	
	PostMessage(WM_COMMAND, ID_PROGRAM_HIDE, 0);

	SetTimer(0,1000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSchdVtrRecDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSchdVtrRecDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/* --------------------------------------------------------------------------

  commands processing

-------------------------------------------------------------------------- */

BOOL CSchdVtrRecDlg::OnMenuCommand(int id)
{
	BOOL ret = FALSE;

	switch(id)
	{
		case ID_PROGRAM_SHOW:
			this->ShowWindow(SW_SHOW);
			ret = TRUE;
			break;

		case ID_PROGRAM_HIDE:
			this->ShowWindow(SW_HIDE);
			ret = TRUE;
			break;

		case ID_PROGRAM_EXIT:
			CDialog::OnCancel();
			ret = TRUE;
			break;

		case ID_PROGRAM_ABOUT:
			{
				CDialog dlg(IDD_ABOUTBOX, this);
				dlg.DoModal();
			}
			break;
	};

	return ret;
};

/* --------------------------------------------------------------------------

  Lists control

-------------------------------------------------------------------------- */

void CSchdVtrRecDlg::OnScheduleListGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;
	int iItemIndx= pItem->iItem;

	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
			// fill subitems
			switch(pItem->iSubItem)
			{
				/* REMAIN TO START */
				case 0:
					lstrcpy(pItem->pszText, "23:32:23:00");
					break;

				/* DATE */
				case 1:
					lstrcpy(pItem->pszText, "2007-12-08 23:32:23:00");
					break;

				/* DURATION */
				case 2:
					lstrcpy(pItem->pszText, "00:18:21:00");
					break;

				/* REMAIN TO FINISH */
				case 3:
					lstrcpy(pItem->pszText, "23:32:23:00");
					break;
			};
	};

};
