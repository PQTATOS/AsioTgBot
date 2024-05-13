#pragma once

#include <string>
#include <iostream>

namespace tg
{
    enum class req_types
    {
        GET,
        POST
    };

    class request
    {
    public:
        request(const req_types type_, const std::string& url_, const std::string& host_);
        std::string to_string() const;
        void add_body(std::string& str);
    private:
        std::string url;
        std::string host;
        req_types type;
        std::string body;
    };
}