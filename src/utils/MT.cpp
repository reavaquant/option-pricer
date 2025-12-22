#include "MT.h"

std::mt19937& MT::generator() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

/**
 * @brief Returns a random floating-point number in the range [0.0, 1.0).
 * 
 * @return A random floating-point number.
 */
double MT::rand_unif() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(generator());
}

/**
 * @brief Returns a random floating-point number from a normal distribution with mean 0.0 and standard deviation 1.0.
 * 
 * @return A random floating-point number from a normal distribution.
 */
double MT::rand_norm() {
    static std::normal_distribution<double> dist(0.0, 1.0);
    return dist(generator());
}
