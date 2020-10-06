#include "PolishNotation.h"
#define macros 0

namespace PN
{ 
	PNstr::PNstr()
	{
		maxsize = TI_STR_MAXSIZE;
		size = 0;
		pnstr = new char[TI_STR_MAXSIZE];
	}

	PNstr::PNstr(int m, int s, char* pns)
	{
		maxsize = m;
		size = s;
		pnstr = pns;
	}

	SymWithPrioryty::SymWithPrioryty()
	{
		sym = LEX_PLUS;
		pRioryty = 2;
	}

	SymWithPrioryty::SymWithPrioryty(char s, short p)
	{
		sym = s;
		pRioryty = p;
	}	

	int GetPrioryty(char sym, SymWithPrioryty prior[])
	{
		for (size_t i = 0; i < SymWithPriorytyAmount; i++)
		{
			if (sym == prior[i].sym)
				return prior[i].pRioryty;
			else
				continue;
		}
		return 0;
	}

	void parseFunctoPnstr(char origStr[], PNstr& res, size_t& i, std::stack<char>& stck, SymWithPrioryty prioryties[])
	{
		char
			func = origStr[i++],
			lex = origStr[++i];
		size_t parmCounter = 0;
		for (; origStr[i] != LEX_RiGHTHESIS; lex = origStr[++i])
		{
			// операнд
			if (lex == LEX_ID && origStr[i + 1] != LEX_LEFTHESIS)
			{
				// запись операнда в порядке следования
				res.pnstr[res.size++] = lex;
				res.pnstr[res.size] = '\0';
				parmCounter++;
			}			
			// функция
			else if (origStr[i + 1] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				parseFunctoPnstr(origStr, res, i, stck, prioryties);
			}
			// операция
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top() == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lex);
				}
				// закрывающая скобка
				else if (lex == LEX_RiGHTHESIS)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top() != LEX_LEFTHESIS; )
					{
						res.pnstr[res.size++] = stck.top();
						res.pnstr[res.size] = '\0';
						stck.pop();
					}
					stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top(), Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top(), prioryties))
					{
						res.pnstr[res.size++] = stck.top();
						res.pnstr[res.size] = '\0';
						stck.pop();
						stck.push(lex);
					}
					// операция в строке проирететней чем в стеке
					else
						stck.push(lex);
				}
			}
			// выталкивание всех операций до опустошения стека
			else if (lex == LEX_COMMA)
			{
				for (; !stck.empty(); )
				{
					res.pnstr[res.size++] = stck.top();
					res.pnstr[res.size] = '\0';
					stck.pop();
				}
			}
			else
				continue;
		}
		// приобразование числа параметров в символ 
		char parCou[2];
		_itoa_s(parmCounter, parCou, 10);
		res.pnstr[res.size++] = parCou[0];
		res.pnstr[res.size++] = func;
		res.pnstr[res.size] = '\0';
	}

	bool PolishNotation(int expressionStart, char origStr[])
	{
		SymWithPrioryty prioryties[SymWithPriorytyAmount] = { prior_1, prior_2, prior_3 };
		PNstr res = PNstr();
		std::stack<char> stck;

		size_t i = expressionStart + 1;
		for (char lex = origStr[i]; origStr[i] != LEX_SEMICOLON; lex = origStr[++i])
		{
			// операнд
			if (lex == LEX_ID && origStr[i + 1] != LEX_LEFTHESIS)
			{
				// запись операнда в порядке следования
				res.pnstr[res.size++] = lex;
				res.pnstr[res.size] = '\0';
			}
			// функция, последнее условие обеспечивает попадание операции в стек
			else if (origStr[i + 1] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<char> stckintern;
				parseFunctoPnstr(origStr, res, i, stckintern, prioryties);
			}
			// знак операции или скобки 
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top() == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lex);
				}
				// закрывающая скобка
				else if (lex == LEX_RiGHTHESIS)
				{	
					// выталкивание операций до открывающей скобки
					for (; !stck.empty() &&  stck.top() != LEX_LEFTHESIS; )
					{
						res.pnstr[res.size++] = stck.top();
						res.pnstr[res.size] = '\0';
						stck.pop();
					}
					if (!stck.empty())
						stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top(), Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top(), prioryties))
					{
						res.pnstr[res.size++] = stck.top();
						res.pnstr[res.size] = '\0';
						stck.pop();
						stck.push(lex);
					}
					else
						stck.push(lex);
				}
				
			}
			// ошибка
			else
			{
				return false;
			}
		}
		// выталкивание операций до опустошения стека
		for (; !stck.empty(); )
		{
			res.pnstr[res.size++] = stck.top();
			res.pnstr[res.size] = '\0';
			stck.pop();
		}

		return true;
	}

#if macros

	bool PolishNotation(int expressionStart, LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		PNstr res = PNstr();
		std::stack<char> stck;

		size_t i = expressionStart + 1;
		for (char lex = lexTable.table[i].lexema[0]; lexTable.table[i].lexema[0] != LEX_SEMICOLON; lex = lexTable.table[++i].lexema[0])
		{
			// операнд
			if (lex == LEX_ID)
			{
				// операнд в порядке следования
				res.pnstr[res.size++] = lex;
				res.pnstr[res.size] = '\0';
			}
			// знак операции или откр. скобка 
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр скобка
				if (stck.empty() || stck.top() == LEX_LEFTHESIS)
				{

				}
			}
			// ошибка
			else
			{

			}
		}

		return false;
	}	

#endif // macros
}
