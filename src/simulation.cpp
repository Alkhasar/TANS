// Base includes
#include <random>

// Project includes
#include "../headers/RadioNuclide.h"

/**
 * This function describes the basic step in the PET simulation
 * 
 * @param dt simulation step duration
 * @param radioNuclide Radionuclide specs
 */
void elementarySimulationStep(float dt, RadioNuclide *radioNuclide){

    int numberOfDecays = ((float) rand()/RAND_MAX)*radioNuclide->getA()*dt; // Number of decayed nuclei

    // - INIZIO TEST
    radioNuclide->setA(radioNuclide->getA()-numberOfDecays);                // Updating the number of radioactive nuclides
    cout << numberOfDecays << endl;
    // -- FINE TEST
    
}