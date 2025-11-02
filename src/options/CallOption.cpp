#include "CallOption.hpp"
#include "EuropeanVanillaOption.hpp"
using namespace std;

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const {
    return OptionType::Call;
}