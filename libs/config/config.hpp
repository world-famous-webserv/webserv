#ifndef LIBS_CONFIG_CONFIG_HPP_
#define LIBS_CONFIG_CONFIG_HPP_

#include <string>  /* std::string */
#include <sstream> /* std::stringstream */
#include <fstream> /* std::ifstream */
#include <vector>  /* std::vector */
#include "block.hpp" /* Block */
#include "utils.hpp" /* res_t */

class Config
{
    public:
        ~Config();
        explicit Config(const std::string &file);
        Config(const Config &obj);
        Config &operator=(const Config &obj);

        bool IsOpen() const;
        std::string ErrorMsg() const;

        BlockHttp_t GetHttp() const { return main_.http; }
        BlockServer_t GetServer(const std::string &host) const;

        std::vector<BlockServer_t> GetServers() const { return main_.http.servers; }

    private:
        Config();
        void Parse(const std::string &file);

        std::string error_msg_;
        BlockMain_t main_;
};

#endif /* LIBS_CONFIG_CONFIG_HPP_ */
