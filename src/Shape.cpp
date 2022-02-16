#include <iostream>
#include "../headers/Shape.h"
using namespace std;

// Shape::Shape(bool (*fx)(double, double, double)){
//     this->_fx = fx;
// };

// double Shape::fx(double R, double theta, double phi){
//     return _fx(R, theta, phi);
// };

void Shape::print(){
    cout << "The shape's cordinates are: " << X << ", " << X << ", " << Z << endl;
};

// Getters
double Shape::getX(){
    return X;
};
double Shape::getY(){
    return Y;
};
double Shape::getZ(){
    return Z;
};

// Setters
void Shape::setX(double X){
    this->X = X;
};
void Shape::setY(double Y){
    this->Y = Y;
};
void Shape::setZ(double Z){
    this->Z = Z;
};

