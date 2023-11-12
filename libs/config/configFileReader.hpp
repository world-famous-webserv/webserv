#ifndef LIBS_CONFIG_CONFIGFILEREADER_HPP_
#define LIBS_CONFIG_CONFIGFILEREADER_HPP_

#include <string>
#include <vector>
#include <fstream>

class ConfigFileReader
{
	public:
		~ConfigFileReader(void);
		ConfigFileReader(void);
		ConfigFileReader(const ConfigFileReader &obj);
		ConfigFileReader &operator=(const ConfigFileReader &obj);

		static std::string read(const std::string &fileName);

		class FileNotFoundException: public std::exception
		{
			virtual const char *what() const throw();
		};
};

#endif /* LIBS_CONFIG_CONFIGFILEREADER_HPP_ */
