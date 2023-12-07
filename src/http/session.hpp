#ifndef SRC_HTTP_SESSION_HPP_
#define SRC_HTTP_SESSION_HPP_

#include "http_response.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#define SESSION_TIMEOUT 1800 // 30분

struct t_session {
	std::string sessionId;
	time_t expiryTime;
};

class Session {
	public:
		static Session & GetInstance(void);
		std::string createSession();
		bool isSessionValid(const std::string & sessionId);
	private:
		std::map<std::string, t_session> sessionMap;
};

#endif /* SRC_HTTP_SESSION_HPP_ */
