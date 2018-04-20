//
// Created by tdh5188 on 4/12/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H
#define POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H


#include "Process.h"
#include "../World/Polynomial.h"

class Master : public Process {
    Polynomial p;


public:
    Master(int pid, std::string pname) : Process(pid, std::move(pname)) { }
    ~Master() override = default;

    int mainProcedure() override;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_CLIENT_H
