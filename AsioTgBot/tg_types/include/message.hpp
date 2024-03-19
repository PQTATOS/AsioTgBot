#pragma once

#include <string>

#include <update.hpp>
#include <chat.hpp>

class message : public update
{
private:
    unsigned int msg_id;
    chat msg_chat;
    std::string msg_text;
public:
    message(std::string& chat_str, std::string text_str);
    ~message() override {};

    update_types get_type() const override;

    unsigned int id() const;
    std::string text() const;
};

