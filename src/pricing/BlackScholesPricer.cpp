#include "BlackScholesPricer.hpp"
#include <algorithm>
#include <cmath>

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

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0.0), asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(false) {}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility) : option_(option), strike_(option ? option->getStrike() : 0.0),asset_price_(asset_price), interest_rate_(interest_rate), volatility_(volatility), is_digital_(true) {}

/**
 * @brief Compute d1 and d2 values for Black-Scholes pricing formula.
 * 
 * @details The Black-Scholes pricing formula requires two normal CDF values, d1 and d2.
 * These values are computed based on the asset price, strike, interest rate, volatility, and maturity time.
 * The computed values are then used to calculate the price of the option.
 * 
 * @return A struct containing the computed d1 and d2 values.
 */
BlackScholesPricer::DValues BlackScholesPricer::computeDValues() const {
    const double T = std::max(option_->getExpiry(), kMinMaturity); // avoid division by zero
    const double volatility = std::max(volatility_, kMinVolatility);
    const double denominator = volatility * std::sqrt(T);
    const double safe_strike = std::max(strike_, kMinPrice);
    const double safe_spot = std::max(asset_price_, kMinPrice);

    const double d1 = (std::log(safe_spot / safe_strike) + (interest_rate_ + 0.5 * volatility * volatility) * T) / denominator;
    return {d1, d1 - denominator};
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
    const double T = option_->getExpiry();
    if (T <= 0.0 || volatility_ < kMinVolatility) {
        return option_->payoff(asset_price_); // payoff at maturity or no volatility
    }

    const DValues d = computeDValues();
    if (!is_digital_) {
        if (option_->getOptionType() == OptionType::Call) {
            return asset_price_ * normalCdf(d.d1) - strike_ * std::exp(-interest_rate_ * T) * normalCdf(d.d2);
        }
        return strike_ * std::exp(-interest_rate_ * T) * normalCdf(-d.d2) - asset_price_ * normalCdf(-d.d1);
    }
    const double discount = std::exp(-interest_rate_ * T);
    if (option_->getOptionType() == OptionType::Call) {
        return discount * normalCdf(d.d2);
    }
    return discount * normalCdf(-d.d2);
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
