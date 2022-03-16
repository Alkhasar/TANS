// Project includes
#include "../../headers/source/RadioNuclide.h"

// External libs
#include "../../libs/loguru/loguru.hpp"

// std includes
#include <math.h>
#include <random>

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
}

/**
 * @brief Destroy the Radio Nuclide
 * 
 */
RadioNuclide::~RadioNuclide(){
    LOG_F(INFO, "Destroying RadioNuclide %p", (void*) this);
}

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
}

/**
 * @brief Getter for decay constant
 * 
 * @return decay constant as double 
 */
double RadioNuclide::getLambda(){
    return lambda;
}

/**
 * @brief Calculates and returns the number of radionuclide at time t
 * 
 * @param t current time
 * @return N_0*exp(-lambda*t) 
 */
double RadioNuclide::getN(double t){
    return N_0*exp(-lambda*t);
}

/**
 * @brief Calculates and returns activity at time t
 * 
 * @param t current time
 * @return exp(-lambda*t)
 */
double RadioNuclide::getA(double t){
    return lambda*getN(t);
}

/**
 * @brief Returns the half life
 * 
 * @return half life 
 */
double RadioNuclide::getTau(){
    return tau;
}

/**
 * @brief Returns current confidency range
 * 
 * @return confidency range 
 */
double RadioNuclide::getConfidency(){
    return confidency;
}

/**
 * @brief Returns elapsed time
 * 
 * @return double 
 */
double RadioNuclide::getElapsedTime(){
    return elapsedTime;
}

/**
 * @brief Returns 2 pair of angles, used as photon angles
 * 
 * @return {{omegaA, thetaA}, {omegaB, thetaB}} 
 */
double** RadioNuclide::sample(){ 

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disA(0, 1);
    std::uniform_real_distribution<> disB(-1, 1);

    double u1 = disA(gen);//(double) rand()/RAND_MAX; 
    double u2 = disB(gen);//(double) rand()/RAND_MAX;

    // Omega nd theta angle sampling
    double omega = 2 * M_PI * u1 ; // Omega 1 from 0 to pi
    double theta = acos(u2); // acos(u2);1 - 2 * 
    
    // Data organization
    double** angles = new double*[2];
    angles[0] = new double[2]{omega, theta};
    angles[1] = new double[2]{std::fmod(omega + M_PI, 2*M_PI), theta};//(omega >= M_PI) ? omega - M_PI : omega + M_PI, theta};// std::fmod(omega + M_PI, 2*M_PI)

    return angles; // Curently emitted back to back
}

/**
 * Return simulation passed time
 * 
 * @param dt Time increment
 */
void RadioNuclide::addElapsedTime(double dt){
    this->elapsedTime += dt; 
    // Updating ativity here
}