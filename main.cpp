
#include <iostream>
#include <typeinfo>

//#include <math.h>
#include <cmath>


#include "World.h"

//#include "World/Population.h"
//#include "World/Individual.h"


int main() {
    std::cout << "Hello, World!" << std::endl;


    Individual me(3, 4);

    auto fitFunct = [](Individual& x){
        return std::sqrt(std::pow(x.getReal(), 2) +
                         std::pow(x.getImaginary(), 2));
    };


    Population p(100, .25, fitFunct);
    p.fitPopulation();

    std::cout << "p[0].getfitness = " << p[0].getFitness() << std::endl;

    return 0;
}