#include <iostream>
#include <cassert>

//23.12.2024
//Реализуйте в классе MedalsTable из примера 18 возможность динамически задавать размер таблицы медалей.
//Текущая реализация для упрощения использует статический массив на 10 элементов.
//Замените его на динамически выделяемый массив.
//Для класса MedalsTable реализуйте семантику копирования и семантику перемещния(две пары конструктор / оператор присваивания)
//
//25.12.2024
//Дополните решение из задания 2 реализацией оператора вызова функции для класса MedalsTable.
//Перегрузка должна принимать в качестве аргумента идентификатор страны и возвращать одну из констант :
//MedalRow::GOLD, MedalRow::SILVER, MedalRow::BRONZE как константу соответствующую максимальному количеству медалей для заданной страны.То есть если,
//к примеру, у Польши 2 золотые, 4 серебрянных и одна бронзовая медаль, то перегрузка оператора вызов функции с параметром POL вернет MedalRow::SILVER.
//
//27.12.2024
//Дополните решение из задания 1 перегрузкой оператора помещения в поток(operator<<) для классов MedalRow и MedalsTable, заменив тем самым соответствующие
//функции - члены print() в них

class MedalRow
{
#define  COUNTRY_MAX_CHARS 4
#define  MEDALS_IDX 3
	char country[COUNTRY_MAX_CHARS];
	int medals[MEDALS_IDX];
public:
	static const int GOLD{ 0 };
	static const int SILVER{ 1 };
	static const int BRONZE{ 2 };
	MedalRow(const char* countryP, const int* medalsP)
	{
		strcpy_s(country, COUNTRY_MAX_CHARS, countryP ? countryP : "NON");
		for (int i{ 0 }; i < MEDALS_IDX; ++i)
		{
			medals[i] = medalsP ? medalsP[i] : 0;
		}
	}
	//оператор присвоения
	MedalRow& operator=(const MedalRow& medalRow)
	{
		if (&medalRow != this) {
			strcpy_s(this->country, COUNTRY_MAX_CHARS, medalRow.country ? medalRow.country : "NON");
			for (int i{ 0 }; i < MEDALS_IDX; ++i)
			{
				medals[i] = medalRow.medals[i] ? medalRow.medals[i] : 0;
			}
		}
		return *this;
	}

	MedalRow() : MedalRow(nullptr, nullptr) {}

	// конструктор копирования
	MedalRow(const MedalRow& medalRow)
	{
		strcpy_s(this->country, COUNTRY_MAX_CHARS, medalRow.country ? medalRow.country : "NON");
		for (int i{ 0 }; i < MEDALS_IDX; ++i)
		{
			medals[i] = medalRow.medals[i] ? medalRow.medals[i] : 0;
		}
	}

	MedalRow& setCountry(const char* countryP)
	{
		if (countryP)
		{
			strcpy_s(country, COUNTRY_MAX_CHARS, countryP);
		}
		return *this;
	}
	const char* getCountry()const { return country; }

	int& operator[](int idx)
	{
		assert((idx >= 0 and idx < MEDALS_IDX) and "Index out "
			"of range!");
		return medals[idx];
	}
	int operator[](int idx)const
	{
		assert((idx >= 0 and idx < MEDALS_IDX) and "Index out "
			"of range!");
		return medals[idx];
	}
	//void print()const
	//{
	//	std::cout << '[' << country << "]-( ";
	//	for (int i{ 0 }; i < MEDALS_IDX; ++i)
	//	{
	//		std::cout << medals[i];
	//		if (i < 2) { std::cout << '\t'; }
	//	}
	//	std::cout << " )\n";
	//}

};

std::ostream& operator<< (std::ostream& os, MedalRow& medal_row)
{
	os << '[' << medal_row.getCountry() << "]-( ";
	for (int i{ 0 }; i < MEDALS_IDX; ++i)
	{
		os << medal_row[i];
		if (i < 2) { std::cout << '\t'; }
	}
	os << " )\n";
	return os;
}

class MedalsTable
{
	int maxSize{ 10 };
	int size;
	MedalRow* medalRows = new MedalRow[maxSize];
	
	int findCountry(const char* country)const
	{
		for (int i{ 0 }; i < size; ++i)
		{

			if (strcmp(medalRows[i].getCountry(),
				country) == 0)
			{
				return i;
			}
		}
		return -1;
	}
public:

	MedalsTable() : size{ 0 } {}

	~MedalsTable()
	{
		delete[]medalRows;
	}
	// конструктор копирования
	MedalsTable(const MedalsTable& medalsTable)
	{
		this->maxSize = medalsTable.maxSize;
		this->medalRows = new MedalRow[this->maxSize];
		this->size = medalsTable.size;
		for (int i = 0; i < medalsTable.maxSize; i++)
		{
			medalRows[i] = medalsTable.medalRows[i];
		}
	}
	// конструктор перемещения
	MedalsTable(MedalsTable&& medalsTable)noexcept
	{
		this->maxSize = medalsTable.maxSize;
		this->medalRows = medalsTable.medalRows;
		this->size = medalsTable.size;
		medalsTable.medalRows = nullptr;
	}
	//опрератор копирования
	MedalsTable& operator= (const MedalsTable& medalsTable)
	{
		if (&medalsTable != this) {
			this->maxSize = medalsTable.maxSize;
			this->medalRows = new MedalRow[this->maxSize];
			this->size = medalsTable.size;
			for (int i = 0; i < medalsTable.maxSize; i++)
			{
				medalRows[i] = medalsTable.medalRows[i];
			}
		}
		return *this;
	}
	//опрератор назначения
	MedalsTable& operator= (MedalsTable&& medalsTable)noexcept
	{
		if (&medalsTable != this) {
			delete[]this->medalRows; 
			this->maxSize = medalsTable.maxSize;
			this->medalRows = medalsTable.medalRows;
			this->size = medalsTable.size;
			medalsTable.medalRows = nullptr;
		}
		return *this;
	}

	MedalRow& operator[](const char* country)
	{
		int idx{ findCountry(country) };
		if (idx == -1)
		{
			assert(size < MedalsTable::maxSize and
				"Table is FULL!");
			idx = size++;
			medalRows[idx].setCountry(country);
		}
		return medalRows[idx];
	}
	const MedalRow& operator[](const char* country)const
	{
		int idx{ findCountry(country) };
		assert(idx != -1 and "Country not found on const "
			"table");
		return medalRows[idx];
	}


	/*void print()const
	{

		for (int i{ 0 }; i < size; ++i)
		{
			std::cout << medalRows[i];
		}
	}*/
	int getSize() const
	{
		return this->size;
	}
	MedalRow& getRows(int idx)const
	{
		return medalRows[idx];
	}

	int operator()(const char* country) const
	{
		int idx{ findCountry(country) };
		assert(idx != -1 && "Country not found");

		const MedalRow& row = medalRows[idx];
		int maxMedal = MedalRow::GOLD;
		if (row[MedalRow::SILVER] > row[maxMedal]) maxMedal = MedalRow::SILVER;
		if (row[MedalRow::BRONZE] > row[maxMedal]) maxMedal = MedalRow::BRONZE;
		return maxMedal;
	}
};


//
std::ostream& operator<<(std::ostream& os, const MedalsTable& medals_table)
{
	for (int i{ 0 }; i < medals_table.getSize() ; ++i)
	{
		os << medals_table.getRows(i);
	}
	return os;
}

int main()
{
	MedalsTable mt1;
	std::cout << "Medals table #1:\n";
	mt1["RUS"][MedalRow::GOLD] = 14;
	mt1["RUS"][MedalRow::SILVER] = 5;
	mt1["HUN"][MedalRow::BRONZE] = 9;
	mt1["HUN"][MedalRow::GOLD] = 7;
	mt1["POL"][MedalRow::GOLD] = 4;
	mt1["POL"][MedalRow::SILVER] = 2;

	//mt1.print();
	std::cout << mt1;

	// создаем константную копию таблицы №1
	std::cout << "\nMedals table #2:\n";
	const MedalsTable mt2{ mt1 };

	//mt2.print();
	std::cout << mt2;

	MedalsTable mt3(std::move(mt2));
	std::cout << "\nMedals table #3:\n";
	std::cout << mt3;
	//mt3.print();
	// раскомментировав следующую строку можно протестировать
	// проверку отсутствия страны в константной таблице
	// медалей
	// программа аварийно завершиться, что нормально!
	// mt2["SLO"].print();

	// Тест оператора ()
	std::cout << "\nMax medal for RUS: ";
	int maxMedal = mt1("RUS");
	if (maxMedal == MedalRow::GOLD) std::cout << "GOLD\n";
	else if (maxMedal == MedalRow::SILVER) std::cout << "SILVER\n";
	else std::cout << "BRONZE\n";

	return 0;
}