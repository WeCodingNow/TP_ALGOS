#include "CMatrixGraph.h"

using std::vector;
using std::initializer_list;
using std::max_element;
using std::max;

CMatrixGraph::CMatrixGraph(const int &count)
  : adjacencyMatrix(count, vector<int>(count, 0)) {}

CMatrixGraph::CMatrixGraph(const IGraph &graph)
  : adjacencyMatrix(graph.VerticesCount(), vector<int>(graph.VerticesCount(), 0))
{
  for (int i = 0; i < graph.VerticesCount(); ++i)
    for (auto nextVertice: graph.GetNextVertices(i))
      ++adjacencyMatrix[i][nextVertice];
}

//считаем сколько у нас всего вершин
constexpr int
countVertices(const initializer_list<Edge> &il)
{
  return (
    begin(il) == end(il)
    ? 0
    : max(
        max_element(begin(il), end(il), [](Edge lhs, Edge rhs) {
          return lhs.first < rhs.first; })->first,
        max_element(begin(il), end(il), [](Edge lhs, Edge rhs) {
          return lhs.second < rhs.second; })->second
      ) + 1
  );
}

CMatrixGraph::CMatrixGraph(const initializer_list<Edge> &edges)
  : adjacencyMatrix(countVertices(edges), vector<int>(countVertices(edges)))
{
  for (auto&& [from, to]: edges)
    adjacencyMatrix[from][to]++;
}

CMatrixGraph::~CMatrixGraph() {}

void
CMatrixGraph::AddEdge(int from, int to) {
  if (from > adjacencyMatrix.size() || to > adjacencyMatrix.size())
    throw std::out_of_range(
      "can't add edge"
      + std::to_string(from)
      + "->"
      + std::to_string(to)
      + "out of bounds (max vertice: "
      + std::to_string(adjacencyMatrix.size())
    );

  adjacencyMatrix[from][to]++;
}

int 
CMatrixGraph::VerticesCount() const {
  return adjacencyMatrix.size();
}

std::vector<int> 
CMatrixGraph::GetNextVertices(int vertex) const {
  vector<int> next;

  for (int i = 0; i < adjacencyMatrix.size(); i++) {
    int vertCount = adjacencyMatrix[vertex][i];
    while (vertCount) {
      next.push_back(i);
      --vertCount;
    }
  }

  return next;
}

std::vector<int> 
CMatrixGraph::GetPrevVertices(int vertex) const {
  vector<int> prev;

  for (int i = 0; i < adjacencyMatrix.size(); i++) {
    int vertexCount = adjacencyMatrix[i][vertex];
    while (vertexCount--) {
      prev.push_back(i);
    }
  }

  return prev;
}