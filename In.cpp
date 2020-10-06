#include"In.h"

namespace In
{
	IN getin(wchar_t infile[])
	{
		setlocale(LC_ALL, "rus");
		std::fstream file(infile, std::ios::in | std::ios::out);
		if (!file.is_open())
			ERROR_THROW(110);

		const unsigned int Code_table[] = IN_CODE_TABLE;

		In::IN ret;
		ret.size = ret.ignor = 0, ret.lines = 1;

		std::string text;
		int ignor = 0, size = 0;
		char ch, replaceCh = '\0';
		for (int colone = 1; file.get(ch); colone++)
		{
			size++;

			if (Code_table[(int)(unsigned char)ch] == ret.F)
			{
				throw ERROR_THROW_IN(111, ret.lines, colone);
			}

			else if (Code_table[(int)(unsigned char)ch] == ret.T)
			{
				if (ch == '\n')
				{
					ret.lines++;
					colone = 0;
				}

				text += ch;
			}

			else if (Code_table[(int)(unsigned char)ch] == ret.I)
			{
				ignor++;
				continue;
			}

			else
			{
				text += '-';
			}
		}

		ret.size = size;	ret.text = text;	ret.ignor = ignor;

		return ret;
	}
}