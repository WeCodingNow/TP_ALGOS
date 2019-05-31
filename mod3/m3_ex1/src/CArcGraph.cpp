#include "CArcGraph.h"

using std::vector;
using std::initializer_list;
using std::max_element;
using std::for_each;

//Запилить конвертации из одного графа в другой (БФС)
CArcGraph::CArcGraph(const int &count)
  : maxVertex(count) {}

CArcGraph::CArcGraph(const IGraph &graph)
  : maxVertex(graph.VerticesCount())
{
  for (int i = 0; i < graph.VerticesCount(); ++i)
    for (auto vert: graph.GetNextVertices(i))
      edges.push_back({i, vert});
};

CArcGraph::CArcGraph(const initializer_list<Edge> &_edges)
  : edges(vector<Edge>(_edges)) {}


CArcGraph::~CArcGraph() {}

void
CArcGraph::AddEdge(int from, int to)
{
  if (from > maxVertex)
    throw std::out_of_range(
      "can't add edge"
      + std::to_string(from)
      + "->"
      + std::to_string(to)
      + "out of bounds (max vertice: "
      + std::to_string(maxVertex)
    );

  edges.push_back({from, to});
}

int
CArcGraph::VerticesCount() const
{
  return (
    begin(edges) == end(edges)
    ? 0
    : max_element(begin(edges), end(edges), [](Edge lhs, Edge rhs) {
        return lhs.first < rhs.first; })->first + 1
  );
}

vector<int>
CArcGraph::GetNextVertices(int vertex) const
{
  vector<int> next;
  for_each(begin(edges), end(edges), [&next, vertex](Edge edge) {
    if (edge.first == vertex)
      next.push_back(edge.second);
  });

  return next;
}

vector<int>
CArcGraph::GetPrevVertices(int vertex) const
{
  vector<int> prev;
  for_each(begin(edges), end(edges), [&prev, vertex](Edge edge) {
    if (edge.second == vertex)
      prev.push_back(edge.first);
  });

  return prev;
}