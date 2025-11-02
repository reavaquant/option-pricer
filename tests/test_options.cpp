#include <cassert>
#include <cmath>

#include "option-pricer/options/CallOption.hpp"
#include "option-pricer/options/PutOption.hpp"

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);

    assert(call.getOptionType() == OptionType::Call);
    assert(put.getOptionType() == OptionType::Put);

    assert(std::abs(call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(put.getExpiry() - 1.0) < 1e-9);

    assert(std::abs(call.payoff(110.0) - 10.0) < 1e-9);
    assert(std::abs(call.payoff(90.0)) < 1e-9);

    assert(std::abs(put.payoff(90.0) - 10.0) < 1e-9);
    assert(std::abs(put.payoff(110.0)) < 1e-9);

    return 0;
}