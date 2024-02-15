#include "Basic.h" // Include the header file "Basic.h"

// Definition of the function myRand, which generates a random number between 0 and 1
double myRand()
{
    // Using rand() to generate a random integer between 0 and RAND_MAX, and then converting it to a double between 0 and 1
    return double(double(rand()) / RAND_MAX);
}
