#include "BinaryTree.hpp"
#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>  // invalid_argument, out_of_range

template <class T>
BinaryTree<T>::BinaryTree() = default;

template <class T>
BinaryTree<T>::BinaryTree(int depth) { setDepth(depth); }

/**
 * Set the depth of the binary tree.
 *
 * @param depth the depth of the tree
 * @throws std::invalid_argument if depth < 0
 */
template <class T>
void BinaryTree<T>::setDepth(int depth) {
  if (depth < 0) throw std::invalid_argument("BinaryTree::setDepth depth < 0");
  depth_ = depth;
  tree_.clear();
  tree_.resize(depth_ + 1);
  for (int n = 0; n <= depth_; ++n) {
    tree_[n].clear();
    tree_[n].resize(n + 1);
  }
}

template <class T>
int BinaryTree<T>::depth() const { return depth_; }

template <class T>
void BinaryTree<T>::setNode(int n, int i, const T& value) {
  checkIndices(n, i);
  tree_[n][i] = value;
}

template <class T>
T BinaryTree<T>::getNode(int n, int i) const {
  checkIndices(n, i);
  return tree_[n][i];
}

/**
 * Returns the maximum width of the nodes in the binary tree.
 *
 * This is useful for printing the binary tree in a formatted way.
 *
 * @return the maximum width of the nodes in the binary tree
 */
template <class T>
std::size_t BinaryTree<T>::valueWidth() const {
  std::size_t width = 1;
  for (const auto& level : tree_) {
    for (const auto& value : level) {
      std::ostringstream oss;
      oss << value;
      width = std::max(width, oss.str().size());
    }
  }
  return width;
}


/**
 * Displays the binary tree in a formatted way.
 *
 * Each level of the tree is printed on a new line,
 * with leading spaces to indent the tree.
 * The values at each node are separated by a single space.
 *
 * @param os a std::ostream to write to
 */
template <class T>
void BinaryTree<T>::display(std::ostream& os) const {
  const std::size_t cellWidth = valueWidth() + 2;
  for (int n = 0; n <= depth_; ++n) {
    const std::size_t indent = static_cast<std::size_t>(
        std::max(0, (depth_ - n) * static_cast<int>(cellWidth) / 2));
    os << std::string(indent, ' ');
    for (int i = 0; i <= n; ++i) {
      os << std::setw(static_cast<int>(cellWidth)) << tree_[n][i];
    }
    os << '\n';
  }
}


/**
 * Checks if n and i are valid indices for the binary tree.
 *
 * @throws std::out_of_range if n or i are out of range
 * @param n the level of the tree
 * @param i the index of the node
 */
template <class T>
void BinaryTree<T>::checkIndices(int n, int i) const {
  if (n < 0 || n > depth_) throw std::out_of_range("BinaryTree: n out of range");
  if (i < 0 || i > n)      throw std::out_of_range("BinaryTree: i out of range");
}
