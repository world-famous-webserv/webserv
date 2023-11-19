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
	std::string	date = "Mon, 27 Jul 2009 12:28:53 GMT"; // 응답이 생성된 날짜와 시간
	std::string	server = "Apache/2.2.14 (win32)"; // 서버 소프트웨어의 이름과 버전
	std::string lastModified = "Wed, 22 Jul 2009 19:15:56 GMT"; // 요청된 리소스의 마지막 수정시간
	std::string	etag = "34aa387-d-1568eb00"; // 요청한 리소스의 엔터티 태그를 나타냄, 리보스의 버전을 식별하는데 사용됨.
	std::string	vary = "Authorization,Accept"; // 캐싱 정책에 대한 정보
	std::string acceptRanges = "bytes"; // 서버가 리소스의 일부만 전송 할 수 있는지 여부를 나타냄
	std::string	contentLength = "88"; // 본문의길이를 바이트 단위로 나타냄
	std::string contentType = "text/html"; // 응답 본문의 데이터 타입을 정의함
	std::string	connection = "Closed"; // 현재 연결의 상태를 나타냄
}

static void Get(int fd);

void	GetResponse();
void	Request();

std::string getRequestedFile(const std::string& filePath); 

//http 메세지
//startline 파싱 
//emptyline 나올때까지 headers들 파싱

#endif /* GET_HPP */
