#include <cassert>
#include <cmath>

#include "option-pricer/utils/MT.hpp"

int main() {
    const int unif_samples = 1000;
    double unif_sum = 0.0;
    double unif_min = 1.0;
    double unif_max = 0.0;

    for (int i = 0; i < unif_samples; ++i) {
        const double v = MT::rand_unif();
        assert(v >= 0.0 && v <= 1.0);
        unif_sum += v;
        unif_min = std::min(unif_min, v);
        unif_max = std::max(unif_max, v);
    }

    const double unif_mean = unif_sum / unif_samples;
    assert(unif_min < unif_max); // not a degenerate stream
    assert(std::abs(unif_mean - 0.5) < 0.05); // close to U(0,1) mean

    // Basic statistical checks on normal samples
    const int norm_samples = 2000;
    double norm_sum = 0.0;
    double norm_sq_sum = 0.0;

    for (int i = 0; i < norm_samples; ++i) {
        const double z = MT::rand_norm();
        assert(std::isfinite(z));
        norm_sum += z;
        norm_sq_sum += z * z;
    }

    const double norm_mean = norm_sum / norm_samples;
    const double norm_var = norm_sq_sum / norm_samples - norm_mean * norm_mean;
    assert(std::abs(norm_mean) < 0.1); // mean near 0
    assert(std::abs(norm_var - 1.0) < 0.2); // variance near 1

    return 0;
}
