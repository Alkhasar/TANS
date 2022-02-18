// Base includes
#include <random>

// Project includes
#include "../headers/RadioNuclide.h"
#include "../headers/Shape.h"
#include "utilities.cpp"

/**
 * This function describes the basic step in the PET simulation
 * 
 * @param dt simulation step duration
 * @param radioNuclide Radionuclide specs
 */
double elementarySimulationStep(double dt, RadioNuclide *radioNuclide){

    double numberOfDecays = gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));
    radioNuclide->addElapsedTime(dt); 

    // - INIZIO TEST
    // cout << numberOfDecays << endl;
    return numberOfDecays;
    // -- FINE TEST
    
}

