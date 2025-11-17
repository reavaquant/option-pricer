#include <cassert>
#include <cmath>
#include <vector>

#include "option-pricer/options/CallOption.hpp"
#include "option-pricer/options/AsianCallOption.hpp"
#include "option-pricer/options/AsianPutOption.hpp"
#include "option-pricer/options/EuropeanDigitalCallOption.hpp"
#include "option-pricer/options/EuropeanDigitalPutOption.hpp"
#include "option-pricer/options/PutOption.hpp"

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);
    EuropeanDigitalCallOption digital_call(1.0, 100.0);
    EuropeanDigitalPutOption digital_put(1.0, 100.0);
    const std::vector<double> asian_time_steps = {0.25, 0.5, 0.75, 1.0};
    AsianCallOption asian_call(1.0, asian_time_steps, 100.0);
    AsianPutOption asian_put(1.0, asian_time_steps, 100.0);

    assert(call.getOptionType() == OptionType::Call);
    assert(put.getOptionType() == OptionType::Put);
    assert(digital_call.getOptionType() == OptionType::Call);
    assert(digital_put.getOptionType() == OptionType::Put);
    assert(asian_call.getOptionType() == OptionType::Call);
    assert(asian_put.getOptionType() == OptionType::Put);

    assert(asian_call.isAsianOption());
    assert(asian_put.isAsianOption());

    assert(std::abs(call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(put.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(digital_call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(digital_put.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(asian_call.getExpiry() - 1.0) < 1e-9);
    assert(std::abs(asian_put.getExpiry() - 1.0) < 1e-9);

    const auto asian_call_steps = asian_call.getTimeSteps();
    assert(asian_call_steps.size() == asian_time_steps.size());
    for (size_t i = 0; i < asian_time_steps.size(); ++i) {
        assert(std::abs(asian_call_steps[i] - asian_time_steps[i]) < 1e-9);
    }

    assert(std::abs(call.payoff(110.0) - 10.0) < 1e-9);
    assert(std::abs(call.payoff(90.0)) < 1e-9);

    assert(std::abs(put.payoff(90.0) - 10.0) < 1e-9);
    assert(std::abs(put.payoff(110.0)) < 1e-9);

    assert(std::abs(digital_call.payoff(120.0) - 1.0) < 1e-9);
    assert(std::abs(digital_call.payoff(99.0)) < 1e-9);
    assert(std::abs(digital_put.payoff(80.0) - 1.0) < 1e-9);
    assert(std::abs(digital_put.payoff(101.0)) < 1e-9);

    std::vector<double> asian_call_path = {90.0, 110.0, 120.0, 100.0};
    const auto asian_call_payoffs = asian_call.payoffPath(asian_call_path);
    assert(asian_call_payoffs.size() == 1);
    assert(std::abs(asian_call_payoffs.front() - 5.0) < 1e-9);

    std::vector<double> asian_put_path = {110.0, 90.0, 80.0, 70.0};
    const auto asian_put_payoffs = asian_put.payoffPath(asian_put_path);
    assert(asian_put_payoffs.size() == 1);
    assert(std::abs(asian_put_payoffs.front() - 12.5) < 1e-9);

    std::vector<double> empty_path;
    assert(asian_call.payoffPath(empty_path).empty());
    assert(asian_put.payoffPath(empty_path).empty());

    return 0;
}
