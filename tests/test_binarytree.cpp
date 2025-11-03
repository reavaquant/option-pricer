#include <cassert>
#include <sstream>
#include <stdexcept>

#include "option-pricer/datastruct/BinaryTree.hpp"

int main() {
    BinaryTree<double> tree;
    tree.setDepth(3);

    tree.setNode(0, 0, 100.0);
    tree.setNode(1, 0, 110.0);
    tree.setNode(1, 1, 90.0);
    tree.setNode(2, 1, 105.0);

    assert(tree.getNode(0, 0) == 100.0);
    assert(tree.getNode(1, 0) == 110.0);
    assert(tree.getNode(1, 1) == 90.0);
    assert(tree.getNode(2, 1) == 105.0);
    assert(tree.depth() == 3);

    std::ostringstream oss;
    tree.display(oss);
    const std::string output = oss.str();
    assert(output.find("100") != std::string::npos);
    assert(output.find("110") != std::string::npos);
    assert(output.find("90")  != std::string::npos);
    assert(output.find("105") != std::string::npos);

    // Vérifie les exceptions pour indices invalides
    bool thrown = false;
    try { tree.getNode(4, 0); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    thrown = false;
    try { tree.setNode(2, 4, 0.0); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    return 0;
}