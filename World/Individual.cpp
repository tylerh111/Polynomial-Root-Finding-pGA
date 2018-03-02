
#include "Individual.h"

#include <iostream>
#include <complex>



Individual::Individual() : std::complex() { }

Individual::Individual(T real, T imaginary): std::complex<T>(real, imaginary) { }


void Individual::setReal(T real) {
    this->real(real);
}


void Individual::setImaginary(T imaginary) {
    this->imag(imaginary);
}


void Individual::setFitness(double fitness) {
    this->_fitness = fitness;
}



Individual* Individual::operator*(Individual &that) {

    std::complex op1 = (std::complex<T>) *this;
    std::complex op2 = (std::complex<T>) that;
    op1 = op1 * op2;

    return new Individual();
}


std::ostream &operator<<(std::ostream& out, const Individual& c) {
    out << c.real() << " + " << c.imag() << "i";
    return out;
}










