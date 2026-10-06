#include <iostream>
#include <Windows.h>

using namespace std;

int n = 0;
int m = 0;

float** matrix;

// Функция потока
DWORD WINAPI Sort(LPVOID param)
{
	// Получаем значение параметра
	int* prow = (int*)param;
	int row = *prow; // Берёт значение по адресу

	cout << "Поток получил строку: " << row << endl;

	for (int i = 0; i < m - 1; i++)
	{
		for (int j = 0; j < m - 1; j++)
		{
			if (matrix[row][j] > matrix[row][j + 1])
			{
				float temp = matrix[row][j];
				matrix[row][j] = matrix[row][j + 1];
				matrix[row][j + 1] = temp;
			}
		}
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "RU");

	cout << "Введите кол-во строк: ";
	cin >> n;
	cout << "Введите кол-во элементов в строке (столбцов): ";
	cin >> m;

	// Создание матрицы по размерам пользователя
	matrix = new float* [n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new float[m];
	}

	// Заполнение матрицы
	cout << "\nВведите значения матрицы:\n";
	for (int i = 0; i < n; i++)
	{
		cout << "Строка № " << i+1 << endl;
		cout << "Введите значение в ячейке: \n";
		for (int j = 0; j < m; j++)
		{
			float z = 0;
			cin >> z;
			matrix[i][j] = z;
		}
	}

	cout << "\nТест вывода матрицы без сортировки\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}

	HANDLE* hThread = new HANDLE[n];
	DWORD* dwThreadID = new DWORD[n];

	// Номер строк
	int* row_numbers = new int[n];

	// Создание потока
	for (int i = 0; i < n; i++)
	{
		row_numbers[i] = i;

		hThread[i] = CreateThread(
			NULL,			// Атрибут безопасности по умолчанию
			0,				// Размер стека по умолчанию
			Sort,			// Имя функции
			&(row_numbers[i]),	// Указатель на параметры (передаёт адрес ячейки памяти в param)
			0,				// Флаг создания
			&dwThreadID[i]	// Адрес переменной для идентификатора
		);
	}

	// Ожидание завершения всех потоков
	WaitForMultipleObjects(
		n,			// Кол-во потоков
		hThread,	// Указатель на массив указателей потоков
		true,		// Флаг ожидания. Показывает - нужно ли дождаться завершения всех потоков
		INFINITE	// Время ожидания завершения в миллисекундах
	);

	cout << "\nТест вывода матрицы после сортировки столбцов\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}