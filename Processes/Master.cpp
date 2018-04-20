//
// Created by tdh5188 on 4/12/18.
//

#include "Master.h"



Master::Master(int pid, std::string pname, Polynomial polynomial) : Process(pid, pname){




}



int Master::mainProcedure() {

    std::cout << "Starting ";

    //while(true){
    for (int i = 0; i < 100; i++){

        //send to all processes to evolve


        //wait for results from all

        //log them


        //display results

    }






    return 0;
}




