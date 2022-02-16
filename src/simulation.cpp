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
float elementarySimulationStep(float dt, RadioNuclide *radioNuclide){

    float numberOfDecays = rejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));
    radioNuclide->setElapsedTime(dt); 

    // - INIZIO TEST
    cout << numberOfDecays << endl;
    return numberOfDecays;
    // -- FINE TEST
    
}

