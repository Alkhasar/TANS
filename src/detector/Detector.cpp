// Project includes
#include "../../headers/detector/Detector.h"
#include "../../headers/utils/FileWriter.h"
#include "../../src/utils/utilities.cpp"

// std includes
#include <math.h>

// external libs
#include "../../libs/loguru/loguru.hpp"

/**
 * @brief Construct a new Detector starting by a base detector centered in (r, 0, 0) with width w
 * and heigth h, and rotates it along the z azis by an angle omega
 * 
 * @param w detector width, direction along z
 * @param h detector heigth, direction along y
 * @param omega rotation angle centered on z axis
 * @param r distance of detector center to origin
 */
Detector::Detector(double w, double h, double omega, double r){
    this->w = w; // Dimension along z
    this->h = h; // Dimension along y
    this->omega = omega;
    this->deltaOmega = atan(h/(2*r)); // Half aperture angle from center in y direction
    this->deltaTheta = atan(w/(2*r)); // Half aperture angle from center in z direction
    this->r = r;
    
    // Declaring vertex array (TODELTE)
    vertex = new double*[4];

    // Assigning vertes with rotations
    vertex[0] = zAxisRotation(new double[3]{r, h/2, w/2}, omega);
    vertex[1] = zAxisRotation(new double[3]{r, h/2, -w/2}, omega);
    vertex[2] = zAxisRotation(new double[3]{r, -h/2, -w/2}, omega);   
    vertex[3] = zAxisRotation(new double[3]{r, -h/2, w/2}, omega);
}

Detector::~Detector(){
    for(int i = 0; i < 4; i++){
        delete[] vertex[i];
    }
    delete[] vertex;
}

/**
 * @brief Checks if the detecor can detect a photon with a certain omega angle centered on
 * z axis. After this z axis check, another check on theta is needed.
 * 
 * @param omega Photon angle
 * @return true if the photon angle is in the detectors range
 * @return false if the photon angle is not in the detectors range
 */
bool Detector::checkOmega(double omega){ // where pi is the photon angle
    if (omega < this->omega + deltaOmega && omega > this->omega) return true; 
    return false;
}

/**
 * @brief Checks if the detector can detect a photon with a certain theta angle, taken in the local
 * frame of reference i.e. detector at angle w = 0
 * 
 * @param theta Photon angle
 * @return true if the photon angle is in the detectors range
 * @return false if the photon angle is not in the detectors range
 */
bool Detector::checkTheta(double theta){
    if (theta > min(-deltaTheta, deltaTheta) && theta < max(-deltaTheta, deltaTheta)) return true; 
    return false;
}

/**
 * @brief Getter for the 4 base vertex
 * 
 * @return 2d array of vertex
 */
double** Detector::getVertex(){
    return vertex;
}

/**
 * @brief Getter for the omega angle
 * 
 * @return the omega angle as double
 */
double Detector::getOmega(){
    return omega;
}

/**
 * @brief Getter for the deltaOmegaAngle
 * 
 * @return deltaOmega angle as double 
 */
double Detector::getDeltaOmega(){
    return deltaOmega;
}


/**
 * @brief Checks if a photon emitted from P with angle omega, theta interacts
 * with this detector
 * 
 * @param P photon origin
 * @param angles photon (omega, theta)
 * @return interaction time
 */
double Detector::interaction(double* P, double* angles){    
    // Simpler declaration of the 3 needed vertex
    double x0 = vertex[0][0];
    double y0 = vertex[0][1];
    double z0 = vertex[0][2];

    double x1 = vertex[1][0];
    double y1 = vertex[1][1];
    double z1 = vertex[1][2];

    double x2 = vertex[2][0];
    double y2 = vertex[2][1];
    double z2 = vertex[2][2];

    double x3 = vertex[3][0];
    double y3 = vertex[3][1];
    double z3 = vertex[3][2];

    
    // Declaring omega, theta
    double omega = angles[0];
    double theta = angles[1];

    // Declaring photon position
    double xp = P[0];
    double yp = P[1];
    double zp = P[2];

    // second point
    double xq = cos(omega)*sin(theta);
    double yq = sin(omega)*sin(theta);
    double zq = cos(theta);

    
    // Getting plane parameters
    double a = (y1 - y0) * (z2 - z0) -( z1 - z0) * (y2 - y1);
    double b = -(x1 - x0) * (z2-z0) + ( z1 - z0) * (x2 - x0);
    double c = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
    double d = -a*x0 - b*y0 - c*z0;

    double l = xp - xq;
    double m = yp - yq;
    double n = zp - zq;

    // Getting interaction point
    double x = (-(d*l) + b*m*xp + c*n*xp - b*l*yp - c*l*zp)/(a*l + b*m + c*n);//-(d + b*yp + c*zp - c*xp*(n/l) - b*xp*(m/l)) / (a + c*(n/l) + b*(m/l)); // (-(x2*yp*z1) + x1*yp*z2 + x2*y1*zp - x1*y2*zp - x2*xp*y1*(n/l) + x1*xp*y2*(n/l) + x2*xp*z1*(m/l) - x1*xp*z2*(m/l))/(y1*z1 - y2*z1 + y1*z2 - x2*y1*(n/l) + x1*y2*(n/l) + x2*z1*(m/l) - x1*z2*(m/l));//
    double y = (m/l)*(x-xp)+yp;//yp + (m/l)*(x-xp);//
    double z = (n/l)*(x-xp)+zp;//zp + (n/l)*(x-xp);//
    
    // DEBUG ANGLES INTERSECTION WITH A SPHERE
    /*double xk = (-(zp*(n/l)) + xp*pow((n/l),2) - yp*(m/l) + xp*pow((m/l),2) - sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2)); //3*cos(omega)*sin(theta);//(1/(2*(2 + tan(M_PI_2 - omega)*tan(M_PI_2 - omega))))*(4*xp - 2*yp*tan(M_PI_2 - omega) - 2*zp*tan(M_PI_2 - omega) - sqrt((-4*xp + 2*yp*tan(M_PI_2 - omega) + 2*zp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega) - 4*(2 + tan(M_PI_2 - omega)*tan(M_PI_2 - omega))*(2*xp*xp - 2*xp*yp*tan(M_PI_2 - omega) - 2*xp*zp*tan(M_PI_2 - omega) - tan(M_PI_2 - omega)*tan(M_PI_2 - omega) + yp*yp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega) + zp*zp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega))));
    double yk = yp - xp*(m/l) - (zp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),2)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((m/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - ((m/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));//3*sin(omega)*sin(theta);
    double zk = zp - xp*(n/l) - (zp*pow((n/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*(n/l)*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) - ((n/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));//3*cos(theta);
    string dataf =to_string(xk) + "," + to_string(yk)+"," + to_string(zk);//to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
    FileWriter::getInstance().writeData(2, dataf);
    

    xk=(-(zp*(n/l)) + xp*pow((n/l),2) - yp*(m/l) + xp*pow((m/l),2) + sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
    yk=yp - xp*(m/l) - (zp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),2)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((m/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) + ((m/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
    zk=zp - xp*(n/l) - (zp*pow((n/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*(n/l)*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + ((n/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
    dataf =to_string(xk) + "," + to_string(yk)+"," + to_string(zk);//to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
    FileWriter::getInstance().writeData(2, dataf);*/

    // Writing angles
    //FileWriter::getInstance().writeData(1, to_string(omega)+","+to_string(theta));



    //if((x <= max(x0, x2) && x >= min(x0, x2)) && (y <= max(y1, y2) && y >= min(y1, y2)) && (z <= max(z0, z1) && z >= min(z0, z1))){
    if((x <= max(max(x0, x1), max(x2, x3)) && x >= min(min(x0, x1), min(x2, x3)) && y <=max(max(y0, y1), max(y2, y3)) && y >= min(min(y0, y1), min(y2, y3)) && z <= max(max(z0, z1), max(z2, z3))&& z >= min(min(z0, z1), min(z2, z3)))){    

        // Calculating distance P1 - (x, y, z)
        double distance = sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y) + (zp-z)*(zp-z));

        // Calculating time taken in ns
        double time = distance / 0.3;

        string data2 = to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
        
        FileWriter::getInstance().writeData(3, data2);
        
        FileWriter::getInstance().writeData(1, to_string(omega)+","+to_string(theta));

        // DEBUG ANGLES INTERSECTION WITH A SPHERE
        double xk = (-(zp*(n/l)) + xp*pow((n/l),2) - yp*(m/l) + xp*pow((m/l),2) - sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2)); //3*cos(omega)*sin(theta);//(1/(2*(2 + tan(M_PI_2 - omega)*tan(M_PI_2 - omega))))*(4*xp - 2*yp*tan(M_PI_2 - omega) - 2*zp*tan(M_PI_2 - omega) - sqrt((-4*xp + 2*yp*tan(M_PI_2 - omega) + 2*zp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega) - 4*(2 + tan(M_PI_2 - omega)*tan(M_PI_2 - omega))*(2*xp*xp - 2*xp*yp*tan(M_PI_2 - omega) - 2*xp*zp*tan(M_PI_2 - omega) - tan(M_PI_2 - omega)*tan(M_PI_2 - omega) + yp*yp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega) + zp*zp*tan(M_PI_2 - omega)*tan(M_PI_2 - omega))));
        double yk = yp - xp*(m/l) - (zp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),2)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((m/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - ((m/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));//3*sin(omega)*sin(theta);
        double zk = zp - xp*(n/l) - (zp*pow((n/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*(n/l)*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) - ((n/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));//3*cos(theta);
        string dataf =to_string(xk) + "," + to_string(yk)+"," + to_string(zk);//to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
        FileWriter::getInstance().writeData(2, dataf);
        

        xk=(-(zp*(n/l)) + xp*pow((n/l),2) - yp*(m/l) + xp*pow((m/l),2) + sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
        yk=yp - xp*(m/l) - (zp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),2)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((m/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) + ((m/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
        zk=zp - xp*(n/l) - (zp*pow((n/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*pow((n/l),3))/(1 + pow((n/l),2) + pow((m/l),2)) - (yp*(n/l)*(m/l))/(1 + pow((n/l),2) + pow((m/l),2)) + (xp*(n/l)*pow((m/l),2))/(1 + pow((n/l),2) + pow((m/l),2)) + ((n/l)*sqrt(pow(r,2) - pow(yp,2) - pow(zp,2) + 2*xp*zp*(n/l) + pow(r,2)*pow((n/l),2) - pow(xp,2)*pow((n/l),2) - pow(yp,2)*pow((n/l),2) + 2*xp*yp*(m/l) + 2*yp*zp*(n/l)*(m/l) + pow(r,2)*pow((m/l),2) - pow(xp,2)*pow((m/l),2) - pow(zp,2)*pow((m/l),2)))/(1 + pow((n/l),2) + pow((m/l),2));
        dataf =to_string(xk) + "," + to_string(yk)+"," + to_string(zk);//to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
        FileWriter::getInstance().writeData(2, dataf);

        // Returning data
        return time;

    }



        //string data2 = to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
        //FileWriter::getInstance().writeData(3, data2);

    // Returning nullptr
    return 0;
}
