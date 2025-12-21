#ifndef CRRPRICER_H
#define CRRPRICER_H
#include "Option.h"
#include "BinaryTree.h" 

class CRRPricer{
public:
    CRRPricer(Option* option, int depth, double S0, double U, double D, double R);
    CRRPricer(Option* option, int depth, double S0, double r, double volatility);
    void compute();
    double get(int n, int i);
    double operator()(bool closed_form = false);
    bool getExercise(int n, int i);

private:
    Option* option_;
    int depth_;
    double S0_, U_, D_, R_;
    BinaryTree<double> optionTree_;
    BinaryTree<bool> exerciseTree_;
    
    bool computed_{false};
    static double binom_coeff(int N, int k);
};

#endif
