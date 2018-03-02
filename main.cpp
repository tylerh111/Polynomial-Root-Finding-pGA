
#include <iostream>
#include <typeinfo>

//#include <math.h>
#include <cmath>
#include <complex>


#include "World.h"




int main() {
    std::cout << "Hello, World!" << std::endl;


    Individual me(3, 4);


    auto fitFunct = [](Individual& x){
        std::complex<double> point(x.getReal(), x.getImaginary());
        std::complex<double> output = point * point + std::complex<double>(1,0);


        return std::sqrt(std::pow(output.real(), 2) +
                         std::pow(output.imag(), 2));
    };


    Population p(100, .25, fitFunct);
    p.fitPopulation();

    std::cout << "p[0].getfitness = " << p[0].getFitness() << std::endl;

    return 0;
}