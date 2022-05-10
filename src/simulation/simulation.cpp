// STD Includes
#include <random>

// External libs
#include "../../libs/loguru/loguru.cpp"

// Project structs include
#include "../../headers/utils/Data.h"

// Debug Enabled
#define DEBUG_PARTICLES 0
#define DEBUG_POINTS 0
#define DEBUG_ANGLES 0
#define DEBUG_HIT 0
#define DEBUG_POS 0

// Project includes
#include "../utils/FileWriter.cpp"
#include "../source/RadioNuclide.cpp"
#include "../source/Shape.cpp"
#include "../source/shapes/Cylinder.cpp"
#include "../source/shapes/Point.cpp"
#include "../source/Source.cpp"
#include "../simulation/Time.cpp"
#include "../detector/geometry/Plane.cpp"
#include "../detector/Detector.cpp"
#include "../detector/DetectorRing.cpp"

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
int main(int argc, char *argv[]){
    // ---------------------------------------------------------------------------------------------------- //
    // INITALIZATION
    // ---------------------------------------------------------------------------------------------------- //
    
    // Starting loguru
    loguru::init(argc, argv);
    
    // Setting log file in logs/log.log in truncation mode (overwrite)
    loguru::add_file("../../logs/log.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);

    // Loguru Logs
    LOG_F(INFO, "Starting PET Montecarlo simulation");

    // Getting seed for srand
    long int seed = 42; //Answear to everything!

    // Loguru Logs
    LOG_F(INFO, "Choosen srand seed: %lu", seed);

    // Random number seed
    std::srand(seed);

    // File writer instance
    FileWriter& fileWriter = FileWriter::getInstance();

    // ---------------------------------------------------------------------------------------------------- //
    // OPTIONS
    // ---------------------------------------------------------------------------------------------------- //

    // Radionuclide parameters
    const double lambda           = 1./(109.771*60.0);
    const double N0               = 2e13;

    // Shape selector
    enum SHAPE_S : int {CYL, PT};
    SHAPE_S s                      = CYL;

    // Shapes data
    const double sourceRadius     = 0.3;
    const double sourceHeigth     = 2.0;
    const double x                = 0;
    const double y                = 0;
    const double z                = 0;

    // Time data
    #if DEBUG_PARTICLES == 1
        // Debug values for nDecays decays
        const double simulationTime   = 0;
        const double nDecays          = 1e5;

        // Edit nDecays
        const double simulationStep   = nDecays*(1e-6/30366);
        const double maxSimTime       = nDecays*(1e-5/30366);
    #else
        const double simulationTime   = 0;
        const double simulationStep   = 1e-4;
        const double maxSimTime       = 1e-3;
    #endif

    // Detector ring data
    const int detectorNumber      = 72;
    const double ringRadius       = 0.6;      
    const double detectorWidth    = 0.3;

    // ---------------------------------------------------------------------------------------------------- //
    // SIMULATION SETUP
    // ---------------------------------------------------------------------------------------------------- // 

    // Generating source RadioNuclide
    RadioNuclide* F18 = new RadioNuclide(lambda, N0);

    // Generating source Shape
    Shape* shape;
    switch(s){
        case CYL:
            shape = new Cylinder(sourceRadius, sourceHeigth, x, y, z);
            break;
        case PT:
            shape = new Point(x, y, z);
            break;
        default:
            LOG_F(ERROR, "Given shape does not exist!");
    }

    // Generating first instance of time tracker
    Time& time = Time::getInstance(simulationTime, simulationStep, maxSimTime);

    // Generating source
    Source* source = new Source(shape, F18);

    // Generating detector ring
    DetectorRing* ring = new DetectorRing(detectorNumber, ringRadius, detectorWidth);

    // Utils Variables
    int totalDecays = 0;
    int detectedDecays = 0;

    // ---------------------------------------------------------------------------------------------------- //
    // SIMULATION
    // ---------------------------------------------------------------------------------------------------- // 

    {   // Debug scope
        LOG_SCOPE_F(INFO, "SIMULATION");

        // Simulation loop
        while(time.update()){
            // Evaluating number of decays
            double numberOfDecays = source->decayedNuclei(time.getSimulationTime() - time.getSimulationStep(), time.getSimulationTime());

            for(int n = 0; n < numberOfDecays; n++){
                // Sampling position and angles
                double* P = source->samplePosition(); 
                double** angles = source->sampleAngles();

                // Preparing data holder
                double* data = nullptr;

                // Random time in time frame
                double randomTime = time.getSimulationTime() + ((double)rand()/RAND_MAX)*(time.getSimulationTime() + time.getSimulationStep());

                // For each of the 2 emitted photons
                for(int gamma = 0; gamma < 2; gamma++){
                    // Check both photons
                    data = ring->checkInteraction(P, angles[gamma]);

                    #if DEBUG_ANGLES == 1
                        // Saving angles data to file
                        string dataAngles = to_string(angles[gamma][0])+","+to_string(angles[gamma][1]);
                        FileWriter::getInstance().writeData(1, dataAngles);  
                    #endif

                    if(data != nullptr){
                        // Data to save
                        uint32_t event      = n + totalDecays;
                        uint8_t  detector   = data[0];
                        double time         = data[1] + randomTime*1e9 + gaussianRejection(0., (250*1e-3)/(2*sqrt(2*log(2)))); //250ps resolution
                        
                        // Data packet
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

    // Printing results to GREP for analysis
    std::cout << "OUTPUT " << maxSimTime << " " << detectorNumber << std::endl;

    // Program Exited Succesfully
    return 0;
}