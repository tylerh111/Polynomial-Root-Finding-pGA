
#include <iostream>
#include <cmath>
#include <random>

#include "mpi.h"

#include "World.h"
#include "Local.h"

#include "Processes/Process.h"
#include "Processes/Master.h"
#include "Processes/Worker.h"




void printPopulationInfo(const Polynomial& polynomial,
                         const unsigned long populationSize,
                         const double acceptedError,
                         const double mutationRate,
                         const double mutationRadius,
                         const double startingRadius){

    std::cout << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout <<   "polynomial      = " << polynomial
              << "\npopulation size = " << populationSize
              << "\naccepted error  = " << acceptedError
              << "\nmutation rate   = " << mutationRate
              << "\nmutation radius = " << mutationRadius
              << "\nstarting radius = " << startingRadius << std::endl;
    std::cout << "------------------------------------------------\n\n";

}






/**
 * argv is formatted as:
 * <executable>    <polynomial-file.txt>
 *              [-p]  --population-size  <pop size>
 *              [-ae] --accepted-error   <accepted error>
 *              [-mr] --mutation-rate    <mutation rate>
 *              [-ms] --mutation-radius  <mutation radius>
 *              [-ss] --starting-radius  <starting radius>
 *              [-l]  --load-information <population-info-file.txt>
 *
 *   where ci is a coefficient for degree i
 *   (ex: mpirun -host localhost ./<executable> 2 0 1
 *          this polynomial will be 2*x^2 + 0*x^1 + 1*x^0)
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {

    //print argv
    //int i = -1;
    //while(argv[++i] != nullptr) std::cout << "argv[" << i << "] = " << argv[i] << std::endl;


    //testing polynomial
    /*Polynomial f = Polynomial(argv[1]);
    Individual test = Individual(10, 2);
    auto fit_func  = mFitnessFunctions::makeAbsoluteValue(f);

    std::cout << "f(x) = " << f.to_string() << std::endl;
    std::cout << "test = " << test << std::endl;
    std::cout << "f(test) = " << f(test.getChromosome()) << std::endl;
    std::cout << "|f(test)| = " << fit_func(test) << std::endl;*/

    //mpi initialization
    int ierr, pid, size;
    char pname[MPI_MAX_PROCESSOR_NAME];

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

//    if (pid == 0) {
//        for (int i = 0; i < size; i++){
//            std::cout << "rank: " << i << std::endl;
//        }
//    }

    Process::setNetworkSize(size);

    //size must be 2 or more
    if (size < 2) throw Process::NetworkSizeException();


    //retrieving information
    Polynomial polynomial = Polynomial(argv[1]);
    unsigned long populationSize = Population::DEF_POPULATION_SIZE;
    double acceptedError         = Population::DEF_ACCEPTED_ERROR;
    double mutationRate          = Population::DEF_MUTATION_RATE;
    double mutationRadius        = Population::DEF_MUTATION_RADIUS;
    double startingRadius        = Population::DEF_START_RADIUS;

    try {
        mInformationRetrieval::getInformation(argc, argv,
                                              populationSize,
                                              acceptedError,
                                              mutationRate, mutationRadius,
                                              startingRadius);
    }
    catch(std::exception &e){
        std::cout << "Parsing error" << std::endl;
        exit(11);
    }



    //setting up processes
    int pname_len;
    MPI_Get_processor_name(pname, &pname_len);


    //setting up seeds
    //unsigned long seeds[size];
    //for (unsigned long i = 0; i < size; i++) seeds[i] = i;

    auto seed = (unsigned long) pid;

    //if we are the master node
    if (pid == Process::MASTER_PID){
        std::cout << "Master process: " << pname << std::endl;


        MPI_Barrier(MPI_COMM_WORLD); //master printed info
        MPI_Barrier(MPI_COMM_WORLD); //wait for everyone else to print

        printPopulationInfo(polynomial, populationSize, acceptedError,mutationRate, mutationRadius, startingRadius);

        MPI_Barrier(MPI_COMM_WORLD); //master printed population info

        Master master(pid, pname, polynomial);
        master.mainProcedure();

    }
    else{
        MPI_Barrier(MPI_COMM_WORLD); //wait for master to print
        std::cout << "Worker process (" << pid << "): " << pname << std::endl;

        MPI_Barrier(MPI_COMM_WORLD); //everyone else printed
        MPI_Barrier(MPI_COMM_WORLD); //wait for master to print population info


        auto fitnessFunction = mFitnessFunctions::makeAbsoluteValue(polynomial);

        Population population = Population(fitnessFunction,
                                           polynomial,
                                           populationSize,
                                           acceptedError,
                                           mutationRate,
                                           mutationRadius);


        Worker worker(pid, pname, polynomial, population, seed);
        worker.mainProcedure();
    }



    ierr = MPI_Finalize();

    return 0;
}
