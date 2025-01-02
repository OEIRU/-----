#include <omp.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
// #include <Windows.h>
using namespace std;
int main()
{
   // SetConsoleOutputCP(1251);
   int n = 2500;
   double* U = NULL;
   if (U == NULL)
   {
      U = new double[n * n];
      if (U == NULL)
      {
         cerr << "Memory allocation error" << endl;
         return 1;
      }
   }

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
   {
      x = new double[n];
      if (x == NULL)
      {
         cerr << "Memory allocation error" << endl;
         delete[] U;
         return 1;
      }
   }

   for (int i = 0; i < n; i++)
      x[i] = rand() % 11;

   ofstream outx;
   outx.open("x.txt");
   if (!outx.is_open())
   {
      cerr << "Can't open file x.txt" << endl;
      delete[] U;
      delete[] x;
      return 1;
   }

   for (int i = 0; i < n; i++)
      outx << x[i] << " ";
   outx.close();

   double* b = NULL;
   if (b == NULL)
   {
      b = new double[n];
      if (b == NULL)
      {
         cerr << "Memory allocation error" << endl;
         delete[] U;
         delete[] x;
         return 1;
      }
   }

   for (int i = 0; i < n; i++) // Ux = b
      for (int k = 0; k < n; k++)
         b[i] += U[i * n + k] * x[k];

   double* y = NULL;
   if (y == NULL)
   {
      y = new double[n];
      if (y == NULL)
      {
         cerr << "Memory allocation error" << endl;
         delete[] U;
         delete[] x;
         delete[] b;
         return 1;
      }
   }

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
   cout << "Время параллельного последовательного вычисления: " << fixed <<
      search_time1 << endl;

   double norm1 = 0;
   for (int i = 0; i < n; i++)
      norm1 += y[i] * y[i];
   cout << "Норма вектора y при послеодовательном вычислении: " << sqrt(norm1) <<
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
   cout << "Время параллельного двухпоточного вычисления: " << fixed << search_time2
      << endl;

   double norm2 = 0;
   for (int i = 0; i < n; i++)
      norm2 += y[i] * y[i];
   cout << "Норма вектора y при двухпоточном вычислении: " << sqrt(norm2) << endl;

   start_time = clock();

   sum = 0;
   for (int i = n - 1; i > -1; i--) //Uy = b 20 потоков
   {
      sum = b[i];
   int max_threads = omp_get_max_threads();
#pragma omp parallel for num_threads(max_threads) reduction(+:sum)
      for (int j = n - 1; j > i; j--)
         sum -= U[n * i + j] * y[j];
      y[i] = sum / U[i * n + i];
   }
   end_time = clock();
   double search_time20 = end_time - start_time;
   cout << "Время параллельного 20 поточном вычисления: " << fixed <<
      search_time20 << endl;

   double norm20 = 0;
   for (int i = 0; i < n; i++)
      norm20 += y[i] * y[i];
   cout << "Норма вектора y при 20 поточном вычислении: " << sqrt(norm20) <<
      endl;

   cout << "Ускорение от двух потоков: " << search_time1 / search_time2 << endl;
   cout << "Ускорение от 20 потоков: " << search_time1 / search_time20 <<
      endl;

   ofstream outy;
   outy.open("y.txt");
   if (!outy.is_open())
   {
      cerr << "Can't open file y.txt" << endl;
      delete[] U;
      delete[] x;
      delete[] b;
      delete[] y;
      return 1;
   }

   for (int i = 0; i < n; i++)
      outy << y[i] << " ";
   outy.close();

   delete[] U;
   delete[] x;
   delete[] b;
   delete[] y;
   return 0;
}
