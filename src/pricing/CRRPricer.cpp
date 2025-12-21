#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "CRRPricer.h"

CRRPricer::CRRPricer(Option* option, int depth, double S0, double U, double D, double R) : option_(option), depth_(depth), S0_(S0) {
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option is null");
    }
    if (option_->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian option not supported");
    }
    if (depth_ < 0) {
        throw std::invalid_argument("CRRPricer: depth must be >= 0");
    }

    // inputs are returns not rates so add 1
    U_ = 1.0 + U;
    D_ = 1.0 + D;
    R_ = 1.0 + R;

    if (U_ <= 0.0 || D_ <= 0.0 || R_ <= 0.0) {
        throw std::invalid_argument("CRRPricer: returns must be > -100%");
    }
    if (!(D_ < R_ && R_ < U_)) {
        throw std::invalid_argument("CRRPricer: need D < R < U"); //arbitrage checks
    }

    optionTree_.setDepth(depth_);
    exerciseTree_.setDepth(depth_);
}

/**
 * @brief Construct a CRRPricer instance.
 * @details This pricer uses the CRR binomial model to estimate the price of an option.
 * It is constructed with an option, a depth, an initial price, an interest rate, and a volatility.
 * The option must not be null, and the depth must be > 0.
 * The interest rate and volatility must be such that the returns D, R, and U satisfy D < R < U.
 * @param option The option to be priced.
 * @param depth The depth of the binomial tree.
 * @param S0 The initial price of the underlying asset.
 * @param r The interest rate of the risk-free asset.
 * @param volatility The volatility of the underlying asset.
 */
CRRPricer::CRRPricer(Option* option, int depth, double S0, double r, double volatility)
    : option_(option), depth_(depth), S0_(S0) {
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option is null");
    }
    if (depth_ <= 0) {
        throw std::invalid_argument("CRRPricer: depth must be > 0");
    }

    double T = option_->getExpiry();
    double dt = T / depth_;
    double step = volatility * std::sqrt(dt);
    U_ = std::exp(step);
    D_ = std::exp(-step);
    R_ = std::exp(r * dt);

    if (!(D_ < R_ && R_ < U_)) {
        throw std::invalid_argument("CRRPricer: need D < R < U");
    }

    optionTree_.setDepth(depth_);
    exerciseTree_.setDepth(depth_);
}

void CRRPricer::compute() {
    double q = (R_ - D_) / (U_ - D_);
    bool american = option_->isAmericanOption();

    for (int i = 0; i <= depth_; ++i) {
        double s = S0_ * std::pow(U_, i) * std::pow(D_, depth_ - i);
        double payoff = option_->payoff(s);
        optionTree_.setNode(depth_, i, payoff);
        exerciseTree_.setNode(depth_, i, american && payoff > 0.0);
    }

    for (int n = depth_ - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double up = optionTree_.getNode(n + 1, i + 1);
            double down = optionTree_.getNode(n + 1, i);
            double cont = (q * up + (1.0 - q) * down) / R_;
            double value = cont;
            bool ex = false;

            if (american) {
                double s = S0_ * std::pow(U_, i) * std::pow(D_, n - i);
                double intrinsic = option_->payoff(s);
                if (intrinsic > cont) {
                    value = intrinsic;
                    ex = true;
                }
            }

            optionTree_.setNode(n, i, value);
            exerciseTree_.setNode(n, i, ex);
        }
    }
    computed_ = true;
}

double CRRPricer::get(int n, int i) {
    if (!computed_) {
        throw std::logic_error("CRRPricer::get needs compute() first");
    }
    return optionTree_.getNode(n, i);
}

bool CRRPricer::getExercise(int n, int i) {
    if (!computed_) {
        throw std::logic_error("CRRPricer::getExercise needs compute() first");
    }
    return exerciseTree_.getNode(n, i);
}

long double CRRPricer::binom_coeff(int N, int k) {
    if (k < 0 || k > N) return 0.0L;
    int m = std::min(k, N - k);
    long double c = 1.0L;
    for (int j = 1; j <= m; ++j) {
        c = c * static_cast<long double>(N - m + j) / static_cast<long double>(j);
    }
    return c;
}

double CRRPricer::operator()(bool closed_form) {
    if (option_->isAmericanOption() && closed_form) {
        throw std::logic_error("CRRPricer: closed form only for European options");
    }

    if (!closed_form) {
        if (!computed_) compute();
        return optionTree_.getNode(0, 0);
    }

    double q = (R_ - D_) / (U_ - D_);
    double price = 0.0;
    for (int i = 0; i <= depth_; ++i) {
        double s = S0_ * std::pow(U_, i) * std::pow(D_, depth_ - i);
        double weight = binom_coeff(depth_, i);
        weight *= std::pow(q, i) * std::pow(1.0 - q, depth_ - i);
        price += weight * option_->payoff(s);
    }
    return price / std::pow(R_, depth_);
}
