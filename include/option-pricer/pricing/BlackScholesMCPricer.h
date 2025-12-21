#ifndef BLACKSCHOLESMCPRICER_H
#define BLACKSCHOLESMCPRICER_H

#include <vector>
#include "Option.h"
#include "EuropeanVanillaOption.h"

class BlackScholesMCPricer {
private:
    Option* option_;
    double initial_price_;
    double interest_rate_;
    double volatility_;
    long long nb_paths_;
    double estimate_;
    double M2_;
    double maturity_;
    std::vector<double> time_steps_;
    std::vector<double> drift_dt_;
    std::vector<double> vol_sqrt_dt_;
    bool has_control_variate_{false};
    double control_mean_;
public:
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
    double price();
    long long getNbPaths() const;
    void generate(int nb_paths);
    double operator()();
    std::vector<double> confidenceInterval();
};

#endif
