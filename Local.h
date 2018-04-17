#ifndef POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H


#include <iostream>
#include <cstdlib>
#include <random>
#include <limits>
#include <ctime>
#include "World/Individual.h"


namespace mRandom {

    extern std::mt19937_64 generator;
    extern bool isSeeded;

    extern void seedGenerator(unsigned long seed);

    extern double getRandUniformDist(double lower, double upper);
    extern double getRandNormalDist(double lower, double upper);
}


namespace mPolynomial {
    extern std::function<double(Individual&)> getPolynomial(int degree, char** coefficients);

}




#endif //POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
