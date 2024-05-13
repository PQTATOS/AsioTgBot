#pragma once

#include <queue>
#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "request.hpp"
#include "response.hpp"
#include "socket.hpp"

class poller
{
public:
    poller();
    ~poller();
    void run();

    void add_request(const tg::request* req);
    tg::response* get_responce();
    bool is_no_requests();
    bool is_no_responce();
private:
    boost::asio::io_service service;
    boost::asio::ssl::context *ssl_ctx;
    std::vector<utility::socket> opened_sockets;

    std::queue<const tg::request*> requests_to_process;
    std::queue<tg::response*> responces_recived;

    void handle_connect(const boost::system::error_code &e, size_t sock_id);
    void handle_handshake(const boost::system::error_code &e, size_t sock_id);
    void send(const boost::system::error_code &e, size_t sock_id);
    void recive_status(const boost::system::error_code &e, size_t sock_id);
    void recive_headers(const boost::system::error_code &e, size_t sock_id);
    void recive_body(const boost::system::error_code &e, size_t sock_id);
    void handle_body(const boost::system::error_code &e, std::size_t bytes_transferred, size_t sock_id);
}; 