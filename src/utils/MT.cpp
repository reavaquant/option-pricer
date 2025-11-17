#include "option-pricer/utils/MT.hpp"

std::mt19937& MT::generator() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

/**
 * @brief Returns a random floating-point number in the range [0.0, 1.0).
 * thread_local makes each thread keep its own instance of the distribution object, so two threads don’t contend over the same std::uniform_real_distribution
 * 
 * @return A random floating-point number.
 */
double MT::rand_unif() {
    static thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(generator());
}

/**
 * @brief Returns a random floating-point number from a normal distribution with mean 0.0 and standard deviation 1.0.
 * thread_local makes each thread keep its own instance of the distribution object, so two threads don’t contend over the same std::normal_distribution state
 * 
 * @return A random floating-point number from a normal distribution.
 */
double MT::rand_norm() {
    static thread_local std::normal_distribution<double> dist(0.0, 1.0);
    return dist(generator());
}
