#pragma once
#include <vector>
#include <unordered_set>

#include <algorithm>
#include <functional>
#include <stdexcept>

#include "IGraph.h"

using Edge = std::pair<int, int>;

template<typename HashFunc = std::hash<int>>
class CSetGraph : public IGraph {
public:
  explicit CSetGraph(const int&);
  CSetGraph(const IGraph &); //также используется для копирования
  CSetGraph(const std::initializer_list<Edge>&);
  ~CSetGraph();

  virtual void AddEdge(int, int) override;
  virtual int VerticesCount() const override;
  virtual std::vector<int> GetNextVertices(int) const override;
  virtual std::vector<int> GetPrevVertices(int) const override;

private:
  std::vector<std::unordered_multiset<int, HashFunc>> adjacencyMaps;
};

template<typename HashFunc>
CSetGraph<HashFunc>::CSetGraph(const int &count)
  : adjacencyMaps(count) {}

template<typename HashFunc>
CSetGraph<HashFunc>::CSetGraph(const IGraph &graph)
  : adjacencyMaps(graph.VerticesCount())
{
  for (int i = 0; i < graph.VerticesCount(); ++i)
    for (auto nextVertice: graph.GetNextVertices(i))
      adjacencyMaps[i].insert({i, nextVertice});
}

//считаем сколько у нас всего вершин
constexpr int
countVertices(const std::initializer_list<Edge> &il)
{
  return (
    begin(il) == end(il)
    ? 0
    : std::max_element(begin(il), end(il), [](Edge lhs, Edge rhs) {
        return lhs.first < rhs.first;
      })->first + 1
  );
}

template<typename HashFunc>
CSetGraph<HashFunc>::CSetGraph(const std::initializer_list<Edge> &edges)
  : adjacencyMaps(countVertices(edges))
{
  for (auto [from, to] : edges)
    adjacencyMaps[from].insert(to);
}

template<typename HashFunc>
CSetGraph<HashFunc>::~CSetGraph() {}

template<typename HashFunc>
void
CSetGraph<HashFunc>::AddEdge(int from, int to) {
  if (from > adjacencyMaps.size() || to > adjacencyMaps.size())
    throw std::out_of_range(
      "can't add edge"
      + std::to_string(from)
      + "->"
      + std::to_string(to)
      + "out of bounds (max vertice: "
      + std::to_string(adjacencyMaps.size())
    );
  adjacencyMaps[from].insert(to);
}

template<typename HashFunc>
int 
CSetGraph<HashFunc>::VerticesCount() const {
  return adjacencyMaps.size();
}

template<typename HashFunc>
std::vector<int> 
CSetGraph<HashFunc>::GetNextVertices(int vertex) const {
  return std::vector<int>(
    begin(adjacencyMaps[vertex]), end(adjacencyMaps[vertex]));
}

template<typename HashFunc>
std::vector<int> 
CSetGraph<HashFunc>::GetPrevVertices(int vertex) const {
  std::vector<int> prev;
  for (int i = 0; i < adjacencyMaps.size(); i++)
    if (adjacencyMaps[i].find(vertex) != end(adjacencyMaps[i]))
      prev.push_back(i);

  return prev;
}