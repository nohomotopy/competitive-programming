#ifndef CP_ALGO_GRAPH_DFS_HPP
#define CP_ALGO_GRAPH_DFS_HPP
#include "base.hpp"
#include <vector>
#include <functional>
namespace cp_algo::graph {
  template<typename graph>
    void dfs(const graph& g, int start, auto&& callback) {
      std::vector<char> visited(g.n());
      std::function<void(int)> go = [&](int v) {
        visited[v] = 1;
        callback(v);
        g.call_adjacent(
            v,
            [&](int e) {
            int u = g.edge(e).to;
            if (!visited[u])
              go(u);
            },
            []() {return false;}
            );
      };
      go(start);
    }
}
#endif 
