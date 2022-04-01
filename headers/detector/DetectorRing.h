#ifndef DETECTOR_RING
#define DETECTOR_RING

// Project includes
#include "Detector.h"

class DetectorRing{
    public:
        // Constructor
        DetectorRing(int, double, double);

        // Destructor
        ~DetectorRing();

        // Getters
        double* checkInteraction(double*, double*);
        void drawDetector();
        void addData(int, Data&);
        void saveData();

    private:
        // Default constructor removed to avoid unwanted intialization
        DetectorRing();

        // Detectors array
        Detector **detectors_;

        // Angle distance between detectors
        double deltaPhi_ = 0;

        // Ring data
        double width_ = 0, height_ = 0, radius_ = 0;
        int nDetectors_ = 0;
};

#endif