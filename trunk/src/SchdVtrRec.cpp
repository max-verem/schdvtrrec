// SchdVtrRec.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SchdVtrRec.h"
#include "SchdVtrRecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecApp

BEGIN_MESSAGE_MAP(CSchdVtrRecApp, CWinApp)
	//{{AFX_MSG_MAP(CSchdVtrRecApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecApp construction

CSchdVtrRecApp::CSchdVtrRecApp()
{
	vtr = NULL;
	vtr_state = 0;
	vtr_tc = 0xFFFFFFFF;
	m_ini = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSchdVtrRecApp object

CSchdVtrRecApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSchdVtrRecApp initialization

BOOL CSchdVtrRecApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	/* parse command line */
	ParseCommandLine(m_opts);

	/* load ini file */
	m_ini = new CSchdVtrRecIni(m_opts.m_schedule_file_name);


	CSchdVtrRecDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CSchdVtrRecApp::ExitInstance() 
{
	if(NULL != m_ini)
	{
		delete m_ini;
		m_ini = NULL;
	};
	
	return CWinApp::ExitInstance();
}
