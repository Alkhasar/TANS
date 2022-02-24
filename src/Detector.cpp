#include "../headers/Detector.h"
#include "utilities.cpp"
#include <math.h>

/**
 * @brief Construct a new Detector starting by a base detector centered in (r, 0, 0) with width w
 * and heigth h, and rotates it along the z azis by an angle omega
 * 
 * @param w detector width, direction along z
 * @param h detector heigth, direction along y
 * @param omega rotation angle centered on z axis
 * @param r distance of detector center to origin
 */
Detector::Detector(double w, double h, double omega, double r){
    this->w = w; // Dimension along z
    this->h = h; // Dimension along y
    this->omega = omega;
    this->deltaOmega = atan(h/(2*r)); // Half aperture angle from center in y direction
    this->deltaTheta = atan(w/(2*r)); // Half aperture angle from center in z direction
    this->r = r;
    
    // Declaring vertex array (TODELTE)
    vertex = new double*[4];

    // Assigning vertes with rotations
    vertex[0] = zAxisRotation(new double[3]{r, h/2, w/2}, omega);
    vertex[1] = zAxisRotation(new double[3]{r, h/2, -w/2}, omega);
    vertex[2] = zAxisRotation(new double[3]{r, -h/2, -w/2}, omega);   
    vertex[3] = zAxisRotation(new double[3]{r, -h/2, w/2}, omega);
}

/**
 * @brief Checks if the detecor can detect a photon with a certain omega angle centered on
 * z axis. After this z axis check, another check on theta is needed.
 * 
 * @param omega Photon angle
 * @return true if the photon angle is in the detectors range
 * @return false if the photon angle is not in the detectors range
 */
bool Detector::checkOmega(double omega){ // where pi is the photon angle
    if (omega < this->omega + deltaOmega && omega > this->omega) return true; 
    return false;
};

/**
 * @brief Checks if the detector can detect a photon with a certain theta angle, taken in the local
 * frame of reference i.e. detector at angle w = 0
 * 
 * @param theta Photon angle
 * @return true if the photon angle is in the detectors range
 * @return false if the photon angle is not in the detectors range
 */
bool Detector::checkTheta(double theta){
    if (theta > min(-deltaTheta, deltaTheta) && theta < max(-deltaTheta, deltaTheta)) return true; 
    return false;
};

/**
 * @brief getter for the 4 base vertex
 * 
 * @return 2d array of vertex
 */
double** Detector::getVertex(){
    return vertex;
}

/**
 * @brief Getters for the omega angle
 * 
 * @return the omega angle as double
 */
double Detector::getOmega(){
    return omega;
}

