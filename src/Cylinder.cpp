// Includes
#include <math.h>
#include <random>

// Project includes
#include "../headers/Cylinder.h"


Cylinder::Cylinder(double radius, double heigth) : Shape(){
    this->radius = radius;
    this->heigth = heigth;
};

/**
 * Checks if a point is in this cyinder
 * 
 * @param X position on X Axis
 * @param Y position on Y Axis
 * @param Z position on Z Axis
 * 
 * @return true if the given point is inside the cylinder
 * @return false if not the given point is inside the cylinder
 */
bool Cylinder::isInside(double X, double Y, double Z){
    // If the given point is not inside the cylinder return false
    if((sqrt((this->getX()+X)*(this->getX()+X)+(this->getY()+Y)*(this->getY()+Y)) > radius) || ((this->getZ()+Z) > heigth)){
        return false;
    }
    return true;
};

double *Cylinder::sample(){
    
    // Defining local variables
    double x = 0;
    double y = 0;
    double z = 0;
    double u1 = 0; 
    double u2 = 0; 
    double u3 = 0;

    do{

        // Extracting 3 random numbers
        u1 = ((double) rand()/RAND_MAX);    
        u2 = ((double) rand()/RAND_MAX);
        u3 = ((double) rand()/RAND_MAX);

        // Sampling (x, y, z) point by remapping it between:
        // Min + (Max - Min)*#rand
        x = -this->radius + 2*(this->radius)*u1;
        y = -this->radius + 2*(this->radius)*u2; 
        z = -0.5*this->heigth + (this->radius)*u3;

    } while(!isInside(x, y, z)); // repeat if the point (x, y, z) is not inside the cylinder

    return (new double[3]{x, y, z});
};
