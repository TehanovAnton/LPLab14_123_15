#pragma once
#define ID_MAXSIZE 5                    // максимальное количество символов в идентификаторе
#define TI_MAXSIZE 4096                 // максимальное количество строк в таблице идентификатороф
#define TI_INT_DEFAULT 0x00000000       // значение по умолчанию для типа integer
#define TI_STR_DEFAULT 0x00             // значение по умолчанию для типа string
#define TI_NULLIDX 0xfffffffff          // нет элемента в таблице идентификатора 
#define TI_STR_MAXSIZE 255
#define NOVSBAR -1
#include "LT.h"
#include<iostream>
#include "Error.h"

namespace IT
{
    enum IDDATATYPE                     // типы данных идентификаторов: integer, string
    {
        INT = 1, STR = 2
    };

    enum IDTYPE
    {
        V = 1, F = 2, P = 3, L = 4      // типы иеднтификаторов: переменная, функция, параметр, литерал
    };

    struct visibleArea
    {
        int aB;
        int aE;

        visibleArea();
    };

    struct Entry                        // строка таблийцы идентификаторов 
    {
        int idxfirstLE;                 // индекс строки в таблице лексем
        char id[ID_MAXSIZE];            // идентификато(автоматически усекается до ID_MAXSIZE)
        IDDATATYPE iddatatype;          // тип данных
        IDTYPE idtype;                  // тип идентификаторов
        union
        {
            int vint;                               // значение integer
            struct
            {
                char len;                           // количество символо в string
                char str[TI_STR_MAXSIZE - 1];       // символы string
            }   vste[TI_STR_MAXSIZE];               // значение string

        } values;                                   // значение идентификатора
        visibleArea vsbAr;

    };

    struct IdTable
    {
        int maxsize;                                // ёмкость таблицы идентификаторов < TI_MAXSIZE
        int size;                                   // текущий таблицы идентификаторов < maxsize
        Entry* table;                               // массив строк таблицы идентификаторов

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

    Entry Getentry(                     // получить строку таблицы идентификаторов
        IdTable& idtable,               // экземпляр таблицы идентификаторов
        int n                           // номер вощвращаемой строки
    );

    int Istd(                           // возврат номера строки(еслти есть) или TI_NULLIDX
        IdTable& idtable,               // экземпляр таблицы идентификаторов
        char id[ID_MAXSIZE]             // идентификаторов
    );

    int IstdByValue(IdTable idTable, int value);

    int IstdByValue(IdTable idTable, char value[]);

    int IstdFromCurrent(IdTable& idtable, char id[ID_MAXSIZE]);

    void Delete(
        IdTable& idtable
    );
}