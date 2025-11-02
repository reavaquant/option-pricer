#include "Option.hpp"

Option::Option(double t) : _expiry(t) {}

double Option::getExpiry() const {
    return _expiry;
}