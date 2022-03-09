#include <math.h>
#include <random>
#include <TGraph2D.h>
#include <array>
#include <TCanvas.h>

using namespace std;

double** sample(){ 

    /*std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis1(0, 2*M_PI);
    std::uniform_real_distribution<> dis2(-M_PI_2, M_PI_2);*/

    double u1 = (double) random()/RAND_MAX; // (double) rand()/RAND_MAX;
    double u2 = (double) random()/RAND_MAX;

    // Omega nd theta angle sampling
    double omega = 2*M_PI*u1; // Omega 1 from 0 to pi
    double theta = acos(1-2*u2); // acos(u2);
    
    // Data organization
    double** angles = new double*[2];
    angles[0] = new double[2]{omega, theta};
    angles[1] = new double[2]{M_PI - omega, M_PI-theta};

    return angles; // Curently emitted back to back
}

int TEST(){
    int n = 100000;

    double x[n];
    double y[n];
    double z[n];

    for(int i = 0; i < n; i++){
        x[i] = 0;
        y[i] = 0;
        z[i] = 0;
    }

    for(int i = 0; i < n; i+=2){
        double** angles = sample(); 

        x[i] = cos(angles[0][0])*sin(angles[0][1]);
        y[i] = sin(angles[0][0])*sin(angles[0][1]);
        z[i] = cos(angles[0][1]);

         
        x[i + 1] = cos(angles[1][0])*sin(angles[1][1]);
        y[i + 1] = sin(angles[1][0])*sin(angles[1][1]);
        z[i + 1] = cos(angles[1][1]);
    }

    TCanvas* c = new TCanvas("c", "ESEMPIO", 200, 200, 400, 400);
    c->cd();
    TGraph2D* g = new TGraph2D(n, x, y, z);
    g->Draw("P");

    // Returning Success
    return 0;
}