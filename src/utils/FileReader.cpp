// Project includes
#include "../../headers/utils/FileReader.h"

// External Libs
#include "../../libs/loguru/loguru.hpp"
#include <iostream>

// Declaring a static constant path to the file
const std::string FileReader::path[] = {"data/simulation/data0.dat"};

// Declaring static constant for binary file access
const bool FileReader::bin[] = {1};

/**
 * @brief This function alows access to the file writer instance
 * 
 * @return static file writer address
 */
FileReader& FileReader::getInstance(){
    // Guarantees Destruction
    static FileReader instance;
    return instance;
}

/**
 * @brief Destroying file writer and closing file
 * 
 */
FileReader::~FileReader(){
    LOG_F(INFO, "Destroying file reader %p", (void *) this);
    for(int i = 0; i < nFiles; i ++){
        file[i]->close();
    }
    delete file;
}

/**
 * @brief Private constructor to avoid double initialization of file writer
 * 
 */
FileReader::FileReader(){
    // Changing loguru scope
    LOG_SCOPE_F(INFO, "FILE Reader");
    LOG_F(WARNING, "File Reader Instantiated, should appear only once!");
    LOG_F(INFO, "Constructing at %p", (void*) this);

    file = new std::fstream*[nFiles];
    for(int i = 0; i < nFiles; i++){    
        LOG_F(INFO, "Opening file: %s Binary Mode: %i", path[i].c_str(), (int) bin[i]);
        file[i] = new std::fstream(path[i], (bin[i]) ? (std::fstream::in  |  std::fstream::binary ) : (std::fstream::in));
        if(file[i]->is_open() == false) {
            LOG_F(ERROR, "The file at %s does not exist!\n", path[i].c_str());
            exit(1);
        }

        // Temp vector
        std::vector<Data> _;

        // Copies all data into buffer
        while (!file[i]->eof()){
            Data dato;
            file[i]->read((char*)&dato, sizeof(dato));
            _.push_back(dato);
        }
        
        // Pushing _ into data
        data.push_back(_);
    }
}

std::vector<Data> FileReader::getData(int i){
    return data[i];
}