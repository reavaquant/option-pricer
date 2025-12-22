#include <cmath>
#include <stdexcept>
#include <vector>
#include "BlackScholesMCPricer.h"
#include "MT.h"



/**
 * @brief Construct a BlackScholesMCPricer instance.
 * @details This pricer uses Monte Carlo simulation to estimate the price of an option.
 * It is constructed with an option, an initial price, an interest rate, and a volatility.
 * The option must not be null, and the time steps of the option will be cached for future use.
 * If the option is an AsianOption, then the time steps of the AsianOption will be used.
 * Otherwise, the time steps will be initialized with the expiry time of the option.
 * @param option The option to be priced.
 * @param initial_price The initial price of the underlying asset.
 * @param interest_rate The interest rate of the risk-free asset.
 * @param volatility The volatility of the underlying asset.
 */
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) : _option(option), _initial_price(initial_price), _interest_rate(interest_rate), _volatility(volatility) {
    if (!_option) {
        throw std::invalid_argument("BlackScholesMCPricer: option pointer must not be null");
    }

    _nb_paths = 0;
    _estimate = 0.0;
    _M2 = 0.0;

    _time_steps = _option->getTimeSteps();

    std::size_t steps = _time_steps.size();
    if (steps == 0) {
        throw std::invalid_argument("BlackScholesMCPricer: need at least one time step");
    }
    _drift_dt.resize(steps);
    _vol_sqrt_dt.resize(steps);
    double last_t = 0.0;
    const double drift = _interest_rate - 0.5 * _volatility * _volatility;
    std::size_t idx = 0;
    double t = 0.0;
    double dt = 0.0;
    while (idx < steps) {
        t = _time_steps[idx];
        dt = t - last_t;
        if (dt <= 0.0) {
            throw std::invalid_argument("BlackScholesMCPricer: time steps must be increasing");
        }
        _drift_dt[idx] = drift * dt;
        _vol_sqrt_dt[idx] = _volatility * std::sqrt(dt);
        last_t = t;
        idx++;
    }
    _maturity = _time_steps[steps - 1];
}

/**
 * @brief Return the number of paths used for Monte Carlo simulation.
 * @return The number of paths used for Monte Carlo simulation.
 * 
 * This function returns the number of paths used for Monte Carlo simulation of an option.
 * The number of paths is set by the generate() function.
 */
int BlackScholesMCPricer::getNbPaths() const {
    return _nb_paths;
}


/**
 * @brief Generate Monte Carlo paths for an option.
 * @details This function generates Monte Carlo paths for an option and updates the estimate of the option price.
 * The number of paths is set by the user, and the function generates both positive and negative paths if the number of paths is odd.
 * The paths are constructed by simulating the underlying asset price at each time step, and the payoff is calculated at the expiry time of the option.
 * The estimate of the option price is updated using the welford algorithm for variance.
 * @param nb_paths The number of Monte Carlo paths to generate.
 */
void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) {
        return;
    }

    const std::size_t steps = _time_steps.size();
    std::vector<double> path_pos(steps);
    std::vector<double> path_neg(steps);
    const double df = std::exp(-_interest_rate * _maturity);
    double s_pos = 0.0;
    double s_neg = 0.0;
    double z = 0.0;
    double payoff_discounted = 0.0;
    double delta = 0.0;

    int generated = 0;
    while (generated < nb_paths) {
        s_pos = _initial_price;
        s_neg = _initial_price;

        for (std::size_t k = 0; k < steps; ++k) { // construct both path
            z = MT::rand_norm();
            s_pos *= std::exp(_drift_dt[k] + _vol_sqrt_dt[k] * z);
            s_neg *= std::exp(_drift_dt[k] - _vol_sqrt_dt[k] * z);
            path_pos[k] = s_pos;
            path_neg[k] = s_neg;
        }

        payoff_discounted = df * _option->payoffPath(path_pos);
        // update mean
        _nb_paths++;
        delta = payoff_discounted - _estimate;
        double n = (double)_nb_paths;
        _estimate += delta / n;
        _M2 += delta * (payoff_discounted - _estimate); // welford algorithm for variance
        generated++;

        if (generated < nb_paths) { //add negative path if nb_paths is odd
            payoff_discounted = df * _option->payoffPath(path_neg);
            _nb_paths++;
            delta = payoff_discounted - _estimate;
            n = (double)_nb_paths;
            _estimate += delta / n;
            _M2 += delta * (payoff_discounted - _estimate);
            generated++;
        }
    }
}

/**
 * @brief Return the estimated price of the option using Monte Carlo simulation.
 * 
 * @return The estimated price of the option.
 * 
 * @throws std::logic_error If generate() has not been called before requesting the price.
 */
double BlackScholesMCPricer::price() {
    if (_nb_paths == 0) {
        throw std::logic_error("BlackScholesMCPricer: call generate() before requesting price");
    }
    return _estimate;
}

/**
 * @brief Calculate the price of the option using Monte Carlo simulation.
 * 
 * @return The estimated price of the option.
 * 
 * This function is an alias for the price() function.
 */
double BlackScholesMCPricer::operator()() {
    return price();
}

/**
 * @brief Return the 95% confidence interval of the estimated price of the option.
 * @return A vector containing the lower and upper bounds of the 95% confidence interval.
 * 
 * The confidence interval is calculated using the standard error of the estimated price
 * and the normal distribution.
 * If generate() has not been called with at least two paths, then a logic_error exception is thrown.
 */
std::vector<double> BlackScholesMCPricer::confidenceInterval() {
    if (_nb_paths < 2) {
        throw std::logic_error("BlackScholesMCPricer: need at least two paths for confidence interval");
    }
    double variance = _M2 / static_cast<double>(_nb_paths - 1);
    double std_err = std::sqrt(variance / static_cast<double>(_nb_paths));
    const double z = 1.96;
    return {_estimate - z * std_err, _estimate + z * std_err};
}
