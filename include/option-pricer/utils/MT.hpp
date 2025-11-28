#ifndef MT_HPP
#define MT_HPP

#include <random>

class MT {
public:
    MT() = delete;
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    static double rand_unif();
    static double rand_norm();

private:
    static std::mt19937& generator();
};

#endif
