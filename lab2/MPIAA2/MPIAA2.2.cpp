#include <omp.h>
#include <iostream>
#include <chrono>
#include <Windows.h>

using namespace std;

int main()
{
   SetConsoleOutputCP(1251);

   int n = 2500; //размерность матрицы

   double* A = NULL;
   if (A == NULL)
      A = new double[n * n];

   for (int i = 0; i < (n * n); i++)
      A[i] = rand() % 11;

   double* B = NULL;
   if (B == NULL)
      B = new double[n * n];

   for (int i = 0; i < (n * n); i++)
      B[i] = rand() % 11;

   double* C = NULL;
   if (C == NULL)
      C = new double[n * n];
   for (int i = 0; i < (n * n); i++)
      C[i] = 0;

   double start_time = clock();
   for (int i = 0; i < n; i++)  //последовательное вычисление
      for (int j = 0; j < n; j++)
         for (int k = 0; k < n; k++)
            C[i * n + j] += A[i * n + k] * B[k * n + j];
   double end_time = clock();
   double search_time1 = end_time - start_time;

   cout << "Время последовательного вычисления: " << search_time1 << endl;

   double norm = 0;
   for (int i = 0; i < (n * n); i++)
      norm += C[i] * C[i];
   cout << "Норма матрицы при использовании 1 потока: "<< fixed <<sqrt(norm)<<endl;

   for (int i = 0; i < (n * n); i++)
      C[i]=0;

   start_time = clock();
#pragma omp parallel for num_threads(2)  //параллельное двухпоточное вычисление
   for (int i = 0; i < n; i++)  
      for (int j = 0; j < n; j++)
         for (int k = 0; k < n; k++)
            C[i * n + j] += A[i * n + k] * B[k * n + j];
   end_time = clock();
   double search_time2 = end_time - start_time;
   cout << "Время параллельного двухпоточного вычисления: " << search_time2 << endl;

   norm = 0;
   for (int i = 0; i < (n * n); i++)
         norm += C[i] * C[i];
   cout << "Норма матрицы при использовании 2 потоков: "<< fixed << sqrt(norm) << endl;

   for (int i = 0; i < (n * n); i++)
      C[i] = 0;

   start_time = clock();
#pragma omp parallel for num_threads(8)    //параллельное вычисление
   for (int i = 0; i < n; i++) 
      for (int j = 0; j < n; j++)
         for (int k = 0; k < n; k++)
            C[i * n + j] += A[i * n + k] * B[k * n + j];
   end_time = clock();
   double search_time16 = end_time - start_time;
   cout << "Время параллельного "<<omp_get_max_threads()<<" поточного вычисления: " << fixed << search_time16 << endl;


   norm = 0;
   for (int i = 0; i < (n * n); i++)
         norm += C[i] * C[i];
   cout << "Норма матрицы при использовании " << omp_get_max_threads() << " потоков: " << sqrt(norm) << endl;
   
   cout << "Ускорение от двух потоков: " << search_time1 / search_time2 << endl;
   cout << "Ускорение от максимума потоков: " << search_time1 / search_time16 << endl;
   
}
