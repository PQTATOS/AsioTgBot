#include "socket.hpp"

using namespace boost::asio::ip;

namespace utility
{
    socket::socket(boost::asio::io_service&  service, boost::asio::ssl::context& ssl_ctx)
    {
        std::cout << "[Socket] created\n";
        this->base_socket = new boost::asio::ssl::stream<tcp::socket>(service, ssl_ctx);
        this->stream = new boost::asio::streambuf();
        this->bytes_transfered = 0;
        this->response = nullptr;
    }

    socket::~socket()
    {
        std::cout << "[Socket] deleted\n";
        if (!this->base_socket)
            delete this->base_socket;
        if (!this->stream)
            delete this->stream;
        if (!this->response)
            delete this->response;
    }

    socket::socket(socket&& other)
    {
        //std::cout << "[Socket] move\n";
        this->base_socket = other.base_socket;
        other.base_socket = nullptr;

        this->stream = other.stream;
        other.stream = nullptr;

        this->response = other.response;
        other.response = nullptr;

        this->bytes_transfered = other.bytes_transfered;
    }

    socket& socket::operator=(socket&& other)
    {
        //std::cout << "[Socket] asigne move\n";
        if (&other != this)
        {
            delete this->base_socket;
            this->base_socket = other.base_socket;
            other.base_socket = nullptr;

            this->bytes_transfered = other.bytes_transfered;

            delete this->stream;
            this->stream = other.stream;
            other.stream = nullptr;

            if (!this->response)
                delete this->response;
            this->response = other.response;
            other.response = nullptr;
        }
        return *this;
    }

    boost::asio::ip::tcp::socket::lowest_layer_type& socket::get_lowest_layer()
    {
        //std::cout << "[Socket] return lowest layer\n";
        return this->base_socket->lowest_layer();
    }

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket::get_socket()
    {
        //std::cout << "[Socket] return socket\n";
        return *(this->base_socket);
    }

    boost::asio::streambuf& socket::get_streambuf()
    {    
        //std::cout << "[Socket] return stream\n";
        return *(this->stream);
    }

    tg::response& socket::get_response()
    {
        //std::cout << "[Socket] return response. is new:" << !this->response << '\n';
        if (!this->response)
            this->response = new tg::response();
        return *(this->response);
    }

    tg::response* socket::release_response()
    {
        tg::response* tmp = this->response;
        this->response = nullptr;
        return tmp;
    }
}