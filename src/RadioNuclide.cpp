/**
 * @file RadioNuclide.cpp
 * @brief RadioNuclide class implementation
 */

#include <math.h>
#include <random>

#include "../headers/RadioNuclide.h"


RadioNuclide::RadioNuclide(){
    LOG_F(WARNING, "Called RadioNuclide default constructor!");
};
RadioNuclide::~RadioNuclide(){
    LOG_F(INFO, "Destroying RadioNuclide 0x%p", this);
};
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
    LOG_F(INFO, "Constructing at: %p",(void*) this);

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

RadioNuclide::RadioNuclide(const RadioNuclide& src){
    LOG_F(INFO, "Copying %p in new object", (void*) &src);
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

double* RadioNuclide::sample(){ 

    // polar and azhimutal angles
    double theta1 = ((double) rand()/RAND_MAX) * 2 * M_PI;
    double theta2 = ((double) rand()/RAND_MAX) * 2 * M_PI + M_PI;

    double phi1  = ((double) rand()/RAND_MAX) * M_PI;
    double phi2  = ((double) rand()/RAND_MAX) * M_PI + M_PI; // POSSIBILE FONTE DI ERRORE SE phi1 e phi2 non sono compresi tra +180 e -180

    return (new double[4]{theta1, theta2, phi1, phi2}); // {theta, phi1, phi2}
};

// Setters

/**
 * Return simulation passed time
 * 
 * @param dt Time increment
 */
void RadioNuclide::addElapsedTime(double dt){
    this->elapsedTime += dt; // Incrementing elapsed timer counter
};

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide initial activity is: " << A_0 << " with decay constant " << lambda << " and half life " << tau << endl;
}

 