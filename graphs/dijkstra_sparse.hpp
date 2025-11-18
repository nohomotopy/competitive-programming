#ifndef CP_ALGO_GRAPH_SPARSE_DIJKSTRA_HPP
#define CP_ALGO_GRAPH_SPARSE_DIJKSTRA_HPP
#include "base.hpp"
#include <algorithm>
#include <queue>

namespace cp_algo::graph {
  template<type _, weighted_edge_type edge_t>
  auto dijkstra(const graph<_, edge_t>& g, int src) {
    static constexpr uint64_t inf = 1e18;
    std::vector<uint64_t> dist(g.n(), inf);
    std::vector<std::pair<node_index, edge_index>> p(g.n());
    using que_t = std::pair<uint64_t, node_index>;
    std::priority_queue<que_t, std::vector<que_t>, std::greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
      auto [dv, v] = pq.top();
      pq.pop();
      if (dv != dist[v]) 
        continue;
      g.call_adjacent(v, [&](auto e) {
          node_index u = g.edge(e).to;
          auto w = g.edge(e).w;
          if (dist[v] + w < dist[u]) {
            p[u] = {v, e};
            dist[u] = dist[v] + w;
            pq.push({dist[u], u});
          }
          });
    }
    return std::pair{dist, p};
  }
  template<type _, weighted_edge_type edge_t>
  auto shortest_path(const graph<_, edge_t>& g, int src, int end) {
    static constexpr uint64_t inf = 1e18;
    auto [dist, p] = dijkstra(g, src);
    std::vector<std::pair<node_index, edge_index>> path;
    int64_t d = dist[end] == inf ? -1 : dist[end];
    while (d != -1 && end != src) {
      path.push_back(p[end]);
      end = p[end].first;
    }
    std::ranges::reverse(path);
    return std::pair{d, path};
  }
}
#endif // CP_ALGO_GRAPH_DIJKSTRA_HPP
