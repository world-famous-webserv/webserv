#include "cgi.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Cgi::~Cgi(void)
{
}

Cgi::Cgi(const std::string &cgipass, HttpRequest& req, HttpResponse& res):
	cgipass_(cgipass),
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
	args.push_back(this->path_);

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

std::string Cgi::GetCgiPass(const location_t &location, const std::string &path)
{
	const std::string ext = path.substr(path.find_last_of('.') + 1);
	if (ext == "") {
		std::cerr << "GetCgiPass - Missing extension" << std::endl;
		return "";
	}
	const std::map<std::string, std::string>::const_iterator it = location.fastcgi_pass.find(ext);
	if (it == location.fastcgi_pass.end()) {
		std::cerr << "GetCgipass - Not supproted extension" << std::endl;
		return "";
	}

	const std::string &cgi_pass = it->second;
	return cgi_pass;
}
