#include "BlackScholesPricer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
    constexpr double kMinMaturity = 1e-9; 
    constexpr double kMinVolatility = 1e-12;
    constexpr double kMinPrice = 1e-12;
    constexpr double kInvSqrtTwoPi = 0.39894228040143267794; // 1 / sqrt(2 * pi)

    double normalCdf(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0)); // standard normal CDF
    }

    double normalPdf(double x) {
        return kInvSqrtTwoPi * std::exp(-0.5 * x * x);
    }
}

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0.0), asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(false) {
    if (!option_) {
        throw std::invalid_argument("BlackScholesPricer: option pointer must not be null");
    }
}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0.0),asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(true) {
    if (!option_) {
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
    if (T <= 0.0 || volatility_ <= 0.0) return option_->payoff(asset_price_);
    double K = option_->_strike; // friend access
    double sigma_sqrt_T = volatility_ * std::sqrt(T);
    double d1 = (std::log(asset_price_ / K) + (interest_rate_ + 0.5 * volatility_ * volatility_) * T) / sigma_sqrt_T;
    double d2 = d1 - sigma_sqrt_T;
    auto N = [](double x){ return 0.5 * std::erfc(-x / std::sqrt(2.0)); };
    if (option_->getOptionType() == OptionType::Call)
        return asset_price_ * N(d1) - K * std::exp(-interest_rate_ * T) * N(d2);
    return K * std::exp(-interest_rate_ * T) * N(-d2) - asset_price_ * N(-d1);
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
    const double T = option_->getExpiry();
    if (T <= 0.0 || volatility_ < kMinVolatility) {
        if (is_digital_) {
            return 0.0;
        }
        if (option_->getOptionType() == OptionType::Call) {
            return asset_price_ > strike_ ? 1.0 : 0.0;
        }
        return asset_price_ < strike_ ? -1.0 : 0.0;
    }
    const DValues d = computeDValues();
    if (!is_digital_) {
        return option_->getOptionType() == OptionType::Call ? normalCdf(d.d1) : normalCdf(d.d1) - 1.0;
    }
    const double volatility = std::max(volatility_, kMinVolatility);
    const double denom = std::max(asset_price_, kMinPrice) * volatility * std::sqrt(T);
    const double discount = std::exp(-interest_rate_ * T);
    const double sensitivity = discount * normalPdf(d.d2) / denom;
    return option_->getOptionType() == OptionType::Call ? sensitivity : -sensitivity;
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
