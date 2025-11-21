#include <cmath>
#include <stdexcept>
#include <vector>
#include "AsianOption.hpp"
#include "BlackScholesMCPricer.hpp"
#include "MT.hpp"

/**
 * @brief Construct a BlackScholesMCPricer object.
 * @param option pointer to an Option object.
 * @param initial_price the initial price of the underlying asset.
 * @param interest_rate the interest rate of the risk-free asset.
 * @param volatility the volatility of the underlying asset.
 * 
 * Construct a BlackScholesMCPricer object with the given parameters.
 * If the option pointer is null, then an invalid_argument exception is thrown.
 */
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) : option_(option), initial_price_(initial_price), interest_rate_(interest_rate), volatility_(volatility) {
    if (!option_) {
        throw std::invalid_argument("BlackScholesMCPricer: option pointer must not be null");
    }
}

/**
 * @brief Return the number of paths used for Monte Carlo simulation.
 * @return The number of paths used for Monte Carlo simulation.
 * 
 * This function returns the number of paths used for Monte Carlo simulation of an option.
 * The number of paths is set by the generate() function.
 */
long long BlackScholesMCPricer::getNbPaths() const {
    return nb_paths_;
}

/**
 * @brief Generate paths for Monte Carlo simulation of an option.
 * @param nb_paths the number of paths to generate.
 * 
 * This function generates paths for Monte Carlo simulation of an option.
 * If the option is an Asian option, then time steps are obtained from the Asian option.
 * Otherwise, the time step is the option's expiry.
 * The function then simulates the option's price at each time step for each path.
 * The simulated prices are used to estimate the option's price and the standard error of the estimate.
 */
void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) {
        return;
    }

    std::vector<double> time_steps;
    if (option_->isAsianOption()) {
        if (auto* asian = dynamic_cast<AsianOption*>(option_)) {
            time_steps = asian->getTimeSteps();
        }
    }
    if (time_steps.empty()) {
        time_steps.push_back(option_->getExpiry());
    }

    const double drift = interest_rate_ - 0.5 * volatility_ * volatility_;

    for (int p = 0; p < nb_paths; ++p) {
        double s = initial_price_;
        double last_t = 0.0;
        std::vector<double> path;
        path.reserve(time_steps.size());

        for (double t : time_steps) {
            if (t < last_t) {
                throw std::invalid_argument("BlackScholesMCPricer: time steps must be non-decreasing");
            }
            const double dt = t - last_t;
            if (dt > 0.0) {
                const double z = MT::rand_norm();
                s *= std::exp(drift * dt + volatility_ * std::sqrt(dt) * z);
            }
            path.push_back(s);
            last_t = t;
        }

        const auto payoffs = option_->payoffPath(path);
        if (payoffs.empty()) {
            continue;
        }

        const double discounted = std::exp(-interest_rate_ * last_t) * payoffs.back();
        ++nb_paths_;
        const double delta = discounted - estimate_;
        estimate_ += delta / static_cast<double>(nb_paths_);
        const double delta2 = discounted - estimate_;
        M2_ += delta * delta2;
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
    if (nb_paths_ == 0) {
        throw std::logic_error("BlackScholesMCPricer: call generate() before requesting price");
    }
    return estimate_;
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
    if (nb_paths_ < 2) {
        throw std::logic_error("BlackScholesMCPricer: need at least two paths for confidence interval");
    }
    const double variance = M2_ / static_cast<double>(nb_paths_ - 1);
    const double std_err = std::sqrt(variance / static_cast<double>(nb_paths_));
    const double z = 1.96; // 95% CI for normal distribution
    return {estimate_ - z * std_err, estimate_ + z * std_err};
}
