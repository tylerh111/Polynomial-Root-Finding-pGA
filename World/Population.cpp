
#include <algorithm>
#include <complex>
#include <exception>
#include <cmath>
#include <ctime>
#include <iomanip>
#include "Population.h"

#include "../Local.h"





// CONSTRUCTORS
Population::Population(unsigned long pop_size) : Population(pop_size, Polynomial(nullptr)) { }

Population::Population(unsigned long pop_size,
                       Polynomial function,
                       double accepted_error,
                       double mut_rate,
                       double mut_radius,
                       unsigned long start_generation) {
    if (pop_size < 3) throw PopulationSizeException();

    this->_population = std::vector<Individual>(pop_size);
    this->_population_size = pop_size;

    this->_combined_population = _population;
    this->_combined_population_size = _population_size;

    this->_accepted_error = accepted_error;
    this->_mutation_rate = mut_rate;
    this->_mutation_radius = mut_radius;
    this->_generation = start_generation;

    this->_fitness_function = function;


}

Population::Population(const Population& that) {
    this->_population_size = that._population_size;
    this->_accepted_error = that._accepted_error;
    this->_generation = that._generation;
    this->_mutation_rate = that._mutation_rate;
    this->_mutation_radius = that._mutation_radius;

    this->_fitness_function = that._fitness_function;


    this->clear();
    this->_population = std::vector<Individual>(that._population_size);
    this->_combined_population = std::vector<Individual>(that._combined_population_size);

    for (int i = 0; i < this->_population_size; i++) {
        _population[i] = Individual(that._population[i]);
    }

    for (int i = 0; i < that._combined_population_size; i++){
        _combined_population[i] = Individual(that._combined_population[i]);
    }
}




void Population::init(double start_radius){
    double offset = std::abs(start_radius);

    for (int i = 0; i < _population_size; i++) {
        _population[i] = Individual(mRandom::getRandUniformDist(0.0, 2*offset) - offset,
                                    mRandom::getRandUniformDist(0.0, 2*offset) - offset);
        _population[i].setFitness(_fitness_function(_population[i]));
        _combined_population[i] = _population[i];
    }
}




/**
 *
 * @param parents
 */
void Population::select(Individual* parents[2]) {
    Individual* ret[2];	          //ret    := used to hold ret values until end to assign parents
    Individual* not_me = nullptr; //not_me := used to avoid selecting same member twice

    unsigned long pop_cap = _population_size;

    //total of all members' fitness
    double totalFitness = populationFitness(_combined_population);

    //probability and accumulated are used to pick from the population using a uniform distribution
    //ex: probability: { 0.5, 0.3, 0.2 }
    //	  accumulated: { 0.5, 0.8, 1.0 } <-- pick based on this array
    auto probability = new double[pop_cap];	//probability[i] = fitness(i) / total_i
    auto accumulated = new double[pop_cap];	//accumulated[i] = accumulated[i - 1] + probability[i]

    //calculating probability
    for (int i = 0; i < pop_cap; i++) probability[i] = _population[i].getFitness() / totalFitness;

    //calculating accumulated
    accumulated[0] = probability[0];
    accumulated[pop_cap - 1] = 1;
    for (int i = 1; i < pop_cap - 1; i++) accumulated[i] = accumulated[i - 1] + probability[i];


    //for each parent (2)
    for (auto &ndx : ret) {
        //do while the parents are equal to each other
        do {
            //get random number [0, 1] from uniform distribution to pick from population
            double num = mRandom::getRandUniformDist(0.0, 1.0);

            //pick from population by setting i where num <= accumulated[i]
            unsigned long i = 0;
            while (i < pop_cap && num > accumulated[i++]) { /*do nothing*/ }

            //if the index we picked was from
            ndx = &_combined_population[--i];

        } while (ndx == not_me);

        //to make sure parents are not equal
        not_me = ret[0];
    }

    //copy parents for higher scoped function
    parents[0] = ret[0];
    parents[1] = ret[1];

    //deleting
    delete[] probability;
    delete[] accumulated;
}






void Population::crossover(Individual offspring[2], Individual* parents[2]) {

    Individual ret[2];

    std::complex<double> p1 = parents[0]->getChromosome();
    std::complex<double> p2 = parents[1]->getChromosome();

    std::complex<double> diff = p1 - p2;
    double abs_diff = std::abs(diff);

    double real_lower = std::min(p1.real(), p2.real()) - abs_diff;
    double real_upper = std::max(p1.real(), p2.real()) + abs_diff;
    double imag_lower = std::min(p1.imag(), p2.imag()) - abs_diff;
    double imag_upper = std::max(p1.imag(), p2.imag()) + abs_diff;


    //geofencing
    ret[0] = Individual(mRandom::getRandUniformDist(real_lower, real_upper),
                        mRandom::getRandUniformDist(imag_lower, imag_upper));
    ret[1] = Individual(mRandom::getRandUniformDist(real_lower, real_upper),
                        mRandom::getRandUniformDist(imag_lower, imag_upper));


    ret[0].setFitness(_fitness_function(ret[0]));
    ret[1].setFitness(_fitness_function(ret[1]));

    offspring[0] = ret[0];
    offspring[1] = ret[1];

}


void Population::mutate(Individual &x){

    if (mRandom::getRandUniformDist(0.0, 1.0) <= _mutation_rate) {
        double new_real;
        double new_imag;

        //pick numbers within range
        do {
            new_real = mRandom::getRandUniformDist(0.0, _mutation_radius);
            new_imag = mRandom::getRandUniformDist(0.0, _mutation_radius);

        } while (std::pow(new_real, 2) + std::pow(new_imag, 2) <= std::pow(_mutation_radius, 2));

        x.setReal(new_real + x.getReal());
        x.setImaginary(new_imag + x.getImaginary());
    }

}

//Replacement policies to checkout
// Chapter 7 Replacement: http://shodhganga.inflibnet.ac.in/bitstream/10603/32680/17/17_chapter%207.pdf
/*

 Generational Replacement:
 - everyone is replaced
 - mu + lambda replacement: parents and offspring fight for survival
 - mu, lambda  replacement: create many new offspring, offspring fight each other for survival

 Steady State Replacement:
 - overlapping population

 Other Replacement policies:
 - Elitism: population is completely replaced besides the best member of of each generation (carried over with no modifications)
 - Delete n-last: n weakest individuals are replaced by n descendants
 - Delete n: arbitrarily chose n individuals of old population to be replaced by n individuals of new population
 - Random Replacement: the children replace two randomly chosen individuals in the population (parents are also candidates for selection)
 - Weak Parent Replacement: weak parent is replaced by strong child (with the four individuals, only the fittest two, parent and child, return to population)
 - Both Parent Replacement: each individual is allowed to breed only once, both parent chromosomes get replaced in each generation by the offsprings.



 */
void Population::replace(Population &replacement){

    //add the new individuals
    for (long i = 0; i < replacement._population_size; i++)
        _population.push_back(replacement[i]);

    //sort from least to greatest
    sort();

    //remove the worst
    for (long i = 0; i < replacement._population_size; i++)
        _population.pop_back();

}




bool Population::checkSolution() const {
    return _population[0].getFitness() <= _accepted_error;
}

bool Population::checkChromosomeConvergence() const {
    double recur_real = _population[0].getReal();
    double recur_imag = _population[0].getImaginary();

    for (int i = 1; i < _population_size; i++){
        if (recur_real != _population[i].getReal() &&
            recur_imag != _population[i].getImaginary())
            return false;
    }

    return true;
}

bool Population::checkFitnessConvergence() const {

    double recur_fitness = _population[0].getFitness();

    for (int i = 1; i < _population_size; i++){
        if (recur_fitness != _population[i].getFitness())
            return false;
    }

    return true;
}

int Population::handleConvergence() {

    for (int i = 0; i < _population_size; i++) {
        double new_real;
        double new_imag;

        //pick numbers within range
        do {
            new_real = mRandom::getRandUniformDist(0.0, _mutation_radius);
            new_imag = mRandom::getRandUniformDist(0.0, _mutation_radius);
        } while (std::pow(new_real, 2) + std::pow(new_imag, 2) <= std::pow(_mutation_radius, 2));


        _population[i].setReal(new_real + _population[i].getReal());
        _population[i].setImaginary(new_imag + _population[i].getImaginary());
    }

}







void Population::add(Individual &x) {
    _population.push_back(x);
}

void Population::addAll(const std::vector<Individual> &vector) {
    _population.insert(std::end(_population), std::begin(vector), std::end(vector));
}

void Population::remove(Individual &x) {
    int i = 0;
    while (i < _population_size && _population[i++] == x) { /* do nothing */ }
    _population.erase(_population.begin() + --i);
}





/*bool shouldPrint(int gen){
    return (gen < 10) ||
           ((gen < 100) && (gen % 10 == 0)) ||
           ((gen < 1000) && (gen % 100 == 0)) ||
           ((gen < 10000) && (gen % 1000 == 0)) ||
           (gen % 10000 == 0);
}*/









Population::STATUS Population::evolve(){

    STATUS status = NOT_FOUND;

    //sort population
    sort();

    //print summary
    printSummary();

    //check for a root
    if (checkSolution()) return FOUND;

    //check & handle convergence
    if (checkChromosomeConvergence()) {
        status = CONVERGED;
        handleConvergence();
    }


    //new generation
    unsigned long new_pop_size = _population_size / 3;
    if (new_pop_size % 2 != 0) new_pop_size++;

    Population new_generation(new_pop_size, _fitness_function);

    for (unsigned long i = 0; i < (_population_size / 3); i+=2) {

        Individual* parents[2];
        select(parents);

        Individual offspring[2];
        crossover(offspring, parents);

        mutate(offspring[0]);
        mutate(offspring[1]);

        new_generation._population.push_back(offspring[0]);
        new_generation._population.push_back(offspring[1]);

        //new_generation._population.at(i)     = offspring[0];
        //new_generation._population.at(i + 1) = offspring[1];
    }

    fitPopulation(new_generation);

    replace(new_generation);

    _generation++;

    _combined_population = _population;

    return status;
}



/*void Population::evolveLoop(int generations, int starting_gen) {

    for (int gen = starting_gen; gen < generations; gen++){

        //sort population
        sort();

        //print summary
        //if (shouldPrint(gen)) summary();
        printSummary();

        //check for a root
        if (checkSolution()) break;

        //check & handle convergence
        if (checkConvergence()) {
            std::cout << "\n\n\t===================\n";
            std::cout << "\t<<< CONVERGENCE >>>\n";
            std::cout << "\t===================\n\n\n";
            handleConvergence();
        }


        //new generation
        unsigned long new_pop_size = _population_size / 3;
        if (new_pop_size % 2 != 0) new_pop_size++;

        Population new_generation(new_pop_size, _fitness_function);//, _fitness_function);

        for (unsigned long i = 0; i < (_population_size / 3); i+=2) {

            Individual* parents[2];
            select(parents);

            Individual offspring[2];
            crossover(offspring, parents);

            mutate(offspring[0]);
            mutate(offspring[1]);

            new_generation._population.at(i)     = offspring[0];
            new_generation._population.at(i + 1) = offspring[1];

        }

        //new_generation.fitPopulation();

        replace(new_generation);

        _generation++;
    }


    std::cout << "solution found in generation " << _generation << std::endl;
    std::cout << "solution = " << _population[0] << std::endl;
    std::cout << "population = [\n" << *this << "\n]" << std::endl;

}*/




/**
 * getSummary() returns a double array in the form of:
 *   { mean, std deviation, min, first quartile, second quartile, third quartile, max }
 *   REQUIRES a sorted population
 * @return
 */
void Population::getSummary(double summary[SUM_SIZE]) const {

    //mean
    double mean = 0;
    for(int i = 0; i < _population_size; i++) mean += _population[i].getFitness();
    mean /= _population_size;

    //std dev
    double std_dev= 0;
    for (int i = 0; i < _population_size; i++)
        std_dev += std::pow((_population[i].getFitness() - mean), 2);
    std_dev = std::sqrt(std_dev / _population_size);

    //quartiles
    auto sec_q_el = (int) std::floor(_population_size / 2);
    double sec_q = _population[sec_q_el].getFitness();

    auto fir_q_el = (int) std::floor(sec_q_el / 2);
    double fir_q = _population[fir_q_el].getFitness();

    auto thr_q_el = fir_q_el + sec_q_el;
    double thr_q = _population[thr_q_el].getFitness();

    //max/min
    double pop_min = _population[0].getFitness();
    double pop_max = _population[_population.size()].getFitness();

    summary[SUM_NDX_MEAN] = mean;
    summary[SUM_NDX_STD_DEV] = std_dev;
    summary[SUM_NDX_MIN] = pop_min;
    summary[SUM_NDX_FIRST_Q] = fir_q;
    summary[SUM_NDX_SECOND_Q] = sec_q;
    summary[SUM_NDX_THIRD_Q] = thr_q;
    summary[SUM_NDX_MAX] = pop_max;
}



/**
 * printSummary() uses getSummary to construct a string of the summary of the population neatly
 *   REQUIRES a sorted population
 * @return string of the summary
 */
std::string Population::printSummary() const {

    double summary[SUM_SIZE];

    getSummary(summary);

    std::stringstream ss;

    ss << "GENERATION :: " << _generation << std::endl;
    ss << std::fixed << std::setw(14) << "Count |"
              << std::fixed << std::setw(14) << "Mean |"
              << std::fixed << std::setw(14) << "Std Dev |"
              << std::fixed << std::setw(14) << "Min |"
              << std::fixed << std::setw(14) << "25% |"
              << std::fixed << std::setw(14) << "50% |"
              << std::fixed << std::setw(14) << "75% |"
              << std::fixed << std::setw(13) << "Max\n";
    ss << "---------------------------------------------------------------------------------------------------------------\n";
    ss << std::scientific << std::setw(12) << std::setprecision(3) <<_population_size << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_MEAN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_STD_DEV] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_MIN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_FIRST_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_SECOND_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_THIRD_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[SUM_NDX_MAX] << "\n";
    ss << "===============================================================================================================\n\n";

    return ss.str();
}





double Population::fitPopulation() {
    double total = 0;
    for (Individual& ndiv : _population) {
        ndiv.setFitness(_fitness_function(ndiv));
        total += ndiv.getFitness();
    }
    return total / _population_size;
}
double Population::fitPopulation(Population& population) {
    double total = 0;
    for (Individual& ndiv : population._population) {
        ndiv.setFitness(population._fitness_function(ndiv));
        total += ndiv.getFitness();
    }
    return total / population._population_size;
}

double Population::populationFitness() {
    double total = 0;
    for (Individual ndiv : _population) total += ndiv.getFitness();
    return total / _population_size;
}
double Population::populationFitness(const Population& pop) {
    double total = 0;
    for (Individual individual : pop._population) total += individual.getFitness();
    return total / pop._population_size;
}
double Population::populationFitness(const std::vector<Individual>& vector) {
    double total = 0;
    for (Individual individual : vector) total += individual.getFitness();
    return total / vector.size();
}


Individual& Population::operator[] (int ndx) {
    return _population[ndx];
}


void Population::sort(bool descending){
    if (descending) std::sort(_population.begin(), _population.end(), std::greater<Individual>());
    else            std::sort(_population.begin(), _population.end(), std::less<Individual>());
}


void Population::clear() {
    _population.clear();
}





std::ostream& operator<<(std::ostream &out, const Population &c) {

    for (int i = 0; i < c._population_size; i++){
        out << std::fixed << std::setw(14) << i
            << std::fixed << std::setw(14) << c._population[i].getReal()
            << std::fixed << std::setw(14) << " + " << c._population[i].getImaginary() << "i ::: "
            << std::fixed << std::setw(14) << c._population[i].getFitness() << std::endl;
    }

    return out;
}






void Population::reset() {
    _combined_population = _population;
    _combined_population_size = _population_size;
}






void Population::integration(const std::vector<Individual>& vector) {
    _combined_population_size += vector.size();
    _combined_population.reserve(_combined_population_size);
    _combined_population.insert(std::end(_combined_population), std::begin(vector), std::end(vector));
}






















