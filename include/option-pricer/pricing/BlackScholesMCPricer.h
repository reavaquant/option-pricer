#ifndef BLACKSCHOLESMCPRICER_H
#define BLACKSCHOLESMCPRICER_H

#include <vector>
#include "Option.h"
#include "EuropeanVanillaOption.h"

class BlackScholesMCPricer {
private:
    Option* _option;
    double _initial_price;
    double _interest_rate;
    double _volatility;
    int _nb_paths;
    double _estimate;
    double _M2;
    double _maturity;
    std::vector<double> _time_steps;
    std::vector<double> _drift_dt;
    std::vector<double> _vol_sqrt_dt;
public:
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
    double price();
    int getNbPaths() const;
    void generate(int nb_paths);
    double operator()();
    std::vector<double> confidenceInterval();
};

#endif
