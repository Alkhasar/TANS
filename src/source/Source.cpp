// STD includes
#include <math.h>
#include <string>

// External libs
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/source/shapes/Cylinder.h"
#include "../../headers/source/Source.h"

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
    shape_ = shape;
    LOG_F(INFO,"shape: %p", (void*) (shape_));

    // Logging radionuclide
    radioNuclide_ = radioNuclide;
    LOG_F(INFO,"radionuclide: %p", (void*) (radioNuclide_));
}

/**
 * @brief Destroy the source
 * 
 */
Source::~Source(){
    LOG_F(INFO, "Destroying Source Object %p",(void*) this);

    // Destroying shape and radionuclide
    delete shape_;
    delete radioNuclide_;
}

/**
 * @brief Copy constructor
 * 
 * @param src source to copy
 */
Source::Source(const Source& src) {   
    LOG_F(INFO, "Copying source %p in new object", (void*) &src);

    // Copying important elements
    shape_ = src.shape_->clone();
    radioNuclide_ = new RadioNuclide(*src.radioNuclide_);
}

/**
 * @brief Calculates the number of decayed nuclei
 * 
 * @param dt elapsed time since last calculation
 * @return number of decayed nuclei
 */
int Source::decayedNuclei(double oldTime, double newTime){

    // Number of decays (temporay)
    int numberOfDecays = radioNuclide_->getN(oldTime) - radioNuclide_->getN(newTime); // TODO: GaussianRejection

    // Logging number of decays
    LOG_F(WARNING, "Number of decays in %.10fs at %.10f is %i", newTime - oldTime, newTime, numberOfDecays);

    // Returning number of decays
    return numberOfDecays;
}

/**
 * @brief Samples a point from the given shape
 * 
 * @return {x, y, z} of the sampled point
 */
double* Source::samplePosition(){
    return shape_->sample();
}

/**
 * @brief samples 2 angles from the radionuclide
 * 
 * @return {omega, theta} of the sampled decay
 */
double** Source::sampleAngles(){
    return radioNuclide_->sample();
}