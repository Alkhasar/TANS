#ifndef CYLINDER
#define CYLINDER

// Project includes
#include "../Shape.h"

class Cylinder : public Shape {
    public:

        // Constructor
        Cylinder(double, double, double, double, double);

        // Destructor
        ~Cylinder();

        // Copy constructor
        Cylinder(const Cylinder&);

        // Virtual class clone
        Shape* clone(); // Probably to delete

        // Virtual method implementation
        bool isInside(double, double, double);
        double* sample();

    private:
        // Default constructor private to avoid unwanted initialization
        Cylinder();

        // Cylinder parameters
        double radius_ = 0;
        double heigth_ = 0;

};

#endif