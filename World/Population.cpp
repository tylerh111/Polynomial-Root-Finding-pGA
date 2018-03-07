
#include <algorithm>
#include <complex>
#include <exception>
#include <cmath>
#include <langinfo.h>
#include "Population.h"
#include "../Local.h"






Population::Population(int pop_size) {
    if (pop_size < 3) throw _pop_size_ex;

    this->_population_size = pop_size;
    this->_mutation_rate = 0;
    this->_alpha_radius = 0;
    this->_generation = 0;

    this->_fitness_function = nullptr;

    this->_population = std::vector<Individual>((unsigned long) pop_size);

}



Population::Population(int pop_size, double mut_rate, double radius, std::function<double(Individual&)>& function){
    if (pop_size < 3) throw _pop_size_ex;

    this->_population_size = pop_size;
    this->_mutation_rate = mut_rate;
    this->_alpha_radius = radius;
    this->_generation = 0;

    this->_fitness_function = function;

    this->_population = std::vector<Individual>((unsigned long) pop_size);


    std::default_random_engine generator;
    generator.seed((unsigned long) std::time(nullptr));
    double offset = 10 - (-10);
    std::uniform_real_distribution<double> distribution(0.0, offset);

    for (int i = 0; i < this->_population_size; i++) {
        _population[i] = Individual(distribution(generator) - offset, distribution(generator) - offset);
        double temp = _fitness_function(_population[i]);
        _population[i].setFitness(_fitness_function(_population[i]));
    }
}



Population::Population(const Population& that) {
    this->_population_size = that._population_size;
    this->_mutation_rate = that._mutation_rate;
    this->_alpha_radius = that._alpha_radius;
    this->_generation = that._generation;

    this->_fitness_function = that._fitness_function;


    this->clear();
    this->_population = std::vector<Individual>((unsigned long) that._population_size);

    for (int i = 0; i < this->_population_size; i++)
        _population[i] = Individual(that._population[i]);
}






void Population::select(Individual parents[2]) {
    Individual* ret[2];	          //ret    := used to hold ret values until end to assign parents
    Individual* not_me = nullptr; //not_me := used to avoid selecting same member twice

    int POP_CAP = _population_size;

    //total of all members' fitness
    double totalFitness = populationFitness();

    //probability and accumulated are used to pick from the population using a uniform distribution
    //ex: probability: { 0.5, 0.3, 0.2 }
    //	  accumulated: { 0.5, 0.8, 1.0 } <-- pick based on this array
    auto probability = new double[POP_CAP];	//probability[i] = fitness(i) / total_i
    auto accumulated = new double[POP_CAP];	//accumulated[i] = accumulated[i - 1] + probability[i]

    //calculating probability
    for (int i = 0; i < POP_CAP; i++) probability[i] = _population[i].getFitness() / totalFitness;

    //calculating accumulated
    accumulated[0] = probability[0];
    accumulated[POP_CAP - 1] = 1;
    for (int i = 1; i < POP_CAP - 1; i++) accumulated[i] = accumulated[i - 1] + probability[i];



    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    //for each parent (2)
    for (int ndx = 0; ndx < 2; ndx++) {
        //do while the parents are equal to each other
        do {
            //get random number [0, 1] from uniform distribution to pick from population
            //double num = get_rand(0.0, 1.0);
            double num = distribution(generator);

            //pick from population by setting i where num <= accumulated[i]
            int i = 0;
            while (i < POP_CAP && num > accumulated[i++]) { /*do nothing*/ }

            ret[ndx] = &_population[--i];
        } while (ret[ndx] == not_me);

        //to make sure parents are not equal
        not_me = ret[0];
    }

    //copy parents for higher scoped function
    parents[0] = *ret[0];
    parents[1] = *ret[1];


    //deleting
    delete ret[0];
    delete ret[1];
    //delete[] ret;

    delete not_me;

    delete[] probability;
    delete[] accumulated;
}






void Population::crossover(Individual offspring[2], Individual& p1, Individual &p2) {

    Individual ret[2];

    //geofencing
    ret[0] = Individual(get_rand(p1.getReal(), p2.getReal()),
                        get_rand(p1.getImaginary(), p2.getImaginary()));

    ret[1] = Individual(get_rand(p1.getReal(), p2.getReal()),
                        get_rand(p1.getImaginary(), p2.getImaginary()));


    /*for (int i = 0; i < 2; i++) {
        bool not_in_range = true;
        while (not_in_range) {
            ret[i] = Individual(get_rand(p1.getReal(), p2.getReal()),
                                get_rand(p1.getImaginary(), p2.getImaginary()));

            double d1 = (ret[i] - p1).getAbsoluteValue();
            double d2 = (ret[i] - p2).getAbsoluteValue();

            not_in_range = (d1 <= 2 * _major_axis) && (d2 <= 2 * _minor_axis);
        }
    }*/

    offspring[0] = ret[0];
    offspring[1] = ret[1];

}


void Population::mutate(Individual &x){

    if (get_rand(0, 1) <= _mutation_rate) {
        double new_real;
        double new_imag;

        do {
            new_real = get_rand(0, _alpha_radius) + x.getReal();
            new_imag = get_rand(0, _alpha_radius) + x.getImaginary();
        } while (std::pow(new_real, 2) + std::pow(new_imag, 2) <= std::pow(_alpha_radius, 2));

        x.setReal(new_real);
        x.setImaginary(new_imag);
    }
}


void Population::replace(Population &replacement){
    int i = replacement._population_size - 1;
    int j = 0;

    while (j < replacement._population_size){
        _population[i++] = replacement[j++];
    }
}






void Population::add(Individual &x) {
    _population.push_back(x);
}

void Population::remove(Individual &x) {
    int i = 0;
    while (i < _population_size && _population[i++] == x) { /* do nothing */ }
    _population.erase(_population.begin() + --i);
}





void Population::evolve(int generations, int starting_gen) {

    //0: count
    //1: mean
    //2: standard deviation
    //3: min
    //4: 1st quartile
    //5: median
    //6: 3rd quartile
    //7: max
    double old_gen_info[8];
    double new_gen_info[8];


    for (int gen = starting_gen; gen < generations; gen++){

        //sort population
        sort();

        //new generation
        int new_pop_size = _population_size / 3;
        if (new_pop_size % 2 != 0) new_pop_size++;

        Population new_generation(new_pop_size);

        for (int i = 0; i < (_population_size / 3); i+=2) {

            Individual parents[2];
            select(parents);

            Individual offspring[2];
            crossover(offspring, parents[0], parents[1]);

            mutate(offspring[0]);
            mutate(offspring[1]);

            new_generation._population.at((unsigned long) i)     = offspring[0];
            new_generation._population.at((unsigned long) i + 1) = offspring[1];

        }

        //get information from previous generation
        getInformation(old_gen_info);

        replace(new_generation);

        //get new information for new generation
        getInformation(new_gen_info);

        //print summary
        summaryGeneration(gen, old_gen_info, new_gen_info);

        _generation++;
    }

}


//0: count
//1: mean
//2: standard deviation
//3: min
//4: 1st quartile
//5: median
//6: 3rd quartile
//7: max
void Population::getInformation(double arr[8]) {
    sort();

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


    arr[0] = _population_size;
    arr[1] = mean;
    arr[2] = std_dev;
    arr[3] = _population[0].getFitness();
    arr[4] = fir_q;
    arr[5] = sec_q;
    arr[6] = thr_q;
    arr[7] = _population[_population_size - 1].getFitness();
}





void Population::summaryGeneration(int prev_gen_no, double old_gen[8], double new_gen[8]) {
    std::cout << "        | Generation " << prev_gen_no << " | Generation " << prev_gen_no + 1 << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Count   | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "Mean    | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "Std Dev | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "Min     | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "25%     | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "50%     | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "75%     | " << old_gen[0] << " | " << new_gen[0] << std::endl;
    std::cout << "Max     | " << old_gen[0] << " | " << new_gen[0] << std::endl;



}




double Population::fitPopulation() {
    double total = 0;
    for (Individual& ndiv : _population) {
        ndiv.setFitness(_fitness_function(ndiv));
        total += ndiv.getFitness();
    }
    return total / _population_size;
}


double Population::populationFitness() {
    double total = 0;
    for (Individual ndiv : _population) total += ndiv.getFitness();
    return total / _population_size;
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
