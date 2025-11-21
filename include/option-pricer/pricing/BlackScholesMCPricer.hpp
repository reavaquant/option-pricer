#ifndef BLACKSCHOLESMCPRICER_HPP
#define BLACKSCHOLESMCPRICER_HPP

#include <vector>
#include "Option.hpp"
#include "EuropeanVanillaOption.hpp"

class BlackScholesMCPricer {
public:
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
    double price();
    double delta();
    long long getNbPaths() const;
    void generate(int nb_paths);
    double operator()();
    std::vector<double> confidenceInterval();
private:
    Option* option_;
    double initial_price_;
    double interest_rate_;
    double volatility_;
    long long nb_paths_{0};
    double estimate_{0.0};
    double M2_{0.0};
};

#endif
