// SchdVtrRecDlg.h : header file
//

#if !defined(AFX_SCHDVTRRECDLG_H__732D88BD_EF84_4665_B3A1_470DF2CEA68C__INCLUDED_)
#define AFX_SCHDVTRRECDLG_H__732D88BD_EF84_4665_B3A1_470DF2CEA68C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecDlg dialog

class CSchdVtrRecDlg : public CDialog
{
	char title[1024];
	HANDLE monitor_thread;
	unsigned long monitor_id;
    HBITMAP m_digits_bitmaps[16];
    HACCEL m_hAccel;

// Construction
public:
	void show_window_on_failed_status();
	CSchdVtrRecDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSchdVtrRecDlg();

	void UpdateStatus(void);
	void UpdateTC(void);
    void update_tc(char* tc);

	void record_stop();
	void record_start();
	void record_eject();
	CListCtrl* schedule_list;

// Dialog Data
	//{{AFX_DATA(CSchdVtrRecDlg)
	enum { IDD = IDD_SCHDVTRREC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSchdVtrRecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HICON m_icon_fail;
	HICON m_icon_ok;
	CMenu m_TrayMenu;
	NOTIFYICONDATA tnd;

	// Generated message map functions
	//{{AFX_MSG(CSchdVtrRecDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnClose();
	afx_msg void InitTray();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT onTrayNotify(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnMenuCommand(UINT id);
	afx_msg BOOL OnVtrControlCommand(UINT id);
	afx_msg void OnScheduleListGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHDVTRRECDLG_H__732D88BD_EF84_4665_B3A1_470DF2CEA68C__INCLUDED_)
