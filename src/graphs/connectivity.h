// Author: Igor Vayzert <igor.weisert@gmail.com>

#ifndef GRAPHS_CONNECTIVITY_H_
#define GRAPHS_CONNECTIVITY_H_

#include <cassert>
#include <vector>

namespace graphs {

class Connectivity {
 public:
  explicit Connectivity(int size) {
    assert(size > 1);
    id_.reserve(size);
    for (int i = 0; i < size; ++i)
      id_.push_back(i);
  }

  bool Connected(int u, int v) {
    assert(u > -1 && u < Size());
    assert(v > -1 && v < Size());
    return Root(u) == Root(v);
  }

  void Union(int u, int v) {
    assert(u > -1 && u < Size());
    assert(v > -1 && v < Size());
    int u_root = Root(u);
    int v_root = Root(v);
    id_[u_root] = v_root;
  }

  int Size() const {
    return id_.size();
  }

 private:
  int Root(int u) {
    while (id_[u] != u) {
      id_[u] = id_[id_[u]];
      u = id_[u];
    }
    return u;
  }

  std::vector<int> id_;
};

}  // namespace graphs

#endif  // GRAPHS_CONNECTIVITY_H_
