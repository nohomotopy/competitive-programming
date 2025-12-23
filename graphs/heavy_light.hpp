#ifndef CP_ALGO_GRAPH_HEAVY_LIGHT_HPP
#define CP_ALGO_GRAPH_HEAVY_LIGHT_HPP
#include "base.hpp"
namespace cp_algo::graph {
  template<typename Graph>
  struct heavy_light {
    using node_index = int;
    const Graph& g;
    std::vector<node_index> parent;
    std::vector<int> depth;
    std::vector<int> sz;
    std::vector<node_index> heavy;
    std::vector<node_index> head;
    std::vector<int> pos; // Segment tree position
    int timer;

    heavy_light(const Graph& g, node_index root = 0) : 
      g(g),
      parent(g.n(), -1),
      depth(g.n(), 0),
      sz(g.n(), 1),
      heavy(g.n(), -1),
      head(g.n()),
      pos(g.n()),
      timer(0)
    {
      dfs_sz(root);
      head[root] = root;
      dfs_hld(root);
    }

    node_index lca(node_index u, node_index v) const {
      while (head[u] != head[v]) {
        if (depth[head[u]] > depth[head[v]])
          u = parent[head[u]];
        else 
          v = parent[head[v]];
      }
      return depth[u] < depth[v] ? u : v;
    }

    int dist(node_index u, node_index v) const {
      return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    template<typename Callback>
    void decompose_path(node_index u, node_index v, Callback&& callback) const {
      while (head[u] != head[v]) {
        if (depth[head[u]] > depth[head[v]]) {
          callback(pos[head[u]], pos[u]);
          u = parent[head[u]];
        }
        else {
          callback(pos[head[v]], pos[v]);
          v = parent[head[v]];
        }
      }
      if (depth[u] > depth[v]) std::swap(u, v);
      callback(pos[u], pos[v]);
    }

    template<typename Callback>
    void decompose_path_edges(node_index u, node_index v, Callback&& callback) const {
      while (head[u] != head[v]) {
        if (depth[head[u]] > depth[head[v]]) {
          callback(pos[head[u]], pos[u]);
          u = parent[head[u]];
        }
        else {
          callback(pos[head[v]], pos[v]);
          v = parent[head[v]];
        }
      }
      if (u != v) {
        if (depth[u] > depth[v]) std::swap(u, v);
        callback(pos[u] + 1, pos[v]);
      }
    }

    std::pair<int, int> subtree_range(node_index u) const {
      return {pos[u], pos[u] + sz[u] - 1};
    }

    int index(node_index u) const {
      return pos[u];
    }

  private:
    void dfs_sz(node_index u) {
      int max_sz = 0;
      g.call_adjacent(u, [&](int e_idx) {
          const auto& edge = g.edge(e_idx);
          node_index v = edge.to;
          if (v != parent[u]) {
            parent[v] = u;
            depth[v] = depth[u] + 1;
            dfs_sz(v);
            sz[u] += sz[v];
            if (sz[v] > max_sz) {
              max_sz = sz[v];
              heavy[u] = v;
            }
          }
      });
    }

    void dfs_hld(node_index u) {
      pos[u] = timer++;
      if (heavy[u] != -1) {
        head[heavy[u]] = head[u];
        dfs_hld(heavy[u]);
      }
      g.call_adjacent(u, [&](int e_idx) {
          const auto& edge = g.edge(e_idx);
          node_index v = edge.to;
          if (v != parent[u] && v != heavy[u]) {
          head[v] = v;
          dfs_hld(v);
          }
      });
    }
  };
}
#endif // CP_ALGO_GRAPH_HEAVY_LIGHT_HPP
