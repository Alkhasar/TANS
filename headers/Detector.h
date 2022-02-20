#ifndef DETECTOR
#define DETECTOR

class Detector{
    public:
        // Constructor
        Detector();
        bool check (double); // interaction check
        void interaction (double ); // simulation of theinteraction process with the detector
        
    private:
        double ID = 0; // identification of detector
        double x = 0, y = 0, z = 0; // cordinate of detector from the origin
        double phi = 0; // zenitale angle from the origin
        double QE = 0; // quantum efficency of detector
        double w_t = 0; // total length
        double w_s = 0; // total sensible
        double h = 0; // height of detector
};

#endif
