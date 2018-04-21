#ifndef POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
#define POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H

//#include <tuple>
#include <utility>
#include <vector>
#include <functional>
#include <random>
#include <cstring>

#include "Individual.h"
#include "Polynomial.h"

#include "../Local.h"


class Population {
private:

    //local population
    std::vector<Individual> _population;
    unsigned long _population_size{};

    //combined population (for migration)
    std::vector<Individual*> _combined_population;
    unsigned long _combined_population_size{};

    //number of generation that this population evolve
    unsigned long _generation{};

    //mutation: rate and radius
    double _mutation_rate{};
    double _mutation_radius{};

    //accepted error
    double _accepted_error{};

    //fitness function
    std::function<double(const Individual&)> _fitness_function;
    //static const std::function<double(const Individual&)> DEF_FITNESS_FUNCTION;

    //range for crossover
    //double _minor_axis; //ellipse b value
    //double _major_axis; //ellipse a value

    //hide default population constructor
    Population() = default;

    explicit Population(unsigned long pop_size);

protected:

    //Genetic operators
    void select(Individual* parents[2]);
    void crossover(Individual offspring[2], Individual* parents[2]);
    void mutate(Individual& x);
    void replace(Population& replacement);

    int handleConvergence();


    //modify population
    void add(Individual& x);
    void addAll(const std::vector<Individual>& vector);

    void remove(Individual& x);

    Individual& get(unsigned long i);

public:

    //if population is not big enough
    class PopulationSizeException : public std::runtime_error{
    public:
        explicit PopulationSizeException(const std::string &msg = "Population size is too small")
                : runtime_error(msg) {}
    };


    //status codes on evolve()
    enum STATUS {
        NOT_FOUND = 0,
        FOUND     = 1,
        CONVERGED = 2};


    //getSummary() indices and size
    static const int SUM_NDX_MEAN     = 0;
    static const int SUM_NDX_STD_DEV  = 1;
    static const int SUM_NDX_MIN      = 2;
    static const int SUM_NDX_FIRST_Q  = 3;
    static const int SUM_NDX_SECOND_Q = 4;
    static const int SUM_NDX_THIRD_Q  = 5;
    static const int SUM_NDX_MAX      = 6;
    static const int SUM_SIZE         = 7;

    //default values for hyper parameters
    static const unsigned long DEF_POPULATION_SIZE  = 100;
    static constexpr double    DEF_ACCEPTED_ERROR   = 0.005;
    static constexpr double    DEF_MUTATION_RATE    = 0.33;
    static constexpr double    DEF_MUTATION_RADIUS  = 100;
    static constexpr double    DEF_START_RADIUS     = 1000000;
    static const unsigned long DEF_START_GENERATION = 0;


    //Constructors
    explicit Population(std::function<double(const Individual&)> function,
                        unsigned long pop_size         = DEF_POPULATION_SIZE,
                        double        accepted_error   = DEF_ACCEPTED_ERROR,
                        double        mut_rate         = DEF_MUTATION_RATE,
                        double        mut_radius       = DEF_MUTATION_RADIUS,
                        unsigned long start_generation = DEF_START_GENERATION);

    Population(const Population& that);
    Population(const Population&& that);
    Population& operator=(const Population& that);
    Population& operator=(const Population&& that);
    ~Population() = default;

    void init(double start_radius = DEF_START_RADIUS);

    //Accessors
    inline unsigned long getPopulationSize() const { return _population_size; }
    inline unsigned long getGeneration()     const { return _generation;      }
    inline double        getMutationRate()   const { return _mutation_rate;   }
    inline double        getMutationRadius() const { return _mutation_radius; }
    inline double        getAcceptedError()  const { return _accepted_error;  }

    //Mutators
    inline void setPopulationSize(unsigned long new_pop_size)       { _population_size = new_pop_size;    }
    inline void setGeneration(unsigned long new_gen)                { _generation = new_gen;              }
    inline void setMutationRate(double new_mut_rate)                { _mutation_rate = new_mut_rate;      }
    inline void setMutationRadius(double new_mut_radius)            { _mutation_radius = new_mut_radius;  }
    inline void setAcceptedError(double new_accepted_val)           { _accepted_error = new_accepted_val; }
    inline void setFitnessFunction(const std::function<double(const Individual&)>& new_fit_funct)
                                                                    { _fitness_function = new_fit_funct;  }



    //add migrates to combined population
    void integration(const std::vector<Individual>& vector);


    STATUS evolve();
    //void evolveLoop(int generations = std::numeric_limits<int>::max(), int starting_gen = 0);



    //population status functions
    bool checkSolution() const;
    bool checkChromosomeConvergence() const;
    bool checkFitnessConvergence() const;

    //set and fit population
    double fitPopulation();
    static double fitPopulation(Population& population);
    double populationFitness();
    static double populationFitness(const Population&);
    double populationFitness(const std::vector<Individual*>& vector);


    //summary of population's history
    std::string printSummary() const;
    void getSummary(double summary[SUM_SIZE]) const;


    const Individual& getBestFit();


    //Operators
    Individual& operator[] (int ndx);
    friend std::ostream& operator<<(std::ostream &out, const Population &c);


    //Other function
    void sort(bool descending = false);
    void clear();
    void reset();



};

#endif //POLYNOMIAL_ROOT_FINDING_PGA_POPULATION_H
