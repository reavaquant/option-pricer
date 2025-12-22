#include <cmath>
#include <stdexcept>
#include "CRRPricer.h"

/**
 * @brief Construct a CRRPricer instance.
 * @details This pricer uses the Cox-Ross-Rubinstein model to estimate the price of an option.
 * It is constructed with an option, the depth of the binomial tree, the initial price of the underlying asset,
 * the risk-free rate, the rate of continuous compounding of the underlying asset, and the rate of continuous dividend yield.
 * @param option The option to be priced.
 * @param depth The depth of the binomial tree.
 * @param S0 The initial price of the underlying asset.
 * @param U The rate of continuous compounding of the underlying asset.
 * @param D The rate of continuous dividend yield.
 * @param R The risk-free rate.
 */
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
 * @brief Construct a CRRPricer instance with parameters.
 * 
 * @details This constructor takes an option, the depth of the binomial tree,
 * the initial price of the underlying asset, the interest rate, and the volatility.
 * It first checks that the option is not null and that the depth is greater than 0.
 * Then it computes the time step of the binomial tree, and uses this to compute U, D, and R.
 * Finally, it checks that D < R < U, and throws an exception if this is not true.
 * 
 * @param option The option to be priced.
 * @param depth The depth of the binomial tree.
 * @param S0 The initial price of the underlying asset.
 * @param r The interest rate of the risk-free asset.
 * @param volatility The volatility of the underlying asset.
 */
CRRPricer::CRRPricer(Option* option, int depth, double S0, double r, double volatility) : option_(option), depth_(depth), S0_(S0) {
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option is null");
    }
    if (option_->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian option not supported");
    }
    if (depth_ <= 0) {
        throw std::invalid_argument("CRRPricer: depth must be > 0");
    }

    double T = option_->getExpiry();
    double dt = T / depth_;
    if (dt <= 0.0) {
        throw std::invalid_argument("CRRPricer: time step must be positive");
    }
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

/**
 * @brief Compute the price of the option using the CRR model.
 * 
 * This function computes the price of the option using the CRR model.
 * It first initializes the binomial tree with the payoff values at each node.
 * Then it iterates over the tree from the last node to the first node, computing the
 * value of each node based on its children. If the option is an American option,
 * it also checks if exercising the option at each node yields a higher value than not
 * exercising it, and updates the value and exercise decision accordingly.
 * Finally, it sets the computed_ flag to true.
 * 
 * @throws std::logic_error if compute() has not been called before.
 */
void CRRPricer::compute() {
    double q = (R_ - D_) / (U_ - D_);
    bool american = option_->isAmericanOption();

    double s = 0.0;
    double payoff = 0.0;
    double up = 0.0;
    double down = 0.0;
    double cont = 0.0;
    double value = 0.0;
    bool ex = false;
    double intrinsic = 0.0;

    for (int i = 0; i <= depth_; ++i) {
        s = S0_ * std::pow(U_, i) * std::pow(D_, depth_ - i);
        payoff = option_->payoff(s);
        optionTree_.setNode(depth_, i, payoff);
        exerciseTree_.setNode(depth_, i, american && payoff > 0.0);
    }

    for (int n = depth_ - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            up = optionTree_.getNode(n + 1, i + 1);
            down = optionTree_.getNode(n + 1, i);
            cont = (q * up + (1.0 - q) * down) / R_;
            value = cont;
            ex = false;

            if (american) {
                s = S0_ * std::pow(U_, i) * std::pow(D_, n - i);
                intrinsic = option_->payoff(s);
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

/**
 * @brief Get the value of the option at node (n, i).
 * 
 * This function returns the value of the option at node (n, i) in the
 * binomial tree. If compute() has not been called before, a
 * std::logic_error is thrown.
 * 
 * @param n the level of the binomial tree
 * @param i the index of the node in the level
 * 
 * @return the value of the option at node (n, i).
 * 
 * @throws std::logic_error if compute() has not been called before.
 */
double CRRPricer::get(int n, int i) {
    if (!computed_) {
        throw std::logic_error("CRRPricer::get needs compute() first");
    }
    return optionTree_.getNode(n, i);
}

/**
 * @brief Get the exercise decision at node (n, i).
 * 
 * @param n the level of the binary tree
 * @param i the index of the node in the level
 * @return true if the option should be exercised at node (n, i), false otherwise.
 * 
 * @throws std::logic_error if compute() has not been called before.
 */
bool CRRPricer::getExercise(int n, int i) {
    if (!computed_) {
        throw std::logic_error("CRRPricer::getExercise needs compute() first");
    }
    return exerciseTree_.getNode(n, i);
}

/**
 * @brief Calculate the binomial coefficient N choose k.
 *
 * @param N The number of items to choose from.
 * @param k The number of items to choose.
 * @return The binomial coefficient N choose k.
 *
 * It also uses symmetry to reduce the number of operations needed.
 */
double CRRPricer::binom_coeff(int N, int k) {
    if (k < 0 || k > N) return 0.0;
    if (k > N - k) k = N - k; // symetry
    double c = 1.0;
    for (int j = 1; j <= k; ++j) {
        c = c * (N - k + j);
        c = c / j;
    }
    return c;
}

/**
 * @brief Calculate the price of the option using the CRR model.
 * 
 * @param closed_form If true, use the closed-form formula for European options.
 * @return The price of the option.
 * 
 * If closed_form is false, the price is calculated using the binomial tree.
 * If closed_form is true, the price is calculated using the closed-form formula.
 * Note that the closed-form formula is only valid for European options.
 */
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
    double s = 0.0;
    double weight = 0.0;
    for (int i = 0; i <= depth_; ++i) {
        s = S0_ * std::pow(U_, i) * std::pow(D_, depth_ - i);
        weight = binom_coeff(depth_, i);
        weight *= std::pow(q, i) * std::pow(1.0 - q, depth_ - i);
        price += weight * option_->payoff(s);
    }
    return price / std::pow(R_, depth_);
}
