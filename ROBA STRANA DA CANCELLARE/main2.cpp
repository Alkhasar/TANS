
// INCLUDES
#include <math.h>
#include <random>

#include <TCanvas.h>
#include <TH1D.h>

#include "libs/loguru/loguru.cpp"

#include "src/utils/FileWriter.cpp"
#include "src/detector/DetectorRing.cpp"
#include "src/detector/Detector.cpp"

using namespace std;

double* sampleAngles(){

    // Random generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    double u1 = dis(gen);
    double u2 = dis(gen);

    // Omega nd theta angle sampling
    double omega = 2 * M_PI * u1 ; 
    double theta = acos(1 - 2 * u2); 
    
    // Data organization
    double* angles = new double[2]{omega, theta};
    return angles;

}

int main2(){

    
    // Particle number
    int N = 100000;

    // Ring Parameters
    double detectorNumber = 4;
    double ringRadius = 6.0;
    double ringWidth = 10.0;

    // Creating file writer
    FileWriter& fileWriter = FileWriter::getInstance();

    // Creating detector ring
    DetectorRing *ring = new DetectorRing(detectorNumber, ringRadius, ringWidth);

    // Source point
    double* p = new double[3]{0, 0, 0};

    // ROOT GRAPHS
    TCanvas* c1 = new TCanvas("c1", "Angle Distribution", 200, 200, 1200, 700);
    c1->Divide(2, 2);
    TH1F* h1 = new TH1F("h1", "OMEGA", 100, 0, 2*M_PI);
    TH1F* h2 = new TH1F("h2", "THETA", 100, 0, M_PI);
    TH1F* h3 = new TH1F("h3", "CHANNELS", 72, 0, 72);

    // Setting Y axis ranges
    h1->GetYaxis()->SetRangeUser(0, 2000);
    h2->GetYaxis()->SetRangeUser(0, 2000);

    for(int n = 0; n < N; n++){
        // Sampling angles
        double* angles = sampleAngles();
        
        // Graphing angles
        h1->Fill(angles[0]);
        h2->Fill(angles[1]);

        // Check interaction 
        double* data = ring->checkInteraction(p, angles);

        if(data != nullptr){
            h3->Fill(data[0]);
            
            // Removing this delete will inevitably cause a memory leak
            delete[] data;
        }

    }

    c1->cd(1);
    h1->Draw();
    c1->cd(2);
    h2->Draw();
    c1->cd(3);
    h3->Draw();

    return 0;
}