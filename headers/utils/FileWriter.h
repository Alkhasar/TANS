#ifndef FILEWRITER
#define FILEWRITER

// Implementazione singleton in cpp 11: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

// Includes        
#include <fstream>          // for filestreams
#include <string>
#include "Data.h"         // this file makes a large use of cpp string class

class FileWriter{
    public:
        // Singleton Pattern
        static FileWriter& getInstance();
        
        ~FileWriter();
        void writeData(int, Data);
        void writeData(int, const std::string&);
        
    private:
        // Singleton Pattern
        FileWriter(); // in thisway we can't create more than one FileWriter
        FileWriter(FileWriter const&); // Do not implent in singleton
        void operator=(FileWriter const&); // Do not implent in singleton

        // File stream
        std::fstream** file;
        static const int nFiles = 4;
        static const std::string path[nFiles];
        static const bool bin[nFiles];
};

#endif