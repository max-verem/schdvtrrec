// SchdVtrRecConf.cpp: implementation of the CSchdVtrRecConf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SchdVtrRec.h"
#include "SchdVtrRecConf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSchdVtrRecConf::CSchdVtrRecConf()
{
	/* set defaults */
	strcpy(m_serial_port_name, "COM1:");
	strcpy(m_schedule_file_name, "SchdVtrRec.ini");
	opt = 0;
}

/*
	Notes:

	http://www.codeguru.com/forum/archive/index.php/t-272123.html

*/

enum CSchdVtrRecConfOpt
{
	optNone = 0,
	optSerialPortName,
	optScheduleFileName
};

static struct
{
	char* name;
	int id;
} opts[] = 
{
	{"port",		optSerialPortName},
	{"p",			optSerialPortName},
	{"schedule",	optScheduleFileName},
	{"s",			optScheduleFileName},
	{NULL, 0}
};

void CSchdVtrRecConf::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	int i;

	/* flag given */
	if(bFlag)
	{
		for(opt = optNone, i = 0; (NULL != opts[i].name) && (optNone == opt); i++)
			if(0 == stricmp(pszParam, opts[i].name))
				opt = opts[i].id;
	}
	else
	{
		/* check what option was previously */
		switch(opt)
		{
			case optSerialPortName:
				strcpy(this->m_serial_port_name, (char*)pszParam);
				break;

			case optScheduleFileName:
				strcpy(this->m_schedule_file_name, (char*)pszParam);
				break;

			default:
				/* do nothing, reserved for future */
				break;
		};

		/* clear option */
		opt = optNone;
	};
};