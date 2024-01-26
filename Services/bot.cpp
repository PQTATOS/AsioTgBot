#include "bot.hpp"

Bot::Bot(std::string token, ParseMode parse_m)
{
    bot_token = token;
    parse_mode = parse_m;
}

void Bot::StartPolling()
{

}