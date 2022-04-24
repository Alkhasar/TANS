// STD Includes
#include <math.h>
#include <random>

// External libs
#include "../../../libs/loguru/loguru.hpp"

// Project includes
#include "../../../headers/source/shapes/Cylinder.h"

/**
 * @brief Constructing a new cylinder with radius and height
 * 
 * @param radius in distance units
 * @param heigth in distance units
 */
Cylinder::Cylinder(double radius, double heigth, double x = 0, double y = 0, double z = 0) : Shape(x, y, z){
        
    // Changing log scope
    LOG_SCOPE_F(INFO, "CYLINDER");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);
    
    // Logging radius
    radius_ = radius;
    LOG_F(INFO, "Radius: %f", radius_);
    
    // Logging heigth
    heigth_ = heigth;
    LOG_F(INFO, "heigth: %f", heigth_);
    
}

/**
 * @brief Destroy the Cylinder
 * 
 */
Cylinder::~Cylinder(){
    LOG_F(INFO, "Destroying cylinder %p", (void*) this);
}

/**
 * @brief Copy constructor
 * 
 * @param src take a cylinder address
 */
Cylinder::Cylinder(const Cylinder& src) :
    Shape(src),
    heigth_(src.heigth_),
    radius_(src.radius_)
    {
        LOG_F(WARNING, "Copying cylinder %p in new object",(void*) &src);
    }

/**
 * @brief Cloning constructor, used in copy constructor of other classes
 * that do not know which shape we have
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
 * @param x position on X Axis
 * @param y position on Y Axis
 * @param z position on Z Axis
 * 
 * @return true if the given point is inside the cylinder
 * @return false if not the given point is inside the cylinder
 */
bool Cylinder::isInside(double x, double y, double z){
    // If the given point is not inside the cylinder return false
    if((sqrt((x_ - x)*(x_ - x)+(y_ - y)*(y_ - y)) > radius_) || ((z_ + z) > heigth_ || (z_ + z) < -heigth_)){
        return false;
    }
    return true;
}

/**
 * @brief Returns a random point sampled inside the cylinder
 * 
 * @return double* 
 */
double* Cylinder::sample(){
    
    // Defining local variables
    double u1 = 0, u2 = 0, u3 = 0;
    double x = 0,  y = 0,  z = 0;

    do{
        // Extracting 3 random numbers
        u1 = ((double) rand()/RAND_MAX);    
        u2 = ((double) rand()/RAND_MAX);
        u3 = ((double) rand()/RAND_MAX);

        // Sampling (x, y, z) point by remapping it between:
        // Min + (Max - Min)*#rand + base
        x = (-radius_ + x_) + 2*radius_*u1;
        y = -radius_ + 2*radius_*u2 + y_; 
        z = -0.5*heigth_ + heigth_*u3 + z_;

        #if DEBUG_POS == 1
            LOG_F(INFO, "POS: %f %f %f", x, y, z);
            LOG_F(WARNING, "POS_0: %f %f %f", x_, y_, z_);
        #endif

    } while(!isInside(x, y, z)); // repeat if the point (x, y, z) is not inside the cylinder

    return (new double[3]{x, y, z});//(new double[3]{x, y, z});
}


