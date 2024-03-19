#pragma once

#include <string>

class chat
{
private:
    unsigned int chat_id;
public:
    chat(std::string& chat_str) : chat_id(std::stoul(chat_str)) {};
    unsigned int id() const { return chat_id; };
};