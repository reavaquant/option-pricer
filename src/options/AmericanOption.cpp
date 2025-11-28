#include "AmericanOption.hpp"

AmericanOption::AmericanOption(double expiry, double strike) : Option(expiry), strike_(strike) {}

double AmericanOption::getStrike() const {
    return strike_;
}

bool AmericanOption::isAmericanOption() const {
    return true;
}
