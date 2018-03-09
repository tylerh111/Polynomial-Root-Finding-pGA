
#include <algorithm>
#include <complex>
#include <exception>
#include <cmath>
#include <ctime>
#include <iomanip>
#include "Population.h"






Population::Population(int pop_size) {
    if (pop_size < 3) throw _pop_size_ex;

    this->_population_size = pop_size;
    this->_mutation_rate = 0;
    this->_alpha_radius = 0;
    this->_generation = 0;

    this->_fitness_function = nullptr;

    this->_population = std::vector<Individual>((unsigned long) pop_size);

}



Population::Population(int pop_size, double mut_rate, double mut_radius, double start_radius, std::function<double(Individual&)>& function){
    if (pop_size < 3) throw _pop_size_ex;

    this->_population_size = pop_size;
    this->_mutation_rate = mut_rate;
    this->_alpha_radius = mut_radius;
    this->_generation = 0;

    this->_fitness_function = function;

    //this->_rng.seed((unsigned long) std::time(nullptr));

    this->_population = std::vector<Individual>((unsigned long) pop_size);
    init(start_radius);
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





void Population::init(double start_radius){
    double offset = start_radius - (-start_radius);
    std::mt19937_64 gen;
    gen.seed((unsigned long) std::time(nullptr));
    std::uniform_real_distribution<double> distribution(0.0, offset);

    for (int i = 0; i < _population_size; i++) {
        //_population[i] = Individual(distribution(_rng) - offset, distribution(_rng) - offset);
        _population[i] = Individual(distribution(gen) - offset, distribution(gen) - offset);
        _population[i].setFitness(_fitness_function(_population[i]));
    }
}











void Population::select(Individual* parents[2]) {
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



    std::default_random_engine _rng;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    //for each parent (2)
    for (int ndx = 0; ndx < 2; ndx++) {
        //do while the parents are equal to each other
        do {
            //get random number [0, 1] from uniform distribution to pick from population
            //double num = get_rand(0.0, 1.0);
            double num = distribution(_rng);

            //pick from population by setting i where num <= accumulated[i]
            int i = 0;
            while (i < POP_CAP && num > accumulated[i++]) { /*do nothing*/ }

            ret[ndx] = &_population[--i];
        } while (ret[ndx] == not_me);

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
    std::complex<double> p2 = parents[0]->getChromosome();

    std::complex<double> diff = p1 - p2;
    double abs_diff = std::abs(diff);

    double real_lower = std::min(p1.real(), p2.real()) - abs_diff;
    double real_upper = std::max(p1.real(), p2.real()) + abs_diff;
    double imag_lower = std::min(p1.imag(), p2.imag()) - abs_diff;
    double imag_upper = std::max(p1.imag(), p2.imag()) + abs_diff;

    std::mt19937_64 gen;
    gen.seed((unsigned long) std::time(nullptr));

    std::uniform_real_distribution<double> dist_real(real_lower, real_upper);
    std::uniform_real_distribution<double> dist_imag(imag_lower, imag_upper);


    //geofencing
    //ret[0] = Individual(dist_real(_rng), dist_imag(_rng));
    //ret[1] = Individual(dist_real(_rng), dist_imag(_rng));
    ret[0] = Individual(dist_real(gen), dist_imag(gen));
    ret[1] = Individual(dist_real(gen), dist_imag(gen));

    ret[0].setFitness(_fitness_function(ret[0]));
    ret[1].setFitness(_fitness_function(ret[1]));


    //ellipse casing fence
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

    std::mt19937_64 gen;
    gen.seed((unsigned long) std::time(nullptr));
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    if (distribution(gen) <= _mutation_rate) {
        double new_real;
        double new_imag;

        std::uniform_real_distribution<double> dist_complex(0.0, _alpha_radius);

        //pick numbers within range
        do {
            new_real = dist_complex(gen);
            new_imag = dist_complex(gen);
        } while (std::pow(new_real, 2) + std::pow(new_imag, 2) <= std::pow(_alpha_radius, 2));



        x.setReal(new_real + x.getReal());
        x.setImaginary(new_imag + x.getImaginary());
    }

}


void Population::replace(Population &replacement){
    int i = replacement._population_size - 1;
    int j = 0;

    while (j < replacement._population_size){
        _population[i++] = replacement[j++];
    }
}




bool Population::checkSolution() const {
    for (int i = 0; i < _population_size; i++)
        if (_population[i].getFitness() == 0) return true;
    return false;
}


bool Population::checkConvergence() const {
    double recur_real = _population[0].getReal();
    double recur_imag = _population[0].getImaginary();

    for (int i = 0; i < _population_size; i++){
        if (recur_real != _population[i].getReal() &&
            recur_imag != _population[i].getImaginary())
            return false;
    }

    return true;
}



void Population::handleConvergence() {

    std::mt19937_64 gen;
    gen.seed((unsigned long) std::time(nullptr));
    std::uniform_real_distribution<double> dist_complex(0.0, _alpha_radius);

    for (int i = 0; i < _population_size; i++) {
        double new_real;
        double new_imag;

        //pick numbers within range
        do {
            new_real = dist_complex(gen);
            new_imag = dist_complex(gen);
        } while (std::pow(new_real, 2) + std::pow(new_imag, 2) <= std::pow(_alpha_radius, 2));


        _population[i].setReal(new_real + _population[i].getReal());
        _population[i].setImaginary(new_imag + _population[i].getImaginary());
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

    for (int gen = starting_gen; gen < generations; gen++){

        //sort population
        sort();

        //print summary
        if ((gen < 10) ||
            ((gen < 100) && (gen % 10 == 0)) ||
            ((gen < 1000) && (gen % 100 == 0)) ||
            ((gen < 10000) && (gen % 1000 == 0)) ||
            (gen % 10000 == 0))
            summary();

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
        int new_pop_size = _population_size / 3;
        if (new_pop_size % 2 != 0) new_pop_size++;

        Population new_generation(new_pop_size);

        for (int i = 0; i < (_population_size / 3); i+=2) {

            Individual* parents[2];
            select(parents);

            Individual offspring[2];
            crossover(offspring, parents);

            mutate(offspring[0]);
            mutate(offspring[1]);

            new_generation._population.at((unsigned long) i)     = offspring[0];
            new_generation._population.at((unsigned long) i + 1) = offspring[1];

        }

        replace(new_generation);

        _generation++;
    }

}





void Population::summary() const {

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


    std::cout << "GENERATION :: " << _generation << std::endl;
    std::cout << std::fixed << std::setw(14) << "Count |"
              << std::fixed << std::setw(14) << "Mean |"
              << std::fixed << std::setw(14) << "Std Dev |"
              << std::fixed << std::setw(14) << "Min |"
              << std::fixed << std::setw(14) << "25% |"
              << std::fixed << std::setw(14) << "50% |"
              << std::fixed << std::setw(14) << "75% |"
              << std::fixed << std::setw(13) << "Max\n";
    std::cout << "---------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::fixed << std::setw(12) << std::setprecision(3) <<_population_size << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << mean << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << std_dev<< " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << _population[0].getFitness() << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << fir_q << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << sec_q << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << thr_q << " | "
              << std::fixed << std::setw(11) << std::setprecision(3) << _population[_population_size-1].getFitness() << "\n";
    std::cout << "===============================================================================================================\n\n";

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
