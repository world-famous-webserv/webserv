#ifndef GET_HPP
# define GET_HPP
# include <string>
# include <fstream>

/*
GET /hello.html HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive
*/

// map의  key : value 구조로 추가하면 될 듯.
typedef struct t_request
{
	std::string	method = "GET"; // 요청의 종류
	std::string	url = "/hello.html" // 요청된 자원의 위치
	std::string	httpVersion = "HTTP/1.1"; // HTTP 버전
	std::string userAgent: "Mozilla/4.0 (compatible; MSIES.01; Windows NT)"; // 요청을 보내는 클라이언트의 정보
	std::string host = "www.tutorialspoint.com"; // 요청이 전송되는 호스트
	std::string acceptLanguage = "en-us"; // 클라이언트가 지원하는 언어
	std::string acceptEncoding = "gzip, deflate"; // 클라이언트가 지원하는 인코딩 방식
	std::string connection = "Keep-Alive"; // 클라이언트와 서버 간의 연결 유지 여부
}

/*
HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
ETag: "34aa387-d-1568eb00"
Vary: Authorization,Accept
Accept-Ranges: bytes
Content-Length: 88
Content-Type: text/html
Connection: Closed
*/

typedef struct t_response
{
	std::string	httpVersion = "HTTP/1.1";
	std::string	date = "Mon, 27 Jul 2009 12:28:53 GMT"; // 생성시간
	std::string	server = "Apache/2.2.14 (win32)";
	std::string lastModified = "Wed, 22 Jul 2009 19:15:56 GMT"; // 마지막 수정시간
	std::string	etag = "34aa387-d-1568eb00";
	std::string	vary = "Authorization,Accept";
	std::string acceptRanges = "bytes";
	std::string	contentLength = "88";
	std::string contentType = "text/html";
	std::string	connection = "Closed";
}

static void Get(int fd);

void	GetResponse();
void	Request();

std::string getRequestedFile(const std::string& filePath); 

http 메세지
startline 파싱 
emptyline 나올때까지 headers들 파싱

#endif /* GET_HPP */
