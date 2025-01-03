#include <iostream>
#include <cassert>

//Реализуйте в классе MedalsTable из примера 18 возможность динамически задавать размер таблицы медалей.
//Текущая реализация для упрощения использует статический массив на 10 элементов.
//Замените его на динамически выделяемый массив.
//Для класса MedalsTable реализуйте семантику копирования и семантику перемещния(две пары конструктор / оператор присваивания)

class MedalRow
{
	static const int COUNTRY_MAX_CHARS = 4;
	static const int MEDALS_IDX = 3;
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
	void print()const
	{
		std::cout << '[' << country << "]-( ";
		for (int i{ 0 }; i < MEDALS_IDX; ++i)
		{
			std::cout << medals[i];
			if (i < 2) { std::cout << '\t'; }
		}
		std::cout << " )\n";
	}

};

class MedalsTable
{
	int maxSize{ 10 };
	MedalRow* medalRows = new MedalRow[maxSize];
	int size;
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
	void print()const
	{

		for (int i{ 0 }; i < size; ++i)
		{
			medalRows[i].print();
		}
	}
};
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
	mt1.print();
	// создаем константную копию таблицы №1
	std::cout << "\nMedals table #2:\n";
	const MedalsTable mt2{ mt1 };
	mt2.print();
	// раскомментировав следующую строку можно протестировать
	// проверку отсутствия страны в константной таблице
	// медалей
	// программа аварийно завершиться, что нормально!
	// mt2["SLO"].print();
	return 0;
}