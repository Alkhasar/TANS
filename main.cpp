// Project includes
#include "src/RadioNuclide.cpp"
#include "src/simulation.cpp"

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
int main(int argc, char *argv[]){

    // -- INIZIO TEST
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 10e8);
    
    for(int i = 0; i < 10; i++)
        elementarySimulationStep(1., F18);

    F18->print();
    // -- FINE TEST

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}