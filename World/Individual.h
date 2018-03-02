#ifndef POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H

#include "Population.h"

#include <iostream>
#include <complex>


typedef int T;


class Individual : public std::complex<T> {
    friend class Population;
private:
    //Chromosomes
    //long long _real_bits;
    //long long _imaginary_bits;

    //double _real;
    //double _imaginary;

    double _fitness;

protected:

    void setReal(T real);
    void setImaginary(T imaginary);
    void setFitness(double fitness);

public:

    //Constructor
    Individual() = default;
    Individual(T real, T imaginary);
    ~Individual() = default;

    //Accessors
    inline T getReal() const      { return this->real(); }
    inline T getImaginary() const { return this->imag(); }
    inline double getFitness() const   { return this->_fitness; }

    //inline bool getRealBit(int car) const      { return (this->_real_bits & 1 << car) != 0; }
    //inline bool getImaginaryBit(int car) const { return (this->_imaginary_bits & 1 << car) != 0; }



    Individual* operator*(Individual& that);

    friend std::ostream& operator<<(std::ostream &out, const Individual &c);





};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
