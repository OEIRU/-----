#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

//Максимальное значение веса = 100
#define INF 101 //(можно и MAX_INT поставить, но так просто числа меньше)

using namespace std;

//функция для чтение матрицы смежности
void ReadAdjacencyMatrix(const string& filename, vector<vector<double>>& adjMatrix, int& n)
{
   ifstream inputFile(filename);

   inputFile >> n; // Чтение количества вершин(размер матрицы)
   adjMatrix.resize(n, vector<double>(n));

   // Чтение матрицы смежности
   for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
         inputFile >> adjMatrix[i][j];

   inputFile.close();
}

void ReadAdjacencyList(const string& filename, vector<vector<double>>& adjMatrix, int& n)
{
   ifstream inputFile(filename);
   int m = 0;
   inputFile >> n >> m; // Чтение количества вершин(размер матрицы)
   adjMatrix.resize(n, vector<double>(n));
   int u, v, w;
   for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
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
      adjMatrix[u - 1][v - 1] = w;
   }
}

//вывести матрицу кратчайших путей в консоль
void printPathsMatrix(const vector<vector<double>>& pathsMatrix, int& n)
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
vector<vector<double>> Floyd(vector<vector<double>> adjMatrix, vector<vector<double>>& path, int& n, const int& num_of_threads)
{
   int i, j;
   for (int k = 0; k < n; k++)
   {
#pragma omp parallel for num_threads(num_of_threads) private(j)
      for (i = 0; i < n; i++) {
         if (adjMatrix[i][k] == INF)
            continue;
         for (j = 0; j < n; j++)
         {
            if (i == j)
               continue; // Пропустить самоциклы
            if (adjMatrix[i][k] + adjMatrix[k][j] < adjMatrix[i][j])
            {
               adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
               path[i][j] = path[i][k];
            }
         }
      }
   }
   return adjMatrix;
}

void ShortestPath(const vector<vector<double>>& pathsMatrix, const vector<vector<double>>& path, int u, int v)
{
   if (pathsMatrix[u][v] == INF)
      cout << "The Path does not exist";
   else
   {
      int k = u;
      while (k != v)
      {
         cout << k+1 <<" ";
         k = path[k][v];
      }
      cout << k + 1;
   }
}

int main()
{
   int u, v;
   double start, finish;
   vector<vector<double>> adjMatrix; //матрица смежности
   vector<vector<double>> path; // матрица кратчайших путей
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

   path.resize(n, vector<double>(n));
   for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
         path[i][j] = j;

   start = clock();
   auto PathsMatrix1 = Floyd(adjMatrix, path,n, 1);
   finish = clock();
   cout << "time 1: " << finish - start << endl;
   //printPathsMatrix(PathsMatrix1, n);

   start = clock();
   auto PathsMatrix2 = Floyd(adjMatrix, path, n, 2);
   finish = clock();
   cout << "time 2: " << finish - start << endl;
   //printPathsMatrix(PathsMatrix2, n);

   start = clock();
   auto PathsMatrix8 = Floyd(adjMatrix, path , n, 8);
   finish = clock();
   cout << "time 8: " << finish - start << endl;
   //printPathsMatrix(PathsMatrix8, n);

   printPathsMatrix(Floyd(adjMatrix, path, n, 1), n);

   cout << "enter u v for path: " << endl;
   cin >> u >> v;
   ShortestPath(PathsMatrix1, path, u-1, v-1);

   return 0;
}
