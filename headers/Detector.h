#ifndef DETECTOR
#define DETECTOR

class Detector{
    public:
        // Double width, heigth, angle, distance center to center
        Detector(double, double, double, double);
        bool checkOmega(double);
        bool checkTheta(double);

        // Method to check interaction
        double interaction(double*, double*);

        // Getters
        double** getVertex();
        double getOmega();
        double getDeltaOmega();


    private:
    
        // Constructor private to avoid default instance
        Detector();
        
        double omega = 0;           // zenitale angle from the origin
        double QE = 0;              // quantum efficency of detector
        double w = 0;               // width
        double h = 0;               // height of detector
        double r = 0;               // distance from center to origin
        double deltaOmega = 0;      // aperture angle y
        double deltaTheta = 0;      // aperture angle xz
        double** vertex;            // List of the 4 main vertex
};

#endif
