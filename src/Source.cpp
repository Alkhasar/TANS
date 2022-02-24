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

/**
 * @brief Builds a new source object given a shape and a radionuclide
 * 
 * @param shape the source shape
 * @param radioNuclide the radio nuclide
 */
Source::Source(Shape * shape, RadioNuclide * radioNuclide){
    
    // Changing log scope
    LOG_SCOPE_F(INFO,"SOURCE");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    // Logging shape
    this->shape = shape;
    LOG_F(INFO,"shape: %p", (void*) (this->shape));

    // Logging radionuclide
    this->radioNuclide = radioNuclide;
    LOG_F(INFO,"radionuclide: %p", (void*) (this->radioNuclide));
};

/**
 * @brief Copy constructor
 * 
 * @param src source to copy
 */
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

/**
 * @brief Destroy the source
 * 
 */
Source::~Source(){
    LOG_F(INFO, "Destroying Source Object %p", this);

    // Destroying shape and radionuclide
    delete this->shape;
    delete this->radioNuclide;
};

/**
 * @brief Calculates the number of decayed nuclei
 * 
 * @param time elapsed time 
 * @return number of decayed nuclei
 */
int Source::timeStepDecays(double time){

    // Getting timestep
    double dt = time - oldEvaluationTime;

    // Number of decays (temporay)
    double numberOfDecays = 100000;//gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));

    // Adjust radionuclide activity
    radioNuclide->addElapsedTime(dt); 

    // Returning number of decays
    return numberOfDecays;

};

/**
 * @brief Samples a point from the given shape
 * 
 * @return {x, y, z} of the sampled point
 */
double* Source::samplePosition(){
    return shape->sample();
}

/**
 * @brief samples 2 angles from the radionuclide
 * 
 * @return {omega, theta} of the sampled decay
 */
double** Source::sampleAngles(){
    return radioNuclide->sample();
}