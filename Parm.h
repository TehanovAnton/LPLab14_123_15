#pragma once
#define PARM_IN L"-in:"					// ключ для файла исходного кода
#define PARM_OUT L"-out:"				// ключ для файла обЪектного кода
#define PARM_LOG L"-log:"				// ключ для журнала
#define PARM_MAX_SIZE 300				// макскимальная длинна строки параметра
#define PARM_OUT_DEFAULT_EXT L".out"	// расширение файла обЪектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"	// расширение файла протокола по умолчанию
#include"TCHAR.h"
#include"Error.h"
#include<iostream>

namespace Parm							// обработка входных параметров
{
	struct PARM							// входные параметры
	{
		wchar_t in[PARM_MAX_SIZE];		// -in: имя фаайла исходного кода
		wchar_t out[PARM_MAX_SIZE];		// -out: имя фаайла обЪектного кода
		wchar_t log[PARM_MAX_SIZE];		// -log: имя фаайла протокола
	};

	PARM  getparm(int argc, _TCHAR* argv[]);
}