#ifndef ASIANCALLOPTION_H
#define ASIANCALLOPTION_H
#include "AsianOption.h"
#include <vector>

class AsianCallOption: public AsianOption {
public:
    AsianCallOption(std::vector<double> timeSteps, double strike);
    double payoff(double asset_price) const override;
    OptionType getOptionType() const override;
private:
    double strike_;
};

#endif
