#include"Parm.h"

namespace Parm
{
	PARM  getparm(int argc, _TCHAR* argv[])	//сформирровать struct parm на основе параметров main
	{
		PARM e;

		wchar_t pr[PARM_MAX_SIZE];	int inIndex = 0, outIndex = 0, logIndex = 0;
		if (argc == 1)
		{
			throw ERROR_THROW(100);
		}
		else
		{
			for (size_t i = 1; i < argc; i++)
			{
				wcsncpy_s(pr, argv[i], wcslen(PARM_IN));
				if (!wcscmp(pr, PARM_IN))
				{
					inIndex = i;
					continue;
				}

				wcsncpy_s(pr, argv[i], wcslen(PARM_OUT));
				if (!wcscmp(pr, PARM_OUT))
				{
					outIndex = i;
					continue;
				}

				wcsncpy_s(pr, argv[i], wcslen(PARM_LOG));
				if (!wcscmp(pr, PARM_LOG))
				{
					logIndex = i;
					continue;
				}
			}

			if (inIndex == 0)
				throw ERROR_THROW(100);

		}

		for (size_t i = 1; i < argc; i++)			// проверкак длин параметров
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);

		wchar_t defInLog[PARM_MAX_SIZE];	wcscpy_s(defInLog, argv[inIndex] + wcslen(PARM_IN));	wcscat_s(defInLog, PARM_LOG_DEFAULT_EXT);
		wchar_t defInOut[PARM_MAX_SIZE];	wcscpy_s(defInOut, argv[inIndex] + wcslen(PARM_IN));	wcscat_s(defInOut, PARM_OUT_DEFAULT_EXT);

		if (inIndex != 0 && outIndex != 0 && logIndex != 0) // заданы in out log
		{
			wcscpy_s(e.in, argv[inIndex] + wcslen(PARM_IN));
			wcscpy_s(e.out, argv[outIndex] + wcslen(PARM_OUT));
			wcscpy_s(e.log, argv[logIndex] + wcslen(PARM_LOG));
		}

		else if (inIndex != 0 && outIndex != 0 && logIndex == 0) // заданы in out
		{
			wcscpy_s(e.in, argv[inIndex] + wcslen(PARM_IN));
			wcscpy_s(e.out, argv[outIndex] + wcslen(PARM_OUT));
			wcscpy_s(e.log, defInLog);
		}

		else if (inIndex != 0 && outIndex == 0 && logIndex != 0) // заданы in Log
		{
			wcscpy_s(e.in, argv[inIndex] + wcslen(PARM_IN));
			wcscpy_s(e.log, argv[logIndex] + wcslen(PARM_LOG));
			wcscpy_s(e.out, defInOut);
		}

		else if (inIndex != 0 && outIndex == 0 && logIndex == 0) // задан in
		{
			wcscpy_s(e.in, argv[inIndex] + wcslen(PARM_IN));
			wcscpy_s(e.out, defInOut);
			wcscpy_s(e.log, defInLog);
		}

		return e;
	}
}