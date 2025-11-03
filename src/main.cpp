#include <iostream>
#include "CallOption.hpp"
#include "PutOption.hpp"
#include "BlackScholesPricer.hpp"
#include "BinaryTree.hpp"

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);

    std::cout << "Call payoff @110 = " << call.payoff(110.0) << "\n";
    std::cout << "Put payoff @90  = " << put.payoff(90.0) << "\n";

    const double spot = 100.0;
    const double rate = 0.05;
    const double vol  = 0.2;

    // ---- BlackScholesPricer ----

    BlackScholesPricer callPricer(&call, spot, rate, vol);
    BlackScholesPricer putPricer(&put, spot, rate, vol);

    std::cout << "\nCall price (Black-Scholes) = " << callPricer.price() << '\n';
    std::cout << "Call delta = " << callPricer.delta() << '\n';
    std::cout << "Put price (Black-Scholes) = " << putPricer.price() << '\n';
    std::cout << "Put delta = " << putPricer.delta() << '\n';

    std::cout << "Call price via operator() = " << callPricer() << '\n'; // operator() simply renvoie price()

    // ---- BinaryTree ----
    BinaryTree<double> tree(3);
    tree.setNode(0, 0, 100.0);
    tree.setNode(1, 0, 110.0);
    tree.setNode(1, 1, 90.0);
    tree.setNode(2, 0, 121.0);
    tree.setNode(2, 1, 99.0);
    tree.setNode(2, 2, 81.0);
    tree.setNode(3, 0, 133.1);
    tree.setNode(3, 1, 108.9);
    tree.setNode(3, 2, 89.1);
    tree.setNode(3, 3, 72.9);

    std::cout << "\nBinary tree (depth " << tree.depth() << "):\n";
    tree.display(std::cout);

    return 0;
}