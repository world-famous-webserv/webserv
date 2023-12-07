#include "session.hpp"

Session & Session::GetInstance(void) {
	static Session instance;
	return instance;
}

std::string makeSessionId() {
	std::ostringstream sessionIdStream;
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	for (int i = 0; i < 16; ++i)
		sessionIdStream << std::rand() % 10;
	return sessionIdStream.str();
}

std::string	Session::createSession() {
	std::string sessionId = makeSessionId();
	while (sessionMap.find(sessionId) != sessionMap.end())
		sessionId = makeSessionId();
	time_t now = time(NULL);
	t_session newSession = {sessionId, now + SESSION_TIMEOUT}; // 만료시간 지정
	sessionMap[sessionId] = newSession;
	return sessionId;
}

bool	Session::isSessionValid(const std::string & sessionId) {
	time_t now = time(NULL);
	if (sessionMap.find(sessionId) != sessionMap.end()) {
		if (sessionMap[sessionId].expiryTime > now)
			return true;
		else
			sessionMap.erase(sessionId);
	}
	return false;
}
