#pragma once
#include <vector>

#include <algorithm>
#include <stdexcept>

#include "IGraph.h"

using Edge = std::pair<int, int>;

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
