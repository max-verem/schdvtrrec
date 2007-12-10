// SchdVtrRecIni.cpp: implementation of the CSchdVtrRecIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SchdVtrRec.h"
#include "SchdVtrRecIni.h"
#include "SchdVtrRecConf.h"

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
	int i, r, j, k;
	unsigned long tc_t, tc_d;
	char
		*l = (char*)malloc(MAX_STRING_LEN),
		*t = (char*)malloc(MAX_STRING_LEN),
		*d = (char*)malloc(MAX_STRING_LEN);
		
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
				if(2 == (r = sscanf(l, "%s %s", t, d)))
				{
					/* parse timecodes */
					tc_t = tc_txt2frames(l);
					tc_d = tc_txt2frames(d);

					/* store value */
					add_timecodes(tc_t, tc_d);
				};
			};
		};

		fclose(f);
	};

	/* try to sort items */
	for(i = 0;i<COUNT; i++)
	{
		/* find minimal */
		for(j = i + 1, k = i; j < COUNT; j++)
			if(START[k] > START[j])
				k = j;
		
		/* swap values */
		if(i != k)
		{
			tc_t = START[i]; START[i] = START[k]; START[k] = tc_t;
			tc_d = DUR[i]; DUR[i] = DUR[k]; DUR[k] = tc_d;
		};
	};

	free(l);
	free(t);
	free(d);
}

CSchdVtrRecIni::~CSchdVtrRecIni()
{

}

void CSchdVtrRecIni::add_timecodes(unsigned long start_frm, unsigned long dur_frm)
{
	/* skip empty periods */
	if(0 == dur_frm) return;

	/* save values */
	START[COUNT] = start_frm;
	DUR[COUNT] = dur_frm;
	COUNT++;
}
