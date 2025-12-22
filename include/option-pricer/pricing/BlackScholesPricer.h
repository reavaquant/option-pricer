#ifndef BLACKSCHOLESPRICER_H
#define BLACKSCHOLESPRICER_H

#include "EuropeanDigitalOption.h"
#include "EuropeanVanillaOption.h"

class BlackScholesPricer {
private:
    EuropeanVanillaOption* _option{nullptr};
    EuropeanDigitalOption* _digital_option{nullptr};
    double _strike;
    double _asset_price;
    double _interest_rate;
    double _volatility;
    bool _is_digital;
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);
    double price() const;
    double delta() const;
    double operator()() const;
};


#endif
