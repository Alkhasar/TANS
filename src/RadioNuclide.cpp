/**
 * @file RadioNuclide.cpp
 * @brief RadioNuclide class implementation
 */

#include "../headers/RadioNuclide.h"

/**
 * @brief Construct a new Radio Nuclide object
 * 
 * @param lambda decay constant
 * @param A current activity
 */
RadioNuclide::RadioNuclide(double lambda, double A){
    this->A = A;
    this->lambda = lambda;
    tau = 1./lambda;
};

// Getters
double RadioNuclide::getLambda(){
    return lambda;
};
double RadioNuclide::getA(){
    return A;
};
double RadioNuclide::getTau(){
    return tau;
};
double RadioNuclide::getConfidency(){
    return confidency;
};

/**
 * Takes ans sets the new activity value
 * 
 * @param A new activity value
 */
void RadioNuclide::setA(double A){
    this->A = A;    // Sets the new activity
}

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide activity is: " << A << " with decay constant " << lambda << " and half life " << tau << endl;
}

 