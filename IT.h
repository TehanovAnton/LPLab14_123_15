#pragma once
#define ID_MAXSIZE 5                    // ������������ ���������� �������� � ��������������
#define TI_MAXSIZE 4096                 // ������������ ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT 0x00000000       // �������� �� ��������� ��� ���� integer
#define TI_STR_DEFAULT 0x00             // �������� �� ��������� ��� ���� string
#define TI_NULLIDX 0xfffffffff          // ��� �������� � ������� �������������� 
#define TI_STR_MAXSIZE 255
#define NOVSBAR -1
#include "LT.h"
#include<iostream>
#include "Error.h"

namespace IT
{
    enum IDDATATYPE                     // ���� ������ ���������������: integer, string
    {
        INT = 1, STR = 2
    };

    enum IDTYPE
    {
        V = 1, F = 2, P = 3, L = 4      // ���� ���������������: ����������, �������, ��������, �������
    };

    struct visibleArea
    {
        int aB;
        int aE;

        visibleArea();
    };

    struct Entry                        // ������ �������� ��������������� 
    {
        int idxfirstLE;                 // ������ ������ � ������� ������
        char id[ID_MAXSIZE];            // ������������(������������� ��������� �� ID_MAXSIZE)
        IDDATATYPE iddatatype;          // ��� ������
        IDTYPE idtype;                  // ��� ���������������
        union
        {
            int vint;                               // �������� integer
            struct
            {
                char len;                           // ���������� ������� � string
                char str[TI_STR_MAXSIZE - 1];       // ������� string
            }   vste[TI_STR_MAXSIZE];               // �������� string

        } values;                                   // �������� ��������������
        visibleArea vsbAr;

    };

    struct IdTable
    {
        int maxsize;                                // ������� ������� ��������������� < TI_MAXSIZE
        int size;                                   // ������� ������� ��������������� < maxsize
        Entry* table;                               // ������ ����� ������� ���������������

        void PrintTable();
    };

    IdTable Create(
        int size // < TI_MAXSIZE
    );

    void Add(
        IdTable& idtable,
        Entry& entry,
        int pos_line,
        int pos_colone
    );

    Entry Getentry(                     // �������� ������ ������� ���������������
        IdTable& idtable,               // ��������� ������� ���������������
        int n                           // ����� ������������ ������
    );

    int Istd(                           // ������� ������ ������(����� ����) ��� TI_NULLIDX
        IdTable& idtable,               // ��������� ������� ���������������
        char id[ID_MAXSIZE]             // ���������������
    );

    int IstdByValue(IdTable idTable, int value);

    int IstdByValue(IdTable idTable, char value[]);

    int IstdFromCurrent(IdTable& idtable, char id[ID_MAXSIZE]);

    void Delete(
        IdTable& idtable
    );
}