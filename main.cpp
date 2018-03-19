
#include <iostream>
#include <typeinfo>

#include <cmath>
//#include <unistd.h>
//#include <sys/wait.h>

#include <random>

#include "World.h"
#include "Local.h"


int main() {
    std::cout << "hello world from Polynomial-Root-Finding-pGA" << std::endl;

    //std::string programPath = "/home/tdh5188/Desktop/test_cpp/test_rp.out";


    /*//testing random numbers
    std::default_random_engine generator;
    double offset = 10000 - (-10000);
    std::uniform_real_distribution<double> distribution(0.0, offset);

    for (int i = 0; i < 100; i++){
        std::cout << "random number " << i << " is " << distribution(generator) - offset << std::endl;
    }

    std::cout << "\n\n";*/

    //std::cout << "what? " << std::pow(0, 0) << std::endl;


    mRandom::seedGenerator(0);

    std::function<double(Individual&)> fitFunct = [](Individual& indv){
        std::complex<double> z = indv.getChromosome();
        return std::abs(
                //std::pow(z, 2) + 1.0
                //(z.real() == 0 && z.imag == 0) ? 1.0 : std::pow(z, z)
                3.1415 * std::pow(z, 16)
                + 12.0 * std::pow(z, 10)
                + 2.0 * std::pow(z, 5)
                + 2.0 * std::pow(z, 4)
                + 3.0 * std::pow(z, 3)
                + 4.0 * std::pow(z, 2)
                + 2.0 * z
                + 2.0
        );
    };

    std::cout << "Creating population\n";
    Population mPop(25000, 0.25, 10, 2500, 1e-3,fitFunct);

    std::cout << "Finished creating population\n";
    std::cout << "Fitting the population based on fitness function\n";
    mPop.sort();

/*
    std::cout << "\nDisplaying all Individual's fitness\n";
    for (int i = 0; i < mPop.getPopulationSize(); i++){
        std::cout << "Individual " << i << ": (" << mPop[i] << ") = " << mPop[i].getFitness() << std::endl;
    }
*/
    std::cout << "\nPopulation's total fitness = " << mPop.populationFitness() << std::endl;


    std::cout << "\n\n\t::EVOLVING::\n\n";
    mPop.evolve();
    //mPop.summary();



    return 0;
}