#include <cassert>
#include <cmath>
#include <stdexcept>

#include "option-pricer/options/AmericanCallOption.h"
#include "option-pricer/options/AmericanPutOption.h"
#include "option-pricer/options/CallOption.h"
#include "option-pricer/options/EuropeanDigitalCallOption.h"
#include "option-pricer/options/EuropeanDigitalPutOption.h"
#include "option-pricer/options/PutOption.h"
#include "option-pricer/pricing/BlackScholesPricer.h"
#include "option-pricer/pricing/BlackScholesMCPricer.h"
#include "option-pricer/pricing/CRRPricer.h"

namespace {
constexpr double kEps = 1e-6;
}

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);
    EuropeanDigitalCallOption digital_call(1.0, 100.0);
    EuropeanDigitalPutOption digital_put(1.0, 100.0);

    // ----BlackScholesPricer tests----
    const double spot = 100.0;
    const double rate = 0.05;
    const double vol = 0.2;

    // Prix théoriques BS (S=K=100, r=5%, sigma=20%, T=1)
    BlackScholesPricer call_pricer(&call, spot, rate, vol);
    BlackScholesPricer put_pricer(&put, spot, rate, vol);

    const double expected_call = 10.450583572185565;
    const double expected_put = 5.573526022256971;

    assert(std::fabs(call_pricer.price() - expected_call) < kEps);
    assert(std::fabs(put_pricer.price() - expected_put) < kEps);

    assert(std::fabs(call_pricer() - expected_call) < kEps);

    const double expected_call_delta = 0.636830651175619;
    const double expected_put_delta = -0.363169348824381;
    assert(std::fabs(call_pricer.delta() - expected_call_delta) < kEps);
    assert(std::fabs(put_pricer.delta() - expected_put_delta) < kEps);

    BlackScholesPricer digital_call_pricer(&digital_call, spot, rate, vol);
    BlackScholesPricer digital_put_pricer(&digital_put, spot, rate, vol);
    const double expected_digital_call = 0.5323248154537634;
    const double expected_digital_put = 0.41890460904695065;
    assert(std::fabs(digital_call_pricer.price() - expected_digital_call) < kEps);
    assert(std::fabs(digital_put_pricer.price() - expected_digital_put) < kEps);

    const double expected_digital_delta = 0.018762017345846895;
    assert(std::fabs(digital_call_pricer.delta() - expected_digital_delta) < kEps);
    assert(std::fabs(digital_put_pricer.delta() + expected_digital_delta) < kEps);

    // ----BlackScholesMCPricer tests----
    constexpr int mc_paths = 200000;
    BlackScholesMCPricer mc_call_pricer(&call, spot, rate, vol);
    mc_call_pricer.generate(mc_paths);
    const double mc_call_price = mc_call_pricer();
    assert(std::fabs(mc_call_price - expected_call) < 0.75); // Monte Carlo tolerance
    assert(mc_call_pricer.getNbPaths() == mc_paths);

    BlackScholesMCPricer mc_put_pricer(&put, spot, rate, vol);
    mc_put_pricer.generate(mc_paths);
    const double mc_put_price = mc_put_pricer.price();
    assert(std::fabs(mc_put_price - expected_put) < 0.75); // Monte Carlo tolerance
    assert(mc_put_pricer.getNbPaths() == mc_paths);

    bool mc_price_without_paths_thrown = false;
    try {
        BlackScholesMCPricer mc_call_no_paths(&call, spot, rate, vol);
        (void)mc_call_no_paths.price();
    } catch (const std::logic_error&) {
        mc_price_without_paths_thrown = true;
    }
    assert(mc_price_without_paths_thrown);

    bool mc_ci_without_paths_thrown = false;
    try {
        BlackScholesMCPricer mc_ci_no_paths(&call, spot, rate, vol);
        (void)mc_ci_no_paths.confidenceInterval();
    } catch (const std::logic_error&) {
        mc_ci_without_paths_thrown = true;
    }
    assert(mc_ci_without_paths_thrown);

    BlackScholesMCPricer mc_ci_pricer(&call, spot, rate, vol);
    mc_ci_pricer.generate(mc_paths);
    const auto ci = mc_ci_pricer.confidenceInterval();
    assert(ci.size() == 2);
    assert(ci[0] < ci[1]);
    assert(ci[0] <= mc_ci_pricer.price() && mc_ci_pricer.price() <= ci[1]);

    // limit case
    CallOption call_expired(0.0, 100.0);
    PutOption put_expired(0.0, 100.0);
    BlackScholesPricer call_expired_pricer(&call_expired, 110.0, rate, vol);
    BlackScholesPricer put_expired_pricer(&put_expired, 90.0, rate, vol);
    assert(std::fabs(call_expired_pricer.price() - 10.0) < kEps);
    assert(std::fabs(call_expired_pricer.delta() - 1.0) < kEps);
    assert(std::fabs(put_expired_pricer.price() - 10.0) < kEps);
    assert(std::fabs(put_expired_pricer.delta() + 1.0) < kEps);

    EuropeanDigitalCallOption digital_call_expired(0.0, 100.0);
    EuropeanDigitalPutOption digital_put_expired(0.0, 100.0);
    BlackScholesPricer digital_call_expired_pricer(&digital_call_expired, 110.0, rate, vol);
    BlackScholesPricer digital_put_expired_pricer(&digital_put_expired, 90.0, rate, vol);
    assert(std::fabs(digital_call_expired_pricer.price() - 1.0) < kEps);
    assert(std::fabs(digital_put_expired_pricer.price() - 1.0) < kEps);
    assert(std::fabs(digital_call_expired_pricer.delta()) < kEps);
    assert(std::fabs(digital_put_expired_pricer.delta()) < kEps);

    // ----CRRPricer tests----
    constexpr double crrS0 = 100.0;
    constexpr double crrU = 1.2;
    constexpr double crrD = 0.8;
    constexpr double crrR = 1.05;
    constexpr int crrDepth = 3;

    CallOption crr_call(1.0, 100.0);
    CRRPricer crr_call_pricer(&crr_call, crrDepth, crrS0, crrU, crrD, crrR);
    const double expected_crr_call = 21.123528776590003;
    assert(std::fabs(crr_call_pricer(false) - expected_crr_call) < kEps);
    assert(std::fabs(crr_call_pricer(true) - expected_crr_call) < kEps);
    crr_call_pricer.compute();
    assert(std::fabs(crr_call_pricer.get(0, 0) - expected_crr_call) < kEps);
    assert(!crr_call_pricer.getExercise(0, 0));

    PutOption crr_put(1.0, 100.0);
    CRRPricer crr_put_pricer(&crr_put, crrDepth, crrS0, crrU, crrD, crrR);
    const double expected_crr_put = 7.507288629737602;
    assert(std::fabs(crr_put_pricer(false) - expected_crr_put) < kEps);
    assert(std::fabs(crr_put_pricer(true) - expected_crr_put) < kEps);

    // CRRPricer constructor using (r, sigma)
    const double ctor_rate = 0.05;
    const double ctor_sigma = 0.2;
    CallOption crr_call_params(1.0, 100.0);
    const double dt = crr_call_params.getExpiry() / static_cast<double>(crrDepth);
    const double derivedU = std::exp(ctor_sigma * std::sqrt(dt));
    const double derivedD = std::exp(-ctor_sigma * std::sqrt(dt));
    const double derivedR = std::exp(ctor_rate * dt);
    CRRPricer crr_from_params(&crr_call_params, crrDepth, crrS0, derivedU, derivedD, derivedR);
    CRRPricer crr_from_rate(&crr_call_params, crrDepth, crrS0, ctor_rate, ctor_sigma);
    assert(std::fabs(crr_from_params(false) - crr_from_rate(false)) < kEps);

    bool null_option_thrown = false;
    try {
        CRRPricer invalid_pricer(nullptr, crrDepth, crrS0, crrU, crrD, crrR);
    } catch (const std::invalid_argument&) {
        null_option_thrown = true;
    }
    assert(null_option_thrown);

    bool arbitrage_params_thrown = false;
    try {
        CallOption dummy(1.0, 100.0);
        CRRPricer invalid_params(&dummy, crrDepth, crrS0, 0.9, 0.95, 0.9);
        (void)invalid_params;
    } catch (const std::invalid_argument&) {
        arbitrage_params_thrown = true;
    }
    assert(arbitrage_params_thrown);

    bool negative_depth_thrown = false;
    try {
        CallOption dummy(1.0, 100.0);
        CRRPricer invalid_depth(&dummy, -1, crrS0, crrU, crrD, crrR);
        (void)invalid_depth;
    } catch (const std::invalid_argument&) {
        negative_depth_thrown = true;
    }
    assert(negative_depth_thrown);

    bool zero_depth_rate_ctor = false;
    try {
        CallOption dummy(1.0, 100.0);
        CRRPricer invalid_rate_depth(&dummy, 0, crrS0, ctor_rate, ctor_sigma);
        (void)invalid_rate_depth;
    } catch (const std::invalid_argument&) {
        zero_depth_rate_ctor = true;
    }
    assert(zero_depth_rate_ctor);

    // American options through CRR
    AmericanCallOption american_call(1.0, 100.0);
    CRRPricer american_call_pricer(&american_call, crrDepth, crrS0, crrU, crrD, crrR);
    american_call_pricer.compute();
    assert(!american_call_pricer.getExercise(0, 0));

    constexpr double americanPutS0 = 90.0;
    AmericanPutOption american_put(1.0, 100.0);
    CRRPricer american_put_pricer(&american_put, crrDepth, americanPutS0, crrU, crrD, crrR);
    american_put_pricer.compute();
    // With these parameters the continuation value dominates the intrinsic,
    // so the optimal policy is to hold.
    const double expected_american_put = 12.891156462585029;
    assert(std::fabs(american_put_pricer.get(0, 0) - expected_american_put) < kEps);
    assert(!american_put_pricer.getExercise(0, 0));

    return 0;
}
