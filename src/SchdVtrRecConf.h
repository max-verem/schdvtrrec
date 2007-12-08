// SchdVtrRecConf.h: interface for the CSchdVtrRecConf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCHDVTRRECCONF_H__CFB8B1F6_56BA_4C0A_87B2_750B9344FCA2__INCLUDED_)
#define AFX_SCHDVTRRECCONF_H__CFB8B1F6_56BA_4C0A_87B2_750B9344FCA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSchdVtrRecConf : public CCommandLineInfo  
{
	int opt;
public:
	char m_serial_port_name[1024];
	char m_schedule_file_name[1024];

	CSchdVtrRecConf();

	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
};

#endif // !defined(AFX_SCHDVTRRECCONF_H__CFB8B1F6_56BA_4C0A_87B2_750B9344FCA2__INCLUDED_)