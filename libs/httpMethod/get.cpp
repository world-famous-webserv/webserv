#include "get.hpp"

void	Get(int fd)
{}

void	GetResponse()
{}

void	GetRequest()

std::string getRequestedFile(const std::string& filePath)
{
    std::ifstream fileStream("." + filePath);
    std::stringstream fileContentStream;
    std::string fileContent;

    if (fileStream.is_open()) {
        fileContentStream << fileStream.rdbuf();
        fileContent = fileContentStream.str();
        fileStream.close();
    }	
}

