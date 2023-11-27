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

        std::vector<BlockServer_t> get_servers(const std::string &name, const size_t port) const;

    private:
        Config();
        void Parse(const std::string &file);

        std::string error_msg_;
        BlockMain_t main_;
};

#endif /* LIBS_CONFIG_CONFIG_HPP_ */
