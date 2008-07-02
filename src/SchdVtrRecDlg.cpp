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

#include <vtr_cmd.h>
#include <vtr_srv.h>
#include <mytimecode.h>

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib") 

static unsigned long WINAPI schedule_monitor(void* p)
{
	unsigned __int64 t;
	CSchdVtrRecDlg* dlg = (CSchdVtrRecDlg*)p;

	theApp.m_ini->RECORD = 0;

	while((0 == theApp.m_ini->f_exit)&&(0 != theApp.m_ini->COUNT))
	{
		t = CSchdVtrRecIni::get_now_tc();

		/* check if item is obsolete */
		if(t > (theApp.m_ini->DATETIME[0] + theApp.m_ini->DUR[0]))
		{
			/* 
			 * first item is obsolete and must be moved to the end of list 
			 * but check if record started before
			 */
			if(0 != theApp.m_ini->RECORD)
			{
				dlg->record_stop();
				if(theApp.m_ini->EJECT[0])
					dlg->record_eject();
				theApp.m_ini->RECORD = 0;
			};

			theApp.m_ini->DATETIME[0] = 
				CSchdVtrRecIni::get_next_day_tc()
				+
				theApp.m_ini->START[0];

			theApp.m_ini->sort_starts();
		}
		else
		{
			if(t < theApp.m_ini->DATETIME[0])
			{
				/* check if still recording */
				if(0 != theApp.m_ini->RECORD)
				{
					dlg->record_stop();
					if(theApp.m_ini->EJECT[0])
						dlg->record_eject();
					theApp.m_ini->RECORD = 0;
				};

				/* update remains */
				theApp.m_ini->REMAIN_TO_START = 
					theApp.m_ini->DATETIME[0]
					-
					t;
				theApp.m_ini->REMAIN_TO_STOP = 0;

				/* check if status OK and window is visible */
				if(theApp.m_ini->REMAIN_TO_START < theApp.m_opts.m_alert_before)
					dlg->show_window_on_failed_status();
			}
			else
			{
				/* check record sent */
				if(0 == theApp.m_ini->RECORD)
				{
					theApp.m_ini->RECORD = t;
					dlg->record_start();
				};


				/* update remains */
				theApp.m_ini->REMAIN_TO_START = 0;
				theApp.m_ini->REMAIN_TO_STOP = 
					theApp.m_ini->DATETIME[0] 
					+ 
					theApp.m_ini->DUR[0]
					-
					t;

				/* check if status OK and window is visible */
				dlg->show_window_on_failed_status();
			};
		};

		dlg->schedule_list->SetItemCountEx(theApp.m_ini->COUNT, LVSICF_NOSCROLL);
	
		Sleep(40 * 5);
	};

	return 0;
};

/*
 * VTR state callback function
 *
 */
static void cb_vtr(void *desc, int state_id, int state_val, void *cookie)
{
	int i = 0, v;
	int vtr_state = 0; 
	int vtr_tc = 0; 

	/* is online */
	vtr_srv_get(desc, VTR_SRV_STATE_ONLINE, &v);
	if(0 != v)
	{
		vtr_state |= VTR_ONLINE;

		/* tape present */
		vtr_srv_get
		(
			desc, 
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_TAPE_UNTHREADED, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_TAPE_LOADED;

		/* vtr in "record not inhibited" */
		vtr_srv_get
		(
			desc, 
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_REC_INHIB, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_RECORD_NOT_INHIBITED;

		/* vtr in "remote" */
		vtr_srv_get
		(
			desc,
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_LOCAL, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_REMOTE_CONTROL;


		/* vtr in "end of tape" */
		vtr_srv_get
		(
			desc,
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_EOT, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_EOT;

		/* vtr in "near end of tape" */
		vtr_srv_get
		(
			desc,
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_NEAR_EOT, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_NEAR_EOT;

		/* vtr in "ejecting" */
		vtr_srv_get
		(
			desc,
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_EJECT, 
			&v
		);
		if(0 == v)
			vtr_state |= VTR_EJECTING;

		/* vtr SVO and SYS alarm */
		vtr_srv_get
		(
			desc,
			VTR_SRV_STATE_STATUS_BIT, 
			VTR_CMD_STATUS_BITS_SVO_ALARM, 
			&v
		);
		if(0 == v)
		{
			vtr_srv_get
			(
				desc,
				VTR_SRV_STATE_STATUS_BIT, 
				VTR_CMD_STATUS_BITS_SYS_ALARM, 
				&v
			);
			if(0 == v)
				vtr_state |= VTR_SYS_SVO_ALARM;
		};


		/* update timecode */
		vtr_srv_get(desc, VTR_SRV_STATE_TC, &vtr_tc);
	};

	/* check status changed */
	if(vtr_state != theApp.vtr_state)
	{
		i = 1;
		theApp.vtr_state = vtr_state;
		((CSchdVtrRecDlg*)cookie)->UpdateStatus();
	};

	/* tc update sync */
	if((0 != i)||(vtr_tc != theApp.vtr_tc))
	{
		theApp.vtr_tc = vtr_tc;
		((CSchdVtrRecDlg*)cookie)->UpdateTC();
	};
};

void CSchdVtrRecDlg::UpdateStatus(void)
{
	CStatic* s;

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_ONLINE);
	s->SetIcon((theApp.vtr_state & VTR_ONLINE)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_TAPE_LOADED);
	s->SetIcon((theApp.vtr_state & VTR_TAPE_LOADED)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_RECORD_NOT_INHIBITED);
	s->SetIcon((theApp.vtr_state & VTR_RECORD_NOT_INHIBITED)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_REMOTE_CONTROL);
	s->SetIcon((theApp.vtr_state & VTR_REMOTE_CONTROL)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_EOT);
	s->SetIcon((theApp.vtr_state & VTR_EOT)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_NEAR_EOT);
	s->SetIcon((theApp.vtr_state & VTR_NEAR_EOT)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_EJECTING);
	s->SetIcon((theApp.vtr_state & VTR_EJECTING)?m_icon_ok:m_icon_fail);

	s = (CStatic*)GetDlgItem(IDC_ICON_VTR_SYS_SVO_ALARM);
	s->SetIcon((theApp.vtr_state & VTR_SYS_SVO_ALARM)?m_icon_ok:m_icon_fail);
};

void CSchdVtrRecDlg::update_tc(char* tc)
{
    unsigned long tc_bcd;
    int i, j;

    static int lcd_digits_map[8] = 
    {
        IDC_TC_VTR_HH_X0,
        IDC_TC_VTR_HH_0X,
        IDC_TC_VTR_MM_X0,
        IDC_TC_VTR_MM_0X,
        IDC_TC_VTR_SS_X0,
        IDC_TC_VTR_SS_0X,
        IDC_TC_VTR_FF_X0,
        IDC_TC_VTR_FF_0X
    };

    /* check if submitted value defined */
    if
    (
        (NULL == tc)
        ||
        (0 == tc[0])
    )
        tc_bcd = 0xAAAAAAAA;
    else
        tc_bcd = tc_txt2bcd(tc);

    /* setup digits */
    for(i = 0; i<8; i++)
    {
        /*
            Find index 
            0: 28
            1: 24
            2: 20
            3: 16
            .....
        */
        j = 0x0000000F & (tc_bcd >> (28 - 4*i));

        /* set digit */
        ((CStatic*)GetDlgItem(lcd_digits_map[i]))
            ->SetBitmap(m_digits_bitmaps[j]);
    };
};

void CSchdVtrRecDlg::UpdateTC(void)
{
	char TC[128];

	if(theApp.vtr_state & VTR_ONLINE)
		tc_bcd2txt((unsigned long)theApp.vtr_tc, TC);
	else
		TC[0] = 0;

    update_tc(TC);
};

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
	/* stop thread */
	theApp.m_ini->f_exit = 1;
	if(INVALID_HANDLE_VALUE != monitor_thread)
		WaitForSingleObject(monitor_thread, INFINITE);

	/* destroy VTR */
	if(NULL != 	theApp.vtr)
	{
		/* stop service */
		vtr_srv_stop(theApp.vtr);

		/* destroy service */
		vtr_srv_destroy(theApp.vtr);
	};


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

CSchdVtrRecDlg::~CSchdVtrRecDlg()
{
    int i;

    /* free loaded objects */
    for(i = 0; i<11; i++)
        if(NULL != m_digits_bitmaps[i])
            ::DeleteObject(m_digits_bitmaps[i]);

};

CSchdVtrRecDlg::CSchdVtrRecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSchdVtrRecDlg::IDD, pParent)
{
    int i;

	//{{AFX_DATA_INIT(CSchdVtrRecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    /* define ids of LCD DIGITS bitmaps */
    static int digits_ids[16] = 
    {
        IDB_DIGIT_0,
        IDB_DIGIT_1,
        IDB_DIGIT_2,
        IDB_DIGIT_3,
        IDB_DIGIT_4,
        IDB_DIGIT_5,
        IDB_DIGIT_6,
        IDB_DIGIT_7,
        IDB_DIGIT_8,
        IDB_DIGIT_9
    };

    /* load digits bitmaps */
    for(i = 0; i<11; i++)
        m_digits_bitmaps[i] = ::LoadBitmap
        (
            AfxGetResourceHandle(), 
            MAKEINTRESOURCE( (i<10)?digits_ids[i]:IDB_DIGIT_SP)
        );

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_icon_ok = AfxGetApp()->LoadIcon(IDI_ICON_VTR_STATUS_OK);
	m_icon_fail = AfxGetApp()->LoadIcon(IDI_ICON_VTR_STATUS_FAIL);

    /* load accelerator */
    int nIDAccel = IDR_ACCELERATORS;
	m_hAccel = ::LoadAccelerators
	(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIDAccel)
	); 
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

	ON_COMMAND_EX(IDC_BUTTON_EJECT, OnVtrControlCommand)
	ON_COMMAND_EX(IDC_BUTTON_REWIND, OnVtrControlCommand)
	ON_COMMAND_EX(IDC_BUTTON_STOP, OnVtrControlCommand)
	ON_COMMAND_EX(IDC_BUTTON_PLAY, OnVtrControlCommand)
	ON_COMMAND_EX(IDC_BUTTON_FASTF, OnVtrControlCommand)
	ON_COMMAND_EX(IDC_BUTTON_RECORD, OnVtrControlCommand)

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
	schedule_list->InsertColumn(4,_T("EJECT"), LVCFMT_LEFT, 60,-1);
	schedule_list->SetItemCountEx(theApp.m_ini->COUNT, LVSICF_NOSCROLL);

	/* init tray way */
	InitTray();
 
	/* show window hidden */	
	PostMessage(WM_COMMAND, ID_PROGRAM_HIDE, 0);

	/* setup timer */
//	SetTimer(0,1000,NULL);

	/* init VTR */
	theApp.vtr = vtr_srv_create();

	/* setup port */
	vtr_srv_set(theApp.vtr,
		VTR_SRV_CONF_SERIAL_PORT,
		theApp.m_opts.m_serial_port_name);

	/* setup callback function */
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_MODEL_ID, VTR_SRV_STATE_ALL_VAL,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_ONLINE, VTR_SRV_STATE_ALL_VAL,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_TC, VTR_SRV_STATE_ALL_VAL,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_TAPE_UNTHREADED,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_LOCAL,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_RECORD,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_PLAY,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_SERVO_LOCK,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_STANDBY,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_EJECT,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_SVO_ALARM,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_SYS_ALARM,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_NEAR_EOT,
		cb_vtr, this);
	vtr_srv_reg_state_cb(theApp.vtr, 
		VTR_SRV_STATE_STATUS_BIT, 
		VTR_CMD_STATUS_BITS_EOT,
		cb_vtr, this);

	/* run service */
	vtr_srv_start(theApp.vtr);

	/* run callback */
	cb_vtr(theApp.vtr, 0, 0, this);

	/* run monitor thread */
	theApp.m_ini->f_exit = 0;
	monitor_thread = CreateThread
	(
		NULL,
		4096,
		schedule_monitor,
		this,
		0,
		&monitor_id
	);

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

BOOL CSchdVtrRecDlg::OnVtrControlCommand(int id)
{
	BOOL ret = FALSE;
	int cmd, r = 0;

	switch(id)
	{
		case IDC_BUTTON_EJECT:
			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_EJECT);
			ret = TRUE;
			break;

		case IDC_BUTTON_REWIND:
			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_REWIND);
			ret = TRUE;
			break;

		case IDC_BUTTON_STOP:
			record_stop();
//			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_STOP);
			ret = TRUE;
			break;

		case IDC_BUTTON_PLAY:
			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_PLAY);
			ret = TRUE;
			break;

		case IDC_BUTTON_FASTF:
			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_FAST_FWD);
			ret = TRUE;
			break;

		case IDC_BUTTON_RECORD:
			record_start();
//			r = vtr_srv_send_cmd_sync(theApp.vtr, NULL, cmd = VTR_CMD_RECORD);
			ret = TRUE;
			break;
	};

	return ret;
};


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

static char* build_long_tc(char* buffer, unsigned __int64 tc)
{
	static char *templ = "%Y-%m-%d %H:%M:%S";
	struct tm *rtime;
	time_t ltime = (time_t)(tc / 25);
	tc %= 25;

	rtime = localtime( &ltime );

	strftime( buffer, 128, templ , rtime );

	sprintf(buffer + strlen(buffer), ":%.2d", (unsigned long)tc);

	return buffer;
};

void CSchdVtrRecDlg::OnScheduleListGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	int iItemIndx = pItem->iItem;
	char* s = NULL;
	char buffer[128]; buffer[0] = 0;

	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		// fill subitems
		switch(pItem->iSubItem)
		{
			/* REMAIN TO START */
			case 0:
				if
				(
					(0 == iItemIndx)
					&&
					(0 != theApp.m_ini->REMAIN_TO_START)
				)
					s = tc_frames2txt((unsigned long)theApp.m_ini->REMAIN_TO_START, buffer);
				else
					s = "";
				break;

			/* DATE TIME*/
			case 1:
				s = build_long_tc(buffer, theApp.m_ini->DATETIME[iItemIndx]);
				break;

			/* DURATION */
			case 2:
				s = tc_frames2txt((unsigned long)theApp.m_ini->DUR[iItemIndx], buffer);
				break;

			/* REMAIN TO FINISH */
			case 3:
				if
				(
					(0 == iItemIndx)
					&&
					(0 != theApp.m_ini->REMAIN_TO_STOP)
				)
					s = tc_frames2txt((unsigned long)theApp.m_ini->REMAIN_TO_STOP, buffer);
				else
					s = "";
				break;

			/* DATE TIME*/
			case 4:
				if(theApp.m_ini->EJECT[iItemIndx])
					s = "EJECT";
				else
					s = "";
				break;
		};

		if(NULL != s)
			lstrcpy(pItem->pszText, s);
	};

};

/**
 * Eject tape
 */
void CSchdVtrRecDlg::record_eject()
{
	vtr_srv_send_cmd_sync(theApp.vtr, NULL, VTR_CMD_EJECT);
};

/*
 *
 * Stop recording
 *
 */
void CSchdVtrRecDlg::record_stop()
{
	vtr_srv_send_cmd_sync(theApp.vtr, NULL, VTR_CMD_STOP);
};

/*
 *
 * Start recording
 *
 */
void CSchdVtrRecDlg::record_start()
{
#define DEC2BCD(V) ((unsigned long)( ((V / 10)<<4) | (V % 10)  ))
	unsigned long d;
	time_t ltime;
	struct tm *rtime;

	/* request time */
	time( &ltime );

	/* local */
	rtime = localtime( &ltime );

	/* setup users bit */
	d = 
		(DEC2BCD( (rtime->tm_year + 1900) / 100 ) << 24)
		|
		(DEC2BCD( (rtime->tm_year + 1900) % 100 ) << 16)
		|
		(DEC2BCD( (rtime->tm_mon + 1) ) << 8)
		|
		(DEC2BCD(rtime->tm_mday ) << 0);

	vtr_srv_send_cmd_sync(theApp.vtr, NULL, VTR_CMD_USER_BIT_PRESET, d);

	/* setup timecode */
	d = 
		(DEC2BCD( rtime->tm_hour ) << 24)
		|
		(DEC2BCD( rtime->tm_min ) << 16)
		|
		(DEC2BCD( rtime->tm_sec ) << 8)
		|
		(DEC2BCD( (timeGetTime() % 1000) / 30) << 0);

	vtr_srv_send_cmd_sync(theApp.vtr, NULL, VTR_CMD_TIME_CODE_PRESET, d);


	/* record */
	vtr_srv_send_cmd_sync(theApp.vtr, NULL, VTR_CMD_RECORD);
};

/*
 * Check if status failed and window in hidden state 
 * If this condition is true - show window in normal state
 */
void CSchdVtrRecDlg::show_window_on_failed_status()
{
	if
	(
		theApp.vtr_state
		!=
		(VTR_ONLINE | VTR_TAPE_LOADED 
			| VTR_RECORD_NOT_INHIBITED | VTR_REMOTE_CONTROL
			| VTR_EOT | VTR_NEAR_EOT | VTR_EJECTING | VTR_SYS_SVO_ALARM)
	)
		ShowWindow(SW_SHOW);
}

BOOL CSchdVtrRecDlg::PreTranslateMessage(MSG* pMsg) 
{
    if( TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) ) return TRUE;

    return CDialog::PreTranslateMessage(pMsg);
}
