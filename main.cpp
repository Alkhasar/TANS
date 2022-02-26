// STD Includes
#include <random>
#include <string>
#include <iostream>

// External libs
#include "libs/loguru/loguru.cpp"

// Project includes
#include "src/source/RadioNuclide.cpp"
#include "src/source/Shape.cpp"
#include "src/source/shapes/Cylinder.cpp"
#include "src/source/Source.cpp"
#include "src/detector/Detector.cpp"
#include "src/detector/DetectorRing.cpp"
#include "src/utils/FileWriter.cpp"

using namespace std;

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
int main(int argc, char *argv[]){

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
    Shape *cylinder = new Cylinder(0.2, 5.);
    Source *source = new Source(cylinder, F18);
    DetectorRing *ring = new DetectorRing(8, 1., 3.);

    // Current Simulation Time
    double simulationTime = 0;
    double maxSimulationTime = 1e-3;
    double simulationStep = 1e-4;

    // Data string
    string msg;

    // Utils Variables
    int totalDecays = 0;
    int detectedDecays = 0;

    {  
        LOG_SCOPE_F(INFO,"SIMULATION"); // 1.524506e-4s per operazione Senza ottimizzazione
                                        // 374.371s per 10^8 dati -> 537Mb File, 1676Mb Debug -> 3.74371us per ciclo -Ofast
                                        // 311.723s per 10^8 dati (No debug) -> 537Mb ->3.11723us per ciclo -Ofast 16.7% più veloce

        // Simulation cycle
        while(simulationTime < maxSimulationTime){

            // Adding simulation time
            simulationTime += simulationStep;

            // Evaluating number of decays
            double numberOfDecays = source->timeStepDecays(simulationTime);

            // Tracking total number of decays
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
                        msg = to_string(data[0]) + "," + to_string(data[1] + simulationTime); // PROBLEMA QUI: Sto SOMMANDO ns CON SECOND
                        fileWriter.writeData(0, msg);
                        detectedDecays++;
                        
                        // Removing this delete will inevitably cause a memory leak
                        delete[] data;
                    }
                }
                

                // Removing these deletes will inevitably cause a memory leak
                delete[] P;
                delete[] angles[0];
                delete[] angles[1];
                delete[] angles;
            }

        }
        LOG_F(INFO, "Total number of decayed nuclei: %i", totalDecays);
        LOG_F(INFO, "Number of detected photons: %i", detectedDecays);
    }

    // Freeing memory
    delete ring;
    delete source;
    

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}