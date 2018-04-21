#ifndef POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H


#include <iostream>
#include <cstdlib>
#include <random>
#include <limits>
#include <ctime>
#include <functional>
#include <complex>
#include "World/Individual.h"
#include "World/Polynomial.h"




/*extern int buf [MAX_SIZE];

auto init_buf = [](int b[MAX_SIZE]){
    for(int i = 0; i < MAX_SIZE; i++)
        b[i] = 0;
};*/

namespace mRandom {

    extern std::mt19937_64 generator;
    extern bool isSeeded;

    extern void seedGenerator(unsigned long seed);

    extern double getRandUniformDist(double lower, double upper);
    extern double getRandNormalDist(double lower, double upper);
}


namespace mFitnessFunctions {
    extern double applyAbsoluteValue(const Individual& individual);
    extern std::function<double(const Individual&)> makeAbsoluteValue(const Polynomial& polynomial);
}


namespace mInformationRetrieval{
    extern void getInformation(int argc,
                               char** argv,
                               unsigned long &pop_s,
                               double &acpt_e,
                               double &mut_rate,
                               double &mut_rad,
                               double &start_rad);
}



#endif //POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
