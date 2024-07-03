#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <cstdint>
#include <queue>
#include <tuple>
#include <limits>
#include <optional>
#include <cassert>

class Graph
{
public:
  using VertexID = std::size_t;
  using Weight = std::int64_t;

private:
  struct AdjListEntry
  {
    VertexID to;
    Weight weight;
  };

  std::vector<std::vector<AdjListEntry>> mAdjacent;

public:
  static constexpr auto infinity = std::numeric_limits<Weight>::max();

  explicit constexpr Graph(VertexID numVertices) : mAdjacent(numVertices) {}

  constexpr void addEdge(VertexID from, VertexID to, Weight weight)
  {
    mAdjacent[from].push_back({.to = to, .weight = weight});
  }

  constexpr auto numVertices() const { return mAdjacent.size(); }

  /// @brief Breadth-first traversal of the graph.
  /// @param start ID of the vertex where the traversal begins.
  /// @param callback A callable object that is called every time a vertex is
  /// visited.
  void bfs(VertexID start, std::invocable<VertexID> auto callback) const
  {
    std::queue<VertexID> frontier;
    std::vector<bool> visited(numVertices());
    frontier.push(start);
    visited[start] = true;
    while (!frontier.empty())
    {
      auto current = frontier.front();
      frontier.pop();
      callback(current); // Call the callback object.
      for (auto [to, _] : mAdjacent[current])
        if (!visited[to])
        {
          frontier.push(to);
          visited[to] = true;
        }
    }
  }

  /// @brief The Dijkstra algorithm that calculates the length of the
  /// shortest path from a given source vertex to each vertex.
  /// @param source The source vertex.
  /// @return A vector of numbers. The element indexed @c i is the length of the
  /// shortest path from @c source to the vertex @c i, or @c Graph::infinity if
  /// no path exists.
  /// @pre The edge weights are non-negative.
  std::vector<Weight> dijkstra(VertexID source) const;

  /// @brief The Bellman-Ford algorithm that calculates the length of the
  /// shortest path from a given source vertex to each vertex.
  /// @param source The source vertex.
  /// @return @c std::nullopt if the graph contains a negative cycle reachable
  /// from @c source. Otherwise, a vector of numbers in which the element
  /// indexed @c i is the length of the shortest path from @c source to the
  /// vertex @c i, or @c Graph::infinity if no path exists.
  std::optional<std::vector<Weight>> bellmanFord(VertexID source) const;
};

#endif // GRAPH_HPP
