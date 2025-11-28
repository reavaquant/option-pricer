#ifndef ASIANOPTION_HPP
#define ASIANOPTION_HPP
#include "EuropeanVanillaOption.hpp"
#include <vector>

class AsianOption : public Option {
private:
    std::vector<double> timeSteps_;
public:
    AsianOption(double expiry, std::vector<double> timeSteps);
    std::vector<double> getTimeSteps() const;
    std::vector<double> payoffPath(const std::vector<double>& path) const override;
    bool isAsianOption() const override;
};

#endif
