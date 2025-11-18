#ifndef CP_ALGO_GRAPH_DENSE_DIJKSTRA_HPP
#define CP_ALGO_GRAPH_DENSE_DIJKSTRA_HPP
#include "base.hpp"
#include <algorithm>
#include <cstdint>
namespace cp_algo::graph {
  template<type _, weighted_edge_type edge_t>
  auto dijkstra(const graph<_, edge_t>& g, int src) {
    static constexpr uint64_t inf = 1e18;
    int n = g.n();
    std::vector<uint64_t> dist(n, inf);
    std::vector<std::pair<node_index, edge_index>> p(n);
    std::vector<bool> visited(n, false);
    dist[src] = 0;
    for (int l = 0; l < n; ++l) {
      node_index v = -1;
      uint64_t dmin = inf;
      for (node_index i = 0; i < n; ++i) {
        if (!visited[i] && dist[i] < dmin) {
          dmin = dist[i];
          v = i;
        }
      } 
      if (v == -1) break;
      visited[v] = true;
      g.call_adjacent(v, [&](edge_index e){
        node_index u = g.edge(e).to;
        uint64_t w = g.edge(e).w;
        if (dist[v] + w < dist[u]) {
          dist[u] = dist[v] + w;
          p[u] = {v, e};
        }
      });
    }
    return std::pair{dist, p};
  }

  template<type _, weighted_edge_type edge_t>
  auto shortest_path(const graph<_, edge_t>& g, int s, int t) {
    static constexpr uint64_t inf = 1e18;
    auto [dist, p] = dijkstra(g, s);
    std::vector<std::pair<node_index, edge_index>> path;
    uint64_t d = dist[t] == inf ? -1 : dist[t];
    while (d != -1 && s != t) {
      path.push_back(p[t]);
      t = p[t].first;
    }
    std::ranges::reverse(path);
    return std::pair{d, path};
  }
} 
#endif // CP_ALGO_GRAPH_DENSE_DIJKSTRA_HPP
