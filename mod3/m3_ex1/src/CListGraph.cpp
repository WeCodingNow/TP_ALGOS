#include "CListGraph.h"

using std::vector;
using std::initializer_list;
using std::max_element;
using std::max;

//Запилить конвертации из одного графа в другой (БФС)
CListGraph::CListGraph(const int &count)
  : adjacencyLists(vector<vector<int>>(count)) {}

CListGraph::CListGraph(const IGraph &graph)
{
  for (int i = 0; i < graph.VerticesCount(); ++i)
    adjacencyLists.push_back(graph.GetNextVertices(i));
};

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

CListGraph::CListGraph(const initializer_list<Edge> &edges)
  : adjacencyLists(countVertices(edges))
{
  for (auto&& [from, to]: edges)
    adjacencyLists[from].push_back(to);
}


CListGraph::~CListGraph() {}

void
CListGraph::AddEdge(int from, int to)
{
  if (from > adjacencyLists.size() || to > adjacencyLists.size())
    throw std::out_of_range(
      "can't add edge"
      + std::to_string(from)
      + "->"
      + std::to_string(to)
      + "out of bounds (max vertice: "
      + std::to_string(adjacencyLists.size())
    );

  adjacencyLists[from].push_back(to);
}

int
CListGraph::VerticesCount() const { return adjacencyLists.size(); }

vector<int>
CListGraph::GetNextVertices(int vertex) const
{
  return adjacencyLists[vertex];
}

vector<int>
CListGraph::GetPrevVertices(int vertex) const
{
  vector<int> prev;

  for (int listNum = 0; listNum < adjacencyLists.size(); listNum++)
    for (auto vert : adjacencyLists[listNum])
      if (vert == vertex)
        prev.push_back(listNum);

  return prev;
}