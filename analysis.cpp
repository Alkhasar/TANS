// STD includes
#include <algorithm>
#include <vector>
#include <TCanvas.h>
#include <TH1D.h> 
#include <iostream>

// Project iclujdes
#include "headers/utils/Data.h"  
#include "src/utils/FileReader.cpp"

// External libs
#include "libs/loguru/loguru.cpp"

using namespace std;

void analysis(){
    FileReader& fileReader= FileReader::getInstance();
    // A monte suddividere i file in n file da (an, an+adeltaN)
    vector<Data> d = fileReader.getData(0);
    sort(d.begin(), d.end(), [](const Data& a, const Data& b){
        return a.time < b.time; // Verifica se Data a o Data b è piu grande
    });
    
    double x = 200; // top left coordinate 
    double y = 200; // top left coordinate
    double w = 600; // measurement in pixels 
    double h = 450; // height measurement of canvas
    TCanvas* c1 = new TCanvas ("c1", "Analisi Monte Carlo", x, y, w, h);
    c1->Divide(1);
    c1->cd(0); 

    TH1D* h1= new TH1D ("h1", "Istogramma ConteggiVsCanale", 72, -0.5, 71.5);
    for(Data dato : d ){
        h1->Fill(dato.detector);
    }

    h1->Draw();
    // Caricare i file in memoria

    // Riordinarli con il quick sort oppure il merge sort

    // Verificare se ho coincidenze vere

}