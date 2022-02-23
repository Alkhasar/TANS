#include "../headers/Cylinder.h"
#include "../headers/RadioNuclide.h"
#include "../headers/Source.h"
#include "../libs/loguru/loguru.hpp"
#include "../headers/Detector.h"
#include "../headers/FileWriter.h"

// #include "utilities.cpp"

#include <math.h>
#include <string>

using namespace std;

Source::Source(Shape * shape, RadioNuclide * radioNuclide){
    
    // Changing log scope
    LOG_SCOPE_F(INFO,"SOURCE");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    // Logging shape
    this->shape = shape;//shape->clone();
    LOG_F(INFO,"shape: %p", (void*) (this->shape));

    // Logging radionuclide
    this->radioNuclide = radioNuclide;//new RadioNuclide(*radioNuclide);
    LOG_F(INFO,"radionuclide: %p", (void*) (this->radioNuclide));
};

Source::Source(const Source& src):
    x(src.x),
    y(src.y),
    z(src.z),
    compton(src.compton)
{   
    LOG_F(INFO, "Copying %p in new object", &src);

    // Copying important elements
    this->shape = src.shape->clone();
    this->radioNuclide = new RadioNuclide(*src.radioNuclide);
};

Source::~Source(){
    LOG_F(INFO, "Destroying Source Object %p", this);

    // Destroying shape and radionuclide
    delete this->shape;
    delete this->radioNuclide;
};

void Source::update(double dt, int nRiv, Detector** riv){
    // Getting file writer instance
    FileWriter& fileWriter = FileWriter::getInstance();
    string data;
    
    double numberOfDecays = 1;//gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));

    radioNuclide->addElapsedTime(dt); 

    double* P1 = shape->sample(); // P1 points to sampled point
    double* angles = radioNuclide->sample();

    // Detector addresses
    Detector* detAdd[2] = {nullptr, nullptr};

    for(int i = 0; i < nRiv; i++){              // For every detectorw
        for(int j = 0; j < 2; j ++){            // Check both photons
            if(riv[i]->check(angles[j])){        // Check wich detector is in thetas range
                
                // WIP
                double xa   = riv[i]->getX() * cos(riv[i]->getOmega()) - riv[i]->getY() * sin(riv[i]->getOmega());
                double ya1  = riv[i]->getX() * sin(riv[i]->getOmega()) + riv[i]->getY() * cos(riv[i]->getOmega()) + riv[i]->getH();
                double ya2  = riv[i]->getX() * sin(riv[i]->getOmega()) + riv[i]->getY() * cos(riv[i]->getOmega()) - riv[i]->getH();

                double xb1 = xa * cos(riv[i]->getOmega()) + ya1 * sin(riv[i]->getOmega());
                double xb2 = xa * cos(riv[i]->getOmega()) + ya2 * sin(riv[i]->getOmega());
                double yb1 = ((- xa * 1.0 / sin(riv[i]->getOmega()) + ya1 * 1.0 / sin(riv[i]->getOmega()) * tan(M_PI_2 - riv[i]->getOmega()))/(1.0 + tan(M_PI_2 - riv[i]->getOmega())*tan(M_PI_2 - riv[i]->getOmega()))); // Cotangente shiftata di pi/2
                double yb2 = ((- xa * 1.0 / sin(riv[i]->getOmega()) + ya2 * 1.0 / sin(riv[i]->getOmega()) * tan(M_PI_2 - riv[i]->getOmega()))/(1.0 + tan(M_PI_2 - riv[i]->getOmega())*tan(M_PI_2 - riv[i]->getOmega())));

                // Alpha values
                double a1 = atan2(yb1 - P1[1], xb1 - P1[0]);
                double a2 = atan2(yb2 - P1[1], xb2 - P1[0]);

                LOG_SCOPE_F(INFO, "Interaction Test"); // Alcuni angoli risultano essere maggiori di pi/2
                
                LOG_F(INFO, "NRIV: %i", i);
                LOG_F(INFO, "RIVX: %f", riv[i]->getX());
                LOG_F(INFO, "RIVY: %f", riv[i]->getY());
                LOG_F(INFO, "xa: %f", xa);  
                LOG_F(INFO, "ya1: %f", ya1);  
                LOG_F(INFO, "ya2: %f", ya2); 

                LOG_F(INFO, "yb1: %f", yb1);  
                LOG_F(INFO, "yb2: %f", yb2);  
                LOG_F(INFO, "xb1: %f", xb1);  
                LOG_F(INFO, "xb2: %f", xb2);     
                LOG_F(WARNING, "Angle a1: %f", a1);    
                LOG_F(WARNING, "Angle a2: %f", a2);
                LOG_F(INFO, "Angle theta: %f", angles[2 + j]);
                LOG_F(INFO, "a1 < a2: %i", a1 < a2);

                // Checking if photon interacts with the detector
                if(angles[2 + j] > min(a1, a2) && angles[2 + j] <  max(a1, a2)){ // Potrebbe accadere che a1 ed a2 siano negativi ERRORE SE UNO POSITIVO ED UNO NEGATIVO
                    detAdd[j] = riv[i];
                    LOG_F(WARNING, "INTERAZIONE");
                }
                else{
                    LOG_F(INFO, "NO INTERAZIONE");
                }
            }
        }
    }
};
