#include "bot.hpp"
#include "network.hpp"

Bot::Bot(std::string token, ParseMode parse_m)
{
    bot_token = token;
    parse_mode = parse_m;
}

void Bot::SetGetUpdateParams(int _offset, int _limit, int _timeout)
{
    offset = _offset;
    limit = _limit;
    timeout = _timeout;
}