// Std includes
#include <math.h>

// Project includes
#include "../headers/detector/DetectorRing.h"
#include "../libs/loguru/loguru.hpp"


/**
 * @brief Construct a detector Ring taking a number n of detectors to arange around a ring of radius r.
 * 
 * @param n number of detectors
 * @param r radius of the detector ring
 * @param width detectors width (along z)
 */
DetectorRing::DetectorRing(int n, double radius, double width){
    // Changing log scope
    LOG_SCOPE_F(INFO,"DETECTOR RING");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    //Asigning and logging parameters
    deltaOmega = 2 * M_PI / n;
    LOG_F(INFO, "Delta Omega: %frad", deltaOmega);

    this->width = width;
    LOG_F(INFO, "Detectors width: %fm", width);

    this->radius = radius;
    LOG_F(INFO, "Detectors center distance from origin: %fm", radius);

    this->n = n;
    LOG_F(INFO, "Number of detectors: %i", n);

    height = 2 * radius * tan(deltaOmega/2);
    LOG_F(INFO, "Detectors height: %fm", height);

    detectors = new Detector*[n];
    for(int i = 0; i < n; i++){
        detectors[i] = new Detector(width, height, deltaOmega*i, radius);
    }
}

/**
 * @brief Destroy the Detector Ring and free detectors memory
 * 
 */
DetectorRing::~DetectorRing(){
    LOG_F(INFO, "Destroying detectors ring %p", (void*) this);
    delete[] detectors;
}

/**
 * @brief Checks if a given photon interacts with any of the the detectors
 * 
 * @param P emitted photon position
 * @param angles emitted photon angles
 * @return TOCHANGE: {xp, yp, zp, x, y, z, time, n} should be only detector number and time
 */
double* DetectorRing::checkInteraction(double* P, double* angles){
    double time = 0;
    for(int i = 0; i < n; i++){
        // Retrieve interaction deltaT from creation to hit
        time = detectors[i]->interaction(P, angles); // Outside of debig should be only time
        if(time != 0){
            // if a time has been given return it with detector number
            return (new double[2]{(double) i, time});
        }
    }
    return nullptr;
}