// STD includes
#include <vector>
#include <string>

// External libs
#include "../../libs/loguru/loguru.cpp"

// Project includes
#include "../../headers/utils/Data.h"  
#include "../utils/FileReader.cpp"

// Root includes
#include <TFile.h>
#include <TApplication.h>
#include <TH1D.h>
#include <TList.h>
#include <TBrowser.h>
#include <TVectorD.h>
#include <TGraph2D.h>

#define DEBUG 0
#define DEBUG_POINTS 1

int analysis(double simTime, double detectorNumber){
    // ---------------------------------------------------------------------------------------------------- //
    // INITALIZATION
    // ---------------------------------------------------------------------------------------------------- //
    
    // Setting log file in logs/log.log in truncation mode (overwrite)
    loguru::add_file("logs/log.log", loguru::FileMode::Append, loguru::Verbosity_MAX);

    // Referencing input data
    const double SIM_DURATION       = simTime;
    const int    N_DET              = (int) detectorNumber;
    
    // Logging inout data
    LOG_F(INFO, "MAX SIMULATION TIME: %f DETECTOR NUMBER: %i", SIM_DURATION, N_DET);

    // FileReader instance
    FileReader& fileReader = FileReader::getInstance();

    // Saving data to vector and sorting it by time
    std::vector<Data> data = fileReader.getData(0);
    std::sort(data.begin(), data.end(),
        [](const Data& a, const Data& b){
            // Sort by time
            return a.time < b.time; 
        }
    );

    // Creating a root file to save our data
    TFile* file = new TFile("data/analysis/analysis.root", "RECREATE");

    // List for root objects
    TList* histList = new TList();

    #if DEBUG_POINTS == 1   
        // 3d plot
        TGraph2D* plot3d = new TGraph2D("data/simulation/data3.dat");
        plot3d->SetTitle("Detected particles");
        plot3d->SetName("3DPlot");
        plot3d->Write("Detected Particles");
    #endif

    // Keeping double and true coincidence
    std::vector<std::vector<Data>> doubleCoincidence;
    std::vector<std::vector<Data>> trueCoincidence;

    // Creating fixed name histograms
    TH1D* h1= new TH1D ("#VSCh", "Counts Vs Channel", N_DET, -0.5, N_DET - 0.5);
    TH1D* h2= new TH1D ("fVSCh", "Frequencies Vs Channel", N_DET, -0.5, N_DET - 0.5);

    // Adding h1 & h2 to list
    histList->Add(h1);
    histList->Add(h2);

    // Histogram vectors
    std::vector<TH1D*> doubleCoincidenceHistograms;
    std::vector<TH1D*> trueCoincidenceHistograms;

    // Adding single channel histogramm
    for(int i = 0; i < N_DET; i++){
        // Double coincidences
        std::string name = "DBL - ch. " + std::to_string(i);
        std::string title = "Double Coincidence of ch. " + std::to_string(i);
        doubleCoincidenceHistograms.push_back(new TH1D (name.c_str(), title.c_str(), N_DET, -0.5, N_DET - 0.5));
        histList->Add(doubleCoincidenceHistograms[i]);

        // True Coincidences
        name = "TRUE - ch. " + std::to_string(i);
        title = "True Coincidence of ch. " + std::to_string(i);
        trueCoincidenceHistograms.push_back(new TH1D (name.c_str(), title.c_str(), N_DET, -0.5, N_DET - 0.5));
        histList->Add(trueCoincidenceHistograms[i]);
    }

    // Vector to hold casual rate
    TVectorD casualRate(N_DET);
    TVectorD trueRate(N_DET);

    // Keeping triple coincidence only as number
    int tripleCoincidence = 0;

    // Noise Equivalent count ratio
    double NEC = 0;
    
    // ---------------------------------------------------------------------------------------------------- //
    // ANALYSIS
    // ---------------------------------------------------------------------------------------------------- //

    // --------------------- //
    // COUNT VS CHANNEL HIST //
    // --------------------- //

    // Filling counts vs channel
    for(Data d : data){
        h1->Fill((int) d.detector);
    }

    // Changing Y axis range
    h1->GetYaxis()->SetRangeUser(0, h1->GetMaximum());

    // -------------------- //
    // FREQ VS CHANNEL HIST //
    // -------------------- //

    for(int i = 0; i < N_DET; i++){
        // Getting casual rate for i-th detector
        double f = h1->GetBinContent(i + 1)/SIM_DURATION;

        // Saving it to vector and histogram
        casualRate[i] = f;

        // i+1 because TH uses bin 0 for underflow and bin length + 1 for overflow
        h2->SetBinContent(i + 1, f);
    }

    // Changing Y axis range
    h2->GetYaxis()->SetRangeUser(0, h2->GetMaximum());

    // Writing casual rate to file
    casualRate.Write("Casual Rate");

    // ------------------------------- //
    // CALCULATING DOUBLE COINCIDENCES //
    // ------------------------------- //

    for(int i = 0; i < data.size() - 4; i++){
        bool cond[4] = {false, false, false, false};
        for(int j = 0; j < 4; j++){
            cond[j] = (data[i+j+1].time - data[i+j].time) > 2; // Risoluzione dei rivelatori 250ps FWMH da rendere indipendente o dai dati
        }

        if((cond[0] == 1) && (cond[1] == 0) && (cond[2] == 1)){
            std::vector<Data> _coinc;
            _coinc.push_back(data[i]);
            _coinc.push_back(data[i+3]);
            doubleCoincidence.push_back(_coinc);
            if(data[i].event == data[i+3].event){
                trueCoincidence.push_back(_coinc);
            }

        } else if((cond[0] == 1) && (cond[1] == 0) && (cond[2] == 0) && (cond[3] == 1)){
            tripleCoincidence++;
        }
    }

    for(int i = 0; i < N_DET; i++){
        int nTrue = 0;

        // Inserting double coincidences in histograms
        for(std::vector<Data> d : doubleCoincidence){
            if(d[0].detector == i) {
                doubleCoincidenceHistograms[i]->Fill(d[1].detector);
            } else if(d[1].detector == i) {
                doubleCoincidenceHistograms[i]->Fill(d[0].detector);
            }
        }

        // Inserting true coincidences in histogram
        for(std::vector<Data> d : trueCoincidence){
            if(d[0].detector == i) {
                trueCoincidenceHistograms[i]->Fill(d[1].detector);
                nTrue++;
            } else if(d[1].detector == i) {
                trueCoincidenceHistograms[i]->Fill(d[0].detector);
                nTrue++;
            }
        }

        // Saving channels true Rate
        trueRate[i] = nTrue / SIM_DURATION;
    }
    
    // Writing true rate to file
    trueRate.Write("True Rate");

    // --- //
    // NEC //
    // --- //

    // NEC = T^2/(T + S + KR) with scatter rate S->0, K->1 for montecarlo , R->Random Count
    NEC = (trueCoincidence.size()*trueCoincidence.size())/(SIM_DURATION * doubleCoincidence.size());

    // ---------------------------------------------------------------------------------------------------- //
    // FINAL STEPS
    // ---------------------------------------------------------------------------------------------------- //

    // Adding all histograms to histograms folders
    histList->Write("Histograms", TObject::kSingleKey);

    #if DEBUG == 1
        // List all saved files
        file->ls();
    #endif

    // Logging Usefull info
    LOG_F(INFO, "TOTAL HITS: %zu", data.size());
    LOG_F(INFO, "DOUBLES: %zu", doubleCoincidence.size());
    LOG_F(INFO, "MULTIPLES: %i", tripleCoincidence);
    LOG_F(INFO, "TRUE: %zu", trueCoincidence.size());
    LOG_F(INFO, "NEC: %f", NEC);

    // Closing root
    gApplication->Terminate(0);

    // Returning no error
    return 0;
}