// Root includes (da togliere)
#include <TH1F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGraph2D.h>

// STD Includes
#include <random>
#include <string>
#include <iostream>

// Libs Includes
#include "libs/loguru/loguru.cpp"

// Project includes
#include "src/RadioNuclide.cpp"
#include "src/simulation.cpp"
#include "src/Shape.cpp"
#include "src/Cylinder.cpp"
#include "src/Source.cpp"
#include "src/Detector.cpp"
#include "src/FileWriter.cpp"
#include "src/DetectorRing.cpp"

using namespace std;

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
//int main(int argc, char *argv[]){ // int argc, char *argv[] se parte da console RIGA PER QUANDO TOGLIAMO ROOT
int main(){
    
    // Finchè usiamo root
    int argc = 1;
    char **argv = new char*[1];
    argv[0] = new char[26]{"PETMontecarloSimulation"};

    // Starting loguru
    loguru::init(argc, argv);
    loguru::add_file("logs/log.log", loguru::FileMode::Truncate, loguru::Verbosity_MAX);

    // Loguru Logs
    LOG_F(INFO, "Starting PET Montecarlo simulation");

    // Getting seed for srand
    long int seed = 42;

    // Loguru Logs
    LOG_F(INFO, "Choosen srand seed: %lu", seed);

    // Random number seed
    srand(seed);
    
    // File path
    FileWriter& fileWriter = FileWriter::getInstance();

    // Creating our setup
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);
    Shape *cylinder = new Cylinder(1, 1);
    Source *source = new Source(cylinder, F18);
    DetectorRing *ring = new DetectorRing(10, 2., 1);

    // Current Simulation Time
    double simulationTime = 0;
    double maxSimulationTime = 1;
    double simulationStep = 1;

    // Simulation cycle
    while(simulationTime < maxSimulationTime){

        // Evaluating number of decays
        double numberOfDecays = source->timeStepDecays(simulationTime);
        
        for(int n = 0; n < numberOfDecays; n++){
            double* P1 = source->samplePosition(); // P1 points to sampled point
            double* angles = source->sampleAngles(); // w1, w2, t1, -t1 ---> t2 = sgn(t1)*pi/2-t1
            
            for(int i = 0; i < ring->getNumberOfDetectors(); i++){              // For every detectorw
                for(int j = 0; j < 2; j ++){            // Check both photons

                    // Getting vertexes
                    double** v = ring->getDetectorVertexes(i);
                    
                    // Simpler declaration of the 3 needed vertex
                    double x0 = v[0][0];
                    double y0 = v[0][1];
                    double z0 = v[0][2];

                    double x1 = v[1][0];
                    double y1 = v[1][1];
                    double z1 = v[1][2];

                    double x2 = v[2][0];
                    double y2 = v[2][1];
                    double z2 = v[2][2];

                    // Declaring photon position
                    double xp = P1[0];
                    double yp = P1[1];
                    double zp = P1[2];

                    // Declaring omega, theta
                    double omega = angles[j];
                    double theta = angles[j+2];
                    
                        // Getting plane parameters
                    double a = (y1 - y0) * (z2 - z0) -( z1 - z0) * (y2 - y1);
                    double b = -(x1 - x0) * (z2-z0) + ( z1 - z0) * (x2 - x0);
                    double c = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
                    double d = -a*x0 - b*y0 - c*z0;

                    // Getting interaction point
                    double x = -(d + b*yp + c*zp - c*xp*tan(theta) - b*xp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
                    double y = -(-a*yp - c*yp*tan(theta) + d*tan(omega) + a*xp*tan(omega) + c*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));
                    double z = -(-a*zp + d*tan(theta) + a*xp*tan(theta) + b*yp*tan(theta) - b*zp*tan(omega)) / (a + c*tan(theta) + b*tan(omega));

                    // Rotated point for condition
                    double* pcond = new double[3]{x, y, z};
                    
                    if((pcond[0] <= max(v[0][0], v[2][0]) && pcond[0] >= min(v[0][0], v[2][0])) && (pcond[1] <= max(v[1][1], v[2][1]) && pcond[1] >= min(v[1][1], v[2][1])) && (pcond[2] <= max(v[0][2], v[1][2]) && pcond[2] >= min(v[0][2], v[1][2]))){
                        // Calculating distance P1 - (x, y, z)
                        double distance = sqrt((xp-x)*(xp-x) + (yp-y)*(yp-y) + (zp-z)*(zp-z));

                        // Calculating time taken in ns
                        double time = distance / 0.3;

                        // Getting msg to write
                        string data = to_string(xp) + "," + to_string(yp) + "," + to_string(zp) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z) + "," + to_string(time);

                        // Saving data to file
                        fileWriter.writeData(data);

                    }
                }
            }
        }

        // Adding simulation time
        simulationTime += simulationStep;
    }

    // Freeing memory
    delete ring;
    delete source;
    


    // TEEEEEEEEEEEEEEEEEEEST
    /*for(int k = 0; k < 1; k++){

        int f = 72;
        double deltaOmega =2* M_PI/f;
        
        double r = 2.;

        // Test n2X
        Detector** det = new Detector*[f];
        for (int i = 0; i < f; i++){
            det[i]= new Detector(1., 2*r*tan(deltaOmega/2), deltaOmega*i, r);//r*cos(deltaOmega*i)*sin(phi), r*sin(deltaOmega*i)*sin(phi), r*cos(phi), 0.1, deltaOmega, deltaOmega*i);
        }
        
        src->update(1, f, det);

        for(int i = 0; i < f; i++)
            delete det[i];
    }
    */
    // Deleting source

    // -- INIZIO TEST
    /*
        TCanvas* c1 = new TCanvas("c1", "F1", 100, 100, 400, 400);
        c1->cd();

        int n = 100000;
        
        Double_t x[n], y[n];

                
        // Loguru Logs
        LOG_F(INFO, "Simulating %i steps!", n);  

        for(int i = 0; i < n; i++){
            double k = elementarySimulationStep(1., F18);
            x[i] = i;
            y[i] = k;
        }

        // Loguru Logs
        LOG_F(INFO, "Generated %i simulation steps!", n); 

        TGraph *g = new TGraph(n, x, y);
        // F18->print();
        g->Draw();
          

        TCanvas* c2 = new TCanvas("c2", "F2", 100, 100, 600, 600);
        c2->cd();

        
        int n1 = 100000;

        Double_t x1[n1], y1[n1], z1[n1];
        double *k1;

        // Loguru Logs
        LOG_F(INFO, "Sampling %i points!", n1);  

        for(int i = 0; i < n1; i++){
            k1 = cyl->sample();
            x1[i] = k1[0];
            y1[i] = k1[1];
            z1[i] = k1[2];
        }

        // Loguru Logs
        LOG_F(INFO, "Sampled %i points!", n1);

        TGraph2D *g1 = new TGraph2D(n, x1, y1, z1);
        g1->GetXaxis()->SetAxisColor(kGreen);
        g1->GetYaxis()->SetAxisColor(kBlue);
        g1->GetZaxis()->SetAxisColor(kPink);

        // cyl->print();
        g1->Draw();
 
    // -- FINE TEST

    */

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}