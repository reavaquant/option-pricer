#ifndef ASIANPUTOPTION_HPP
#define ASIANPUTOPTION_HPP
#include "AsianOption.hpp"
#include <vector>

class AsianPutOption: public AsianOption {
public:
    AsianPutOption(double expiry, std::vector<double> timeSteps, double strike);
    double payoff(double asset_price) const override;
    OptionType getOptionType() const override;
private:
    double strike_;
};

#endif
