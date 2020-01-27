#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <iostream>

#define CMAX  10  
#define VMAX  10  

int NC, NV, NOPTIMAL, P1, P2, XERR;
double TS[CMAX][VMAX]; // Матрица с максимально допустимым размером 10х10
int count = 1;

void Data() {
	double R1, R2;
	char R;
	int I, J;
	printf("\n Симплекс-метод by Yankin Georgij\n\n");
	printf(" Найти максимум или минимум (Y - максимум /N - минимум) ? "); scanf_s("%c", &R);
	printf("\n Введите количество переменных в целевой функции? "); scanf_s("%d", &NV);
	printf("\n Введите количество неравенств? "); scanf_s("%d", &NC);
	if (R == 'Y' || R == 'y')
		R1 = 1.0;
	else
		R1 = -1.0;
	printf("\n Введите коеффициенты целевой функции:\n");

	for (J = 1; J <= NV; J++) {
		printf("       #%d ? ", J); scanf_s("%lf", &R2); // Запись целевой функции
		TS[1][J + 1] = R2 * R1;
	}

	printf("       Число после знака неравенства ? "); scanf_s("%lf", &R2);
	TS[1][1] = R2 * R1;

	for (I = 1; I <= NC; I++) {
		printf("\n Введите коеффициенты неравенства #%d:\n", I);
		for (J = 1; J <= NV; J++) {
			printf("       #%d ? ", J); scanf_s("%lf", &R2); // Ввод коеффициентов для каждого ограничения(неравенства)
			TS[I + 1][J + 1] = -R2;
		}
		printf("       Число после знака неравенства ? "); scanf_s("%lf", &TS[I + 1][1]);
	}

	printf("\n\n Результаты:\n\n");
	for (J = 1; J <= NV; J++)  TS[0][J + 1] = J;
	for (I = NV + 1; I <= NV + NC; I++)  TS[I - NV + 1][0] = I;
}

void Pivot();
void Formula();
void Optimize();

void Simplex() { // Главная функция, которая выполняет расчет метода
e10: Pivot();
	Formula();
	Optimize();
	if (NOPTIMAL == 1) goto e10;
}

void Pivot() { // Первая инициализация и перерасчет симплекс таблицы

	double RAP, V, XMAX;
	int I, J;
	std::cout << "Итерация номер:" << count;
	printf("\n");
	printf("\n");

	for (int i = 1; i <= NC + 1; i++)
	{
		for (int j = 1; j <= NV + 1; j++)
		{
			if ((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 3 && j == 1) || (i == 4 && j == 1)) {
				if (i == 1 && j == 1) {
					std::cout << "B" << "\n";
				}
				std::cout.precision(2);
				std::cout << "|" << TS[i][j] << "|";
				printf("\t\t");
			}
			else {
				std::cout.precision(2);
				std::cout << "|" << -TS[i][j] << "|";
				printf("\t\t");
			}
		}
		printf("\n");
	}
	count = count + 1;
	if (count == 2) {
		printf("\n       Значение целевой функции: %0.2f\n", TS[1][1]);
	}

	printf("\n");

	XMAX = 0.0;
	for (J = 2; J <= NV + 1; J++) {
		if (TS[1][J] > 0.0 && TS[1][J] > XMAX) {
			XMAX = TS[1][J];
			P2 = J;
		}
	}
	RAP = 999999.0;
	for (I = 2; I <= NC + 1; I++) {
		if (TS[I][P2] >= 0.0) goto e10;
		V = fabs(TS[I][1] / TS[I][P2]);
		if (V < RAP) {
			RAP = V;
			P1 = I;
		}
	e10:;
	}

	V = TS[0][P2];
	TS[0][P2] = TS[P1][0];
	TS[P1][0] = V;
}

void Formula() { // Функция для нахождения разрешающих строк и столбцов
	;
	//Labels: e60,e70,e100,e110;
	int I, J;

	for (I = 1; I <= NC + 1; I++) {
		if (I == P1) goto e70;
		for (J = 1; J <= NV + 1; J++) {
			if (J == P2) goto e60;
			TS[I][J] -= TS[P1][J] * TS[I][P2] / TS[P1][P2];
		e60:;
		}
	e70:;
	}

	TS[P1][P2] = 1.0 / TS[P1][P2];

	for (J = 1; J <= NV + 1; J++) {
		if (J == P2) goto e100;
		TS[P1][J] *= fabs(TS[P1][P2]);
	e100:;
	}
	for (I = 1; I <= NC + 1; I++) {
		if (I == P1) goto e110;
		TS[I][P2] *= TS[P1][P2];
	e110:;
	}

}

void Optimize() {  // Проверка решения на оптимальность
	int I, J;
	for (I = 2; I <= NC + 1; I++)
		if (TS[I][1] < 0.0)  XERR = 1;
	NOPTIMAL = 0;
	if (XERR == 1) {
		return;
	}
	for (J = 2; J <= NV + 1; J++)
		if (TS[1][J] > 0.0)  NOPTIMAL = 1;
}

void Results() { // Вывод результатов работы программы на экран
	//Labels: e30,e70,e100;
	int I, J;
	if (count >= 2) {
		std::cout << "Последняя итерация";
		printf("\n");
		printf("\n");
		for (int i = 1; i <= NC + 1; i++)
		{
			for (int j = 1; j <= NV + 1; j++)
			{
				if ((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 3 && j == 1) || (i == 4 && j == 1)) {
					if (i == 1 && j == 1) {
						std::cout << "B" << "\n";
					}
					std::cout.precision(2);
					std::cout << "|" << TS[i][j] << "|";
					printf("\t\t");
				}
				else {
					std::cout.precision(2);
					std::cout << "|" << -TS[i][j] << "|";
					printf("\t\t");
				}
			}
			printf("\n");
		}
		if (XERR == 0) goto e30;
		printf(" NO SOLUTION.\n"); goto e100;
	e30:for (I = 1; I <= NV; I++)
		for (J = 2; J <= NC + 1; J++) {
			if (TS[J][0] != 1.0*I) goto e70;
			printf("\n");
			printf("       Значение #%d: %0.2f\n", I, TS[J][1]);
		e70:;
		}
		printf("\n       Значение целевой функции: %0.2f\n", TS[1][1]);
	e100:printf("\n");
	}
}

void main() {

	setlocale(LC_ALL, "Russian");

	Data();
	Simplex();
	Results();
	_getch();

}