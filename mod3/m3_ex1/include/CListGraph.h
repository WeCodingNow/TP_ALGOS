#pragma once
#include <vector>

#include <algorithm>
#include <stdexcept>

#include "IGraph.h"

using Edge = std::pair<int, int>;

class CListGraph : public IGraph {
public:
  explicit CListGraph(const int&);
  CListGraph(const IGraph &); //также используется для копирования
  CListGraph(const std::initializer_list<Edge>&);
  ~CListGraph();

  virtual void AddEdge(int, int) override;
  virtual int VerticesCount() const override;
  virtual std::vector<int> GetNextVertices(int) const override;
  virtual std::vector<int> GetPrevVertices(int) const override;

private:
  std::vector<std::vector<int>> adjacencyLists;
};