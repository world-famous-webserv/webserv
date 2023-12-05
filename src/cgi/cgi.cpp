#include "cgi.hpp"

Cgi::~Cgi(void) { }

Cgi::Cgi(HttpRequest &request, const location_t &location):
    request_body_(request.body().str())
{
	if (request.header("Auth-Scheme") != "")
		env_["AUTH_TYPE"] = request.header("Authorization");

	env_["CONTENT_LENGTH"] = to_string(request_body_.size());
	env_["CONTENT_TYPE"] = request.header("Content-Type");
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_["PATH_INFO"] = location.cgi_pass;
	env_["PATH_TRANSLATED"] = location.cgi_pass;
	env_["QUERY_STRING"] = request.header("Query");
	env_["REMOTE_HOST"] = request.header("Authorization");
	env_["REMOTE_USER"] = request.header("Authorization");
	env_["REQUEST_METHOD"] = request.method();
	env_["SCRIPT_NAME"] = location.cgi_pass;
	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_SOFTWARE"] = "Webserv/1.0";
}

Cgi::Cgi(const Cgi &obj)
{
	if (this == &obj)
        return;
    env_ = obj.env_;
    request_body_ = obj.request_body_;
	return;
}

Cgi	&Cgi::operator=(const Cgi &obj) {
	if (this == &obj)
        return *this;
    env_ = obj.env_;
    request_body_ = obj.request_body_;
	return *this;
}

std::string Cgi::run(std::string script)
{
	char **env;
	try {
        env = new char*[env_.size() + 1];
        env[env_.size()] = NULL;
        int	i = 0;
        for (std::map<std::string, std::string>::const_iterator it = env_.begin(); it != env_.end(); ++it) {
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

	std::cout << "request_body_: " << request_body_ << std::endl;

    fwrite(request_body_.c_str(), 1, request_body_.size(), fin);
    fseek(fin, 0, SEEK_SET);

	const pid_t pid = fork();
    std::ostringstream oss;
	if (pid == -1) {
		std::cerr << "Cgi fork failed." << '\n';
		return "500";
	} else if (pid == 0) {
		static const char *python = "/Library/Frameworks/Python.framework/Versions/3.11/bin/python3";
		const char *argv[3] = {python, (char *)script.c_str(), NULL};
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
		std::string line; std::cin >> line;
		std::cout << "test: " << line << std::endl;
		execve(python, (char **)argv, env);
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

std::string Cgi::to_string(const int i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
}
