//
// Created by tdh5188 on 4/12/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
#define POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H


#include "Process.h"
#include "../World/Polynomial.h"
#include "../World/Population.h"

class Worker : public Process {
private:
    Polynomial& polynomial;
    Population& population;
    unsigned long seed;

public:
    Worker() = default;
    Worker(int pid, std::string pname, Polynomial& polynomial, Population& population, unsigned long seed);
    ~Worker() override = default;

    int mainProcedure() override;
};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_SERVER_H
