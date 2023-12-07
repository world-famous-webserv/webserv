#include "cookie.hpp"

std::string	Expires() { // 쿠키의 만료시간 설정
	std::time_t currentTime = std::time(NULL);
	std::time_t expirationTime = currentTime + 3600; // 1시간(60분 * 60초)
	std::tm*	expireTimeInfo = std::gmtime(&expirationTime);
	
	char expires[100];
	std::strftime(expires, sizeof(expires), "%a, %d-%b-%Y %H:%M:%S GMT", expireTimeInfo);
	return expires;
}

std::string	Domain() { // 쿠키가 전송되는 도메인을 설정 (default: 현재 도메인)
	return "localhost";
}

std::string Path() { // 쿠키가 유효한 경로를 설정 (default: 현재 페이지 경로가 적용)
	return "/";
}

std::string makeCookie(const std::string & key, const std::string & val)
{
	std::string cookie = key + "=" + val + ";";

	bool	expires = true;// 쿠키의 만료시간 설정
	bool	domain = false;// 쿠키가 전송되는 도메인을 설정 (default: 현재 도메인)
	bool	path = false;// 쿠키가 유효한 경로를 설정 (default: 현재 페이지 경로가 적용)
	bool	secure = false; // HTTPS 프로토콜을 통해만 쿠키가 전송되도록 설정
	bool	httpOnly = true; // JavaScript에서 쿠키에 접근할 수 없음

	if (expires)
		cookie += "Expires=" + Expires() + ";";
	if (domain)
		cookie += "Domain=" + Domain() + ";";
	if (path)
		cookie += "Path=" + Path() + ";";
	if (secure)
		cookie += "Secure;";
	if (httpOnly)
		cookie += "HttpOnly;";
	return cookie;
}
