//
// Created by tdh5188 on 3/18/18.
//

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

