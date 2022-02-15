/**
 * @file RadioNuclide.cpp
 * @brief RadioNuclide class implementation
 */

#include <math.h>

#include "../headers/RadioNuclide.h"

/**
 * @brief Construct a new Radio Nuclide object
 * 
 * @param lambda decay constant
 * @param A current activity
 */
RadioNuclide::RadioNuclide(double lambda, double N_0){
    this->A_0 = N_0*lambda;
    this->lambda = lambda;
    this->N_0 = N_0;
    tau = 1./lambda;
};

// Getters
double RadioNuclide::getLambda(){
    return lambda;
};

double RadioNuclide::getN(double t){
    return N_0*exp(-lambda*t);
};

double RadioNuclide::getA(double t){
    return lambda*getN(t);
};
double RadioNuclide::getTau(){
    return tau;
};
double RadioNuclide::getConfidency(){
    return confidency;
};

double RadioNuclide::getElapsedTime(){
    return elapsedTime;
};

// Setters

/**
 * Return simulation passed time
 * 
 * @param dt Time increment
 */
void RadioNuclide::setElapsedTime(double dt){
    elapsedTime += dt; // Incrementing elapsed timer counter
};

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide initial activity is: " << A_0 << " with decay constant " << lambda << " and half life " << tau << endl;
}

 