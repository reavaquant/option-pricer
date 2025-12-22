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

  /// Set the depth of the binary tree.
  ///
  /// @param depth the number of levels in the binary tree.
  ///
  /// @throws std::invalid_argument if depth < 0.
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

  /// Set a node in the binary tree.
  ///
  /// @param n the level of the binary tree (0 <= n <= depth_).
  /// @param i the index of the node in the level (0 <= i <= n).
  /// @param value the value to be stored in the node.
  ///
  /// @throws std::out_of_range if n or i are out of range.
  void setNode(int n, int i, const T& value) {
    checkIndices(n, i);
    tree_[n][i] = value;
  }

  /// @brief Get a node in the binary tree.
  ///
  /// @param n the level of the binary tree (0 <= n <= depth_).
  /// @param i the index of the node in the level (0 <= i <= n).
  ///
  /// @return the value stored in the node.
  ///
  /// @throws std::out_of_range if n or i are out of range.
  T getNode(int n, int i) const {
    checkIndices(n, i);
    return tree_[n][i];
  }

  /// Display the binary tree in a formatted manner.
  ///
  /// The tree is displayed with each level on a separate line,
  /// and each node is separated by a space. The tree is also
  /// indented to show the structure of the tree.
  ///
  /// @param os the output stream to write to (default is std::cout).
  void display(std::ostream& os = std::cout) const {
    const std::size_t w = valueWidth();
    const int gap = (int)(w + 2);

    int indent = 0;
    std::string val;
    int pad = 0;
    int connIndent = 0;

    for (int n = 0; n <= depth_; ++n) {
      indent = (depth_ - n) * gap / 2;
      if (indent < 0) indent = 0;
      os << std::string(indent, ' ');
      for (int i = 0; i <= n; ++i) {
        std::ostringstream ss;
        ss << tree_[n][i];
        val = ss.str();
        pad = gap - (int)val.size();
        if (pad < 1) pad = 1;
        os << val;
        if (i < n) os << std::string(pad, ' ');
      }
      os << '\n';

      if (n < depth_) {
        connIndent = indent - 1;
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

  /// @brief Check if the indices are valid.
  ///
  /// @param n the level of the binary tree (0 <= n <= depth_).
  /// @param i the index of the node in the level (0 <= i <= n).
  ///
  /// @throws std::out_of_range if n or i are out of range.
  void checkIndices(int n, int i) const {
    if (n < 0 || n > depth_) throw std::out_of_range("BinaryTree: n out of range");
    if (i < 0 || i > n) throw std::out_of_range("BinaryTree: i out of range");
  }

  /// @brief Compute the maximum width of the values in the binary tree.
  ///
  /// This function iterates over all values in the binary tree and computes
  /// the maximum width of the values when converted to a string.
  ///
  /// @return The maximum width of the values in the binary tree.
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
