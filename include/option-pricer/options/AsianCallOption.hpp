#ifndef ASIANCALLOPTION_HPP
#define ASIANCALLOPTION_HPP
#include "AsianOption.hpp"
#include <vector>

class AsianCallOption: public AsianOption {
public:
    AsianCallOption(double expiry, std::vector<double> timeSteps, double strike);
    double payoff(double asset_price) const override;
    OptionType getOptionType() const override;
private:
    double strike_;
};

#endif
