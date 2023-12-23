#include "../http.hpp"
#include <cstdio>

void Http::Delete(const location_t& location, const std::string url)
{
	std::string path(conf_.GetPath(url));
	
	struct stat fileStat;
	if (stat(path.c_str(), &fileStat) == -1)  {
		if (errno == ENOENT)
			return response_.GenerateError(kNotFound, location.error_page); // 파일이 존재하지 않는 경우
		else
			return response_.GenerateError(kInternalServerError, location.error_page); // 파일 존재 여부 확인 중 오류
	}
	if (access(path.c_str(), R_OK) != 0)
		return response_.GenerateError(kForbidden, location.error_page); // 읽을 수 없는 경우 (권한 없음)
	if (std::remove(path.c_str()) != 0)
		return response_.GenerateError(kInternalServerError, location.error_page); // 파일 삭제 실패
	response_.set_status(kNoContent); // 파일 삭제 성공
	response_.set_done(true);
}
