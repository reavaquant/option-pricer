#include "BlackScholesPricer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

static double normalCdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

static double normalPdf(double x) {
    const double inv_sqrt_2pi = 1 / std::sqrt(2.0 * M_PI);
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0), asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(false) {
    if (asset_price_ <= 0.0 || volatility_ <= 0.0 || option_ == nullptr || strike_ <= 0.0) {
        throw std::invalid_argument("BlackScholesPricer: invalid parameters");
    }
}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility) : digital_option_(option), strike_(option ? option->getStrike() : 0),asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(true) {
    if (asset_price_ <= 0.0 || volatility_ <= 0.0 || digital_option_ == nullptr || strike_ <= 0.0) {
        throw std::invalid_argument("BlackScholesPricer: invalid parameters");
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
    const double T = is_digital_ ? digital_option_->getExpiry() : option_->getExpiry();
    if (T <= 0.0) {
        return is_digital_ ? digital_option_->payoff(asset_price_) : option_->payoff(asset_price_);
    }
    const double K = strike_;
    double sigma_sqrt_T = volatility_ * std::sqrt(T);
    double d1 = (std::log(asset_price_ / K) + (interest_rate_ + 0.5 * volatility_ * volatility_) * T) / sigma_sqrt_T;
    double d2 = d1 - sigma_sqrt_T;

    if (is_digital_) {
        double disc = std::exp(-interest_rate_ * T);
        if (digital_option_->getOptionType() == OptionType::Call) {
            return disc * normalCdf(d2);
        }
        return disc * normalCdf(-d2);
    }

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
    const double T = is_digital_ ? digital_option_->getExpiry() : option_->getExpiry();
    if (T <= 0.0) {
        if (is_digital_) return 0.0;
        return (option_->getOptionType() == OptionType::Call) ? (asset_price_ > strike_ ? 1.0 : 0.0) : (asset_price_ < strike_ ? -1.0 : 0.0);
    }

    double sigma_sqrt_T = volatility_ * std::sqrt(T);
    double d1 = (std::log(asset_price_ / strike_) + (interest_rate_ + 0.5 * volatility_ * volatility_) * T) / sigma_sqrt_T;

    if (is_digital_) {
        const double d2 = d1 - sigma_sqrt_T;
        const double disc = std::exp(-interest_rate_ * T);
        const double factor = disc * normalPdf(d2) / (asset_price_ * sigma_sqrt_T);
        return (digital_option_->getOptionType() == OptionType::Call) ? factor : -factor;
    }

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
