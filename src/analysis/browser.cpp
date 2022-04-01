// STD includes
#include <string>

// Root includes
#include <TBrowser.h>
#include <TFile.h>

TBrowser* browser(){
    TFile* file = new TFile("../../data/analysis/analysis.root");
    TBrowser* browser = new TBrowser("PET Data Analysis", file);
    return browser;
}