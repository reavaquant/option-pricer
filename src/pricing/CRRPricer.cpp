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
CRRPricer::CRRPricer(Option* option, int depth, double S0, double U, double D, double R) : _option(option), _depth(depth), _S0(S0) {
    if (!_option) {
        throw std::invalid_argument("CRRPricer: option is null");
    }
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian option not supported");
    }
    if (_depth < 0) {
        throw std::invalid_argument("CRRPricer: depth must be >= 0");
    }

    // inputs are returns not rates so add 1
    _U = 1.0 + U;
    _D = 1.0 + D;
    _R = 1.0 + R;

    if (_U <= 0.0 || _D <= 0.0 || _R <= 0.0) {
        throw std::invalid_argument("CRRPricer: returns must be > -100%");
    }
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("CRRPricer: need D < R < U"); //arbitrage checks
    }

    _optionTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
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
CRRPricer::CRRPricer(Option* option, int depth, double S0, double r, double volatility) : _option(option), _depth(depth), _S0(S0) {
    if (!_option) {
        throw std::invalid_argument("CRRPricer: option is null");
    }
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer: Asian option not supported");
    }
    if (_depth <= 0) {
        throw std::invalid_argument("CRRPricer: depth must be > 0");
    }

    double T = _option->getExpiry();
    double dt = T / _depth;
    if (dt <= 0.0) {
        throw std::invalid_argument("CRRPricer: time step must be positive");
    }
    const double drift = (r + 0.5 * volatility * volatility) * dt;
    const double step = volatility * std::sqrt(dt);
    _U = std::exp(drift + step);
    _D = std::exp(drift - step);
    _R = std::exp(r * dt);

    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("CRRPricer: need D < R < U");
    }

    _optionTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
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
 * Finally, it sets the _computed flag to true.
 * 
 * @throws std::logic_error if compute() has not been called before.
 */
void CRRPricer::compute() {
    double q = (_R - _D) / (_U - _D);
    bool american = _option->isAmericanOption();

    double s = 0.0;
    double payoff = 0.0;
    bool exercise_now = false;
    double up = 0.0;
    double down = 0.0;
    double cont = 0.0;
    double value = 0.0;
    bool ex = false;
    double intrinsic = 0.0;

    for (int i = 0; i <= _depth; ++i) {
        s = _S0 * std::pow(_U, i) * std::pow(_D, _depth - i);
        payoff = _option->payoff(s);
        _optionTree.setNode(_depth, i, payoff);
        exercise_now = american && payoff >= 0.0;
        _exerciseTree.setNode(_depth, i, exercise_now);
    }

    for (int n = _depth - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            up = _optionTree.getNode(n + 1, i + 1);
            down = _optionTree.getNode(n + 1, i);
            cont = (q * up + (1.0 - q) * down) / _R;
            value = cont;
            ex = false;

            if (american) {
                s = _S0 * std::pow(_U, i) * std::pow(_D, n - i);
                intrinsic = _option->payoff(s);
                if (intrinsic >= cont) {
                    value = intrinsic;
                    ex = true;
                }
            }

            _optionTree.setNode(n, i, value);
            _exerciseTree.setNode(n, i, ex);
        }
    }
    _computed = true;
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
    if (!_computed) {
        throw std::logic_error("CRRPricer::get needs compute() first");
    }
    return _optionTree.getNode(n, i);
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
    if (!_computed) {
        throw std::logic_error("CRRPricer::getExercise needs compute() first");
    }
    return _exerciseTree.getNode(n, i);
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
    if (_option->isAmericanOption() && closed_form) {
        throw std::logic_error("CRRPricer: closed form only for European options");
    }

    if (!closed_form) {
        if (!_computed) compute();
        return _optionTree.getNode(0, 0);
    }

    double q = (_R - _D) / (_U - _D);
    double price = 0.0;
    double s = 0.0;
    double weight = 0.0;
    for (int i = 0; i <= _depth; ++i) {
        s = _S0 * std::pow(_U, i) * std::pow(_D, _depth - i);
        weight = binom_coeff(_depth, i);
        weight *= std::pow(q, i) * std::pow(1.0 - q, _depth - i);
        price += weight * _option->payoff(s);
    }
    return price / std::pow(_R, _depth);
}
