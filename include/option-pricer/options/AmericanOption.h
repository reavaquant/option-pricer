#ifndef AMERICANOPTION_H
#define AMERICANOPTION_H

#include "Option.h"

class AmericanOption : public Option {
protected:
    double strike_;
public:
    AmericanOption(double expiry, double strike);

    double getStrike() const;
    bool isAmericanOption() const override;

    virtual OptionType getOptionType() const override = 0;
    virtual double payoff(double asset_price) const override = 0;
};

#endif
