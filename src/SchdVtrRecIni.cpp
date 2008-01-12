// SchdVtrRecIni.cpp: implementation of the CSchdVtrRecIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SchdVtrRec.h"
#include "SchdVtrRecIni.h"
#include "SchdVtrRecConf.h"

#include <Mmsystem.h>

#include <mytimecode.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_STRING_LEN			1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSchdVtrRecIni::CSchdVtrRecIni(char* filename)
{
	int i, r;
	unsigned long tc_t, tc_d, f_eject;
	char
		*l = (char*)malloc(MAX_STRING_LEN),
		*t = (char*)malloc(MAX_STRING_LEN),
		*d = (char*)malloc(MAX_STRING_LEN),
		*e = (char*)malloc(MAX_STRING_LEN);
	
	unsigned __int64 this_day = get_curr_day_tc();

	/* setup defaults */
	COUNT = 0;

	/* compose filename */
	if(0 == stricmp(filename, DEFAULT_SCHEDULE_FILE))
	{
		GetModuleFileName(AfxGetInstanceHandle(), l, MAX_STRING_LEN);
		for(i = strlen(l) - 1; i>=0; i--)
			if
			(
				('/' == l[i])
				||
				('\\' == l[i])
			)
				i = -2;
			else
				l[i] = 0;

		strcat(l, DEFAULT_SCHEDULE_FILE);
	}
	else
		strcpy(l, filename);

	/* load file */
	FILE* f = fopen(l, "rt");
	if(NULL != f)
	{
		while(!feof(f))
		{
			l[0] = 0;

			/* get string from file */
			fgets(l, MAX_STRING_LEN, f);

			/* check fo comment */
			if('#' != l[0])
			{
				/* scan for two values */
				if(3 == (r = sscanf(l, "%s %s %s", t, d, e)))
				{
					/* parse timecodes */
					tc_t = tc_txt2frames(l);
					tc_d = tc_txt2frames(d);
					
					/* parse eject flag */
					f_eject = 0;
					if
					(
						('E' == e[0]) || 
						('e' == e[0]) ||
						('^' == e[0]) ||
						('+' == e[0])
					)
						f_eject = 1;


					/* store value */
					add_timecodes(tc_t, tc_d, this_day + tc_t, f_eject);
				};
			};
		};

		fclose(f);
	};

	/* sort timemingd */
	sort_starts();

	free(l);
	free(t);
	free(d);
	free(e);
}

#define SWAP_U(A, B, T)										\
{															\
	unsigned T t;											\
	t = A; A = B; B = t;									\
}

void CSchdVtrRecIni::sort_starts(void)
{
	int i, j, k;

	/* try to sort items */
	for(i = 0;i<COUNT; i++)
	{
		/* find minimal */
		for(j = i + 1, k = i; j < COUNT; j++)
			if(DATETIME[k] > DATETIME[j])
				k = j;
		
		/* swap values */
		if(i != k)
		{
			SWAP_U(START[i], START[k], long);
			SWAP_U(DUR[i], DUR[k], long);
			SWAP_U(DATETIME[i], DATETIME[k], __int64);
			SWAP_U(EJECT[i], EJECT[k], long);
		};
	};
};

unsigned __int64 CSchdVtrRecIni::get_now_tc(void)
{
	time_t ltime;
	struct tm *rtime;
	unsigned __int64 t;

	/* request time */
	time( &ltime );
	rtime = localtime( &ltime );
	ltime = mktime(rtime);

	t = timeGetTime();
	t %= 1000;
	t /= 40;

	return 
		t
		+
		((unsigned __int64)ltime)*25;
		
};

unsigned __int64 CSchdVtrRecIni::get_next_day_tc(void)
{
	time_t ltime;
	struct tm *rtime;

	/* request time */
	time( &ltime );
	rtime = localtime( &ltime );
	rtime->tm_hour = 0;
	rtime->tm_min = 0;
	rtime->tm_sec = 0;
	rtime->tm_mday += 1;
	ltime = mktime(rtime);
	
	return 
		((unsigned __int64)ltime)*25;
};

unsigned __int64 CSchdVtrRecIni::get_curr_day_tc(void)
{
	time_t ltime;
	struct tm *rtime;

	/* request time */
	time( &ltime );
	rtime = localtime( &ltime );
	rtime->tm_hour = 0;
	rtime->tm_min = 0;
	rtime->tm_sec = 0;
	ltime = mktime(rtime);
	
	return 
		((unsigned __int64)ltime)*25;
};

CSchdVtrRecIni::~CSchdVtrRecIni()
{

}

void CSchdVtrRecIni::add_timecodes(unsigned long start_frm, unsigned long dur_frm, unsigned __int64 dur_abs_frm, unsigned long eject)
{
	/* skip empty periods */
	if(0 == dur_frm) return;

	/* save values */
	START[COUNT] = start_frm;
	DUR[COUNT] = dur_frm;
	DATETIME[COUNT] = dur_abs_frm;
	EJECT[COUNT] = eject;

	COUNT++;
}
