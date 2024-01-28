#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <vector>

#include "bot.hpp"
#include "TelegramSchemas/update.hpp"
#include "request.hpp"

class Network
{
public:
    Network() {};
    void ConnectBot(Bot* bot);
    void StartPolling();
private:
    void SendRequest(boost::asio::streambuf& request);
    void SendGETRequest(Request* req);
    void SendPOSTRequest();
    std::vector<Update*>* MakeUpdateRequest(Bot* bot);

    std::vector<Bot*> bots;
    boost::asio::io_service service;
    boost::asio::ssl::context ssl_ctx{boost::asio::ssl::context::sslv23_client};
};