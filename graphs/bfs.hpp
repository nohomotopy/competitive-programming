#ifndef CP_ALGO_GRAPH_BFS_HPP
#define CP_ALGO_GRAPH_BFS_HPP
#include "base.hpp"
#include <queue>
namespace cp_algo::graph {
  template <typename graph, typename callback, typename terminate>
    std::vector<int> bfs(graph const& g, int src, callback&& cb, terminate&& term) {
      std::vector<int> dist(g.n(), -1);
      std::queue<int> q;
      dist[src] = 0;
      q.push(src);
      while (!q.empty() && !term()) {
        int v = q.front(); q.pop();
        cb(v, dist[v]); // callback v
        if (term()) break;
        g.call_adjacent(
            v,
            [&](int e) {
              int u = g.edge(e).to;
              if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                q.push(u);
              }
            },
            term
            );
      }
      return dist;
    }
}// namespace cp_algo::graph
#endif // CP_ALGO_GRAPH_BFS_HPP
