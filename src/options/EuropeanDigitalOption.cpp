#include "EuropeanDigitalOption.h"
#include <stdexcept>

/**
 * Constructs a European digital option with the given expiry and strike price.
 *
 * @param expiry the expiry date of the option.
 * @param strike the strike price of the option.
 * @throws std::invalid_argument if expiry or strike is negative.
 */
EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike) : Option(expiry), _strike(strike) {
    if (strike < 0.0) {
        throw std::invalid_argument("EuropeanDigitalOption: strike must be nonnegative");
    }
}

/**
 * Returns the strike price of the European digital option.
 *
 * @return the strike price of the option.
 */
double EuropeanDigitalOption::getStrike() const {
    return _strike;
}
