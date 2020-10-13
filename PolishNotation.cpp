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

	PNLTstr::PNLTstr()
	{
		maxsize = TI_STR_MAXSIZE;
		size = 0;
		pnstr = new LT::Entry[TI_STR_MAXSIZE];
	}

	PNLTstr::PNLTstr(int m)
	{
		maxsize = m;
		size = 0;
		pnstr = new LT::Entry[m];
	}

	void PNLTstr::Addpnstr(LT::Entry entryL, IT::IdTable& idTable, int indx)
	{
		this->pnstr[indx] = entryL;
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

	void parseFunctoPnstr(char origStr[], PNstr& res, int& i, std::stack<char>& stck, SymWithPrioryty prioryties[])
	{
		char
			func = origStr[i++],
			lex = origStr[++i];
		int parmCounter = 0;
		for (; origStr[i] != LEX_RiGHTHESIS; lex = origStr[++i])
		{
			// операнд
			if (lex == LEX_ID && origStr[i + 1] != LEX_LEFTHESIS || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.pnstr[res.size++] = lex;
				res.pnstr[res.size] = '\0';
				parmCounter++;
			}			
			// функция
			else if (origStr[i + 1] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<char> stckintern;
				parseFunctoPnstr(origStr, res, i, stckintern, prioryties);
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
		// выталкивание операций до опустошения стека
		for (; !stck.empty(); )
		{
			res.pnstr[res.size++] = stck.top();
			res.pnstr[res.size] = '\0';
			stck.pop();
		}
		// приобразование числа параметров в символ 
		char parCou[2];
		_itoa_s(parmCounter, parCou, 10);
		res.pnstr[res.size++] = parCou[0];
		res.pnstr[res.size++] = func;
		res.pnstr[res.size] = '\0';
	}
	void parseArrtoPnstr(char origStr[], PNstr& res, int& i, std::stack<char>& stck, SymWithPrioryty prioryties[])
	{
		char			
			lex = origStr[++i];
		int parmCounter = 0;
		for (; origStr[i] != LEX_RSQBRACKET; lex = origStr[++i])
		{
			// операнд
			if (lex == LEX_ID || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.pnstr[res.size++] = lex;
				res.pnstr[res.size] = '\0';
				parmCounter++;
			}
			// функция
			/*else if (origStr[i + 1] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				parseFunctoPnstr(origStr, res, i, stck, prioryties);
			}*/
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<char> stckintern;
				parseArrtoPnstr(origStr, res, i, stckintern, prioryties);
			}
			// операция
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top() == LEX_LSQBRACKET || lex == LEX_LSQBRACKET)
				{
					stck.push(lex);
				}
				// закрывающая скобка
				else if (lex == LEX_RSQBRACKET)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top() != LEX_RSQBRACKET; )
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
		for (; !stck.empty(); )
		{
			res.pnstr[res.size++] = stck.top();
			res.pnstr[res.size] = '\0';
			stck.pop();
		}
		// приобразование числа параметров в символ 
		char parCou[2];
		_itoa_s(parmCounter, parCou, 10);
		res.pnstr[res.size++] = parCou[0];
		res.pnstr[res.size++] = '@';
		res.pnstr[res.size] = '\0';
	}
	bool PolishNotation(int expressionStart, char origStr[])
	{
		SymWithPrioryty prioryties[SymWithPriorytyAmount] = { prior_1, prior_2, prior_3 };
		PNstr res = PNstr();
		std::stack<char> stck;

		int i = expressionStart + 1;
		for (char lex = origStr[i]; origStr[i] != LEX_SEMICOLON; lex = origStr[++i])
		{
			// операнд
			if ((lex == LEX_ID && origStr[i + 1] != LEX_LEFTHESIS) || lex == LEX_LITERAL)
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
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<char> stckintern;
				parseArrtoPnstr(origStr, res, i, stckintern, prioryties);
			}
			// знаки операций или скобки 
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top() == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lex);
				}
				// закрывающая скобка
				else if (lex == LEX_RiGHTHESIS || lex == LEX_RSQBRACKET)
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

		std::cout << res.pnstr;

		return true;
	}


	void parseFunctoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck,
		SymWithPrioryty prioryties[])
	{
		LT::Entry
			func = lexTable.table[i++],
			lexLT = lexTable.table[++i];
		int parmCounter = 0;
		for (char lex = lexTable.table[i].lexema[0]; lexTable.table[i].lexema[0] != LEX_RiGHTHESIS;
			lex = lexTable.table[++i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if (lex == LEX_ID && lexTable.table[i + 1].lexema[0] != LEX_LEFTHESIS || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++);
				parmCounter++;
			}
			// функция
			else if (lexTable.table[i + 1].lexema[0] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<LT::Entry> stckintern;
				parseFunctoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// операция
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RiGHTHESIS)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top().lexema[0] != LEX_LEFTHESIS; )
					{					
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
					}
					stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().lexema[0], Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().lexema[0], prioryties))
					{						
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
						stck.push(lexLT);
					}
					// операция в строке проирететней чем в стеке
					else
						stck.push(lexLT);
				}
			}
			// выталкивание всех операций до опустошения стека
			else if (lex == LEX_COMMA)
			{
				for (; !stck.empty(); )
				{
					res.Addpnstr(stck.top(), idTable, res.size++);
					stck.pop();
				}
			}
			else
				continue;
		}
		// выталкивание операций до опустошения стека
		for (; !stck.empty(); )
		{
			res.Addpnstr(stck.top(), idTable, res.size++);
			stck.pop();
		}

		// приобразование числа параметров в символ 
		char parmCounterStr[2];
		_itoa_s(parmCounter, parmCounterStr, 10);
		LT::Entry el = LT::Entry(parmCounterStr[0], -1,-1);

		res.Addpnstr(el, idTable, res.size++);
		res.Addpnstr(func, idTable, res.size++);
	}
	void parseArrtoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck,
		SymWithPrioryty prioryties[])
	{
		char
			lex = lexTable.table[++i].lexema[0];
		int parmCounter = 0;
		for (LT::Entry lexLT = lexTable.table[i]; lexTable.table[i].lexema[0] != LEX_RSQBRACKET;
			lex = lexTable.table[++i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if (lex == LEX_ID || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++);
				parmCounter++;
			}
			// функция
			/*else if (origStr[i + 1] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				parseFunctoPnstr(origStr, res, i, stck, prioryties);
			}*/
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<LT::Entry> stckintern;
				parseArrtoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// операция
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LSQBRACKET || lex == LEX_LSQBRACKET)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RSQBRACKET)
				{
					// выталкивание операций до открывающей скобки
					for (; stck.top().lexema[0] != LEX_RSQBRACKET; )
					{
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
					}
					stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().lexema[0], Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().lexema[0], prioryties))
					{
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
						stck.push(lexLT);
					}
					// операция в строке проирететней чем в стеке
					else
						stck.push(lexLT);
				}
			}
			// выталкивание всех операций до опустошения стека
			else if (lex == LEX_COMMA)
			{
				for (; !stck.empty(); )
				{
					res.Addpnstr(stck.top(), idTable, res.size++);
					stck.pop();
				}
			}
			else
				continue;
		}
		for (; !stck.empty(); )
		{
			res.Addpnstr(stck.top(), idTable, res.size++);
			stck.pop();
		}

		// приобразование числа параметров в символ 
		char parmCounterStr[2];
		_itoa_s(parmCounter, parmCounterStr, 10);
		LT::Entry el = LT::Entry(parmCounterStr[0], -1, -1);
		LT::Entry elArr = LT::Entry('@', -1, -1);

		res.Addpnstr(el, idTable, res.size++);
		res.Addpnstr(elArr, idTable, res.size++);
	}
	bool PolishNotation(int expressionStart, LT::LexTable& lexTable, IT::IdTable& idTable, int& countEmptyEl)
	{
		SymWithPrioryty prioryties[SymWithPriorytyAmount] = { prior_1, prior_2, prior_3 };
		PNLTstr res = PNLTstr();
		std::stack<LT::Entry> stck;

		int i = expressionStart + 1;
		char lex = lexTable.table[i].lexema[0];
		for (LT::Entry lexLT = lexTable.table[i]; lexTable.table[i].lexema[0] != LEX_SEMICOLON;
			lex = lexTable.table[++i].lexema[0], lexLT = lexTable.table[i])
		{
			// операнд
			if ((lex == LEX_ID && lexTable.table[i + 1].lexema[0] != LEX_LEFTHESIS) || lex == LEX_LITERAL)
			{
				// запись операнда в порядке следования
				res.Addpnstr(lexLT, idTable, res.size++);
			}
			// функция, последнее условие обеспечивает попадание операции в стек
			else if (lexTable.table[i + 1].lexema[0] == LEX_LEFTHESIS && lex == LEX_ID)
			{
				std::stack<LT::Entry> stckintern;
				parseFunctoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// массив 
			else if (lex == LEX_LSQBRACKET)
			{
				std::stack<LT::Entry> stckintern;
				parseArrtoPnstrLT(lexTable, idTable, res, i, stckintern, prioryties);
			}
			// знаки операций или скобки 
			else if (isSymInStr(lex, OperationsLEFTTHESIS))
			{
				// стек пуст или в вершине откр. скобка
				if (stck.empty() || stck.top().lexema[0] == LEX_LEFTHESIS || lex == LEX_LEFTHESIS)
				{
					stck.push(lexLT);
				}
				// закрывающая скобка
				else if (lex == LEX_RiGHTHESIS || lex == LEX_RSQBRACKET)
				{
					// выталкивание операций до открывающей скобки
					for (; !stck.empty() && stck.top().lexema[0] != LEX_LEFTHESIS; )
					{
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
					}
					if (!stck.empty())
						stck.pop();
				}
				// в верщине стека и строки опрерация 
				else if (isSymInStr(lex, Operations) && isSymInStr(stck.top().lexema[0], Operations))
				{
					// операция выталкивает из стека операцию с большим либо равным приоритетом
					if (GetPrioryty(lex, prioryties) <= GetPrioryty(stck.top().lexema[0], prioryties))
					{
						res.Addpnstr(stck.top(), idTable, res.size++);
						stck.pop();
						stck.push(lexLT);
					}
					else
						stck.push(lexLT);
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
			res.Addpnstr(stck.top(), idTable, res.size++);
			stck.pop();
		}

		// вставка обработанной части LT 
		for (int i = expressionStart + 1, e = 0; lexTable.table[i].lexema[0] != LEX_SEMICOLON; i++,  e++)
		{
			if (e < res.size)
			{
				lexTable.table[i] = res.pnstr[e];

				if (!LT::ITPointerBefore(lexTable, i - 1, lexTable.table[i].idxTI))
				{
					idTable.table[lexTable.table[i].idxTI].idxfirstLE = i;
				}
			}
			else
			{
				lexTable.table[i].sn = lexTable.table[i].idxTI = TI_NULLIDX;
				lexTable.table[i].lexema[0] = '\0';
				countEmptyEl++;
			}
		}

		return true;
	}
}
