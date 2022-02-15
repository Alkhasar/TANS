// Root includes
#include <TH1F.h>
#include <TGraph.h>

// Project includes
#include "src/RadioNuclide.cpp"
#include "src/simulation.cpp"

/**
 * Launches program from console
 *
 * @param argc Number of elements in argv array passed by console
 * @param argv commands array passed by console
 * 
 * @returns execution state
 */
int main(){ // int argc, char *argv[] se parte da console

    
    RadioNuclide *F18 = new RadioNuclide(1./(109.771*60), 2*10e12);

    // -- INIZIO TEST

        int n = 100000;
        
        Double_t x[n], y[n];
        for(int i = 0; i < n; i++){
            float k = elementarySimulationStep(1., F18);
            x[i] = i;
            y[i] = k;
        }
        TGraph *g = new TGraph(n, x, y);
        F18->print();
        g->Draw();
    // -- FINE TEST

    return 0;   // Return 0 ("Succesful Execution") at the end of the program 
}