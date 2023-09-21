#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "Listener.hpp"

// 클래스 구조 변경 --> 깔끔하게 기능별로 
// io_multflexing은 지우고, 나중에;
// src/main에서 불러와서 사용할 예정임. 여기있는 메인은 지우기

int main(int argc, char** argv) {
	if (argc < 2 || argc > 4) {
		printf("usage: server <port> [<ip>]\n");
		exit(1);
	}

	fd_set	readfds;
	int 	clientSockets[MAX_CLIENTS] = {0, }; // 클라이언트 소켓 배열

	TCPStream* stream = NULL;
	Listener* acceptor = NULL;
	if (argc == 3)
		acceptor = new Listener(atoi(argv[1]), argv[2]);
	else 
		acceptor = new Listener(atoi(argv[1]));

	if (acceptor->start() == 0) {
		while (1) {
			stream = acceptor->accept();
			if (stream != NULL) {
				size_t len;
				char line[256];
				while ((len = stream->receive(line, sizeof(line))) > 0) { // len < 0 이면 오류
					line[len] = '\0';
					std::cout << " rcv_len - "<< strlen(line) << " received - \"" << line << "\"" << std::endl;
					stream->send(line, len);
				}
				delete stream;
			}
		}
	}
	perror("Could not start the server");
	exit(-1);
}


// class 에 i/o_muliflexing 관련 함수들 넣어놓고 운영체제에 따라 다르게 처리
// class 생성과 함수 사용시 그에 맞는 함수호출
