#include "../headers/Detector.h"
#include <math.h>

Detector::Detector(double x, double y, double z, double h, double w, double omega){
    this->x = x;
    this->y = y;
    this->z = z;
    this->h = h;
    this->w = w;
    this->omega = omega;
    deltaOmega = l/ sqrt((x*x) + (y*y) + (z*z)); // l*r, eventualmente possiamo passarlo comeparametro 
};

bool Detector::check(double phi){ // where phi is the photon angle
    if (phi < omega + deltaOmega && phi > omega) return true; 
    return false;
};

double Detector::getH(){return h;};
double Detector::getOmega(){return omega;};
double Detector::getX(){return x;};
double Detector::getY(){return y;};
double Detector::getZ(){return z;};

// Local coordinates transform
double* Detector::transformToLocalCoordinates(double xg, double yg, double zg){
    return new double[3]{xg*cos(omega) - yg*sin(omega), xg*sin(omega) + yg*cos(omega), zg};
};

double Detector::transformToLocalOmega(double gOmega){
    return gOmega - omega;
};

double* Detector::returnToGlobalCoordinates(double xl, double yl, double zl){
    return new double[3]{xl*cos(-omega) - yl*sin(-omega), xl*sin(-omega) + yl*cos(-omega), zl};
};

double Detector::returnToGlobalOmega(double lOmega){
    return lOmega + omega;
};

