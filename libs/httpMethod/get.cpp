#include "get.hpp"

// GET 메소드는 주어진 URI를 사용하여 주어진 서버에서 정보를 검색하는 데 사용됩니다. GET을 사용하는 요청은 데이터 검색만 해야 하며 데이터에 다른 영향을 주어서는 안 됩니다.


void	Get(int fd)
{
	std::string	fileContent = getRequestedFile(filePath);

	std::string response;
	if (!fileContent.emtpy())
		response = "HTTP/1.1 200 OK\r\n\r\n" + fileContent;
	else
		response = "HTTP/1.1 404 Not Found\r\n\r\n404 Error: File not found";

}

void	GetResponse()
{}

void	GetRequest()
{}

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
	return fileContent;
}

