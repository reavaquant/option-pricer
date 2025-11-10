#include "EuropeanDigitalOption.hpp"
#include <iostream>

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike) : Option(expiry), strike_(strike) {
    if (expiry < 0.0 || strike < 0.0) {
        std::cerr << "expiry and strike price must be nonnegative" << std::endl;
    }
}

double EuropeanDigitalOption::getStrike() const {
    return strike_;
}
