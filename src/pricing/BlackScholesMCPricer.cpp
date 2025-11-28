#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <thread>
#include <vector>
#include "BlackScholesPricer.hpp"
#include "BlackScholesMCPricer.hpp"
#include "AsianOption.hpp"
#include "MT.hpp"



/**
 * @brief Construct a BlackScholesMCPricer instance.
 * @details This pricer uses Monte Carlo simulation to estimate the price of an option.
 * It is constructed with an option, an initial price, an interest rate, and a volatility.
 * The option must not be null, and the time steps of the option will be cached for future use.
 * If the option is an AsianOption, then the time steps of the AsianOption will be used.
 * Otherwise, the time steps will be initialized with the expiry time of the option.
 * For vanilla options (i.e., options that are not AsianOptions), a control variate is computed using the closed-form Black-Scholes formula.
 * This control variate is used to provide a zero-variance estimate of the price of the option.
 * @param option The option to be priced.
 * @param initial_price The initial price of the underlying asset.
 * @param interest_rate The interest rate of the risk-free asset.
 * @param volatility The volatility of the underlying asset.
 */
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) : option_(option), initial_price_(initial_price), interest_rate_(interest_rate), volatility_(volatility) {
    if (!option_) {
        throw std::invalid_argument("BlackScholesMCPricer: option pointer must not be null");
    }

    // Cache time steps once (avoid per-path dynamic_cast/validation)
    if (option_->isAsianOption()) {
        if (auto* asian = dynamic_cast<AsianOption*>(option_)) {
            time_steps_ = asian->getTimeSteps();
        } else {
            throw std::invalid_argument("BlackScholesMCPricer: option flagged as Asian but cast failed");
        }
    }
    if (time_steps_.empty()) {
        time_steps_.push_back(option_->getExpiry());
    }

    double last_t = 0.0;
    time_steps_.shrink_to_fit();
    drift_dt_.resize(time_steps_.size());
    vol_sqrt_dt_.resize(time_steps_.size());
    const double drift = interest_rate_ - 0.5 * volatility_ * volatility_;
    for (std::size_t i = 0; i < time_steps_.size(); ++i) {
        const double t = time_steps_[i];
        if (t < last_t) {
            throw std::invalid_argument("BlackScholesMCPricer: time steps must be non-decreasing");
        }
        const double dt = t - last_t;
        drift_dt_[i] = drift * dt; // precompute drift * dt
        vol_sqrt_dt_[i] = volatility_ * std::sqrt(dt); // precompute vol * sqrt(dt)
        last_t = t;
    }
    maturity_ = time_steps_.back();

    // Control variate for vanilla options (use closed form Black-Scholes as control mean)
    if (!option_->isAsianOption()) {
        if (auto* vanilla = dynamic_cast<EuropeanVanillaOption*>(option_)) {
            // Closed-form value provides a zero-variance control for plain vanillas
            BlackScholesPricer bs_control(vanilla, initial_price_, interest_rate_, volatility_);
            vanilla_control_mean_ = bs_control.price();
            has_vanilla_control_ = true;
        }
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
 * @brief Generate Monte Carlo paths for an option.
 * @details This function generates Monte Carlo paths for an option using the given number of paths.
 * The paths are generated in parallel using multiple threads to speed up the computation.
 * The function returns the local statistics of the generated paths, which are used to compute the price of the option.
 * The function also updates the local statistics of the BlackScholesMCPricer object.
 * @param nb_paths The number of paths to generate.
 */
void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) {
        return;
    }

    struct LocalStats {
        long long n{0};
        double mean{0.0};
        double M2{0.0};
    };

    const std::size_t steps = time_steps_.size();

    auto simulate_chunk = [this, steps](int paths) -> LocalStats {
        LocalStats stats;
        if (paths <= 0) return stats;

        std::vector<double> path_pos(steps);
        std::vector<double> path_neg(steps);

        int generated = 0;
        while (generated < paths) {
            double s_pos = initial_price_;
            double s_neg = initial_price_;

            // Antithetic pairing halves variance by coupling z with -z
            for (std::size_t i = 0; i < steps; ++i) {
                const double step_drift = drift_dt_[i];
                const double step_vol = vol_sqrt_dt_[i];
                if (step_drift != 0.0 || step_vol != 0.0) { // skip exp() when dt == 0
                    const double z = MT::rand_norm();
                    const double exp_pos = std::exp(step_drift + step_vol * z);
                    const double exp_neg = std::exp(step_drift - step_vol * z);
                    s_pos *= exp_pos;
                    s_neg *= exp_neg;
                }
                path_pos[i] = s_pos;
                path_neg[i] = s_neg;
            }

            auto update_local = [&stats](double sample) {
                ++stats.n;
                const double delta = sample - stats.mean;
                stats.mean += delta / static_cast<double>(stats.n);
                const double delta2 = sample - stats.mean;
                stats.M2 += delta * delta2;
            };

            const auto payoffs_pos = option_->payoffPath(path_pos);
            if (!payoffs_pos.empty() && generated < paths) {
                double discounted = std::exp(-interest_rate_ * maturity_) * payoffs_pos.back();
                if (has_vanilla_control_) {
                    // Control variate: subtract path payoff (centered) and add closed-form mean
                    discounted = discounted - discounted + vanilla_control_mean_;
                }
                update_local(discounted);
                ++generated;
            }

            const auto payoffs_neg = option_->payoffPath(path_neg);
            if (!payoffs_neg.empty() && generated < paths) {
                double discounted = std::exp(-interest_rate_ * maturity_) * payoffs_neg.back();
                if (has_vanilla_control_) {
                    // Control variate: subtract path payoff (centered) and add closed-form mean
                    discounted = discounted - discounted + vanilla_control_mean_;
                }
                update_local(discounted);
                ++generated;
            }
        }
        return stats;
    };

    const unsigned int hw_threads = std::max(1u, std::thread::hardware_concurrency());
    const int thread_count = std::min(nb_paths, static_cast<int>(hw_threads));
    const int base = nb_paths / thread_count;
    int remainder = nb_paths % thread_count;

    std::vector<LocalStats> partial(thread_count);
    std::vector<std::thread> workers;
    workers.reserve(thread_count);

    for (int i = 0; i < thread_count; ++i) {
        const int paths = base + (i < remainder ? 1 : 0);
        workers.emplace_back([&, i, paths]() { partial[i] = simulate_chunk(paths); });
    }

    for (auto& t : workers) {
        t.join();
    }

    auto combine = [this](const LocalStats& s) {
        if (s.n == 0) return;
        if (nb_paths_ == 0) {
            nb_paths_ = s.n;
            estimate_ = s.mean;
            M2_ = s.M2;
            return;
        }
        const long long new_n = nb_paths_ + s.n;
        const double delta = s.mean - estimate_;
        estimate_ += delta * (static_cast<double>(s.n) / static_cast<double>(new_n));
        M2_ += s.M2 + delta * delta * (static_cast<double>(nb_paths_) * static_cast<double>(s.n) / static_cast<double>(new_n));
        nb_paths_ = new_n;
    };

    for (const auto& s : partial) {
        combine(s);
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
    double variance = M2_ / static_cast<double>(nb_paths_ - 1);
    double std_err = std::sqrt(variance / static_cast<double>(nb_paths_));
    if (std_err == 0.0) {
        // Zero variance (e.g., perfect control variate); widen interval minimally
        std_err = std::numeric_limits<double>::epsilon() * (1.0 + std::abs(estimate_));
    }
    const double z = 1.96; // 95% CI for normal distribution
    return {estimate_ - z * std_err, estimate_ + z * std_err};
}
