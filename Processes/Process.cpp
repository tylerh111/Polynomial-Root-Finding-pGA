//
// Created by tdh5188 on 4/12/18.
//

#include "Process.h"

#include <utility>

Process::Process(int pid, std::string pname) {
    this->pid = pid;
    this->pname = std::move(pname);
}


int Process::networkSize = -1;

void Process::setNetworkSize(int networkSize){
    Process::networkSize = networkSize;
}

