// STD Includes
#include <random>
#include <string>
#include <iostream>

// External libs
#include "libs/loguru/loguru.cpp"

// Project structs include
#include "headers/utils/Data.h"

// Project includes
#include "src/source/RadioNuclide.cpp"
#include "src/source/Shape.cpp"
#include "src/source/shapes/Cylinder.cpp"
#include "src/source/shapes/Point.cpp"
#include "src/source/Source.cpp"
#include "src/detector/Detector.cpp"
#include "src/detector/DetectorRing.cpp"
#include "src/utils/FileWriter.cpp"
#include "src/utils/utilities.cpp"

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
//int main(){
    //int argc = 2;
   //char** argv = new char*[argc]{"main.cpp", "PROGRAMMA"};

    // Starting loguru
    loguru::init(argc, argv);
    loguru::add_file("logs/log.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);

    // Loguru Logs
    LOG_F(INFO, "Starting PET Montecarlo simulation");

    // Getting seed for srand
    long int seed = 13973216;

    // Loguru Logs
    LOG_F(INFO, "Choosen srand seed: %lu", seed);

    // Random number seed
    srand(seed);

    // Data struct size
    LOG_F(INFO, "Size of Data struct: %i bytes", (int) sizeof(Data));

    // File path
    FileWriter& fileWriter = FileWriter::getInstance();

    /****************
     *  SIMULAZIONE *
     ****************/

    // Creating our setup
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);
    Shape *cylinder = new Cylinder(0.2, 0.0001);
    Point* point = new Point(0., 0., 0.);
    Source *source = new Source(point, F18);
    DetectorRing *ring = new DetectorRing(72, 6., 10.);

    // Current Simulation Time
    double simulationTime = 0;
    double maxSimulationTime = 5e-5;
    double simulationStep = 1e-7;

    // Utils Variables
    int totalDecays = 0;
    int detectedDecays = 0;

    {   // Debug scope
        LOG_SCOPE_F(INFO,"SIMULATION"); // 1.524506e-4s per operazione Senza ottimizzazione
                                        // 374.371s per 10^8 dati -> 537Mb File, 1676Mb Debug -> 3.74371us per ciclo -Ofast
                                        // 311.723s per 10^8 dati (No debug) -> 537Mb ->3.11723us per ciclo -Ofast 16.7% più veloce

        // Simulation cycle
        while(simulationTime < maxSimulationTime){

            // Adding simulation time
            simulationTime += simulationStep;

            // Evaluating number of decays
            double numberOfDecays = source->timeStepDecays(simulationTime);
            
            for(int n = 0; n < numberOfDecays; n++){
                // Sampling position and angles
                double* P = source->samplePosition(); // P1 points to sampled point
                double** angles = source->sampleAngles(); // w1, w2, t1, -t1 ---> t2 = sgn(t1)*pi/2-t1
                
                // Preparing data holder
                double* data = nullptr;

                // For every detector
                for(int i = 0; i < 1; i++){            // Check both photons
                    data = ring->checkInteraction(P, angles[i]);
                
                    if(data != nullptr){
                        // Data string
                        string msg2;
                        Data msg{(uint32_t) n + totalDecays, (uint8_t) data[0], data[1] + ((simulationTime - simulationStep) + ((double)rand()/RAND_MAX)*simulationStep*1e9) + gaussianRejection(0., (250*1e-3)/(2*sqrt(2*log(2))))};

                        // Saving number of detector, time
                        msg2 = to_string(n + totalDecays) + "," + to_string((int)data[0]) + "," + to_string(data[1] + ((simulationTime - simulationStep) + ((double)rand()/RAND_MAX)*simulationStep*1e9) + gaussianRejection(0., (250*1e-3)/(2*sqrt(2*log(2))))); // FWHM = 250ps
                        fileWriter.writeData(0, msg);
                        //fileWriter.writeData(2, msg2);
                        // detectedDecays++;
                        
                        // Removing this delete will inevitably cause a memory leak
                        delete[] data;
                    }
                }
                
                

                // Removing these deletes will inevitably cause a memory leak
                delete[] P;
                delete[] angles[0];
                delete[] angles[1];
                delete[] angles;

                // Tracking total number of decays
                // totalDecays++;
            }

            
            // Tracking total number of decays assuming A(t) constant in dt
            totalDecays += numberOfDecays;

        }
        LOG_F(INFO, "Total number of decayed nuclei: %i", totalDecays);
        LOG_F(INFO, "Number of detected photons: %i", detectedDecays);
    }

    // Freeing memory
    delete ring;
    delete source;
    
    /************
     *  ANALISI *
     ************/

    

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}