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
	int f_exit;

	int COUNT;
	unsigned __int64
		DATETIME[MAX_SCHD_RECORDS],
		REMAIN_TO_START,
		REMAIN_TO_STOP,
		RECORD;

	unsigned long 
		START[MAX_SCHD_RECORDS],
		DUR[MAX_SCHD_RECORDS];

	void sort_starts(void);

	CSchdVtrRecIni(char* filename);
	virtual ~CSchdVtrRecIni();

	static unsigned __int64 get_curr_day_tc(void);
	static unsigned __int64 get_next_day_tc(void);
	static unsigned __int64 get_now_tc(void);
private:
	void add_timecodes(unsigned long start_frm, unsigned long dur_frm, unsigned __int64 dur_abs_frm);
};

#endif // !defined(AFX_SCHDVTRRECINI_H__7E037EB8_5AB7_400A_B482_A43F47ACE138__INCLUDED_)
