#ifndef CYLINDER
#define CYLINDER

// Project includes
#include "../Shape.h"

class Cylinder : public Shape {
    public:
        // Constructors and destroyers
        ~Cylinder();
        Cylinder(double, double);
        Cylinder(const Cylinder&);
        Shape* clone(); // Probably to delete

        // Methods
        bool isInside(double, double, double);
        double* sample();

    private:
        // Constructor private to avoid unwanted initialization
        Cylinder();

        // Cylinder parameters
        double radius = 0;
        double heigth = 0;

};

#endif