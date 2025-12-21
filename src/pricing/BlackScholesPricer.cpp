#include "BlackScholesPricer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

static double normalCdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0), asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(false) {
    if (asset_price_ <= 0.0 || volatility_ <= 0.0 || option_->getExpiry() <= 0.0) {
        throw std::invalid_argument("BlackScholesPricer: invalid parameters");
    }
}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility) : digital_option_(option), strike_(option ? option->getStrike() : 0),asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(true) {
    if (!digital_option_) {
        throw std::invalid_argument("BlackScholesPricer: option pointer must not be null");
    }
}

/**
 * @brief Calculate the price of the option using the Black-Scholes model.
 * @return The price of the option.
 * 
 * The Black-Scholes model is used to calculate the price of the option.
 * If the time to maturity is less than or equal to zero or if the volatility is less than
 * or equal to zero, then the payoff of the option at maturity is returned.
 * Otherwise, the price of the option is calculated using the Black-Scholes model.
 */
double BlackScholesPricer::price() const {
    double T = option_->getExpiry();
    if (T <= 0.0) return option_->payoff(asset_price_);
    double K = option_-> strike_; // friend access
    double sigma_sqrt_T = volatility_ * std::sqrt(T);
    double d1 = (std::log(asset_price_ / K) + (interest_rate_ + 0.5 * volatility_ * volatility_) * T) / sigma_sqrt_T;
    double d2 = d1 - sigma_sqrt_T;
    if (option_->getOptionType() == OptionType::Call)
        return asset_price_ * normalCdf(d1) - K * std::exp(-interest_rate_ * T) * normalCdf(d2);
    return K * std::exp(-interest_rate_ * T) * normalCdf(-d2) - asset_price_ * normalCdf(-d1);
}

/**
 * @brief Compute the delta (sensitivity) of the option price with respect to the underlying asset price.
 * 
 * @return The delta (sensitivity) of the option price.
 * 
 * The delta (sensitivity) of the option price is computed using the Black-Scholes model.
 * If the time to maturity is less than or equal to zero or if the volatility is less than or equal to zero,
 * then the delta is computed as a step function.
 * Otherwise, the delta is computed as the partial derivative of the option price with respect to the underlying asset price.
 */
double BlackScholesPricer::delta() const {
    double T = option_->getExpiry();
    if (T <= 0.0) {
        return (option_->getOptionType() == OptionType::Call) ? (asset_price_ > option_-> strike_ ? 1.0 : 0.0) : (asset_price_ < option_-> strike_ ? -1.0 : 0.0);
    }
    double sigma_sqrt_T = volatility_ * std::sqrt(T);
    double d1 = (std::log(asset_price_ / option_-> strike_) + (interest_rate_ + 0.5 * volatility_ * volatility_) * T) / sigma_sqrt_T;
    return option_->getOptionType() == OptionType::Call ? normalCdf(d1) : normalCdf(d1) - 1.0;
}

/**
 * @brief Calculate the price of the option using the Black-Scholes model.
 * 
 * @return The price of the option.
 * 
 * This function is an alias for the price() function.
 * It is provided to allow the option price to be calculated using the operator().
 */
double BlackScholesPricer::operator()() const {
    return price();   
}
