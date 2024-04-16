#pragma once

#include <boost/asio.hpp>

#include <string>
#include <unordered_map>

namespace tg
{
    class response
    {
    public:
        boost::asio::streambuf raw_responce;
        unsigned int status_code;
        std::string status_msg;

        std::unordered_map<std::string, std::string> headers;
        std::string body;
        size_t body_size;
    };
}