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
#include "src/Time.cpp"

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
    Shape *cylinder = new Cylinder(3.0, 3.0);
    //Point* point = new Point(2., -2., 2.);
    Source *source = new Source(cylinder, F18);
    DetectorRing *ring = new DetectorRing(200, 6., 10.);

    // Utils Variables
    int totalDecays = 0;
    int detectedDecays = 0;

    {   // Debug scope
        LOG_SCOPE_F(INFO,"SIMULATION"); // 1.524506e-4s per operazione Senza ottimizzazione
                                        // 374.371s per 10^8 dati -> 537Mb File, 1676Mb Debug -> 3.74371us per ciclo -Ofast
                                        // 311.723s per 10^8 dati (No debug) -> 537Mb ->3.11723us per ciclo -Ofast 16.7% più veloce
        // Simulation cycle
        while(Time::getTime().update()){

            // Evaluating number of decays
            double numberOfDecays = source->timeStepDecays(Time::getTime().getSimulationTime());
            
            for(int n = 0; n < numberOfDecays; n++){
                // Sampling position and angles
                double* P = source->samplePosition(); // P1 points to sampled point
                double** angles = source->sampleAngles(); // w1, w2, t1, -t1 ---> t2 = sgn(t1)*pi/2-t1

                // Preparing data holder
                double* data = nullptr;

                // For each of the 2 emitted photons
                for(int i = 0; i < 2; i++){            
                    // Check both photons
                    data = ring->checkInteraction(P, angles[i]);

                    // Saving angles data to file
                    string dataB = to_string(angles[i][0])+","+to_string(angles[i][1]);
                    FileWriter::getInstance().writeData(1, dataB);  

                    if(data != nullptr){
                        // Data to save
                        uint32_t event      = n + totalDecays;
                        uint8_t  detector   = data[0];
                        double time         = data[1] + ((Time::getTime().getSimulationTime() - Time::getTime().getTimeStep())*1e9 + ((double)rand()/RAND_MAX)*Time::getTime().getTimeStep()*1e9) + gaussianRejection(0., (250*1e-3)/(2*sqrt(2*log(2))));

                        // Data string
                        Data msg {event, detector, time};
                        ring->addData((int) data[0], msg);

                        // Tracking detected decays
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

                // Tracking total number of decays
                totalDecays++;
            }

            // Tracking total number of decays assuming A(t) constant in dt
            // totalDecays += numberOfDecays;
            ring->saveData();

        }
        LOG_F(INFO, "Total number of decayed nuclei: %i", totalDecays);
        LOG_F(INFO, "Number of detected photons: %i", detectedDecays);
    }

    // Freeing memory
    delete ring;
    delete source;

    

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}