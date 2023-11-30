#ifndef LIBS_CONFIG_CONFIG_HPP_
#define LIBS_CONFIG_CONFIG_HPP_

#include <string>  /* std::string */
#include <sstream> /* std::stringstream */
#include <fstream> /* std::ifstream */
#include <vector>  /* std::vector */
#include <set>     /* std::set */
#include "block.hpp" /* Block */

class Config
{
    public:
        ~Config();
        explicit Config(const std::string &file);
        Config(const Config &obj);
        Config &operator=(const Config &obj);

        bool IsOpen() const;
        std::string ErrorMsg() const;

		static std::vector<std::string> StringSplit(const std::string &str);
        static bool CheckBrackets(const std::vector<std::string> &tokens);

        http_t GetHttp() const { return main_.http; }
        server_t GetServer(const std::string &host) const;

        std::vector<server_t> GetServers() const { return main_.http.servers; }

    private:
        Config();
        void Parse(const std::string &file);
        void CheckDuplicatedPort();

        std::string error_msg_;
        main_t main_;
};

#endif /* LIBS_CONFIG_CONFIG_HPP_ */
