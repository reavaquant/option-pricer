#include "Option.hpp"

Option::Option(double expiry) : expiry_(expiry) {}

double Option::getExpiry() const {
    return expiry_;
}