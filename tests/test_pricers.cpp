#include <cassert>
#include <cmath>

#include "option-pricer/options/CallOption.hpp"
#include "option-pricer/options/PutOption.hpp"
#include "option-pricer/pricing/BlackScholesPricer.hpp"

namespace {
constexpr double kEps = 1e-6;
}

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);

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

    // limit case
    CallOption call_expired(0.0, 100.0);
    PutOption put_expired(0.0, 100.0);
    BlackScholesPricer call_expired_pricer(&call_expired, 110.0, rate, vol);
    BlackScholesPricer put_expired_pricer(&put_expired, 90.0, rate, vol);
    assert(std::fabs(call_expired_pricer.price() - 10.0) < kEps);
    assert(std::fabs(call_expired_pricer.delta() - 1.0) < kEps);
    assert(std::fabs(put_expired_pricer.price() - 10.0) < kEps);
    assert(std::fabs(put_expired_pricer.delta() + 1.0) < kEps);

    return 0;
}