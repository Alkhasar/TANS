// Project includes
#include "../../headers/utils/FileWriter.h"

// External Libs
#include "../../libs/loguru/loguru.hpp"

// Declaring a static constant path to the file
const std::string FileWriter::path[] = {"data/simulation/data.dat", "data/simulation/debug.dat", "data/simulation/data2.dat", "data/simulation/data3.dat"};
    
// Declaring static constant for binary file access
const bool FileWriter::bin[] = {1, 0, 0, 0};

/**
 * @brief This function alows access to the file writer instance
 * 
 * @return static file writer address
 */
FileWriter& FileWriter::getInstance(){
    // Guarantees Destruction
    static FileWriter instance;
    return instance;
}

/**
 * @brief Destroying file writer and closing file
 * 
 */
FileWriter::~FileWriter(){
    LOG_F(INFO, "Destroying file writer %p", (void *) this);
    for(int i = 0; i < nFiles; i ++){
        file[i]->close();
    }
    delete file;
}

/**
 * @brief Private constructor to avoid double initialization of file writer
 * 
 */
FileWriter::FileWriter(){
    // Changing loguru scope
    LOG_SCOPE_F(INFO, "FILE WRITER");
    LOG_F(WARNING, "File Writer Instantiated, should appear only once!");
    LOG_F(INFO, "Constructing at %p", (void*) this);

    file = new std::fstream*[nFiles];
    for(int i = 0; i < nFiles; i++){    
        LOG_F(INFO, "Opening file: %s Binary Mode: %i", path[i].c_str(), (int) bin[i]);
        file[i] = new std::fstream(path[i], (bin[i]) ? (std::fstream::in | std::fstream::out | std::fstream::trunc |  std::fstream::binary ) : (std::fstream::in | std::fstream::out | std::fstream::trunc));
        if(file[i]->is_open() == false) {
            LOG_F(ERROR, "The file at %s does not exist!\n", path[i].c_str());
            exit(1);
        }
    }
}

/**
 * @brief Method to save data inside or opened file
 * 
 * @param data a string to save
 */
void FileWriter::writeData(int i, Data data){
    if(file[i]->is_open()){
        file[i]->write((char*)&data, sizeof(Data));
    } else {
        LOG_F(ERROR, "The data file has unexpectedly closed!");
        throw std::runtime_error("The data file has unexpectedly closed!");
    }
}

/**
 * @brief Method to save data inside or opened file
 * 
 * @param data a string to save
 */
void FileWriter::writeData(int i, std::string data){
    if(file[i]->is_open()){
        *file[i] << data << std::endl;
    } else {
        LOG_F(ERROR, "The data file has unexpectedly closed!");
        throw std::runtime_error("The data file has unexpectedly closed!");
    }
}