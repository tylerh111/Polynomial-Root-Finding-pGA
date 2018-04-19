//
// Created by tdh5188 on 4/18/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H

#include "Individual.h"

#include <complex>
#include <string>
#include <fstream>


//TODO: someday make this class abstract (operator is virtual) so fitness function is customizable.
//TODO:   This is a framework creation issue
class Polynomial{
    std::string filename;
    int degree;
    std::complex<double>* coefficients;

public:
    Polynomial();
    explicit Polynomial(char* filename);
    ~Polynomial();

    double operator()(const Individual& individual) const;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H
