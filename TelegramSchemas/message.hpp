#include "user.hpp"
#include "chat.hpp"

class Message
{
private:
    int id;
    int thread_id;

    User* from;
    Chat* sender_chat;
    Chat* chat;
    unsigned int date;

}

class MessageReactionUpdated
{

}

class MessageReactionCountUpdated
{

}