#include "utils.hpp"

const char *Utils::WhiteSpaces = " \f\n\r\t\v";

Utils::~Utils() { }
Utils::Utils() { }
Utils::Utils(const Utils &obj) { *this = obj; }
Utils& Utils::operator=(const Utils &obj)
{
    if (this == &obj)
        return *this;
    return *this;
}

std::vector<std::string> Utils::stringSplit(const std::string &str, const std::string &whitespace)
{
	std::vector<std::string> tokens;
	std::string::size_type start, end = 0;
	while (true) {
		start = str.find_first_not_of(whitespace, end);
		if (start == std::string::npos)
			break ;
		end = str.find_first_of(whitespace, start);
		if (end == std::string::npos) {
			tokens.push_back(str.substr(start));
			break ;
		}
        int s = start, e = end;
        while (s < e) {
            if (str[s] == '{')
                tokens.push_back("{");
            else if (str[s] == '}')
                tokens.push_back("}");
            else if (str[s] == ';')
                tokens.push_back(";");
            else
                break;
            ++s;
        }
        std::vector<std::string> tmp;
        while (s < e) {
            if (str[e - 1] == '{')
                tmp.push_back("{");
            else if (str[e - 1] == '}')
                tmp.push_back("}");
            else if (str[e - 1] == ';')
                tmp.push_back(";");
            else
                break;
            --e;
        }
        if (e > s)
            tokens.push_back(str.substr(s, e - s));
        for (std::vector<std::string>::reverse_iterator it = tmp.rbegin(); it != tmp.rend(); ++it)
            tokens.push_back(*it);
	}
	return tokens;
}

bool Utils::CheckBrackets(const std::vector<std::string> &tokens)
{
    int brackets = 0;
    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "{")
            ++brackets;
        else if (*it == "}")
            --brackets;
        if (brackets < 0)
            return false;
    }
    return brackets == 0;
}

int Utils::StoiTime(const std::string &str)
{
    int time = 0;
    size_t i = 0;
    while (i < str.size() && std::isdigit(str[i]))
        time = time * 10 + str[i++] - '0';
    if (i == str.size())
        return time;
    if (str[i] == 's')
        return time;
    if (str[i] == 'm')
        return time * 60;
    if (str[i] == 'h')
        return time * 60 * 60;
    if (str[i] == 'd')
        return time * 60 * 60 * 24;
    return -1;
}
