#ifndef BLACKSCHOLESPRICER_HPP
#define BLACKSCHOLESPRICER_HPP

#include "EuropeanDigitalOption.hpp"
#include "EuropeanVanillaOption.hpp"

class BlackScholesPricer {
public:
    BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility);
    BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility);
    double price() const;
    double delta() const;
    double operator()() const;

private:
    Option* option_;
    double strike_;
    double asset_price_;
    double interest_rate_;
    double volatility_;
    bool is_digital_;

    struct DValues { //utility struct for normal cdf values
        double d1;
        double d2;
    };
    DValues computeDValues() const;
};


#endif
