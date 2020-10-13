#pragma once
#include<stack>
#include"LT.h"
#include"IT.h"

#define SymWithPriorytyAmount 8
#define OperationsLEFTTHESIS "+-*/()[]"
#define Operations "+-*/"
#define prior_1 {'(', 1}, {')', 1}, {'[', 1}, {']', 1}
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

	struct PNLTstr
	{
		int maxsize;
		int size;
		LT::Entry* pnstr;
		PNLTstr();
		PNLTstr(int m);
		void Addpnstr(LT::Entry entryL, IT::IdTable& idTable, int indx);
	};

	struct SymWithPrioryty
	{
		char sym;
		short pRioryty;

		SymWithPrioryty();
		SymWithPrioryty(char s, short p);		
	};

	int GetPrioryty(char sym, SymWithPrioryty prior[]);

	void parseFunctoPnstr(char origStr[], PNstr& res, int& i, std::stack<char>& stck, SymWithPrioryty prioryties[]);
	void parseArrtoPnstr(char origStr[], PNstr& res, int& i, std::stack<char>& stck, SymWithPrioryty prioryties[]);
	bool PolishNotation(int expressionStart, char origStr[]);

	void parseFunctoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck, SymWithPrioryty prioryties[]);
	void parseArrtoPnstrLT(LT::LexTable& lexTable, IT::IdTable& idTable, PNLTstr& res, int& i, std::stack<LT::Entry>& stck, SymWithPrioryty prioryties[]);
	bool PolishNotation(int exprStart, LT::LexTable& lexTable, IT::IdTable& idTable, int& countEmptyEl);
}