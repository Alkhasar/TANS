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
    string path = "data/simulation/data.dat";
    FileWriter& fileWriter = FileWriter::getInstance(path);
    

    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);
    Shape *cyl = new Cylinder(0.1, 10);
    Source *src = new Source(cyl, F18);
    

    // TEEEEEEEEEEEEEEEEEEEST
    for(int k = 0; k < 10; k++){

        int f = 4;
        // Test n2X
        Detector** det = new Detector*[f];
        for (int i = 0; i < f; i++){
            double phi = M_PI_2;
            double deltaOmega = 2*M_PI/f;
            double r = 1.;
            det[i]= new Detector(r*cos(deltaOmega*i)*sin(phi), r*sin(deltaOmega*i)*sin(phi), r*cos(phi), 0.1, deltaOmega, deltaOmega*i);
        }
        
        src->update(1, f, det);

        for(int i = 0; i < f; i++)
            delete det[i];
    }

    // Deleting source
    delete(src);


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