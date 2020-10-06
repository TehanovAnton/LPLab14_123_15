#pragma once
#include<fstream>
#include"In.h"
#include"Parm.h"
#include<string>
#include<stdarg.h>
#include<time.h>
namespace Log	// работа с протоколом
{
	struct LOG	// протокол
	{
		wchar_t logfile[PARM_MAX_SIZE];		// имя файла протокола
		std::ofstream* stream;				// входной поток протокола
	};

	static const LOG INITLOG = { L"", NULL };	// структура начальной инициализации LOG
	LOG getlog(wchar_t logfile[]);

	void WriteLine(LOG log, char* c, ...);
	void WriteLine(LOG log, wchar_t c[], ...);
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteError(LOG log, Error::ERROR error);
	void Close(LOG log);
}