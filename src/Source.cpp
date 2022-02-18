#include "../headers/Cylinder.h"
#include "../headers/RadioNuclide.h"
#include "../headers/Source.h"
#include "utilities.cpp"
#include "../libs/loguru/loguru.hpp"
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

void Source::update(double dt){
    double numberOfDecays = gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));
    radioNuclide->addElapsedTime(dt); 
};
