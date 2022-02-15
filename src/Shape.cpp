#include <iostream>
#include "../headers/Shape.h"
using namespace std;

Shape::Shape(bool (*fx)(double, double, double)){
    this->_fx = fx;
};
double Shape::fx(double R, double theta, double phi){
    return _fx(R, theta, phi);
};
void Shape::print(){
    cout << "The shape's cordinates are: " << R << ", " << theta << ", " << phi << endl;
};

// Getters
double Shape::getR(){
    return R;
};
double Shape::getTheta(){
    return theta;
};
double Shape::getPhi(){
    return phi;
};
double Shape::getX(){
    
};

// Setters
void Shape::setR(double R){
    this->R = R;
};
void Shape::setTheta(double theta){
    this->theta = theta;
};
void Shape::setPhi(double phi){
    this->phi = phi;
};

