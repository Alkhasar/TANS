#include <iostream>
#include "../headers/Shape.h"
using namespace std;

Shape::Shape(){
};

Shape::Shape(const Shape& shape){
    LOG_F(WARNING, "Copy constructor with shape: %p", (void*) &shape);
};

Shape::~Shape(){
    LOG_F(INFO, "Destroying shape: %p", (void*) this);
};


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

