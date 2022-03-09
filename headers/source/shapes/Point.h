#ifndef POINT
#define POINT

// Project includes
#include "../Shape.h"

class Point : public Shape {
    public:
        // Constructors and destroyers
        ~Point();
        Point(double, double, double);
        Point(const Point&);
        Shape* clone(); // Probably to delete

        // Methods
        bool isInside(double, double, double);
        double* sample();

    private:
        // Constructor private to avoid unwanted initialization
        Point();

        // Point parameters
        double x = 0;
        double y = 0;
        double z = 0;

};

#endif