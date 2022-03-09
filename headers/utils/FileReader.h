#ifndef FILEREADER
#define FILEREADER

// Implementazione singleton in cpp 11: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

// Includes        
#include <fstream>          // for filestreams
#include <string>
#include "Data.h"         // this file makes a large use of cpp string class
#include <vector>
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