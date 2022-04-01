// STD includes
#include <math.h>
#include <vector>
#include <iostream>

// External includes
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/detector/DetectorRing.h"
#include "../../headers/utils/FileWriter.h"
#include "../../headers/utils/Data.h"

/**
 * @brief Construct a detector Ring taking a number n of detectors to arange around a ring of radius r.
 * 
 * @param n number of detectors
 * @param radius radius of the detector ring
 * @param width detectors width (along z)
 */
DetectorRing::DetectorRing(int n, double radius, double width){
    // Changing log scope
    LOG_SCOPE_F(INFO,"DETECTOR RING");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);

    //Asigning and logging parameters
    nDetectors_ = n;
    LOG_F(INFO, "Number of detectors: %i", nDetectors_);

    deltaPhi_ = 2 * M_PI / nDetectors_;
    LOG_F(INFO, "Delta Omega: %frad", deltaPhi_);

    width_ = width;
    LOG_F(INFO, "Detectors width: %fm", width_);

    radius_ = radius;
    LOG_F(INFO, "Detectors center distance from origin: %fm", radius_);

    height_ = 2 * radius_ * tan(deltaPhi_/2);
    LOG_F(INFO, "Detectors height: %fm", height_);

    detectors_ = new Detector*[nDetectors_];
    for(int i = 0; i < nDetectors_; i++){
        LOG_F(INFO, "Generating Detector N°: %i", i);
        detectors_[i] = new Detector(width_, height_, deltaPhi_*i, radius_);
    }
}

/**
 * @brief Destroy the Detector Ring and free detectors memory
 * 
 */
DetectorRing::~DetectorRing(){
    LOG_F(INFO, "Destroying detectors ring %p", (void*) this);
    delete[] detectors_;
}

/**
 * @brief Checks if a given photon interacts with any of the the detectors
 * 
 * @param P emitted photon position
 * @param angles emitted photon angles
 * @return detector number and time
 */
double* DetectorRing::checkInteraction(double* P, double* angles){
    // For every detector in the detector ring
    for(int i = 0; i < nDetectors_; i++){
        //Retrieve interaction deltaT from creation to hit
        double time = detectors_[i]->checkInteraction(P, angles); // Outside of debug should be only time
        if(time != 0){
            // if a time has been given return it with detector number
            return (new double[2]{(double) i, time});
        }
    }
    return nullptr;
}

/**
 * @brief Adding the hit to the detector
 * 
 * @param i ith detector in the ring
 * @param data data to add
 */
void DetectorRing::addData(int i, Data& data){
    detectors_[i]->addData(data);
}

/**
 * @brief checks for valid data in every detectors and saves it
 * 
 */
void DetectorRing::saveData(){
    // File writer instance
    FileWriter& fileWriter = FileWriter::getInstance();

    // Checks conditions over every detector
    for(int i = 0; i < nDetectors_; i++){
        std::vector<Data> data = detectors_[i]->getData();
        std::sort(data.begin(), data.end(), 
            // Lambda Function to sort data based on Data.time
            [](const Data& a, const Data& b){
                return a.time < b.time; 
            }
        );
        // For every hit on the detector
        for(Data d : data){
            // Save data if it is not paralysed
            if(!detectors_[i]->paralysed(d.time)){
                fileWriter.writeData(0, d);
            }
        }

        // Clear detectors data
        detectors_[i]->clearData();
    }
}