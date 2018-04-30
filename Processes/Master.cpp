//
// Created by tdh5188 on 4/12/18.
//

#include <iomanip>
#include "Master.h"


Master::Master(int pid, std::string pname, Polynomial& polynomial)
        : polynomial(polynomial), finished(false), generation(0), shouldPrintHeader(true) {
    this->pid = pid;
    this->pname = std::move(pname);
    for(int& i : this->buffer) i = 0;
//    this->continue_status_list = std::vector<int>(networkSize);
//    for(int& i : this->continue_status_list) i = -1;
//    this->continue_mutex_list = std::vector<std::mutex>(networkSize);
//    this->continue_cv_list = std::vector<std::condition_variable>(networkSize);
}




void Master::printHeader() {
    //std::lock_guard<std::mutex> lock(mutex_stdout);
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


void Master::printReport(int id, double *summary, std::string note) {
    //std::lock_guard<std::mutex> lock(mutex_stdout);
    std::cout << std::scientific << std::setw(11) << std::setprecision(3) << id << "  | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MEAN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_STD_DEV] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MIN] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_FIRST_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_SECOND_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_THIRD_Q] << " | "
              << std::scientific << std::setw(11) << std::setprecision(3) << summary[Population::SUM_NDX_MAX] << note << "\n";
}



//void Master::processHandler(const int tid) {
//
//    double summary[Population::SUM_SIZE];
//
//    //print first header
//    /*std::lock_guard<std::mutex> lock_header(mutex_printHeader);
//    printHeader();
//    lock_header.~lock_guard();*/
////
////    int id;
////    MPI_Comm_rank(MPI_COMM_WORLD, &id);
////    std::cout << "thread id (" << tid << ") = " << id << std::endl;
//
//
//
//
//    std::complex<double> final;
//
//    int x = NOTHING;
//
//    long l_gen = -1;
//
//    //std::string note = "";
//
//    while(!finished){
//
//        for(int i = 1; i < networkSize; i++){
//            std::unique_lock<std::mutex> lock(continue_mutex_list[i]);
//            continue_cv_list[i].wait(lock, [this, i, l_gen](){ return continue_status_list[i] == l_gen; });
//        }
//
//
//
//        std::cout << "thread " << tid << " looping" << std::endl;
//
//        //increment the generation and print header counter if local counter is the same
//        std::lock_guard<std::mutex> lock_generation(mutex_generation);
//        std::cout << "thread " << tid << " generation = " << generation << " l_gen = " << l_gen << std::endl;
//        if (generation != l_gen) {
//            generation++;
//            printHeader();
//        }
//        lock_generation.~lock_guard();
//
//
//        //allow worker to continue
//        MPI_Send(&x, 1, MPI_INTEGER, tid, 0, MPI_COMM_WORLD);
//
//        std::cout << "thread " << tid << " waiting for response" << std::endl;
//
//
//        //receive status from worker
//        MPI_Recv(&x, 1, MPI_INTEGER, tid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//        std::cout << "thread " << tid << " received a response" << std::endl;
//
//        if(x == COMPLETE || x == COMPLETE_MORE) {
//            std::lock_guard<std::mutex> lock(mutex_finished);
//            finished = true;
//        }
//        if(x == COMPLETE_MORE){
//            MPI_Recv(&final, 1, MPI_DOUBLE_COMPLEX, tid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//        }
//
//        std::cout << "thread " << tid << " waiting for results" << std::endl;
//
//
//        //get summary of progress
//        MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, tid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//
//        std::cout << "thread " << tid << " received results" << std::endl;
//
//        //print summary
//        if (x == CONVERGED) printReport(tid, summary, " ==> Convergence");
//        else printReport(tid, summary);
//
//
//
//        l_gen++;
//    }
//
//    cv_finished.notify_all();
//
//}




int Master::mainProcedure(){
    auto startTime  = std::chrono::high_resolution_clock::now();
    auto epochStart = startTime;

    std::complex<double> migration_buf[MIGRATION_LIMIT];

    std::cout << "starting" << std::endl;

    int code = NOTHING;

    std::complex<double> final;
    int finder = -1;
    double summary[Population::SUM_SIZE];

    printHeader();

    //get initial population stats
    for (int i = 1; i < networkSize; i++){
        MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, i, TAG_SUMMARY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printReport(i, summary," ==> Initial population");
    }

    code = CONTINUE;
    generation++;

    while (true){

        epochStart = std::chrono::system_clock::now();

        //wait for workers to finish their evolutions
        MPI_Barrier(MPI_COMM_WORLD);

        if (finished) code = STOP;

        //allow workers to continue
        MPI_Bcast(&code, 1, MPI_INTEGER, MASTER_PID, MPI_COMM_WORLD);

        if (finished) break;

        printHeader();
//        std::cout << "starting at master" << std::endl;

        for(int wid = 1; wid < networkSize; wid++){

            //allow process wid to send report and other information to master
            MPI_Send(&code, 1, MPI_INTEGER, wid, TAG_QUEUED_UP, MPI_COMM_WORLD);

            //receive status from worker
            MPI_Recv(&code, 1, MPI_INTEGER, wid, TAG_STATUS_UPDATE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(code == COMPLETE || code == COMPLETE_MORE) {
                finished = true;
            }
            if(code == COMPLETE_MORE) {
                MPI_Recv(&final, 1, MPI_DOUBLE_COMPLEX, wid, TAG_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                finder = wid;
            }


            //get summary of progress
            MPI_Recv(summary, Population::SUM_SIZE, MPI_DOUBLE, wid, TAG_SUMMARY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


            //print summary
            if (code == CONVERGED) printReport(wid, summary, " ==> Convergence");
            else printReport(wid, summary);

        }

        std::chrono::duration<double, std::milli> epochDuration = std::chrono::high_resolution_clock::now() - epochStart;

        std::cout << std::defaultfloat << std::setprecision(10)  << "Epoch during: " << 0.001 * epochDuration.count() << "s" << std::endl;


        /*//migration and integration
        for (int i = 1; i < networkSize; i++){
            MPI_Bcast(&migration_buf, MIGRATION_LIMIT, MPI_DOUBLE_COMPLEX, i, MPI_COMM_WORLD);
        }*/

        generation++;
    }

    std::cout << "\n\n";
    std::cout << std::defaultfloat << "f(x) = " << polynomial << "\n\n";

    std::complex<double> finalResults[networkSize];
    for (int i = 1; i < networkSize; i++){
        MPI_Recv(&finalResults[i], 1, MPI_DOUBLE_COMPLEX, i, TAG_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Worker (" << i << ") best result = " << finalResults[i].real() << " + " << finalResults[i].imag() << "i ==> " << std::abs(polynomial(finalResults[i])) << std::endl;
    }


    std::cout << "\n";
    std::cout << std::fixed
              << "f(x) = 0 when x ~= " << final.real() << " + " << final.imag() << "i"
              << " with the fitness of " << std::abs(polynomial(final)) << std::endl;
    std::cout << "found by " << finder << std::endl;

    std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - startTime;

    std::cout << std::fixed << std::setprecision(10) << "time elapsed: " << 0.001 * duration.count() << "s" << std::endl;


}


//int Master::mainProcedure() {
//
//    std::cout << "starting" << std::endl;
//
//    std::thread threads[networkSize];
//
//
//
//    //start threads
//    for(int i = 1; i < networkSize; i++) {
//        std::cout << "starting thread " << i << std::endl;
//        threads[i] = std::thread([this](int id){ this->processHandler(id); }, i);
//    }
//
//
//    //wait until the we found a result
//    std::unique_lock<std::mutex> lock(mutex_finished);
//    while(!finished) cv_finished.wait(lock, [this]() { return finished; });
//
//
//    //join with threads
//    for(int i = 1; i < networkSize; i++) {
//        std::cout << "joining with thread " << i << std::endl;
//        if (threads[i].joinable()) threads[i].join();
//    }
//
//    //print time elapsed
//
//
//
//    return 0;
//}


