#include "Files.h"

#include <fstream>

std::string Files::readFile(std::string filePath)
{
    std::string fileContent;
    std::ifstream fs(filePath.c_str());

    if (!fs.is_open())
    {
        // could not read file
    }

    std::string line;
    while (!fs.eof()) {
        std::getline(fs, line);
        fileContent.append(line + "\n");
    }

    fs.close();
    return fileContent;
}
