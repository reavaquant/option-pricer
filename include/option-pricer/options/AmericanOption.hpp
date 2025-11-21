#ifndef AMERICANOPTION_HPP
#define AMERICANOPTION_HPP

#include "Option.hpp"

class AmericanOption : public Option {
public:
    using Option::Option;
    bool isAmericanOption() const override;
};

#endif
