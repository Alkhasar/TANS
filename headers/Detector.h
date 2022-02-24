#ifndef DETECTOR
#define DETECTOR

class Detector{
    public:
        // Constructor
        Detector();
        // Double width, heigth, angle, distance center to center
        Detector(double, double, double, double);
        bool checkOmega(double); // interaction check over omega
        bool checkTheta(double);
        void interaction (double ); // simulation of theinteraction process with the detector
        
        // Getter
        double getX();
        double getY();
        double getZ();
        double getOmega();
        double getH();
        double** getVertex();
        double getDeltaOmega(){return deltaOmega;};

        // Local Coordinates
        double* transformToLocalCoordinates(double, double, double);
        double transformToLocalOmega(double);

        // Global coordinates
        double* returnToGlobalCoordinates(double, double, double);
        double returnToGlobalOmega(double);


    private:
        double ID = 0; // identification of detector
        double x = 0, y = 0, z = 0; // cordinate of detector from the origin
        double omega = 0; // zenitale angle from the origin
        double QE = 0; // quantum efficency of detector
        double w = 0; // width
        double h = 0; // height of detector
        double l = 0; // arc length
        double r = 0;
        double deltaOmega = 0; // aperture angle y
        double deltaTheta = 0; // aperture angle xz
        double** vertex;
};

#endif
