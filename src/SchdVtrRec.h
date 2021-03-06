// SchdVtrRec.h : main header file for the SCHDVTRREC application
//

#if !defined(AFX_SCHDVTRREC_H__35F7803D_C1E0_4420_BE8B_BBB37F704564__INCLUDED_)
#define AFX_SCHDVTRREC_H__35F7803D_C1E0_4420_BE8B_BBB37F704564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "SchdVtrRecConf.h"
#include "SchdVtrRecIni.h"

#define VTR_ONLINE					(1<<0)
#define VTR_TAPE_LOADED				(1<<1)
#define VTR_RECORD_NOT_INHIBITED	(1<<2)
#define VTR_REMOTE_CONTROL			(1<<3)
#define VTR_EOT						(1<<4)
#define VTR_NEAR_EOT				(1<<5)
#define VTR_EJECTING				(1<<6)
#define VTR_SYS_SVO_ALARM			(1<<7)

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecApp:
// See SchdVtrRec.cpp for the implementation of this class
//

class CSchdVtrRecApp : public CWinApp
{
public:
	CSchdVtrRecConf m_opts;
	CSchdVtrRecIni* m_ini;
	void* vtr;
	int vtr_state;
	int vtr_tc;

	CSchdVtrRecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSchdVtrRecApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSchdVtrRecApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHDVTRREC_H__35F7803D_C1E0_4420_BE8B_BBB37F704564__INCLUDED_)
