#ifndef EUROPEANVANILLAOPTION_HPP
#define EUROPEANVANILLAOPTION_HPP
#include "Option.hpp"

class EuropeanVanillaOption : public Option {
private:
    double _strike;
public:
    EuropeanVanillaOption(double expiry, double strike);
    OptionType getOptionType() const override;
    double payoff(double) const override;
};

#endif