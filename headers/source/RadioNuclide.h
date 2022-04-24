#ifndef RADIO_NUCLIDE
#define RADIO_NUCLIDE

class RadioNuclide {
    public:
        // Constructor
        RadioNuclide(double, double);

        // Copy constructor
        RadioNuclide(const RadioNuclide&);

        // Destructor
        ~RadioNuclide();

        // Methods
        double** sample();

        // Getters
        double getN(double);

    private:
        // Private constructor to avoid empty initialization
        RadioNuclide();
        
        double lambda_ = 0;     // Decay constant
        double N0_ = 0;         // Initial Radionuclides
        double A0_ = 0;         // Activity
        double tau_ = 0;        // Half life
};
#endif