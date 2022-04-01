// STD includes
#include <iostream>

// External libs
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/source/Shape.h"

/**
 * @brief Construct a new Shape object
 * 
 * @param x center coordinate
 * @param y center coordinate
 * @param z center coordinate
 */
Shape::Shape(double x, double y, double z){

    // Changing log scope
    LOG_SCOPE_F(INFO, "SHAPE");
    
    // Logging construction
    LOG_F(INFO, "Constructing at: %p",(void*) this);

    // Assigning x position
    x_ = x;
    LOG_F(INFO, "X: %f", x_);

    // Assigning y position
    y_ = y;
    LOG_F(INFO, "Y: %f", y_);

    // Assigning z position
    z_ = z;
    LOG_F(INFO, "Z: %f", z_);

    LOG_F(INFO, "Shape created at O=(%f, %f, %f)", x_, y_, z_);
}

/**
 * @brief Destroy the Shape base class
 * 
 */
Shape::~Shape(){
    LOG_F(INFO, "Destroying shape %p", (void*) this);
}

Shape::Shape(const Shape& src):
    x_(src.x_),
    y_(src.y_),
    z_(src.z_)
    {
        LOG_F(WARNING, "Copying shape %p in new object", (void*) &src);
    }