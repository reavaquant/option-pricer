#include <iostream>
#include "CallOption.hpp"
#include "PutOption.hpp"
#include "BlackScholesPricer.hpp"

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

    return 0;
}