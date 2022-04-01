// STD includes
#include <math.h>
#include <random>
#include <string>

// External libs
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/source/RadioNuclide.h"


/**
 * @brief Construct a new Radio Nuclide object
 * 
 * @param lambda decay constant
 * @param N_0 starting number of nuclei
 */
RadioNuclide::RadioNuclide(double lambda, double N0){

    // Changing log scope
    LOG_SCOPE_F(INFO, "RADIO NUCLIDE");
    
    // Checking parameters
    LOG_IF_F(ERROR, lambda==0 || N0==0, "Radionuclide not initalised correctly! Check that both lambda and N_0 are greater then 0!");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p",(void*) this);

    // Logging Activity
    A0_ = N0*lambda;
    LOG_F(INFO, "Activity: %f", A0_);

    // Logging decay constant
    lambda_ = lambda;
    LOG_F(INFO, "lambda: %f", lambda_);

    // Logging N
    N0_ = N0;
    LOG_F(INFO, "N_0: %f", N0_);

    // Logging tau
    tau_ = 1./lambda_;
    LOG_F(INFO, "tau: %lf", tau_);
}

/**
 * @brief Destroy the Radio Nuclide and log it
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
    lambda_(src.lambda_),
    N0_(src.N0_),
    A0_(src.A0_),
    tau_(src.tau_)
{
    LOG_F(INFO, "Copying %p in new object", (void*) &src);
}

/**
 * @brief Calculates and returns the number of radionuclide at time t
 * 
 * @param t current time
 * @return N_0*exp(-lambda*t) 
 */
double RadioNuclide::getN(double t){
    return N0_*exp(-lambda_*t);
}


/**
 * @brief Returns 2 pair of angles, used as photon angles
 * 
 *          theta -> [0, Pi]
 *          phi   -> [0, 2Pi]
 * 
 * @return {{phiA, thetaA}, {phiB, thetaB}} 
 */
double** RadioNuclide::sample(){ 
    
    // Lines of code to keep track of code to use the builtin
    // random number generator
    
    /*static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> disA(0, 1);

    double u1 = disA(gen);
    double u2 = disA(gen);*/

    // Generating a pair of random uniform numbers between (0, 1);
    double u1 = (double) rand()/RAND_MAX; 
    double u2 = (double) rand()/RAND_MAX; 

    // Omega nd theta angle sampling
    double phi = 2 * M_PI * u1 ; 
    double theta = acos(1 - 2*u2);
    
    // Data organization
    double** angles = new double*[2];
    angles[0] = new double[2]{phi, theta};
    angles[1] = new double[2]{std::fmod(phi + M_PI, 2*M_PI),  M_PI - theta};

    // Debug stuff
    #if DEBUG_ANGLES == 1
        std::string s = std::to_string(angles[0][0]) + " " + std::to_string(angles[0][1]) + " " + std::to_string(angles[1][0]) + " " + std::to_string(angles[1][1]);
        FileWriter::getInstance().writeData(4, s);
    #endif

    return angles;
}