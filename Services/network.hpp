#pragma once
#include <boost/asio.hpp>

class Network
{
public:
    Network() {};
    void SendGETRequest();
    void SendPOSTRequest();
private:
    void SendRequest(boost::asio::streambuf& request);
};