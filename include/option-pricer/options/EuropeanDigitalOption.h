#ifndef EUROPEANDIGITALOPTION_H
#define EUROPEANDIGITALOPTION_H
#include "Option.h"

class BlackScholesPricer; // forward declaration

class EuropeanDigitalOption : public Option {
protected:
    double _strike;
public:
    EuropeanDigitalOption(double expiry, double strike);

    OptionType getOptionType() const override = 0;
    double payoff(double asset_price) const override = 0;

    double getStrike() const;

    friend class BlackScholesPricer; // give access to _strike
};

#endif
