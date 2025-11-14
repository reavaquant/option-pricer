#ifndef OPTION_HPP
#define OPTION_HPP
#include <vector>

enum class OptionType {
    Call,
    Put
};

class Option{
protected:
    double expiry_;
public:
    Option(double expiry);
    double getExpiry() const;
    virtual double payoff(double asset_price) const = 0;
    virtual OptionType getOptionType() const = 0;
    virtual ~Option(){}
    virtual std::vector<double> payoffPath(const std::vector<double>& path) const;
    virtual bool isAsianOption() const;
};

#endif