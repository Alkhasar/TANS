// STD includes
#include <algorithm>
#include <vector>
#include <TCanvas.h>
#include <TH1D.h> 
#include <iostream>
#include <string>

// Project iclujdes
#include "headers/utils/Data.h"  
#include "src/utils/FileReader.cpp"
#include "src/utils/FileWriter.cpp"
#include "src/Time.cpp"

// External libs
#include "libs/loguru/loguru.cpp"

using namespace std;

void analysis(){
    FileReader& fileReader= FileReader::getInstance();
    // A monte suddividere i file in n file da (an, an+adeltaN)
    vector<Data> d = fileReader.getData(0);
    sort(d.begin(), d.end(), [](const Data& a, const Data& b){
        
        //return a.event < b.event;
        return a.time < b.time; // Verifica se Data a o Data b è piu grande
    });

    
    double x = 200; // top left coordinate 
    double y = 200; // top left coordinate
    double w = 1200; // measurement in pixels 
    double h = 450; // height measurement of canvas

    TCanvas* c1 = new TCanvas ("c1", "Analisi Monte Carlo", x, y, w, h);
    c1->Divide(2);
    c1->cd(1); 

    TH1D* h1= new TH1D ("h1", "Istogramma ConteggiVsCanale", 200, -0.5, 199.5);
    h1->GetYaxis()->SetRangeUser(0, 5000);

    for(Data dato : d ){
        //cout << dato.event << " " << (int) dato.detector << " " << dato.time << endl;
        int d = (int) dato.detector;
        h1->Fill(d);
    }

    h1->Draw();

    // Second part
    c1->cd(2);

    TH1D* h2= new TH1D ("h2", "Istogramma FrequenzeVsCanale", 200, -0.5, 199.5);
    h2->GetYaxis()->SetRangeUser(0, 60e6);
    vector<double> casualRate;

    for(int i = 0; i < 200; i++){
        casualRate.push_back(h1->GetBinContent(i)/5e-5);
        h2->AddBinContent(i, h1->GetBinContent(i)/5e-5);
    }

    h2->Draw();

    // Filling the third slot
    c1->cd(3);

    vector<Data*> trueCoincidence;
    long int rateTriple = 0;
    long int rateDouble = 0;
    
    for(int i = 0; i < d.size() - 4; i++){ // Here, we are loosing 5 potenital hits
        bool cond[4] = {false, false, false, false};
        for(int j = 0; j < 4; j++){
            cond[j] = (d[i+j+1].time - d[i+j].time) > 0.2; // Risoluzione dei rivelatori da rendere indipendente o dai dati
            //cout << d[i+j+1].time - d[i+j].time<< endl;
        }

        if((cond[0] == 1) && (cond[1] == 0) && (cond[2] == 1)){
            Data* _coinc = new Data[2]{d[0], d[2]};
            trueCoincidence.push_back(_coinc);
            rateDouble++;

        } else if((cond[0] == 1) && (cond[1] == 0) && (cond[2] == 0) && (cond[3] == 1)){
            rateTriple++;
        }
    }

    cout << "TOTAL DOUBLE: " << rateDouble << endl; 
    cout << "TOTAL TRIPLE: " << rateTriple << endl;
    cout << "RATE DOUBLE: " << rateDouble/Time::getTime().getMaxSimualationTime() << endl;
    cout << "RATE TRIPLE: " << rateTriple/Time::getTime().getMaxSimualationTime() << endl;



}