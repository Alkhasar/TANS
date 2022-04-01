#ifndef DETECTOR
#define DETECTOR

// STD includes
#include <vector>

// Project Struct include
#include "../../headers/utils/Data.h"

// Project includes
#include "geometry/Plane.h"


class Detector {
    public:
        // Constructor
        Detector(double, double, double, double);

        // Destructor
        ~Detector();

        // Copy constructor
        Detector(const Detector&);

        // Function to add hit to buffer
        void addData(Data&);
        std::vector<Data> getData(){return data_;};
        void clearData(){data_.clear();};

        // Function to check if the detector is paralysed
        bool paralysed(double);

        // Method to check interaction
        double checkInteraction(double*, double*);


    private:
    
        // Constructor private to avoid default instance
        Detector();

        // Detector plane
        Plane* plane_;

        // Detector parameters
        double width_ = 0, heigth_ = 0;
        double phi_ = 0;
        double deltaPhi_ = 0;
        double distance_ = 0;

        // Data vector to check paralysed state
        std::vector<Data> data_;

        // Time of Last hit on the detector in ns
        double lastHit_ = 0;

        // Detector resolution in ns 
        double timeResolution_ = 0.;  
};   

#endif