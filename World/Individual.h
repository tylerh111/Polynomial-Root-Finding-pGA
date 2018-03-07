#ifndef POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H

#include "Population.h"

#include <iostream>
#include <complex>
#include <cmath>



class Individual {
    friend class Population;
private:
    //Chromosomes
    double _real;
    double _imaginary;

    std::complex<double> _chromosome;

    double _fitness;

protected:
    //explicit Individual(double real, double imaginary, double fitness);

    void setReal(double real);
    void setImaginary(double imaginary);

    void setChromosome(double real, double imaginary);
    void setChromosome(std::complex<double> complex);

    void setFitness(double fitness);

public:

    //Constructor
    Individual();
    explicit Individual(double real, double imaginary);
    explicit Individual(std::complex<double> complex);
    Individual(const Individual& other) = default;
    Individual(Individual&& other) = default;

    //Destructor
    ~Individual() = default;

    //Accessors
    inline double getReal() const                     { return this->_real; }
    inline double getImaginary() const                { return this->_imaginary; }
    inline std::complex<double> getChromosome() const { return this->_chromosome; }
    inline double getFitness() const                  { return this->_fitness; }


    inline double getAbsoluteValue() const { return std::abs(this->_chromosome); }


    //Operations
    inline friend Individual operator+ (const Individual& lhs, const Individual& rhs) { return Individual(lhs.getChromosome() + rhs.getChromosome()); }
    inline friend Individual operator- (const Individual& lhs, const Individual& rhs) { return Individual(lhs.getChromosome() - rhs.getChromosome()); }
    inline friend Individual operator* (const Individual& lhs, const Individual& rhs) { return Individual(lhs.getChromosome() * rhs.getChromosome()); }
    inline friend Individual operator/ (const Individual& lhs, const Individual& rhs) { return Individual(lhs.getChromosome() / rhs.getChromosome()); }

    inline friend bool operator<  (const Individual& lhs, const Individual& rhs)      { return lhs.getFitness() <  rhs.getFitness(); }
    inline friend bool operator>  (const Individual& lhs, const Individual& rhs)      { return lhs.getFitness() >  rhs.getFitness(); }
    inline friend bool operator<= (const Individual& lhs, const Individual& rhs)      { return lhs.getFitness() <= rhs.getFitness(); }
    inline friend bool operator>= (const Individual& lhs, const Individual& rhs)      { return lhs.getFitness() >= rhs.getFitness(); }

    inline friend bool operator== (const Individual& lhs, const Individual& rhs)      { return lhs.getChromosome() == rhs.getChromosome(); }
    inline friend bool operator!= (const Individual& lhs, const Individual& rhs)      { return lhs.getChromosome() != rhs.getChromosome(); }


    Individual& operator= (const Individual& that) = default;
    Individual& operator= (Individual&& that) = default;
    Individual& operator= (const std::complex<double>& that);



    explicit operator std::complex<double>() const;


    friend std::ostream& operator<<(std::ostream &out, const Individual &c);





};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_INDIVIDUAL_H
