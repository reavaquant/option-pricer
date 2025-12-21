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
    EuropeanVanillaOption* option_;
    EuropeanDigitalOption* digital_option_;
    double strike_;
    double asset_price_;
    double interest_rate_;
    double volatility_;
    bool is_digital_;
};


#endif
