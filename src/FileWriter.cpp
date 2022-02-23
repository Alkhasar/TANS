#include "../headers/FileWriter.h"
#include "../libs/loguru/loguru.hpp"

FileWriter& FileWriter::getInstance(std::string path = ""){
    // Guarantees Destruction
    static FileWriter instance(path);
    return instance;
};

FileWriter::~FileWriter(){
    LOG_F(INFO, "Destroying file reader!");
    delete file;
};

// Private constructor
FileWriter::FileWriter(std::string path){
    LOG_F(WARNING, "File Writer Instantiated, should appear only once!");
    LOG_F(INFO, "Opening file: %s", path.c_str());
    file = new std::fstream(path, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if(file->is_open() == false) {
		LOG_F(ERROR, "The file at %s does not exist!\n", path.c_str());
		exit(1);
    }
};

// Writing string to file
void FileWriter::writeData(std::string data){
    if(file->is_open()){
        *file << data;
    } else {
        LOG_F(ERROR, "The data file has unexpectedly closed!");
        throw std::runtime_error("The data file has unexpectedly closed!");
    }
}