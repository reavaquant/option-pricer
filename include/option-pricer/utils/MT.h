#ifndef MT_H
#define MT_H

#include <random>

class MT {
private:
    static std::mt19937& generator();
public:
    MT() = delete;
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    static double rand_unif();
    static double rand_norm();
};

#endif
