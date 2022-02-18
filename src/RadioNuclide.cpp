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

    // Changing log scope
    LOG_SCOPE_F(INFO, "RADIO NUCLIDE");
    

    // Logging construction
    LOG_F(INFO, "Constructing at: 0x%x", this);

    // Logging Activity
    this->A_0 = N_0*lambda;
    LOG_F(INFO, "Activity: %f", this->A_0);

    // Logging decay constant
    this->lambda = lambda;
    LOG_F(INFO, "lambda: %f", this->lambda);

    // Logging N
    this->N_0 = N_0;
    LOG_F(INFO, "N_0: %f", this->N_0);

    // Logging tau
    tau = 1./lambda;
    LOG_F(INFO, "tau: %f", this->tau);
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
void RadioNuclide::addElapsedTime(double dt){
    elapsedTime += dt; // Incrementing elapsed timer counter
};

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide initial activity is: " << A_0 << " with decay constant " << lambda << " and half life " << tau << endl;
}

 