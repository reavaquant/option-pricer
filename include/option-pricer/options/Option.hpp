#ifndef OPTION_H
#define OPTION_H

#include <iostream>

class Option{
private:
    double _expiry;
public:
    Option(double){};
    double getExpiry() const;
    virtual double payoff(double) const;
    virtual ~Option(){}
};

#endif