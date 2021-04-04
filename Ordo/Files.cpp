#include "Files.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string Files::readFile(std::string filePath)
{
    std::string fileContent;
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fs.open(filePath);
        std::stringstream stream;
        stream << fs.rdbuf();
        fileContent = stream.str();
        fs.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR: File " << filePath << "could not be read";        
    }

    return fileContent;
}
