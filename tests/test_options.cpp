#include <cassert>
#include <cmath>

#include "option-pricer/options/CallOption.hpp"
#include "option-pricer/options/EuropeanDigitalCallOption.hpp"
#include "option-pricer/options/EuropeanDigitalPutOption.hpp"
#include "option-pricer/options/PutOption.hpp"
#include "option-pricer/options/AmericanCallOption.hpp"
#include "option-pricer/options/AmericanPutOption.hpp"


int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);
    EuropeanDigitalCallOption digital_call(1.0, 100.0);
    EuropeanDigitalPutOption digital_put(1.0, 100.0);
    AmericanCallOption american_call(1.0, 100.0);
    AmericanPutOption american_put(1.0, 100.0);

    assert(call.getOptionType() == OptionType::Call);
    assert(put.getOptionType() == OptionType::Put);
    assert(digital_call.getOptionType() == OptionType::Call);
    assert(digital_put.getOptionType() == OptionType::Put);

    assert(std::abs(call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(put.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(digital_call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(digital_put.getExpiry() - 1.0) < 1e-9);

    assert(std::abs(call.payoff(110.0) - 10.0) < 1e-9);
    assert(std::abs(call.payoff(90.0)) < 1e-9);

    assert(std::abs(put.payoff(90.0) - 10.0) < 1e-9);
    assert(std::abs(put.payoff(110.0)) < 1e-9);

    assert(std::abs(digital_call.payoff(120.0) - 1.0) < 1e-9);
    assert(std::abs(digital_call.payoff(99.0)) < 1e-9);
    assert(std::abs(digital_put.payoff(80.0) - 1.0) < 1e-9);
    assert(std::abs(digital_put.payoff(101.0)) < 1e-9);

    assert(american_call.isAmericanOption());
    assert(american_put.isAmericanOption());
    assert(american_call.getOptionType() == OptionType::Call);
    assert(american_put.getOptionType() == OptionType::Put);
    assert(std::abs(american_call.payoff(120.0) - 20.0) < 1e-9);
    assert(std::abs(american_call.payoff(90.0)) < 1e-9);
    assert(std::abs(american_put.payoff(80.0) - 20.0) < 1e-9);
    assert(std::abs(american_put.payoff(120.0)) < 1e-9);

    return 0;
}
