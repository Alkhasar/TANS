// Includes
#include <math.h>
#include <random>

// Project includes
#include "../headers/Cylinder.h"

/**
 * @brief Constructing a new cylinder with radius and height
 * 
 * @param radius in distance units
 * @param heigth in distance units
 */
Cylinder::Cylinder(double radius, double heigth) : Shape(){
        
    // Changing log scope
    LOG_SCOPE_F(INFO, "CYLINDER");
    

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    
    // Logging radius
    this->radius = radius;
    LOG_F(INFO, "Radius: %f", this->radius);
    
    // Logging heigth
    this->heigth = heigth;
    LOG_F(INFO, "heigth: %f", this->heigth);
    
};

/**
 * @brief Copy constructor
 * 
 * @param src take a cylinder address
 */
Cylinder::Cylinder(const Cylinder& src):
    radius(src.radius),
    heigth(src.heigth)
    {
        LOG_F(WARNING, "Copy 0x%p in new object", &src);
    };

/**
 * @brief Destroy the Cylinder
 * 
 */
Cylinder::~Cylinder(){
    LOG_F(INFO, "Destroying cylinder %p", (void*) this);
};


/**
 * @brief Cloning constructor 
 * 
 * @return Cloned cylinder
 */
Shape* Cylinder::clone(){
    LOG_F(WARNING, "Cloning Cylinder at: %p", (void* )this);
    return new Cylinder(*this);
}

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
    double u1 = 0, u2 = 0, u3 = 0;
    double x = 0,  y = 0,  z = 0;

    do{
        // Extracting 3 random numbers
        u1 = ((double) rand()/RAND_MAX);    
        u2 = ((double) rand()/RAND_MAX);
        u3 = ((double) rand()/RAND_MAX);

        // Sampling (x, y, z) point by remapping it between:
        // Min + (Max - Min)*#rand
        x = -this->radius + 2*(this->radius)*u1;
        y = -this->radius + 2*(this->radius)*u2; 
        z = -0.5*this->heigth + (this->heigth)*u3;

    } while(!isInside(x, y, z)); // repeat if the point (x, y, z) is not inside the cylinder

    return (new double[3]{x, y, z});
};
