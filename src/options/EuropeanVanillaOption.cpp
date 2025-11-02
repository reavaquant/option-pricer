#include "EuropeanVanillaOption.hpp"
#include <iostream>
using namespace std;

EuropeanVanillaOption::EuropeanVanillaOption(double h, double s) : Option(s){
    if(h < 0 && s < 0){
        cout << "expiry and strike price must be nonnegative" << endl;
    }
}

