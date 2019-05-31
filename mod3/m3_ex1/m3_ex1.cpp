#include <iostream>
#include <iterator>
#include <cassert>

#include "CListGraph.h"
#include "CMatrixGraph.h"
#include "CSetGraph.h"
#include "CArcGraph.h"
#include "GraphTraversal.h"

using std::vector;
using std::initializer_list;
using std::ostream_iterator;

template<class GraphType>
std::ostream& TestGraphInsertion(std::ostream& os = std::cout)
{
  auto cit = ostream_iterator<int>(os, " ");
  GraphType gr(5);
  gr.AddEdge(0, 1);
  gr.AddEdge(0, 2);
  gr.AddEdge(0, 3);
  gr.AddEdge(1, 4);
  gr.AddEdge(2, 1);
  gr.AddEdge(2, 3);
  gr.AddEdge(2, 4);
  gr.AddEdge(4, 3);

  return os;
}

template<class GraphType>
std::ostream& TestGraphListConstruction(std::ostream& os = std::cout)
{
  auto cit = ostream_iterator<int>(os, " ");

  initializer_list<Edge> list = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 4},
    {2, 1}, {2, 3}, {2, 4},
    {4, 3}
  };

  GraphType gr(list);
  DFS(gr, 0, [&cit](int arg){ cit = arg;});

  os << std::endl;
  BFS(gr, 0, [&cit](int arg){ cit = arg;});

  return os;
}

template<class GraphType>
std::ostream& TestGraphTraversal(std::ostream& os = std::cout)
{
  auto cit = ostream_iterator<int>(os, " ");

  initializer_list<Edge> list = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 4},
    {2, 1}, {2, 3}, {2, 4},
    {4, 3}
  };

  GraphType gr(list);
  DFS(gr, 0, [&cit](int arg){ cit = arg;});
  os << std::endl;

  BFS(gr, 0, [&cit](int arg){ cit = arg;});

  return os;
}

template<class GraphType>
std::ostream& TestGraphPrev(std::ostream& os = std::cout)
{
  auto cit = ostream_iterator<int>(os, " ");

  initializer_list<Edge> list = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 4},
    {2, 1}, {2, 3}, {2, 4},
    {4, 3}
  };

  GraphType gr(list);

  auto tempVec = gr.GetPrevVertices(1);
  std::copy(begin(tempVec), end(tempVec), cit);

  // os << endl;
  os << std::endl;
  tempVec = gr.GetPrevVertices(4);
  std::copy(begin(tempVec), end(tempVec), cit);

  return os;
}

template<class GraphType1, class GraphType2>
std::ostream& TestGraphConversion(std::ostream& os = std::cout)
{
  auto cit = ostream_iterator<int>(os, " ");

  initializer_list<Edge> list = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 4},
    {2, 1}, {2, 3}, {2, 4},
    {4, 3}
  };

  GraphType1 gr1(list);
  GraphType2 gr2 = gr1;

  DFS(gr1, 0, [&cit](int arg){ cit = arg;});

  os << std::endl;
  DFS(gr2, 0, [&cit](int arg){ cit = arg;});

  return os;
}

int main()
{
  using std::cout, std::endl;

  cout << "Testing insertion" << endl;
  TestGraphInsertion<CListGraph>() << "CListGraph\n";
  TestGraphInsertion<CMatrixGraph>() << "CMatrixGraph\n";
  TestGraphInsertion<decltype(CSetGraph(5))>() << "CSetGraph\n";
  TestGraphInsertion<CArcGraph>() << "CArcGraph\n";

  cout << "Testing list construction" << endl;
  TestGraphListConstruction<CListGraph>() << "CListGraph\n";
  TestGraphListConstruction<CMatrixGraph>() << "CMatrixGraph\n";
  TestGraphListConstruction<decltype(CSetGraph(5))>() << "CSetGraph\n";
  TestGraphListConstruction<CArcGraph>() << "CArcGraph\n";

  cout << "Testing PreviousVertices()" << endl;
  TestGraphPrev<CListGraph>() << "CListGraph\n";
  TestGraphPrev<CMatrixGraph>() << "CMatrixGraph\n";
  TestGraphPrev<decltype(CSetGraph(5))>() << "CSetGraph\n";
  TestGraphPrev<CArcGraph>() << "CArcGraph\n";

  cout << "Testing traversal" << endl;
  TestGraphTraversal<CListGraph>() << "CListGraph\n";
  TestGraphTraversal<CMatrixGraph>() << "CMatrixGraph\n";
  TestGraphTraversal<decltype(CSetGraph(5))>() << "CSetGraph\n";
  TestGraphTraversal<CArcGraph>() << "CArcGraph\n";

  cout << "Testing conversion" << endl;
  TestGraphConversion<CListGraph, CListGraph>() << "CListGraph -> CListGraph\n";
  TestGraphConversion<CListGraph, CMatrixGraph>() << "CListGraph -> CMatrixGraph\n";
  TestGraphConversion<CListGraph, decltype(CSetGraph(5))>() << "CSetGraph\n";
  TestGraphConversion<CListGraph, CArcGraph>() << "CListGraph -> CArcGraph\n";
}

//Для каждой вершины хранить не только кол-во кратчайших путей, но и длину
