#ifndef RADIO_NUCLIDE
#define RADIO_NUCLIDE

#include <iostream>
using namespace std;

class RadioNuclide {
    public:
        // Constructor
        RadioNuclide(double, double);

        // Getters
        double getLambda();
        double getA();
        double getTau();
        double getConfidency();

        // Setters
        void setA(double);

        // Printing radionuclide data
        void print();

    private:
        double lambda = 0;  // Decay constant
        double A = 0;       // Activity
        double tau = 0;     // Half life
        double confidency = 0.05; // Gaussian distribution sigma (std of activity in %) 
};
#endif