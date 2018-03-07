
#include "Individual.h"

#include <iostream>
#include <complex>


// CONSTRUCTORS
Individual::Individual() {
    _real = 0;
    _imaginary = 0;
    _chromosome = std::complex<double>(0,0);
    _fitness = -1;
}

Individual::Individual(double real, double imaginary){
    _real = real;
    _imaginary = imaginary;
    _chromosome = std::complex<double>(real,imaginary);
    _fitness = -1;

}

/*Individual::Individual(double real, double imaginary, double fitness){
    _real = real;
    _imaginary = imaginary;
    _chromosome = std::complex<double>(real,imaginary);
    _fitness = fitness;

}*/

Individual::Individual(std::complex<double> complex){
    _real = complex.real();
    _imaginary = complex.imag();
    _chromosome = complex;
    _fitness = -1;

}




// MUTATORS
void Individual::setReal(double real) {
    _real= real;
    _chromosome.real(real);
}

void Individual::setImaginary(double imaginary) {
    _imaginary = imaginary;
    _chromosome.imag(imaginary);
}

void Individual::setChromosome(double real, double imaginary){
    _real = real;
    _imaginary = imaginary;
    _chromosome.real(real);
    _chromosome.imag(imaginary);
}

void Individual::setChromosome(std::complex<double> complex){
    _real = complex.real();
    _imaginary = complex.imag();
    _chromosome = complex;
}

void Individual::setFitness(double fitness) {
    _fitness = fitness;
}





Individual& Individual::operator= (const std::complex<double>& that){
    this->_real = that.real();
    this->_imaginary = that.imag();
    this->_chromosome = that;
    this->_fitness = -1;
    return *this;
}


Individual::operator std::complex<double>() const { return this->_chromosome; }


std::ostream& operator<<(std::ostream& out, const Individual& c) {
    out << c._real << " + " << c._imaginary << "i";
    return out;
}










