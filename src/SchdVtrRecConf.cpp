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
	strcpy(m_serial_port_name, DEFAULT_SERIAL_PORT);
	strcpy(m_schedule_file_name, DEFAULT_SCHEDULE_FILE);
	m_alert_before = DEFAULT_ALERT_TIME;
    m_setup_tc = 0;
	opt = 0;
}

/*
	Notes:

	http://www.codeguru.com/forum/archive/index.php/t-272123.html

*/

enum CSchdVtrRecConfOpt
{
	optNone = 0,
    optSetupTC,
	optSerialPortName,
	optScheduleFileName,
	optAlertTime
};

static struct
{
	char* name;
	int id;
} opts[] = 
{
    {"tc",          optSetupTC},
	{"port",		optSerialPortName},
	{"p",			optSerialPortName},
	{"schedule",	optScheduleFileName},
	{"s",			optScheduleFileName},
	{"alert",		optAlertTime},
	{"a",			optAlertTime},
	{NULL, 0}
};

void CSchdVtrRecConf::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	int i;

	/* flag given */
	if(bFlag)
	{
		for(opt = optNone, i = 0; (NULL != opts[i].name) && (optNone == opt); i++)
            if(0 == _stricmp(pszParam, opts[i].name))
				opt = opts[i].id;
	}
	else
	{
		/* check what option was previously */
		switch(opt)
		{
            case optSetupTC:
                this->m_setup_tc = atol((char*)pszParam);
                break;

			case optAlertTime:
				this->m_alert_before = atol((char*)pszParam);
				break;

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