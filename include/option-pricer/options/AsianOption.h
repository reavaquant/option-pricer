#ifndef ASIANOPTION_H
#define ASIANOPTION_H
#include "EuropeanVanillaOption.h"
#include <vector>

class AsianOption : public Option {
private:
    std::vector<double> timeSteps_;
public:
    AsianOption(std::vector<double> timeSteps);
    std::vector<double> getTimeSteps() const;
    std::vector<double> payoffPath(const std::vector<double>& path) const override;
    bool isAsianOption() const override;
};

#endif
