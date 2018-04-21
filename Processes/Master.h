//
// Created by tdh5188 on 4/12/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H
#define POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H

#include <thread>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include "Process.h"
#include "../World/Polynomial.h"
#include "../World/Population.h"

class Master : public Process {
private:
    Polynomial& polynomial;
    bool finished;

    int generation;
    bool shouldPrintHeader;

    std::mutex mutex_finished;
    std::mutex mutex_generation;
    std::mutex mutex_stdout;
    std::mutex mutex_printHeader;

    std::condition_variable cv_finished;


public:
    Master(int pid, std::string pname, Polynomial& polynomial);

    ~Master() override = default;

    void processHandler(int tid);

    void printHeader();
    void printReport(int id, double summary[Population::SUM_SIZE]);

    //void onSummaryReceived(double ret[Population::SUM_SIZE]);

    int mainProcedure() override;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H
