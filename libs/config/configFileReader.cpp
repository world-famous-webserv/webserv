#include "configFileReader.hpp"

ConfigFileReader::~ConfigFileReader(void) { }
ConfigFileReader::ConfigFileReader(void) { }
ConfigFileReader::ConfigFileReader(const ConfigFileReader &obj) { *this = obj; }
ConfigFileReader &ConfigFileReader::operator=(const ConfigFileReader &obj)
{
	if (this == &obj)
		return *this;
	return *this;
}

std::string ConfigFileReader::read(const std::string &fileName)
{
    std::ifstream in(fileName.c_str(), std::ios_base::in);
	if (in.is_open() == false)
        throw FileNotFoundException();
	std::string ret, line;
	while (std::getline(in, line))
		ret += " " + line;
	return ret;
}

const char *ConfigFileReader::FileNotFoundException::what() const throw()
{
	return "File not found.";
}
