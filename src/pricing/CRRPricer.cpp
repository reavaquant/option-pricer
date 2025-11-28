#include <stdexcept>
#include <cmath>
#include <cassert>
#include "CRRPricer.hpp"

/**
 * @brief Constructor for CRRPricer.
 * @details Construct a CRRPricer object for the given option and parameters.
 * @param option the option to price
 * @param depth the depth of the binomial tree
 * @param S0 the initial spot price
 * @param U the up factor (either a return or a multiplicative factor > 0)
 * @param D the down factor (either a return or a multiplicative factor > 0)
 * @param R the risk-free rate (either a return or a multiplicative factor > 0)
 * @throw std::invalid_argument if option pointer is null, or if the option is an Asian option, or if the factors do not satisfy 0 < D < R < U
 */
CRRPricer::CRRPricer(Option* option, int depth, double S0, double U, double D, double R) : option_(option), depth_(depth), S0_(S0), U_(U), D_(D), R_(R) {
    //arbitrage check
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option pointer must not be null");
    }
    if (option_->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian options are not supported");
    }

    // Accept either multiplicative factors (>0) or returns (all three provided as returns)
    auto as_factor = [](double x) {
        const double factor = 1.0 + x;
        if (factor <= 0.0) {
            throw std::invalid_argument("CRRPricer: invalid factor after converting return");
        }
        return factor;
    };

    const bool looks_like_return = (U_ < 1.0 && D_ < 1.0 && R_ < 1.0);
    if (looks_like_return) {
        U_ = as_factor(U_);
        D_ = as_factor(D_);
        R_ = as_factor(R_);
    } else {
        if (U_ <= 0.0 || D_ <= 0.0 || R_ <= 0.0) {
            throw std::invalid_argument("CRRPricer: multiplicative factors must be positive");
        }
    }

    if (!(D_ < R_ && R_ < U_)) {
        throw std::invalid_argument("CRRPricer: arbitrage detected (need D < R < U)");
    }
    if (U_ <= D_) throw std::invalid_argument("CRRPricer: up factor must exceed down factor");

    optionTree_.setDepth(depth_);
    exerciseTree_.setDepth(depth_);
}

/**
 * @brief Construct a CRRPricer instance using the risk-free rate and volatility.
 * @details This constructor uses the risk-free rate and volatility to compute the up and down factors for the binomial tree.
 * It first checks that the option pointer is not null, and that the depth is positive.
 * It then computes the up and down factors using the risk-free rate and volatility.
 * Finally, it checks for arbitrage and sets the depth of the binomial trees.
 * @throw std::invalid_argument if the option pointer is null, or if the depth is not positive, or if the factors do not satisfy 0 < D < R < U.
 * @param option the option to be priced
 * @param depth the depth of the binomial tree
 * @param S0 the initial spot price
 * @param r the risk-free rate (either a return or a multiplicative factor > 0)
 * @param volatility the volatility of the underlying asset (either a return or a multiplicative factor > 0)
 */
CRRPricer::CRRPricer(Option* option, int depth, double S0, double r, double volatility)
    : option_(option), depth_(depth), S0_(S0) {
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option pointer must not be null");
    }
    if (depth_ <= 0) {
        throw std::invalid_argument("CRRPricer: depth must be positive when using r/sigma constructor");
    }

    const double expiry = option_->getExpiry();
    const double dt = expiry / static_cast<double>(depth_);
    const double sigma_step = volatility * std::sqrt(dt);
    U_ = std::exp(sigma_step);
    D_ = std::exp(-sigma_step);
    R_ = std::exp(r * dt);

    if (!(D_ < R_ && R_ < U_)) {
        throw std::invalid_argument("CRRPricer: arbitrage detected (need D < R < U)");
    }
    if (U_ <= D_) throw std::invalid_argument("CRRPricer: up factor must exceed down factor");

    optionTree_.setDepth(depth_);
    exerciseTree_.setDepth(depth_);
}

/**
 * @brief Compute the price of an option using the CRR binomial tree method.
 * 
 * @details This function uses the CRR binomial tree method to compute the price of an option.
 * It first sets the spot price at the depth of the tree, then uses backward induction to compute the prices at each node of the tree.
 */
void CRRPricer::compute() {
    const double R = R_;
    double q = (R - D_) / (U_ - D_);
    const bool isAmerican = option_->isAmericanOption();

    //set spot price at the depth
    double s = S0_;
    for (int k = 0; k < depth_; ++k) s *= D_;
    const double ratio = U_ / D_;
    for (int i = 0; i <= depth_; ++i) {
        const double payoff = option_->payoff(s);
        optionTree_.setNode(depth_, i, payoff);
        exerciseTree_.setNode(depth_, i, isAmerican && payoff > 0.0);
        s *= ratio;
    }

    //backward induction
    for(int n = depth_-1; n >= 0; --n) {
        for(int i = 0; i <= n; ++i) {
            const double up = optionTree_.getNode(n + 1, i + 1);
            const double down = optionTree_.getNode(n + 1, i);
            const double cont = (q * up + (1.0 - q) * down) / R;
            double nodeValue = cont;
            bool exercise = false;
            if (isAmerican) {
                const double spot = S0_ * std::pow(U_, i) * std::pow(D_, n - i);
                const double intrinsic = option_->payoff(spot);
                if (intrinsic >= cont) {
                    nodeValue = intrinsic;
                    exercise = true;
                }
            }
            optionTree_.setNode(n, i, nodeValue);
            exerciseTree_.setNode(n, i, exercise);
        }
    }
    computed_ = true;
}

/**
 * @brief Get the price of the option at a specific node in the binary tree.
 * @details This function returns the price of the option at the node (n, i) in the binary tree.
 * It requires that the compute() function has been called first.
 * @param n The level of the binary tree.
 * @param i The index of the node at level n.
 * @return The price of the option at the node (n, i).
 */
double CRRPricer::get(int n, int i) {
    assert(computed_ && "CRR_EuroPricer::get requires compute() first.");
    return optionTree_.getNode(n, i);
}

bool CRRPricer::getExercise(int n, int i) {
    assert(computed_ && "CRRPricer::getExercise requires compute() first.");
    return exerciseTree_.getNode(n, i);
}

/**
 * @brief Calculate the binomial coefficient N choose k.
 * @details This function calculates the binomial coefficient N choose k, i.e. the number of combinations of k items chosen from a set of N items.
 * @param N The total number of items.
 * @param k The number of items to choose.
 * @return The binomial coefficient N choose k.
 */
long double CRRPricer::binom_coeff(int N, int k) {
  if (k < 0 || k > N) return 0.0L;
  int m = std::min(k, N - k);
  long double c = 1.0L;
  for (int j = 1; j <= m; ++j) {
    c = c * static_cast<long double>(N - m + j) / static_cast<long double>(j);
  }
  return c;
}

/**
 * @brief Calculate the price of the option using the binomial model.
 * @details This function calculates the price of the option using the binomial model.
 * If closed_form is true, it will use the closed form formula to calculate the price.
 * If closed_form is false, it will use the binomial model to calculate the price.
 * Note that the closed form formula is only supported for European options.
 * @param closed_form Whether to use the closed form formula.
 * @return The price of the option.
 */
double CRRPricer::operator()(bool closed_form) {
    if (option_->isAmericanOption() && closed_form) {
        throw std::logic_error("CRRPricer: closed form formula only supported for European options");
    }

    if (!closed_form) {
        if (!computed_) compute();
        return optionTree_.getNode(0, 0);
    }

    // closed form price
    const double R = R_;
    const double q = (R - D_) / (U_ - D_);
    double s = S0_;
    for (int k = 0; k < depth_; ++k) s *= D_;
    const double ratio = U_ / D_;
    double sum = 0.0;
    for (int i = 0; i <= depth_; ++i) {
        const double comb = binom_coeff(depth_, i);
        const double prob = std::pow(q, i) * std::pow(1.0 - q, depth_ - i);
        sum += comb * prob * option_->payoff(s);
        s *= ratio; 
    }
    return sum / std::pow(R, depth_);
}
