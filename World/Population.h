#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H

//#include <tuple>
#include <vector>
#include <functional>

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

protected:

    explicit Population(int pop_size);


    //Genetic operators
    void select(Individual parents[2]);
    void crossover(Individual offspring[2], Individual& p1, Individual& p2);
    void mutate(Individual& x);
    void replace(Population& replacement);


    //modify population
    void add(Individual& x);
    void remove(Individual& x);


    void getInformation(double arr[8]);



public:

    //Constructors
    Population(int pop_size, double mut_rate, double alpha, std::function<double(Individual&)>& function);
    Population(const Population& that);
    ~Population() = default;


    //Accessors
    inline int getPopulationSize()  const { return _population_size; }
    inline double getMutationRate() const { return _mutation_rate; }
    inline double getAlphaRadius()  const { return _alpha_radius; }

    void evolve(int generations = 1, int starting_gen = 0);


    double fitPopulation();
    double populationFitness();

    //summary of population's history
    //void summary();
    //summary of population's current generation
    void summaryGeneration(double gen[8]);
    void summaryGeneration(int prev_gen_no, double old_gen[8], double new_gen[8]);



    //Operators
    Population& operator= (const Population& that) = default;

    Individual& operator[] (int ndx);

    //Other function
    void sort(bool descending = false);
    void clear();



};

#endif //POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
