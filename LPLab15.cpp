#include "LPLab15.h"
#include "PolishNotation.h"

#define LPLab 1	
// индексы лексемы LEX_EQUALS
#define EXP1 28 // позиция первогог выражения
#define EXP2 50 // позиция второго выражения
#define EXP1 66 // позиция третьего выражения
// и т.д.
													 // [i,i] -> i(i, i)
#define test0 "=i-l*i;"								 
#define test1 "=(l+l)*(l+l)-i;"
#define test2 "=i*i(i+i(i(i,i)+i,i))-i/i(i,i);"
#define test3 "=i*[i+[[i,i]+i,i]]-i/[i,i];"
#define test4 "=i*i()-i/i(i,i);"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{		
#if LPLab

		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << "-in:" << parm.in << ", -out:" << parm.out << ", log:" << parm.log << std::endl << std::endl;

		log = Log::getlog(parm.log);

		In::IN in = In::getin(parm.in);
		std::cout << in.text << "\n\n";
		std::cout << "всего символов: " << in.size << std::endl;
		std::cout << "всего строк: " << in.lines << std::endl;
		std::cout << "пропущенно: " << in.ignor << "\n\n";

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		std::fstream file(parm.in, std::ios::in | std::ios::out);

		// считал в переменную текст
		std::string str = "";
		for (char ch; file.get(ch); ) { str += ch; }
		file.close();

		// раставлени сепараторов, очистка от повторяющихся переносов и пробелов
		// LPLab14-1
		std::string resStr = processText(str);

		LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
		lexTable.GetLexemsPosition(str);					// заполнение массива положений

		// запись в parm.out
		file.open(parm.out, std::ios::out);
		file << resStr;
		resStr.clear();
		file.close();

		IT::IdTable idTable = IT::Create(TI_MAXSIZE);

		// LPLab14-2
		LTITBuilding(lexTable, idTable, str);

		exposingNamespaces(lexTable, idTable);

		//LPLab15
		ChangeLTWithPN(lexTable, idTable);

		//вывод LT
		lexTable.PrintTable();

		//вывод IT
		idTable.PrintTable();

		Log::Close(log);

#endif // LPLab
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message;	printf("\t(%d, %d)\n\n", e.intext.line, e.intext.col);

		Log::WriteError(log, e);
		Log::Close(log);
	}
	return 0;
}