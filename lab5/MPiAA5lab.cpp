#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

//������������ �������� ���� = 100
#define INF 101 //(����� � MAX_INT ���������, �� ��� ������ ����� ������)

using namespace std;

//������� ��� ������ ������� ���������
void ReadAdjacencyMatrix(const string& filename, vector<vector<int>>& adjMatrix, int& n) 
{
   ifstream inputFile(filename);

   inputFile >> n; // ������ ���������� ������(������ �������)
   adjMatrix.resize(n, vector<int>(n));

   // ������ ������� ���������
   for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
         inputFile >> adjMatrix[i][j];

   inputFile.close();
}

void ReadAdjacencyList(const string& filename, vector<vector<int>>& adjMatrix, int& n)
{
   ifstream inputFile(filename);
   int m=0;
   inputFile >> n >> m; // ������ ���������� ������(������ �������)
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

   // ������ ������ ��������� � ���������� ������� ���������
   for (int i = 0; i < m; i++) 
   {
      inputFile >> u >> v >> w;
      adjMatrix[u-1][v-1] = w;
   }
}

//������� ������� ���������� ����� � �������
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

//�������� ������ ��������� ������� ���������, ���������� ������, ���������� ������� ��� OpenMP, ���������� ������� ���������� �����
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
               continue; // ���������� ���������
            if (adjMatrix[i][k] + adjMatrix[k][j] < adjMatrix[i][j])
               adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
         }
   }
   return adjMatrix;
}

int main()
{
   double start, finish, time;
   vector<vector<int>> adjMatrix; //������� ���������
   int n = 0; //������ ������� ���������
   bool x = 1;

   cout << "Choose matrix 1 or list 0: "; // ������� ��� ������� ������
   cin >> x;
   cout << endl;

   if (x)
   {
      string filename = "matrix.txt"; // ���� � �������� ���������
      ReadAdjacencyMatrix(filename, adjMatrix, n);
   }
   else
   {
      string filename = "list.txt"; // ���� �� ������� ���������
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