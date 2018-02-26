
#include "Population.h"
#include "../Local.h"




Population::Population(int pop_size, double mutation_rate, std::function<double(Individual&)> funct) {
    this->_population_size = pop_size;
    this->_mutation_rate = mutation_rate;
    this->_fitness_function = funct;

    this->_population = new Individual*[this->_population_size];


    for (int i = 0; i < pop_size; i++)
        _population[i] = new Individual(1, 1);


}


Population::~Population() {
    //delete each individual
    for (int i = 0; i < _population_size; i++)
        delete _population[i];
    //delete popuation
    delete[] _population;
}



/*
Individual Population::crossover(Individual& x1, Individual &x2)[2] {
    Individual arr[2] = {x1, x2};
    return (Individual[2])(arr);
}
*/




void Population::fitPopulation(){
    for (int i = 0; i < _population_size; i++){
        _population[i]->setFitness(_fitness_function(*_population[i]));
    }
}




Individual& Population::operator[] (int ndx){
    return *_population[ndx];
}


