//
// Created by tdh5188 on 4/18/18.
//

#include "Polynomial.h"

Polynomial::Polynomial() : Polynomial(nullptr) { }


/**
 * Polynomial file format:
 * - first line is an integer (d) where d is the degree of the polynomial
 * - following lines are d lines of complex numbers (a b)
 *    where a and b are real numbers; a is the real part and b is the complex part
 * @param filename
 * @return
 */
Polynomial::Polynomial(char* filename){
    this->filename.assign("");
    this->filename.assign(filename);
    std::ifstream infile(filename);

    degree = -1;
    infile >> degree;

    coefficients = new std::complex<double>[degree];

    double real, imag;
    int i = 0;
    while (infile >> real >> imag){
        coefficients[i].real(real);
        coefficients[i].imag(imag);
    }
}


Polynomial::~Polynomial() {
    delete[] coefficients;
}


double Polynomial::operator()(const Individual &individual) const {
    if (filename.empty()) return 0;
    std::complex<double> z = individual.getChromosome();
    std::complex<double> retVal(0,0);
    for (int i = degree, j = 1; j < degree + 2; i--, j++)
        retVal = retVal + coefficients[j] * std::pow(z, i);

    return std::abs(retVal);
}


