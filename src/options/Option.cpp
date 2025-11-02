#include "Option.hpp"

Option::Option(double t){
    t = _expiry;
}

double Option::getExpiry() const {
    return _expiry;
}