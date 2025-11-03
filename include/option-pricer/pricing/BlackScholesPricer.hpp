#ifndef BLACKSCHOLESPRICER_HPP
#define BLACKSCHOLESPRICER_HPP

#include "EuropeanVanillaOption.hpp"

struct DValues { //utility struct for normal cdf values
    double d1;
    double d2;
};

class BlackScholesPricer {
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    double price() const;
    double delta() const;
    double operator()() const;
    DValues computeDValues() const;

private:
    EuropeanVanillaOption* option_;
    double asset_price_;
    double interest_rate_;
    double volatility_;
};


#endif