#pragma once
#include <iostream>
#include <stdexcept>
#include <sstream>
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
    const std::size_t w = valueWidth();
    const int gap = (int)(w + 2);
    for (int n = 0; n <= depth_; ++n) {
      int indent = (depth_ - n) * gap / 2;
      if (indent < 0) indent = 0;
      os << std::string(indent, ' ');
      for (int i = 0; i <= n; ++i) {
        std::ostringstream ss;
        ss << tree_[n][i];
        const std::string val = ss.str();
        int pad = gap - (int)val.size();
        if (pad < 1) pad = 1;
        os << val;
        if (i < n) os << std::string(pad, ' ');
      }
      os << '\n';

      if (n < depth_) {
        int connIndent = indent - 1;
        if (connIndent < 0) connIndent = 0;
        os << std::string(connIndent, ' ');
        for (int i = 0; i <= n; ++i) {
          os << "/ \\";
          if (i < n) os << std::string(gap - 1, ' ');
        }
        os << '\n';
      }
    }
  }

private:
  int depth_;
  std::vector<std::vector<T>> tree_;

  void checkIndices(int n, int i) const {
    if (n < 0 || n > depth_) throw std::out_of_range("BinaryTree: n out of range");
    if (i < 0 || i > n) throw std::out_of_range("BinaryTree: i out of range");
  }

  std::size_t valueWidth() const {
    std::size_t w = 1;
    for (const auto& level : tree_) {
      for (const auto& v : level) {
        std::ostringstream ss;
        ss << v;
        const std::size_t len = ss.str().size();
        if (len > w) w = len;
      }
    }
    return w;
  }
};
