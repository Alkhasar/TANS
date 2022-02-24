#ifndef DETECTOR_RING
#define DETECTOR_RING

#include "Detector.h"

class DetectorRing{
    public:
        DetectorRing(int, double, double);
        ~DetectorRing();

        // Getters
        int getNumberOfDetectors(){return n;}
        double** getDetectorVertexes(int);

    private:
        Detector **detectors;
        double deltaOmega = 0, w = 0, h = 0, r = 0, n = 0;
};

#endif
