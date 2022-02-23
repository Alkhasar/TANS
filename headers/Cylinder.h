#ifndef CYLINDER
#define CYLINDER

#include "Shape.h"

class Cylinder : public Shape {
    public:
        // Constructors and destroyers
        Cylinder();
        ~Cylinder();
        Cylinder(double, double);
        Cylinder(const Cylinder&);
        Shape* clone();

        // Methods
        bool isInside(double, double, double);
        double * sample();

    private:
        double radius = 0;
        double heigth = 0;

};

#endif