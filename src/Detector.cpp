#include "../headers/Detector.h"
#include <math.h>

Detector::Detector(double x, double y, double z, double h, double l, double omega){
    this->x = x;
    this->y = y;
    this->z = z;
    this->h = h;
    this->l = l;
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