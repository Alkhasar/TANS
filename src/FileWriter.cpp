#include "../headers/FileWriter.h"
#include "../libs/loguru/loguru.hpp"

// Declaring a static constant path to the file
const std::string FileWriter::path = "data/simulation/data.txt";

/**
 * @brief This function alows access to the file writer instance
 * 
 * @return static file writer address
 */
FileWriter& FileWriter::getInstance(){
    // Guarantees Destruction
    static FileWriter instance;
    return instance;
};

/**
 * @brief Destroying file writer and closing file
 * 
 */
FileWriter::~FileWriter(){
    LOG_F(INFO, "Destroying file reader!");
    file->close();
    delete file;
};

/**
 * @brief Private constructor to avoid double initialization of file writer
 * 
 */
FileWriter::FileWriter(){
    //string path = "data/simulation/data.dat";
    LOG_F(WARNING, "File Writer Instantiated, should appear only once!");
    LOG_F(INFO, "Opening file: %s", path.c_str());
    file = new std::fstream(FileWriter::path, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if(file->is_open() == false) {
		LOG_F(ERROR, "The file at %s does not exist!\n", path.c_str());
		exit(1);
    }
};

/**
 * @brief Method to save data inside or opened file
 * 
 * @param data a string to save
 */
void FileWriter::writeData(std::string data){
    if(file->is_open()){
        *file << data << std::endl;
    } else {
        LOG_F(ERROR, "The data file has unexpectedly closed!");
        throw std::runtime_error("The data file has unexpectedly closed!");
    }
}