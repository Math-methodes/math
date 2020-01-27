#include <iostream> //Для потоков ввода-вывода
#include <locale.h> //Для русского языка
#include <stdlib.h>  //Для функции abs().
#include <conio.h>
#define TRUE 1
#define FALSE 0
#define MaxNodes 7   //Количество вершин.
#define MaxInt 1000  //Машинный эквивалент бесконечности.

using namespace std;

//Описание типа узла.
struct Uzel
{
  int Element; //Заданный номер вершины.
  int Propusk; //Пропускная способность.
  int Metka;   //Помечена вершина или нет.
};


class Spisok
{
  private:
      int C[MaxNodes][MaxNodes];    //Матрица начальных пропускных способностей.
      int c[MaxNodes][MaxNodes];    //Матрица конечных пропускных способностей.
      int Put[MaxNodes][MaxNodes];  //Матрица сквозных путей.
      int Potok [MaxNodes];         //Потоки.
      int Est (Uzel*,int,int);		//Функция проверяет, есть ли вершина k в пути S.
      int Tpk (int*,int,int);		//Функция проверяет, есть ли вершина k в массиве "тупиковых" вершин.
  public:
      void Vvod_Ves();
      int Reshenie ();
      void Vyvod(int);

};

void main()
{
	setlocale(LC_ALL, "Rus");
  Spisok A;

  A.Vvod_Ves();
  A.Vyvod(A.Reshenie());
  _getch();
}

void Spisok::Vvod_Ves()
//Ввод матрицы пропускных способностей.
{
  cout << "Вводите пропускные способности ребер:\n";
  for (int i=0;i<MaxNodes;i++)
   for (int j=0;j<MaxNodes;j++)
     {
       cout << "Введите C[" << (i+1) << "," << (j+1) << "]: "; 
       cin >> C[i][j];
       c[i][j] = C[i][j];
     }
}

void Spisok::Vyvod(int n)
//Вывод результатов.
{
  //Вычисление максимального объема потока.
	int sum = 0;
	for (int i = 0; i <= n; sum += Potok[i++]);
  cout << "\nМаксимальный объем потока в сети: " << sum;
  cout << "\nЗначения потоков по различным ребрам:\n";
  for (int i=0;i<MaxNodes;i++)
   for (int j=i;j<MaxNodes;j++)
     if (C[i][j])
     {
         cout << "Ребро (" << (i+1) << "," << (j+1) <<"): ";
         cout << "(" << C[i][j]  << "," << C[j][i] << ")-(";
         cout << c[i][j]  << "," << c[j][i] << ")=(";
         cout << (C[i][j]-c[i][j]) << "," << (C[j][i]-c[j][i]) << ") ";
         cout << "Поток: " << abs(C[i][j]-c[i][j]) << " ";
         if (C[i][j]-c[i][j]!=0)
         {
           cout << "Направление: ";
           if (C[i][j]-c[i][j]>0)
              cout << (i+1) << "->" << (j+1);
           else
              cout << (j+1) << "->" << (i+1);
         }
         cout << endl;
     }
}

int Spisok::Reshenie()
{
  Uzel SS[MaxNodes]; //Множество узлов, в которые можно перейти.
  Uzel S[MaxNodes]; //Путь.
  int i,j=0,k,el,mx,mn;
  int m; //Текущее количество вершин в пути.
  int nomer=-1; //Текущее количество сквозных потоков.
  int Tupik[MaxNodes]; //Перечень "тупиковых" вершин.
  int N_Tupik; //Количество элементов в массиве.

  while (j!=-1)
  {
    i=m=0;
    S[i].Element=0;
    S[i].Propusk=MaxInt;
    S[i].Metka=TRUE;
    el=0;
    N_Tupik=-1;
    while (el!=MaxNodes-1)
    {
      j=-1;
      for (k=0;k<MaxNodes;k++)
       if (c[i][k]!=0) //Если есть ненулевой поток...
        if (i>0)   //и в путь уже включены вершины...
        {
          if (!Est(&S[0],m,k) && !Tpk(&Tupik[0],N_Tupik,k)) 
                            //то включаем текущую вершину,
                           //если ее нет в пути и если она не "тупиковая".
          {  
             SS[++j].Element=k;
             SS[j].Propusk=c[i][k];
             SS[j].Metka=FALSE;
          }
        }
        else 
          if (!Tpk(&Tupik[0],N_Tupik,k)) //Не вернулись ли назад?
               //Поток не нулевой, и это первая вершина.
          {    //Включаем эту вершину в путь.
               SS[++j].Element=k;
               SS[j].Propusk=c[i][k];
               SS[j].Metka=FALSE;
          }
      if (j!=-1) //Есть продолжение.
      {
         mx=SS[0].Propusk;
         el=0;
         for (k=1;k<=j;k++)
          if (SS[k].Propusk>mx)
            { el=k; mx=SS[k].Propusk; }
         S[++m].Element=SS[el].Element;
         S[m].Propusk=mx;
         S[m].Metka=TRUE;
         if (SS[el].Element==MaxNodes-1) //Найден сквозной путь.
         {
           nomer++;
           //Запоминаем сквозной путь.
           for (k=0;k<=m;k++)
              Put[nomer][k]=S[k].Element;
           //Ищем минимальный поток.
           mn=S[0].Propusk;
           el=0;
           for (k=1;k<=m;k++)
            if (S[k].Propusk<mn)
              { el=k; mn=S[k].Propusk; }
           Potok[nomer]=mn; //Запоминаем его.
           //Вычисляем остаточные пропускные способности.
           for (k=0;k<m;k++)
           { 
             c[S[k].Element][S[k+1].Element] -= Potok[nomer];
             c[S[k+1].Element][S[k].Element] += Potok[nomer];
           }
           el=MaxNodes-1; //Переход к следующей итерации.
           j=0;
         }
         else i=S[m].Element;
      }
      else //Продолжения нет. Это возможно тогда, когда:
      {
         if (i==0)  //а) все пропускные способности нулевые.
                    //   В этом случае - выход
              el=MaxNodes-1;
         else       //б) мы попали в тупик. Запомним тупиковую вершину
                    //   в массиве и отступим назад на одну вершину.
         {
           Tupik[++N_Tupik]=S[m].Element;
           m--;
           i=S[m].Element;
         }
      }
    }
  }
  return nomer; //Возвращает количество сквозных потоков.
}

int Spisok::Est(Uzel S[], int m, int k)
//Функция проверяет, есть ли вершина k в пути S.
//m - текущее количество элементов в пути.
//Возвращает 1, если вершина есть, и 0 - в противном случае.
{
  for (int l=0;l<=m;l++)
    if (S[l].Element==k) return 1;
  return 0;
}

int Spisok::Tpk(int Tupik[],int N_Tupik, int k) 
//Функция проверяет, есть ли вершина k в массиве "тупиковых" вершин.
//N_Tupik - текущее количество вершин в массиве.
//Возвращает 1, если вершина есть, и 0 - в противном случае.
{
  if (N_Tupik==-1) return 0;
  for (int l=0;l<=N_Tupik;l++)
    if (Tupik[l]==k) return 1;
  return 0;
}
