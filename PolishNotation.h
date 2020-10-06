#pragma once
#include<stack>
#include"LT.h"
#include"IT.h"

#define SymWithPriorytyAmount 6
#define OperationsLEFTTHESIS "+-*/()"
#define Operations "+-*/"
#define prior_1 {'(', 1}, {')', 1}
#define prior_2 {'+', 2}, {'-', 2}
#define prior_3 {'*', 3}, {'/', 3}

namespace PN // PolishNotation
{
	struct PNstr
	{
		int maxsize;
		int size;
		char* pnstr;
		PNstr();
		PNstr(int m, int s, char* pns);
	};

	struct SymWithPrioryty
	{
		char sym;
		short pRioryty;

		SymWithPrioryty();
		SymWithPrioryty(char s, short p);		
	};

	int GetPrioryty(char sym, SymWithPrioryty prior[]);

	void parseFunctoPnstr(char origStr[], PNstr& res, size_t& i, std::stack<char>& stck, SymWithPrioryty prioryties[]);

	bool PolishNotation(int expressionStart, char origStr[]);

	//bool PolishNotation(int exprStart, LT::LexTable& lexTable, IT::IdTable& idTable);
}