
#include "Individual.h"

#include <iostream>
#include <complex>



Individual::Individual() {
    this->_real = 0;
    this->_imaginary = 0;
    this->_real_bits = 0;
    this->_imaginary_bits = 0;
}


Individual::Individual(double real, double imaginary) {
    this->_real = real;
    this->_imaginary = imaginary;
    this->_real_bits = 0;
    this->_imaginary_bits = 0;
}




void Individual::setFitness(double fitness) {
    this->_fitness = fitness;
}




