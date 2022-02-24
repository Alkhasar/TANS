/**
 * @file RadioNuclide.cpp
 * @brief RadioNuclide class implementation
 */

#include <math.h>
#include <random>

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

/**
 * @brief Destroy the Radio Nuclide
 * 
 */
RadioNuclide::~RadioNuclide(){
    LOG_F(INFO, "Destroying RadioNuclide 0x%p", this);
};

/**
 * @brief Copy constructor
 * 
 * @param src radio nuclide address
 */
RadioNuclide::RadioNuclide(const RadioNuclide& src):
    lambda(src.lambda),
    N_0(src.N_0),
    A_0(src.A_0),
    tau(src.tau),
    elapsedTime(src.elapsedTime)
{
    LOG_F(INFO, "Copying %p in new object", (void*) &src);
};

/**
 * @brief Getter for decay constant
 * 
 * @return decay constant as double 
 */
double RadioNuclide::getLambda(){
    return lambda;
};

/**
 * @brief Calculates and returns the number of radionuclide at time t
 * 
 * @param t current time
 * @return N_0*exp(-lambda*t) 
 */
double RadioNuclide::getN(double t){
    return N_0*exp(-lambda*t);
};

/**
 * @brief Calculates and returns activity at time t
 * 
 * @param t current time
 * @return exp(-lambda*t)
 */
double RadioNuclide::getA(double t){
    return lambda*getN(t);
};

/**
 * @brief Returns the half life
 * 
 * @return half life 
 */
double RadioNuclide::getTau(){
    return tau;
};

/**
 * @brief Returns current confidency range
 * 
 * @return confidency range 
 */
double RadioNuclide::getConfidency(){
    return confidency;
};

/**
 * @brief Returns elapsed time
 * 
 * @return double 
 */
double RadioNuclide::getElapsedTime(){
    return elapsedTime;
};

/**
 * @brief Returns 2 pair of angles, used as photon angles
 * 
 * @return {{omegaA, thetaA}, {omegaB, thetaB}} 
 */
double** RadioNuclide::sample(){ 

    // Omega nd theta angle sampling
    double omega = ((double) rand()/RAND_MAX) * M_PI; // Omega 1 from 0 to pi
    double theta  = -M_PI_2 + ((double) rand()/RAND_MAX) * M_PI_2; // theta 1 from -pi/2 to pi/2

    // Data organization
    double** angles = new double*[2];
    angles[0] = new double[2]{omega, theta};
    angles[1] = new double[2]{omega - M_PI, -theta};

    return angles; // Curently emitted back to back
};

/**
 * Return simulation passed time
 * 
 * @param dt Time increment
 */
void RadioNuclide::addElapsedTime(double dt){
    this->elapsedTime += dt; 
    // Updating ativity here
};

/**
 * Prints the radionuclide data
 */
void RadioNuclide::print(){
    cout << "The radionuclide initial activity is: " << A_0 << " with decay constant " << lambda << " and half life " << tau << endl;
}

 