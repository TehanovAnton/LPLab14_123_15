#pragma once
#define LEXEMA_FIXSIZE 1			// ������������� ������ �������
#define LT_MAXSIZE 4096				// ������������ ���������� ����� � ������� ������
#define LT_TILNULLIDX 0xffffffff	// ��� �������� ������� ��������������� 
#define LEX_INTEGER 't'				// ������� ��� integer
#define LEX_STRING 't'				// ������� ��� string
#define LEX_ID	'i'					// ������� ��� ���������������
#define LEX_LITERAL 'l'				// ������� ��� ��������
#define LEX_FUNCTION 'f'			// ������� ��� function				
#define LEX_DECLARE 'd'				// ������� ��� declare					
#define LEX_RETURN 'r'				// ������� ��� return					
#define LEX_PRINT 'p'				// ������� ��� print						
#define LEX_SEMICOLON ';'			// ������� ��� ;						
#define LEX_COMMA ','				// ������� ��� ,							
#define LEX_LEFTBRACE '{'			// ������� ��� {						
#define LEX_BRACELET '}'			// ������� ��� }						
#define LEX_LEFTHESIS '('			// ������� ��� (						
#define LEX_RiGHTHESIS ')'			// ������� ��� )						
#define LEX_PLUS '+'				// ������� ��� +							
#define LEX_MINUS '-'				// ������� ��� -							
#define LEX_STAR '*'				// ������� ��� *							
#define LEX_DIRSLASH '/'			// ������� ��� /
#define LEX_EQUAl '='				// ������� ��� =
#define LEX_MAIN 'm'				// �������� ��� main
#define LEX_LSQBRACKET '['
#define LEX_RSQBRACKET ']'

#include "IT.h"
#include "SpecialSym.h"
#include "Error.h"
#include<iostream>
#include<string>

bool isSymInStr(char sym, const char str[]);

namespace LT							// ������� ������		
{
	struct Entry						// ������ ������� ������
	{
		char lexema[LEXEMA_FIXSIZE + 1];	// �������			
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� L�_�I_NULLIDX
		Entry();

		Entry(char shortLexm, int line, int idxTI);
	};

	struct Position						// ��������� ������ ��������� ��������� �������� ������ ���������
	{
		unsigned int colone;
		unsigned int line;
		Position();
		Position(int cl, int ln);
	};

	struct LexTable						// ��������� ������� ������
	{
		int maxsize;				    // ������� ������� ������ < LT_MAXSIZE
		int size;						// ������� ������ ������� ������ < maxsize
		Entry* table;					// ������ ����� ������� ������
		int posSize;
		Position* positions;				// ������ ��������� ������

		void GetLexemsPosition(std::string  originalText);		// ������� ��������� ��������� ����� �������� � ��������� ������ ����������
		void PrintTable();										// ����� �������
	};

	LexTable Create(								// ������� ������� ������
		int size						// ������� ������� ������ < LT_MAXSIZE
	);

	void Add(										// �������� ������ � ������� ������
		LexTable& lextable,					// ��������� ������� ������
		Entry& entry							// ������ ������� ������
	);

	Entry GetEntry(
		LexTable& lextable,				// ��������� ������� ������
		int n							// ����� ���������� ������
	);

	int IstdByID(LexTable lexTable, char value[]);

	void Delete(									// ������� ������� ������ (���������� ������)
		LexTable& lextable
	);
}