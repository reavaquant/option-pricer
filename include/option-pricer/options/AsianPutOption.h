#ifndef ASIANPUTOPTION_H
#define ASIANPUTOPTION_H
#include "AsianOption.h"
#include <vector>

class AsianPutOption: public AsianOption {
private:
    double _strike;
public:
    AsianPutOption(std::vector<double> timeSteps, double strike);
    double payoff(double asset_price) const override;
    OptionType getOptionType() const override;
};

#endif
