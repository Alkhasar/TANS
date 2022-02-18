#include <TMath.h>
#include <random>

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
}