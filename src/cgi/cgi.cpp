#include "cgi.hpp"

std::string Cgi::to_string(const int i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

std::string Cgi::run(HttpRequest &request, const location_t &location, const std::string &script)
{
	std::map<std::string, std::string> env_map;
	if (request.header("Auth-Scheme") != "")
		env_map["AUTH_TYPE"] = request.header("Authorization");
	env_map["CONTENT_LENGTH"] = to_string(request.body().str().size());
	env_map["CONTENT_TYPE"] = request.header("Content-Type");
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["QUERY_STRING"] = request.header("Query");
	env_map["REMOTE_HOST"] = request.header("Authorization");
	env_map["REMOTE_USER"] = request.header("Authorization");
	env_map["REQUEST_METHOD"] = request.method();
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["SERVER_SOFTWARE"] = "Webserv/1.0";

	char **env;
	try {
        env = new char*[env_map.size() + 1];
        env[env_map.size()] = NULL;
        int	i = 0;
        for (std::map<std::string, std::string>::const_iterator it = env_map.begin(); it != env_map.end(); ++it) {
            const std::string element = it->first + "=" + it->second;
            env[i] = new char[element.size() + 1];
            env[i] = strcpy(env[i], element.c_str());
            ++i;
        }
	}
	catch (const std::bad_alloc &e) {
		std::cerr << e.what() << std::endl;
        return "";
    }

	const int dup_stdin = dup(STDIN_FILENO);
	const int dup_stdout = dup(STDOUT_FILENO);

	FILE *const fin = tmpfile();
	FILE *const fout = tmpfile();
	const int in = fileno(fin);
	const int out = fileno(fout);

    fwrite(request.body().str().c_str(), 1, request.body().str().size(), fin);
    fseek(fin, 0, SEEK_SET);

	const pid_t pid = fork();
    std::ostringstream oss;
	if (pid == -1) {
		std::cerr << "Cgi fork failed." << '\n';
		return "500";
	} else if (pid == 0) {
		const std::string ext = script.substr(script.find_last_of('.') + 1);
		const std::map<std::string, std::string>::const_iterator it = location.fastcgi_param.find(ext);

		if (it == location.fastcgi_param.end()) {
			std::cerr << "Cgi: extension not found." << '\n';
			return "501";
		}
		const char *const ext_path = it->second.c_str();
		const char *const script_path = script.c_str();
		const char *const argv[3] = {ext_path, script_path, NULL};
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
		execve(ext_path, (char *const *)argv, env);
		std::cerr << "Cgi execve failed." << '\n';
		write(STDOUT_FILENO, "500", 3);
	} else {
		char buffer[1 << 16] = {0};

		waitpid(-1, NULL, 0);
		fseek(fout, 0, SEEK_SET);

        int ret = 0;
		while (feof(fout) == 0 && (ret = fread(buffer, 1, (1 << 16) - 1, fout)) > 0) {
            buffer[ret] = '\0';
            oss << buffer;
		}
	}

	dup2(dup_stdin, STDIN_FILENO);
	dup2(dup_stdout, STDOUT_FILENO);
	fclose(fin);
	fclose(fout);
	close(in);
	close(out);
	close(dup_stdin);
	close(dup_stdout);

	for (size_t i = 0; env[i]; ++i)
		delete[] env[i];
	delete[] env;

	return oss.str();
}
