#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;
struct Graph {
  int vertices;
  vector < vector < int >> adjacencyList;
  Graph(int v): vertices(v), adjacencyList(v + 1) {}
  void add_edge(int u, int v) {
    adjacencyList[u].push_back(v);
    adjacencyList[v].push_back(u);
  }
  void DFS(int startVertex, vector < bool > & visited, ofstream & outputFile) {
    stack < int > s;
    s.push(startVertex);
    while (!s.empty()) {
      int currentVertex = s.top();
      s.pop();
      if (!visited[currentVertex]) {
        visited[currentVertex] = true;
        outputFile << currentVertex << " ";
        for (int neighbor: adjacencyList[currentVertex]) {
          if (!visited[neighbor]) {
            s.push(neighbor);
          }
        }
      }
    }
  }
  int connected_components() {
    vector < bool > visited(vertices + 1, false);
    int components = 0;
    ofstream outputComponentsFile("components.txt");
    for (int i = 1; i <= vertices; ++i) {
      if (!visited[i]) {
        DFS(i, visited, outputComponentsFile);
        outputComponentsFile << endl;
        components++;
      }
    }
    outputComponentsFile.close();
    return components;
  }
};
int main() {
  ifstream inputFile("input.txt");
  int vertices, edges;
  inputFile >> vertices >> edges;
  Graph graph(vertices);
  for (int i = 0; i < edges; ++i) {
    int u, v;
    inputFile >> u >> v;
    graph.add_edge(u, v);
  }
  inputFile.close();
  ofstream outputTraversalFile("output.txt");
  vector < bool > visited(vertices + 1, false);
  cout << "Enter the starting vertex for the traversal: ";
  int startVertex;
  cin >> startVertex;
  cout << "DFS results are in output.txt" << endl;
  graph.DFS(startVertex, visited, outputTraversalFile);
  outputTraversalFile.close();
  int components = graph.connected_components();
  cout << "The number of connected components in the graph: " << components << endl;
  return 0;
}