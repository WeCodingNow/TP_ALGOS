#include "GraphTraversal.h"

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
    vector<int> adjacenctVertices = graph.GetNextVertices(current);
    for (auto const &v : adjacenctVertices)
      if (!visited[v]) {
        qu.push(v);
        visited[v] = true;
      }
  }
}


inline void
recursive_dfs(
  const IGraph &graph, vector<bool> *visited, int vertex, Visitor visitor)
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
  vector<bool> visited(graph.VerticesCount(), false);

  for (int i = 0; i < graph.VerticesCount(); i++)
    if (!visited[i])
      recursive_dfs(graph, &visited, i, visitor); 
}