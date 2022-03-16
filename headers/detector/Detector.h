#ifndef DETECTOR
#define DETECTOR

#include "../../headers/utils/Data.h"
#include <vector>

class Detector{
    public:
        // Constructor Destructor
        Detector(double, double, double, double);
        ~Detector();
        Detector(Detector const&);
        void operator=(Detector const&);

        // Method to check interaction
        double interaction(double*, double*);

        // Getters
        double** getVertex();
        double* getVertex(int);
        double getOmega();
        double getDeltaOmega();

        // Function to sample a point on the detector, used for debug purpose
        double* samplePoint();

        // Function to check if the detector is paralysed
        bool paralysed(double);

        // Function to add hit to buffer
        void addData(Data&);
        vector<Data> getData(){return data;};
        void clearData(){data.clear();};

    private:
    
        // Constructor private to avoid default instance
        Detector();

        double a=0, b=0, c=0, d=0;  // Detector Plane Parameters
        double lastTimeHit = 0;     // Time of Last hit on the detector
        double omega = 0;           // Zenital angle from the origin
        double w = 0, h = 0;        // Detector width and heigth
        double r = 0;               // Distance from the detector center to Origin
        double deltaOmega = 0;      // Aperture angle y
        double deltaTheta = 0;      // Aperture angle xz
        double timeResolution = 2;  // Detector resolution in ns 

        double* center;             // Detector center coordinater
        double** vertex;            // List of the 4 main vertex
        double* normalVector;       // Detector plane normal vector
        vector<Data> data;
};              

#endif
