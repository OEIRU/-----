// MPIAA2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <ctime> 
#include <omp.h>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
   
   int n = 5000000;
   double s = 0;

   double* x = NULL; // объявление
   if (x == NULL)
      x = new double[n]; // выделение памяти

   ifstream inputFile("vector_x.txt");
   for (int i = 0; i < n; i++)
      inputFile >> x[i];
   inputFile.close();

   double* y = NULL;
   if (y == NULL)
      y = new double[n];
   
   ifstream inputFile2("vector_y.txt");
   for (int i = 0; i < n; i++)
      inputFile2 >> y[i];
   inputFile2.close();


   auto start_time = clock();
//#pragma omp parallel num_threads(2) reduction(+:s) 
//   {
//#pragma omp for
       for (int i = 0; i < n; i++)
           s += x[i] * y[i];
   //}

 
   auto end_time = clock();
   auto search_time = end_time - start_time;

   cout << sqrt(s)<<"\n";
   cout << sqrt(search_time);
}
