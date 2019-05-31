#include <iostream>

#include <vector>
#include <map>

#include <stdexcept>
#include <algorithm>
#include <functional>


////////////////////////////////////////////////////////////////////////////////

struct IGraph {
  virtual ~IGraph() {}

  virtual void AddEdge(int from, int to) = 0;

  virtual int VerticesCount() const = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

////////////////////////////////////////////////////////////////////////////////

using Visitor = std::function<void(int)>;

inline void
recursive_dfs(
  const IGraph &graph, std::vector<bool> *visited, int vertex, Visitor visitor)
{
  if (!visited)
    throw std::runtime_error("visited is nullptr");

  if (vertex > visited->size())
    throw std::runtime_error("vertex is not in graph!");

  (*visited)[vertex] = true;
  visitor(vertex);

  for (auto vert: graph.GetNextVertices(vertex))
    if (!(*visited)[vert])
      recursive_dfs(graph, visited, vert, visitor);
}

void
DFS(const IGraph &graph, int vertex, Visitor visitor)
{
  std::vector<bool> visited(graph.VerticesCount(), false);

  for (int i = 0; i < graph.VerticesCount(); i++)
    if (!visited[i])
      recursive_dfs(graph, &visited, i, visitor); 
}
////////////////////////////////////////////////////////////////////////////////

using Edge = std::pair<int, int>;

class CListGraph : public IGraph {
public:
  explicit CListGraph(const int&);
  CListGraph(const IGraph &); //также используется для копирования
  CListGraph(const std::initializer_list<Edge>&);
  ~CListGraph() {};

  virtual void AddEdge(int, int) override;
  virtual int VerticesCount() const override;
  virtual std::vector<int> GetNextVertices(int) const override;
  virtual std::vector<int> GetPrevVertices(int) const override;

private:
  std::vector<std::vector<int>> adjacencyLists;
};

////////////////////////////////////////////////////////////////////////////////

/* Класс-адаптор, который превращает ориентированный граф в неориентированный */

template<typename GraphType>
class CUnordGraph : public IGraph {
public:
  explicit CUnordGraph(const int&);
  CUnordGraph(const IGraph &); //также используется для копирования
  CUnordGraph(const std::initializer_list<Edge>&);
  ~CUnordGraph() {};

  inline virtual void AddEdge(int, int) override;
  inline virtual int VerticesCount() const override;
  inline virtual std::vector<int> GetNextVertices(int) const override;
  inline virtual std::vector<int> GetPrevVertices(int) const override;

  inline std::vector<int> GetAdjacentEdges(int) const;
private:
  GraphType innerGraph;
  void _updatePaths();
};

////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::initializer_list;
using std::max_element;

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
    : max_element(begin(il), end(il), [](Edge lhs, Edge rhs) {
        return lhs.first < rhs.first; })->first + 1
  );
}

CListGraph::CListGraph(const initializer_list<Edge> &edges)
  : adjacencyLists(countVertices(edges))
{
  for (auto&& [from, to]: edges)
    adjacencyLists[from].push_back(to);
}
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

////////////////////////////////////////////////////////////////////////////////

//добавляем обратные пути
template<typename GraphType>
void
CUnordGraph<GraphType>::_updatePaths()
{
  auto copyGraph = innerGraph;
  DFS(copyGraph, 0, [this, &copyGraph](int curVert){
    for (auto nextVert : copyGraph.GetNextVertices(curVert))
      innerGraph.AddEdge(nextVert, curVert);
      // this->CListGraph::AddEdge(nextVert, curVert);
  });
}

template<typename GraphType>
CUnordGraph<GraphType>::CUnordGraph(const int& verticesAmount)
  : innerGraph(verticesAmount) {}

template<typename GraphType>
CUnordGraph<GraphType>::CUnordGraph(const IGraph& graph)
  : innerGraph(graph)
{
  _updatePaths();
}

template<typename GraphType>
CUnordGraph<GraphType>::CUnordGraph(const initializer_list<Edge> &edges)
  : innerGraph(edges)
{
  _updatePaths();
}


template<typename GraphType>
void
CUnordGraph<GraphType>::AddEdge(int from, int to)
{
  innerGraph.AddEdge(from, to);
  innerGraph.AddEdge(to, from);
}

template<typename GraphType>
int
CUnordGraph<GraphType>::VerticesCount() const
{
  return innerGraph.VerticesCount();
}

template<typename GraphType>
std::vector<int>
CUnordGraph<GraphType>::GetNextVertices(int vert) const
{
  return GetAdjacentEdges(vert);
}

template<typename GraphType>
std::vector<int>
CUnordGraph<GraphType>::GetPrevVertices(int vert) const
{
  return GetAdjacentEdges(vert);
}

template<typename GraphType>
std::vector<int>
CUnordGraph<GraphType>::GetAdjacentEdges(int vert) const
{
  return innerGraph.GetNextVertices(vert);
}

////////////////////////////////////////////////////////////////////////////////

/* Dijkstras algorith */

////////////////////////////////////////////////////////////////////////////////

using std::cout, std::endl;

int main() {
  initializer_list<Edge> list = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 4},
    {2, 1}, {2, 3}, {2, 4},
    {4, 3}
  };

  CListGraph gr(list);
  DFS(gr, 0, [](int arg){cout << arg << ' ';});
  cout << endl;

  CUnordGraph<CListGraph> ugr(list);
  DFS(ugr, 0, [](int arg){cout << arg << ' ';});
  cout << endl;

  return 0;
}