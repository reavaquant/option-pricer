#include <stdexcept>
#include <cmath>
#include <cassert>
#include "CRRPricer.hpp"

/**
 * @brief Construct a CRRPricer instance.
 * @details This pricer uses the Carr, Madan and Chang (CRR) algorithm to estimate the price of an option.
 * It is constructed with an option, an initial price, the up factor, the down factor, and the risk-free rate.
 * The option must not be null, and the time steps of the option will be cached for future use.
 * If the option is an AsianOption, then the time steps of the AsianOption will be used.
 * Otherwise, the time steps will be initialized with the expiry time of the option.
 * @param option The option to be priced.
 * @param depth The depth of the binary tree used to estimate the price of the option.
 * @param S0 The initial price of the underlying asset.
 * @param U The up factor (multiplicative factor or return).
 * @param D The down factor (multiplicative factor or return).
 * @param R The risk-free rate (multiplicative factor or return).
 */
CRRPricer::CRRPricer(Option* option, int depth, double S0, double U, double D, double R) : option_(option), depth_(depth), S0_(S0), U_(U), D_(D), R_(R) {
    //arbitrage check
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option pointer must not be null");
    }
    if (option_->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian options are not supported");
    }

    // Accept either multiplicative factors (>0) or returns (typically small, can be negative)
    auto to_factor = [](double x) {
        if (x <= 0.0 || x < 0.5) {
            const double factor = 1.0 + x;
            if (factor <= 0.0) {
                throw std::invalid_argument("CRRPricer: invalid factor after converting return");
            }
            return factor;
        }
        return x;
    };
    U_ = to_factor(U_);
    D_ = to_factor(D_);
    R_ = to_factor(R_);

    if (!(D_ < R_ && R_ < U_)) {
        throw std::invalid_argument("CRRPricer: arbitrage detected (need D < R < U)");
    }
    if (U_ <= D_) throw std::invalid_argument("CRRPricer: up factor must exceed down factor");

    optionTree_.setDepth(depth_);
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

    //set spot price at the depth
    double s = S0_;
    for (int k = 0; k < depth_; ++k) s *= D_;
    const double ratio = U_ / D_;
    for (int i = 0; i <= depth_; ++i) {
        optionTree_.setNode(depth_, i, option_->payoff(s));
        s *= ratio;
    }

    //backward induction
    for(int n = depth_-1; n >= 0; --n) {
        for(int i = 0; i <= n; ++i) {
            const double up = optionTree_.getNode(n + 1, i + 1);
            const double down = optionTree_.getNode(n + 1, i);
            const double cont = (q * up + (1.0 - q) * down) / R;
            optionTree_.setNode(n, i, cont);
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
 * @brief Return the price of the option using either the CRR binomial tree method or closed form.
 * 
 * @details This function returns the price of the option using either the CRR binomial tree method (if closed_form is false) or using the closed form (if closed_form is true).
 * If the CRR binomial tree method is used, it first calls the compute() function to compute the prices at each node of the tree, then returns the price at the root node.
 * If the closed form is used, it computes the price of the option directly without using the binary tree.
 * @param closed_form A boolean indicating whether to use the CRR binomial tree method (false) or the closed form (true).
 * @return The price of the option.
 */
double CRRPricer::operator()(bool closed_form) {
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
