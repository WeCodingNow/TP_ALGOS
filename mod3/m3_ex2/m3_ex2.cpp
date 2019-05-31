#include <iostream>
#include <fstream>

#include <vector>
#include <queue>
#include <map>

#include <stdexcept>
#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////

using std::istream, std::cout, std::cin, std::endl;

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

using std::queue;
using std::vector;

void BFS(const IGraph &graph, int vertex, Visitor visitor) {
  vector<bool> visited(graph.VerticesCount(), false);
  queue<int> qu;
  qu.push(vertex);
  visited[vertex] = true;

  while (qu.size() != 0) {
    int current = qu.front();
    qu.pop();
    visitor(current);
    // vector<int> adjacenctVertices = graph.GetNextVertices(current);
    for (auto const &v : graph.GetNextVertices(current))
      if (!visited[v]) {
        qu.push(v);
        visited[v] = true;
      }
  }
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

class CMatrixGraph : public IGraph {
public:
  explicit CMatrixGraph(const int&);
  CMatrixGraph(const IGraph &); //также используется для копирования
  CMatrixGraph(const std::initializer_list<Edge>&);
  ~CMatrixGraph();

  virtual void AddEdge(int, int) override;
  virtual int VerticesCount() const override;
  virtual std::vector<int> GetNextVertices(int) const override;
  virtual std::vector<int> GetPrevVertices(int) const override;

private:
  std::vector<std::vector<int>> adjacencyMatrix;
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
using std::max;

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

CMatrixGraph::CMatrixGraph(const int &count)
  : adjacencyMatrix(count, vector<int>(count, 0)) {}

CMatrixGraph::CMatrixGraph(const IGraph &graph)
  : adjacencyMatrix(graph.VerticesCount(), vector<int>(graph.VerticesCount(), 0))
{
  for (int i = 0; i < graph.VerticesCount(); ++i)
    for (auto nextVertice: graph.GetNextVertices(i))
      ++adjacencyMatrix[i][nextVertice];
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
////////////////////////////////////////////////////////////////////////////////

//добавляем обратные пути
template<typename GraphType>
void
CUnordGraph<GraphType>::_updatePaths()
{
  auto copyGraph = innerGraph;
  vector<Edge> needToAdd;
  DFS(copyGraph, 0, [&needToAdd, &copyGraph](int curVert){
    for (auto nextVert : copyGraph.GetNextVertices(curVert))
      needToAdd.push_back({nextVert, curVert});
      // this->CListGraph::AddEdge(nextVert, curVert);
  });
  for (auto [first, second]: needToAdd)
      innerGraph.AddEdge(first, second);

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

using std::pair;

int
findShortestPathsAmount(const IGraph& graph, int from, int to)
{
  vector<bool> visited(graph.VerticesCount(), false);
  vector<int> pathLengths(graph.VerticesCount(), __INT_MAX__);
  vector<int> shortestPaths(graph.VerticesCount(), 0);


  // int path = 0;
  pathLengths[from] = 0;
  shortestPaths[from] = 1;

  queue<int> qu;
  qu.push(from);
  visited[from] = true;

  while (qu.size() != 0) {
    int current = qu.front();
    qu.pop();

    for (auto const &v : graph.GetNextVertices(current)) {

      if (pathLengths[v] > pathLengths[current] + 1) {
        pathLengths[v] = pathLengths[current] + 1;
        shortestPaths[v] = shortestPaths[current];
      }
      else if (pathLengths[v] == pathLengths[current] + 1) {
        shortestPaths[v] += shortestPaths[current];
      }

      if (!visited[v]) {
        qu.push(v);
        visited[v] = true;
      }
    }
  }

  return shortestPaths[to];
}

inline int
findShortestPathsAmount(const IGraph& graph, pair<int, int> path)
{
  auto [from, to] = path;
  return findShortestPathsAmount(graph, from, to);
}

////////////////////////////////////////////////////////////////////////////////


inline pair<int, int>
readPair(istream& is = cin)
{
  int first = 0; int second = 0;
  is >> first >> second;
  return {first, second};
}

template<typename GraphType>
inline GraphType
readGraph(istream& is = cin)
{
  int verticeCount = 0, edgeCount = 0;
  is >> verticeCount >> edgeCount;
  GraphType tempGraph(verticeCount);

  int from = 0, to = 0;
  for (int i = 0; i < edgeCount; i++) {
    // cin >> from >> to;
    auto [from, to] = readPair(is);
    tempGraph.AddEdge(from, to);
  }
  
  return tempGraph;
}

////////////////////////////////////////////////////////////////////////////////


int main() {
  auto gr = readGraph<CUnordGraph<CListGraph>>();
  cout << findShortestPathsAmount(gr, readPair()) << endl;

  return 0;
}
