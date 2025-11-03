#ifndef EUROPEANVANILLAOPTION_HPP
#define EUROPEANVANILLAOPTION_HPP
#include "Option.hpp"

class BlackScholesPricer; // forward declaration

class EuropeanVanillaOption : public Option {
protected:
    double strike_;
public:
    EuropeanVanillaOption(double expiry, double strike);

    OptionType getOptionType() const override = 0;
    double payoff(double asset_price) const override = 0;

    double getStrike() const;

    friend class BlackScholesPricer; // give access to _strike
};

#endif