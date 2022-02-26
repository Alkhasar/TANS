// Project includes
#include "../headers/detector/Detector.h"
#include "../headers/utils/FileWriter.h"
#include "../src/utils/utilities.cpp"

// std includes
#include <math.h>

// external libs
#include "../libs/loguru/loguru.hpp"

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

Detector::~Detector(){
    for(int i = 0; i < 4; i++){
        delete[] vertex[i];
    }
    delete[] vertex;
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
}

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
}

/**
 * @brief Getter for the 4 base vertex
 * 
 * @return 2d array of vertex
 */
double** Detector::getVertex(){
    return vertex;
}

/**
 * @brief Getter for the omega angle
 * 
 * @return the omega angle as double
 */
double Detector::getOmega(){
    return omega;
}

/**
 * @brief Getter for the deltaOmegaAngle
 * 
 * @return deltaOmega angle as double 
 */
double Detector::getDeltaOmega(){
    return deltaOmega;
}

/**
 * @brief Checks if a photon emitted from P with angle omega, theta interacts
 * with this detector
 * 
 * @param P photon origin
 * @param angles photon (omega, theta)
 * @return interaction time
 */
double Detector::interaction(double* P, double* angles){    
    // Simpler declaration of the 3 needed vertex
    double x0 = vertex[0][0];
    double y0 = vertex[0][1];
    double z0 = vertex[0][2];

    double x1 = vertex[1][0];
    double y1 = vertex[1][1];
    double z1 = vertex[1][2];

    double x2 = vertex[2][0];
    double y2 = vertex[2][1];
    double z2 = vertex[2][2];

    // Declaring photon position
    double xp = P[0];
    double yp = P[1];
    double zp = P[2];

    // Declaring omega, theta
    double omega = angles[0];
    double theta = angles[1];
    
    // Getting plane parameters
    double a = (y1 - y0) * (z2 - z0) -( z1 - z0) * (y2 - y1);
    double b = -(x1 - x0) * (z2-z0) + ( z1 - z0) * (x2 - x0);
    double c = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
    double d = -a*x0 - b*y0 - c*z0;

    // Getting interaction point
    double x = -(d + b*yp + c*zp - c*xp*tan(theta) - b*xp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
    double y = -(-a*yp - c*yp*tan(theta) + d*tan(omega) + a*xp*tan(omega) + c*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
    double z = -(-a*zp + d*tan(theta) + a*xp*tan(theta) + b*yp*tan(theta) - b*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
    
    if((x <= max(x0, x2) && x >= min(x0, x2)) && (y <= max(y1, y2) && y >= min(y1, y2)) && (z <= max(z0, z1) && z >= min(z0, z1))){
        // Calculating distance P1 - (x, y, z)
        double distance = sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y) + (zp-z)*(zp-z));

        // Calculating time taken in ns
        double time = distance / 0.3;

        // Getting msg to write
        string data = to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);

        // Saving data to file
        FileWriter::getInstance().writeData(1, data);

        // Returning data
        return time;

    }

    // Returning nullptr
    return 0;
}
