#ifndef POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
#define POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H


#include <iostream>
#include <cstdlib>
#include <random>
#include <limits>
#include <ctime>

/*	int get_rand(low_bound = 0, up_bound = 0, is_low_incl = true, is_up_incl = false)
 *	get_rand provides a random number between range provided
 *		@param low_bound := lower bound for random number
 *		@param up_bound  := upper bound for random number
 *		@param is_low_incl := inclusive lower bound if true
 *		@param is_up_incl  := inclusive upper bound if true
 *
 *		@return random number
 *
 *	if up_bound == 0: random number between [0, RAND_MAX] is returned
 */
/*
int get_rand(int low_bound = 0, int up_bound = 0, bool is_low_incl = true, bool is_up_incl = false) {
    int ret = 0;

    if (up_bound != 0) {
        int bounds = ((int) is_up_incl << 1) | (int)is_low_incl;

        //calculate and return random values with specified boundaries
        switch (bounds) {
            case 0: ret = std::rand() % (up_bound - low_bound - 1) + low_bound + 1;	break;	//non inclusive
            case 1: ret = std::rand() % (up_bound - low_bound) + low_bound;			break;	//lower bound inclusive
            case 2: ret = std::rand() % (up_bound - low_bound) + low_bound + 1;		break;	//upper bound inclusvie
            case 3: ret = std::rand() % (up_bound - low_bound + 1) + low_bound;		break;	//inclusive

            default: std::cout << "not possible!!!!" << std::endl;				//error
        }
    }
    else
        ret = std::rand();
    return ret;
}
*/




double get_rand(double lower_bound = 0.0, double upper_bound = std::numeric_limits<double>::max()){

    std::default_random_engine generator;
    //generator.seed((unsigned long) std::time(nullptr));
    double offset = upper_bound - lower_bound;
    std::uniform_real_distribution<double> distribution(0.0, offset);

    return distribution(generator) - offset;
}













#endif //POLYNOMIAL_ROOT_FINDING_PGA_LOCAL_H
