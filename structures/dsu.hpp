#ifndef CP_ALGO_STRUCTURES_DSU_HPP
#define CP_ALGO_STRUCTURES_DSU_HPP 
#include <vector>
#include <numeric>
namespace cp_algo::structures {
  template<typename T> 
    void swap(T& x, T& y) {
      T temp = x;
      x = y;
      y = temp;
    }
  struct dsu {
    std::vector<int> parent, rank;
    dsu(int v) : parent(v) {
      std::iota(rank.begin(), rank.end(), 0);
    }

    int find(int v) {
      return (v == parent[v]) ? v : (parent[v] = find(parent[v]));
    }

    bool unite(int a, int b) {
      a = find(a), b = find(b);
      if (a == b) return false;
      if (rank[a] < rank[b]) cp_algo::structures::swap<int>(a, b);
      parent[b] = a;
      if (rank[a] == rank[b]) rank[a]++;
      return true;
    }
  };
}
#endif
