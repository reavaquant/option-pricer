#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>

class Option{
protected:
    double _expiry;
public:
    Option(double) {};
    double getExpiry() const;
    virtual double payoff(double) const;
    virtual ~Option(){}
};

#endif