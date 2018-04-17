//
// Created by tdh5188 on 4/12/18.
//

#ifndef POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H
#define POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H

#include <mpi.h>

//Abstract Class
class Process {
protected:
    int pid;
    std::string pname;



public:
    static int networkSize;
    const static int CLIENT_PID = 0;

    Process() : pid(0), pname("") { };
    Process(int pid, std::string pname);

    virtual ~Process() = default;

    inline const int getPId()           const { return pid; }
    inline const std::string getPName() const { return pname; }
    inline const int getNetworkSize()   const { return Process::networkSize; }

    static void setNetworkSize(int networkSize);

    virtual int mainProcedure() = 0;

};


#endif //POLYNOMIAL_ROOT_FINDING_PGA_PROCESS_H
