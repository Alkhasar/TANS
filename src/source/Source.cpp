// Project includes
#include "../../headers/source/shapes/Cylinder.h"
#include "../../headers/source/Source.h"
#include "../../headers/detector/Detector.h"
#include "../../headers/utils/FileWriter.h"

// External libs includess
#include "../../libs/loguru/loguru.hpp"

// Std includes
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
}

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
    LOG_F(INFO, "Copying %p in new object",(void*) &src);

    // Copying important elements
    this->shape = src.shape->clone();
    this->radioNuclide = new RadioNuclide(*src.radioNuclide);
}

/**
 * @brief Destroy the source
 * 
 */
Source::~Source(){
    LOG_F(INFO, "Destroying Source Object %p",(void*) this);

    // Destroying shape and radionuclide
    delete this->shape;
    delete this->radioNuclide;
}

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
    int numberOfDecays = radioNuclide->getN(oldEvaluationTime) - radioNuclide->getN(time);//gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));

    // Logging number of decays
    LOG_F(WARNING, "Number of decays in %fs is %i", dt, numberOfDecays);

    // Adjust radionuclide activity
    radioNuclide->addElapsedTime(dt); 

    // Setting old evaluation time
    oldEvaluationTime = time;

    // Returning number of decays
    return numberOfDecays;

}

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