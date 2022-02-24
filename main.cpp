// STD Includes
#include <random>
#include <string>
#include <iostream>

// Libs Includes
#include "libs/loguru/loguru.cpp"

// Project includes
#include "src/RadioNuclide.cpp"
#include "src/Shape.cpp"
#include "src/Cylinder.cpp"
#include "src/Source.cpp"
#include "src/Detector.cpp"
#include "src/FileWriter.cpp"
#include "src/DetectorRing.cpp"

using namespace std;

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
//int main(int argc, char *argv[]){ // int argc, char *argv[] se parte da console RIGA PER QUANDO TOGLIAMO ROOT
int main(){
    
    // Finchè usiamo root
    int argc = 1;
    char **argv = new char*[1];
    argv[0] = new char[26]{"PETMontecarloSimulation"};

    // Starting loguru
    loguru::init(argc, argv);
    loguru::add_file("logs/log.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);

    // Loguru Logs
    LOG_F(INFO, "Starting PET Montecarlo simulation");

    // Getting seed for srand
    long int seed = 42;

    // Loguru Logs
    LOG_F(INFO, "Choosen srand seed: %lu", seed);

    // Random number seed
    srand(seed);
    
    // File path
    FileWriter& fileWriter = FileWriter::getInstance();

    // Creating our setup
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);
    Shape *cylinder = new Cylinder(1., 10.);
    Source *source = new Source(cylinder, F18);
    DetectorRing *ring = new DetectorRing(10, 2., 1);

    // Current Simulation Time
    double simulationTime = 0;
    double maxSimulationTime = 1;
    double simulationStep = 1;

    // Data string
    string msg;

    // Utils Variables
    int totalDecays = 0;
    int detectedDecays = 0;

    
    {   
        LOG_SCOPE_F(INFO,"SIMULATION");

        // Simulation cycle
        while(simulationTime < maxSimulationTime){

            // Evaluating number of decays
            double numberOfDecays = source->timeStepDecays(simulationTime);

            totalDecays += numberOfDecays;
            
            for(int n = 0; n < numberOfDecays; n++){
                // Sampling position and angles
                double* P = source->samplePosition(); // P1 points to sampled point
                double** angles = source->sampleAngles(); // w1, w2, t1, -t1 ---> t2 = sgn(t1)*pi/2-t1

                // Preparing data holder
                double* data = nullptr;

                // For every detector
                for(int i = 0; i < 2; i ++){            // Check both photons
                    data = ring->checkInteraction(P, angles[i]);
                    if(data != nullptr){
                        // Saving number of detector, time
                        msg = to_string(data[0]) + "," + to_string(data[1] + simulationTime);
                        fileWriter.writeData(0, msg);
                        detectedDecays++;
                    }
                }
            }

            // Adding simulation time
            simulationTime += simulationStep;
        }
        LOG_F(INFO, "Total number of decayed nuclei: %i", totalDecays);
        LOG_F(INFO, "Number of detected decays: %i", detectedDecays);
    }

    // Freeing memory
    delete ring;
    delete source;
    

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}