#include "../headers/Cylinder.h"
#include "../headers/RadioNuclide.h"
#include "../headers/Source.h"
#include "../libs/loguru/loguru.hpp"
#include "../headers/Detector.h"

#include "../headers/FileWriter.h"

// #include "utilities.cpp"

#include <math.h>
#include <string>

using namespace std;

Source::Source(Shape * shape, RadioNuclide * radioNuclide){
    
    // Changing log scope
    LOG_SCOPE_F(INFO,"SOURCE");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    // Logging shape
    this->shape = shape;
    LOG_F(INFO,"shape: %p", (void*) (this->shape));

    // Logging radionuclide
    this->radioNuclide = radioNuclide;
    LOG_F(INFO,"radionuclide: %p", (void*) (this->radioNuclide));
};

Source::Source(const Source& src):
    x(src.x),
    y(src.y),
    z(src.z),
    compton(src.compton)
{   
    LOG_F(INFO, "Copying %p in new object", &src);

    // Copying important elements
    this->shape = src.shape->clone();
    this->radioNuclide = new RadioNuclide(*src.radioNuclide);
};

Source::~Source(){
    LOG_F(INFO, "Destroying Source Object %p", this);

    // Destroying shape and radionuclide
    delete this->shape;
    delete this->radioNuclide;
};

int Source::timeStepDecays(double time){//, int nRiv, Detector** riv){
    
    // Getting timestep
    double dt = time - oldEvaluationTime;

    // Number of decays (temporay)
    double numberOfDecays = 10000;//gaussianRejection(0, radioNuclide->getConfidency(), radioNuclide->getA(radioNuclide->getElapsedTime()));

    // Adjust radionuclide activity
    radioNuclide->addElapsedTime(dt); 

    // Returning number of decays
    return numberOfDecays;

};

/**
 * @brief Samples a point from the given shape
 * 
 * @return {x, y, z} of the sampled point
 */
double* Source::samplePosition(){
    return shape->sample();
}

/**
 * @brief samples 2 angles from the radionuclide
 * 
 * @return {omega, theta} of the sampled decay
 */
double* Source::sampleAngles(){
    return radioNuclide->sample();
}

/*
    // Getting file writer instance
    FileWriter& fileWriter = FileWriter::getInstance();
    string data;

    for(int d = 0; d < numberOfDecays; d++){
        double* P1 = shape->sample(); // P1 points to sampled point
        double* angles = radioNuclide->sample(); // w1, w2, t1, -t1 ---> t2 = sgn(t1)*pi/2-t1
        double time;
        
        for(int i = 0; i < nRiv; i++){              // For every detectorw
            for(int j = 0; j < 2; j ++){            // Check both photons
                        // Getting vertexes
                        double** v = riv[i]->getVertex();
                        
                        // Simpler declaration of the 3 needed vertex
                        double x0 = v[0][0];
                        double y0 = v[0][1];
                        double z0 = v[0][2];

                        double x1 = v[1][0];
                        double y1 = v[1][1];
                        double z1 = v[1][2];

                        double x2 = v[2][0];
                        double y2 = v[2][1];
                        double z2 = v[2][2];

                        // Declaring photon position
                        double xp = P1[0];
                        double yp = P1[1];
                        double zp = P1[2];

                        // Declaring omega, theta
                        double omega = angles[j];
                        double theta = angles[j+2];
                        
                              // Getting plane parameters
                        double a = (y1 - y0) * (z2 - z0) -( z1 - z0) * (y2 - y1);
                        double b = -(x1 - x0) * (z2-z0) + ( z1 - z0) * (x2 - x0);
                        double c = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
                        double d = -a*x0 - b*y0 - c*z0;

                        // Getting interaction point
                        double x = -(d + b*yp + c*zp - c*xp*tan(theta) - b*xp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
                        double y = -(-a*yp - c*yp*tan(theta) + d*tan(omega) + a*xp*tan(omega) + c*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
                        double z = -(-a*zp + d*tan(theta) + a*xp*tan(theta) + b*yp*tan(theta) - b*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));

                        // Rotated point for condition
                        double* pcond = new double[3]{x, y, z};
                        
                        if((pcond[0] <= max(v[0][0], v[2][0]) && pcond[0] >= min(v[0][0], v[2][0])) && (pcond[1] <= max(v[1][1], v[2][1]) && pcond[1] >= min(v[1][1], v[2][1])) && (pcond[2] <= max(v[0][2], v[1][2]) && pcond[2] >= min(v[0][2], v[1][2]))){
                            // Calculating distance P1 - (x, y, z)
                            double distance = sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y) + (zp-z)*(zp-z));

                            // Calculating time taken
                            double time = distance / 0.3;

                            // Getting msg to write
                            data = to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);

                            string msg = "INTERACTION FROM P1: (" + to_string(xp) + ", " + to_string(yp) + " ," + to_string(zp) + ")m that took a time t = " +to_string(time) + "ns at det" + to_string(i);

                            // Logging interaction
                            LOG_F(INFO, "%s", msg.c_str());
                            fileWriter.writeData(data);


                    
                    }
                
            }
        }
    }
    */