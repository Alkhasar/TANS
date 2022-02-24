#include "../headers/DetectorRing.h"
#include "../libs/loguru/loguru.hpp"
#include <math.h>

/**
 * @brief Construct a detector Ring taking a number n of detectors to arange around a ring of radius r.
 * 
 * @param n number of detectors
 * @param r radius of the detector ring
 * @param w detectors width (along z)
 */
DetectorRing::DetectorRing(int n, double r, double w){
    // Changing log scope
    LOG_SCOPE_F(INFO,"DETECTOR RING");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    //Asigning and logging parameters
    deltaOmega = 2 * M_PI / n;
    LOG_F(INFO, "Delta Omega: %fm", deltaOmega);

    this->w = w;
    LOG_F(INFO, "Detectors width: %fm", w);

    this->r = r;
    LOG_F(INFO, "Detectors center distance from origin: %fm", r);

    this->n = n;
    LOG_F(INFO, "Number of detectors: %i", n);

    h = 2 * r * tan(deltaOmega/2);
    LOG_F(INFO, "Detectors height: %fm", h);

    detectors = new Detector*[n];
    for(int i = 0; i < n; i++){
        detectors[i] = new Detector(w, h, deltaOmega*i, r);
    }
}

/**
 * @brief Destroy the Detector Ring and free detectors memory
 * 
 */
DetectorRing::~DetectorRing(){
    LOG_F(INFO, "Deleting detectors ring");
    delete[] detectors;
}

/**
 * @brief Returns all 4 vertexes of the given detector
 * 
 * @param i detector index starting from theta = 0
 * @return [[x1, y1, z1], ... , [x4, y4, z4]] 2d vertex array
 */
double** DetectorRing::getDetectorVertexes(int i){
    return detectors[i]->getVertex();
}