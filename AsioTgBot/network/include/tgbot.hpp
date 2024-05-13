#pragma once

#include <string>
#include <iostream>

enum class parse_mode
{
    MarkdownV2,
    Markdown,
    HTML
};

class tgbot
{
public:
    tgbot(std::string token, parse_mode mode = parse_mode::HTML);
private:
    std::string bot_token;
    parse_mode p_mode;
};