#ifndef LIBS_CONFIG_CONFIG_HPP_
#define LIBS_CONFIG_CONFIG_HPP_

#include <string>  /* std::string */
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

        bool is_open() const;
        std::string error_msg() const;

        BlockMain_t &main() { return main_; }
        const BlockMain_t &main() const { return main_; }

        BlockHttp_t &http() { return main_.http_; }
        const BlockHttp_t &http() const { return main_.http_; }

        std::vector<BlockServer_t> &servers() { return main_.http_.servers_; }
        const std::vector<BlockServer_t> &servers() const { return main_.http_.servers_; }

        BlockServer_t &server() { return main_.http_.servers_[0]; }
        const BlockServer_t &server() const { return main_.http_.servers_[0]; }

        std::vector<BlockLocation_t> &locations() { return server().locations_; }
        const std::vector<BlockLocation_t> &locations() const { return server().locations_; }

    private:
        Config();
        void Parse(const std::string &file);

        std::string error_msg_;
        BlockMain_t main_;
};

#endif /* LIBS_CONFIG_CONFIG_HPP_ */
