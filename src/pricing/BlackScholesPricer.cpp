#include "BlackScholesPricer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

/**
 * @brief Compute the cumulative distribution function of the standard normal distribution.
 * 
 * @param x The value at which to compute the cumulative distribution function.
 * @return The cumulative distribution function of the standard normal distribution evaluated at x.
 */
static double normalCdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

/**
 * @brief Compute the probability density function of the standard normal distribution.
 * 
 * @param x The value at which to compute the probability density function.
 * @return The probability density function of the standard normal distribution evaluated at x.
 */
static double normalPdf(double x) {
    const double inv_sqrt_2pi = 1 / std::sqrt(2.0 * M_PI);
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double interest_rate, double volatility) : _option(option), _strike(option ? option->getStrike() : 0), _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility), _is_digital(false) {
    if (_asset_price <= 0.0 || _volatility <= 0.0 || _option == nullptr || _strike <= 0.0) {
        throw std::invalid_argument("BlackScholesPricer: invalid parameters");
    }
}

BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option, double asset_price, double interest_rate, double volatility) : _digital_option(option), _strike(option ? option->getStrike() : 0), _asset_price(asset_price), _interest_rate(interest_rate), _volatility(volatility), _is_digital(true) {
    if (_asset_price <= 0.0 || _volatility <= 0.0 || _digital_option == nullptr || _strike <= 0.0) {
        throw std::invalid_argument("BlackScholesPricer: invalid parameters");
    }
}


/**
 * @brief Return the estimated price of the option using the Black-Scholes model.
 * @return The estimated price of the option.
 * 
 * The price of the option is calculated using the Black-Scholes model.
 * If the time to maturity is less than or equal to zero, then the price of the option at maturity is returned.
 * Otherwise, the price of the option is calculated using the Black-Scholes model.
 */
double BlackScholesPricer::price() const {
    const double T = _is_digital ? _digital_option->getExpiry() : _option->getExpiry();
    if (T <= 0.0) {
        return _is_digital ? _digital_option->payoff(_asset_price) : _option->payoff(_asset_price);
    }
    const double K = _strike;
    double sigma_sqrt_T = _volatility * std::sqrt(T);
    double d1 = (std::log(_asset_price / K) + (_interest_rate + 0.5 * _volatility * _volatility) * T) / sigma_sqrt_T;
    double d2 = d1 - sigma_sqrt_T;

    if (_is_digital) {
        double disc = std::exp(-_interest_rate * T);
        if (_digital_option->getOptionType() == OptionType::Call) {
            return disc * normalCdf(d2);
        }
        return disc * normalCdf(-d2);
    }

    if (_option->getOptionType() == OptionType::Call)
        return _asset_price * normalCdf(d1) - K * std::exp(-_interest_rate * T) * normalCdf(d2);
    return K * std::exp(-_interest_rate * T) * normalCdf(-d2) - _asset_price * normalCdf(-d1);
}


/**
 * @brief Calculate the delta of the option using the Black-Scholes model.
 * @return The delta of the option.
 * 
 * The delta of the option is calculated using the Black-Scholes model.
 * If the time to maturity is less than or equal to zero, then the delta of the option at maturity is returned.
 * Otherwise, the delta of the option is calculated using the Black-Scholes model.
 */
double BlackScholesPricer::delta() const {
    const double T = _is_digital ? _digital_option->getExpiry() : _option->getExpiry();
    if (T <= 0.0) {
        if (_is_digital) return 0.0;
        return (_option->getOptionType() == OptionType::Call) ? (_asset_price > _strike ? 1.0 : 0.0) : (_asset_price < _strike ? -1.0 : 0.0);
    }

    double sigma_sqrt_T = _volatility * std::sqrt(T);
    double d1 = (std::log(_asset_price / _strike) + (_interest_rate + 0.5 * _volatility * _volatility) * T) / sigma_sqrt_T;

    if (_is_digital) {
        const double d2 = d1 - sigma_sqrt_T;
        const double disc = std::exp(-_interest_rate * T);
        const double factor = disc * normalPdf(d2) / (_asset_price * sigma_sqrt_T);
        return (_digital_option->getOptionType() == OptionType::Call) ? factor : -factor;
    }

    return _option->getOptionType() == OptionType::Call ? normalCdf(d1) : normalCdf(d1) - 1.0;
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
