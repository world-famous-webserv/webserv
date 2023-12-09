#include "cgi.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Cgi::~Cgi(void)
{
}

Cgi::Cgi(const Conf &conf, const std::string &url, HttpRequest& req, HttpResponse& res):
	conf_(conf),
	url_(url),
	request_(req),
	response_(res)
{
	this->Open();
}

/* ************************************************************************** */
// Utility
/* ************************************************************************** */

void Cgi::Child(void)
{
	// args
	std::vector<std::string> args;
	args.push_back(this->cgipass_);
	args.push_back(this->cgifile_);

	std::vector<char*> args_p;
	for (std::size_t i = 0; i < args.size(); ++i)
		args_p.push_back(const_cast<char*>(args[i].c_str()));
	args_p.push_back(NULL);

	// environment
	std::vector<std::string> envp;
	if (request_.header("Auth-Scheme") != "")
		envp.push_back("AUTH_TYPE=" + request_.header("Authorization"));
	std::stringstream len;
	len << request_.body().str().length();
	envp.push_back("CONTENT_LENGTH=" + len.str());
	envp.push_back("CONTENT_TYPE=" + request_.header("Content-Type"));
	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envp.push_back("QUERY_STRING=" + request_.header("Query"));
	envp.push_back("REMOTE_HOST=" + request_.header("Authorization"));
	envp.push_back("REMOTE_USER=" + request_.header("Authorization"));
	envp.push_back("REQUEST_METHOD=" + request_.method());
	envp.push_back("SERVER_PROTOCOL=" + request_.version());
	envp.push_back("SERVER_SOFTWARE=Webserv/1.0");

	std::vector<char*> envp_p;
	for (std::size_t i = 0; i < envp.size(); ++i)
		envp_p.push_back(const_cast<char*>(envp[i].c_str()));
	envp_p.push_back(NULL);

	execve(args[0].c_str(), &args_p[0], &envp_p[0]);
	std::exit(EXIT_FAILURE);
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void Cgi::Open()
{
	std::cout << "Cgi::Open" << std::endl;

	const int location_idx = conf_.GetLocationIdx(url_);
	if (location_idx == -1) {
		std::cerr << "Cgi Open - Invalid location" << std::endl;
		response_.set_status(kNotFound);
		return;
	}

	const std::string path = conf_.GetPath(url_);
	if (path == "") {
		std::cerr << "Cgi Open - Invalid path" << std::endl;
		response_.set_status(kNotFound);
		return;
	}

	const std::string ext = conf_.GetExt(url_);
	if (ext == "") {
		std::cerr << "Cgi Open - Extension Missing" << std::endl;
		response_.set_status(kNotFound);
		return;
	}

	const location_t &location = conf_.GetLocation(location_idx);
	const std::map<std::string, std::string>::const_iterator it = location.fastcgi_pass.find(ext);
	if (it == location.fastcgi_pass.end()) {
		std::cerr << "Cgi Open - Not supproted extension" << std::endl;
		response_.set_status(kNotFound);
		return;
	}

	// set cgi pass
	cgipass_ = it->second;
	cgifile_ = path;

	int fd[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
		std::cerr << "- socketpair error: " << strerror(errno) << std::endl;
		response_.set_status(kInternalServerError);
		return;
	}
	pid_ = fork();
	if (pid_ == -1) {
		std::cerr << "- fork erorr: " << strerror(errno) << std::endl;
		response_.set_status(kInternalServerError);
		close(fd[0]);
		close(fd[1]);
		return;
	}
	if (pid_ == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		this->Child();
		std::exit(EXIT_FAILURE);
	}
	else
	{
		identifier_ = fd[0];
		close(fd[1]);
	}
}

void Cgi::Read(void)
{
	if (identifier_ == -1)
		return ;
	char buf[1024] = {0};
	ssize_t nbytes = recv(identifier_, buf, sizeof(buf), 0);
	if (nbytes < 0) {
		response_.set_status(kInternalServerError);
		return this->Broken(errno);
	}
	if (nbytes == 0) return this->Update();
	// put
	response_.body().write(buf, nbytes);
}

void Cgi::Write(void)
{
	if (identifier_ == -1)
		return;
}

void Cgi::Update(void)
{
	if (identifier_ == -1)
		return;
	int status;
	if (waitpid(pid_, &status, WNOHANG) != 0)
	{
		if (WIFEXITED(status))
			response_.set_done(true);
		else
			response_.set_status(kInternalServerError);
		this->Close();
	}
}
