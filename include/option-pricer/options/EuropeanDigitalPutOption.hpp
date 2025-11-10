#ifndef EUROPEANDIGITALPUTOPTION_HPP
#define EUROPEANDIGITALPUTOPTION_HPP

#include "EuropeanDigitalOption.hpp"

class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalPutOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double asset_price) const override;
};

#endif
