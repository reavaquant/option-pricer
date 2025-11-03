#pragma once
#include <iosfwd>
#include <vector>

// Arbre triangulaire pour CRR : niveau n contient n+1 éléments.
template <class T>
class BinaryTree {
public:
  BinaryTree();
  explicit BinaryTree(int depth);

  void setDepth(int depth);
  int depth() const;

  void setNode(int n, int i, const T& value);
  T getNode(int n, int i) const;

  void display(std::ostream& os) const;

private:
  int depth_{0};
  std::vector<std::vector<T>> tree_;

  void checkIndices(int n, int i) const; // lance std::out_of_range si invalide
  std::size_t valueWidth() const;
};

#include "BinaryTree.tpp"
