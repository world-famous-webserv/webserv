#include "../http.hpp"
#include <cstdio>

HttpStatus Http::Delete(const std::string path)
{
	struct stat fileStat;
	
	if (stat(path.c_str(), &fileStat) == 0) {
		if (access(path.c_str(), R_OK) != 0)
			return kForbidden; // 읽을 수 없는 경우 (권한 없음)
		if (std::remove(path.c_str()) != 0)
			return kInternalServerError; // 파일 삭제 실패
		return kNoContent; // 파일 삭제 성공
	}
	else {
		if (errno == ENOENT)
			return kNotFound; // 파일이 존재하지 않는 경우
		else
			return kInternalServerError; // 파일 존재 여부 확인 중 오류
	}
}
