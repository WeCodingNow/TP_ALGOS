#pragma once
#include <vector>

#include <algorithm>
#include <stdexcept>

#include "IGraph.h"

using Edge = std::pair<int, int>;

class CArcGraph : public IGraph {
public:
  explicit CArcGraph(const int&);
  CArcGraph(const IGraph &); //также используется для копирования
  CArcGraph(const std::initializer_list<Edge>&);
  ~CArcGraph();

  virtual void AddEdge(int, int) override;
  virtual int VerticesCount() const override;
  virtual std::vector<int> GetNextVertices(int) const override;
  virtual std::vector<int> GetPrevVertices(int) const override;

private:
  int maxVertex;
  std::vector<Edge> edges;
};