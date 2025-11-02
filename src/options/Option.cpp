#include "Option.hpp"

Option::Option(double expiry) : _expiry(expiry) {}

double Option::getExpiry() const {
    return _expiry;
}