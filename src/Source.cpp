#include "../headers/Cylinder.h"
#include "../headers/RadioNuclide.h"
#include "../headers/Source.h"
#include "utilities.cpp"
#include "../libs/loguru/loguru.hpp"
#include "../headers/Detector.h"

using namespace std;

Source::Source(Shape * shape, RadioNuclide * RadioNuclide){
    
    // Changing log scope
    LOG_SCOPE_F(INFO,"SOURCE");

    // Logging construction
    LOG_F(INFO, "Constructing at: 0x%x",this);

    // Logging shape
    this->shape = shape;
    LOG_F(INFO,"shape: %x", this->shape);

    // Logging radionuclide
    LOG_F(INFO,"radionuclide: %x", this->radioNuclide);
    this->radioNuclide = radioNuclide;
};

void Source::update(double dt, int nRiv, Detector* riv){
    double numberOfDecays = 1;//gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));
    radioNuclide->addElapsedTime(dt); 

    double* P1 = shape->sample(); // P1 points to sampled point
    double* angles = radioNuclide->sample();

    Detector* riv1;
    Detector* riv2;
    for(int i = 0; i < nRiv; i++){
        if (riv[i].check(angles[1])){
            riv1= &riv[i];
        }
        if (riv[i].check(angles[1]+M_PI)){
            riv2= &riv[i];
        }
    }
    
    double a1 = 0;
    double a2 = 0;
    double a3 = 0;
    double a4 = 0;
    
    bool a; 
    bool b;
    bool c;
    bool d;


};
