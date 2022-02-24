#include <TMath.h>
#include <math.h>
#include <random>

using namespace std;

double gaussianRejection(double, double, double);
double* zAxisRotation(double*, double);

#ifndef UTILS
#define UTILS

/**
 * @brief this function is for the gaussian rejection
 * 
 * @param mu is the mean value of the gaussian distribution
 * @param sigma is the std of the gaussian distribution
 * @param max is the maximum of the box surrounding the gaussian function
 * @return double 
 */

double gaussianRejection(double mu, double sigma, double max){ 
    double x, y;
    do{
        double u1 = rand()/RAND_MAX;
        double u2 = rand()/RAND_MAX;

        x = -3.0*sigma + (6*sigma)*u1;
        y = max*u1;
        
    } while(y > max*TMath::Gaus(x, mu, sigma, true));
    return max*TMath::Gaus(x, mu, sigma, true);
};

/**
 * @brief Takes a point and an angle, returns the same point rotated by omega
 * along the z axis
 * 
 * @param p point
 * @param omega rotation angle
 * @return rotated point
 */
double* zAxisRotation(double* p, double omega){
    return new double[3]{p[0] * cos(omega) - p[1] * sin(omega), p[0] * sin(omega) + p[1] * cos(omega), p[2]};
};

#endif