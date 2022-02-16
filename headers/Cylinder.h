#ifndef CYLINDER
#define CYLINDER

#include "Shape.h"

class Cylinder : public Shape{
    public:
        Cylinder(double, double);
        bool isInside(double, double, double);
        double * sample();

    private:
        double radius;
        double heigth;

};

#endif