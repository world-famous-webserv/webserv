#include "cgi.hpp"

/* ************************************************************************** */
// Orthodox Canonical Form
/* ************************************************************************** */

Cgi::~Cgi(void)
{
	std::cerr << "Cgi::Close - " << request_.method() << "(" << pid_ << ")" <<std::endl;
	int status = 0;
	int ret = waitpid(pid_, &status, WNOHANG);
	if (pid_ != -1 && ret == 0) {
		kill(pid_, SIGKILL);
		ret = waitpid(pid_, &status, 0);
		pid_ = -1;
	}
}

Cgi::Cgi(const location_t& location, HttpRequest& req, HttpResponse& res):
	location_(location),
	request_(req),
	response_(res),
	step_(kParseHeader)
{
}

/* ************************************************************************** */
// Utility
/* ************************************************************************** */

std::string Cgi::GetCgi(const location_t& location, const std::string path)
{
	std::string::size_type idx = path.rfind('.');
	if (idx == std::string::npos)
		return "";
	std::string extension = path.substr(idx + 1);
	std::map<std::string, std::string>::const_iterator it = location.fastcgi_pass.find(extension);
	if (it == location.fastcgi_pass.end())
		return "";
	return it->second;
}

void Cgi::Child(void)
{
	const std::string url = request_.uri();
    const std::string file = location_.root + url.substr(location_.name.length(), url.length() - location_.name.length());
	const std::string program = GetCgi(location_, file);
	std::cerr << "program = " << program << std::endl;
	std::cerr << "file = " << file << std::endl;

	if (chdir(location_.root.c_str()) == -1)
		std::exit(1);
	// args
	std::vector<std::string> args;
	args.push_back(program);

	std::vector<char*> args_p;
	for (std::size_t i = 0; i < args.size(); ++i)
		args_p.push_back(const_cast<char*>(args[i].c_str()));
	args_p.push_back(NULL);

	// environment
	std::vector<std::string> envp;
	envp.push_back("PATH_INFO=" + file);
	if (request_.header("Auth-Scheme") != "")
		envp.push_back("AUTH_TYPE=" + request_.header("Authorization"));
	std::stringstream len;
	len << request_.body().str().length();
	envp.push_back("CONTENT_LENGTH=" + len.str());
	envp.push_back("CONTENT_TYPE=" + request_.header("Content-Type"));
	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (request_.header("Query").empty() == false)
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

std::cerr << "argx[x] " << args[0] << std::endl;
	for (std::size_t i = 0; i < envp.size(); ++i)
std::cerr << "envp[" << i << "] " << envp[i] << std::endl;

std::cerr << "execute" << std::endl;
	execve(args[0].c_str(), &args_p[0], &envp_p[0]);
std::cerr << "execute fail" << std::endl;
	std::exit(EXIT_FAILURE);
}

/* ************************************************************************** */
// Parse
/* ************************************************************************** */

static std::string Trim(const std::string &str)
{
	std::string trim(str);
	trim.erase(0, trim.find_first_not_of(" \t\n\r\f\v"));
	trim.erase(trim.find_last_not_of(" \t\n\r\f\v") + 1);
	return trim;
}

const std::string Cgi::header(const std::string& key) const
{
	std::string lower(Trim(key));
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	std::map<std::string, std::string>::const_iterator it = headers_.find(lower);
	return it != headers_.end()? it->second : "";
}

void Cgi::add_header(const std::string &key, const std::string &val)
{
	std::string lower(Trim(key));
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	headers_[lower] = Trim(val);
}

bool Cgi::GetLine(std::stringstream& in, std::string& line)
{
	in.clear();
	while (in.good())
	{
		char ch = in.get();
		if (!in.good())
			return false;
		if (ch != '\n')
		{
			buf_.put(ch);
			continue;
		}
		std::getline(buf_, line);
		if (line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
		buf_.str();
		buf_.clear();
		return true;
	}
	return false;
}

void Cgi::ParseHeader(const std::string& line)
{
	if (line.empty())
	{
		step_ = kParseBody;
		return;
	}
	std::istringstream iss(line);
	std::string key, val;
	std::getline(iss, key, ':');
	std::getline(iss, val);

	this->add_header(key, val);
	std::cerr << "CGI Header: " << key << " = " << val << std::endl;
}

void Cgi::ParseBody(void)
{
	char buf[1024 * 1024];

	analyzer_.clear();
	response_.body().clear();
	while (analyzer_.read(buf, sizeof(buf)) || analyzer_.gcount() > 0)
		response_.body().write(buf, analyzer_.gcount());
//std::cerr << "move total: " << response_.body().tellp() << std::endl;
	analyzer_.clear();
	response_.body().clear();
}

void Cgi::ParseDone(void)
{
	std::map<std::string, std::string>::iterator it;
	it = headers_.find("status");
	if (it != headers_.end()) {
		int status = kOk;
		std::istringstream iss(it->second);
		iss >> status;
		response_.set_status(static_cast<HttpStatus>(status));
		headers_.erase(it);
	}
	for (it = headers_.begin(); it != headers_.end(); ++it)
		response_.add_header(it->first, it->second);
	response_.set_done(true);
	this->Close();
}

/* ************************************************************************** */
// Main
/* ************************************************************************** */

void Cgi::Open()
{
	int fd[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1) {
		std::cerr << "- socketpair error: " << strerror(errno) << std::endl;
		response_.set_status(kInternalServerError);
		return;
	}
	fcntl(fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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
	}
	else
	{
		identifier_ = fd[0];
		close(fd[1]);
	}
	std::cerr << "Cgi::Open - " << request_.method() << "(" << pid_ << ")" <<std::endl;
}

void Cgi::Read(void)
{
	if (identifier_ == -1)
		return ;
	char buf[1024 * 1024] = {0};
	ssize_t nbytes = recv(identifier_, buf, sizeof(buf), MSG_DONTWAIT);
	if (nbytes < 0) {
		response_.set_status(kInternalServerError);
		return this->Broken(errno);
	}
	if (nbytes == 0) return this->ParseDone();
	// put
	analyzer_.clear();
	analyzer_.write(buf, nbytes);
//	std::cerr << "read total: " << analyzer_.tellp() << std::endl;
}

void Cgi::Write(void)
{
	if (identifier_ == -1)
		return;
	if (request_.body().tellp() == 0) {
		shutdown(identifier_, SHUT_WR);	// not allow function, but need half-close
		return;
	}
	std::streampos curr = request_.body().tellg();
	// get
	char buf[1024 * 1024] = {0};
	request_.body().read(buf, sizeof(buf));
	if (request_.body().gcount() <= 0) return;
	// send
	ssize_t nbytes = send(identifier_, buf, request_.body().gcount(), MSG_DONTWAIT);
	if (nbytes < 0) return this->ParseDone();
	if (nbytes > 0) {
		request_.body().clear();
		request_.body().seekg(curr);
		request_.body().seekg(nbytes, std::ios::cur);
//	std::cerr << "write total: " << request_.body().tellg() << std::endl;
		return;
	}
	request_.body().clear();
	request_.body().seekg(curr);
}

void Cgi::Update(void)
{
	analyzer_.clear();
	std::string line;
	if (step_ == kParseHeader && this->GetLine(analyzer_, line) == true)
		this->ParseHeader(line);
	else if (step_ == kParseBody)
		this->ParseBody();
	else if (step_ == kParseDone)
		this->ParseDone();
}
