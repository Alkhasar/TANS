#ifndef FILEREADER
#define FILEREADER

// STD includes
#include <fstream>
#include <string>
#include <vector>

// Project structs
#include "Data.h"

class FileReader{
    public:
        // Singleton Pattern
        static FileReader& getInstance();
        
        ~FileReader();
        std::vector<Data> getData(int);
        
    private:
        // Singleton Pattern
        FileReader(); // in thisway we can't create more than one FileWriter
        FileReader(FileReader const&); // Do not implent in singleton
        void operator=(FileReader const&); // Do not implent in singleton

        // File stream
        std::fstream** file;
        static const int nFiles = 1;
        static const std::string path[nFiles];
        static const bool bin[nFiles];
        
        // 2D Vector data
        std::vector<std::vector<Data>> data;


};


#endif