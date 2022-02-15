#include <TMath.h>
#include <random>

float rejection(double mu, double sigma, double max){
    srand(1);
    float x, y;
    do{
        float u1 = rand()/RAND_MAX;
        float u2 = rand()/RAND_MAX;

        x = -3.0*sigma + (6*sigma)*u1;
        y = max*u1;
    } while(y > max*TMath::Gaus(x, mu, sigma, true));
    return max*TMath::Gaus(x, mu, sigma, true);
}