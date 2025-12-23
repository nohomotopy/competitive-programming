#ifndef CP_ALGO_GRAPH_LCA_EULER_TOUR_HPP
#define CP_ALGO_GRAPH_LCA_EULER_TOUR_HPP
#include "base.hpp"
namespace cp_algo::graph {
  template<typename Graph>
    struct lca_euler_tour {
      using node_index = int;
      const Graph& g;
      std::vector<int> first_visit;
      std::vector<node_index> tour;
      std::vector<int> depth;
      std::vector<std::vector<node_index>> st;

      lca_euler_tour(const Graph& g, node_index root = 0) :
        g(g),
        first_visit(g.n()),
        depth(g.n())
      {
        tour.reserve(2 * g.n());
        dfs(root, root, 0);
        build_sparse_table();
      }

      node_index lca(node_index u, node_index v) const {
        int l = first_visit[u];
        int r = first_visit[v];
        if (l > r) std::swap(l, r);
        int len = r - l + 1;
        int k = std::bit_width((unsigned)len) - 1; // floor(log2(len))
        node_index u1 = st[k][l];
        node_index u2 = st[k][r - (1 << k) + 1];
        return (depth[u1] < depth[u2]) ? u1 : u2;
      }

      int dist(node_index u, node_index v) {
        return depth[u] + depth[v] - 2 * depth[lca(u,v)];
      }
      private:
      void dfs(node_index u, node_index p, int d) {
        depth[u] = d;
        first_visit[u] = tour.size();
        tour.push_back(u);
        g.call_adjacent(u, [&](int e_idx) {
            int v = g.edge(e_idx).to;
            if (v != p) {
              dfs(v, u, d + 1);
              tour.push_back(u);
            }
            });
      }

      void build_sparse_table() {
        int n_tour = tour.size();
        int max_log = std::bit_width((unsigned) n_tour);
        st.assign(max_log, std::vector<node_index>(n_tour));
        for (int i = 0; i < n_tour; ++i)
          st[0][i] = tour[i];
        for (int j = 1; j < max_log; ++j) {
          for (int i = 0; i + (1 << j) <= n_tour; ++i) {
            node_index u1 = st[j - 1][i];
            node_index u2 = st[j - 1][i + (1 << (j - 1))];
            st[j][i] = (depth[u1] < depth[u2]) ? u1 : u2;
          }
        }
      }
    };
}
#endif // CP_ALGO_GRAPH_LCA_EULER_TOUR_HPP
