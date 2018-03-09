#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H

//#include <tuple>
#include <vector>
#include <functional>
#include <random>

#include "Individual.h"
class Individual;

class Population {
private:
    std::vector<Individual> _population;

    int _population_size;
    double _mutation_rate;
    std::function<double(Individual&)> _fitness_function;

    //range for crossover
    //double _major_axis; //ellipse a value
    //double _minor_axis; //ellipse b value

    //range for mutation
    double _alpha_radius;

    int _generation;

    //hide default population constructor
    Population() = default;


    struct PopulationSizeException : public std::exception{
        const char* what() const throw() override {
            return "Population size is incorrect.";
        }
    } _pop_size_ex;


    //random number generator
    //std::default_random_engine _rng;


protected:

    explicit Population(int pop_size);


    //Genetic operators
    void select(Individual* parents[2]);
    void crossover(Individual offspring[2], Individual* parents[2]);
    void mutate(Individual& x);
    void replace(Population& replacement);

    bool checkSolution() const;
    bool checkConvergence() const;
    void handleConvergence();

    //modify population
    void add(Individual& x);
    void remove(Individual& x);


public:

    //Constructors
    Population(int pop_size, double mut_rate, double mut_radius, double start_radius,
               std::function<double(Individual&)>& function);
    Population(const Population& that);
    ~Population() = default;

    void init(double start_radius);

    //Accessors
    inline int getPopulationSize()  const { return _population_size; }
    inline double getMutationRate() const { return _mutation_rate; }
    inline double getAlphaRadius()  const { return _alpha_radius; }

    void evolve(int generations = std::numeric_limits<int>::max(), int starting_gen = 0);


    double fitPopulation();
    double populationFitness();

    //summary of population's history
    void summary() const;


    //Operators
    Population& operator= (const Population& that) = default;

    Individual& operator[] (int ndx);

    //Other function
    void sort(bool descending = false);
    void clear();


};

#endif //POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
