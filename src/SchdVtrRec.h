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

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecApp:
// See SchdVtrRec.cpp for the implementation of this class
//

class CSchdVtrRecApp : public CWinApp
{
public:
	CSchdVtrRecConf m_opts;

	CSchdVtrRecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSchdVtrRecApp)
	public:
	virtual BOOL InitInstance();
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
