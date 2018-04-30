//
// Created by tdh5188 on 3/18/18.
//

#include <complex>
#include <cstdlib>
#include <cmath>
#include <valarray>
#include <cstring>
#include "Local.h"


std::random_device rng::device;
std::mt19937_64 rng::generator(rng::device());
//std::mt19937_64 rng::generator(0);
bool rng::isSeeded = false;

void rng::seedGenerator(unsigned long seed) {
    generator.seed(seed);
    isSeeded = true;
}

double rng::getRealUniformDist(double lower, double upper) {
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}



double rng::getRealNormalDist(double lower, double upper) {
    std::normal_distribution<double> distribution(lower, upper);
    return distribution(generator);
}



/*std::mt19937_64 mRandom::generator;
bool mRandom::isSeeded = false;

void mRandom::seedGenerator(unsigned long seed) {
    generator.seed(seed);
    isSeeded = true;
}

double mRandom::getRealUniformDist(double lower, double upper) {
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}

double mRandom::getRealNormalDist(double lower, double upper) {
    std::normal_distribution<double> distribution(lower, upper);
    return distribution(generator);
}*/




std::function<double(const Individual&)> mFitnessFunctions::makeAbsoluteValue(const Polynomial& polynomial) {
    return [&polynomial](const Individual& individual)->double {
        return std::abs(polynomial(individual.getChromosome()));
    };
}


double mFitnessFunctions::applyAbsoluteValue(const Individual& individual) {
    return std::abs(individual.getChromosome());
}



void mInformationRetrieval::getInformation(int argc, char **argv, unsigned long &pop_s, double &acpt_e,
                                           double &mut_rate, double &mut_rad, double &start_rad) {
    for (int j = 0; j < argc; j++) {
        if (strcmp("-p", argv[j]) == 0 || strcmp("--population-size", argv[j]) == 0) {
            pop_s     = strtoul(argv[++j], nullptr, 0);
        }
        if (strcmp("-ae", argv[j]) == 0 || strcmp("--accepted-error", argv[j]) == 0) {
            acpt_e    = strtod(argv[++j], nullptr);
        }
        if (strcmp("-mr", argv[j]) == 0 || strcmp("--mutation-rate", argv[j]) == 0) {
            mut_rate  = strtod(argv[++j], nullptr);
        }
        if (strcmp("-ms", argv[j]) == 0 || strcmp("--mutation-radius", argv[j]) == 0) {
            mut_rad   = strtod(argv[++j], nullptr);
        }
        if (strcmp("-ss", argv[j]) == 0 || strcmp("--starting-radius", argv[j]) == 0) {
            start_rad = strtod(argv[++j], nullptr);
        }

    }
}







//NetworkSizeException::NetworkSizeException(const std::string &__arg) : runtime_error(__arg) {}




