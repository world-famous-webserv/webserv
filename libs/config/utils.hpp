#ifndef LIBS_CONFIG_UTILS_HPP_
# define LIBS_CONFIG_UTILS_HPP_

# include <string>
# include <vector>
# include <iostream>

class Utils
{
    public:
		static std::vector<std::string> stringSplit(const std::string &str, const std::string &whitespace=WhiteSpaces);
        static bool CheckBrackets(const std::vector<std::string> &tokens);
        static int Stoi(const std::string &str);
    private:
        static const char *WhiteSpaces;

        ~Utils();
        Utils();
        Utils(const Utils &obj);
        Utils& operator=(const Utils &obj);
};

#endif /* LIBS_CONFIG_UTILS_HPP_ */
