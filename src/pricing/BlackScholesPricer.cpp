#include "BlackScholesPricer.hpp"
#include <cmath>

DValues BlackScholesPricer::computeDValues() const {
    const double T = option_->getExpiry();
    const double K = option_->getStrike();
    const double volatility = volatility_;
    const double S = asset_price_;

    const double d1 = (std::log(S / K) + (interest_rate_ + 0.5 * volatility * volatility) * T) / (volatility * std::sqrt(T));
    return {d1, d1 - volatility * std::sqrt(T)};
}

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : option_(option), asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility) {}

double normalCdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0)); // standard normal CDF
}

double BlackScholesPricer::price() const {
    const double T = option_->getExpiry();
    const double K = option_->getStrike();
    const double r = interest_rate_;
    const double sigma = volatility_;
    const double S = asset_price_;

    if (T <= 0.0 || volatility_ < 1e-12) {
        return option_->payoff(asset_price_); // payoff at maturity or no volatility
    }

    DValues d = computeDValues();
    const double d1 = d.d1;
    const double d2 = d.d2;

    if (option_->getOptionType() == OptionType::Call) {
        return asset_price_ * normalCdf(d1) - K * std::exp(-interest_rate_ * T) * normalCdf(d2);
    }
    return  K * std::exp(-interest_rate_ * T) * normalCdf(-d2) - asset_price_ * normalCdf(-d1);
}

double BlackScholesPricer::delta() const {
    const double T = option_->getExpiry();
    if (T <= 0.0 || volatility_ < 1e-12) {
        return option_->getOptionType() == OptionType::Call ? 0.0 : -1.0;
    }

    DValues d = computeDValues();
    return option_->getOptionType() == OptionType::Call ? normalCdf(d.d1) : normalCdf(d.d1) - 1.0;
}

double BlackScholesPricer::operator()() const {
    return price();   
}