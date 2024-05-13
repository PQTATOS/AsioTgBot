#include "tgbot.hpp"

tgbot::tgbot(std::string token, parse_mode mode) 
    : bot_token(token), p_mode(mode) 
{
    std::cout << "Bot created\n";
};

