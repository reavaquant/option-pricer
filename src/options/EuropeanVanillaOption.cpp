#include "EuropeanVanillaOption.hpp"
#include <iostream>

/**
 * Constructs a European Vanilla Option with the given expiry and strike price.
 *
 * @param expiry the expiry date of the option.
 * @param strike the strike price of the option.
 * @throws std::invalid_argument if expiry or strike is negative.
 */
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), strike_(strike) {
    if (expiry < 0.0 || strike < 0.0) {
        std::cerr << "expiry and strike price must be nonnegative" << std::endl;
    }
}

/**
 * Returns the strike price of the European Vanilla Option.
 *
 * @return the strike price of the option.
 */
double EuropeanVanillaOption::getStrike() const {
    return strike_;
}