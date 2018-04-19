//
// Created by tdh5188 on 3/18/18.
//

#include <complex>
#include <cstdlib>
#include <cmath>
#include <valarray>
#include "Local.h"


std::mt19937_64 mRandom::generator;
bool mRandom::isSeeded = false;

void mRandom::seedGenerator(unsigned long seed) {
    generator.seed(seed);
    isSeeded = true;
}

double mRandom::getRandUniformDist(double lower, double upper) {
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}

double mRandom::getRandNormalDist(double lower, double upper) {
    std::normal_distribution<double> distribution(lower, upper);
    return distribution(generator);
}




std::function<double(const Individual&)> mFitnessFunctions::makeAbsoluteValue(const Polynomial& polynomial) {
    return [&polynomial](const Individual& individual)->double {
        return std::abs(polynomial(individual.getChromosome()));
    };
}


double mFitnessFunctions::applyAbsoluteValue(const Individual& individual) {
    return std::abs(individual.getChromosome());
}








//NetworkSizeException::NetworkSizeException(const std::string &__arg) : runtime_error(__arg) {}




