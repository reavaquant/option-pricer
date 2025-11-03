#ifndef BLACKSCHOLESPRICER_HPP
#define BLACKSCHOLESPRICER_HPP

#include "EuropeanVanillaOption.hpp"

class BlackScholesPricer {
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    double price() const;
    double delta() const;
    double operator()() const;

private:
    EuropeanVanillaOption* option_;
    double asset_price_;
    double interest_rate_;
    double volatility_;
};


#endif