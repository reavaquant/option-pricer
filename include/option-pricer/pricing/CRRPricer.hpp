#ifndef CRRPRICER_HPP
#define CRRPRICER_HPP
#include "Option.hpp"
#include "BinaryTree.hpp" 

class CRRPricer{
public:
    CRRPricer(Option* option, int depth, double S0, double U, double D, double R);
    CRRPricer(Option* option, int depth, double S0, double r, double volatility);
    void compute();
    double get(int n, int i);
    double operator()(bool closed_form);
    bool getExercise(int n, int i);

private:
    Option* option_;
    int depth_;
    double S0_, U_, D_, R_;
    BinaryTree<double> optionTree_;
    BinaryTree<bool> exerciseTree_;
    
    bool computed_{false};
    static long double binom_coeff(int N, int k);
};

#endif
