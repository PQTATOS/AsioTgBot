#pragma once

#include <iostream>
#include <queue>

#include "request.hpp"
#include "response.hpp"
#include "tgbot.hpp"
#include "poller.hpp"
#include "message.hpp"
#include "update.hpp"

class dispatcher
{
public:
    dispatcher();
    ~dispatcher();
    void add_bot(const tgbot *bot_);
    void start();
private:
    const tgbot *bot;
    poller *net_poller;

    std::queue<update*> updates_to_process;

    void get_updates();
    void send_messages();
    void process_responces();
    update* convert_str(std::string str);
};