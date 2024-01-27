#pragma once
#include <string>
#include <vector>

enum class ParseMode
{
    MarkdownV2,
    Markdown,
    HTML
};

class Bot
{
private:
    std::string bot_token;
    ParseMode parse_mode;
    int offset = -1;
    int limit = 100;
    int timeout = 60;
    int repoll_time = 120;
public:
    Bot(std::string token, ParseMode parse_m);
    void SetGetUpdateParams(int offset, int limit, int timeout);
    void SetAllowedUpdates(std::vector<std::string>& types);

    int Offset() { return offset; };
    int Timeout() { return timeout; };
    int Limit() { return limit; };
    int RepollTime() { return repoll_time; };
    const std::string Token() { return bot_token; };
    const ParseMode GetParseMode() { return parse_mode; };
};
