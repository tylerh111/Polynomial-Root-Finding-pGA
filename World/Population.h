#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H

//#include <tuple>
#include <functional>

#include "Individual.h"
class Individual;

class Population {
    //friend class Individual;
private:
    Individual** _population;
    int _population_size;
    double _mutation_rate;
    std::function<double(Individual&)> _fitness_function;

    //hide default population constructor
    Population() = default;

public:

    //Constructors
    Population(int pop_size, double mut_rate, std::function<double(Individual&)> function);
    ~Population();

    //Genetic operators
    Individual& select();
    Individual* crossover(Individual& x1, Individual& x2);
    void mutate(Individual& x);
    void replace(Population& replacement);

    //double getFitness(Individual& x);

    void fitPopulation();


    void sort();

    //Operators
    Individual& operator[] (int ndx);



};

#endif //POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
