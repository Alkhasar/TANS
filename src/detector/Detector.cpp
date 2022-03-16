// Project includes
#include "../../headers/detector/Detector.h"
#include "../../headers/utils/FileWriter.h"
#include "../../src/utils/utilities.cpp"
#include "../../headers/Time.h"

// std includes
#include <math.h>
#include <iostream>
#include <random>

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
    // Changing log scope
    LOG_SCOPE_F(INFO,"DETECTOR");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);
    
    this->w = w; // Dimension along z
    LOG_F(INFO, "Width: %lfm", w);

    this->h = h; // Dimension along y
    LOG_F(INFO, "Heigth: %lfm", h);

    this->omega = omega;
    LOG_F(INFO, "Omega: %lfrad", omega);

    this->deltaOmega = atan(h/(2*r)); // Half aperture angle from center in y direction
    LOG_F(INFO, "Width: %lfrad", deltaOmega);

    this->deltaTheta = atan(w/(2*r)); // Half aperture angle from center in z direction
    LOG_F(INFO, "DeltaTheta: %lfrad", deltaTheta);

    this->r = r;
    LOG_F(INFO, "Radius: %lfm", r);

    // Declaring vertex array
    vertex = new double*[4];

    // Assigning vertes with rotations
    vertex[0] = zAxisRotation(new double[3]{r, h/2, w/2}, omega);
    vertex[1] = zAxisRotation(new double[3]{r, h/2, -w/2}, omega);
    vertex[2] = zAxisRotation(new double[3]{r, -h/2, -w/2}, omega);   
    vertex[3] = zAxisRotation(new double[3]{r, -h/2, w/2}, omega);

    for(int i = 0; i < 4; i++){
        LOG_F(INFO, "Detector Vertex: v%i=(%lf, %lf, %lf)", i, vertex[i][0], vertex[i][1], vertex[i][2]);
    }


    // Getting plane parameters
    a = (vertex[1][1] - vertex[0][1]) * (vertex[2][2] - vertex[0][2]) -( vertex[1][2] -vertex[0][2]) * (vertex[2][1] - vertex[1][1]);
    b = -(vertex[1][0] - vertex[0][0]) * (vertex[2][2]- vertex[0][2]) + ( vertex[1][2] - vertex[0][2]) * (vertex[2][0] - vertex[0][0]);
    c = (vertex[1][0] - vertex[0][0]) * (vertex[2][1] - vertex[0][1]) - (vertex[1][1] - vertex[0][1]) * (vertex[2][0] - vertex[0][0]);
    d = -a*vertex[0][0] - b*vertex[0][1] - c*vertex[0][2];
    LOG_F(INFO, "Detector Plane parameters: a=%lf b=%lf c=%lf d=%lf)", a, b, c, d);
    
    // Detector center
    center = zAxisRotation(new double[3]{r, 0, 0}, omega);
    LOG_F(INFO, "Center Point: (%lf, %lf, %lf)", center[0], center[1], center[2]);
    

    // Plane normal vector
    double norm = sqrt(a*a+b*b+c*c);
    this->normalVector = new double[3]{a/norm, b/norm, c/norm};
    LOG_F(INFO, "Normal Vector: n=(%lf, %lf, %lf)", normalVector[0], normalVector[1], normalVector[2]);
    
}

/**
 * @brief Destroy the Detector and clear dynamic memory
 * 
 */
Detector::~Detector(){
    LOG_F(INFO, "Destroying Detector %p", (void*) this);
    for(int i = 0; i < 4; i++){
        delete[] vertex[i];
    }
    delete[] vertex;
    delete[] normalVector;
    delete[] center;
}

/**
 * @brief Detector copy constructor
 * 
 * @param src The source detector to copy
 */
Detector::Detector(const Detector& src):
    w(src.w),
    h(src.h),
    omega(src.omega),
    deltaOmega(src.deltaOmega),
    deltaTheta(src.deltaTheta),
    r(src.r),
    a(src.a),
    b(src.b),
    c(src.b),
    d(src.b)
{   
    
    LOG_F(INFO, "Copying detector %p into %p", (void*) &src, (void*) this);

    // Declaring dynamic arrays
    vertex = new double*[4];
    center = new double[3];
    normalVector = new double[3];

    // Assigning values to vertex
    for(int i = 0; i < 4; i++){
        vertex[i] = src.vertex[i];
    }

    // Assigning values to center and normal vector
    for(int i = 0; i < 3; i++){
        center[i] = src.center[i];
        normalVector[i] = src.normalVector[i];
    }
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
 * @brief Getter for the 4 base vertex
 * 
 * @return 2d array of vertex
 */
double* Detector::getVertex(int i){
    return vertex[i];
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
 * @brief Adding a hit to buffer
 * 
 * @param data to add
 */
void Detector::addData(Data& data){
    this->data.push_back(data);
}

/**
 * @brief Checks if the detector is paralysed by last signal
 * 
 * @param time in nanosecond
 * @return true if deltaTime > 2ns
 * @return false if deltaTime < 2ns
 */
bool Detector::paralysed(double time){
    if(time - lastTimeHit > timeResolution){
        lastTimeHit = time;
        return false;
    }
    return true;
}

/**
 * @brief Method to return a random point on the surface
 * 
 * @return double* 
 */
double* Detector::samplePoint(){
    double X = r;
    double Y = -h/2 + ((double) rand()/RAND_MAX)*(h);
    double Z = -w/2 + ((double) rand()/RAND_MAX)*(w);  

    return zAxisRotation(new double[3]{X, Y, Z}, omega);
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
    
    // Declaring omega, theta
    double omega = angles[0];
    double theta = angles[1];
    
    // Radius for angles
    double k = 2*sqrt(P[0]*P[0]+P[1]*P[1]+P[2]*P[2]);

    // second point
    double xq = k*cos(omega)*sin(theta);
    double yq = k*sin(omega)*sin(theta);
    double zq = k*cos(theta); 

    double norm = sqrt((xq - P[0])*(xq - P[0])+(yq - P[1])*(yq - P[1])+(zq - P[2])*(zq - P[2]));

    // Line director (normalised)
    double l = (xq - P[0])/norm;
    double m = (yq - P[1])/norm;
    double n = (zq - P[2])/norm;

    // Parameter
    double t = (-d - a*P[0] - b*P[1] - c*P[2])/(a*l + b*m + c*n);

    // Getting interaction point
    double x = P[0] + l * t;
    double y = P[1] + m * t;
    double z = P[2] + n * t;

    if((x <= max(vertex[0][0], vertex[2][0]) && x >= min(vertex[0][0], vertex[2][0])) && (y <= max(vertex[1][1], vertex[2][1]) && y >= min(vertex[1][1], vertex[2][1])) && (z <= max(vertex[0][2], vertex[1][2]) && z >= min(vertex[0][2], vertex[1][2]))){

        // Gamma angle to check collision in th right direction
        double gamma = acos(l*normalVector[0] + m*normalVector[1] + n*normalVector[2]);
        if(gamma <= M_PI_2 && gamma >= -M_PI_2){
            // Distance between point and detector
            double distance = sqrt((P[0]-x)*(P[0]-x) + (P[1]-y)*(P[1]-y) + (P[2]-z)*(P[2]-z));

            // Calculating time taken in ns
            double time = distance / 0.3;

            // Checking if this detector is paralysed
            if(paralysed(time)){
                // Saving some debug data
                string dataA = to_string(P[0]) + "," + to_string(P[1]) + "," + to_string(P[2]) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
                FileWriter::getInstance().writeData(3, dataA);

                // Returning data
                return time;
            }
        }
    }

    // Returning 0
    return 0;
}