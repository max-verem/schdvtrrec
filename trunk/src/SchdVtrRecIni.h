// SchdVtrRecIni.h: interface for the CSchdVtrRecIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCHDVTRRECINI_H__7E037EB8_5AB7_400A_B482_A43F47ACE138__INCLUDED_)
#define AFX_SCHDVTRRECINI_H__7E037EB8_5AB7_400A_B482_A43F47ACE138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SCHD_RECORDS	1024

class CSchdVtrRecIni  
{
public:
	int COUNT;
	unsigned long START[MAX_SCHD_RECORDS];
	unsigned long DUR[MAX_SCHD_RECORDS];

	CSchdVtrRecIni(char* filename);
	virtual ~CSchdVtrRecIni();

private:
	void add_timecodes(unsigned long start_frm, unsigned long dur_frm);
};

#endif // !defined(AFX_SCHDVTRRECINI_H__7E037EB8_5AB7_400A_B482_A43F47ACE138__INCLUDED_)
