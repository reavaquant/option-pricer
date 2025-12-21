#include "EuropeanVanillaOption.h"
#include <stdexcept>

/**
 * Constructs a European Vanilla Option with the given expiry and strike price.
 *
 * @param expiry the expiry date of the option.
 * @param strike the strike price of the option.
 * @throws std::invalid_argument if expiry or strike is negative.
 */
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike) {
    if (strike < 0) {
        throw std::invalid_argument("EuropeanVanillaOption: strike must be nonnegative");
    }
}

/**
 * Returns the strike price of the European Vanilla Option.
 *
 * @return the strike price of the option.
 */
double EuropeanVanillaOption::getStrike() const {
    return _strike;
}
