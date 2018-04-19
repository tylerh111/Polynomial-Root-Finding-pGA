//
// Created by tdh5188 on 4/12/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
#define POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H


#include "Process.h"

class Worker : public Process {
public:
    Worker(int pid, std::string pname) : Process(pid, std::move(pname)) { }
    ~Worker() override = default;

    int mainProcedure() override;
};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
