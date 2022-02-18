#ifndef CYLINDER
#define CYLINDER

#include "Shape.h"

class Cylinder : public Shape {
    public:
        Cylinder(double, double);
        bool isInside(double, double, double);
        double * sample();

    private:
        double radius = 0;
        double heigth = 0;

};

#endif