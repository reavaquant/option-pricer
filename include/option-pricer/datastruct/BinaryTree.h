#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

template <class T>
class BinaryTree {
public:
  BinaryTree() : depth_(0) {}
  explicit BinaryTree(int depth) : depth_(0) { setDepth(depth); }

  void setDepth(int depth) {
    if (depth < 0) {
      throw std::invalid_argument("BinaryTree: depth must be >= 0");
    }
    depth_ = depth;
    tree_.assign(depth_ + 1, std::vector<T>());
    for (int n = 0; n <= depth_; ++n) {
      tree_[n].assign(n + 1, T());
    }
  }

  int depth() const { return depth_; }

  void setNode(int n, int i, const T& value) {
    checkIndices(n, i);
    tree_[n][i] = value;
  }

  T getNode(int n, int i) const {
    checkIndices(n, i);
    return tree_[n][i];
  }

  void display(std::ostream& os = std::cout) const {
    for (int n = 0; n <= depth_; ++n) {
      for (int i = 0; i <= n; ++i) {
        os << tree_[n][i];
        if (i < n) os << " ";
      }
      os << '\n';
    }
  }

private:
  int depth_;
  std::vector<std::vector<T>> tree_;

  void checkIndices(int n, int i) const {
    if (n < 0 || n > depth_) throw std::out_of_range("BinaryTree: n out of range");
    if (i < 0 || i > n) throw std::out_of_range("BinaryTree: i out of range");
  }
};
