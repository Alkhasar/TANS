#ifndef POINT
#define POINT

// Project includes
#include "../Shape.h"

class Point : public Shape {
    public:
        
        // Constructor
        Point(double, double, double);
        
        // Destructor
        ~Point();
        
        // Copy constructor
        Point(const Point&);

        // Virtual class clone
        Shape* clone();

        // This method should never be called with point
        bool isInside(double, double, double) {return true;};
        
        // Returns the point
        double* sample();

    private:
        // Default constructor private to avoid unwanted initialization
        Point();
};

#endif