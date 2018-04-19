
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

    Polynomial f = Polynomial(argv[1]);
    Individual test = Individual(10, 2);
    auto fit_func  = mFitnessFunctions::makeAbsoluteValue(f);

    std::cout << "f(x) = " << f.to_string() << std::endl;
    std::cout << "test = " << test << std::endl;
    std::cout << "f(test) = " << f(test.getChromosome()) << std::endl;
    std::cout << "|f(test)| = " << fit_func(test) << std::endl;



    //mpi initialization
    int ierr, pid, size;
    char pname[MPI_MAX_PROCESSOR_NAME];

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    Process::setNetworkSize(size);

    //size must be 2 or more
    if (size < 2) throw Process::NetworkSizeException();


    //retrieving information
    Polynomial polynomial = Polynomial(argv[1]);


    //if (argc <)



    //setting up processes
    int pname_len;
    MPI_Get_processor_name(pname, &pname_len);

    //if we are the master node
    if (pid == Process::MASTER_PID){
        std::cout << "master process" << std::endl;
        std::cout << "pname = " << pname << std::endl;
        Master master(pid, pname);
    }
    else{

        std::cout << "server process" << std::endl;
        Worker worker(pid, pname);
    }



    ierr = MPI_Finalize();

    return 0;
}
