#ifndef CP_ALGO_GRAPH_LCA_BINARY_LIFTING_HPP
#define CP_ALGO_GRAPH_LCA_BINARY_LIFTING_HPP
#include "base.hpp"
namespace cp_algo::graph {
  template<typename Graph>
    struct lca_binary_lifting {
      using node_index = int;
      const Graph& g;
      std::vector<int> depth;
      std::vector<int> tin, tout;
      std::vector<std::vector<node_index>> up;
      int timer;
      int L;
      
      lca_binary_lifting(const Graph& g, node_index root = 0) :
        g(g),
        depth(g.n()),
        tin(g.n()),
        tout(g.n()),
        timer(0)
      {
        L = std::bit_width((unsigned) g.n());
        up.assign(g.n(), std::vector<node_index>(L + 1));
        dfs(root, root, 0);
      }

      bool is_ancestor(node_index u, node_index v) const {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
      }

      node_index lca(node_index u, node_index v) const {
        if (is_ancestor(u,v)) return u;
        if (is_ancestor(v,u)) return v;
        for (int i = L; i >= 0; --i) {
          if (!is_ancestor(up[u][i], v))
            u = up[u][i];
        }
        return up[u][0];
      }

      int dist(node_index u, node_index v) const {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
      }

      node_index kth_ancestor(node_index u, int k) const {
        for (int i = 0; i <= L; ++i) {
          if ((k >> i) & 1) 
            u = up[u][i];
        }
        return u;
      }

    private:
      void dfs(node_index u, node_index p, int d) {
        depth[u] = d;
        tin[u] = ++timer;
        up[u][0] = p;
        for (int i = 1; i <= L; ++i) 
          up[u][i] = up[up[u][i - 1]][i - 1];
        g.call_adjacent(u, [&](int e_idx) {
          node_index v = g.edge(e_idx).to;
          if (v != p) 
            dfs(v, u, d + 1);
        });
        tout[u] = ++timer;
      }
    };
}
#endif // CP_ALGO_GRAPH_LCA_BINARY_LIFTING_HPP
