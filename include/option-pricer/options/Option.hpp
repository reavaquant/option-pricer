#ifndef OPTION_HPP
#define OPTION_HPP

enum class OptionType {
    Call,
    Put
};

class Option{
protected:
    double _expiry;
public:
    Option(double expiry);
    double getExpiry() const;
    virtual double payoff(double underlying) const = 0;
    virtual OptionType getOptionType() const = 0;
    virtual ~Option(){}
};

#endif