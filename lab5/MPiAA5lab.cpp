#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

//Максимальное значение веса = 100
#define INF 101 //(можно и MAX_INT поставить, но так просто числа меньше)

using namespace std;

//функция для чтение матрицы смежности
void ReadAdjacencyMatrix(const string& filename, vector<vector<int>>& adjMatrix, int& n) 
{
   ifstream inputFile(filename);

   inputFile >> n; // Чтение количества вершин(размер матрицы)
   adjMatrix.resize(n, vector<int>(n));

   // Чтение матрицы смежности
   for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
         inputFile >> adjMatrix[i][j];

   inputFile.close();
}

void ReadAdjacencyList(const string& filename, vector<vector<int>>& adjMatrix, int& n)
{
   ifstream inputFile(filename);
   int m=0;
   inputFile >> n >> m; // Чтение количества вершин(размер матрицы)
   adjMatrix.resize(n, vector<int>(n));
   int u, v, w;
   for (int i = 0; i < n; i++)
      for (int j = 0; i < n; i++)
      {
         if (i == j)
            adjMatrix[i][j] = 0;
         else
            adjMatrix[i][j] = INF;
      }

   // Чтение списка смежности и заполнение матрицы смежности
   for (int i = 0; i < m; i++) 
   {
      inputFile >> u >> v >> w;
      adjMatrix[u-1][v-1] = w;
   }
}

//вывести матрицу кратчайших путей в консоль
void printPathsMatrix(const vector<vector<int>>& pathsMatrix, int& n)   
{
   cout << "Shortest Path Matrix:" << endl;
   for (int i = 0; i < n; i++)
   {
      for (int j = 0; j < n; j++)
      {
         if (pathsMatrix[i][j] == INF)
         {
            cout.width(3);
            cout << "INF" << " ";
         }
         else
         {
            cout.width(3);
            cout << pathsMatrix[i][j] << " ";
         }
      }
      cout << endl;
   }
}

//алгоритм флойда принимает матрицу смежности, количество вершин, количество потоков для OpenMP, возвращает матрицу кратчайших путей
vector<vector<int>> Floyd(vector<vector<int>> adjMatrix, int& n, const int& num_of_threads)
{
   int i, j;
   for (int k = 0; k < n; k++)
   {
#pragma omp parallel for num_threads(num_of_threads) private(j)
      for (i = 0; i < n; i++)
         for (j = 0; j < n; j++)
         {
            if (i == j)
               continue; // Пропустить самоциклы
            if (adjMatrix[i][k] + adjMatrix[k][j] < adjMatrix[i][j])
               adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
         }
   }
   return adjMatrix;
}

int main()
{
   double start, finish, time;
   vector<vector<int>> adjMatrix; //матрица смежности
   int n = 0; //размер матрицы смежности
   bool x = 1;

   cout << "Choose matrix 1 or list 0: "; // выбрать тип входных данных
   cin >> x;
   cout << endl;

   if (x)
   {
      string filename = "matrix.txt"; // файл с матрицей смежности
      ReadAdjacencyMatrix(filename, adjMatrix, n);
   }
   else
   {
      string filename = "list.txt"; // файл со списком смежности
      ReadAdjacencyList(filename, adjMatrix, n);
   }
   start = clock();
   auto PathsMatrix1 = Floyd(adjMatrix, n, 1);
   finish = clock();
   cout << "time 1: " << finish - start << endl;
   printPathsMatrix(PathsMatrix1, n);

   start = clock();
   auto PathsMatrix2 = Floyd(adjMatrix, n, 2);
   finish = clock();
   cout << "time 2: " << finish - start << endl;
   //printPathsMatrix(PathsMatrix2, n);

   start = clock();
   auto PathsMatrix8 = Floyd(adjMatrix, n, 8);
   finish = clock();
   cout << "time 8: " << finish - start << endl;
   //printPathsMatrix(PathsMatrix8, n);

   //printPathsMatrix(Floyd(adjMatrix, n, 1), n);
   return 0;
}