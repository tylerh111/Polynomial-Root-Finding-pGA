
#include <iostream>
#include <typeinfo>

#include <cmath>
//#include <unistd.h>
//#include <sys/wait.h>

#include "World.h"
#include <complex>




int main() {
    std::cout << "hello world from Polynomial-Root-Finding-pGA" << std::endl;

    //std::string programPath = "/home/tdh5188/Desktop/test_cpp/test_rp.out";

    //std::complex test(10,2);
    //std::complex result = test * test;

    Individual me(3, 4);

    auto ret = me * me;

    std::cout << "me^2 = " << ret << std::endl;

    std::complex<double> this_is_insane(10,3);

    std::complex<double> temp = static_cast<std::complex<double>>(me);

    me = this_is_insane;
    this_is_insane = temp;

    std::cout << "this_is_insane = " << this_is_insane << std::endl;
    std::cout << "me = " << me << std::endl;


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