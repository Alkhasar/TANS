// Root includes (da togliere)
#include <TH1F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TGraph2D.h>

// Includes
#include <random>

// Project includes
#include "src/RadioNuclide.cpp"
#include "src/simulation.cpp"
#include "src/Shape.cpp"
#include "src/Cylinder.cpp"

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
int main(){ // int argc, char *argv[] se parte da console

    // Random number seed
    srand(42);
    
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);
    Shape *cyl = new Cylinder(3, 10);

    // -- INIZIO TEST

        TCanvas* c1 = new TCanvas("c1", "F1", 100, 100, 400, 400);
        c1->cd();

        int n = 100000;
        
        Double_t x[n], y[n];
        for(int i = 0; i < n; i++){
            double k = elementarySimulationStep(1., F18);
            x[i] = i;
            y[i] = k;
        }
        TGraph *g = new TGraph(n, x, y);
        F18->print();
        g->Draw();

        TCanvas* c2 = new TCanvas("c2", "F2", 100, 100, 600, 600);
        c2->cd();

        
        Double_t x1[n], y1[n], z1[n];
        double *k1;
        for(int i = 0; i < n; i++){
            k1 = cyl->sample();
            x1[i] = k1[0];
            y1[i] = k1[1];
            z1[i] = k1[2];
        }
        TGraph2D *g1 = new TGraph2D(n, x1, y1, z1);
        g1->GetXaxis()->SetAxisColor(kGreen);
        g1->GetYaxis()->SetAxisColor(kBlue);
        g1->GetZaxis()->SetAxisColor(kPink);


        cyl->print();
        g1->Draw();
    // -- FINE TEST

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}