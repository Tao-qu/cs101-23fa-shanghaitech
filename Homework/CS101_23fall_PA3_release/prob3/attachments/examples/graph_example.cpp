#include <iostream>

#include "../graph.hpp"

int main()
{
  Graph g(5);
  g.addEdge(0, 1, 5);
  g.addEdge(0, 4, 3);
  g.addEdge(1, 2, 6);
  g.addEdge(1, 3, 4);
  g.addEdge(2, 3, 7);
  g.addEdge(4, 3, 6);
  g.addEdge(3, 2, 2);

  g.bfs(0, [](auto x)
        { std::cout << x << ' '; });
  std::cout << std::endl;

  auto dist = g.dijkstra(0);
  for (auto x : dist)
    std::cout << x << ' ';
  std::cout << std::endl;

  return 0;
}