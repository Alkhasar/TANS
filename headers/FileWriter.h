#ifndef FILEWRITER
#define FILEWRITER

// Implementazione singleton in cpp 11: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

// Includes        
#include <fstream>          // for filestreams
#include <string.h>         // this file makes a large use of cpp string class

class FileWriter{
    public:
        // Singleton Pattern
        static FileWriter& getInstance();
        
        ~FileWriter();
        void writeData(std::string);
        
    private:
        // Singleton Pattern
        FileWriter();
        FileWriter(FileWriter const&); // Do not implent in singleton
        void operator=(FileWriter const&); // Do not implent in singleton

        // File stream
        std::fstream* file;
        static const std::string path;
};

#endif