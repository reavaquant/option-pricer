#include "AmericanOption.h"

AmericanOption::AmericanOption(double expiry, double strike) : Option(expiry), strike_(strike) {
    if (strike < 0.0) {
        throw std::invalid_argument("AmericanOption: strike must be nonnegative");
    }
}

/**
 * Returns the strike price of the American option.
 *
 * @return the strike price of the option.
 */
double AmericanOption::getStrike() const {
    return strike_;
}

/**
 * Returns true if the option is an American option.
 *
 * This function is used to identify which type of option an object is.
 * @return true if the option is an American option, false otherwise.
 */
bool AmericanOption::isAmericanOption() const {
    return true;
}
