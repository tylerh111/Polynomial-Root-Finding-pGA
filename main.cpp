
#include <iostream>
#include <typeinfo>

//#include <math.h>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

#include "World.h"
#include <complex>




int main() {
    std::cout << "hello world from Polynomial-Root-Finding-pGA" << std::endl;

    std::string programPath = "/home/tdh5188/Desktop/test_cpp/test_rp.out";



    Individual me(3, 4);


    std::cout << "me^2 = " << *(me * me) << std::endl;

    //std::complex<double> ret(2, 3);
    //std::cout << "ret = " << ret * ret << std::endl;

/*
    auto fitFunct = [](Individual& x){
        std::complex<double> point(x.getReal(), x.getImaginary());
        std::complex<double> output = point * point + std::complex<double>(1,0);


        return std::sqrt(std::pow(output.real(), 2) +
                         std::pow(output.imag(), 2));

        std::complex output = x * x;


        return std::pow()
        //return std::sqrt(std::pow(x.getReal(), 2) +
        //                 std::pow(x.getImaginary(), 2));
    };



    Population p(100, .25, fitFunct);
    p.fitPopulation();

    std::cout << "p[0].getfitness = " << p[0].getFitness() << std::endl;
*/
    return 0;
}