// Includes
#include <math.h>
#include <random>

// Project includes
#include "../../../headers/source/shapes/Point.h"
#include "../../../libs/loguru/loguru.hpp"


/**
 * @brief Constructing a new Point with radius and height
 * 
 * @param radius in distance units
 * @param heigth in distance units
 */
Point::Point(double x, double y, double z) : Shape(){
        
    // Changing log scope
    LOG_SCOPE_F(INFO, "Point");
    

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    
    // Logging X
    this->x = x;
    LOG_F(INFO, "X: %f", this->x);
    
    // Logging Y
    this->y = y;
    LOG_F(INFO, "Y: %f", this->y);

    // Logging Z
    this->z = z;
    LOG_F(INFO, "Z: %f", this->z);
    
}

/**
 * @brief Copy constructor
 * 
 * @param src take a Point address
 */
Point::Point(const Point& src):
    x(src.x),
    y(src.y),
    z(src.z)
    {
        LOG_F(WARNING, "Copy %p in new object",(void*) &src);
    }

/**
 * @brief Destroy the Point
 * 
 */
Point::~Point(){
    LOG_F(INFO, "Destroying Point %p", (void*) this);
}


/**
 * @brief Cloning constructor 
 * 
 * @return Cloned Point
 */
Shape* Point::clone(){
    LOG_F(WARNING, "Cloning Point at: %p", (void* )this);
    return new Point(*this);
}

/**
 * Checks if a Point is in this cyinder
 * 
 * @param X position on X Axis
 * @param Y position on Y Axis
 * @param Z position on Z Axis
 * 
 * @return true if the given Point is inside the Point
 * @return false if not the given Point is inside the Point
 */
bool Point::isInside(double, double, double){return true;}

double *Point::sample(){
    return (new double[3]{this->x, this->y, this->z});
}
