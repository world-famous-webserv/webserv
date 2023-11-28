#ifndef LIBS_INDEX_HPP
# define LIBS_INDEX_HPP

# include <string>
# include <dirent.h>
# include <fstream>
# include <iostream>
# include "http_code.hpp"
# include <unistd.h>

bool hasAccess(const char *fileName, int mode) {
	return (access(fileName, mode) == 0); // 파일에 대한 접근 권한을 확인하여 반환
}

bool	index(){
	std::string path = "index.html"; // << config 에서  index.html 이 "index.htm" "index.ht" "index.h" 로 루트된 경우
	std::string paths[4] = {"/Users/seojo/Desktop/webserv/index.htm", 
							"/Users/seojo/Desktop/webserv/index.ht", 
							"/Users/seojo/Desktop/webserv/index.h"};

	int	i = 0;
	while(!paths[i].empty()) {
		if (hasAccess(paths[i].c_str(), F_OK) == 0){ // 파일 존재여부 확인.
			++i;
			continue;
		}
		else{
			std::fstream file(paths[i]);
			if (file.is_open()) {
				std::string line;
				while (std::getline(file, line)) //access // stat
					BODY += line + "\n";  // << body 에 입력
				http_status = kOk;  // <<< status 변경
				file.close();
				return EXIT_SUCCESS;
			}
			else{
				if (hasAccess(paths[i].c_str(), R_OK) == 0) // 파일 읽기권한 확인
					http_status = kForbidden; // <<< status 변경
				return EXIT_FAILURE;
			}
		}
		return EXIT_FAILURE;
	}
}

void	autoindex()
{
	if (!autoindex) {
		http_status = kForbidden; // <<< status 변경
		return ;
	}
	std::string	path = "index/";
}

#endif /* LIBS_INDEX_HPP */