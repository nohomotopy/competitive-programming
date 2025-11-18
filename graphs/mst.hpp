#ifndef CP_ALGO_GRAPH_MST_HPP
#define CP_ALGO_GRAPH_MST_HPP
#include "base.hpp"
#include "../structures/dsu.hpp"
#include <algorithm>
namespace cp_algo::graph {
  template<weighted_edge_type edge_t>
  auto mst(const graph<undirected, edge_t>& g) {
    std::vector<std::pair<int64_t, edge_index>> edges;
    g.call_edges([&](edge_index e) {
        edges.emplace_back(g.edge(e).w, e);
        });
    std::ranges::sort(edges);
    structures::dsu ds(g.n());
    int64_t total = 0;
    int cnt = 0;
    std::vector<edge_index> mst;
    for (auto [w, e] : edges) {
      if (ds.unite(g.edge(e).to, g.edge(e ^ 1).to)) {
        total += w;
        mst.push_back(e);
        if (++cnt == g.n() - 1) break;
      }
    }
    return std::pair{total, mst};
  }
}
#endif 
