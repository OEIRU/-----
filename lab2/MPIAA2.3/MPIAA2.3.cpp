#include <omp.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
using namespace std;
int main()
{
   SetConsoleOutputCP(1251);
   int n = 2500;
   double* U = NULL;
   if (U == NULL)
      U = new double[n * n];
   for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
      {
         if (i == j)
            U[n * i + j] = 10;
         else if (i < j)
            U[n * i + j] = 1;
         else
            U[n * i + j] = 0;
      }
   double* x = NULL;
   if (x == NULL)
      x = new double[n];
   for (int i = 0; i < n; i++)
         x[i] = rand() % 11;
   ofstream outx;
   outx.open("x.txt");
   for (int i = 0; i < n; i++)
      outx << x[i] << " ";
   outx.close();
   double* b = NULL;
   if (b == NULL)
      b = new double[n];
   for (int i = 0; i < n; i++)
      b[i] = 0;
   for (int i = 0; i < n; i++) // Ux = b
      for (int k = 0; k < n; k++)
         b[i] += U[i * n + k] * x[k];
   double* y = NULL;
   if (y == NULL)
      y = new double[n];
   double start_time = clock();
   for (int i = n - 1; i > -1; i--) //Uy = b последовательно
   {
      y[i] = b[i];
      for (int j = i + 1; j < n; j++)
         y[i] -= U[n * i + j] * y[j];
      y[i] /= U[i * n + i];
   }
   double end_time = clock();
   double search_time1 = end_time - start_time;
   cout << "¬рем€ параллельного последовательного вычислени€: " << fixed <<
      search_time1 << endl;
   double norm1 = 0;
   for (int i = 0; i < n; i++)
      norm1 += y[i] * y[i];
   cout << "Ќорма вектора y при послеодовательном вычислении: " << sqrt(norm1) <<
      endl;
   start_time = clock();
   double sum = 0;
   for (int i = n - 1; i > -1; i--) //Uy = b 2 потока
   {
      sum = b[i];
#pragma omp parallel for num_threads(2) reduction(+:sum)
      for (int j = n - 1; j > i; j--)
         sum -= U[n * i + j] * y[j];
      y[i] = sum / U[i * n + i];
   }
   end_time = clock();
   double search_time2 = end_time - start_time;
   cout << "¬рем€ параллельного двухпоточного вычислени€: " << fixed << search_time2
      << endl;
   double norm2 = 0;
   for (int i = 0; i < n; i++)
      norm2 += y[i] * y[i];
   cout << "Ќорма вектора y при двухпоточном вычислении: " << sqrt(norm2) << endl;
   start_time = clock();

   sum = 0;
   for (int i = n - 1; i > -1; i--) //Uy = b 16 потоков
   {
         sum = b[i];
#pragma omp parallel for num_threads(8) reduction(+:sum)
      for (int j = n - 1; j > i; j--)
         sum -= U[n * i + j] * y[j];
      y[i] = sum/ U[i * n + i];
   }
   end_time = clock();
   double search_time16 = end_time - start_time;
   cout << "¬рем€ параллельного шестнадцатипоточного вычислени€: " << fixed <<
      search_time16 << endl;
   double norm16 = 0;
   for (int i = 0; i < n; i++)
      norm16 += y[i] * y[i];
   cout << "Ќорма вектора y при шестнадцатипоточном вычислении: " << sqrt(norm16) <<
      endl;
   cout << "”скорение от двух потоков: " << search_time1 / search_time2 << endl;
   cout << "”скорение от шестнадцати потоков: " << search_time1 / search_time16 <<
      endl;
   ofstream outy;
   outy.open("y.txt");
   for (int i = 0; i < n; i++)
      outy << y[i] << " ";
}