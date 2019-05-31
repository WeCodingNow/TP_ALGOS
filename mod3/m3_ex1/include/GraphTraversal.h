#pragma once
#include <vector>
#include <queue>

#include <functional>

#include "IGraph.h"

using Visitor = std::function<void(int)>;

/* Обход в ширину - Волновой
алгоритм - рассматривает все вершины, достижимые за ход; после формирует для
каждой вершины список вершин, достижимых за два хода */
void
BFS(const IGraph&, int, Visitor);

/* Обход в глубину - берём вершину из списка смежности, идём по ее списку
смежности; по пути помечаем, что мы зашли в вершину */
void
DFS(const IGraph&, int, Visitor);