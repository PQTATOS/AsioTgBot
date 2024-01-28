#pragma once
#include <string>
#include <vector>
#include <utility>

#include "bot.hpp"

class Request
{
private:
    std::vector<std::pair<std::string, std::string>> json;
    int body_size;
    std::string body;
    std::string url;

public:
    Request(Bot* bot, const std::string& url);

    const std::string* Url() { return &url; };
    const std::string* Body() { return &body; };
    int BodySize() { return body_size; };
    void AddToBody(std::string& key, std::string& value);
    bool IsEmptyBody();

    void AddUrlParams(std::vector<std::pair<std::string, std::string>>& params);
};