#include "config.hpp"

Config::~Config() { }

Config::Config(const std::string &file): error_msg_("")
{
    Parse(file);
}

Config::Config(const Config &obj)
{
    *this = obj;
}

Config &Config::operator=(const Config &obj)
{
    if (this == &obj)
        return *this;
    error_msg_ = obj.error_msg_;
    main_ = obj.main_;
    return *this;
}

void Config::Parse(const std::string &file)
{
    std::ifstream in(file.c_str(), std::ios_base::in);
	if (in.is_open() == false) {
        error_msg_ = "Config: File not found.";
        return;
    }
	std::string read;
    for (std::string line; std::getline(in, line);) {
        if (line.size())
            read += line + "\n";
    }
	in.close();
    const std::vector<std::string> tokens = Utils::stringSplit(read);

    if (Utils::CheckBrackets(tokens) == false) {
        error_msg_ = "Config: Brackets are not balanced.";
        return;
    }

    // for (std::vector<std::string>::size_type i = 0, end = tokens.size(); i < end; ++i)
    //     std::cout << tokens[i] << " ";

    size_t idx = 0;
    main_ = Block::ParseMain(tokens, idx, error_msg_);
}

bool Config::is_open() const
{
    return error_msg_.empty();
}

std::string Config::error_msg() const
{
    return error_msg_;
}
