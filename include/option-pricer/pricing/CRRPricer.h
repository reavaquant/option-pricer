#ifndef CRRPRICER_H
#define CRRPRICER_H
#include "Option.h"
#include "BinaryTree.h" 

class CRRPricer{
private:
    Option* _option;
    int _depth;
    double _S0, _U, _D, _R;
    BinaryTree<double> _optionTree;
    BinaryTree<bool> _exerciseTree;
    
    bool _computed{false};
    static double binom_coeff(int N, int k);
public:
    CRRPricer(Option* option, int depth, double S0, double U, double D, double R);
    CRRPricer(Option* option, int depth, double S0, double r, double volatility);
    void compute();
    double get(int n, int i);
    double operator()(bool closed_form = false);
    bool getExercise(int n, int i);
};

#endif
