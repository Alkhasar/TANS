// STD includes
#include <math.h>
#include <random>

// External libs
#include "../../../libs/loguru/loguru.hpp"

// Project includes
#include "../../../headers/source/shapes/Point.h"

/**
 * @brief Constructing a new Point with radius and height
 * 
 * @param radius in distance units
 * @param heigth in distance units
 */
Point::Point(double x, double y, double z) : Shape(x, y, z){
        
    // Changing log scope
    LOG_SCOPE_F(INFO, "Point");
    
    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

}

/**
 * @brief Destroy the Point
 * 
 */
Point::~Point(){
    LOG_F(INFO, "Destroying Point %p", (void*) this);
}

/**
 * @brief Copy constructor
 * 
 * @param src take a Point address
 */
Point::Point(const Point& src) :
    Shape(src)
    {
        LOG_F(WARNING, "Copying point %p in new object",(void*) &src);
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
 * @brief Returns its position
 * 
 * @return point array
 */
double *Point::sample(){
    return (new double[3]{x_, y_, z_});
}