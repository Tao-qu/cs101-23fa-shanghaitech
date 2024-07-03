#include "graph.hpp"

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight>
{
  // FIXME: This is a naive O(V^2) implementation. Overwrite it to make it run
  // within O(ElogV) time, which is more efficient when E = o(V^2/logV).
  // std::vector<Weight> dist(numVertices(), infinity);
  // std::vector<bool> done(numVertices(), false);
  // dist[source] = 0;
  // for (auto rest = numVertices(); rest > 0; --rest)
  // {
  //   VertexID current = -1;
  //   Weight minDist = infinity;
  //   for (VertexID i = 0; i != numVertices(); ++i)
  //     if (!done[i] && dist[i] < minDist)
  //     {
  //       current = i;
  //       minDist = dist[i];
  //     }
  //   done[current] = true;
  //   for (auto [to, weight] : mAdjacent[current])
  //     if (dist[current] + weight < dist[to])
  //       dist[to] = dist[current] + weight;
  // }
  // return dist;
  std::vector<Weight> dist(numVertices(), infinity);
  dist[source] = 0;

  using Pair = std::pair<Weight, VertexID>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> queue;
  queue.push({0, source});

  while (!queue.empty())
  {
    VertexID current = queue.top().second;
    Weight currentDist = queue.top().first;
    queue.pop();

    if (currentDist != dist[current])
    {
      continue;
    }

    for (auto [to, weight] : mAdjacent[current])
    {
      if (dist[current] + weight < dist[to])
      {
        dist[to] = dist[current] + weight;
        queue.push({dist[to], to});
      }
    }
  }

  return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>>
{
  // TODO: Implement this.
  std::vector<Weight> dist(numVertices(), infinity);
  dist[source] = 0;

  for (auto rest = numVertices() - 1; rest > 0; --rest)
  {
    for (VertexID from = 0; from != numVertices(); ++from)
    {
      for (auto [to, weight] : mAdjacent[from])
      {
        if (dist[from] != infinity && dist[from] + weight < dist[to])
        {
          dist[to] = dist[from] + weight;
        }
      }
    }
  }

  for (VertexID from = 0; from != numVertices(); ++from)
  {
    for (auto [to, weight] : mAdjacent[from])
    {
      if (dist[from] != infinity && dist[from] + weight < dist[to])
      {
        return std::nullopt;
      }
    }
  }

  return dist;
}