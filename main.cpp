
#include <iostream>
#include <typeinfo>

#include <cmath>
//#include <unistd.h>
//#include <sys/wait.h>

#include <random>

#include "World.h"
#include <complex>




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




    std::function<double(Individual&)> fitFunct = [](Individual& x){
        //std::complex<double> temp = x.getChromosome();
        //std::complex<double> output = std::pow(temp, 2) + 1.0;

        return std::abs(std::pow(x.getChromosome(), 2) + 1.0);
    };

    std::cout << "Creating population\n";
    Population mPop(100, 0.25, 5, fitFunct);
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
    mPop.evolve(100);



    return 0;
}