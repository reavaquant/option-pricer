#ifndef EUROPEANVANILLAOPTION_HPP
#define EUROPEANVANILLAOPTION_HPP
#include "Option.hpp"

class EuropeanVanillaOption : public Option {
private:
    double _strike;
public:
    EuropeanVanillaOption(double, double) : Option(_expiry) {}; //pas sur de l'implementation de _expiray

};



#endif