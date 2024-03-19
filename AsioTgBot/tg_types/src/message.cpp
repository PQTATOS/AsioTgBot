#include "message.hpp"

message::message(std::string& chat_str, std::string text_str)
    : msg_chat(chat(chat_str))
{
    this->msg_id = 1;
    msg_text = text_str;
};

update_types message::get_type() const
{
    return update_types::Message;
};

unsigned int message::id() const
{
    return this->msg_id;
};

std::string message::text() const
{
    return this->msg_text;
};