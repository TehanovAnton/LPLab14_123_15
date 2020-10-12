#pragma once
#include<iostream>
#include<cwchar>

#include"Error.h"
#include"Parm.h"
#include"In.h"
#include"Log.h"
#include"TCHAR.h"
#include<fstream>
#include"FST.h"
#include"IT.h"
#include"LT.h"
#include "SpecialSym.h"

#define TOKENINIT 1

void setDefValue(IT::Entry& entryI)
{
	if (entryI.iddatatype == IT::INT)
		entryI.values.vint = 0;
	else
	{
		entryI.values.vste->len = 0;
		entryI.values.vste->str[0] = '\0';
	}
}
void setEntryIId(IT::Entry& entryI, char lexem[])
{
	for (size_t i = 0; i < strlen(lexem) && i < ID_MAXSIZE; i++)
	{
		entryI.id[i] = lexem[i];
	}
}

void parsStringLiteraltoValue(char lexem[], char retValue[])
{
	size_t e = 1;
	for (; e < strlen(lexem) - 1; e++)
	{
		retValue[e - 1] = lexem[e];
	}
	retValue[e - 1] = '\0';
}

char* IdbyLexem(char lexem[])
{
	char* ID = new char[ID_MAXSIZE + 1];
	size_t i = 0;
	for (; i < strlen(lexem) && i < ID_MAXSIZE; i++)
	{
		ID[i] = lexem[i];
	}
	ID[i] = '\0';

	return ID;
}

void strcopy(char* destination, char source[])
{
	size_t i = 0;
	for (; i < strlen(source); i++)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

}

bool strcamper(char idtableEl[], char findingEl[])
{
	bool res = true;
	for (size_t i = 0; i < ID_MAXSIZE; i++)
	{
		res &= idtableEl[i] == findingEl[i];
	}

	return res;
}

void set_aE(IT::IdTable& idTable, int lstChngId, int aB, int aE);
void set_aB(LT::LexTable lexTable, IT::IdTable& idTable, int& lstChngId, int aB, int& aE);
void exposingNamespaces(LT::LexTable lexTable, IT::IdTable& idTable);

void FillLT(LT::LexTable& lexTable, LT::Entry& entryL, char shrtLex[], int line)
{
	// ���������� LT; ������ �� IT � ��� �������� ����� ��������������� ��� ���������� IT
	entryL = LT::Entry(shrtLex[0], line, LT_TILNULLIDX);

	LT::Add(lexTable, entryL);
}

bool FillIT(LT::LexTable& lexTable, IT::IdTable& idTable, IT::Entry& entryI, int lexCounter, char lexem[], int currentToken, int lastToken, int preLastToken, int& litCounter)
{
	entryI = IT::Entry();

	// ����������
	// �����������
	if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_DECLARE)
	{
		// ��� ����������
		setEntryIId(entryI, lexem);

		// ��� ������; ������������ �������� ������� �������� ��� ���������� �������
		entryI.iddatatype = lastToken == 1 ? IT::STR : IT::INT;

		// ��� ��������������
		entryI.idtype = IT::V;

		// ������ �� LT
		entryI.idxfirstLE = lexCounter;

		// �������� �� ���������
		setDefValue(entryI);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;

	}
	// �������
	// �����������
	else if (lexTable.table[lexTable.size - 2].lexema[0] == LEX_FUNCTION)
	{
		// ��� �������
		setEntryIId(entryI, lexem);

		// ��� ������; ������������ �������� ������� �������� ��� ���������� �������
		entryI.iddatatype = preLastToken == 1 ? IT::STR : IT::INT;

		// ��� ��������������
		entryI.idtype = IT::F;

		// ������ �� LT
		entryI.idxfirstLE = lexCounter;

		// �������� �� ���������
		setDefValue(entryI);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// ��������
	// �����������
	else if (lexTable.table[lexTable.size - 3].lexema[0] == LEX_LEFTHESIS || lexTable.table[lexTable.size - 3].lexema[0] == LEX_COMMA)
	{
		// ��� ���������
		setEntryIId(entryI, lexem);

		// ��� ������; ������������ �������� ������� �������� ��� ���������� �������
		entryI.iddatatype = lastToken == 1 ? IT::STR : IT::INT;

		// ��� ��������������
		entryI.idtype = IT::P;

		// ������ �� LT
		entryI.idxfirstLE = lexCounter;

		// �������� �� ���������
		setDefValue(entryI);

		// �������� ��������������� ������� � LT
		lexTable.table[lexTable.size - 1].idxTI = idTable.size;
	}
	// �������
	else if (currentToken == 3 || currentToken == 4)
	{
		if (currentToken == 4)
		{
			int vint = atoi(lexem);
			// ������ ����� �� int literal
			if (IT::IstdByValue(idTable, vint) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdByValue(idTable, vint);
				return true;
			}
			// �� ������ int literal
			else
			{
				// ��� ��������
				char buffer[1000];
				_itoa_s(++litCounter, buffer, 1000, 10);
				entryI.id[0] = 'L';
				strcopy(entryI.id + 1, buffer);

				// ��� ������; ������������ �������� ������� �������� ��� ���������� �������
				entryI.iddatatype = IT::INT;

				// ��� ��������������
				entryI.idtype = IT::L;

				// ������ �� LT
				entryI.idxfirstLE = lexCounter;

				// �������� �� ���������
				entryI.values.vint = vint;

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
		else
		{
			char value[1000];
			parsStringLiteraltoValue(lexem, value);
			// ������ ����� �� string literal
			if (IT::IstdByValue(idTable, value) != (int)TI_NULLIDX)
			{
				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = IT::IstdByValue(idTable, value);
				return true;
			}
			// �� ������ string literal
			else
			{
				// ��� ��������
				char buffer[1000];
				_itoa_s(++litCounter, buffer, 1000, 10);
				strcopy(entryI.id, buffer);

				// ��� ������; ������������ �������� ������� �������� ��� ���������� �������
				entryI.iddatatype = IT::STR;

				// ��� ��������������
				entryI.idtype = IT::L;

				// ������ �� LT
				entryI.idxfirstLE = lexCounter;

				// �������� �� ���������
				strcopy(entryI.values.vste->str, value);

				// �������� ��������������� ������� � LT
				lexTable.table[lexTable.size - 1].idxTI = idTable.size;
			}
		}
	}
	else
	{
		lexTable.table[lexTable.size - 1].idxTI = IT::IstdFromCurrent(idTable, IdbyLexem(lexem));
		return true;
	}

	IT::Add(idTable, entryI, lexTable.positions[lexCounter - 1].line, lexTable.positions[lexCounter - 1].colone);
	return true;
}

bool parsingLexem(char lexem[], LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry& entryL, IT::Entry& entryI, int line, int& litCounter, int& lastToken, int& preLastToken, int lexCounter)
{

#if TOKENINIT

	FST::FST leftBracetL(
		lexem,                                                                                                        // ������� ������������� 
		leftBracetLTok);

	FST::FST rightBracetL(
		lexem,                                                                                                        // ������� ������������� 
		rightBracetLTok);

	FST::FST leftFigureBracetL(
		lexem,                                                                                                        // ������� ������������� 
		leftFigureBracetLTok);

	FST::FST rightFigureBracetL(
		lexem,                                                                                                        // ������� ������������� 
		rightFigureBracetLTok);

	FST::FST commaL(
		lexem,                                                                                                        // ������� ������������� 
		commaLTok);

	FST::FST semicolonL(
		lexem,                                                                                                        // ������� ������������� 
		semicolonLTok);

	FST::FST plusL(
		lexem,                                                                                                        // ������� ������������� 
		plusLTok);

	FST::FST minusL(
		lexem,                                                                                                        // ������� ������������� 
		minusLTok);

	FST::FST multiplicationL(
		lexem,                                                                                                        // ������� ������������� 
		multiplicationLTok);

	FST::FST devisionL(
		lexem,                                                                                                        // ������� ������������� 
		devisionLTok);

	FST::FST equalL(
		lexem,                                                                                                        // ������� ������������� 
		equalLTok);

	FST::FST integerKeyWord(
		lexem,
		integerKWTOk);

	FST::FST stringKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		stringKWTok);

	FST::FST functionKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		functionKWTok);

	FST::FST declareKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		declareKWTok);

	FST::FST returnKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		returnKWTok);

	FST::FST printKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		printKWTok);

	FST::FST mainKeyWord(
		lexem,                                                                                                        // ������� ������������� 
		mainKWTok);

	FST::FST identificator(
		lexem,                                                                                                        // ������� ������������� 
		identificatorTok);

	FST::FST integerLiteral(
		lexem,
		integerLiteralTok);

	FST::FST stringLiteral(
		lexem,
		stringLiteralTok);

#endif // TOKENINIT	

	FST::FST tokenLexems[NUMLEXEM] = { mainKeyWord, stringKeyWord, integerKeyWord, stringLiteral, integerLiteral, functionKeyWord, declareKeyWord,
		returnKeyWord, printKeyWord, identificator, leftBracetL, rightBracetL, leftFigureBracetL, rightFigureBracetL, commaL, semicolonL, plusL,
		minusL, multiplicationL, devisionL,	equalL };

	char shortLexm[NUMLEXEM] = { LEX_ID, LEX_STRING, LEX_INTEGER, LEX_LITERAL, LEX_LITERAL , LEX_FUNCTION, LEX_DECLARE,
		LEX_RETURN, LEX_PRINT, LEX_ID, LEX_LEFTHESIS, LEX_RiGHTHESIS, LEX_LEFTBRACE, LEX_BRACELET, LEX_COMMA, LEX_SEMICOLON, LEX_PLUS,
		LEX_MINUS, LEX_STAR, LEX_DIRSLASH, LEX_EQUAl };

	for (size_t i = 0; i < NUMLEXEM; i++)
	{
		if (FST::execute(tokenLexems[i]))
		{
			char mainKW[] = "main";
			char shrtLex[2] = { shortLexm[i] , '\0' };
			if (strcamper(lexem, mainKW))
				shrtLex[0] = LEX_MAIN;


			FillLT(lexTable, entryL, shrtLex, line);

			if (shrtLex[0] == LEX_ID || shrtLex[0] == LEX_LITERAL)
			{
				FillIT(lexTable, idTable, entryI, lexCounter, lexem, i, lastToken, preLastToken, litCounter);
			}

			preLastToken = lastToken;
			lastToken = i;

			return true;
		}
		else
			continue;
	}

	throw ERROR_THROW_IN(115, line, lexTable.positions[lexCounter - 1].colone)

	return false;
}

std::string processText(std::string str)
{
	std::string resStr = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		// ������ ����� ���������
		if (str[i] == quotation)
		{
			resStr += str[i];
			i++;

			for (; str[i] != quotation; i++)
				resStr += str[i];

			resStr += str[i];
			continue;
		}

		if (!isSymInStr(str[i], spaceAndNewlineSym))
		{
			// ������ ������ �������� ����� �����������
			if (isSymInStr(str[i], separators) && isSymInStr(str[i + 1], spaceSym))
			{
				resStr += str[i];
				i++;
				continue;
			}

			resStr += str[i];
		}
		else
		{
			// ������ ������ �������� ����� ������������
			if (isSymInStr(str[i], spaceSym) && isSymInStr(str[i + 1], separators))
			{
				continue;
			}

			resStr += str[i];
			// ������ ������ �������� ����� ������������
			while (isSymInStr(str[i + 1], spaceAndNewlineSym)) i++;
		}
	}

	return resStr;
}

void LTITBuilding(LT::LexTable& lexTable, IT::IdTable& idTable, std::string orifginalStr)
{
	char longLexem[TI_STR_MAXSIZE];
	int litCounter = 0, lastToken = 0, preLastToken = 0, lexCounter = 0;
	LT::Entry entryL;
	IT::Entry entryI;
	for (size_t i = 0, e = 0, lexCounter = 0; i < orifginalStr.length(); i++, e++)
	{
		if (isSymInStr(orifginalStr[i], sepSpaNewlSym))
		{
			if (e != 0)
			{
				parsingLexem(longLexem, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);
			}

			if (!isSymInStr(orifginalStr[i], spaceAndNewlineSym))
			{
				char lexem[2];
				lexem[0] = orifginalStr[i]; lexem[1] = '\0';
				parsingLexem(lexem, lexTable, idTable, entryL, entryI, lexTable.positions[lexCounter++].line, litCounter, lastToken, preLastToken, lexCounter);
			}

			//����� ����� ������
			if (orifginalStr[i] == newlSym)
				e = -1;
			else
			{
				e = -1;
				memset(longLexem, 0, sizeof(char) * TI_STR_MAXSIZE);
			}
		}
		// ���������� �������
		else
		{
			// ������ ����� �������
			if (orifginalStr[i] == quotation)
			{
				longLexem[e] = orifginalStr[i]; i++; e++;
				longLexem[e + 1] = '\0';
				while (orifginalStr[i] != quotation)
				{
					longLexem[e] = orifginalStr[i];
					longLexem[e + 1] = '\0';
					i++; e++;
				}
				longLexem[e] = orifginalStr[i];
				longLexem[e + 1] = '\0';
				continue;
			}
			else
			{
				longLexem[e] = orifginalStr[i];
				longLexem[e + 1] = '\0';
			}
		}
	}
}

void set_aB(LT::LexTable lexTable, IT::IdTable& idTable, int& lstChngId, int aB, int& aE)
{
	for (int i = aB + 1; lexTable.table[i].lexema[0] != LEX_BRACELET &&
		i < lexTable.size &&
		!(lexTable.table[i].lexema[0] == LEX_RiGHTHESIS && lexTable.table[i + 1].lexema[0] == LEX_SEMICOLON); i++, aE = i)
	{
		LT::Entry elLT = lexTable.table[i];
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 1].lexema[0] == LEX_FUNCTION || elLT.lexema[0] == LEX_MAIN)
		{
			set_aB(lexTable, idTable, lstChngId, i, aE);

			set_aE(idTable, lstChngId, i, aE);
		}
		else
		{
			if (elLT.lexema[0] == LEX_ID && idTable.table[elLT.idxTI].vsbAr.aB == NOVSBAR)
			{ 
				idTable.table[elLT.idxTI].vsbAr.aB = aB;
				lstChngId = elLT.idxTI;
			}
			
			else
			{ continue; }
		}
	}
}

void set_aE(IT::IdTable& idTable, int lstChngId, int aB, int aE)
{
	for (int i = lstChngId; i >= 0; i--)
	{
		if (idTable.table[i].vsbAr.aB == aB)
		{
			idTable.table[i].vsbAr.aE = aE; 
		}
	}
}


bool operator <= (IT::visibleArea a, IT::visibleArea b)
{
	return (a.aB > b.aB && a.aE < b.aE) || (a.aB == b.aB && a.aE == b.aE);
}

bool operator != (IT::visibleArea a, IT::visibleArea b)
{
	return (a.aB < b.aB&& a.aE < b.aE) || (a.aB > b.aB && a.aE > b.aE);
}

bool operator == (IT::visibleArea a, IT::visibleArea b)
{
	return a.aB == b.aB&& a.aE == b.aE;
}

//bool operator > (IT::visibleArea a, IT::visibleArea b)
//{
//	return a.aB < b.aB&& a.aE > b.aE;
//}

void exposingNamespaces(LT::LexTable lexTable, IT::IdTable& idTable)
{
	int lasatChangedIdentificator = 0;
	for (int i = 0; i < lexTable.size; i++)
	{
		LT::Entry elLT = lexTable.table[i];
		if (elLT.lexema[0] == LEX_ID && lexTable.table[i - 1].lexema[0] == LEX_FUNCTION || elLT.lexema[0] == LEX_MAIN)
		{
			int aB = i;
			set_aB(lexTable, idTable, lasatChangedIdentificator, aB, i);

			set_aE(idTable, lasatChangedIdentificator, aB, i);
		}
	}

	// �������� �� ������ ���������������
	for (int i = 0; i < idTable.size; i++)
	{
		IT::Entry elIT = idTable.table[i];
		for (int e = i - 1; e >= 0; e--)
		{
			if (!(elIT.vsbAr != idTable.table[e].vsbAr) && elIT.idtype == idTable.table[e].idtype)
			{
				if (strcamper(elIT.id, idTable.table[e].id) && elIT.vsbAr <= idTable.table[e].vsbAr)
				{
					printf("��������������� ��������������: %s,\t{%d, %d}\n", elIT.id, lexTable.positions[elIT.idxfirstLE - 1].line, lexTable.positions[elIT.idxfirstLE - 1].colone);
				}
			}
		}

	}

}
