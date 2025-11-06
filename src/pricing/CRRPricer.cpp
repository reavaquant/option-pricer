#include <stdexcept>
#include <cmath>
#include <cassert>
#include "CRRPricer.hpp"

namespace{
    
}

CRRPricer::CRRPricer(Option* option, int depth, double S0, double U, double D, double R) : option_(option), depth_(depth), S0_(S0), U_(U), D_(D), R_(R) {
    //arbitrage check
    if (!option_) {
        throw std::invalid_argument("CRRPricer: option pointer must not be null");
    }
    if (!(D < R && R < U)) {
        throw std::invalid_argument("CRRPricer: arbitrage detected (need D < R < U)");
    }
    if (U <= D) throw std::invalid_argument("CRRPricer: up factor must exceed down factor");

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

double CRRPricer::get(int n, int i) {
    assert(computed_ && "CRR_EuroPricer::get requires compute() first.");
    return optionTree_.getNode(n, i);
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

long double CRRPricer::binom_coeff(int N, int k) {
    if (k < 0 || k > N) return 0.0;
    int m = (k < (N - k)) ? k : (N - k);
    double c = 1.0;
    for (int j = 1; j <= m; ++j) {
        c = c * (N - m + j) / static_cast<double>(j);
    }
    return c;
}

double CRRPricer::operator()(bool closed_form = false) {
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