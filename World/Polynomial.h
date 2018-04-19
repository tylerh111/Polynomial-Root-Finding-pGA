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
    int coefficients_len;

public:
    Polynomial();
    explicit Polynomial(char* filename);
    ~Polynomial();

    Polynomial(const Polynomial& other);
    Polynomial(const Polynomial&& other) noexcept;
    Polynomial& operator=(const Polynomial& other);
    Polynomial& operator=(Polynomial&& other) noexcept;

    inline int getDegree() { return degree; }
    inline std::string getFilename() { return filename; }
    inline const std::complex<double>* getCoefficients() { return coefficients; };

    std::string to_string() const;
    double operator()(const Individual& individual) const;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_POLYNOMIAL_H
