#include "IT.h"
namespace IT
{
	visibleArea::visibleArea()
	{
		aB = aE = NOVSBAR;
	}

	IdTable Create(int size)
	{
		IdTable lextable = { size, 0, new Entry[size] };
		return lextable;
	}

	void Add(IdTable& idtable, Entry& entry, int pos_line, int pos_colone)
	{
		if (idtable.size == idtable.maxsize)
		{
			throw ERROR_THROW_IN(114, pos_line, pos_colone)
		}
		else
		{
			idtable.table[idtable.size] = entry;
			idtable.size++;
			entry = IT::Entry();
		}
	}

	Entry Getentry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int Istd(IdTable& idtable, char id[ID_MAXSIZE])
	{
		for (size_t i = 0; i < idtable.size; i++)
		{
			bool res = true;
			// сравнение строк
			for (size_t e = 0; e < ID_MAXSIZE && res; e++)
			{
				res &= idtable.table[i].id[e] == id[e];
			}

			if (res)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int IstdByValue(IdTable idTable, int value)
	{			
		for (size_t i = 0; i < idTable.size; i++)
		{
			if (value == idTable.table[i].values.vint)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int IstdByValue(IdTable idTable, char value[])
	{
		for (size_t i = 0; i < idTable.size; i++)
		{
			bool res = true;
			// сравнение значений по символам 
			for (size_t e = 0; e < strlen(value) && res; e++)
			{
				res &= idTable.table[i].values.vste->str[e] == value[e];
			}

			if (res)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int IstdFromCurrent(IdTable& idtable, char* id)
	{
		for (int i = idtable.size - 1; i >= 0; i--)
		{
			bool res = true;
			// сравнение строк
			for (size_t e = 0; e < strlen(id) && e < ID_MAXSIZE && res; e++)
			{
				res &= idtable.table[i].id[e] == id[e];
			}

			if (res)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idTable)
	{
		delete& idTable;
	}
	void IdTable::PrintTable()
	{
		std::cout << "\n\n";
		for (size_t i = 0; i < this->size; i++)
		{
			if (this->table[i].iddatatype == IT::INT)
			{
				std::printf("id: %s, value: %d, idxfirstLE: %d, datatype: INT\n", this->table[i].id, this->table[i].values.vint, this->table[i].idxfirstLE);
			}
			else
				std::printf("id: %s, value: %s, idxfirstLE: %d, datatype: STR;\n", this->table[i].id, this->table[i].values.vste->str, this->table[i].idxfirstLE);			//std::cout << this->table[i].id << " = " << this->table[i].values.vste->str << "; " << this->table[i].idxfirstLE;

			std::cout << '\n';
		}
	}
}