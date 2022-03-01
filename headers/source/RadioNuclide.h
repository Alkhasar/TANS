#ifndef RADIO_NUCLIDE
#define RADIO_NUCLIDE

class RadioNuclide {
    public:
        // Constructor
        ~RadioNuclide();
        RadioNuclide(double, double);
        RadioNuclide(const RadioNuclide&);

        // Methods
        double **sample();

        // Getters
        double getLambda();
        double getN(double);
        double getA(double);
        double getTau();
        double getElapsedTime();
        double getConfidency();

        //Setters
        void addElapsedTime(double);

    private:
        // Private constructor to avoid empty initialization
        RadioNuclide();
        
        double lambda = 0;  // Decay constant
        double N_0 = 0;       // Initial Radionuclides
        double A_0 = 0;       // Activity
        double tau = 0;     // Half life
        double confidency = 0.05; // Gaussian distribution sigma (std of activity in %) 
        double elapsedTime = 0; // Elapsed time
};
#endif