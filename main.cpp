
#include <iostream>
#include <cmath>
#include <random>

#include "mpi.h"

#include "World.h"
#include "Local.h"

#include "Processes/Process.h"
#include "Processes/Master.h"
#include "Processes/Worker.h"


/**
 * argv is formatted as: <executable> cn ... c1 c0
 *   where ci is a coefficient for degree i
 *   (ex: mpirun -host localhost ./<executable> 2 0 1
 *          this polynomial will be 2*x^2 + 0*x^1 + 1*x^0)
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {




    //mpi initialization
    int ierr, pid, size;
    char pname[MPI_MAX_PROCESSOR_NAME];

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    Process::setNetworkSize(size);

    int pnameLen;
    MPI_Get_processor_name(pname, &pnameLen);

    Process::NetworkSizeException e = Process::NetworkSizeException();

    //size must be 2 or more
    if (size < 2) throw Process::NetworkSizeException();
    //if we are the master node
    if (pid == Process::MASTER_PID){
        std::cout << "master process" << std::endl;
        std::cout << "pname = " << pname << std::endl;
        Master master (pid, pname);
    }
    else{
        //auto polynomial = mPolynomial::getPolynomial(argc - 2, argv);
        std::cout << "server process" << std::endl;
        Worker worker(pid, pname);
    }






/*
    std::cout << "hello world from Polynomial-Root-Finding-pGA" << std::endl;

    mRandom::seedGenerator(0);

    std::function<double(Individual&)> fitFunct = [](Individual& indv){
        std::complex<double> z = indv.getChromosome();
        return std::abs(
                std::pow(z, 2) + 1.0
                // (z.real() == 0 && z.imag == 0) ? 1.0 : std::pow(z, z)
                // 3.1415 * std::pow(z, 16)
                // + 12.0 * std::pow(z, 10)
                // + 2.0 * std::pow(z, 5)
                // + 2.0 * std::pow(z, 4)
                // + 3.0 * std::pow(z, 3)
                // + 4.0 * std::pow(z, 2)
                // + 2.0 * z
                // + 2.0
        );
    };

    std::cout << "Creating population\n";
    Population mPop(2500, 0.25, 10, 2500, 1e-3,fitFunct);

    std::cout << "Finished creating population\n";
    std::cout << "Fitting the population based on fitness function\n";
    mPop.sort();

*/
/*
    std::cout << "\nDisplaying all Individual's fitness\n";
    for (int i = 0; i < mPop.getPopulationSize(); i++){
        std::cout << "Individual " << i << ": (" << mPop[i] << ") = " << mPop[i].getFitness() << std::endl;
    }
*//*

    std::cout << "\nPopulation's total fitness = " << mPop.populationFitness() << std::endl;


    std::cout << "\n\n\t::EVOLVING::\n\n";
    //mPop.evolve();
    //mPop.summary();
*/



    ierr = MPI_Finalize();

    return 0;
}
