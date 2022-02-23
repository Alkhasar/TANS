#ifndef DETECTOR
#define DETECTOR

class Detector{
    public:
        // Constructor
        Detector();
        Detector(double, double, double, double, double, double);
        bool check (double); // interaction check over omega
        void interaction (double ); // simulation of theinteraction process with the detector
        
        double getX();
        double getY();
        double getZ();

        double getOmega();
        double getH();

    private:
        double ID = 0; // identification of detector
        double x = 0, y = 0, z = 0; // cordinate of detector from the origin
        double omega = 0; // zenitale angle from the origin
        double QE = 0; // quantum efficency of detector
        double l = 0; // arch length
        double h = 0; // height of detector
        double d = 0; // depth
        double deltaOmega = 0; // aperture angle
};

#endif
