#ifndef POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H

#include "Population.h"
//class Population;

class Individual {
    friend class Population;
private:
    //Chromosomes
    long long _real_bits;
    long long _imaginary_bits;

    double _real;
    double _imaginary;

    double _fitness;

protected:

    void setReal(double real);
    void setImaginary(double imaginary);
    void setFitness(double fitness);

public:

    //static auto fitness_function;



    //Constructor
    Individual();
    Individual(double real, double imaginary);
    ~Individual() = default;

    //Accessors
    inline double getReal() const      { return this->_real; }
    inline double getImaginary() const { return this->_imaginary; }

    inline bool getRealBit(int car) const      { return (this->_real_bits & 1 << car) != 0; }
    inline bool getImaginaryBit(int car) const { return (this->_imaginary_bits & 1 << car) != 0; }

    inline double getFitness() const { return this->_fitness; }






};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
