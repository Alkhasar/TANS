#ifndef DETECTOR_RING
#define DETECTOR_RING

// Project includes
#include "Detector.h"

class DetectorRing{
    public:
        // Constructor and destructor
        DetectorRing(int, double, double);
        ~DetectorRing();

        // Getters
        int getNumberOfDetectors(){return n;}
        double* checkInteraction(double*, double*);
        void drawDetector();
        void addData(int, Data&);
        void saveData();

    private:
        Detector **detectors;
        double deltaOmega = 0;
        double width = 0;
        double height = 0;
        double radius = 0; 
        double n = 0;
        bool t = true;
};

#endif
