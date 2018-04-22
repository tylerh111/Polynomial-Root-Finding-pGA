//
// Created by tdh5188 on 4/12/18.
//

#include <iomanip>
#include "Master.h"


Master::Master(int pid, std::string pname, Polynomial& polynomial)
        : polynomial(polynomial), finished(false), generation(-1), shouldPrintHeader(true) {
    this->pid = pid;
    this->pname = std::move(pname);
    for(int& i : this->buffer) i = 0;
}



//void Master::onSummaryReceived(double ret[Population::SUM_SIZE]){
//
//}


void Master::printHeader() {
    std::lock_guard<std::mutex> lock(mutex_stdout);
    std::cout << "==============================================================================================================" << std::endl;
    std::cout << "Generation: " << generation << std::endl;
    std::cout << std::fixed << std::setw(14) << "Worker Id |"
              << std::fixed << std::setw(14) << "Mean |"
              << std::fixed << std::setw(14) << "Std Dev |"
              << std::fixed << std::setw(14) << "Min |"
              << std::fixed << std::setw(14) << "25% |"
              << std::fixed << std::setw(14) << "50% |"
              << std::fixed << std::setw(14) << "75% |"
              << std::fixed << std::setw(13) << "Max\n";
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
}


void Master::printReport(int id, double *summary) {
    std::lock_guard<std::mutex> lock(mutex_stdout);
    std::cout << std::scientific << std::setw(11) << std::setprecision(3) << id << "  | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MEAN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_STD_DEV] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MIN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_FIRST_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_SECOND_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_THIRD_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MAX] << "\n";
}



void Master::processHandler(const int tid) {

    double summary[Population::SUM_SIZE];

    //print first header
    std::lock_guard<std::mutex> lock_header(mutex_printHeader);
    printHeader();
    lock_header.~lock_guard();

    std::complex<double> final;

    int x = NOTHING;

    long l_gen = -1;

    while(!finished){

        //increment the generation and print header counter if local counter is the same
        std::lock_guard<std::mutex> lock_generation(mutex_generation);
        if (generation == l_gen) {
            generation++;
            printHeader();
        }
        lock_generation.~lock_guard();


        //allow worker to continue
        MPI_Send(&x, 1, MPI_INTEGER, tid, TAG_CONTINUE, MPI_COMM_WORLD);

        std::cout << "waiting to recv information\n";

        //receive status from worker
        MPI_Recv(&x, 1, MPI_INTEGER, tid, TAG_STATUS_UPDATE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        //TODO: use buffer to check status of worker more extensively
        std::lock_guard<std::mutex> lock_finish(mutex_finished);
        if(x == COMPLETE || x == COMPLETE_MORE) finished = true;
        lock_finish.~lock_guard();

        if(x == COMPLETE_MORE){
            MPI_Recv(&final, 1,MPI_DOUBLE_COMPLEX, tid, TAG_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        std::cout << "waiting to recv summary report\n";

        //get summary of progress
        MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, tid, TAG_SUMMARY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //print summary
        printReport(tid, summary);

        l_gen++;
    }

    cv_finished.notify_all();

}





int Master::mainProcedure() {

    std::cout << "Starting";

    std::thread threads[networkSize];

    //TODO: start time


    //start threads
    for(int i = 1; i < networkSize; i++) {
        std::cout << "starting thread " << i << std::endl;
        threads[i] = std::thread([this](int id){ this->processHandler(id); }, i);
    }


    //wait until the we found a result
    std::unique_lock<std::mutex> lock(mutex_finished);
    while(!finished) cv_finished.wait(lock, [this]() { return finished; });


    //join with threads
    for(int i = 1; i < networkSize; i++) {
        std::cout << "joining with thread " << i << std::endl;
        if (threads[i].joinable()) threads[i].join();
    }

    //TODO: end time
    //print time elapsed



    return 0;
}


