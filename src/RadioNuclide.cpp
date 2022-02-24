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
    double omega1 = ((double) rand()/RAND_MAX) * M_PI; // Omega 1 from 0 to pi
    // double omega2 = ((double) rand()/RAND_MAX) * M_PI + M_PI; // Omega 2 from pi to 2pi

    double theta1  = -M_PI_2 + ((double) rand()/RAND_MAX) * M_PI_2; // theta 1 from -pi/2 to pi/2
    // double theta2  = -M_PI_2 + ((double) rand()/RAND_MAX) * M_PI_2; // POSSIBILE FONTE DI ERRORE SE phi1 e phi2 non sono compresi tra +180 e -180

    return (new double[4]{omega1, omega1 - M_PI, theta1, -theta1}); // {theta, phi1, phi2} AL MOMENTO EMESSI BACK TO BACK
};

// Setters

/**
 * Return simulation passed time
 * 
 * @param dt Time increment
 */
void RadioNuclide::addElapsedTime(double dt){
    this->elapsedTime += dt; // Incrementing elapsed timer counter
    // Updating ativity
};

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide initial activity is: " << A_0 << " with decay constant " << lambda << " and half life " << tau << endl;
}

 