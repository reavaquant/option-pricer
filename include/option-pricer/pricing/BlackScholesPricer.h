#ifndef BLACKSCHOLESPRICER_H
#define BLACKSCHOLESPRICER_H

#include "EuropeanDigitalOption.h"
#include "EuropeanVanillaOption.h"

class BlackScholesPricer {
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);
    double price() const;
    double delta() const;
    double operator()() const;

private:
    EuropeanVanillaOption* option_{nullptr};
    EuropeanDigitalOption* digital_option_{nullptr};
    double strike_{0.0};
    double asset_price_{0.0};
    double interest_rate_{0.0};
    double volatility_{0.0};
    bool is_digital_{false};
};


#endif
