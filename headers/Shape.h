#ifndef SHAPE
#define SHAPE

#include <iostream>
using namespace std;

class Shape {
    public:

        // Constructor
        Shape(bool (*fx)(double, double, double));

        double fx(double, double, double);
        void print();

        // Getters
        double getR();
        double getTheta();
        double getPhi();
        double getX();
        double getY();
        double getZ();

        // Setters
        void setR(double);
        void setTheta(double);
        void setPhi(double);
        void setX(double);
        void setY(double);
        void setZ(double);

    private:

        double R = 0;
        double theta = 0;
        double phi = 0;
        double x = 0;
        bool (*_fx)(double, double, double);


};

#endif