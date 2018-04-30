//
// Created by tdh5188 on 4/12/18.
//

#include "Worker.h"
#include "../Local.h"


Worker::Worker(int pid, std::string pname, Polynomial& polynomial, Population& population, unsigned long seed, double start_radius)
        : polynomial(polynomial), population(population), seed(seed), start_radius(start_radius) {
    this->pid = pid;
    this->pname = std::move(pname);
    for(int& i : this->buffer) i = 0;
    rng::seedGenerator(seed);
    for(int i = 0; i < 20; i++){
        rng::getRealNormalDist(0, 10000.0);
    }
}



/*MPI_Send(
        void* data,
        int count,
        MPI_Datatype datatype,
        int destination,
        int tag,
        MPI_Comm communicator)
MPI_Recv(
        void* data,
        int count,
        MPI_Datatype datatype,
        int source,
        int tag,
        MPI_Comm communicator,
        MPI_Status* status)*/
//int Worker::mainProcedure() {
//
//    //initialize population
//    population.init();
//
//    //x is the status code
//    int x = NOTHING;
//
//    bool continueEvolving = true;
//
//    //evolutionary loop
//
//    while (continueEvolving){
//        std::cout << pid << " waiting" << std::endl;
//
//        //wait to start
//        MPI_Recv(&x, MAX_SIZE, MPI_INTEGER, MASTER_PID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//        std::cout << pid << " evolving" << std::endl;
//
//        //evolve population once
//        int status = population.evolve();
//
//        if (status == Population::FOUND) {
//            x = COMPLETE_MORE;
//            continueEvolving = false;
//
//            //send individual that was found
//            MPI_Send(&x, 1, MPI_INTEGER, MASTER_PID, 0, MPI_COMM_WORLD);
//            std::complex<double> found = (std::complex<double>) population.getBestFit();
//            MPI_Send(&found, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, 0, MPI_COMM_WORLD);
//        }
//        else if (status == Population::CONVERGED) {
//            x = CONVERGED;
//            MPI_Send(&x, 1, MPI_INTEGER, MASTER_PID, 0, MPI_COMM_WORLD);
//        }
//        else if (status == Population::NOT_FOUND) {
//            x = CONTINUE;
//            MPI_Send(&x, 1, MPI_INTEGER, MASTER_PID, 0, MPI_COMM_WORLD);
//        }
//        else {
//            std::cout << "Unknown status code reported" << std::endl;
//            x = CONTINUE;
//            MPI_Send(&x, 1, MPI_INTEGER, MASTER_PID, 0, MPI_COMM_WORLD);
//        }
//
//        std::cout << pid << " sending results" << std::endl;
//
//        //get a summary of the population statistics
//        double summary[Population::SUM_SIZE];
//        population.getSummary(summary);
//
//        //send array to master node
//        MPI_Send(summary, Population::SUM_SIZE, MPI_DOUBLE, MASTER_PID, 0, MPI_COMM_WORLD);
//
//
//        //TODO: log progress
//
//        //TODO: integration / migrate
//    }
//
//    const std::complex<double> best = population.getBestFit().getChromosome();
//
//    MPI_Send(&best, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, 0, MPI_COMM_WORLD);
//
//    return 0;
//}






int Worker::mainProcedure() {

    //code is the status code
    int code = NOTHING;
    bool continueEvolving = true;
    double summary[Population::SUM_SIZE];

    std::complex<double> migration_buf[MIGRATION_LIMIT];


    //initialize population
    population.init(start_radius);
    //initial population statistics
    population.getSummary(summary);


    //send statistics to master node
    MPI_Send(summary, Population::SUM_SIZE, MPI_DOUBLE, MASTER_PID, TAG_SUMMARY, MPI_COMM_WORLD);



    //evolutionary loop
    while (true){
//        std::cout << pid << " waiting" << std::endl;

        //wait to start
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&code, 1, MPI_INTEGER, MASTER_PID, MPI_COMM_WORLD);

        if (code == STOP) break;

//        std::cout << pid << " evolving" << std::endl;

        //evolve population once
        int status = population.evolve();

//        std::cout << pid << " queued" << std::endl;

        MPI_Recv(&code, 1, MPI_INTEGER, MASTER_PID, TAG_QUEUED_UP, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (status == Population::FOUND) {
            code = COMPLETE_MORE;
            //continueEvolving = false;

            //send individual that was found
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
            std::complex<double> found = (std::complex<double>) population.getBestFit();
            MPI_Send(&found, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, TAG_FINAL, MPI_COMM_WORLD);
        }
        else if (status == Population::CONVERGED) {
            code = CONVERGED;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }
        else if (status == Population::NOT_FOUND) {
            code = CONTINUE;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }
        else {
            std::cout << "Unknown status code reported" << std::endl;
            code = CONTINUE;
            MPI_Send(&code, 1, MPI_INTEGER, MASTER_PID, TAG_STATUS_UPDATE, MPI_COMM_WORLD);
        }

//        std::cout << pid << " sending results" << std::endl;

        //get a summary of the population statistics
        population.getSummary(summary);

        //send array to master node
        MPI_Send(summary, Population::SUM_SIZE, MPI_DOUBLE, MASTER_PID, TAG_SUMMARY, MPI_COMM_WORLD);


        //Migration and integration
        /*for (int i = 1; i < networkSize; i++){
            //get migration population to send
            if (i == pid){
                population.migration(migration_buf);
            }
            MPI_Bcast(&migration_buf, MIGRATION_LIMIT, MPI_DOUBLE_COMPLEX, i, MPI_COMM_WORLD);
            if (i != pid){
                population.integration(migration_buf);
            }
        }*/

    }

    const std::complex<double> best = population.getBestFit().getChromosome();

    MPI_Send(&best, 1, MPI_DOUBLE_COMPLEX, MASTER_PID, TAG_FINAL, MPI_COMM_WORLD);

    return 0;
}
































